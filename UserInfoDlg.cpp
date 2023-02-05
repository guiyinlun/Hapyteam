// UserDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "hapyteamclient.h"
#include "UserInfoDlg.h"
#include "afxdialogex.h"



// CUserDlg �Ի���

IMPLEMENT_DYNAMIC(CUserInfoDlg, CBCGPDialog)

	CUserInfoDlg::CUserInfoDlg(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CUserInfoDlg::IDD, pParent)
{
	EnableVisualManagerStyle(TRUE, TRUE);
	//EnableLayout();
}

CUserInfoDlg::~CUserInfoDlg()
{
}

void CUserInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUserInfoDlg, CBCGPDialog)
END_MESSAGE_MAP()


// CUserDlg ��Ϣ�������

BOOL CUserInfoDlg::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_wndTab.CreateOnPlaceHolder(this, IDC_userinfoTAB, 0);
	CBCGPStaticLayout* pLayout = (CBCGPStaticLayout*)GetLayout();
	if (pLayout != NULL)
	{
		pLayout->AddAnchor(m_wndTab.GetSafeHwnd(), 
			CBCGPStaticLayout::e_MoveTypeNone, 
			CBCGPStaticLayout::e_SizeTypeBoth);
	}
	OnReset();
	//���
	CBCGPTabWnd::Style style;
	style = CBCGPTabWnd::STYLE_UNDERLINE;
	m_wndTab.ModifyTabStyle (style);
	//����
	
	m_wndTab.SetFont(&theApp.m_font);

	//��ǩҳλ��
	CBCGPTabWnd::Location location;
	location = CBCGPTabWnd::LOCATION_TOP;/*CBCGPTabWnd::LOCATION_BOTTOM;*/
	m_wndTab.SetLocation (location);

	m_wndTab.RecalcLayout ();
	m_wndTab.RedrawWindow ();
	
	return TRUE;  

}
void CUserInfoDlg::OnReset(){
	m_wndTab.RemoveAllTabs();
	CRect thisrect;
	m_wndTab.GetClientRect(&thisrect); 

	//����
	m_account.Create(IDD_AccountDlg,&m_wndTab);
	m_basic.Create(IDD_BasicDlg,&m_wndTab);
	//��С
	m_account.MoveWindow(&thisrect);
	m_basic.MoveWindow(&thisrect);
	//���
	CString cstr;
	cstr.LoadString(IDS_AccountInformation);
	m_wndTab.AddTab (&m_account, cstr);
	cstr.LoadString(IDS_EssentialInformation);
	m_wndTab.AddTab (&m_basic, cstr);

	m_wndTab.SetActiveTab(0);

	m_wndTab.RecalcLayout ();
	m_wndTab.RedrawWindow ();
	UpdateData(FALSE);
}

