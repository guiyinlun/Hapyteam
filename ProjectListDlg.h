#pragma once
#include <Core/api.h>
#include "afxcmn.h"

// CProjectList �Ի���

class CProjectList : public CBCGPDialog
{
	DECLARE_DYNAMIC(CProjectList)

public:
	CProjectList(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CProjectList();

// �Ի�������
	enum { IDD = IDD_ProjectlistDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
public:
	CImageList		m_ImageList;
	CBCGPListCtrl	m_prolist;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnNMDblclkprojectlist(NMHDR *pNMHDR, LRESULT *pResult);
};

