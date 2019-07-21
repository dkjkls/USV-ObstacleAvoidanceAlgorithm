#pragma once
#include "afxcmn.h"


// SetUnknownOC 对话框

class SetUnknownOC : public CDialogEx
{
	DECLARE_DYNAMIC(SetUnknownOC)

public:
	SetUnknownOC(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SetUnknownOC();

	void calculate_unknownOC();//计算未知障碍圆的数据

// 对话框数据
	enum { IDD = IDD_SetUnknownOC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl LIST_UnknownOC;

	UINT UnknownOC_X;
	UINT UnknownOC_Y;
	UINT UnknownOC_R;
	float UnknownOC_DA;
	float UnknownOC_V;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedAddunknownoc();
	afx_msg void OnBnClickedDeleteunknownoc();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedSunknownociutput();
	afx_msg void OnBnClickedSunknownocoutput();
};
