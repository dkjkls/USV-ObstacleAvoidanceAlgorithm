#pragma once
#include "afxwin.h"
#include "resource.h"


// Set_start_goal_Position 对话框


class Set_start_goal_Position : public CDialogEx
{
	DECLARE_DYNAMIC(Set_start_goal_Position)

public:
	Set_start_goal_Position(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Set_start_goal_Position();

// 对话框数据
	enum { IDD = IDD_Set_start_goal_Position };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
//	CEdit StartX;
	UINT StartX;
	afx_msg void OnEnChangeStartx();
	UINT StartY;
	UINT GoalY;
	UINT GoalX;
//	afx_msg void OnBnClickedOk();
	afx_msg void OnSetSGPosionOk();
	float V_Kn;
};
