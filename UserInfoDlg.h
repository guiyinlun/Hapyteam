#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include"AccountInfoDlg.h"
#include"BasicInfoDlg.h"
// CUserDlg 对话框

class CUserInfoDlg : public CBCGPDialog
{
	DECLARE_DYNAMIC(CUserInfoDlg)

public:
	CUserInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUserInfoDlg();

// 对话框数据
	enum { IDD = IDD_UserInfoDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CBCGPTabWnd m_wndTab;
	CImageList m_TabImageList;
	CAccountInfoDlg m_account;
	CBasicInfoDlg m_basic;
		afx_msg LRESULT HandleInitDialog(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	void OnReset();
};
