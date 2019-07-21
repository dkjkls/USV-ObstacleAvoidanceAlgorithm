#pragma once
#include "afxwin.h"


// SetDoPSO 对话框

class SetDoPSO : public CDialogEx
{
	DECLARE_DYNAMIC(SetDoPSO)

public:
	SetDoPSO(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SetDoPSO();

// 对话框数据
	enum { IDD = IDD_SetDoPSO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	UINT PSOgenerationsDO;
	UINT PSOparticle_numDO;
	BOOL PSOchangeDO;
	CEdit PSOchangeV;
	UINT PSOchangeVdo;
	CEdit PSOchangeDa;
	UINT PSOchangeDado;
	afx_msg void OnBnClickedPsochangedo();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
