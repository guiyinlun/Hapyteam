// Myformview.cpp : 实现文件
#include "stdafx.h"
#include "hapyteamclient.h"
#include "FileDetailsLayoutView.h"
#include "FolderListView.h"
#include "CustomMethods.h"

// CMyTabView
IMPLEMENT_DYNCREATE(CFileDetailsLayoutView, CBCGPFormView)

	CFileDetailsLayoutView::CFileDetailsLayoutView()
	: CBCGPFormView(CFileDetailsLayoutView::IDD)
{
	EnableVisualManagerStyle();
	EnableLayout();
	m_curSelTab=0;
	this->ResetIsLoad();	
}

CFileDetailsLayoutView::~CFileDetailsLayoutView()
{
}

void CFileDetailsLayoutView::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFileDetailsLayoutView, CBCGPFormView)	
	ON_MESSAGE(WM_INITDIALOG, HandleInitDialog)
	ON_REGISTERED_MESSAGE(BCGM_CHANGING_ACTIVE_TAB, OnChangingActiveTab)
	ON_REGISTERED_MESSAGE(BCGM_CHANGE_ACTIVE_TAB, OnChangeActiveTab)
END_MESSAGE_MAP()


// CMyTabView 诊断

#ifdef _DEBUG
void CFileDetailsLayoutView::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFileDetailsLayoutView::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyTabView 消息处理程序

LRESULT CFileDetailsLayoutView::HandleInitDialog(WPARAM wParam, LPARAM lParam){
	BOOL bRes = (BOOL)CBCGPFormView::HandleInitDialog(wParam, lParam);
	m_wndTab.CreateOnPlaceHolder(this, IDC_MYTab,0);
	m_wndTab.SetDialogControl();
	CRect thisrect,viewrect;
	this->GetClientRect(&viewrect);
	thisrect.SetRect(0,0,viewrect.right,viewrect.bottom);
	m_wndTab.MoveWindow(&thisrect);
	CBCGPStaticLayout* pLayout = (CBCGPStaticLayout*)GetLayout();
	if (pLayout != NULL)
	{
		pLayout->AddAnchor(m_wndTab.GetSafeHwnd(), 
			CBCGPStaticLayout::e_MoveTypeNone, CBCGPStaticLayout::e_SizeTypeBoth);

	}
	return bRes;
}
void CFileDetailsLayoutView::OnInitialUpdate()
{
	CBCGPFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();	
	//风格
	CBCGPTabWnd::Style style;
	style = CBCGPTabWnd::STYLE_UNDERLINE;
	m_wndTab.ModifyTabStyle (style);
	//字体
	m_wndTab.SetFont(&theApp.m_font);
	//标签页位置
	CBCGPTabWnd::Location location;
	location = CBCGPTabWnd::LOCATION_TOP;/*CBCGPTabWnd::LOCATION_BOTTOM;*/
	m_wndTab.SetLocation (location);
	//交换
	m_wndTab.EnableTabSwap(FALSE);
	OnReset();
	//保存当前选择
	m_curSelTab = 0;
	m_wndTab.RecalcLayout ();
	m_wndTab.RedrawWindow ();
}
void CFileDetailsLayoutView::OnReset(){
	m_wndTab.RemoveAllTabs();
	CRect thisrect;
	m_wndTab.GetClientRect(&thisrect); 
	//创建
	m_pversionDlg.Create(IDD_VersionsDlg,&m_wndTab);
	m_preferenceDlg.Create(IDD_ReferenceDlg,&m_wndTab);
	m_pbyrelerenceDlg.Create(IDD_ByReferenceList,&m_wndTab);
	m_previsionDlg.Create(IDD_RevisionListDlg,&m_wndTab);
	m_pannotateDlg.Create(IDD_ReadCircleAnnotateDlg,&m_wndTab);
	//大小
	m_pversionDlg.MoveWindow(&thisrect);
	m_preferenceDlg.MoveWindow(&thisrect);
	m_pbyrelerenceDlg.MoveWindow(&thisrect);
	m_previsionDlg.MoveWindow(&thisrect);
	m_pannotateDlg.MoveWindow(&thisrect);
	//添加
	CString cstr;
	cstr.LoadString(IDS_Version);
	cstr=L"  "+cstr+L"  ";
	m_wndTab.AddTab (&m_pversionDlg,cstr);
	cstr.LoadString(IDS_Uses);
	cstr=L"  "+cstr+L"  ";
	m_wndTab.AddTab (&m_preferenceDlg, cstr);
	cstr.LoadString(IDS_WhereUsed);
	cstr=L"  "+cstr+L"  ";
	m_wndTab.AddTab (&m_pbyrelerenceDlg, cstr);
	cstr.LoadString(IDS_ChangeOrder);
	cstr=L"  "+cstr+L"  ";
	m_wndTab.AddTab (&m_previsionDlg, cstr);
	cstr.LoadString(IDS_Annotate);
	cstr=L"  "+cstr+L"  ";
	m_wndTab.AddTab (&m_pannotateDlg, cstr);


	m_wndTab.SetActiveTab(0);
	m_wndTab.RecalcLayout ();
	m_wndTab.RedrawWindow ();
	UpdateData(FALSE);
}
LRESULT CFileDetailsLayoutView::OnChangingActiveTab(WPARAM wp, LPARAM /*lp*/)
{
	m_curSelTab=(int) wp;
	CMainFrame*pMainFrame = DYNAMIC_DOWNCAST(CMainFrame,AfxGetMainWnd());
	CFolderListView*pfolderlistview = DYNAMIC_DOWNCAST(CFolderListView,pMainFrame->m_wndSplitter2.GetPane(0,0));
	HapyProjectFile* selectFile=pfolderlistview->m_pselectFile;
	if(selectFile==NULL) return 0;
	BeginWaitCursor();
	switch (this->m_curSelTab)
	{
	case 0:
		if(isLoad[0]!=TRUE){
			m_pversionDlg.ResetList();
			m_pversionDlg.LoadData(selectFile);
			isLoad[0]=TRUE;
		}
		break;
	case 1:
		if(isLoad[1]!=TRUE){
			m_preferenceDlg.ResetComboAndTree();
			m_preferenceDlg.LoadCombo(selectFile);
			m_preferenceDlg.LoadData(selectFile,selectFile->FileVersion());
			isLoad[1]=TRUE;
		}
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	}
	EndWaitCursor();
	return 0;
}
LRESULT CFileDetailsLayoutView::OnChangeActiveTab(WPARAM wp, LPARAM /*lp*/)
{
	m_curSelTab=(int) wp;
	return 0;
}

void CFileDetailsLayoutView::ResetIsLoad(){
	for(int i=0;i<5;i++){
		this->isLoad[i]=FALSE;
	}
}
