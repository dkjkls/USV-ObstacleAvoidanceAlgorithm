// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。  
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。  
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://go.microsoft.com/fwlink/?LinkId=238214。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// USVView.h : USVView 类的接口
//

#pragma once
#define pi 3.1416
#define generations 200//粒子群迭代次数
#define particle_num 100//粒子数
#define setV_numMAX 300//设置速度次数最大值
#define drawscale 0.25//比例尺1:4
#define safe_expand 24//静态障碍物安全膨胀参数，三倍船长，未换算
#define safe_expandT 6//静态障碍物安全膨胀参数，三倍船长，已换算
#define Kn_ms 0.514444//节转换为m/s

typedef struct ViewSizeDEF
{
	UINT H, W;
	bool change;
}ViewSize;

typedef struct StartGoalPOINTDEF
{
	UINT sx, sy, gx, gy;
	float v_kn, v_ms;
}StartGoalPOINT;

typedef struct StaticOCDEF
{
	unsigned int x, y, r;
	float pm;//极径
	float pa;//极角
}StaticOC;

typedef struct DynamicOCDEF
{
	unsigned int x, y, r;
	float pm;//极径
	float pmChange;//相撞那点的极径
	//float paChange;//相撞那点的极角
	float xChange;//相撞路段起点的避障结束转弯点x
	float yChange;//相撞路段起点的避障结束转弯点y
	float pa;//极角
	float Da;//方向角
	float V;//速度
	float min;
	float max;
	float Vso;//障碍物，usv合成速度
	float Vpa;//合成角，usv速度交角
	float pa_vso;//合成速度的角度
	float a_path_DOC;//该时刻动态障碍物与路径点连线角度
	float a_path;//该时刻路径段的连线角度
}DynamicOC;

typedef struct defREC
{
	float x, y;//显示屏中的坐标
	float xEX, yEX;//显示屏中的加上扩展值的坐标
	float Pm;//开始点坐标系的 极径
	float Pa;//开始点坐标系的 极角
	float PmEX;//开始点坐标系的 加上扩展值的 极径
	float PaEX;//开始点坐标系的 加上扩展值的 极角
}REC;

typedef struct StaticORDEF
{
	UINT CenterX, CenterY, HalfL, HalfW;
	float CenterPm;//极径
	float CenterPa;//极角
	REC Peak[4];
	float Da;//方向角
}StaticOR;

typedef struct DynamicORDEF
{
	UINT CenterX, CenterY, HalfL, HalfW;
	float CenterPm;//极径
	float CenterPa;//极角
	float pmChange;//相撞那点的极径
	//float paChange;//相撞那点的极角
	float xChange;//相撞路段起点的避障结束转弯点x
	float yChange;//相撞路段起点的避障结束转弯点y
	REC Peak[4];
	float Da;//方向角
	float V;//速度
	float min;
	float max;
	float Vso;//障碍物，usv合成速度
	float Vpa;//合成角，usv速度交角
	float pa_vso;//合成速度的角度
	float a_path_DOC;//该时刻动态障碍物与路径点连线角度
	float a_path;//该时刻路径段的连线角度
}DynamicOR;

typedef struct ForbidADEF
{
	float min;
	float max;
}ForbidA;

typedef struct PSODEF
{
	int PSOgenerations;//迭代次数
	int PSOparticle_num;//粒子数
	int PSOsetV_numMAX;//设置速度最大次数
	int PSOsetInitialV;//初始化速度参数
}PSO;

typedef struct PSO_DODEF
{
	UINT PSOgenerations;//迭代次数
	UINT PSOparticle_num;//粒子数
	UINT WeightV;//设置速度变化权重
	UINT WeightDa;//设置角度变化权重
	bool changeWeight;//是否可调权重值
}PSO_DO;

typedef struct bestFitnessLISTDEF
{
	int num;
	float bestFitness;
}bestFitnessLIST;

typedef struct bestEachGenerationsDEF
{
	int PSOgenerations;//迭代次数
	int PSOparticle_num;//粒子数
	int PSOsetV_numMAX;//设置速度最大次数
	int PSOsetInitialV;//初始化速度参数
	float UsedTime;
	int bestFitness_num;
	float bestFitness;
	float gbestFitness_OptimizePATH;
	int thisPSOnum;
}bestEachGenerations;

typedef struct SOBSTICLEDEF
{
	int form;//障碍物形式（1为圆，2为方）
	int rcnum;//方形的peak点（peak1，peak3）
	int num;//在该障碍物数组的次序（0开始）
	float pm;//极径值
}SOBSTICLE;

typedef struct DOBSTICLEDEF
{
	int form;//障碍物形式（1为圆，2为方）
	int rcnum;//方形的peak点（peak1，peak3）
	int num;//在该障碍物数组的次序（0开始）
	//float pm;//极径值
	int pointNum;//在哪个路径节点后	
	bool ColREG;//计算满足避碰规则的通行方向,true为左侧，FALSE为右侧
}DOBSTICLE;

typedef struct DoCalNumDEF//需要计算动态障碍的次数
{
	int CalNum;//需要计算动态障碍的次数，即分布在几个路径段中
	int calnumPATH;//每段路径段包含动态障碍物的个数
	int startNUM;//开始的位置
	float pm;//该段极径的最大值
	float xChange;//相撞路段起点的避障结束转弯点x
	float yChange;//相撞路段起点的避障结束转弯点y
}DoCalNum;

