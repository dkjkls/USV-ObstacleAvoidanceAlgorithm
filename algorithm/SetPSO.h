#pragma once
#include "resource.h"

// SetPSO 对话框

class SetPSO : public CDialogEx
{
	DECLARE_DYNAMIC(SetPSO)

public:
	SetPSO(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SetPSO();

// 对话框数据
	enum { IDD = IDD_SetPSO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int PSOgenerations;
	int PSOparticle_num;
	int PSOsetV_numMAX;
	int PSOsetInitialV;
	afx_msg void OnBnClickedOk();

};
