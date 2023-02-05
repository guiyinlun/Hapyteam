// VersionupDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "hapyteamclient.h"
#include "RevisionListDlg.h"
#include "afxdialogex.h"


// CVersionupDlg 对话框

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


// CVersionupDlg 消息处理程序


BOOL CRevisionListDlg::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect thisrect;
	this->GetParent()->GetClientRect(&thisrect);
	// 位置大小
	m_list.SetWindowPos(this,0,0,thisrect.Width(),thisrect.Height(),SWP_NOZORDER);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
