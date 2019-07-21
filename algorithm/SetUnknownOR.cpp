// SetUnknownOR.cpp : 实现文件
//

#include "stdafx.h"
#include "USV.h"
#include "SetUnknownOR.h"
#include "afxdialogex.h"
#include "SetStaticObstacleCircle.h"
#include "SetStaticObstacleRectangle.h"
#include "afxdialogex.h"
#include "USVDoc.h"
#include "USVView.h"
#include "math.h"
#include <afxdb.h>

extern UnknownOR unknownor[20];
extern StartGoalPOINT startgoalp;
extern bool state_unknownOR;//方形未知障碍物状态
extern int testNum;//测试次数
extern int thisPSOnum;//本次pso测试次数
extern Unknown unknown;//局部未知避障

// SetUnknownOR 对话框

IMPLEMENT_DYNAMIC(SetUnknownOR, CDialogEx)

SetUnknownOR::SetUnknownOR(CWnd* pParent /*=NULL*/)
	: CDialogEx(SetUnknownOR::IDD, pParent)
	, UnknownOR_X(0)
	, UnknownOR_Y(0)
	, UnknownOR_HL(0)
	, UnknownOR_HW(0)
	, UnknownOR_DA(0)
	, UnknownOR_V(0)
{

}

SetUnknownOR::~SetUnknownOR()
{
}

void SetUnknownOR::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_UnknownOR, LIST_UnknownOR);
	DDX_Text(pDX, IDC_UnknownOR_X, UnknownOR_X);
	DDV_MinMaxUInt(pDX, UnknownOR_X, 0, 2000);
	DDX_Text(pDX, IDC_UnknownOR_Y, UnknownOR_Y);
	DDV_MinMaxUInt(pDX, UnknownOR_Y, 0, 2000);
	DDX_Text(pDX, IDC_UnknownOR_HL, UnknownOR_HL);
	DDV_MinMaxUInt(pDX, UnknownOR_HL, 0, 2000);
	DDX_Text(pDX, IDC_UnknownOR_HW, UnknownOR_HW);
	DDV_MinMaxUInt(pDX, UnknownOR_HW, 0, 2000);
	DDX_Text(pDX, IDC_UnknownOR_DA, UnknownOR_DA);
	DDX_Text(pDX, IDC_UnknownOR_V, UnknownOR_V);
}


BEGIN_MESSAGE_MAP(SetUnknownOR, CDialogEx)
	ON_BN_CLICKED(IDAddUnknownOR, &SetUnknownOR::OnBnClickedAddunknownor)
	ON_BN_CLICKED(IDDeleteUnknownOR, &SetUnknownOR::OnBnClickedDeleteunknownor)
	ON_BN_CLICKED(IDOK, &SetUnknownOR::OnBnClickedOk)
	ON_BN_CLICKED(IDSUnknownORInput, &SetUnknownOR::OnBnClickedSunknownorinput)
	ON_BN_CLICKED(IDSUnknownOROutput, &SetUnknownOR::OnBnClickedSunknownoroutput)
END_MESSAGE_MAP()


// SetUnknownOR 消息处理程序

