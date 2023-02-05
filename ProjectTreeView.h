#pragma once
#include"CustomMethods.h"
#include "FolderListView.h"
#include "FileDetailsLayoutView.h"
#include <Interface/HapyObject.h>
#include "Interface/HapyProjectFile.h"
#include "Interface/HapyProjectFolder.h"

// CProjectStructureTreeView 窗体视图
class CProjectTreeView : public CBCGPFormView
{
	DECLARE_DYNCREATE(CProjectTreeView)

protected:
	CProjectTreeView();           // 动态创建所使用的受保护的构造函数
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	//树控件
	CHAPYTreeCtrl		m_treeGrid;
	//字体
	CFont				m_font; 
	//列表视图
	CFolderListView		*m_pfolderlistview;
	//详细页视图
	CFileDetailsLayoutView	*m_pdetailsview;
	
public:
	//初始化
	afx_msg LRESULT HandleInitDialog(WPARAM wParam, LPARAM lParam);
	virtual void OnInitialUpdate();
	//选中树节点
	afx_msg void OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult);
	//右键菜单
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);

public:
	//重置树
	void ResetTree();
	//添加树节点
	void AddTreeNode(HapyProjectFolder* projectFolder,HTREEITEM parentItem);
	//加载数据
	void LoadData(HapyProjectFolder* projectFolder);
	//在父节点下根据对象指针查找节点
	HTREEITEM FindItemByData(HTREEITEM parentItem,HapyProjectFolder* projectFolder);
	//清空子节点
	void RemoveChildNode(HTREEITEM parentItem);
};


