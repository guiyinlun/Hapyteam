#pragma once
#include"CustomMethods.h"


class CFileSelectDlg : public CBCGPDialog
{
	DECLARE_DYNAMIC(CFileSelectDlg)

public:
	CFileSelectDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFileSelectDlg();
	
// 对话框数据
	enum { IDD = IDD_FileSelectDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	CHAPYTreeCtrl	m_treeGrid;
	CFolderListView	*m_pfolderlistview;
	void ResetTree();
	void LoadData(HapyProjectFile* thisfile,CBCGPGridRow* parentR);
	void LoadAllData(vector<HapyProjectFile*> thisfiles);
	void GetCheckNode( HTREEITEM hRoot);
	void InsertAllLeaves(HTREEITEM hRoot);


public:
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnHeaderCheckClick(WPARAM wp, LPARAM lp);
	afx_msg void OnItemExpanding(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnCheckClick(WPARAM wp, LPARAM lp);
	afx_msg void OnBnClickedOkfileselect();
	afx_msg void OnBnClickedCancelfileselect();
};
