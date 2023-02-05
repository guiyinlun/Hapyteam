#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CReferenced 对话框

class CByReferenceListDlg : public CBCGPDialog
{
	DECLARE_DYNAMIC(CByReferenceListDlg)

public:
	CByReferenceListDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CByReferenceListDlg();

// 对话框数据
	enum { IDD = IDD_ByReferenceList };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CTreeCtrl m_tree;
	CComboBox m_combo;
};
