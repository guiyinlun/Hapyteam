#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include"VersionListDlg.h"
#include"ReferenceListDlg.h"
#include"ByReferenceListDlg.h"
#include"RevisionListDlg.h"
#include"AnnotateDlg.h"
#include "resource.h"

class CFileDetailsLayoutView : public CBCGPFormView
{
	DECLARE_DYNCREATE(CFileDetailsLayoutView)

protected:
	CFileDetailsLayoutView();           // 动态创建所使用的受保护的构造函数
	virtual ~CFileDetailsLayoutView();

public:
	enum { IDD = IDD_FileDetailsLayoutView };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	//Tab控件
	CBCGPTabWnd				m_wndTab;
	//五个子页面
	CVersionListDlg			m_pversionDlg;
	CReferenceListDlg		m_preferenceDlg;
	CByReferenceListDlg		m_pbyrelerenceDlg;	
	CRevisionListDlg		m_previsionDlg;
	CAnnotateDlg			m_pannotateDlg;
	//当前Tab号
	int						m_curSelTab;
	//子页是否加载
	bool					isLoad[5];
public:
	//重置加载信号
	void					ResetIsLoad();
protected:
	afx_msg LRESULT HandleInitDialog(WPARAM wParam, LPARAM lParam);
	virtual void OnInitialUpdate();		
	void OnReset();
	afx_msg LRESULT OnChangingActiveTab(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnChangeActiveTab(WPARAM wp, LPARAM lp);
};


