#include "stdafx.h"
#include "ParticleDO.h"
#include "USVDoc.h"
#include "USVView.h"
#include "Particle.h"
#include "math.h"
#include "float.h"
#include "SetStaticObstacleRectangle.h"
#include "SetStaticObstacleCircle.h"

float ParticleDO::Vmax = 50;  //通常设置Vmax=Xmax-Xmin;    
float ParticleDO::Vmin = 0;
float ParticleDO::c1 = 2.0f;
float ParticleDO::c2 = 2.0f;
int ParticleDO::BasicP_num = 0;

extern StartGoalPOINT startgoalp;
extern DynamicOC dynamicoc[20];
extern PSO_DO pso_do;//粒子群参数
extern PSO pso;//粒子群参数
extern DOBSTICLE DObsticle_num[20];//动态障碍物（圆和方）是否与USV相撞的信息数组
extern DynamicOR dynamicor[20];
extern DoCalNum docalNUM[20];//需要计算动态障碍的次数
extern int allp;//静态障碍物个数
extern Pathpoint gpathBest[50];  //全局最优路径解
extern StaticOR staticor[20];
extern StaticOC staticoc[20];
extern SOBSTICLE SObsticle_num[30];//障碍物数组，一维为按维度极径排序，二维为方圆判断（1为圆，2为方）及在该障碍物数组的次序（0开始）
//extern PathpointDO gpathBestDO[30];//动态全局最优路径解

ParticleDO::ParticleDO()
{
}

ParticleDO::~ParticleDO()
{
}

void ParticleDO::set_BasicP_num(int m)
{
	BasicP_num = m;
	/*calnumPATH = docalNUM[BasicP_num].calnumPATH;
	startNUM = docalNUM[BasicP_num].startNUM;
	pointNum = DObsticle_num[startNUM].pointNum;*/
}

int ParticleDO::get_BasicP_num()const
{
	return BasicP_num;
}

void ParticleDO::set_pathpoint()
{	
	//初始化该粒子
	calnumPATH = docalNUM[BasicP_num].calnumPATH;
	startNUM = docalNUM[BasicP_num].startNUM;
	pointNum = DObsticle_num[startNUM].pointNum;

	Particle tempParticle;
	ColREG_do = 1;
	int flag = 0;
	int set_num = 0;
	do
	{
		pathpoint.DAchange = tempParticle.NormalRandom(0, (pi / 2), (-pi), pi);//角度变化量正态分布函数初始化
		pathpoint.Vchange = tempParticle.NormalRandom(0, (Vmax / 2), (-Vmax), Vmax);;//速度变化量正态分布函数初始化
		if ((pathpoint.Vchange + startgoalp.v_kn) > Vmax)
		{
			pathpoint.Vchange = Vmax - startgoalp.v_kn;
		}
		else if ((pathpoint.Vchange + startgoalp.v_kn) < Vmin)
		{
			pathpoint.Vchange = (-startgoalp.v_kn);
		}
		
		flag = judge_PATH_effectivity();//判断路径点是否符合要求，不符合要求为1

		set_num++;
		if (set_num >= pso.PSOsetV_numMAX)//200
		{
			flag = 2;
			ColREG_do = 0;
			set_num = 0;
		}
	} while (flag == 1);
	
	if (flag == 2)
	{
		do
		{
			pathpoint.DAchange = tempParticle.NormalRandom(0, (pi / 2), (-pi), pi);//角度变化量正态分布函数初始化
			pathpoint.Vchange = tempParticle.NormalRandom(0, (Vmax / 2), (-Vmax), Vmax);;//速度变化量正态分布函数初始化
			if ((pathpoint.Vchange + startgoalp.v_kn) > Vmax)
			{
				pathpoint.Vchange = Vmax - startgoalp.v_kn;
			}
			else if ((pathpoint.Vchange + startgoalp.v_kn) < Vmin)
			{
				pathpoint.Vchange = (-startgoalp.v_kn);
			}

			flag = judge_PATH_effectivity();//判断路径点是否符合要求，不符合要求为1

			set_num++;
			if (set_num >= 200)//pso.PSOsetV_numMAX
			{
				flag = 2;
				ColREG_do = 0;
			}
		} while (flag == 1);
	}

	pathpointBest = pathpoint;
	V[0] = 2 * pi / (float)pso.PSOsetInitialV;  // 修改2*pi/8?(PMmax - PMmin) / 8.0f   角度变化
	V[1] = 2.0;//速度变化
	Fitness = calculate_Fitness(pathpoint);
}

