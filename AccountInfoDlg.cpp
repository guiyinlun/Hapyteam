// Account.cpp : 实现文件
//

#include "stdafx.h"
#include "hapyteamclient.h"
#include "AccountInfoDlg.h"
#include "afxdialogex.h"
#include <Interface/HapyUserManager.h>
#include <Core/StringUtils.h>


// CAccount 对话框

IMPLEMENT_DYNAMIC(CAccountInfoDlg, CBCGPDialog)

CAccountInfoDlg::CAccountInfoDlg(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CAccountInfoDlg::IDD, pParent)
{
	EnableVisualManagerStyle(TRUE, TRUE);
}

CAccountInfoDlg::~CAccountInfoDlg()
{
}

void CAccountInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_userinfoEDIT, m_username);
	DDX_Control(pDX, IDC_passwdinfoEDIT, m_password);
	DDX_Control(pDX, IDC_phoneinfoEDIT, m_phone);
	DDX_Control(pDX, IDC_emailinfoEDIT, m_email);
}


BEGIN_MESSAGE_MAP(CAccountInfoDlg, CBCGPDialog)
END_MESSAGE_MAP()


// CAccount 消息处理程序


BOOL CAccountInfoDlg::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();


	CBCGPStaticLayout* pLayout = (CBCGPStaticLayout*)GetLayout();
	if (pLayout != NULL)
	{
		pLayout->AddAnchor(IDC_usernameS, CBCGPStaticLayout::e_MoveTypeBoth, CBCGPStaticLayout::e_SizeTypeNone);
		pLayout->AddAnchor(IDC_passwordS, CBCGPStaticLayout::e_MoveTypeBoth, CBCGPStaticLayout::e_SizeTypeNone);
		pLayout->AddAnchor(IDC_phoneS, CBCGPStaticLayout::e_MoveTypeBoth, CBCGPStaticLayout::e_SizeTypeNone);
		pLayout->AddAnchor(IDC_emailS, CBCGPStaticLayout::e_MoveTypeBoth, CBCGPStaticLayout::e_SizeTypeNone);
		pLayout->AddAnchor(IDC_userinfoEDIT, CBCGPStaticLayout::e_MoveTypeBoth, CBCGPStaticLayout::e_SizeTypeNone);
		pLayout->AddAnchor(IDC_passwdinfoEDIT, CBCGPStaticLayout::e_MoveTypeBoth, CBCGPStaticLayout::e_SizeTypeNone);
		pLayout->AddAnchor(IDC_phoneinfoEDIT, CBCGPStaticLayout::e_MoveTypeBoth, CBCGPStaticLayout::e_SizeTypeNone);
		pLayout->AddAnchor(IDC_emailinfoEDIT, CBCGPStaticLayout::e_MoveTypeBoth, CBCGPStaticLayout::e_SizeTypeNone);
	}
	//EDIT垂直居中显示
	SetFont(&theApp.m_font);
	SetEditBoxesVerticalAlignment(TA_CENTER);
		myinfo=HapyUserManager::GetInstance()->CurrentUser();
	if(myinfo!=NULL)
	{
		username=myinfo->UserName();
		userphone=myinfo->UserPhone();
		useremail=myinfo->UserEmail();
		CString cusername(username.c_str());
		CString cuserphone(userphone.c_str());
		CString cuseremail(useremail.c_str());

		m_username.SetWindowTextW(cusername);
		m_password.SetWindowTextW(L"●●●●●●");
		m_phone.SetWindowTextW(cuserphone);
		m_email.SetWindowTextW(cusername);
	}
	UpdateWindow();
	return TRUE; 
}

