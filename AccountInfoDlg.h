#pragma once
#include <Interface/HapyUserManager.h>
#include<string>
#include "afxwin.h"

// CAccount 对话框
//账户信息对话框
class CAccountInfoDlg : public CBCGPDialog
{
	DECLARE_DYNAMIC(CAccountInfoDlg)

public:
	CAccountInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAccountInfoDlg();
	UserInfo * myinfo;

	CBCGPEdit m_username;
	CBCGPEdit m_password;
	CBCGPEdit m_phone;
	CBCGPEdit m_email;

	std::string username;
	std::string userphone;
	std::string useremail;
// 对话框数据
	enum { IDD = IDD_AccountDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
