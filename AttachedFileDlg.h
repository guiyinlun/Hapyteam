#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "CustomMethods.h"
#include <Core/api.h>


// addReference 对话框
//附加文件对话框
class CAttachedFileDlg : public CBCGPDialog
{
	DECLARE_DYNAMIC(CAttachedFileDlg)

public:
	CAttachedFileDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAttachedFileDlg();

// 对话框数据
	enum { IDD = IDD_AttachedFileDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CHAPYTreeCtrl m_treeGrid;
	CHAPYListCtrl m_listGrid;
public:
	void ResetTree();
	void AddTreeNode(HapyProjectFolder* projectFolder,HTREEITEM parentItem);
	void LoadTreeData();
	void LoadListData();
protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnTvnSelchangedaddtree(NMHDR *pNMHDR, LRESULT *pResult);
};
