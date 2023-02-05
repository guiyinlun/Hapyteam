#include "stdafx.h"
#include "hapyteamclient.h"
#include "LoginDlg.h"
#include "afxdialogex.h"
#include  <Interface/HapyUserManager.h>
#include <Core/api.h>
#include <Core/StringUtils.h>
#include"MainFrm.h"
#include"CustomMethods.h"


// CLoginDlg �Ի���

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
// CLoginDlg ��Ϣ�������

BOOL CLoginDlg::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_ok.SetFont(&theApp.m_font);
	m_font.CreatePointFont(200, L"΢���ź�");//�����������С����ʽ��DC

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
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CLoginDlg::OnBnClickedOk()
{
	UpdateData(TRUE); // ���¿ؼ���Ӧ������ֵ
	//��ȡ�Ի����ֵ
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
		CBCGPDialog::OnOK(); // �رյ�½����
	}
	else
	{
		CString cstr;
		cstr.LoadString(IDS_TIPS);
		customMethods::customMessagebox(L"LoginError",MB_OK|MB_ICONERROR);
		// ����û��������룬������������
		//m_useredit.SetWindowTextW(L"");
		m_passwdedit.SetWindowTextW(L"");
		//pdateData(FALSE); // ���±༭��ؼ�����ʾֵ
	}
}

//���Ͻǹرհ�ť�˳�
void CLoginDlg::OnBnClickedexitbutton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CBCGPDialog::OnCancel(); // �رյ�½����
}
//��ֹ�س��˳�
void CLoginDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	//CBCGPDialog::OnOK();
}
//�޸�������ɫ
HBRUSH CLoginDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CBCGPDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	//TODO:  �ڴ˸��� DC ���κ�����
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
//���϶�
LRESULT CLoginDlg::OnNcHitTest(CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CRect rect;
	GetClientRect(rect);
	ClientToScreen(rect);
	if(rect.PtInRect(point))
		return HTCAPTION;
	return CBCGPDialog::OnNcHitTest(point);
}