float ParticleDO::calculate_Fitness(PathpointDO pathpoint)
{
	/*calnumPATH = docalNUM[BasicP_num].calnumPATH;
	startNUM = docalNUM[BasicP_num].startNUM;
	pointNum = DObsticle_num[startNUM].pointNum;

	float Fitness_temp, a_path;
	int n;
	n = DObsticle_num[startNUM].num;
	if (DObsticle_num[n].form == 1)//圆形动态障碍物
	{
		a_path = dynamicoc[n].a_path;
	}
	else if (DObsticle_num[n].form == 2)//矩形动态障碍物
	{
		a_path = dynamicor[n].a_path;
	}*/
	float Fitness_temp;
	Fitness_temp = pso_do.WeightV *abs( pathpoint.Vchange) + pso_do.WeightDa * abs(pathpoint.DAchange);

	return Fitness_temp;
}

void ParticleDO::set_Fitness()
{
	if (setV_num<pso.PSOsetV_numMAX)
	{
		float temp = calculate_Fitness(pathpoint);
		if (temp<Fitness)
		{
			Fitness = temp;
			pathpointBest = pathpoint;//particle[].pathpoint[]数组赋值给pathpointBest[]
		}
	}
	/*else
	{
	Fitness = FLT_MAX;//float最大值
	}*/
}

float ParticleDO::get_Fitness()const
{
	return Fitness;
}

void ParticleDO::setV(PathpointDO *gpathBestDO, float w)
{
	float r1, r2;
	//srand((int)time(NULL));

	//至此，该维的障碍物禁入角已求出，未将跨度正X轴的禁入域分开，需判断(forbidA[i].max>forbidA[i].min)，
	//若为否，则为跨度X轴禁入域，需分开为（forbidA[i].max,2*pi）（0,forbidA[i].min）

	bool flag = 0;
	setV_num = 0;

	do
	{
		r1 = rand() / (float)RAND_MAX;
		r2 = rand() / (float)RAND_MAX;
		
		V[0] = w*V[0] + c1*r1*(pathpointBest.DAchange - pathpoint.DAchange) + c2*r2*(gpathBestDO->DAchange - pathpoint.DAchange);
		pathpoint.DAchange = pathpoint.DAchange + V[0];

		V[1] = w*V[1] + c1*r1*(pathpointBest.Vchange - pathpoint.Vchange) + c2*r2*(gpathBestDO->Vchange - pathpoint.Vchange);
		pathpoint.Vchange = pathpoint.Vchange + V[1];

		if ((pathpoint.Vchange + startgoalp.v_kn) > Vmax)
		{
			pathpoint.Vchange = Vmax - startgoalp.v_kn;
		}
		else if ((pathpoint.Vchange + startgoalp.v_kn) < Vmin)
		{
			pathpoint.Vchange = (-startgoalp.v_kn);
		}

		flag = judge_PATH_effectivity();//判断路径点是否符合要求，不符合要求为1

		setV_num++;
		if (setV_num >= pso.PSOsetV_numMAX)
		{
			flag = 0;
		}
	} while (flag == 1);

	/*if (flag == 2)//若初始计算次数超过pso.PSOsetV_numMAX，则返回上一路径值重算
	{
	i = i - 2;
	continue;
	}*/
	
}

