// VersionupDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "hapyteamclient.h"
#include "RevisionListDlg.h"
#include "afxdialogex.h"


// CVersionupDlg �Ի���

IMPLEMENT_DYNAMIC(CRevisionListDlg, CBCGPDialog)

CRevisionListDlg::CRevisionListDlg(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CRevisionListDlg::IDD, pParent)
{
	EnableVisualManagerStyle(TRUE, TRUE);
	EnableLayout();
}

CRevisionListDlg::~CRevisionListDlg()
{
}

void CRevisionListDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_versionupLIST, m_list);
}


BEGIN_MESSAGE_MAP(CRevisionListDlg, CBCGPDialog)
END_MESSAGE_MAP()


// CVersionupDlg ��Ϣ�������


BOOL CRevisionListDlg::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CRect thisrect;
	this->GetParent()->GetClientRect(&thisrect);
	// λ�ô�С
	m_list.SetWindowPos(this,0,0,thisrect.Width(),thisrect.Height(),SWP_NOZORDER);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
