#pragma once
#include "afxcmn.h"


// CVersionupDlg 对话框

class CRevisionListDlg : public CBCGPDialog
{
	DECLARE_DYNAMIC(CRevisionListDlg)

public:
	CRevisionListDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRevisionListDlg();

// 对话框数据
	enum { IDD = IDD_RevisionListDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list;
	virtual BOOL OnInitDialog();
};