bool ParticleDO::judge_PATH_effectivity()
{
	calnumPATH = docalNUM[BasicP_num].calnumPATH;
	startNUM = docalNUM[BasicP_num].startNUM;
	pointNum = DObsticle_num[startNUM].pointNum;

	bool flag = 0;
	float change_pa_vso;//合成速度角度变化值
	int temp_startNUM = startNUM;//开始位置
	float pa_after;//合成角度变化之后的值
	int n;//在各自障碍物数组中的位置
	float min, max;

	//1.判断是否可避让本路径段的动态障碍物（加入了海事规则）
	for (int i = 0; i < calnumPATH; i++)
	{
		n = DObsticle_num[temp_startNUM].num;
		if (DObsticle_num[temp_startNUM].form == 1)//圆形动态障碍物
		{
			change_pa_vso = (((-sin(dynamicoc[n].Vpa)*pathpoint.Vchange) + startgoalp.v_kn * cos(dynamicoc[n].Vpa)*pathpoint.DAchange) / dynamicoc[n].Vso);//合成速度角度变化量
			pa_after = dynamicoc[n].pa_vso + change_pa_vso;
			
			if (pa_after < 0)
			{
				pa_after = pa_after + 2 * pi;
			}
			else if (pa_after >= (2 * pi))
			{
				pa_after = pa_after - 2 * pi;
			}

			if (ColREG_do==1)
			{
				if (DObsticle_num[temp_startNUM].ColREG == true)
				{
					min = dynamicoc[n].max;
					max = pi + dynamicoc[n].a_path_DOC;
					if (max >= 2 * pi)
					{
						max = max - 2 * pi;
					}
				}
				else if (DObsticle_num[temp_startNUM].ColREG == false)
				{
					max = dynamicoc[n].min;
					min = pi + dynamicoc[n].a_path_DOC;
					if (min >= 2 * pi)
					{
						min = min - 2 * pi;
					}
				}
			}
			else if (ColREG_do == 0)
			{
				max = dynamicoc[n].min;
				min = dynamicoc[n].max;
			}			

		}
		else if (DObsticle_num[temp_startNUM].form == 2)//矩形动态障碍物
		{
			change_pa_vso = (((-sin(dynamicor[n].Vpa)*pathpoint.Vchange) + startgoalp.v_kn * cos(dynamicor[n].Vpa)*pathpoint.DAchange) / dynamicor[n].Vso);//合成速度角度变化量
			pa_after = dynamicor[n].pa_vso + change_pa_vso;
			
			if (pa_after < 0)
			{
				pa_after = pa_after + 2 * pi;
			}
			else if (pa_after >= (2 * pi))
			{
				pa_after = pa_after - 2 * pi;
			}

			if (ColREG_do == 1)
			{
				if (DObsticle_num[temp_startNUM].ColREG == true)
				{
					min = dynamicor[n].max;
					max = pi + dynamicor[n].a_path_DOC;
					if (max >= 2 * pi)
					{
						max = max - 2 * pi;
					}
				}
				else if (DObsticle_num[temp_startNUM].ColREG == false)
				{
					max = dynamicor[n].min;
					min = pi + dynamicor[n].a_path_DOC;
					if (min >= 2 * pi)
					{
						min = min - 2 * pi;
					}
				}
			}
			else if (ColREG_do == 0)
			{
				max = dynamicor[n].min;
				min = dynamicor[n].max;
			}
		}

		//进行判断
		if (min>max)
		{
			if (!((min <= pa_after) && (pa_after < 2 * pi) || (0 <= pa_after) && (pa_after <max)))
			{
				flag = 1;
				break;
			}
		}
		else
		{
			if (!((min <= pa_after) && (pa_after <= max)))
			{
				flag = 1;
				break;
			}
		}

		temp_startNUM++;

	}


	//2.判断是否可避让静态障碍物
	if (flag == 0)
	{
		if ((SObsticle_num[0].form != 1) && (SObsticle_num[0].form != 2))//没有静态障碍物，全部pointNum = -1
		{
			flag = 0;
		}
		else
		{
			float x0, y0, x1, y1, tempY, Xobs_t, Yobs_t, k, Ab, temppm;
			n = DObsticle_num[startNUM].num;
			float pa_v;//改变后路径的方向角
			float Vso; //改变后的合成速度
			float Vusv;// 为改变后USV的速度

			if (DObsticle_num[startNUM].form == 1)//圆形动态障碍物
			{
				pa_v = dynamicoc[n].a_path + pathpoint.DAchange;
				Vusv = startgoalp.v_kn + pathpoint.Vchange;
				//改变后
				Ab = abs(pa_v - dynamicoc[n].Da*pi / 180);//路径段速度方向和障碍物速度方向夹角
				Vso = sqrt(pow((float)Vusv, 2) + pow((float)dynamicoc[n].V, 2) - 2 * Vusv * dynamicoc[n].V * cos(Ab));
			}
			else if (DObsticle_num[startNUM].form == 2)//矩形动态障碍物	
			{
				pa_v = dynamicor[n].a_path + pathpoint.DAchange;
				Vusv = startgoalp.v_kn + pathpoint.Vchange;
				//改变后
				Ab = abs(pa_v - dynamicor[n].Da*pi / 180);//路径段速度方向和障碍物速度方向夹角
				Vso = sqrt(pow((float)Vusv, 2) + pow((float)dynamicor[n].V, 2) - 2 * Vusv * dynamicor[n].V * cos(Ab));
			}

			if (pa_v < 0)
				pa_v = pa_v + 2 * pi;
			else if (pa_v>=2 * pi)
				pa_v = pa_v - 2 * pi;

			if (pointNum == -1)//1.起始点和第一路径点    //显示坐标系下
			{
				x0 = startgoalp.sx;
				y0 = startgoalp.sy;
			}
			else  //显示极坐标系下
			{
				x0 = startgoalp.sx + gpathBest[pointNum].GeneralP.pm * cos(gpathBest[pointNum].GeneralP.pa);
				y0 = startgoalp.sy - gpathBest[pointNum].GeneralP.pm * sin(gpathBest[pointNum].GeneralP.pa);
			}
			//新增路径点
			float newpointPA, newpointPM;
			point_d newPoint;
			SetStaticObstacleCircle calculate_PA;
			ParticleDO calculateNewpoint;

			newPoint = calculateNewpoint.calculate_newpoint(x0, y0, docalNUM[BasicP_num].xChange, docalNUM[BasicP_num].yChange, Vso, pa_v, Vusv);
			
			temppm = sqrt(pow((float)abs(startgoalp.sx - newPoint.x), 2) + pow((float)abs(startgoalp.sy - newPoint.y), 2));
			if (temppm <= docalNUM[BasicP_num].pm)
			{
				newpointPM = temppm;
				newpointPA = calculate_PA.calculate_PolarAngle(newPoint.x, newPoint.y);
			}
			else
			{
				x0 = x0 - startgoalp.sx;
				y0 = y0 - startgoalp.sy;
				newpointPM = docalNUM[BasicP_num].pm;
				newpointPA = calculateNewpoint.calculate_newpointPA(x0, y0, pa_v, docalNUM[BasicP_num].pm);
			}
			
			//newpointPA = calculate_PA.calculate_PolarAngle(newPoint.x, newPoint.y);//极坐标系下

			for (int z = 0; z < 2; z++)
			{
				if (z==0)//路径段第一点与避障点
				{
					if (pointNum == -1)//1.起始点和第一路径点    //极坐标系下
					{
						x0 = 0.0;
						y0 = 0.0;
					}
					else  //极坐标系下
					{
						x0 = gpathBest[pointNum].GeneralP.pm * cos(gpathBest[pointNum].GeneralP.pa);
						y0 = gpathBest[pointNum].GeneralP.pm * sin(gpathBest[pointNum].GeneralP.pa);
					}
				}
				else//避障点与路段末点
				{
					if (pointNum == -1)//1.起始点和第一路径点    //极坐标系下
					{
						if ((SObsticle_num[0].form == 1)||(SObsticle_num[0].form == 2))
						{
							x0 = gpathBest[pointNum + 1].GeneralP.pm * cos(gpathBest[pointNum + 1].GeneralP.pa);
							y0 = gpathBest[pointNum + 1].GeneralP.pm * sin(gpathBest[pointNum + 1].GeneralP.pa);
						}
						else
						{
							x0 = startgoalp.gx - startgoalp.sx;
							y0 = startgoalp.sy - startgoalp.gy;
						}
					}
					else  //极坐标系下
					{
						if (pointNum + 1==allp)
						{
							x0 = startgoalp.gx - startgoalp.sx;
							y0 = startgoalp.sy - startgoalp.gy;
						}
						else
						{
							x0 = gpathBest[pointNum + 1].GeneralP.pm * cos(gpathBest[pointNum + 1].GeneralP.pa);
							y0 = gpathBest[pointNum + 1].GeneralP.pm * sin(gpathBest[pointNum + 1].GeneralP.pa);
						}						
					}
				}
				
				//显示坐标下
				x1 = startgoalp.sx + newpointPM * cos(newpointPA);
				y1 = startgoalp.sy - newpointPM * sin(newpointPA);
				x0 = startgoalp.sx + x0;
				y0 = startgoalp.sy - y0;

				for (int m = 0; m < allp; m++)
				{
					int j = SObsticle_num[m].num;
					if (SObsticle_num[m].form == 1)//圆形静态障碍物
					{
						//情况1：圆心连线不与路径相交，但障碍圆跨过路径
						if ((x0 - x1) == 0)//考虑k为无穷大，即路径垂直于x轴，概率小
						{
							tempY = abs(x0 - staticoc[j].x);//障碍圆心与路径之间的距离

							//路径点处的直角坐标系值
							Xobs_t = x0;
							Yobs_t = staticoc[j].y;
						}
						else
						{
							k = (y0 - y1) / (x0 - x1);
							tempY = abs((k* staticoc[j].x - staticoc[j].y + y0 - k*x0) / sqrt(pow(k, 2) + 1.0));//障碍圆心与路径之间的距离

							//路径点处的直角坐标系值
							Xobs_t = (staticoc[j].x + k * staticoc[j].y + k * (k*x0 - y0)) / (1 + k*k);
							Yobs_t = k*(Xobs_t - x0) + y0;
						}

						if (tempY < (staticoc[j].r / 2 + safe_expandT)) //与障碍圆相交
						{
							//障碍物与外切直线的交点的直角坐标系值
							flag = calculate_1pointIN2point(x0, y0, x1, y1, Xobs_t, Xobs_t);//障碍圆心在起点和终点之间
							if (flag==1)
							{
								break;
							}
						}

						//情况2：圆心连线不与路径相交，但障碍圆与路径只有一个交点
						float tempPM, tempPA;
						tempPM = sqrt(pow((float)abs((float)startgoalp.sx - x1), 2) + pow((float)abs((float)startgoalp.sy - y1), 2));
						tempPA = calculate_2point_pa((float)startgoalp.sx, (float)startgoalp.sy, x1, y1);
						if (pointNum==-1)
						{
							if ((pow(gpathBest[0].GeneralP.pm, 2) + pow(tempPM, 2) - 2 * gpathBest[0].GeneralP.pm*tempPM*cos(abs(gpathBest[0].GeneralP.pa - tempPA)))
								<= pow((float)(staticoc[j].r / 2 + safe_expandT), 2))
							{
								flag = 1;
								break;
							}
						}
						else
						{
							if ((pow(gpathBest[pointNum].GeneralP.pm, 2) + pow(tempPM, 2) - 2 * gpathBest[pointNum].GeneralP.pm*tempPM*cos(abs(gpathBest[pointNum].GeneralP.pa - tempPA)))
								<= pow((float)(staticoc[j].r / 2 + safe_expandT), 2))
							{
								flag = 1;
								break;
							}
						}						

					}
					else if (SObsticle_num[m].form == 2)//方形静态障碍物
					{
						//路径点转换为显示坐标系下计算
						point_d s1, e1;//判断的四个顶点

						s1.x = x0;
						s1.y = y0;
						e1.x = x1;
						e1.y = y1;

						if (IsLineIntersectRect(s1, e1, staticor[j].Peak[0], staticor[j].Peak[1], staticor[j].Peak[2], staticor[j].Peak[3]))//穿过障碍物
						{
							flag = 1;
							break;
						}
					}
				}
			}			
		}
	}	
	
	return flag;
}

