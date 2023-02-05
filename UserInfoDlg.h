#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include"AccountInfoDlg.h"
#include"BasicInfoDlg.h"
// CUserDlg �Ի���

class CUserInfoDlg : public CBCGPDialog
{
	DECLARE_DYNAMIC(CUserInfoDlg)

public:
	CUserInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUserInfoDlg();

// �Ի�������
	enum { IDD = IDD_UserInfoDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
