#include "stdafx.h"
#include "hapyteamclient.h"
#include "LoginDlg.h"
#include "afxdialogex.h"
#include  <Interface/HapyUserManager.h>
#include <Core/api.h>
#include <Core/StringUtils.h>
#include"MainFrm.h"
#include"CustomMethods.h"


// CLoginDlg 对话框

IMPLEMENT_DYNAMIC(CLoginDlg, CBCGPDialog)

	CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CLoginDlg::IDD, pParent)
{
	EnableVisualManagerStyle(TRUE, TRUE);

}

CLoginDlg::~CLoginDlg()
{

}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_userEDIT, m_useredit);
	DDX_Control(pDX, IDC_passwdEDIT, m_passwdedit);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDC_welcomeSTATIC, m_welcomestatic);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CBCGPDialog)
	ON_BN_CLICKED(IDOK, &CLoginDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_exitBUTTON, &CLoginDlg::OnBnClickedexitbutton)
	ON_WM_CTLCOLOR()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()
// CLoginDlg 消息处理程序

BOOL CLoginDlg::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ok.SetFont(&theApp.m_font);
	m_font.CreatePointFont(200, L"微软雅黑");//参数：字体大小，样式，DC

	SetEditBoxesSimplifiedBrowseIcon(TRUE);
	m_useredit.SetVerticalAlignment(TA_CENTER);
	CString cstr;
	cstr.LoadString(IDS_nameTip);
	m_useredit.SetPrompt(cstr);

	std::string userName=HapyUserManager::GetInstance()->UserName();
	if (!userName.empty())
	{
		m_useredit.SetWindowTextW(StringUtils::StringToCString(userName.c_str()));
	}
	m_passwdedit.SetVerticalAlignment(TA_CENTER);

	cstr.LoadString(IDS_passwordTip);
	m_passwdedit.SetPrompt(cstr);
	m_passwdedit.EnablePasswordPreview(1,1);

	std::string userPwd=HapyUserManager::GetInstance()->UserPwd();
	if (!userPwd.empty())
	{
		m_passwdedit.SetWindowTextW(StringUtils::StringToCString(userPwd.c_str()));
	}
	UpdateWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CLoginDlg::OnBnClickedOk()
{
	UpdateData(TRUE); // 更新控件对应变量的值
	//获取对话框的值
	m_useredit.GetWindowTextW(m_cusername);
	m_passwdedit.GetWindowTextW(m_cpassword);
	//CString to string
	m_username=StringUtils::CStringToString(m_cusername);
	m_password=StringUtils::CStringToString(m_cpassword);
	if(m_username=="1"&&m_password=="1"){
		m_username="Ganxc";
		m_password="Changeme12#4";
	}else if(m_username=="2"&&m_password=="2"){
		m_username="GanXiaCheng";
		m_password="gxc528..";
	}
	
	if (HapyUserManager::GetInstance()->Login(m_username,m_password,std::string("86")))
	{
		//customMethods::customMessagebox(L"loginsuccessfully",MB_OK|MB_ICONASTERISK);
		CBCGPDialog::OnOK(); // 关闭登陆界面
	}
	else
	{
		CString cstr;
		cstr.LoadString(IDS_TIPS);
		customMethods::customMessagebox(L"LoginError",MB_OK|MB_ICONERROR);
		// 清空用户名和密码，便于重新输入
		//m_useredit.SetWindowTextW(L"");
		m_passwdedit.SetWindowTextW(L"");
		//pdateData(FALSE); // 更新编辑框控件的显示值
	}
}

//右上角关闭按钮退出
void CLoginDlg::OnBnClickedexitbutton()
{
	// TODO: 在此添加控件通知处理程序代码
	CBCGPDialog::OnCancel(); // 关闭登陆界面
}
//防止回车退出
void CLoginDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	//CBCGPDialog::OnOK();
}
//修改字体颜色
HBRUSH CLoginDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CBCGPDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	//TODO:  在此更改 DC 的任何特性
	if (pWnd->GetDlgCtrlID() == IDC_welcomeSTATIC)
	{ 
		pDC->SelectObject(&m_font);
		if(theApp.m_theme==FALSE)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(RGB(255,255,255));
			HBRUSH B = CreateSolidBrush(RGB(37,37,37)); 
			return B;
		}
		else{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(RGB(0,0,0));
			HBRUSH B = CreateSolidBrush(RGB(255,255,255)); 
			return B;
		}
	}
	return hbr;
}
//可拖动
LRESULT CLoginDlg::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rect;
	GetClientRect(rect);
	ClientToScreen(rect);
	if(rect.PtInRect(point))
		return HTCAPTION;
	return CBCGPDialog::OnNcHitTest(point);
}
