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
	//清空所有路径节点
	BOOL DeleteAllItem();
};
class CFolderListView : public CBCGPFormView
{
	DECLARE_DYNCREATE(CFolderListView)

protected:
	CFolderListView();           // 动态创建所使用的受保护的构造函数
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()	
public:
	//列表控件
	CHAPYListCtrl		 m_listGrid;
	//路径栏控件
	CHAPYBreadcrumb		 m_Breadcrumb;
	//刷新按钮
	CBCGPButton          m_listUpdate;
	//单个选中的文件
	HapyProjectFile*     m_pselectFile;
	//多选的文件
	vector<HapyProjectFile*>
		m_pselectFiles;
	//文件过滤（过滤不可检入文件）
	vector<string>       m_filefilter;
	//文件类型图映射
	map<std::string,int> m_ExtensionAndIconMapping;
	HACCEL				 m_hAccel;
	//刷新状态
	bool				 isRefresh;
	//树视图
	CProjectTreeView	*m_pprojecttreeview;
	//详细页视图
	CFileDetailsLayoutView
		*m_pdetailsView;
	//版本
	CVersionListDlg		*m_pversionDlg;
	//引用
	CReferenceListDlg	*m_preferenceDlg;
	//被引用
	CByReferenceListDlg	*m_pbyrelerenceDlg;
	//升版记录
	CRevisionListDlg	*m_previsionDlg;
	//圈阅批注
	CAnnotateDlg		*m_pannotateDlg;
public:
	//初始化
	afx_msg LRESULT HandleInitDialog(WPARAM wParam, LPARAM lParam);
	virtual void OnInitialUpdate();
	//右键菜单
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	//双击打开目录
	afx_msg LRESULT OnGridDoubleClicked(WPARAM wParam, LPARAM lParam);
	//列表选中改变
	afx_msg LRESULT  OnSelchangedList(WPARAM wParam, LPARAM lParam);
	//路径栏选中改变
	afx_msg void OnBredcrumbSelectionChanged (NMHDR* pNotifyStruct, LRESULT* result);
	//调整大小
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//关闭
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	//检入
	afx_msg void OnCheckIn();
	//检出
	afx_msg void OnCheckOut();
	//获取
	afx_msg void OnDownload();
	//撤销检出
	afx_msg void OnUndoCheckOut();
	//Ctrl+F热键
	afx_msg void OnCtrlf();
	//刷新
	afx_msg void OnBnClickedListupdate();


public:
	//重置列表
	void ResetList();
	//加载列表数据
	void LoadData(HapyProjectFolder* hapyFolder);
	//重置路径栏
	void ResetBreadcrumb();
	//添加路径节点
	void AddBreadcrumb(HapyProjectFolder* node,HBREADCRUMBITEM parentItem);
	//加载路径栏数据
	void LoadBreadcrumb(HapyProjectFolder* folder);
	//获取该目录下所有文件
	void GetAllSubFiles(HapyProjectFolder* folder);
	//获取所有选中的文件
	void GetAllSelectFiles();
	//热键消息
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


