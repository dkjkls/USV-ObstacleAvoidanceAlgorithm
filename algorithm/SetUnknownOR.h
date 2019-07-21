#pragma once
#include "afxcmn.h"
#include "SetStaticObstacleRectangle.h"

// SetUnknownOR 对话框

class SetUnknownOR : public CDialogEx
{
	DECLARE_DYNAMIC(SetUnknownOR)

public:
	SetUnknownOR(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SetUnknownOR();

// 对话框数据
	enum { IDD = IDD_SetUnknownOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl LIST_UnknownOR;
	UINT UnknownOR_X;
	UINT UnknownOR_Y;
	UINT UnknownOR_HL;
	UINT UnknownOR_HW;
	float UnknownOR_DA;
	float UnknownOR_V;

	void CalculateUnknownOR(int i);//计算方形障碍物数据
	PointInRC calculate_pointInDRC(int m, float x, float y);//计算点在矩形的那个位置（共9个）

	afx_msg void OnBnClickedAddunknownor();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedDeleteunknownor();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedSunknownorinput();
	afx_msg void OnBnClickedSunknownoroutput();
};