typedef struct UnknownOCDEF
{
	unsigned int x, y, r;
	float xEX, yEX, rEX,pmEX;
	float pm;//极径
	float pa;//极角
	float Da;//方向角
	float V;//速度
	int t;//持续时间
}UnknownOC;

typedef struct UnknownORDEF
{
	UINT CenterX, CenterY, HalfL, HalfW;
	float CenterPm;//极径
	float CenterPa;//极角
	REC Peak[4];
	float Da;//方向角
	float V;//速度
	int t;//持续时间
}UnknownOR;

typedef struct UnknownODEF
{
	int form;//障碍物形式（1为圆，2为方）
	float pm;//极径	
	int pointNum;//在哪个路径节点后	
	int pointNumBOOL;//若有附加点，0无附加点，-1在当前路径点和附加点之间，1为在附加点和路径末点之间
	int t;//持续时间
	float Da;//方向角
	float V;//速度
	int num;//在该障碍物数组的次序（0开始）
	int CalNum;//需要计算动态障碍的次数，即分布在几个路径段中
	int calnumPATH;//每段路径段包含动态障碍物的个数
	bool collision;//是否相碰（0未碰，1碰）
	bool collisionDONE;//是否已完成碰撞

	float xEX, yEX, rEX;//圆形障碍物
	REC Peak[4];//矩形障碍物
}UnknownO;

typedef struct UnknownDEF//USV路径点的参数
{
	float UnknownR;//视野半径
	float UnknownT;//避障间隔
	float min;//视野角度小值（以当前路径点为圆心）
	float max;//视野角度大值（以当前路径点为圆心）
	float x0;//当前x坐标
	float y0;//当前y坐标
	int t;//当前时间
	//int pointNum;//当前路径点
}Unknown;

typedef struct point_dDef
{
	float x, y;
}point_d;

typedef struct PathpointDOdef
{
	float Vchange;//速度变化量
	float DAchange;//角度变化量
}PathpointDO;

typedef struct PolarCoordinatedef
{
	float pm;//极径
	float pa;//极角
	float v;//速度
}PolarCoordinate;

class USVView : public CView
{
protected: // 仅从序列化创建
	USVView();
	DECLARE_DYNCREATE(USVView)

// 特性
public:

	USVDoc* GetDocument() const;
	//float bestFitnessLIST[generations][2];//每代适应度值

	void DrawArrow(point_d p1, point_d p2, double theta, int length,COLORREF rgb);//画箭头
	void DrawFillet();//画到圆角

	float calculateDOtime(int pointNum);//计算动态障碍物与路径相交时所在路径终点，所用的时间

	void calculate_forbidThisDimA();//本维上的障碍物禁入角，及最后一维到目标点的禁入角，在此求，可省略（粒子数×代数倍）计算
	void calculate_SObsticle_num();//计算静态障碍物（圆和方）的维数信息数组
	void calculate_DObsticle_num();//计算动态障碍物（圆和方）是否与USV相撞的信息数组
	void calculate_DObsticle_forbidA();//计算动态相交障碍物禁入角
	REC calculate_drChange(point_d s, point_d e, REC Peak1, REC Peak2, REC Peak3, REC Peak4);//计算动态矩形障碍物与路径点的最远端

	float multi(point_d p1, point_d p2, point_d p0);//求矢量[p0, p1], [p0, p2]的叉积  
	bool IsIntersected(point_d s1, point_d e1, point_d s2, point_d e2);//判断线段是否相交 
	bool IsLineIntersectRect(point_d ptStart, point_d ptEnd, REC Peak0, REC Peak1, REC Peak2, REC Peak3);// 判断线段是否与矩形相交

	void calculate_unknownO();//对障碍物按极径排序
	bool judge_collision(int startpoint,float L);//判断是否相碰
	void calculate_collision(int startpoint, float L, PolarCoordinate *Addition, PolarCoordinate General);//相碰后的避障
	bool judge_so_collision(float x0, float y0, float x1, float y1, int pointNum);//判断是否和静态障碍物相碰
	void drawWD(float x, float y, float pa);//画滚动窗口

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~USVView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnCalculatePSO();
	afx_msg void OnSet_start_goal_Position();

	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSetstaticobstaclecircle();
	afx_msg void OnCombostartgoal();
	afx_msg void OnComboSoc();
	afx_msg void OnDataoutput();
	afx_msg void OnComboPathpoint();
	afx_msg void OnBtsetpso();
//	afx_msg void OnOptimizepath();
	afx_msg void OnOptimizepath();
	afx_msg void OnCalculateandoptimize();
//	afx_msg void OnSetstaticobstaclerectangle();
	afx_msg void OnSetstaticobstaclerectangle();
	afx_msg void OnSetviewsize();
	afx_msg void OnSetdynamicOC();
	afx_msg void OnSetdynamicOR();
	afx_msg void OnComboViewinformation();
	afx_msg void OnComboDo();
	afx_msg void OnComboDoun();
	afx_msg void OnCalculatedoPso();
	afx_msg void Onsetpsodo();

	afx_msg void OnViewdo();
	afx_msg void OnSetunknownoc();
	afx_msg void OnSetunknownor();

	afx_msg void OnSetunknown();
	afx_msg void OnCalculateunknown();
};

#ifndef _DEBUG  // USVView.cpp 中的调试版本
inline USVDoc* USVView::GetDocument() const
   { return reinterpret_cast<USVDoc*>(m_pDocument); }
#endif