float ParticleDO::calculate_newpointPA(float x0, float y0, float pa_v, float pm)//计算新增路径点的极角(直线和圆的交点)//需转换为极坐标下
{
	//极坐标系下
	float a, b, c, b4ac, x1, y1, x, y, pa, x00,y00,x10,y10,temppa;
	a = 1 + pow(tan(pa_v), 2);
	b = 2 * tan(pa_v)*(y0 - tan(pa_v)*x0);
	c = pow((tan(pa_v)*x0 - y0), 2) - pm*pm;
	b4ac = b*b - 4 * a*c;
	x1 = (-b + sqrt(b4ac)) / (2 * a);
	y1 = tan(pa_v)*(x1 - x0) + y0;
	//x2 = (-b - sqrt(b4ac)) / (2 * a);
	//y2 = tan(pa_v)*(x2 - x0) + y0;

	//转换为显示坐标系下
	x00 = startgoalp.sx + x0;
	y00 = startgoalp.sy - y0;
	x10 = startgoalp.sx + x1;
	y10 = startgoalp.sy - y1;

	temppa = calculate_2point_pa(x00, y00, x10, y10);
	if (((pa_v - 0.1) <= temppa) && (temppa<=(pa_v+0.1)))//计算有误差
	{
		x = x1;
		y = y1;
	}	
	else
	{
		x = (-b - sqrt(b4ac)) / (2 * a);
		y = tan(pa_v)*(x - x0) + y0;
	}

	SetStaticObstacleCircle calculate_PA;
	x = x+startgoalp.sx;
	y = startgoalp.sy - y;
	pa = calculate_PA.calculate_PolarAngle(x,y);//极角;

	return pa;
}

