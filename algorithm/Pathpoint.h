#pragma once
#include "USVDoc.h"
#include "USVView.h"

class Pathpoint
{
public:
	Pathpoint();
	Pathpoint(float pm, float pa);
	~Pathpoint();
	PolarCoordinate GeneralP;//常规点
	PolarCoordinate AdditionP;//路径节点后的附加点AdditionP[3]
	PolarCoordinate AdditionS;//开始点到一维路径节点之间的附加点AdditionS[3]
	
	//第一点为路径点的极坐标系下的pa、pm，后面只算pa，为从当前点到下一步长的极坐标（以当前点为极点）		
	PolarCoordinate AdditionPU[200];//动态未知障碍物路径点（若无动态已知附加点，仅有此值，若有动态已知附加点，则此值为上半段）
	PolarCoordinate AdditionSU[200];//开始点到一维路径节点之间的动态未知障碍物路径点（若无动态已知附加点，仅有此值，若有动态已知附加点，则此值为上半段）
	PolarCoordinate AdditionPUa[200];//动态未知障碍物路径点（有动态已知附加点，此值为下半段）
	PolarCoordinate AdditionSUa[200];//开始点到一维路径节点之间的动态未知障碍物路径点（有动态已知附加点，此值为下半段）
};