#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include"CustomMethods.h"
#include"FileDetailsLayoutView.h"
#include"VersionListDlg.h"
#include"ReferenceListDlg.h"
#include"ByReferenceListDlg.h"
#include"RevisionListDlg.h"
#include"AnnotateDlg.h"
#include <Interface/HapyProjectFolder.h>

class CProjectTreeView;
class CHAPYBreadcrumb:public CBCGPBreadcrumb
{ 
public:
	//�������·���ڵ�
	BOOL DeleteAllItem();
};
class CFolderListView : public CBCGPFormView
{
	DECLARE_DYNCREATE(CFolderListView)

protected:
	CFolderListView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CFolderListView();
public:
	enum { IDD = IDD_FolderListView };
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
	//�б�ؼ�
	CHAPYListCtrl		 m_listGrid;
	//·�����ؼ�
	CHAPYBreadcrumb		 m_Breadcrumb;
	//ˢ�°�ť
	CBCGPButton          m_listUpdate;
	//����ѡ�е��ļ�
	HapyProjectFile*     m_pselectFile;
	//��ѡ���ļ�
	vector<HapyProjectFile*>
		m_pselectFiles;
	//�ļ����ˣ����˲��ɼ����ļ���
	vector<string>       m_filefilter;
	//�ļ�����ͼӳ��
	map<std::string,int> m_ExtensionAndIconMapping;
	HACCEL				 m_hAccel;
	//ˢ��״̬
	bool				 isRefresh;
	//����ͼ
	CProjectTreeView	*m_pprojecttreeview;
	//��ϸҳ��ͼ
	CFileDetailsLayoutView
		*m_pdetailsView;
	//�汾
	CVersionListDlg		*m_pversionDlg;
	//����
	CReferenceListDlg	*m_preferenceDlg;
	//������
	CByReferenceListDlg	*m_pbyrelerenceDlg;
	//�����¼
	CRevisionListDlg	*m_previsionDlg;
	//Ȧ����ע
	CAnnotateDlg		*m_pannotateDlg;
public:
	//��ʼ��
	afx_msg LRESULT HandleInitDialog(WPARAM wParam, LPARAM lParam);
	virtual void OnInitialUpdate();
	//�Ҽ��˵�
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	//˫����Ŀ¼
	afx_msg LRESULT OnGridDoubleClicked(WPARAM wParam, LPARAM lParam);
	//�б�ѡ�иı�
	afx_msg LRESULT  OnSelchangedList(WPARAM wParam, LPARAM lParam);
	//·����ѡ�иı�
	afx_msg void OnBredcrumbSelectionChanged (NMHDR* pNotifyStruct, LRESULT* result);
	//������С
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//�ر�
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	//����
	afx_msg void OnCheckIn();
	//���
	afx_msg void OnCheckOut();
	//��ȡ
	afx_msg void OnDownload();
	//�������
	afx_msg void OnUndoCheckOut();
	//Ctrl+F�ȼ�
	afx_msg void OnCtrlf();
	//ˢ��
	afx_msg void OnBnClickedListupdate();


public:
	//�����б�
	void ResetList();
	//�����б�����
	void LoadData(HapyProjectFolder* hapyFolder);
	//����·����
	void ResetBreadcrumb();
	//���·���ڵ�
	void AddBreadcrumb(HapyProjectFolder* node,HBREADCRUMBITEM parentItem);
	//����·��������
	void LoadBreadcrumb(HapyProjectFolder* folder);
	//��ȡ��Ŀ¼�������ļ�
	void GetAllSubFiles(HapyProjectFolder* folder);
	//��ȡ����ѡ�е��ļ�
	void GetAllSelectFiles();
	//�ȼ���Ϣ
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