//计算新增路径点//需转换为显示坐标系下(x0,y0相撞路段的起点，xyChange为相撞起点时刻障碍物结束避障点坐标，Vso为改变后的合成速度，pa_v为改变后路径的方向角,Vusv为改变后USV的速度)
point_d ParticleDO::calculate_newpoint(float x0, float y0, float xChange, float yChange, float Vso, float pa_v, float Vusv)
{
	//需转换为显示坐标系下
	point_d newpoint;
	float t,pmNew,L;
	pmNew = sqrt(pow((float)abs(x0 - xChange), 2) + pow((float)abs(y0 - yChange), 2));
	t = pmNew / drawscale / (Vso* Kn_ms);
	L = Vusv * Kn_ms * (t+1) * drawscale;//2014.9.30
	newpoint.x = x0 + L * cos(pa_v);
	newpoint.y = y0 - L * sin(pa_v);
	
	return newpoint;
}

point_d ParticleDO::calculate_point_segment_circle(float x0, float y0, float x1, float y1, float r)//计算倒角时(线段和圆的交点)，x0，y0同时为圆心//需转换为显示坐标下
{
	//显示坐标下
	point_d point;
	float x, y,k;
	k = (y1-y0) / (x1-x0);
	x = x0 + sqrt(r*r/(1+k*k));
	y = k*(x - x0) + y0;

	if (calculate_1pointIN2pointLINE(x0, y0, x1, y1, x, y))
	{
		point.x = x;
		point.y = y;
	}
	else
	{
		x = x0 - sqrt(r*r / (1 + k*k));
		y = k*(x - x0) + y0;
		point.x = x;
		point.y = y;
	}
	
	return point;
}

