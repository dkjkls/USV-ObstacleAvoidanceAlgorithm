#pragma once


// SetUnknown 对话框

class SetUnknown : public CDialogEx
{
	DECLARE_DYNAMIC(SetUnknown)

public:
	SetUnknown(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SetUnknown();

// 对话框数据
	enum { IDD = IDD_Set_Unknown };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	float UnknownR;
	float UnknownT;
	afx_msg void OnBnClickedOk();
};
