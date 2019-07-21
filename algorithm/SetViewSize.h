#pragma once
#include "afxwin.h"


// SetViewSize 对话框

class SetViewSize : public CDialogEx
{
	DECLARE_DYNAMIC(SetViewSize)

public:
	SetViewSize(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SetViewSize();

// 对话框数据
	enum { IDD = IDD_SetViewSize };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	UINT setviewL;
	UINT setviewW;
	afx_msg void OnBnClickedOk();
	BOOL SetViewChange;
	afx_msg void OnBnClickedChange();
	CEdit setviewLC;
	CEdit setviewWC;
	virtual BOOL OnInitDialog();
};
