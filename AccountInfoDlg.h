#pragma once
#include <Interface/HapyUserManager.h>
#include<string>
#include "afxwin.h"

// CAccount �Ի���
//�˻���Ϣ�Ի���
class CAccountInfoDlg : public CBCGPDialog
{
	DECLARE_DYNAMIC(CAccountInfoDlg)

public:
	CAccountInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAccountInfoDlg();
	UserInfo * myinfo;

	CBCGPEdit m_username;
	CBCGPEdit m_password;
	CBCGPEdit m_phone;
	CBCGPEdit m_email;

	std::string username;
	std::string userphone;
	std::string useremail;
// �Ի�������
	enum { IDD = IDD_AccountDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