bool ParticleDO::calculate_ColREG(int num)//计算满足避碰规则的通行方向 true为左侧通行，FALSE为右侧通行
{
	int m = DObsticle_num[num].pointNum;//从哪个节点开始
	float x1, y1, x2, y2,pa1,pa2,pa,Vobs;
	bool ColREG;

	if (DObsticle_num[num].form==1)
	{
		pa2 = dynamicoc[DObsticle_num[num].num].Da;
		Vobs = dynamicoc[DObsticle_num[num].num].V;
	}
	else if (DObsticle_num[num].form == 2)
	{
		pa2 = dynamicor[DObsticle_num[num].num].Da;
		Vobs = dynamicor[DObsticle_num[num].num].V;
	}
	
	if (m == -1)
	{
		x1 = startgoalp.sx;
		y1 = startgoalp.sy;
		if ((SObsticle_num[0].form != 1) & (SObsticle_num[0].form != 2))//没有静态障碍物
		{
			x2 = startgoalp.gx;
			y2 = startgoalp.gy;
		}
		else
		{
			x2 = startgoalp.sx + gpathBest[m + 1].GeneralP.pm*cos(gpathBest[m + 1].GeneralP.pa);
			y2 = startgoalp.sy - gpathBest[m + 1].GeneralP.pm*sin(gpathBest[m + 1].GeneralP.pa);
		}
	}
	else if (m == allp-1)
	{
		x1 = startgoalp.sx + gpathBest[m].GeneralP.pm*cos(gpathBest[m].GeneralP.pa);
		y1 = startgoalp.sy - gpathBest[m].GeneralP.pm*sin(gpathBest[m].GeneralP.pa);
		x2 = startgoalp.gx;
		y2 = startgoalp.gy;
	}
	else
	{
		x1 = startgoalp.sx + gpathBest[m].GeneralP.pm*cos(gpathBest[m].GeneralP.pa);
		y1 = startgoalp.sy - gpathBest[m].GeneralP.pm*sin(gpathBest[m].GeneralP.pa);
		x2 = startgoalp.sx + gpathBest[m + 1].GeneralP.pm*cos(gpathBest[m + 1].GeneralP.pa);
		y2 = startgoalp.sy - gpathBest[m + 1].GeneralP.pm*sin(gpathBest[m + 1].GeneralP.pa);
	}
	pa1 = calculate_2point_pa(x1, y1, x2, y2) * 180 / pi;

	pa = abs(pa1-pa2);//交角

	if ((0 <= pa) && (pa < 45))//追越：障碍物在右侧，看两者速度，usv速度大则左侧通行，否则右侧通行
	{
		if (startgoalp.v_kn>Vobs)
		{
			ColREG = true;
		}
		else
		{
			ColREG = false;
		}
	}
	else if ( (315 < pa) && (pa <= 360) )//追越：障碍物在左侧（看两者速度，usv速度大则左侧通行，否则右侧通行）
	{
		ColREG = true;
	}
	else if ((165<pa)&&(pa<195))//相遇：右侧通行
	{
		ColREG = false;
	}
	else if ((45<=pa) && (pa<=165))//右侧交叉相遇：右侧通行
	{
		if (pa1 <= 180)
			ColREG = false;
		else
			ColREG = true;
	}
	else if ((195 <= pa) && (pa <= 315))//左侧交叉相遇：左侧通行
	{
		if (pa1 <= 180)
			ColREG = true;
		else
			ColREG = false;
	}
	
	return ColREG;
}

float ParticleDO::calculate_2point_pa(float x1, float y1, float x2, float y2)//计算两点的角度，极坐标下  1为起始点 到 2终点  返回弧度值 //显示坐标系下  (相当于以1为圆点的极坐标系)
{
	float pa;
	//显示坐标系下
	if (x2 == x1)
	{
		if (y2<y1)
			pa = pi / 2;
		else
			pa = 3 * pi / 2;
	}
	else
	{
		if (y2==y1)
		{
			if (x2 > x1)
				pa = 0;
			else
				pa = pi;			
		}
		else
		{
			pa = (-atan((y2 - y1) / (x2 - x1)));
			if (x2>x1)
			{
				if (pa<0)
				{
					pa = pa + 2 * pi;
				}
			}
			else
			{
				pa = pa + pi;
			}
		}		
	}

	return pa;	
}

bool ParticleDO::calculate_1pointIN2point(float x1, float y1, float x2, float y2,float x,float y)//计算一点是否在一条线段中间（针对计算障碍物圆计算）
{
	bool flag=0;
	float xmax, xmin, ymax, ymin;
	if (x2 == x1)
	{
		if (y2 > y1)
		{
			xmax = x2;
			xmin = x2;
			ymax = y2;
			ymin = y1;
		}
		else
		{
			xmax = x2;
			xmin = x2;
			ymax = y1;
			ymin = y2;
		}
	}
	else if (y2 == y1)
	{
		if (x2 > x1)
		{
			xmax = x2;
			xmin = x1;
			ymax = y2;
			ymin = y2;
		}
		else
		{
			xmax = x1;
			xmin = x2;
			ymax = y2;
			ymin = y2;
		}
	}
	else
	{
		if (y2 > y1)
		{
			if (x2>x1)
			{
				xmax = x2;
				xmin = x1;
				ymax = y2;
				ymin = y1;
			}
			else if (x1>x2)
			{
				xmax = x1;
				xmin = x2;
				ymax = y2;
				ymin = y1;
			}
		}
		else if (y2<y1)
		{
			if (x2>x1)
			{
				xmax = x2;
				xmin = x1;
				ymax = y1;
				ymin = y2;
			}
			else if (x1>x2)
			{
				xmax = x1;
				xmin = x2;
				ymax = y1;
				ymin = y2;
			}
		}
	}

	if (((xmin <= x) && (x <= xmax)) || ((ymin <= y) &&( y <= ymax)))
	{
		flag = 1;
	}

	return flag;
}