BOOL SetUnknownOR::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	CString Field[7] = { "序号", "中心点X", "中心点Y", "半长", "半宽", "方向角", "速度" };//列表视的表头
	LIST_UnknownOR.InsertColumn(0, Field[0], LVCFMT_CENTER, 41);
	for (int j = 1; j<7; j++)
	{
		LIST_UnknownOR.InsertColumn(j, Field[j], LVCFMT_CENTER, 63);				//插入表头标题
	}
	//初始化输入框	
	UnknownOR_X = 0;
	UnknownOR_Y = 0;
	UnknownOR_HL = 0;
	UnknownOR_HW = 0;
	UnknownOR_DA = 0;
	UnknownOR_V = 0;

	//读入静态障碍物（方）数据--添加始末点更改后的 极径，极角 更改
	for (UINT i = 0; i < 20; i++)
	{
		if (unknownor[i].CenterPm != 0)
		{
			UpdateData(TRUE);

			int num = LIST_UnknownOR.GetItemCount();

			CString stemp[7];
			stemp[0].Format("%d", num + 1);
			stemp[1].Format("%d", unknownor[i].CenterX);
			stemp[2].Format("%d", unknownor[i].CenterY);
			stemp[3].Format("%d", unknownor[i].HalfL);
			stemp[4].Format("%d", unknownor[i].HalfW);
			stemp[5].Format("%0.3f", unknownor[i].Da);
			stemp[6].Format("%0.3f", unknownor[i].V);
			LIST_UnknownOR.InsertItem(num, stemp[0]);		//插入行
			for (int j = 1; j < 7; j++)
			{
				LIST_UnknownOR.SetItemText(num, j, stemp[j]);					//设置该行的不同列的显示字符
			}
			UpdateData(false);
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void SetUnknownOR::OnBnClickedAddunknownor()
{
	// TODO:  在此添加控件通知处理程序代码

	UpdateData(TRUE);
	if (UnknownOR_HL != 0)
	{
		int num = LIST_UnknownOR.GetItemCount();
		unknownor[num].CenterX = UnknownOR_X;
		unknownor[num].CenterY = UnknownOR_Y;
		unknownor[num].HalfL = UnknownOR_HL;
		unknownor[num].HalfW = UnknownOR_HW;
		unknownor[num].Da = UnknownOR_DA;
		unknownor[num].V = UnknownOR_V;

		CalculateUnknownOR(num);//计算方形障碍物数据

		CString stemp[7];
		stemp[0].Format("%d", num + 1);
		stemp[1].Format("%d", UnknownOR_X);
		stemp[2].Format("%d", UnknownOR_Y);
		stemp[3].Format("%d", UnknownOR_HL);
		stemp[4].Format("%d", UnknownOR_HW);
		stemp[5].Format("%0.3f", UnknownOR_DA);
		stemp[6].Format("%0.3f", UnknownOR_V);

		LIST_UnknownOR.InsertItem(num, stemp[0]);		//插入行
		for (int i = 1; i<7; i++)
		{
			LIST_UnknownOR.SetItemText(num, i, stemp[i]);					//设置该行的不同列的显示字符
		}

		UnknownOR_X = 0;
		UnknownOR_Y = 0;
		UnknownOR_HL = 0;
		UnknownOR_HW = 0;
		UnknownOR_DA = 0;
		UnknownOR_V = 0;
	}

	UpdateData(false);
}

void SetUnknownOR::OnBnClickedDeleteunknownor()
{
	// TODO:  在此添加控件通知处理程序代码
	int num = LIST_UnknownOR.GetItemCount();
	while (LIST_UnknownOR.DeleteItem(num - 1));
	unknownor[num - 1] = { 0 };
}

void SetUnknownOR::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	testNum = 0;//测试次数
	thisPSOnum = 0;//本次pso测试次数

	int num = LIST_UnknownOR.GetItemCount();

	//对unknownor数组按中心点极径值  进行从小到大排序
	for (int j = 0; j < num - 1; j++)
	{
		for (int i = 0; i < num - 1 - j; i++)
		{
			UnknownOR temp;
			if (unknownor[i].CenterPm>unknownor[i + 1].CenterPm)
			{
				temp = unknownor[i];
				unknownor[i] = unknownor[i + 1];
				unknownor[i + 1] = temp;
			}
		}
	}

	if (unknownor[0].HalfL != 0)
	{
		state_unknownOR = 1;
	}

	CDialogEx::OnOK();
}

void SetUnknownOR::CalculateUnknownOR(int i)//计算方形障碍物数据
{
	float tempL = unknownor[i].V * unknown.UnknownT * drawscale * Kn_ms;
	SetStaticObstacleCircle temp;
	unknownor[i].CenterPm = sqrt(pow((float)abs(int(startgoalp.sx - unknownor[i].CenterX)), 2) + pow((float)abs(int(startgoalp.sy - unknownor[i].CenterY)), 2));//极径
	unknownor[i].CenterPa = temp.calculate_PolarAngle((float)unknownor[i].CenterX, (float)unknownor[i].CenterY);//极角

	float tempX, tempY;
	float tempPA = 0.0 - unknownor[i].Da * 2 * pi / 360;
	//局部坐标系中0的顶点(逆时针排序)
	tempX = (float)unknownor[i].HalfL;
	tempY = (float)unknownor[i].HalfW;
	//从局部坐标系转换到全局坐标系
	unknownor[i].Peak[0].x = (float)tempX * cos(tempPA) - (float)tempY * sin(tempPA) + (float)unknownor[i].CenterX;
	unknownor[i].Peak[0].y = (float)tempX * sin(tempPA) + (float)tempY * cos(tempPA) + (float)unknownor[i].CenterY;
	unknownor[i].Peak[0].Pm = sqrt(pow((float)abs((float)startgoalp.sx - unknownor[i].Peak[0].x), 2) + pow((float)abs((float)startgoalp.sy - unknownor[i].Peak[0].y), 2));//极径;
	unknownor[i].Peak[0].Pa = temp.calculate_PolarAngle(unknownor[i].Peak[0].x, unknownor[i].Peak[0].y);//极角

	//局部坐标系中0的顶点(扩展坐标)
	tempX = (float)unknownor[i].HalfL + tempL;
	tempY = (float)unknownor[i].HalfW + tempL;
	//从局部坐标系转换到全局坐标系
	unknownor[i].Peak[0].xEX = (float)tempX * cos(tempPA) - (float)tempY * sin(tempPA) + (float)unknownor[i].CenterX;
	unknownor[i].Peak[0].yEX = (float)tempX * sin(tempPA) + (float)tempY * cos(tempPA) + (float)unknownor[i].CenterY;
	unknownor[i].Peak[0].PmEX = sqrt(pow((float)abs((float)startgoalp.sx - unknownor[i].Peak[0].xEX), 2) + pow((float)abs((float)startgoalp.sy - unknownor[i].Peak[0].yEX), 2));//极径;
	unknownor[i].Peak[0].PaEX = temp.calculate_PolarAngle(unknownor[i].Peak[0].xEX, unknownor[i].Peak[0].yEX);//极角

	//局部坐标系中1的顶点
	tempX = (float)unknownor[i].HalfL;
	tempY = 0.0 - (float)unknownor[i].HalfW;
	//从局部坐标系转换到全局坐标系
	unknownor[i].Peak[1].x = (float)tempX * cos(tempPA) - (float)tempY * sin(tempPA) + (float)unknownor[i].CenterX;
	unknownor[i].Peak[1].y = (float)tempX * sin(tempPA) + (float)tempY * cos(tempPA) + (float)unknownor[i].CenterY;
	unknownor[i].Peak[1].Pm = sqrt(pow((float)abs((float)startgoalp.sx - unknownor[i].Peak[1].x), 2) + pow((float)abs((float)startgoalp.sy - unknownor[i].Peak[1].y), 2));//极径;
	unknownor[i].Peak[1].Pa = temp.calculate_PolarAngle(unknownor[i].Peak[1].x, unknownor[i].Peak[1].y);//极角

	//局部坐标系中1的顶点(扩展坐标)
	tempX = (float)unknownor[i].HalfL + tempL;
	tempY = 0.0 - (float)unknownor[i].HalfW - tempL;
	//从局部坐标系转换到全局坐标系
	unknownor[i].Peak[1].xEX = (float)tempX * cos(tempPA) - (float)tempY * sin(tempPA) + (float)unknownor[i].CenterX;
	unknownor[i].Peak[1].yEX = (float)tempX * sin(tempPA) + (float)tempY * cos(tempPA) + (float)unknownor[i].CenterY;
	unknownor[i].Peak[1].PmEX = sqrt(pow((float)abs((float)startgoalp.sx - unknownor[i].Peak[1].xEX), 2) + pow((float)abs((float)startgoalp.sy - unknownor[i].Peak[1].yEX), 2));//极径;
	unknownor[i].Peak[1].PaEX = temp.calculate_PolarAngle(unknownor[i].Peak[1].xEX, unknownor[i].Peak[1].yEX);//极角

	//局部坐标系中2的顶点
	tempX = 0.0 - (float)unknownor[i].HalfL;
	tempY = 0.0 - (float)unknownor[i].HalfW;
	//从局部坐标系转换到全局坐标系
	unknownor[i].Peak[2].x = (float)tempX * cos(tempPA) - (float)tempY * sin(tempPA) + (float)unknownor[i].CenterX;
	unknownor[i].Peak[2].y = (float)tempX * sin(tempPA) + (float)tempY * cos(tempPA) + (float)unknownor[i].CenterY;
	unknownor[i].Peak[2].Pm = sqrt(pow((float)abs((float)startgoalp.sx - unknownor[i].Peak[2].x), 2) + pow((float)abs((float)startgoalp.sy - unknownor[i].Peak[2].y), 2));//极径;
	unknownor[i].Peak[2].Pa = temp.calculate_PolarAngle(unknownor[i].Peak[2].x, unknownor[i].Peak[2].y);//极角

	//局部坐标系中2的顶点(扩展坐标)
	tempX = 0.0 - (float)unknownor[i].HalfL - tempL;
	tempY = 0.0 - (float)unknownor[i].HalfW - tempL;
	//从局部坐标系转换到全局坐标系
	unknownor[i].Peak[2].xEX = (float)tempX * cos(tempPA) - (float)tempY * sin(tempPA) + (float)unknownor[i].CenterX;
	unknownor[i].Peak[2].yEX = (float)tempX * sin(tempPA) + (float)tempY * cos(tempPA) + (float)unknownor[i].CenterY;
	unknownor[i].Peak[2].PmEX = sqrt(pow((float)abs((float)startgoalp.sx - unknownor[i].Peak[2].xEX), 2) + pow((float)abs((float)startgoalp.sy - unknownor[i].Peak[2].yEX), 2));//极径;
	unknownor[i].Peak[2].PaEX = temp.calculate_PolarAngle(unknownor[i].Peak[2].xEX, unknownor[i].Peak[2].yEX);//极角

	//局部坐标系中3的顶点
	tempX = 0.0 - (float)unknownor[i].HalfL;
	tempY = (float)unknownor[i].HalfW;
	//从局部坐标系转换到全局坐标系
	unknownor[i].Peak[3].x = (float)tempX * cos(tempPA) - (float)tempY * sin(tempPA) + (float)unknownor[i].CenterX;
	unknownor[i].Peak[3].y = (float)tempX * sin(tempPA) + (float)tempY * cos(tempPA) + (float)unknownor[i].CenterY;
	unknownor[i].Peak[3].Pm = sqrt(pow((float)abs((float)startgoalp.sx - unknownor[i].Peak[3].x), 2) + pow((float)abs((float)startgoalp.sy - unknownor[i].Peak[3].y), 2));//极径;
	unknownor[i].Peak[3].Pa = temp.calculate_PolarAngle(unknownor[i].Peak[3].x, unknownor[i].Peak[3].y);//极角

	//局部坐标系中3的顶点(扩展坐标)
	tempX = 0.0 - (float)unknownor[i].HalfL - tempL;
	tempY = (float)unknownor[i].HalfW + tempL;
	//从局部坐标系转换到全局坐标系
	unknownor[i].Peak[3].xEX = (float)tempX * cos(tempPA) - (float)tempY * sin(tempPA) + (float)unknownor[i].CenterX;
	unknownor[i].Peak[3].yEX = (float)tempX * sin(tempPA) + (float)tempY * cos(tempPA) + (float)unknownor[i].CenterY;
	unknownor[i].Peak[3].PmEX = sqrt(pow((float)abs((float)startgoalp.sx - unknownor[i].Peak[3].xEX), 2) + pow((float)abs((float)startgoalp.sy - unknownor[i].Peak[3].yEX), 2));//极径;
	unknownor[i].Peak[3].PaEX = temp.calculate_PolarAngle(unknownor[i].Peak[3].xEX, unknownor[i].Peak[3].yEX);//极角

	//对四个顶点按pm进行排序
	//找出最小值的下标
	float min = 0.0;
	int flag = 0;
	for (int m = 0; m < 4; m++)
	{
		if (m == 0)
		{
			min = unknownor[i].Peak[0].Pm;
			flag = 0;
		}
		else
		{
			if (min>unknownor[i].Peak[m].Pm)
			{
				min = unknownor[i].Peak[m].Pm;
				flag = m;
			}
		}
	}
	//进行排序
	REC tempPeak;
	if (flag == 0)
	{
	}
	else if (flag == 1)
	{
		tempPeak = unknownor[i].Peak[0];
		unknownor[i].Peak[0] = unknownor[i].Peak[1];
		unknownor[i].Peak[1] = unknownor[i].Peak[2];
		unknownor[i].Peak[2] = unknownor[i].Peak[3];
		unknownor[i].Peak[3] = tempPeak;
	}
	else if (flag == 2)
	{
		tempPeak = unknownor[i].Peak[0];
		unknownor[i].Peak[0] = unknownor[i].Peak[2];
		unknownor[i].Peak[2] = tempPeak;
		tempPeak = unknownor[i].Peak[3];
		unknownor[i].Peak[3] = unknownor[i].Peak[1];
		unknownor[i].Peak[1] = tempPeak;
	}
	else if (flag == 3)
	{
		tempPeak = unknownor[i].Peak[0];
		unknownor[i].Peak[0] = unknownor[i].Peak[3];
		unknownor[i].Peak[3] = unknownor[i].Peak[2];
		unknownor[i].Peak[2] = unknownor[i].Peak[1];
		unknownor[i].Peak[1] = tempPeak;
	}

	//更改末端扩展点
	float k1, k2;
	if ((0 < unknownor[i].Da) && (unknownor[i].Da <90))
	{
		k1 = (unknownor[i].Peak[3].yEX - unknownor[i].Peak[0].yEX) / (unknownor[i].Peak[3].xEX - unknownor[i].Peak[0].xEX);
		unknownor[i].Peak[0].xEX = (k1*(k1*unknownor[i].Peak[0].xEX - unknownor[i].Peak[0].yEX) + unknownor[i].Peak[0].x + k1*unknownor[i].Peak[0].y) / (1 + k1*k1);
		unknownor[i].Peak[0].yEX = (-1.0/k1) * (unknownor[i].Peak[0].xEX - unknownor[i].Peak[0].x) + unknownor[i].Peak[0].y;
		unknownor[i].Peak[0].PmEX = sqrt(pow((float)abs((float)startgoalp.sx - unknownor[i].Peak[0].xEX), 2) + pow((float)abs((float)startgoalp.sy - unknownor[i].Peak[0].yEX), 2));//极径;
		unknownor[i].Peak[0].PaEX = temp.calculate_PolarAngle(unknownor[i].Peak[0].xEX, unknownor[i].Peak[0].yEX);//极角

		k2 = (unknownor[i].Peak[1].yEX - unknownor[i].Peak[2].yEX) / (unknownor[i].Peak[1].xEX - unknownor[i].Peak[2].xEX);
		unknownor[i].Peak[1].xEX = (k2*(k2*unknownor[i].Peak[1].xEX - unknownor[i].Peak[1].yEX) + unknownor[i].Peak[1].x + k1*unknownor[i].Peak[1].y) / (1 + k2*k2);
		unknownor[i].Peak[1].yEX = (-1.0 / k2) * (unknownor[i].Peak[1].xEX - unknownor[i].Peak[1].x) + unknownor[i].Peak[1].y;
		unknownor[i].Peak[1].PmEX = sqrt(pow((float)abs((float)startgoalp.sx - unknownor[i].Peak[1].xEX), 2) + pow((float)abs((float)startgoalp.sy - unknownor[i].Peak[1].yEX), 2));//极径;
		unknownor[i].Peak[1].PaEX = temp.calculate_PolarAngle(unknownor[i].Peak[1].xEX, unknownor[i].Peak[1].yEX);//极角
	}
	else if (unknownor[i].Da ==0)
	{
		if (unknownor[i].CenterY < startgoalp.sy)
		{
			unknownor[i].Peak[0].xEX = unknownor[i].Peak[0].x;
			unknownor[i].Peak[0].PmEX = sqrt(pow((float)abs((float)startgoalp.sx - unknownor[i].Peak[0].xEX), 2) + pow((float)abs((float)startgoalp.sy - unknownor[i].Peak[0].yEX), 2));//极径;
			unknownor[i].Peak[0].PaEX = temp.calculate_PolarAngle(unknownor[i].Peak[0].xEX, unknownor[i].Peak[0].yEX);//极角

			unknownor[i].Peak[3].xEX = unknownor[i].Peak[3].x;
			unknownor[i].Peak[3].PmEX = sqrt(pow((float)abs((float)startgoalp.sx - unknownor[i].Peak[3].xEX), 2) + pow((float)abs((float)startgoalp.sy - unknownor[i].Peak[3].yEX), 2));//极径;
			unknownor[i].Peak[3].PaEX = temp.calculate_PolarAngle(unknownor[i].Peak[3].xEX, unknownor[i].Peak[3].yEX);//极角
		}
		else
		{
			unknownor[i].Peak[0].xEX = unknownor[i].Peak[0].x;
			unknownor[i].Peak[0].PmEX = sqrt(pow((float)abs((float)startgoalp.sx - unknownor[i].Peak[0].xEX), 2) + pow((float)abs((float)startgoalp.sy - unknownor[i].Peak[0].yEX), 2));//极径;
			unknownor[i].Peak[0].PaEX = temp.calculate_PolarAngle(unknownor[i].Peak[0].xEX, unknownor[i].Peak[0].yEX);//极角

			unknownor[i].Peak[1].xEX = unknownor[i].Peak[1].x;
			unknownor[i].Peak[1].PmEX = sqrt(pow((float)abs((float)startgoalp.sx - unknownor[i].Peak[1].xEX), 2) + pow((float)abs((float)startgoalp.sy - unknownor[i].Peak[1].yEX), 2));//极径;
			unknownor[i].Peak[1].PaEX = temp.calculate_PolarAngle(unknownor[i].Peak[1].xEX, unknownor[i].Peak[1].yEX);//极角
		}		
	}
	else if ((90 < unknownor[i].Da) && (unknownor[i].Da <180))
	{
		k1 = (unknownor[i].Peak[3].yEX - unknownor[i].Peak[2].yEX) / (unknownor[i].Peak[3].xEX - unknownor[i].Peak[2].xEX);
		unknownor[i].Peak[2].xEX = (k1*(k1*unknownor[i].Peak[2].xEX - unknownor[i].Peak[2].yEX) + unknownor[i].Peak[2].x + k1*unknownor[i].Peak[2].y) / (1 + k1*k1);
		unknownor[i].Peak[2].yEX = (-1.0 / k1) * (unknownor[i].Peak[2].xEX - unknownor[i].Peak[2].x) + unknownor[i].Peak[2].y;
		unknownor[i].Peak[2].PmEX = sqrt(pow((float)abs((float)startgoalp.sx - unknownor[i].Peak[2].xEX), 2) + pow((float)abs((float)startgoalp.sy - unknownor[i].Peak[2].yEX), 2));//极径;
		unknownor[i].Peak[2].PaEX = temp.calculate_PolarAngle(unknownor[i].Peak[2].xEX, unknownor[i].Peak[2].yEX);//极角

		k2 = (unknownor[i].Peak[1].yEX - unknownor[i].Peak[0].yEX) / (unknownor[i].Peak[1].xEX - unknownor[i].Peak[0].xEX);
		unknownor[i].Peak[1].xEX = (k2*(k2*unknownor[i].Peak[1].xEX - unknownor[i].Peak[1].yEX) + unknownor[i].Peak[1].x + k1*unknownor[i].Peak[1].y) / (1 + k2*k2);
		unknownor[i].Peak[1].yEX = (-1.0 / k2) * (unknownor[i].Peak[1].xEX - unknownor[i].Peak[1].x) + unknownor[i].Peak[1].y;
		unknownor[i].Peak[1].PmEX = sqrt(pow((float)abs((float)startgoalp.sx - unknownor[i].Peak[1].xEX), 2) + pow((float)abs((float)startgoalp.sy - unknownor[i].Peak[1].yEX), 2));//极径;
		unknownor[i].Peak[1].PaEX = temp.calculate_PolarAngle(unknownor[i].Peak[1].xEX, unknownor[i].Peak[1].yEX);//极角
	}
	else if (unknownor[i].Da == 90)
	{
		if (unknownor[i].CenterY < startgoalp.sy)
		{
			unknownor[i].Peak[0].yEX = unknownor[i].Peak[0].y;
			unknownor[i].Peak[0].PmEX = sqrt(pow((float)abs((float)startgoalp.sx - unknownor[i].Peak[0].xEX), 2) + pow((float)abs((float)startgoalp.sy - unknownor[i].Peak[0].yEX), 2));//极径;
			unknownor[i].Peak[0].PaEX = temp.calculate_PolarAngle(unknownor[i].Peak[0].xEX, unknownor[i].Peak[0].yEX);//极角

			unknownor[i].Peak[1].yEX = unknownor[i].Peak[1].y;
			unknownor[i].Peak[1].PmEX = sqrt(pow((float)abs((float)startgoalp.sx - unknownor[i].Peak[1].xEX), 2) + pow((float)abs((float)startgoalp.sy - unknownor[i].Peak[1].yEX), 2));//极径;
			unknownor[i].Peak[1].PaEX = temp.calculate_PolarAngle(unknownor[i].Peak[1].xEX, unknownor[i].Peak[1].yEX);//极角
		}
		else
		{
			unknownor[i].Peak[2].yEX = unknownor[i].Peak[2].y;
			unknownor[i].Peak[2].PmEX = sqrt(pow((float)abs((float)startgoalp.sx - unknownor[i].Peak[2].xEX), 2) + pow((float)abs((float)startgoalp.sy - unknownor[i].Peak[2].yEX), 2));//极径;
			unknownor[i].Peak[2].PaEX = temp.calculate_PolarAngle(unknownor[i].Peak[2].xEX, unknownor[i].Peak[2].yEX);//极角

			unknownor[i].Peak[1].yEX = unknownor[i].Peak[1].y;
			unknownor[i].Peak[1].PmEX = sqrt(pow((float)abs((float)startgoalp.sx - unknownor[i].Peak[1].xEX), 2) + pow((float)abs((float)startgoalp.sy - unknownor[i].Peak[1].yEX), 2));//极径;
			unknownor[i].Peak[1].PaEX = temp.calculate_PolarAngle(unknownor[i].Peak[1].xEX, unknownor[i].Peak[1].yEX);//极角
		}		
	}
	else if ((180 < unknownor[i].Da) && (unknownor[i].Da <270))
	{
		k1 = (unknownor[i].Peak[1].yEX - unknownor[i].Peak[2].yEX) / (unknownor[i].Peak[1].xEX - unknownor[i].Peak[2].xEX);
		unknownor[i].Peak[2].xEX = (k1*(k1*unknownor[i].Peak[2].xEX - unknownor[i].Peak[2].yEX) + unknownor[i].Peak[2].x + k1*unknownor[i].Peak[2].y) / (1 + k1*k1);
		unknownor[i].Peak[2].yEX = (-1.0 / k1) * (unknownor[i].Peak[2].xEX - unknownor[i].Peak[2].x) + unknownor[i].Peak[2].y;
		unknownor[i].Peak[2].PmEX = sqrt(pow((float)abs((float)startgoalp.sx - unknownor[i].Peak[2].xEX), 2) + pow((float)abs((float)startgoalp.sy - unknownor[i].Peak[2].yEX), 2));//极径;
		unknownor[i].Peak[2].PaEX = temp.calculate_PolarAngle(unknownor[i].Peak[2].xEX, unknownor[i].Peak[2].yEX);//极角

		k2 = (unknownor[i].Peak[3].yEX - unknownor[i].Peak[0].yEX) / (unknownor[i].Peak[3].xEX - unknownor[i].Peak[0].xEX);
		unknownor[i].Peak[3].xEX = (k2*(k2*unknownor[i].Peak[3].xEX - unknownor[i].Peak[3].yEX) + unknownor[i].Peak[3].x + k1*unknownor[i].Peak[3].y) / (1 + k2*k2);
		unknownor[i].Peak[3].yEX = (-1.0 / k2) * (unknownor[i].Peak[3].xEX - unknownor[i].Peak[3].x) + unknownor[i].Peak[3].y;
		unknownor[i].Peak[3].PmEX = sqrt(pow((float)abs((float)startgoalp.sx - unknownor[i].Peak[3].xEX), 2) + pow((float)abs((float)startgoalp.sy - unknownor[i].Peak[3].yEX), 2));//极径;
		unknownor[i].Peak[3].PaEX = temp.calculate_PolarAngle(unknownor[i].Peak[3].xEX, unknownor[i].Peak[3].yEX);//极角
	}
	else if (unknownor[i].Da == 180)
	{
		if (unknownor[i].CenterY < startgoalp.sy)
		{
			unknownor[i].Peak[2].xEX = unknownor[i].Peak[2].x;
			unknownor[i].Peak[2].PmEX = sqrt(pow((float)abs((float)startgoalp.sx - unknownor[i].Peak[2].xEX), 2) + pow((float)abs((float)startgoalp.sy - unknownor[i].Peak[2].yEX), 2));//极径;
			unknownor[i].Peak[2].PaEX = temp.calculate_PolarAngle(unknownor[i].Peak[2].xEX, unknownor[i].Peak[2].yEX);//极角

			unknownor[i].Peak[1].xEX = unknownor[i].Peak[1].x;
			unknownor[i].Peak[1].PmEX = sqrt(pow((float)abs((float)startgoalp.sx - unknownor[i].Peak[1].xEX), 2) + pow((float)abs((float)startgoalp.sy - unknownor[i].Peak[1].yEX), 2));//极径;
			unknownor[i].Peak[1].PaEX = temp.calculate_PolarAngle(unknownor[i].Peak[1].xEX, unknownor[i].Peak[1].yEX);//极角
		}
		else
		{
			unknownor[i].Peak[2].xEX = unknownor[i].Peak[2].x;
			unknownor[i].Peak[2].PmEX = sqrt(pow((float)abs((float)startgoalp.sx - unknownor[i].Peak[2].xEX), 2) + pow((float)abs((float)startgoalp.sy - unknownor[i].Peak[2].yEX), 2));//极径;
			unknownor[i].Peak[2].PaEX = temp.calculate_PolarAngle(unknownor[i].Peak[2].xEX, unknownor[i].Peak[2].yEX);//极角

			unknownor[i].Peak[3].xEX = unknownor[i].Peak[3].x;
			unknownor[i].Peak[3].PmEX = sqrt(pow((float)abs((float)startgoalp.sx - unknownor[i].Peak[3].xEX), 2) + pow((float)abs((float)startgoalp.sy - unknownor[i].Peak[3].yEX), 2));//极径;
			unknownor[i].Peak[3].PaEX = temp.calculate_PolarAngle(unknownor[i].Peak[3].xEX, unknownor[i].Peak[3].yEX);//极角
		}		
	}
	else if ((270 < unknownor[i].Da) && (unknownor[i].Da < 360))
	{
		k1 = (unknownor[i].Peak[1].yEX - unknownor[i].Peak[0].yEX) / (unknownor[i].Peak[1].xEX - unknownor[i].Peak[0].xEX);
		unknownor[i].Peak[0].xEX = (k1*(k1*unknownor[i].Peak[0].xEX - unknownor[i].Peak[0].yEX) + unknownor[i].Peak[0].x + k1*unknownor[i].Peak[0].y) / (1 + k1*k1);
		unknownor[i].Peak[0].yEX = (-1.0 / k1) * (unknownor[i].Peak[0].xEX - unknownor[i].Peak[0].x) + unknownor[i].Peak[0].y;
		unknownor[i].Peak[0].PmEX = sqrt(pow((float)abs((float)startgoalp.sx - unknownor[i].Peak[0].xEX), 2) + pow((float)abs((float)startgoalp.sy - unknownor[i].Peak[0].yEX), 2));//极径;
		unknownor[i].Peak[0].PaEX = temp.calculate_PolarAngle(unknownor[i].Peak[0].xEX, unknownor[i].Peak[0].yEX);//极角

		k2 = (unknownor[i].Peak[3].yEX - unknownor[i].Peak[2].yEX) / (unknownor[i].Peak[3].xEX - unknownor[i].Peak[2].xEX);
		unknownor[i].Peak[3].xEX = (k2*(k2*unknownor[i].Peak[3].xEX - unknownor[i].Peak[3].yEX) + unknownor[i].Peak[3].x + k1*unknownor[i].Peak[3].y) / (1 + k2*k2);
		unknownor[i].Peak[3].yEX = (-1.0 / k2) * (unknownor[i].Peak[3].xEX - unknownor[i].Peak[3].x) + unknownor[i].Peak[3].y;
		unknownor[i].Peak[3].PmEX = sqrt(pow((float)abs((float)startgoalp.sx - unknownor[i].Peak[3].xEX), 2) + pow((float)abs((float)startgoalp.sy - unknownor[i].Peak[3].yEX), 2));//极径;
		unknownor[i].Peak[3].PaEX = temp.calculate_PolarAngle(unknownor[i].Peak[3].xEX, unknownor[i].Peak[3].yEX);//极角
	}
	else if (unknownor[i].Da == 270)
	{
		if (unknownor[i].CenterY < startgoalp.sy)
		{
			unknownor[i].Peak[2].yEX = unknownor[i].Peak[2].y;
			unknownor[i].Peak[2].PmEX = sqrt(pow((float)abs((float)startgoalp.sx - unknownor[i].Peak[2].xEX), 2) + pow((float)abs((float)startgoalp.sy - unknownor[i].Peak[2].yEX), 2));//极径;
			unknownor[i].Peak[2].PaEX = temp.calculate_PolarAngle(unknownor[i].Peak[2].xEX, unknownor[i].Peak[2].yEX);//极角

			unknownor[i].Peak[3].yEX = unknownor[i].Peak[3].y;
			unknownor[i].Peak[3].PmEX = sqrt(pow((float)abs((float)startgoalp.sx - unknownor[i].Peak[3].xEX), 2) + pow((float)abs((float)startgoalp.sy - unknownor[i].Peak[3].yEX), 2));//极径;
			unknownor[i].Peak[3].PaEX = temp.calculate_PolarAngle(unknownor[i].Peak[3].xEX, unknownor[i].Peak[3].yEX);//极角
		}
		else
		{
			unknownor[i].Peak[0].yEX = unknownor[i].Peak[0].y;
			unknownor[i].Peak[0].PmEX = sqrt(pow((float)abs((float)startgoalp.sx - unknownor[i].Peak[0].xEX), 2) + pow((float)abs((float)startgoalp.sy - unknownor[i].Peak[0].yEX), 2));//极径;
			unknownor[i].Peak[0].PaEX = temp.calculate_PolarAngle(unknownor[i].Peak[0].xEX, unknownor[i].Peak[0].yEX);//极角

			unknownor[i].Peak[3].yEX = unknownor[i].Peak[3].y;
			unknownor[i].Peak[3].PmEX = sqrt(pow((float)abs((float)startgoalp.sx - unknownor[i].Peak[3].xEX), 2) + pow((float)abs((float)startgoalp.sy - unknownor[i].Peak[3].yEX), 2));//极径;
			unknownor[i].Peak[3].PaEX = temp.calculate_PolarAngle(unknownor[i].Peak[3].xEX, unknownor[i].Peak[3].yEX);//极角
		}		
	}
}


void SetUnknownOR::OnBnClickedSunknownorinput()
{
	// TODO:  在此添加控件通知处理程序代码
	CFileDialog dlg(TRUE, //TRUE或FALSE。TRUE为打开文件；FALSE为保存文件
		"xls", //为缺省的扩展名
		"FileList", //为显示在文件名组合框的编辑框的文件名，一般可选NULL 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,//为对话框风格，一般为OFN_HIDEREADONLY   |   OFN_OVERWRITEPROMPT,即隐藏只读选项和覆盖已有文件前提示。 
		"Excel 文件(*.xls)|*.xls||"//为下拉列表枢中显示文件类型
		);
	dlg.m_ofn.lpstrTitle = "导入未知障碍物（方）数据";

	if (dlg.DoModal() != IDOK)
		return;
	CString strFilePath;
	//获得文件路径名
	strFilePath = dlg.GetPathName();
	//判断文件是否已经存在，存在则打开文件
	DWORD dwRe = GetFileAttributes(strFilePath);
	if (dwRe != (DWORD)-1)
	{
		//ShellExecute(NULL, NULL, strFilePath, NULL, NULL, SW_RESTORE); 
	}
	else return;

	CDatabase db;//数据库库需要包含头文件 #include <afxdb.h>
	CString sDriver = "MICROSOFT EXCEL DRIVER (*.XLS)"; // Excel驱动
	CString sSql, arr[7];

	sSql.Format("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s", sDriver, strFilePath, strFilePath);
	if (!db.OpenEx(sSql, CDatabase::noOdbcDialog))//连接数据源DJB．xls
	{
		MessageBox("打开EXCEL文件失败!", "错误");
		return;
	}
	//打开EXCEL表
	CRecordset pset(&db);
	LIST_UnknownOR.DeleteAllItems();

	//AfxMessageBox("OK");
	/* sSql = "SELECT 学号,姓名,成绩 "
	"FROM EXCELDEMO";      */
	// "ORDER BY 姓名";
	sSql.Format("SELECT 序号,中心点X, 中心点Y, 半长, 半宽, 方向角, 速度 FROM 未知障碍物（方）数据");
	pset.Open(CRecordset::forwardOnly, sSql, CRecordset::readOnly);
	while (!pset.IsEOF())
	{
		pset.GetFieldValue("序号", arr[0]);//前面字段必须与表中的相同，否则出错。
		pset.GetFieldValue("中心点X", arr[1]);
		pset.GetFieldValue("中心点Y", arr[2]);
		pset.GetFieldValue("半长", arr[3]);
		pset.GetFieldValue("半宽", arr[4]);
		pset.GetFieldValue("方向角", arr[5]);
		pset.GetFieldValue("速度", arr[6]);

		int count = LIST_UnknownOR.GetItemCount();//插入到ListCtrl中
		LIST_UnknownOR.InsertItem(count, arr[0]);
		LIST_UnknownOR.SetItemText(count, 1, arr[1]);
		LIST_UnknownOR.SetItemText(count, 2, arr[2]);
		LIST_UnknownOR.SetItemText(count, 3, arr[3]);
		LIST_UnknownOR.SetItemText(count, 4, arr[4]);
		LIST_UnknownOR.SetItemText(count, 5, arr[5]);
		LIST_UnknownOR.SetItemText(count, 6, arr[6]);

		pset.MoveNext();
	}
	db.Close();

	int num = LIST_UnknownOR.GetItemCount();

	for (int i = 0; i < 20; i++)
	{
		unknownor[i] = { 0 };//清零
	}
	//把列表数据添加进数组unknownor[]
	for (int i = 0; i < num; i++)
	{
		unknownor[i].CenterX = atoi(LIST_UnknownOR.GetItemText(i, 1));
		unknownor[i].CenterY = atoi(LIST_UnknownOR.GetItemText(i, 2));
		unknownor[i].HalfL = atoi(LIST_UnknownOR.GetItemText(i, 3));
		unknownor[i].HalfW = atoi(LIST_UnknownOR.GetItemText(i, 4));
		unknownor[i].Da = atof(LIST_UnknownOR.GetItemText(i, 5));
		unknownor[i].V = atof(LIST_UnknownOR.GetItemText(i, 6));
		CalculateUnknownOR(i);//计算方形障碍物数据
	}

	MessageBox("Excel数据成功导入系统!", "导入成功");
}

void SetUnknownOR::OnBnClickedSunknownoroutput()
{
	// TODO:  在此添加控件通知处理程序代码
	if (LIST_UnknownOR.GetItemCount() <= 0)
	{
		AfxMessageBox("列表中没有记录需要保存！");
		return;
	}
	//打开另存为对话框 ，需要包含 #include <Afxdlgs.h>
	CFileDialog dlg(FALSE,
		"xls",
		"FileList",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		"Excel 文件(*.xls)|*.xls||");
	dlg.m_ofn.lpstrTitle = "USV未知障碍物（方）数据另存为";

	if (dlg.DoModal() != IDOK)
		return;
	CString strFilePath;
	//获得文件路径名
	strFilePath = dlg.GetPathName();
	//判断文件是否已经存在，存在则删除重建
	DWORD dwRe = GetFileAttributes(strFilePath);
	if (dwRe != (DWORD)-1)
	{
		DeleteFile(strFilePath);
	}

	CDatabase database;//数据库库需要包含头文件 #include <afxdb.h>
	CString sDriver = "MICROSOFT EXCEL DRIVER (*.XLS)"; // Excel驱动
	CString sSql, strInsert;

	TRY
	{
		// 创建进行存取的字符串
		sSql.Format("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s", sDriver, strFilePath, strFilePath);

		// 创建数据库 (既Excel表格文件)
		if (database.OpenEx(sSql, CDatabase::noOdbcDialog))
		{
			//获得列别框总列数
			int iColumnNum, iRowCount;
			LVCOLUMN lvCol;
			CString strColName; //用于保存列标题名称
			int i, j; //列、行循环参数

			iColumnNum = LIST_UnknownOR.GetHeaderCtrl()->GetItemCount();
			iRowCount = LIST_UnknownOR.GetItemCount();

			sSql = " CREATE TABLE 未知障碍物（方）数据 ( ";
			strInsert = " INSERT INTO 未知障碍物（方）数据 ( ";
			//获得列标题名称
			lvCol.mask = LVCF_TEXT; //必需设置，说明LVCOLUMN变量中pszText参数有效
			lvCol.cchTextMax = 32; //必设，pszText参数所指向的字符串的大小
			lvCol.pszText = strColName.GetBuffer(32); //必设，pszText 所指向的字符串的实际存储位置。
			//以上三个参数设置后才能通过 GetColumn()函数获得列标题的名称
			for (i = 0; i< iColumnNum; i++)
			{
				if (!(LIST_UnknownOR.GetColumn(i, &lvCol)))
					return;
				if (i<iColumnNum - 1)
				{
					sSql = sSql + lvCol.pszText + " TEXT , ";
					strInsert = strInsert + lvCol.pszText + " , ";
				}
				else
				{
					sSql = sSql + lvCol.pszText + " TEXT ) ";
					strInsert = strInsert + lvCol.pszText + " )  VALUES ( ";
				}
			}
			//创建Excel表格文件
			database.ExecuteSQL(sSql);

			//循环提取记录并插入到EXCEL中
			sSql = strInsert;
			char chTemp[33];
			for (j = 0; j<iRowCount; j++)
			{
				memset(chTemp, 0, 33);
				for (i = 0; i<iColumnNum; i++)
				{
					LIST_UnknownOR.GetItemText(j, i, chTemp, 33);
					if (i < (iColumnNum - 1))
					{
						sSql = sSql + "'" + chTemp + "' , ";
					}
					else
					{
						sSql = sSql + "'" + chTemp + "' ) ";
					}
				}
				//将记录插入到表格中
				database.ExecuteSQL(sSql);
				sSql = strInsert;
			}

			sSql = " CREATE TABLE 始末点 ( 起点X TEXT , 起点Y TEXT , 终点X TEXT , 终点Y TEXT ) ";
			//创建Excel表格文件
			database.ExecuteSQL(sSql);

			CString stemp[4];
			stemp[0].Format("%d", startgoalp.sx);
			stemp[1].Format("%d", startgoalp.sy);
			stemp[2].Format("%d", startgoalp.gx);
			stemp[3].Format("%d", startgoalp.gy);
			strInsert = " INSERT INTO 始末点 (起点X , 起点Y , 终点X , 终点Y )  VALUES ( ";
			strInsert = strInsert + "'" + stemp[0] + "' , " + "'" + stemp[1] + "' , " + "'" + stemp[2] + "' , " + "'" + stemp[3] + "') ";
			database.ExecuteSQL(strInsert);
		}

		// 关闭Excel表格文件
		database.Close();

		AfxMessageBox("保存未知障碍物（方）数据为Excel文件成功！");
	}
	CATCH_ALL(e)
	{
		//错误类型很多，根据需要进行报错。
		AfxMessageBox("Excel文件保存失败。");
	}
	END_CATCH_ALL;
}

PointInRC SetUnknownOR::calculate_pointInDRC(int m, float x, float y)//计算点在矩形的那个位置（共9个）
{
	float y01, y12, y23, y30, tempX1, tempX2, tempY1, tempY2;
	PointInRC pointInRC;

	//显示坐标系下计算，返回极坐标系下值
	//01
	tempX1 = unknownor[m].Peak[0].xEX;
	tempX2 = unknownor[m].Peak[1].xEX;
	tempY1 = unknownor[m].Peak[0].yEX;
	tempY2 = unknownor[m].Peak[1].yEX;
	if ((tempX1 - tempX2) == 0)
	{
		y01 = x - tempX1;
	}
	else
	{
		y01 = (tempY1 - tempY2) / (tempX1 - tempX2) * (x - tempX1) + tempY1 - y;
	}

	//12
	tempX1 = unknownor[m].Peak[1].xEX;
	tempX2 = unknownor[m].Peak[2].xEX;
	tempY1 = unknownor[m].Peak[1].yEX;
	tempY2 = unknownor[m].Peak[2].yEX;
	if ((tempX1 - tempX2) == 0)
	{
		y12 = x - tempX1;
	}
	else
	{
		y12 = (tempY1 - tempY2) / (tempX1 - tempX2) * (x - tempX1) + tempY1 - y;
	}

	//23
	tempX1 = unknownor[m].Peak[2].xEX;
	tempX2 = unknownor[m].Peak[3].xEX;
	tempY1 = unknownor[m].Peak[2].yEX;
	tempY2 = unknownor[m].Peak[3].yEX;
	if ((tempX1 - tempX2) == 0)
	{
		y23 = x - tempX1;
	}
	else
	{
		y23 = (tempY1 - tempY2) / (tempX1 - tempX2) * (x - tempX1) + tempY1 - y;
	}

	//30
	tempX1 = unknownor[m].Peak[3].xEX;
	tempX2 = unknownor[m].Peak[0].xEX;
	tempY1 = unknownor[m].Peak[3].yEX;
	tempY2 = unknownor[m].Peak[0].yEX;
	if ((tempX1 - tempX2) == 0)
	{
		y30 = x - tempX1;
	}
	else
	{
		y30 = (tempY1 - tempY2) / (tempX1 - tempX2) * (x - tempX1) + tempY1 - y;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////

	if ((y01 >= 0) & (y12 >= 0) & (y30 <= 0) & (y23 <= 0))
	{
		pointInRC.point = 5;//在矩形内部
	}
	else if ((y01 < 0) & (y30 > 0))
	{
		pointInRC.point = 0;
		pointInRC.min = 1;
		pointInRC.max = 3;
	}
	else if ((y01 < 0) & (y12 < 0))
	{
		pointInRC.point = 1;
		pointInRC.min = 2;
		pointInRC.max = 0;
	}
	else if ((y12 < 0) & (y23 > 0))
	{
		pointInRC.point = 2;
		pointInRC.min = 3;
		pointInRC.max = 1;
	}
	else if ((y23 > 0) & (y30 > 0))
	{
		pointInRC.point = 3;
		pointInRC.min = 0;
		pointInRC.max = 2;
	}
	else if ((y01 < 0) & (y12 >= 0) & (y30 <= 0))
	{
		pointInRC.point = 10;
		pointInRC.min = 1;
		pointInRC.max = 0;
	}
	else if ((y01 >= 0) & (y12 < 0) & (y23 <= 0))
	{
		pointInRC.point = 12;
		pointInRC.min = 2;
		pointInRC.max = 1;
	}
	else if ((y12 >= 0) & (y23 > 0) & (y30 <= 0))
	{
		pointInRC.point = 23;
		pointInRC.min = 3;
		pointInRC.max = 2;
	}
	else if ((y01 >= 0) & (y23 <= 0) & (y30 > 0))
	{
		pointInRC.point = 30;
		pointInRC.min = 0;
		pointInRC.max = 3;
	}
	return pointInRC;
}