// SetDoPSO.cpp : 实现文件
//

#include "stdafx.h"
#include "USV.h"
#include "USVDoc.h"
#include "USVView.h"
#include "SetDoPSO.h"
#include "afxdialogex.h"

extern PSO_DO pso_do;//粒子群参数
extern int thisPSOnum;//本次pso测试次数

// SetDoPSO 对话框

IMPLEMENT_DYNAMIC(SetDoPSO, CDialogEx)

SetDoPSO::SetDoPSO(CWnd* pParent /*=NULL*/)
	: CDialogEx(SetDoPSO::IDD, pParent)
	, PSOgenerationsDO(0)
	, PSOparticle_numDO(0)
	, PSOchangeDO(FALSE)
	, PSOchangeVdo(0)
	, PSOchangeDado(0)
{

}

SetDoPSO::~SetDoPSO()
{
}

void SetDoPSO::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PSOgenerationsDO, PSOgenerationsDO);
	DDV_MinMaxUInt(pDX, PSOgenerationsDO, 0, 2000);
	DDX_Text(pDX, IDC_PSOparticle_numDO, PSOparticle_numDO);
	DDV_MinMaxUInt(pDX, PSOparticle_numDO, 0, 2000);
	DDX_Check(pDX, IDC_PSOchangeDO, PSOchangeDO);
	DDX_Control(pDX, IDC_PSOchangeV, PSOchangeV);
	DDX_Text(pDX, IDC_PSOchangeV, PSOchangeVdo);
	DDX_Control(pDX, IDC_PSOchangeDa, PSOchangeDa);
	DDX_Text(pDX, IDC_PSOchangeDa, PSOchangeDado);
}


BEGIN_MESSAGE_MAP(SetDoPSO, CDialogEx)
	ON_BN_CLICKED(IDC_PSOchangeDO, &SetDoPSO::OnBnClickedPsochangedo)
	ON_BN_CLICKED(IDOK, &SetDoPSO::OnBnClickedOk)
END_MESSAGE_MAP()


// SetDoPSO 消息处理程序


void SetDoPSO::OnBnClickedPsochangedo()
{
	// TODO:  在此添加控件通知处理程序代码
	if (PSOchangeDO)
	{
		PSOchangeDO = FALSE;
	}
	else
	{
		PSOchangeDO = TRUE;
	}
	if (PSOchangeDO)
	{
		PSOchangeV.EnableWindow(TRUE);
		PSOchangeDa.EnableWindow(TRUE);
	}
	if (!PSOchangeDO)
	{
		PSOchangeV.EnableWindow(FALSE);
		PSOchangeDa.EnableWindow(FALSE);
	}
}


BOOL SetDoPSO::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	if (PSOchangeDO)
	{
		PSOchangeV.EnableWindow(TRUE);
		PSOchangeDa.EnableWindow(TRUE);
	}
	if (!PSOchangeDO)
	{
		PSOchangeV.EnableWindow(FALSE);
		PSOchangeDa.EnableWindow(FALSE);
	}
	
	PSOgenerationsDO = pso_do.PSOgenerations;
	PSOparticle_numDO = pso_do.PSOparticle_num;
	PSOchangeVdo = pso_do.WeightV;
	PSOchangeDado = pso_do.WeightDa;
	PSOchangeDO = pso_do.changeWeight;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void SetDoPSO::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialogEx::OnOK();

	UpdateData(TRUE);//若无此更新，写入数值为上次的值

	thisPSOnum = 0;//本次pso测试次数

	pso_do.PSOgenerations = PSOgenerationsDO;
	pso_do.PSOparticle_num = PSOparticle_numDO;
	pso_do.WeightV = PSOchangeVdo;
	pso_do.WeightDa = PSOchangeDado;
	pso_do.changeWeight = PSOchangeDO;
	
	UpdateData(FALSE);
}
