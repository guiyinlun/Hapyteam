#include "stdafx.h"
#include "hapyteamclient.h"
#include "ProjectTreeView.h"
#include"MainFrm.h"
#include "FolderListView.h"
#include "FileSelectDlg.h"
#include <Core/FileUploader.h>
#include <Core/FileDownloader.h>
#include <Interface/HapyProjectManager.h>


UINT  DownloadThread(LPVOID lparam);


IMPLEMENT_DYNCREATE(CProjectTreeView, CBCGPFormView)

	CProjectTreeView::CProjectTreeView()
	: CBCGPFormView(CProjectTreeView::IDD)
{
	EnableVisualManagerStyle();
	EnableLayout();
	m_font.CreatePointFont(110, L"微软雅黑");
	m_pfolderlistview=NULL;
	m_pdetailsview=NULL;
}

CProjectTreeView::~CProjectTreeView()
{
}

void CProjectTreeView::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CProjectTreeView, CBCGPFormView)
	ON_MESSAGE(WM_INITDIALOG, &CProjectTreeView::HandleInitDialog)
	ON_NOTIFY(TVN_SELCHANGED, IDC_ProjectStructureTree, &CProjectTreeView::OnSelchangedTree)
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()


// CMyTreeView 诊断

#ifdef _DEBUG
void CProjectTreeView::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CProjectTreeView::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}
#endif
#endif //_DEBUG



LRESULT CProjectTreeView::HandleInitDialog(WPARAM wParam, LPARAM lParam)
{
	BOOL bRes = (BOOL)CBCGPFormView::HandleInitDialog(wParam, lParam);
	UINT nTreeStyles = WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | TVS_LINESATROOT |
		TVS_HASLINES | //有线
		TVS_HASBUTTONS | //有按钮
		TVS_SHOWSELALWAYS|
		TVS_TRACKSELECT |
		TVS_FULLROWSELECT; //整行选中
	m_treeGrid.CreateOnPlaceHolder(this, IDC_ProjectStructureTree, 0, nTreeStyles);

	CRect thisrect,viewrect;
	this->GetClientRect(&viewrect);
	thisrect.SetRect(0,0,viewrect.right,viewrect.bottom);
	m_treeGrid.MoveWindow(&thisrect);
	CBCGPStaticLayout* pLayout = (CBCGPStaticLayout*)GetLayout();
	if (pLayout != NULL)
	{
		pLayout->AddAnchor(m_treeGrid.GetSafeHwnd(), 
			CBCGPStaticLayout::e_MoveTypeNone, CBCGPStaticLayout::e_SizeTypeBoth);
	}
	return bRes;

}

void CProjectTreeView::OnInitialUpdate()
{
	CBCGPFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	//设置
	m_treeGrid.EnableHeader(FALSE,  BCGP_GRID_HEADER_SORT);
	m_treeGrid.EnableColumnAutoSize(TRUE);
	m_treeGrid.EnableGridLines(FALSE);
	m_treeGrid.EnableAlternateRows(FALSE);
	m_treeGrid.SetCustomRowHeight(38,38);//设置行宽，第二个参数决定

	m_treeGrid.SetVisualManagerColorTheme(TRUE);
	CMainFrame*pMainFrame = DYNAMIC_DOWNCAST(CMainFrame,AfxGetMainWnd());
	m_treeGrid.SetImageList(&pMainFrame->m_ImageList,LVSIL_NORMAL);
	m_treeGrid.SetFont(&m_font);

	m_treeGrid.AdjustLayout();
	m_treeGrid.RedrawWindow ();
}

void CProjectTreeView::OnSelchangedTree(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	HTREEITEM hTreeItem = m_treeGrid.GetSelectedItem ();
	if (hTreeItem == NULL)return;
	BeginWaitCursor();
	HapyProjectFolder* selectFolder =(HapyProjectFolder*)m_treeGrid.GetItemData(hTreeItem);
	if(selectFolder==NULL) return;
	//路径改变
	//istreechange=TRUE;
	CString pathstr=StringUtils::StringToCString(selectFolder->RelativePath().erase(0,1).c_str());
	m_pfolderlistview->m_Breadcrumb.SelectPath(pathstr,_T('\\'));
	//list
	m_pfolderlistview->ResetList();
	m_pfolderlistview->LoadData(selectFolder);
	EndWaitCursor();
	//刷新tab页
	m_pfolderlistview->m_pversionDlg->m_listGrid.RemoveAll();
	m_pfolderlistview->m_preferenceDlg->m_treeGrid.DeleteAllItems();
	m_pfolderlistview->m_preferenceDlg->m_Combo.ResetContent();
	m_treeGrid.Expand(hTreeItem,TVE_EXPAND);
	*pResult = 0;
}