bool ParticleDO::calculate_1pointIN2pointLINE(float x, float y, float x1, float y1, float x2, float y2)//计算一点是否在一条线段，以
{
	bool flag = 0;

	if ((((x - x1)*(x - x2))>0) || (((y - y1)*(y - y2))>0))
	{
		flag = 1;
	}

	return flag;
}

float ParticleDO::multi(point_d p1, point_d p2, point_d p0)//求矢量[p0, p1], [p0, p2]的叉积  
{
	return (p1.x - p0.x) * (p2.y - p0.y) - (p2.x - p0.x) * (p1.y - p0.y);
	//p0是顶点            
	//若结果等于0，则这三点共线  
	//若结果大于0，则p0p2在p0p1的逆时针方向    
	//若结果小于0，则p0p2在p0p1的顺时针方向    
}

bool ParticleDO::IsIntersected(point_d s1, point_d e1, point_d s2, point_d e2)//判断线段是否相交  
{
	//1.快速排斥试验判断以两条线段为对角线的两个矩形是否相交      
	//2.跨立试验	
	if ((max(s1.x, e1.x) > min(s2.x, e2.x)) &&
		(max(s2.x, e2.x) > min(s1.x, e1.x)) &&
		(max(s1.y, e1.y) > min(s2.y, e2.y)) &&
		(max(s2.y, e2.y) > min(s1.y, e1.y)) &&
		(multi(s2, e1, s1) * multi(e1, e2, s1) >= 0) &&
		(multi(s1, e2, s2) * multi(e2, e1, s2) >= 0))
		return true;
	return false;
}

bool ParticleDO::IsLineIntersectRect(point_d ptStart, point_d ptEnd, REC Peak0, REC Peak1, REC Peak2, REC Peak3)// 判断线段是否与矩形相交
{
	// Two point both are in rect// One point is in rect, another not.
	SetStaticObstacleRectangle Rectangle;
	if ((Rectangle.calculate_pointInRC(Peak0, Peak1, Peak2, Peak3, ptStart.x, ptStart.y).point == 5) || (Rectangle.calculate_pointInRC(Peak0, Peak1, Peak2, Peak3, ptEnd.x, ptEnd.y).point == 5))
		return TRUE;
	// Two point both aren't in rect
	point_d s2, e2;
	s2.x = Peak0.xEX;
	s2.y = Peak0.yEX;
	e2.x = Peak1.xEX;
	e2.y = Peak1.yEX;
	if (IsIntersected(ptStart, ptEnd, s2, e2))
		return TRUE;

	s2.x = Peak1.xEX;
	s2.y = Peak1.yEX;
	e2.x = Peak2.xEX;
	e2.y = Peak2.yEX;
	if (IsIntersected(ptStart, ptEnd, s2, e2))
		return TRUE;

	s2.x = Peak2.xEX;
	s2.y = Peak2.yEX;
	e2.x = Peak3.xEX;
	e2.y = Peak3.yEX;
	if (IsIntersected(ptStart, ptEnd, s2, e2))
		return TRUE;

	s2.x = Peak0.xEX;
	s2.y = Peak0.yEX;
	e2.x = Peak3.xEX;
	e2.y = Peak3.yEX;
	if (IsIntersected(ptStart, ptEnd, s2, e2))
		return TRUE;

	return FALSE;
}

