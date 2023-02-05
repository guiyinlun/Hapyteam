// hapyteamclient.cpp : 定义应用程序的类行为。
#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "hapyteamclient.h"
#include "MainFrm.h"
#include "hapyteamclientDoc.h"
#include "LoginDlg.h"
#include "HAPYTheme.h"
#include <Core/Utility.h>
#include <boost/filesystem.hpp>
#include<Interface/HapyUserManager.h>
#include<Interface/HapyProjectManager.h>
#include "CustomMethods.h"
#include <Core/StringUtils.h>
#include "locale.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int  iMaxUserToolbars		= 10;
const UINT uiFirstUserToolBarId	= AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId	= uiFirstUserToolBarId + iMaxUserToolbars - 1;
// ChapyteamclientApp

BEGIN_MESSAGE_MAP(ChapyteamclientApp, CBCGPWinApp)
	ON_COMMAND(ID_APP_ABOUT, &ChapyteamclientApp::OnAppAbout)
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, CBCGPWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN,CBCGPWinApp::OnFileOpen)
END_MESSAGE_MAP()


// ChapyteamclientApp 构造

ChapyteamclientApp::ChapyteamclientApp()
{
	CBCGPVisualManagerVS2012::m_bAutoGrayscaleImages = FALSE;
	CBCGPVisualManagerVS2012::m_bThinSlider = TRUE;
	//初始化
	m_theme=0;
	m_languagecstr=L"chinese";
	m_loginstatus=FALSE;
	g_bRestart=FALSE;
	m_font.CreatePointFont(100, L"微软雅黑");
	m_thumbnailpath="";
	//建立缩略图文件夹
	std::wstring configpath=Utility::GetModulePath();
	configpath.append(_T("AppConfig.ini"));
	m_configpath=configpath.c_str();
	m_theme= GetPrivateProfileInt(TEXT("Config"), TEXT("theme"), 0, m_configpath);
	m_language=GetPrivateProfileInt(TEXT("Config"), TEXT("language"), 0, m_configpath);
	SetTheme(m_theme);
	setlocale(LC_CTYPE, "chs");

	//SetVisualTheme( BCGP_VISUAL_THEME_OFFICE_2016_WHITE,RUNTIME_CLASS(CHAPYTheme));
}
void ChapyteamclientApp::SetTheme(BOOL bRead)
{
	CHAPYTheme::SetStyle(bRead ?CHAPYTheme::HAPY_White:CHAPYTheme::HAPY_Black);
	CBCGPVisualManager::SetDefaultManager(RUNTIME_CLASS(CHAPYTheme));
}

// 唯一的一个 ChapyteamclientApp 对象

ChapyteamclientApp theApp;
// ChapyteamclientApp 初始化

