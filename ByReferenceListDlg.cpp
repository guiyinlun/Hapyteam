// Referenced.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "hapyteamclient.h"
#include "ByReferenceListDlg.h"
#include "afxdialogex.h"


// CReferenced �Ի���

IMPLEMENT_DYNAMIC(CByReferenceListDlg, CBCGPDialog)

CByReferenceListDlg::CByReferenceListDlg(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CByReferenceListDlg::IDD, pParent)
{
	EnableVisualManagerStyle(TRUE, TRUE);
	EnableLayout();
}

CByReferenceListDlg::~CByReferenceListDlg()
{
}

void CByReferenceListDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_referencedTREE, m_tree);
	DDX_Control(pDX, IDC_referencedCOMBO, m_combo);
}


BEGIN_MESSAGE_MAP(CByReferenceListDlg, CBCGPDialog)
END_MESSAGE_MAP()


// CReferenced ��Ϣ�������


BOOL CByReferenceListDlg::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CRect thisrect;
	this->GetParent()->GetClientRect(&thisrect);
	// λ�ô�С
	m_combo.SetWindowPos(this,0,5,150,100,SWP_NOZORDER);
	m_tree.SetWindowPos(this,0,30,thisrect.Width(),thisrect.Height(),SWP_NOZORDER);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
