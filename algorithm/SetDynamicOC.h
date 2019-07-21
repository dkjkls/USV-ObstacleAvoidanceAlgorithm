#pragma once
#include "afxcmn.h"


// SetDynamicOC 对话框

class SetDynamicOC : public CDialogEx
{
	DECLARE_DYNAMIC(SetDynamicOC)

public:
	SetDynamicOC(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SetDynamicOC();

// 对话框数据
	enum { IDD = IDD_SetDynamicOC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	UINT DynamicOC_X;
	UINT DynamicOC_Y;
	UINT DynamicOC_R;
	float DynamicOC_DA;
	float DynamicOC_V;
	virtual BOOL OnInitDialog();
//	CListCtrl LIST_DynamicOC;
	CListCtrl LIST_DynamicOC;
	afx_msg void OnBnClickedAddDynamicoc();
	afx_msg void OnBnClickedDeletedynamicoc();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedSdynamicociutput();
	afx_msg void OnBnClickedSdynamicocoutputc();
};
