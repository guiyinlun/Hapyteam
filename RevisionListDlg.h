#pragma once
#include "afxcmn.h"


// CVersionupDlg �Ի���

class CRevisionListDlg : public CBCGPDialog
{
	DECLARE_DYNAMIC(CRevisionListDlg)

public:
	CRevisionListDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRevisionListDlg();

// �Ի�������
	enum { IDD = IDD_RevisionListDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list;
	virtual BOOL OnInitDialog();
};
