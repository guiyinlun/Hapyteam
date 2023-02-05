#pragma once


// CBasic 对话框

class CBasicInfoDlg : public CBCGPDialog
{
	DECLARE_DYNAMIC(CBasicInfoDlg)

public:
	CBasicInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBasicInfoDlg();

// 对话框数据
	enum { IDD = IDD_BasicDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
