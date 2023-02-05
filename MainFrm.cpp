#include "stdafx.h"
#include "MainFrm.h"
#include "hapyteamclient.h"
#include "hapyteamclientDoc.h"
#include "ProjectTreeView.h"
#include "FolderListView.h"
#include "FileDetailsLayoutView.h"
#include "ProjectListDlg.h"
#include "LoginDlg.h"
#include "FolderListColumnSelectDlg.h"
#include "CustomMethods.h"
#include "UserInfoDlg.h"
#include <Core/Utility.h>
#include <Models/UserInfo.h>
#include <Interface/HapyObject.h>
#include <Interface/HapyProjectManager.h>
#include <Interface/HapyProject.h>
#include <boost/filesystem.hpp>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif
DownloadInfo::DownloadInfo(){
	m_downloadtype=FALSE;
	m_selectfileid=0;
	m_projectid=0;
	m_downloadpath="";	
}
// CMainFrame
IMPLEMENT_DYNCREATE(CMainFrame, CBCGPFrameWnd)

	BEGIN_MESSAGE_MAP(CMainFrame, CBCGPFrameWnd)
		ON_WM_CREATE()			
		ON_WM_CLOSE()
		ON_WM_SIZE()		
		ON_WM_DESTROY()
		ON_COMMAND(ID_Menu_Login, &CMainFrame::OnLogin)
		ON_COMMAND(ID_Menu_UserInfo, &CMainFrame::OnUserInfo)
		ON_COMMAND(ID_Menu_Logout, &CMainFrame::OnLogout)
		ON_COMMAND(ID_Menu_ProjectList, &CMainFrame::OnProjectList)
		ON_COMMAND(ID_Menu_WhiteTheme, &CMainFrame::OnWhiteTheme)
		ON_COMMAND(ID_Menu_BlackTheme, &CMainFrame::OnBlackTheme)
		ON_COMMAND(ID_Menu_FileListColumnSelect, &CMainFrame::OnFolderListColumnSelect)
		ON_COMMAND(ID_Menu_PropertiesBar, &CMainFrame::OnPropertiesViewBar)
		ON_COMMAND(ID_Menu_Chinese, &CMainFrame::OnChinese)
		ON_COMMAND(ID_Menu_English, &CMainFrame::OnEnglish)
		ON_UPDATE_COMMAND_UI(ID_Menu_Logout, &CMainFrame::OnUpdateLogout)
		ON_UPDATE_COMMAND_UI(ID_Menu_Login, &CMainFrame::OnUpdateLogin)
		ON_UPDATE_COMMAND_UI(ID_Menu_UserInfo, &CMainFrame::OnUpdateUserInfo)
		ON_UPDATE_COMMAND_UI(ID_Menu_BlackTheme, &CMainFrame::OnUpdateBlackTheme)
		ON_UPDATE_COMMAND_UI(ID_Menu_WhiteTheme, &CMainFrame::OnUpdateWhiteTheme)
		ON_UPDATE_COMMAND_UI(ID_Menu_ProjectList, &CMainFrame::OnUpdateprojectList)
		ON_UPDATE_COMMAND_UI(ID_Menu_English, &CMainFrame::OnUpdateMenuEnglish)
		ON_UPDATE_COMMAND_UI(ID_Menu_Chinese, &CMainFrame::OnUpdateMenuChinese)
	END_MESSAGE_MAP()

	static UINT indicators[] =
	{
		ID_SEPARATOR,           // ״̬��ָʾ��
		IDS_USERNAME,
	};

	// CMainFrame ����/����

	CMainFrame::CMainFrame()
	{
		m_projectid=0;
		m_workfolderpath="";/*����Ŀ¼*/
		isviewlink=FALSE;
		isIconLoad=FALSE;
		pprojecttreeview=NULL;
		pfolderlistview=NULL;
		pdetailsview=NULL;
		//����ͼ��
		hIcon[unknow] = theApp.LoadIcon(IDI_unknow); 
		hIcon[Wfolder] = theApp.LoadIcon(IDI_Wfolder);  
		hIcon[Yfolder] = theApp.LoadIcon(IDI_Yfolder);   
		hIcon[empty] = theApp.LoadIcon(IDI_empty); 
		hIcon[checkoutBySelf] = theApp.LoadIcon(IDI_checkoutBySelfW); 
		hIcon[checkoutBySomeOne] = theApp.LoadIcon(IDI_checkOutBySomeOneW); 
		hIcon[filecheckin] = theApp.LoadIcon(IDI_filecheckinW); 
		hIcon[localfilenotmatch] = theApp.LoadIcon(IDI_localfilenotmatch); 
		hIcon[fileArchived] = theApp.LoadIcon(IDI_FileArchivedW);
		hIcon[zip]=theApp.LoadIcon(IDI_zip);
		hIcon[ipt]=theApp.LoadIcon(IDI_ipt);
		hIcon[doc]=theApp.LoadIcon(IDI_word);
		hIcon[docx]=theApp.LoadIcon(IDI_word);
		hIcon[vsd]=theApp.LoadIcon(IDI_vsd);
		hIcon[txt]=theApp.LoadIcon(IDI_txt);
		hIcon[ses]=theApp.LoadIcon(IDI_smart3D);
		hIcon[sldprt]=theApp.LoadIcon(IDI_sldprt);
		hIcon[sldasm]=theApp.LoadIcon(IDI_sldasm);
		hIcon[rvt]=theApp.LoadIcon(IDI_rvt);
		hIcon[rvm]=theApp.LoadIcon(IDI_rvm);
		hIcon[rtf]=theApp.LoadIcon(IDI_rtf);
		hIcon[rfa]=theApp.LoadIcon(IDI_rfa);
		hIcon[prt]=theApp.LoadIcon(IDI_prt);
		hIcon[ppt]=theApp.LoadIcon(IDI_ppt);
		hIcon[pptx]=theApp.LoadIcon(IDI_ppt);
		hIcon[png]=theApp.LoadIcon(IDI_png);
		hIcon[one]=theApp.LoadIcon(IDI_onenote);
		hIcon[pdf]=theApp.LoadIcon(IDI_pdf);
		hIcon[nwf]=theApp.LoadIcon(IDI_nwf);
		hIcon[nwc]=theApp.LoadIcon(IDI_nwc);
		hIcon[mpp]=theApp.LoadIcon(IDI_mpp);
		hIcon[jpg]=theApp.LoadIcon(IDI_jpg);
		hIcon[ipj]=theApp.LoadIcon(IDI_ipj);
		hIcon[gif]=theApp.LoadIcon(IDI_gif);
		hIcon[xlsx]=theApp.LoadIcon(IDI_excel);
		hIcon[xls]=theApp.LoadIcon(IDI_excel);
		hIcon[dxf]=theApp.LoadIcon(IDI_dxf);
		hIcon[dwg]=theApp.LoadIcon(IDI_dwg);
		hIcon[dgn]=theApp.LoadIcon(IDI_dgn);
		hIcon[csv]=theApp.LoadIcon(IDI_csv);
		hIcon[catproduct]=theApp.LoadIcon(IDI_catproduct);
		hIcon[catpart]=theApp.LoadIcon(IDI_catpart);
		hIcon[assembly]=theApp.LoadIcon(IDI_assembly);
		hIcon[myasm]=theApp.LoadIcon(IDI_asm);
		hIcon[iam]=theApp.LoadIcon(IDI_iam);
		hIcon[idw]=theApp.LoadIcon(IDI_idw);
		hIcon[slddrw]=theApp.LoadIcon(IDI_slddrw);
		addHAPYIcon();
	}

	CMainFrame::~CMainFrame()
	{
	}


	int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (CBCGPFrameWnd::OnCreate(lpCreateStruct) == -1)
			return -1;
		//BOOL bNameValid;
		CBCGPToolBar::EnableQuickCustomization();


		// Menu will not take the focus on activation:
		CBCGPPopupMenu::SetForceMenuFocus(FALSE);
		if (!m_wndMenuBar.Create(this))
		{
			TRACE0("δ�ܴ����˵���\n");
			return -1;      // δ�ܴ���
		}
		m_wndMenuBar.SetFont(&theApp.m_font);
		m_wndMenuBar.SetBarStyle(m_wndMenuBar.GetBarStyle() | CBRS_SIZE_DYNAMIC);

		//m_wndMenuBar.EnablePopupMode(true);
		if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
			!m_wndToolBar.LoadToolBar(IDR_MAINFRAME, 0, 0, FALSE, 0, 0, IDB_TOOLBAR_HC))
		{
			TRACE0("Failed to create toolbar\n");
			return -1;      // fail to create
		}


		if (!m_wndStatusBar.Create(this) ||
			!m_wndStatusBar.SetIndicators(indicators,
			sizeof(indicators)/sizeof(UINT)))
		{
			TRACE0("δ�ܴ���״̬��\n");
			return -1;      // δ�ܴ���
		}
		m_wndStatusBar.SetFont(&theApp.m_font);
		UserInfo* curUser= HapyUserManager::GetInstance()->CurrentUser();
		if(curUser !=NULL){
			std::string username=curUser->UserName();
			CString welcome;
			welcome.LoadString(IDS_WelcomeUser);
			CString str=welcome+L": "+username.c_str();
			m_wndStatusBar.SetPaneText(1, str); 
		}	

		// Load control bar icons:
		CBCGPToolBarImages imagesWorkspace;
		imagesWorkspace.SetImageSize(CSize(16, 16));
		imagesWorkspace.Load(IDB_WORKSPACE);
		globalUtils.ScaleByDPI(imagesWorkspace);

		const int nPaneSize = globalUtils.ScaleByDPI(300);

		CString cstr;
		cstr.LoadString(IDS_FileProperties);
		if (!m_wndPropBar.Create(cstr, this, CRect(0, 0, 250, 250),
			TRUE, 
			ID_VIEW_PROPERTIES,
			WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | 
			WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
		{
			TRACE0("Failed to create Properties Bar\n");
			return FALSE;		// fail to create
		}


		//����
		m_wndMenuBar.SetSizes(CSize (40, 40), CSize (16, 16)); 

		CString strMainToolbarTitle;
		strMainToolbarTitle.LoadString(IDS_MAIN_TOOLBAR);
		m_wndToolBar.SetWindowText(strMainToolbarTitle);

		// TODO: Delete these three lines if you don't want the toolbar to be dockable
		m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
		m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
		m_wndPropBar.EnableDocking(CBRS_ALIGN_ANY);

		EnableDocking(CBRS_ALIGN_ANY);
		EnableAutoHideBars(CBRS_ALIGN_ANY);

		DockControlBar(&m_wndMenuBar);
		//DockControlBar(&m_wndToolBar);
		DockControlBar(&m_wndPropBar);
		m_wndPropBar.SetAutoHideMode(TRUE,CBRS_RIGHT,FALSE);
		m_wndToolBar.ShowWindow(FALSE);
		return 0;
	}

	BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,CCreateContext* pContext)
	{
		// ������ִ���
		CRect rc;
		this->GetClientRect(&rc);
		if (m_wndSplitter1.CreateStatic(this, 1, 2) == NULL)
			return FALSE;
		if (!m_wndSplitter1.CreateView(0, 0, RUNTIME_CLASS(CProjectTreeView), CSize(rc.Width() / 5, rc.Height()), pContext)) {

			m_wndSplitter1.DestroyWindow();
			return FALSE;
		}
		if (m_wndSplitter2.CreateStatic(&m_wndSplitter1, 2, 1, WS_CHILD | WS_VISIBLE, m_wndSplitter1.IdFromRowCol(0, 1)) == NULL)
			return FALSE; 
		if(!m_wndSplitter2.CreateView(0, 0, RUNTIME_CLASS(CFolderListView), CSize(rc.Width()/5*4, (rc.Height())/5*2), pContext)||
			!m_wndSplitter2.CreateView(1, 0, RUNTIME_CLASS(CFileDetailsLayoutView), CSize(rc.Width() / 5 * 4, (rc.Height())/5*3), pContext) ){

				m_wndSplitter2.DestroyWindow();
				return FALSE;
		}
		m_wndSplitter1.SetBorderSize(1);
		m_wndSplitter2.SetBorderSize(1);

		return TRUE;
	}

	BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
	{
		if( !CBCGPFrameWnd::PreCreateWindow(cs) )
			return FALSE;
		//
		return TRUE;
	}

	// CMainFrame ���

