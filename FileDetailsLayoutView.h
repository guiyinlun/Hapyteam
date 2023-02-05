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
	CFileDetailsLayoutView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
public:
	//Tab�ؼ�
	CBCGPTabWnd				m_wndTab;
	//�����ҳ��
	CVersionListDlg			m_pversionDlg;
	CReferenceListDlg		m_preferenceDlg;
	CByReferenceListDlg		m_pbyrelerenceDlg;	
	CRevisionListDlg		m_previsionDlg;
	CAnnotateDlg			m_pannotateDlg;
	//��ǰTab��
	int						m_curSelTab;
	//��ҳ�Ƿ����
	bool					isLoad[5];
public:
	//���ü����ź�
	void					ResetIsLoad();
protected:
	afx_msg LRESULT HandleInitDialog(WPARAM wParam, LPARAM lParam);
	virtual void OnInitialUpdate();		
	void OnReset();
	afx_msg LRESULT OnChangingActiveTab(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnChangeActiveTab(WPARAM wp, LPARAM lp);
};


