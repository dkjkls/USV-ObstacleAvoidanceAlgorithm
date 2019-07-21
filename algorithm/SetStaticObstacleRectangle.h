#pragma once
#include "afxvslistbox.h"
#include "resource.h"
#include "afxcmn.h"
#include "USVDoc.h"
#include "USVView.h"

// SetStaticObstacleRectangle 对话框

struct PointInRC//点在矩形中的位置结构体
{
	int point, min, max;//逆时针方向取min，max
};

class SetStaticObstacleRectangle : public CDialogEx
{
	DECLARE_DYNAMIC(SetStaticObstacleRectangle)

public:
	SetStaticObstacleRectangle(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SetStaticObstacleRectangle();
	void CalculateStaticOR(int i);//计算方形障碍物数据
	PointInRC calculate_pointInRC(REC Peak0, REC Peak1, REC Peak2, REC Peak3, float x, float y);//计算点在矩形的那个位置（共9个）

// 对话框数据
	enum { IDD = IDD_SetStaticObstacleRectangle };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnStnClicked65535();
	virtual BOOL OnInitDialog();
	CListCtrl LIST_StaticObstacleRec;
	UINT CenterX;
	UINT CenterY;
	float DirectionA;
//	UINT PeakX;
//	UINT PeakY;
	UINT HalfLength;
	UINT HalfWidth;
	afx_msg void OnBnClickedAddobstacler();
	afx_msg void OnBnClickedDeleteobstacler();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedSobstaclecinputr();
	afx_msg void OnBnClickedSobstaclecoutputr();
};