BOOL ChapyteamclientApp::InitInstance()
{
	/*如果一个运行在 Windows XP 上的应用程序清单指定要
	使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	将它设置为包括所有要在应用程序中使用的
	公共控件类。*/
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CBCGPWinApp::InitInstance();
	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	//BCG程序加了
	globalData.m_bUseVisualManagerInBuiltInDialogs = TRUE;
	globalData.m_bUseDlgFontInControls = TRUE;
	globalData.m_bIsDlgWsCaptionStyle = TRUE;
	globalData.m_bIsMiniFrameWsCaptionStyle = TRUE;
	globalData.m_FrameCornerType = globalData.m_DlgCornerType = globalData.m_PopupMenuCornerType = BCGP_DWMWCP_ROUND;
	globalData.m_MiniFrameCornerType = BCGP_DWMWCP_ROUNDSMALL;

	AfxEnableControlContainer();
	//指定是否应启用了Windows 7任务栏的交互(TRUE)，或禁用(FALSE）。
	EnableTaskbarInteraction(FALSE);
	// 使用 RichEdit 控件需要  AfxInitRichEdit2()	
	// AfxInitRichEdit2();
	// 标准初始化
	SetRegistryKey(_T("hapyteam1"));// 程序用来存储配置信息的注册表值
	LoadStdProfileSettings(); // 加载标准的 INI 文件选项(包括 MRU)
	//SetRegistryBase (_T("Settings"));
	//读取配置文件
	int languagenum=GetPrivateProfileInt(TEXT("Config"), TEXT("language"), 0, m_configpath);
	// 加载对应的 DLL 文件
	/* 0-Chinese  1-English*/
	if(languagenum){
		m_hLangDLL=LoadLibrary((LPCWSTR)_T("English.dll"));
		if (m_hLangDLL!=NULL)
		{
			AfxSetResourceHandle(m_hLangDLL);
			m_languagecstr=L"english";
			//设置线程UI语言为英语
			LANGID idLang = SetThreadUILanguage(MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT));		
		}
		//SetThreadLocale(idLang);
		//SetThreadLocale(MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT));
	}
	 
	//---------------------------------------
	if(HapyUserManager::GetInstance()->CurrentUser()==NULL){
		CLoginDlg m_logindlg;
		if(m_logindlg.DoModal()==IDOK){
			m_loginstatus=TRUE;
		}else exit(0);
	}
	
	//---------------------------------------
	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(ChapyteamclientDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(CProjectTreeView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
	// 分析标准 shell 命令、DDE、打开文件操作的命令行
	//m_nCmdShow = SW_SHOWMAXIMIZED;  
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	LoadLastProject();
	return TRUE;
}

int ChapyteamclientApp::ExitInstance()
{
	//保存配置到配置文件.ini
	CString value;
	value.Format(_T("%d"),m_theme);
	WritePrivateProfileString(TEXT("Config"), TEXT("theme"), value, m_configpath);
	value.Format(_T("%d"),m_language);
	WritePrivateProfileString(TEXT("Config"), TEXT("language"), value, m_configpath);
	//保存上次项目
	if (HapyProjectManager::GetInstance()->CurrentProject() !=NULL)
	{
		HapyProject* curProject= HapyProjectManager::GetInstance()->CurrentProject();
		CString cstr(curProject->ProjectName().c_str());
		WritePrivateProfileString(TEXT("Config"), TEXT("projectname"), cstr, m_configpath);
	}
	//删除缩略图
	wstring tempPath = Utility::getTempPath();
	std::string sThumbPath = StringUtils::WString2String(tempPath);
	sThumbPath.append("Thums");
	boost::filesystem::remove_all(sThumbPath);
	//清除状态
	int isclean= GetPrivateProfileInt(TEXT("Config"), TEXT("isclean"), 0, m_configpath);
	if(isclean==0){
		CleanState();
		value.Format(_T("%d"),1);
		WritePrivateProfileString(TEXT("Config"), TEXT("isclean"), value, m_configpath);
	}
	//TODO: 处理可能已添加的附加资源
	AfxOleTerm(FALSE);

	if(m_hLangDLL)
	{
		FreeLibrary(m_hLangDLL);
	}
	if (g_bRestart)
	{
		STARTUPINFO StartupInfo={0};
		PROCESS_INFORMATION ProcessInfo;

		StartupInfo.cb=sizeof(STARTUPINFO);
		char Path[256];
		GetModuleFileName(NULL,(LPWSTR)(LPCTSTR)Path,250);
		CreateProcess(NULL,(LPWSTR)(LPCTSTR)Path,NULL,NULL,FALSE,0,NULL,NULL,&StartupInfo,&ProcessInfo);
	}
	g_bRestart = FALSE;
	return CBCGPWinApp::ExitInstance();
}
// ChapyteamclientApp 消息处理程序


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CBCGPDialog
{
public:
	CAboutDlg();
	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CBCGPDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CBCGPDialog)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void ChapyteamclientApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// ChapyteamclientApp 自定义加载/保存方法

void ChapyteamclientApp::PreLoadState()
{
	//GetContextMenuManager()->AddMenu(_T("My folderlistmenu"), IDR_POPUP_EDIT);
	//GetContextMenuManager()->AddMenu(_T("My structtreemenu"), IDR_TreeMENU);
}

 void ChapyteamclientApp::LoadLastProject(){
	 CString projectname;
	 GetPrivateProfileString(L"Config", L"projectname", L"",
		 projectname.GetBuffer(MAX_PATH),MAX_PATH,m_configpath);
	 string str=StringUtils::CStringToString(projectname);

	 vector<HapyProject*> projs=HapyProjectManager::GetInstance()->GetProjects();
	 HapyProjectManager::GetInstance()->SwitchProject(StringUtils::CStringToString(projectname));
	 if (HapyProjectManager::GetInstance()->CurrentProject()!=NULL)
	 {
		 BeginWaitCursor();
		 CMainFrame*pMainFrame = DYNAMIC_DOWNCAST(CMainFrame,AfxGetMainWnd());
		 CProjectTreeView*pprojecttreeview = DYNAMIC_DOWNCAST(CProjectTreeView,pMainFrame->m_wndSplitter1.GetPane(0,0));
		 CFolderListView*pfolderlistview = DYNAMIC_DOWNCAST(CFolderListView,pMainFrame->m_wndSplitter2.GetPane(0,0));
		 HapyProject* curProject= HapyProjectManager::GetInstance()->CurrentProject();
		 curProject->Refresh();
		 if(pMainFrame->isIconLoad==FALSE){
			 if(m_theme)pMainFrame->OnWhiteTheme();
			 else pMainFrame->OnBlackTheme();
			 pMainFrame->isIconLoad=TRUE;
		 }
		 pMainFrame->addHAPYIcon();
		 pMainFrame->CreateViewLink();BeginWaitCursor();
		 pprojecttreeview->ResetTree();
		 pprojecttreeview->LoadData(curProject->ProjectRoot());
		 pfolderlistview->ResetBreadcrumb();
		 pfolderlistview->LoadBreadcrumb(curProject->ProjectRoot());
		 EndWaitCursor();
		 pprojecttreeview->m_treeGrid.SelectItem(pprojecttreeview->m_treeGrid.GetRootItem());
	 }
 }
// ChapyteamclientApp 消息处理程序
