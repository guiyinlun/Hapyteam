#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include"CustomMethods.h"


extern int nSel;
// CQuoteDlg 对话框

class CReferenceListDlg : public CBCGPDialog
{
	DECLARE_DYNAMIC(CReferenceListDlg)

public:
	CReferenceListDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CReferenceListDlg();

// 对话框数据
	enum { IDD = IDD_ReferenceDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CBCGPComboBox	m_Combo;
	CHAPYTreeCtrl	m_treeGrid;
	HapyProjectFile* m_pselectfile;
protected:
	virtual BOOL OnInitDialog();
	//选中改变下拉列表框
	afx_msg void OnCbnSelchangequotecombo();
	//节点展开（逐级加载）
	afx_msg void OnItemExpanding(NMHDR* pNMHDR, LRESULT* pResult);
	//关闭
	afx_msg void OnDestroy();
public:
	//重置
	void ResetComboAndTree();
	//加载下拉列表框
	void LoadCombo(HapyProjectFile* file);
	//添加引用树节点
	void AddOneData(HapyProjectFile* file,CBCGPGridRow* parentR);
	//根据版本combonum加载数据
	void LoadData(HapyProjectFile* file,int combonum);
};
