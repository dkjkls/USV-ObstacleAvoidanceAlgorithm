#pragma once
#include "Pathpoint.h"

class Particle
{
public:
	Particle();
	~Particle();
	void set_BasicP_num(int m);
	int get_BasicP_num()const;

	void set_pathpoint();
	float NormalRandom(float miu, float sigma, float min, float max);//正态分布函数初始化
	float AverageRandom(float min, float max);
	float Normal(float x, float miu, float sigma);

	void calculate_forbidA(int i);
	bool judge_PATH_effectivity(int i);
	float calculate_Fitness(Pathpoint *pathpoint);
	void set_Fitness();
	float get_Fitness()const;

	void setV(Pathpoint *gpathBest, float w);
	void setV_pathpoint();

	float calculatePA(float X, float Y);

	Pathpoint pathpointBest[50];

private:
	Pathpoint pathpoint[50]; //= new Pathpoint[BasicP_num];

	float Fitness;  //适应度  
	static int BasicP_num;//基准维数
	//int Dim_num;//总维数，转向点
	float V[50];

	int setV_num;//设置速度次数

	static float PMmax, PMmin;
	static float Vmax, Vmin; //它们是用来对坐标和速度进行限制的，限制它只能在一定的范围内。  
	static float c1, c2; //c1,c2是学习因子。  
};