float ParticleDO::calculate_DynamicL(Pathpoint *pathpoint)
{
	float pa, length, start, end, Fitness_temp, sg;
	
	//首段
	if (pathpoint[0].AdditionS.pm != 0.0)
	{
		if (abs(pathpoint[0].GeneralP.pa - pathpoint[0].AdditionS.pa)<pi)
		{
			pa = abs(pathpoint[0].GeneralP.pa - pathpoint[0].AdditionS.pa);
		}
		else
		{
			pa = 2 * pi - abs(pathpoint[0].GeneralP.pa - pathpoint[0].AdditionS.pa);
		}

		start = pathpoint[0].AdditionS.pm + sqrt(pow((float)pathpoint[0].GeneralP.pm, 2) + pow((float)pathpoint[0].AdditionS.pm, 2) - 2 * pathpoint[0].GeneralP.pm*pathpoint[0].AdditionS.pm*cos(pa));
		
	}
	else
	{
		start = pathpoint[0].GeneralP.pm;
	}

	//尾段
	if (pathpoint[allp - 1].AdditionP.pm != 0.0)
	{
		if (abs(pathpoint[allp - 1].GeneralP.pa - pathpoint[allp - 1].AdditionP.pa)<pi)
		{
			pa = abs(pathpoint[allp - 1].GeneralP.pa - pathpoint[allp - 1].AdditionP.pa);
		}
		else
		{
			pa = 2 * pi - abs(pathpoint[allp - 1].GeneralP.pa - pathpoint[allp - 1].AdditionP.pa);
		}

		end = sqrt(pow((float)pathpoint[allp - 1].GeneralP.pm, 2) + pow((float)pathpoint[allp - 1].AdditionP.pm, 2) - 2 * pathpoint[allp - 1].GeneralP.pm*pathpoint[allp - 1].AdditionP.pm*cos(pa));

		sg = sqrt(pow((float)abs(int(startgoalp.sx - startgoalp.gx)), 2) + pow((float)abs(int(startgoalp.sy - startgoalp.gy)), 2));
		end = end + sqrt(pow((float)pathpoint[allp - 1].AdditionP.pm, 2) + pow((float)sg, 2) - 2 * sg*pathpoint[allp - 1].AdditionP.pm*cos(pathpoint[allp - 1].AdditionP.pa));

	}
	else
	{
		sg = sqrt(pow((float)abs(int(startgoalp.sx - startgoalp.gx)), 2) + pow((float)abs(int(startgoalp.sy - startgoalp.gy)), 2));
		end = sqrt(pow((float)pathpoint[allp - 1].GeneralP.pm, 2) + pow((float)sg, 2) - 2 * sg*pathpoint[allp - 1].GeneralP.pm*cos(pathpoint[allp - 1].GeneralP.pa));
	}
	
	Fitness_temp = start + end;//首段和尾段距离


	for (int i = 0; i < allp - 1; i++)
	{
		if (pathpoint[i].AdditionP.pm!=0.0)
		{
			if (abs(pathpoint[i].GeneralP.pa - pathpoint[i].AdditionP.pa)<pi)
			{
				pa = abs(pathpoint[i].GeneralP.pa - pathpoint[i].AdditionP.pa);
			}
			else
			{
				pa = 2 * pi - abs(pathpoint[i].GeneralP.pa - pathpoint[i].AdditionP.pa);
			}

			length = sqrt(pow((float)pathpoint[i].GeneralP.pm, 2) + pow((float)pathpoint[i].AdditionP.pm, 2) - 2 * pathpoint[i].GeneralP.pm*pathpoint[i].AdditionP.pm*cos(pa));

			if (abs(pathpoint[i+1].GeneralP.pa - pathpoint[i].AdditionP.pa)<pi)
			{
				pa = abs(pathpoint[i+1].GeneralP.pa - pathpoint[i].AdditionP.pa);
			}
			else
			{
				pa = 2 * pi - abs(pathpoint[i+1].GeneralP.pa - pathpoint[i].AdditionP.pa);
			}

			length = length + sqrt(pow((float)pathpoint[i + 1].GeneralP.pm, 2) + pow((float)pathpoint[i].AdditionP.pm, 2) - 2 * pathpoint[i + 1].GeneralP.pm*pathpoint[i].AdditionP.pm*cos(pa));
		}
		else
		{
			if (abs(pathpoint[i].GeneralP.pa - pathpoint[i + 1].GeneralP.pa)<pi)
			{
				pa = abs(pathpoint[i].GeneralP.pa - pathpoint[i + 1].GeneralP.pa);
			}
			else
			{
				pa = 2 * pi - abs(pathpoint[i].GeneralP.pa - pathpoint[i + 1].GeneralP.pa);
			}

			length = sqrt(pow((float)pathpoint[i].GeneralP.pm, 2) + pow((float)pathpoint[i + 1].GeneralP.pm, 2) - 2 * pathpoint[i].GeneralP.pm*pathpoint[i + 1].GeneralP.pm*cos(pa));
		}		

		Fitness_temp = Fitness_temp + length;
	}
	return Fitness_temp;
}

float  ParticleDO::calculate_Pa_02PI(float pa)//返回值域在0-2pi的极径值
{
	float polarA = pa;

	//pa取在0-2*pi中间
	if (polarA >= 2 * pi)
		polarA = fmod(polarA, (float)(2 * pi));
	else if (polarA < 0)
	{
		polarA = fmod(polarA, (float)(2 * pi));
		polarA = polarA + 2 * pi;
	}
	return polarA;
}