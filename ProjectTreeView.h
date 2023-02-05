#pragma once
#include"CustomMethods.h"
#include "FolderListView.h"
#include "FileDetailsLayoutView.h"
#include <Interface/HapyObject.h>
#include "Interface/HapyProjectFile.h"
#include "Interface/HapyProjectFolder.h"

// CProjectStructureTreeView ������ͼ
class CProjectTreeView : public CBCGPFormView
{
	DECLARE_DYNCREATE(CProjectTreeView)

protected:
	CProjectTreeView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CProjectTreeView();

public:
	enum { IDD = IDD_ProjectStructureTreeView };

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
	//���ؼ�
	CHAPYTreeCtrl		m_treeGrid;
	//����
	CFont				m_font; 
	//�б���ͼ
	CFolderListView		*m_pfolderlistview;
	//��ϸҳ��ͼ
	CFileDetailsLayoutView	*m_pdetailsview;
	
public:
	//��ʼ��
	afx_msg LRESULT HandleInitDialog(WPARAM wParam, LPARAM lParam);
	virtual void OnInitialUpdate();
	//ѡ�����ڵ�
	afx_msg void OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult);
	//�Ҽ��˵�
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);

public:
	//������
	void ResetTree();
	//������ڵ�
	void AddTreeNode(HapyProjectFolder* projectFolder,HTREEITEM parentItem);
	//��������
	void LoadData(HapyProjectFolder* projectFolder);
	//�ڸ��ڵ��¸��ݶ���ָ����ҽڵ�
	HTREEITEM FindItemByData(HTREEITEM parentItem,HapyProjectFolder* projectFolder);
	//����ӽڵ�
	void RemoveChildNode(HTREEITEM parentItem);
};


