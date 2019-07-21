#pragma once
#include "Pathpoint.h"
#include "USVDoc.h"
#include "USVView.h"

class ParticleDO
{
public:
	ParticleDO();
	~ParticleDO();
	void set_BasicP_num(int m);
	int get_BasicP_num()const;

	void set_pathpoint();
	bool calculate_ColREG(int num);//计算满足避碰规则的通行方向 true为左侧通行，FALSE为右侧通行
	bool judge_PATH_effectivity();
	float calculate_2point_pa(float x1, float y1, float x2, float y2);//计算两点的角度，极坐标下  1为起始点 到 2终点  返回弧度值
	bool calculate_1pointIN2point(float x1, float y1, float x2, float y2, float x, float y);//计算一点是否在一条线段中间（针对计算障碍物圆计算）
	bool calculate_1pointIN2pointLINE(float x1, float y1, float x2, float y2, float x, float y);//计算一点是否在一条线段上
	float calculate_DynamicL(Pathpoint *pathpoint);
	float calculate_Pa_02PI(float pa);//返回值域在0-2pi的极径值

	float multi(point_d p1, point_d p2, point_d p0);//求矢量[p0, p1], [p0, p2]的叉积  
	bool IsIntersected(point_d s1, point_d e1, point_d s2, point_d e2);//判断线段是否相交 
	bool IsLineIntersectRect(point_d ptStart, point_d ptEnd, REC Peak0, REC Peak1, REC Peak2, REC Peak3);// 判断线段是否与矩形相交
	float calculate_newpointPA(float x0, float y0, float pa_v, float pm);//计算新增路径点的极角(直线和圆的交点)
	point_d calculate_newpoint(float x0, float y0, float xChange, float yChange, float Vso, float pa_v, float Vusv);//计算新增路径点//需转换为极坐标下
	point_d calculate_point_segment_circle(float x0, float y0, float x1, float y1, float r);//计算倒角时(直线和圆的交点)，x0，y0同时为圆心//需转换为显示坐标下
	float calculate_Fitness(PathpointDO pathpoint);

	void set_Fitness();
	float get_Fitness()const;
	void setV(PathpointDO *gpathBestDO, float w);

	PathpointDO pathpointBest;

private:
	PathpointDO pathpoint; //= new Pathpoint[BasicP_num];

	bool ColREG_do;//1为进行海事规则避障，0为不进行海事规则避障

	float Fitness;  //适应度  
	static int BasicP_num;//该维中所处docalNUM[z]中的位置
	float V[2];
	int setV_num;//设置速度次数

	int pointNum;//在哪个路径节点后	
	int calnumPATH;//该段路径段包含动态障碍物的个数
	int startNUM;//开始的位置

	static float PMmax, PMmin;
	static float Vmax, Vmin; //它们是用来对坐标和速度进行限制的，限制它只能在一定的范围内。  
	static float c1, c2; //c1,c2是学习因子。  
};

