#pragma once
#include "afxcmn.h"
#include "SetStaticObstacleRectangle.h"

/*
struct PointInRC//点在矩形中的位置结构体
{
	int point, min, max;//逆时针方向取min，max
};*/

// SetDynamicOR 对话框

class SetDynamicOR : public CDialogEx
{
	DECLARE_DYNAMIC(SetDynamicOR)

public:
	SetDynamicOR(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SetDynamicOR();

// 对话框数据
	enum { IDD = IDD_SetDynamicOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	UINT DynamicOR_X;
	UINT DynamicOR_Y;
	UINT DynamicOR_HL;
	UINT DynamicOR_HW;
	float DynamicOR_DA;
	float DynamicOR_V;
	virtual BOOL OnInitDialog();
	CListCtrl LIST_DynamicOR;
	afx_msg void OnBnClickedAdddynamicOR();

	void CalculateDynamicOR(int i);//计算方形障碍物数据
	PointInRC calculate_pointInDRC(int m, float x, float y);//计算点在矩形的那个位置（共9个）

	afx_msg void OnBnClickedDeletedynamicOR();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedSdynamicorinput();
	afx_msg void OnBnClickedSdynamicoroutput();
};