#ifdef _DEBUG
	void CMainFrame::AssertValid() const
	{
		CBCGPFrameWnd::AssertValid();
	}

	void CMainFrame::Dump(CDumpContext& dc) const
	{
		CBCGPFrameWnd::Dump(dc);
	}
#endif //_DEBUG


	// CMainFrame ��Ϣ�������
	CFolderListView* CMainFrame::GetRightPane()
	{
		CWnd* pWnd = m_wndSplitter2.GetPane(0, 0);
		CFolderListView* pView = DYNAMIC_DOWNCAST(CFolderListView, pWnd);
		return pView;
	}

	LRESULT CMainFrame::OnToolbarReset(WPARAM /*wp*/,LPARAM)
	{
		// TODO: reset toolbar with id = (UINT) wp to its initial state:
		//
		// UINT uiToolBarId = (UINT) wp;
		// if (uiToolBarId == IDR_MAINFRAME)
		// {
		//		do something with m_wndToolBar
		// }

		return 0;
	}
	void CMainFrame::ActivateFrame(int nCmdShow)
	{
		// TODO: �ڴ����ר�ô����/����û���
		nCmdShow=SW_SHOWMAXIMIZED;
		CBCGPFrameWnd::ActivateFrame(nCmdShow);
	}
	//����
	void CMainFrame::OnLogin()
	{
		// TODO: �ڴ���������������
		CLoginDlg m_logindlg;
		if(m_logindlg.DoModal()== IDOK){
			//����״̬
			theApp.m_loginstatus=TRUE;
			UserInfo* curUser= HapyUserManager::GetInstance()->CurrentUser();
			if(curUser!=NULL){
				string username=curUser->UserName();
				CString welcome;
				welcome.LoadString(IDS_WelcomeUser);
				CString str=welcome+L": "+username.c_str();
				m_wndStatusBar.SetPaneText(1, str); 

			}	
		}
	}
	//��Ŀ
	void CMainFrame::OnProjectList()
	{
		/*�ж��û��Ƿ��¼*/
		if(HapyUserManager::GetInstance()->CurrentUser() ==NULL){
			return;
		}
		//�´���Ŀ׼���׶�
		m_projectid=0;
		//����Ŀ�б�Ի���
		CProjectList m_prolistdlg;
		if(m_prolistdlg.DoModal()==IDOK){
			BeginWaitCursor();
			HapyProject* curProject= HapyProjectManager::GetInstance()->CurrentProject();
			if (curProject!=NULL)
			{
				
					curProject->Refresh();
				
				
				if(isIconLoad==FALSE){
					if(theApp.m_theme)this->OnWhiteTheme();
					else this->OnBlackTheme();
					isIconLoad=TRUE;
				}
				addHAPYIcon();
				this->CreateViewLink();
				pfolderlistview->m_listGrid.RemoveAll();
				pfolderlistview->m_listGrid.AdjustLayout();

				BeginWaitCursor();
				HapyProjectFolder* RootFolder=curProject->ProjectRoot();

				//tree
				pprojecttreeview->ResetTree();
				pprojecttreeview->LoadData(RootFolder);
				//Breadcrumb
				pfolderlistview->ResetBreadcrumb();
				pfolderlistview->LoadBreadcrumb(RootFolder);
				EndWaitCursor();
				pprojecttreeview->m_treeGrid.SelectItem(pprojecttreeview->m_treeGrid.GetRootItem());
				
			}
		}
	}
	//�û���Ϣ
	void CMainFrame::OnUserInfo()
	{
		// TODO: �ڴ���������������
		if(HapyUserManager::GetInstance()->CurrentUser() ==NULL){
			return;
		}
		CUserInfoDlg m_userdlg;
		m_userdlg.DoModal();

	}
	//�ǳ�
	void CMainFrame::OnLogout()
	{
		// TODO: �ڴ���������������

		if(HapyUserManager::GetInstance()->Logout()){
			this->CreateViewLink();
			//delete
			pprojecttreeview->m_treeGrid.DeleteAllItems();
			pprojecttreeview->m_treeGrid.DeleteAllColumns();
			pfolderlistview->m_listGrid.RemoveAll();
			pfolderlistview->m_Breadcrumb.DeleteAllItem();
			//ˢ��tabҳ
			pfolderlistview->m_pversionDlg->m_listGrid.RemoveAll();
			pfolderlistview->m_preferenceDlg->m_treeGrid.DeleteAllItems();
			pfolderlistview->m_preferenceDlg->m_Combo.ResetContent();
			m_wndStatusBar.SetPaneText(1, _T("")); 
			//�ǳ�״̬
			theApp.m_loginstatus=FALSE;
			//customMethods::customMessagebox(L"logoutsuccessfully",MB_OK|MB_ICONASTERISK); 

		}else	
			customMethods::customMessagebox(L"logoutfailure",MB_OK|MB_ICONERROR);

	}
	//�ر�-2
	void CMainFrame::OnClose()
	{

		/*CFolderListView* plistview = DYNAMIC_DOWNCAST(CFolderListView,this->m_wndSplitter2.GetPane(0,0));
		CFileDetailsLayoutView*pdetailsView = DYNAMIC_DOWNCAST(CFileDetailsLayoutView,this->m_wndSplitter2.GetPane(1,0));
		if(plistview->m_pVersionThread){
		pdetailsView->m_pversionDlg.m_listGrid.exitflag=FALSE;
		WaitForSingleObject(plistview->m_pVersionThread->m_hThread,INFINITE);
		}
		if(plistview->m_pRelerenceThread){
		pdetailsView->m_preferenceDlg.m_treeGrid.exitflag=FALSE;	
		WaitForSingleObject(plistview->m_pRelerenceThread->m_hThread,INFINITE);
		}*/
		CBCGPFrameWnd::OnClose();
	}
	void CMainFrame::OnDestroy()
	{
		//�˳���ʱ��ǳ�
		HapyUserManager::GetInstance()->Logout();
		CBCGPFrameWnd::OnDestroy();
	}
	//���ñ���
	void CMainFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
	{
		CBCGPFrameWnd::OnUpdateFrameTitle(bAddToTitle);
		CString csAppName;
		csAppName.Format(AFX_IDS_APP_TITLE);
		SetWindowText(_T("HAPYTEAM"));
	}
	//OnSize
	void CMainFrame::OnSize(UINT nType, int cx, int cy)
	{
		CBCGPFrameWnd::OnSize(nType, cx, cy);
		// TODO: �ڴ˴������Ϣ����������
		if(m_wndSplitter1&&m_wndSplitter2)
		{
			CRect thisrect,viewrect;
			this->GetClientRect(&viewrect);
			// �������ڻ��ֱ��� 
			//Splitter1
			m_wndSplitter1.SetColumnInfo(0, (int)(viewrect.Width() * 0.2), 50);//�ı��С
			m_wndSplitter1.SetRowInfo(0, (int)viewrect.Height(), 50);

			//Splitter2
			m_wndSplitter2.SetColumnInfo(0, (int)(viewrect.Width() * 0.8), 50);//�ı��С
			m_wndSplitter2.SetRowInfo(0, (int)viewrect.Height()/5*3, 50);
			m_wndSplitter2.SetRowInfo(1, (int)viewrect.Height()/5*2, 50);
			// �����������������Ժ����»��Ʒָ���, ��ֹ��С�󣬷ָ���λ�ô���
			m_wndSplitter1.RecalcLayout();
			m_wndSplitter2.RecalcLayout();

		}

	}
	//ǳɫ
	void CMainFrame::OnWhiteTheme()
	{
		CProjectTreeView* pprojecttreeview = DYNAMIC_DOWNCAST(CProjectTreeView,m_wndSplitter1.GetPane(0,0));
		CFolderListView* pfolderlistview = DYNAMIC_DOWNCAST(CFolderListView,m_wndSplitter2.GetPane(0,0));
		CFileDetailsLayoutView* pdetailsview = DYNAMIC_DOWNCAST(CFileDetailsLayoutView,m_wndSplitter2.GetPane(1,0));
		theApp.SetTheme(TRUE);
		theApp.m_theme=TRUE;
		//�����ض�����
		pdetailsview->m_pversionDlg.m_listGrid.SetVisualManagerColorTheme(TRUE);
		pdetailsview->m_preferenceDlg.m_treeGrid.SetVisualManagerColorTheme(TRUE);
		hIcon[Wfolder] = theApp.LoadIcon(IDI_Bfolder); 
		hIcon[checkoutBySelf] = theApp.LoadIcon(IDI_checkoutBySelfB); 
		hIcon[filecheckin] = theApp.LoadIcon(IDI_filecheckinB); 
		hIcon[checkoutBySomeOne] = theApp.LoadIcon(IDI_checkOutBySomeOneB); 
		hIcon[fileArchived] = theApp.LoadIcon(IDI_FileArchivedB);
		addHAPYIcon();
		pprojecttreeview->m_treeGrid.AdjustLayout();
		pfolderlistview->m_listGrid.AdjustLayout();
	}
	//��ɫ
	void CMainFrame::OnBlackTheme()
	{
		CProjectTreeView*pprojecttreeview = DYNAMIC_DOWNCAST(CProjectTreeView,m_wndSplitter1.GetPane(0,0));
		CFolderListView*pfolderlistview = DYNAMIC_DOWNCAST(CFolderListView,m_wndSplitter2.GetPane(0,0));
		CFileDetailsLayoutView*pdetailsview = DYNAMIC_DOWNCAST(CFileDetailsLayoutView,m_wndSplitter2.GetPane(1,0));
		theApp.SetTheme(FALSE);
		theApp.m_theme=FALSE;
		//�����ض�����
		pdetailsview->m_pversionDlg.m_listGrid.SetVisualManagerColorTheme(TRUE);
		pdetailsview->m_preferenceDlg.m_treeGrid.SetVisualManagerColorTheme(TRUE);
		hIcon[Wfolder] = theApp.LoadIcon(IDI_Wfolder); 
		hIcon[checkoutBySelf] = theApp.LoadIcon(IDI_checkoutBySelfW); 
		hIcon[filecheckin] = theApp.LoadIcon(IDI_filecheckinW); 
		hIcon[checkoutBySomeOne] = theApp.LoadIcon(IDI_checkOutBySomeOneW); 
		hIcon[fileArchived] = theApp.LoadIcon(IDI_FileArchivedW);
		addHAPYIcon();
		pprojecttreeview->m_treeGrid.AdjustLayout();
		pfolderlistview->m_listGrid.AdjustLayout();

	}
	//���ͼ��
	void CMainFrame::addHAPYIcon()
	{
		m_ImageList.DeleteImageList();
		m_ImageList.Create(16, 16, ILC_COLOR32|ILC_MASK,thisend,1);
		for (int i=0; i<thisend; i++)   
		{   
			m_ImageList.Add(hIcon[i]);  	
		}
	}
	//�ļ���������ѡ��
	void CMainFrame::OnFolderListColumnSelect()
	{
		// TODO: �ڴ���������������
		CFolderListColumnSelectDlg m_listcomsel;
		m_listcomsel.DoModal();

	}
	//���Ա�
	void CMainFrame::OnPropertiesViewBar()
	{
		// TODO: �ڴ���������������
		m_wndPropBar.ShowControlBar(TRUE,FALSE,FALSE);
		m_wndPropBar.SetAutoHideMode(TRUE,CBRS_RIGHT,FALSE);
	}
	//�л�����
	void CMainFrame::OnChinese()
	{
		CString strLoad;
		strLoad.LoadString(IDS_TIPS);
		int i=customMethods::customMessagebox(strLoad,L"Really change to Chinese version?",MB_OKCANCEL|MB_ICONQUESTION);
		if(i==1){
			theApp.m_language=0;
		}
		theApp.g_bRestart=TRUE;
		//PostQuitMessage(0);
		AfxGetMainWnd()->SendMessage(WM_CLOSE);
	}
	//�л�Ӣ��
	void CMainFrame::OnEnglish()
	{
		CString strLoad;
		strLoad.LoadString(IDS_TIPS);
		int i=customMethods::customMessagebox(strLoad,L"ȷ���л���Ӣ�İ汾?",MB_OKCANCEL|MB_ICONQUESTION);
		if(i==1){	
			theApp.m_language=1;
		}
		theApp.g_bRestart=TRUE;
		//PostQuitMessage(0);
		AfxGetMainWnd()->SendMessage(WM_CLOSE);
	}
	BOOL CMainFrame::OnShowPopupMenu(CBCGPPopupMenu* pMenuPopup){


		return CBCGPFrameWnd::OnShowPopupMenu(pMenuPopup);
	}
	void CMainFrame::CreateViewLink(){
		if(isviewlink==FALSE){
			//��������ͼ������
			pprojecttreeview = DYNAMIC_DOWNCAST(CProjectTreeView,m_wndSplitter1.GetPane(0,0));
			pfolderlistview = DYNAMIC_DOWNCAST(CFolderListView,m_wndSplitter2.GetPane(0,0));
			pdetailsview = DYNAMIC_DOWNCAST(CFileDetailsLayoutView,m_wndSplitter2.GetPane(1,0));
			CVersionListDlg*		pversiondlg =&pdetailsview->m_pversionDlg;
			CReferenceListDlg*		m_preferencedlg =&pdetailsview->m_preferenceDlg;
			CByReferenceListDlg*	pbyrelerencedlg =&pdetailsview->m_pbyrelerenceDlg;	
			CRevisionListDlg*		previsiondlg =&pdetailsview->m_previsionDlg;
			CAnnotateDlg*			pannotatedlg =&pdetailsview->m_pannotateDlg;
			/*projecttreeview*/
			pprojecttreeview->m_pfolderlistview=pfolderlistview;
			pprojecttreeview->m_pdetailsview=pdetailsview;
			/*folderlistview*/
			pfolderlistview->m_pprojecttreeview=pprojecttreeview;
			pfolderlistview->m_pdetailsView=pdetailsview;
			pfolderlistview->m_pversionDlg=pversiondlg ;
			pfolderlistview->m_preferenceDlg=m_preferencedlg;
			pfolderlistview->m_pbyrelerenceDlg=pbyrelerencedlg;
			pfolderlistview->m_previsionDlg=previsiondlg;
			pfolderlistview->m_pannotateDlg=pannotatedlg;
			isviewlink=TRUE;
		}
	}
	//�����ò˵�״̬�������õı�ң�
	void CMainFrame::OnUpdateLogin(CCmdUI *pCmdUI)
	{
		// TODO: �ڴ������������û����洦��������
		pCmdUI->Enable(!theApp.m_loginstatus);
	}
	void CMainFrame::OnUpdateLogout(CCmdUI *pCmdUI)
	{
		pCmdUI->Enable(theApp.m_loginstatus);
	}
	void CMainFrame::OnUpdateUserInfo(CCmdUI *pCmdUI)
	{
		// TODO: �ڴ������������û����洦��������
		pCmdUI->Enable(theApp.m_loginstatus);
	}
	void CMainFrame::OnUpdateprojectList(CCmdUI *pCmdUI)
	{
		// TODO: �ڴ������������û����洦��������
		pCmdUI->Enable(theApp.m_loginstatus);
	}
	void CMainFrame::OnUpdateBlackTheme(CCmdUI *pCmdUI)
	{
		// TODO: �ڴ������������û����洦��������
		pCmdUI->Enable(theApp.m_theme);
		pCmdUI->SetCheck(!theApp.m_theme);
	}
	void CMainFrame::OnUpdateWhiteTheme(CCmdUI *pCmdUI)
	{
		// TODO: �ڴ������������û����洦��������
		pCmdUI->Enable(!theApp.m_theme);
		pCmdUI->SetCheck(theApp.m_theme);
	}
	void CMainFrame::OnUpdateMenuChinese(CCmdUI *pCmdUI)
	{
		pCmdUI->Enable(theApp.m_language);
		pCmdUI->SetCheck(!theApp.m_language);
	}
	void CMainFrame::OnUpdateMenuEnglish(CCmdUI *pCmdUI)
	{
		pCmdUI->Enable(!theApp.m_language);
		pCmdUI->SetCheck(theApp.m_language);
	}
	
