#pragma once
#include "FilePropertyBar.h"
#include "FolderListView.h"
#include"CustomMethods.h"

class  DownloadInfo
{
public:
	DownloadInfo();
	bool	m_downloadtype;
	int		m_selectfileid;
	int		m_projectid;
	string	m_downloadpath;
};
class CMainFrame : public CBCGPFrameWnd
{

protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)
public:
	virtual ~CMainFrame();
	CFolderListView* GetRightPane();
	
	// ��д
public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


protected: 
	DECLARE_MESSAGE_MAP()
protected:  
	//CBCGPMenuBar m_wndMenuBar;
	CBCGPStatusBar			m_wndStatusBar;
	CBCGPMenuBar			m_wndMenuBar;
	CBCGPToolBar			m_wndToolBar;
	HICON					hIcon[48];		/*48=enmu HAPYicon:thisend*/
	bool					isviewlink;
	
	CProjectTreeView*		pprojecttreeview;
	CFolderListView*		pfolderlistview;
	CFileDetailsLayoutView* pdetailsview ;
public:
	CHAPYPropertyBar        m_wndPropBar;
	CBCGPSplitterWnd		m_wndSplitter1;
	CBCGPSplitterWnd		m_wndSplitter2;
	CImageList				m_ImageList;
	HINSTANCE				m_hLangDLL;
	int						m_projectid;/*��ǰ����Ŀid*/
	string					m_workfolderpath;/*����Ŀ¼*/
	bool					isIconLoad;
public:
	void addHAPYIcon();/*�޸������л�ָ��ͼ�����Ҫ�������һ�β�����͸��Ч��*/
	afx_msg void OnBlackTheme();
	afx_msg void OnWhiteTheme();
	BOOL OnShowPopupMenu (CBCGPPopupMenu* pMenuPopup);
	void CreateViewLink();
	// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnToolbarReset(WPARAM,LPARAM);
	afx_msg void OnLogin();
	afx_msg void OnUserInfo();
	afx_msg void OnProjectList();
	afx_msg void OnLogout();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual void OnUpdateFrameTitle(BOOL bAddToTitle);
	virtual void ActivateFrame(int nCmdShow = -1);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnFolderListColumnSelect();
	afx_msg void OnPropertiesViewBar();
	afx_msg void OnChinese();
	afx_msg void OnEnglish();
	afx_msg void OnUpdateLogout(CCmdUI *pCmdUI);
	afx_msg void OnUpdateLogin(CCmdUI *pCmdUI);
	afx_msg void OnUpdateUserInfo(CCmdUI *pCmdUI);
	afx_msg void OnUpdateBlackTheme(CCmdUI *pCmdUI);
	afx_msg void OnUpdateWhiteTheme(CCmdUI *pCmdUI);
	afx_msg void OnUpdateprojectList(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMenuEnglish(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMenuChinese(CCmdUI *pCmdUI);
	 
};


