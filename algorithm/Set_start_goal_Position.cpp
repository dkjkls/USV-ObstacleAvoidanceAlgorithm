// Set_start_goal_Position.cpp : 实现文件
//

#include "stdafx.h"
#include "Set_start_goal_Position.h"
#include "afxdialogex.h"
#include "USVDoc.h"
#include "USVView.h"

extern bool state_sgposition;//始末点状态
extern int testNum;//测试次数
extern int thisPSOnum;//本次pso测试次数
extern StartGoalPOINT startgoalp;//始末点状态

// Set_start_goal_Position 对话框

IMPLEMENT_DYNAMIC(Set_start_goal_Position, CDialogEx)


Set_start_goal_Position::Set_start_goal_Position(CWnd* pParent /*=NULL*/)
	: CDialogEx(Set_start_goal_Position::IDD, pParent)
	, StartX(0)
	, StartY(0)
	, GoalY(0)
	, GoalX(0)
	, V_Kn(0)
{

}

Set_start_goal_Position::~Set_start_goal_Position()
{
}

void Set_start_goal_Position::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_StartX, StartX);
	DDX_Text(pDX, IDC_StartX, StartX);
	DDV_MinMaxUInt(pDX, StartX, 0, 2000);
	DDX_Text(pDX, IDC_StartY, StartY);
	DDV_MinMaxUInt(pDX, StartY, 0, 2000);
	DDX_Text(pDX, IDC_GoalY, GoalY);
	DDV_MinMaxUInt(pDX, GoalY, 0, 2000);
	DDX_Text(pDX, IDC_GoalX, GoalX);
	DDV_MinMaxUInt(pDX, GoalX, 0, 2000);
	DDX_Text(pDX, IDC_GoalY2, V_Kn);
	DDV_MinMaxFloat(pDX, V_Kn, 0, 150);
}


BEGIN_MESSAGE_MAP(Set_start_goal_Position, CDialogEx)
	ON_EN_CHANGE(IDC_StartX, &Set_start_goal_Position::OnEnChangeStartx)
	ON_BN_CLICKED(IDOK, &Set_start_goal_Position::OnSetSGPosionOk)
END_MESSAGE_MAP()


// Set_start_goal_Position 消息处理程序


void Set_start_goal_Position::OnEnChangeStartx()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void Set_start_goal_Position::OnSetSGPosionOk()
{
	// TODO:  在此添加控件通知处理程序代码

	testNum = 0;//测试次数
	thisPSOnum = 0;//本次pso测试次数

	UpdateData(TRUE);//若无此更新，写入数值为上次的值
	state_sgposition = 1;
	Set_start_goal_Position setstartgoalp;
	startgoalp.sx = StartX;
	startgoalp.sy = StartY;
	startgoalp.gx = GoalX;
	startgoalp.gy = GoalY;
	startgoalp.v_kn = V_Kn;
	startgoalp.v_ms = startgoalp.v_kn * Kn_ms;
	UpdateData(FALSE);
	CDialogEx::OnOK();
}
