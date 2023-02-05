#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include"CustomMethods.h"
#include "resource.h"
#include<Interface/HapyProjectFile.h>


// CVersionsDlg 对话框

class CVersionListDlg : public CBCGPDialog
{
	DECLARE_DYNAMIC(CVersionListDlg)

public:
	CVersionListDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CVersionListDlg();

// 对话框数据
	enum { IDD = IDD_VersionsDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CHAPYListCtrl	m_listGrid;
	CImageList		m_versionlistImageList;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
public:
	void ResetList();
	void LoadData(HapyProjectFile* file);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnMenuDownload();
};
