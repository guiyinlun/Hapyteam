#pragma once
#include <Core/api.h>
#include "afxcmn.h"

// CProjectList 对话框

class CProjectList : public CBCGPDialog
{
	DECLARE_DYNAMIC(CProjectList)

public:
	CProjectList(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CProjectList();

// 对话框数据
	enum { IDD = IDD_ProjectlistDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	CImageList		m_ImageList;
	CBCGPListCtrl	m_prolist;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnNMDblclkprojectlist(NMHDR *pNMHDR, LRESULT *pResult);
};

