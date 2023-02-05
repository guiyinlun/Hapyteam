// PropertiesViewBar.cpp : implementation of the CBCGPPropBar class
//

#include "stdafx.h"
#include "hapyteamclient.h"
#include "FilePropertyBar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CBCGPPropBar class

CHAPYPropertyBar::CHAPYPropertyBar()
{

}

CHAPYPropertyBar::~CHAPYPropertyBar()
{
}

BEGIN_MESSAGE_MAP(CHAPYPropertyBar, CBCGPDockingControlBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar message handlers

void CHAPYPropertyBar::AdjustLayout ()
{
	if (GetSafeHwnd () == NULL)
	{
		return;
	}
	 
	CRect rectClient;
	GetClientRect (rectClient);

	m_propertyList.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width (), rectClient.Height (), SWP_NOACTIVATE | SWP_NOZORDER);
}

int CHAPYPropertyBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty ();

	if (!m_propertyList.Create (WS_VISIBLE | WS_CHILD, rectDummy, this, 1))
	{
		TRACE0("Failed to create Properies Grid \n");
		return -1;      // fail to create
	}

	InitPropList ();
	AdjustLayout ();
	return 0;
}

void CHAPYPropertyBar::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);
	AdjustLayout ();
}

void CHAPYPropertyBar::InitPropList ()
{
	// Setup general look:
	m_propertyList.EnableHeaderCtrl(FALSE);
	m_propertyList.EnableToolBar(FALSE);
	m_propertyList.EnableSearchBox(FALSE);
	//m_wndPropList.EnableDescriptionArea();//底部说明
	//m_wndPropList.SetVSDotNetLook();//vs.net外观
	//m_wndPropList.MarkModifiedProperties();//告诉属性列表如何显示已修改的属性
	//m_wndPropList.EnableContextMenu();

	m_propertyList.SetFont(&theApp.m_font);
	CBCGPProp* pProp=NULL;
	//头
	CString cstr;
	cstr.LoadString(IDS_SystemProperty);
	CBCGPProp* pGroup1 = new CBCGPProp (cstr);
	int IDS[]={IDS_FileName,IDS_FileSize,IDS_CreationTime,IDS_Author,IDS_State,IDS_Version,
				IDS_Revision,IDS_CheckedInBy,IDS_CheckedIn,IDS_CheckedOutBy,IDS_CheckedOut};
	for(int i=0;i<(sizeof(IDS) / sizeof(IDS[0]));i++){
		cstr.LoadString(IDS[i]);
		pProp=new CBCGPProp (cstr, _T(""));
		pProp->Enable(FALSE);
		pGroup1->AddSubItem (pProp);
	}
	/*pProp=new CBCGPProp (_T("文件名称"), _T(""));
	pProp->Enable(FALSE);
	pGroup1->AddSubItem (pProp);

	pProp=new CBCGPProp (_T("文件类型"), _T(""));
	pProp->Enable(FALSE);
	pGroup1->AddSubItem (pProp);

	pProp=new CBCGPProp (_T("文件大小"), _T(""));
	pProp->Enable(FALSE);
	pGroup1->AddSubItem (pProp);

	pProp=new CBCGPProp (_T("作者"), _T(""));
	pProp->Enable(FALSE);
	pGroup1->AddSubItem (pProp);

	pProp=new CBCGPProp (_T("创建时间"), _T(""));
	pProp->Enable(FALSE);
	pGroup1->AddSubItem (pProp);

	pProp=new CBCGPProp (_T("版本"), _T(""));
	pProp->Enable(FALSE);
	pGroup1->AddSubItem (pProp);

	pProp=new CBCGPProp (_T("版次"), _T(""));
	pProp->Enable(FALSE);
	pGroup1->AddSubItem (pProp);

	pProp=new CBCGPProp (_T("生命周期状态"), _T(""));
	pProp->Enable(FALSE);
	pGroup1->AddSubItem (pProp);

	pProp=new CBCGPProp (_T("检出人"), _T(""));
	pProp->Enable(FALSE);
	pGroup1->AddSubItem (pProp);

	pProp=new CBCGPProp (_T("检出时间"), _T(""));
	pProp->Enable(FALSE);
	pGroup1->AddSubItem (pProp);
	pProp=new CBCGPProp (_T("检入人"), _T(""));
	pProp->Enable(FALSE);
	pGroup1->AddSubItem (pProp);

	pProp=new CBCGPProp (_T("检入时间"), _T(""));
	pProp->Enable(FALSE);
	pGroup1->AddSubItem (pProp);*/

	m_propertyList.AddProperty (pGroup1);
}

void CHAPYPropertyBar::ResetList(){
	this->m_propertyList.ResetOriginalValues(TRUE);


}
void CHAPYPropertyBar::LoadData(){
	CBCGPProp* pPropone=this->m_propertyList.GetProperty(0);
	CBCGPProp* pProp=NULL;
	CMainFrame*pMainFrame = DYNAMIC_DOWNCAST(CMainFrame,AfxGetMainWnd());
	CFolderListView*pfolderlistview = DYNAMIC_DOWNCAST(CFolderListView,pMainFrame->m_wndSplitter2.GetPane(0,0));
	CBCGPGridRow* pchild= pfolderlistview->m_listGrid.GetCurSel();
	CString cstr[11];
	cstr[0]=(CString)pchild->GetItem (listname)->GetValue();
	cstr[1]=(CString)pchild->GetItem (listsize)->GetValue();
	cstr[2]=(CString)pchild->GetItem (listcreatetime)->GetValue();
	cstr[3]=(CString)pchild->GetItem (listauthor)->GetValue();
	cstr[4]=(CString)pchild->GetItem (listlife)->GetValue();
	cstr[5]=(CString)pchild->GetItem (listversion)->GetValue();
	cstr[6]=(CString)pchild->GetItem (listrevision)->GetValue();
	cstr[7]=(CString)pchild->GetItem (listin)->GetValue();
	cstr[8]=(CString)pchild->GetItem (listintime)->GetValue();
	cstr[9]=(CString)pchild->GetItem (listout)->GetValue();
	cstr[10]=(CString)pchild->GetItem (listouttime)->GetValue();
	for(int i=0;i<11;i++){
		pProp=pPropone->GetSubItem(i);
		pProp->SetValue((_variant_t)(LPCTSTR)cstr[i]);
	}
}
void CHAPYPropertyBar::OnSetFocus(CWnd* pOldWnd) 
{
	CBCGPDockingControlBar::OnSetFocus(pOldWnd);
	m_propertyList.SetFocus ();
}
