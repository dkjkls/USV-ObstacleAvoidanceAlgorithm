// SetUnknown.cpp : 实现文件
//

#include "stdafx.h"
#include "USV.h"
#include "SetUnknown.h"
#include "afxdialogex.h"
#include "USVDoc.h"
#include "USVView.h"

extern Unknown unknown;//局部未知避障

// SetUnknown 对话框

IMPLEMENT_DYNAMIC(SetUnknown, CDialogEx)

SetUnknown::SetUnknown(CWnd* pParent /*=NULL*/)
	: CDialogEx(SetUnknown::IDD, pParent)
	, UnknownR(0)
	, UnknownT(0)
{

}

SetUnknown::~SetUnknown()
{
}

void SetUnknown::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_UnknownR, UnknownR);
	DDX_Text(pDX, IDC_UnknownL, UnknownT);
}


BEGIN_MESSAGE_MAP(SetUnknown, CDialogEx)
	ON_BN_CLICKED(IDOK, &SetUnknown::OnBnClickedOk)
END_MESSAGE_MAP()

// SetUnknown 消息处理程序

void SetUnknown::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码

	UpdateData(TRUE);//若无此更新，写入数值为上次的值

	unknown.UnknownR = UnknownR;
	unknown.UnknownT = UnknownT;

	UpdateData(FALSE);

	CDialogEx::OnOK();
}