void CProjectTreeView::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	//CMenu menu;
	//menu.LoadMenuW(IDR_TreeMENU);//加载菜单
	//CMenu *contextMenu = menu.GetSubMenu(0);//加载子项
	//ASSERT(contextMenu != NULL);
	//HTREEITEM hItem = m_treeGrid.GetSelectedItem();
	//if(hItem==NULL)
	//	return;
	////CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;
	////pPopupMenu->Create (this, point.x, point.y, contextMenu->Detach (),FALSE,TRUE);
	////pPopupMenu->SetForceMenuFocus(TRUE);
	////pPopupMenu->SetAutoDestroy(TRUE);
	/////CMainFrame*pMainFrame = DYNAMIC_DOWNCAST(CMainFrame,AfxGetMainWnd());
	////pMainFrame->OnShowPopupMenu(pPopupMenu);
	////pPopupMenu->ActivatePopupMenu((CBCGPFrameWnd *)AfxGetMainWnd(),pPopupMenu);
	////::SetForegroundWindow(pPopupMenu->m_hWnd);
	//menu.GetSubMenu(0)->TrackPopupMenu
	//	(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, GetParent());//创建一个浮动的菜单
}



void CProjectTreeView::ResetTree(){
	this->m_treeGrid.DeleteAllItems();
	this->m_treeGrid.DeleteAllColumns();
	this->m_treeGrid.InsertColumn(0, _T("项目"), 100);
	/*this->m_treeGrid.InsertColumn(1, _T(""), 20);
	this->m_treeGrid.SetColumnVisible(1,FALSE);*/
}

void CProjectTreeView::AddTreeNode(HapyProjectFolder* projectFolder,HTREEITEM parentItem)
{
	//HapyProjectFolder* projectFolder= dynamic_cast<HapyProjectFolder*>(node);
	CString	 name=StringUtils::StringToCString(projectFolder->Name().c_str());
	if(name=="_V") return;
	wstring wname=StringUtils::String2WString(projectFolder->Name());
	LPCWSTR result = wname.c_str();
	HTREEITEM m_item = this->m_treeGrid.InsertItem(result,Wfolder,Wfolder,parentItem);

	this->m_treeGrid.SetItemData(m_item,(DWORD_PTR)projectFolder);
	vector<HapyProjectFolder*>* subFolders=projectFolder->SubFolders();
	if(subFolders->size()>0){
		for (vector<HapyProjectFolder*>::iterator it =subFolders->begin();it!=subFolders->end();it++ )
		{
			HapyProjectFolder* childFolder= *it;
			AddTreeNode(childFolder,m_item);
		}
		this->m_treeGrid.Expand(m_item,TVE_COLLAPSE);
	}
}
void CProjectTreeView::LoadData(HapyProjectFolder* projectFolder){
	m_treeGrid.SetRedraw(FALSE);
	AddTreeNode(projectFolder,NULL);
	this->m_treeGrid.Expand(this->m_treeGrid.GetRootItem(),TVE_EXPAND);
	this->m_treeGrid.SetRedraw(TRUE);
	this->m_treeGrid.AdjustLayout();
}
HTREEITEM  CProjectTreeView::FindItemByData(HTREEITEM parentItem,HapyProjectFolder* projectFolder){
	if (parentItem==NULL)
	{
		return NULL;
	}
	HTREEITEM hChild=this->m_treeGrid.GetChildItem(parentItem);
	while(hChild){
		HapyProjectFolder* item_Data=(HapyProjectFolder*) this->m_treeGrid.GetItemData(hChild);
		if (item_Data!=NULL && item_Data == projectFolder)
		{
			return hChild;
		}
		hChild = this->m_treeGrid.GetNextItem(hChild, TVGN_NEXT);
	}

	return NULL;
}

void CProjectTreeView::RemoveChildNode(HTREEITEM parentItem){
	vector<HTREEITEM> childs;
	HTREEITEM hChild= this->m_treeGrid.GetChildItem(parentItem);
	while(hChild){
		childs.push_back(hChild);
		hChild = this->m_treeGrid.GetNextItem(hChild, TVGN_NEXT);	
	}
	if (childs.size()>0)
	{
		for (vector<HTREEITEM>::iterator it= childs.begin();it!=childs.end();it++)
		{
			HTREEITEM item= *it;
			this->m_treeGrid.DeleteItem(item);
		}
	}
}
