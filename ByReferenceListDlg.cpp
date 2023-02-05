// Referenced.cpp : 实现文件
//

#include "stdafx.h"
#include "hapyteamclient.h"
#include "ByReferenceListDlg.h"
#include "afxdialogex.h"


// CReferenced 对话框

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


// CReferenced 消息处理程序


BOOL CByReferenceListDlg::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect thisrect;
	this->GetParent()->GetClientRect(&thisrect);
	// 位置大小
	m_combo.SetWindowPos(this,0,5,150,100,SWP_NOZORDER);
	m_tree.SetWindowPos(this,0,30,thisrect.Width(),thisrect.Height(),SWP_NOZORDER);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
