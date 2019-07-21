// SetViewSize.cpp : 实现文件
//

#include "stdafx.h"
#include "USV.h"
#include "USVDoc.h"
#include "USVView.h"
#include "SetViewSize.h"
#include "afxdialogex.h"

extern ViewSize viewsize;//视图大小

// SetViewSize 对话框

IMPLEMENT_DYNAMIC(SetViewSize, CDialogEx)

SetViewSize::SetViewSize(CWnd* pParent /*=NULL*/)
	: CDialogEx(SetViewSize::IDD, pParent)
	, setviewL(0)
	, setviewW(0)
	, SetViewChange(FALSE)
{
}

SetViewSize::~SetViewSize()
{
}

void SetViewSize::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SetViewLength, setviewL);
	DDX_Text(pDX, IDC_SetViewWidth, setviewW);
	DDX_Check(pDX, IDC_Change, SetViewChange);
	DDX_Control(pDX, IDC_SetViewLength, setviewLC);
	DDX_Control(pDX, IDC_SetViewWidth, setviewWC);
}


BEGIN_MESSAGE_MAP(SetViewSize, CDialogEx)
	ON_BN_CLICKED(IDOK, &SetViewSize::OnBnClickedOk)
	ON_BN_CLICKED(IDC_Change, &SetViewSize::OnBnClickedChange)
END_MESSAGE_MAP()


// SetViewSize 消息处理程序


void SetViewSize::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	
	UpdateData(TRUE);//若无此更新，写入数值为上次的值

	viewsize.W = setviewL ;
	viewsize.H = setviewW ;
	viewsize.change=SetViewChange;
	UpdateData(FALSE);
	
	CDialogEx::OnOK();
}


void SetViewSize::OnBnClickedChange()
{
	// TODO:  在此添加控件通知处理程序代码
		
	if (SetViewChange)
	{
		SetViewChange = FALSE;
	}
	else
	{
		SetViewChange = TRUE;
	}
	if (SetViewChange)
	{
		setviewLC.EnableWindow(FALSE);
		setviewWC.EnableWindow(FALSE);
	}
	if (!SetViewChange)
	{
		setviewLC.EnableWindow(TRUE); 
		setviewWC.EnableWindow(TRUE);
	}
}


BOOL SetViewSize::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	if (SetViewChange)
	{
		setviewLC.EnableWindow(FALSE);
		setviewWC.EnableWindow(FALSE);
	}
	if (!SetViewChange)
	{
		setviewLC.EnableWindow(TRUE);
		setviewWC.EnableWindow(TRUE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
