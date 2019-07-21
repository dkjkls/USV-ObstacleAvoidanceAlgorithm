#pragma once
#include "afxvslistbox.h"
#include "resource.h"
#include "afxcmn.h"

// SetStaticObstacleCircle 对话框

class SetStaticObstacleCircle : public CDialogEx
{
	DECLARE_DYNAMIC(SetStaticObstacleCircle)

public:
	SetStaticObstacleCircle(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SetStaticObstacleCircle();

// 对话框数据
	enum { IDD = IDD_SetStaticObstacleCircle };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
//	CVSListBox MfcVSListBox;

	virtual BOOL OnInitDialog();
	CListCtrl LIST_StaticObstacle;
	UINT StaticObstacleX;
	UINT StaticObstacleY;
	UINT StaticObstacleR;
	afx_msg void OnBnClickedAddobstaclec();
	afx_msg void OnBnClickedDeleteobstaclec();
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeStaticobstacley();
	afx_msg void OnBnClickedSobstaclecinput();
	afx_msg void OnBnClickedSobstaclecoutput();
	static float calculate_PolarAngle(float StaticObstacleX, float StaticObstacleY);
};
