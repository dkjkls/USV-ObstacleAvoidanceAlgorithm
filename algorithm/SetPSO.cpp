// SetPSO.cpp : 实现文件
//

#include "stdafx.h"
#include "USV.h"
#include "USVDoc.h"
#include "USVView.h"
#include "SetPSO.h"
#include "afxdialogex.h"


// SetPSO 对话框

extern PSO pso;//粒子群参数
extern int thisPSOnum;//本次pso测试次数

IMPLEMENT_DYNAMIC(SetPSO, CDialogEx)

SetPSO::SetPSO(CWnd* pParent /*=NULL*/)
	: CDialogEx(SetPSO::IDD, pParent)
	, PSOgenerations(0)
	, PSOparticle_num(0)
	, PSOsetV_numMAX(0)
	, PSOsetInitialV(0)
{

}

SetPSO::~SetPSO()
{
}

void SetPSO::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PSOgenerations, PSOgenerations);
	DDV_MinMaxInt(pDX, PSOgenerations, 0, 10000);
	DDX_Text(pDX, IDC_PSOparticle_num, PSOparticle_num);
	DDV_MinMaxInt(pDX, PSOparticle_num, 0, 10000);
	DDX_Text(pDX, IDC_PSOsetV_numMAX, PSOsetV_numMAX);
	DDV_MinMaxInt(pDX, PSOsetV_numMAX, 0, 10000);
	//  DDX_Text(pDX, IDC_PSOsetInitialV, PSOsetInitialV);
	//  DDV_MinMaxFloat(pDX, PSOsetInitialV, 0, 1000);
	DDX_Text(pDX, IDC_PSOsetInitialV, PSOsetInitialV);
	DDV_MinMaxInt(pDX, PSOsetInitialV, 0, 1000);
}


BEGIN_MESSAGE_MAP(SetPSO, CDialogEx)
	ON_BN_CLICKED(IDOK, &SetPSO::OnBnClickedOk)
END_MESSAGE_MAP()


// SetPSO 消息处理程序


void SetPSO::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);//若无此更新，写入数值为上次的值
	
	thisPSOnum = 0;//本次pso测试次数

	pso.PSOgenerations = PSOgenerations;
	pso.PSOparticle_num = PSOparticle_num;
	pso.PSOsetInitialV = PSOsetInitialV;
	pso.PSOsetV_numMAX = PSOsetV_numMAX;

	UpdateData(FALSE);

	CDialogEx::OnOK();
}
