// Basic.cpp : 实现文件
//

#include "stdafx.h"
#include "hapyteamclient.h"
#include "BasicInfoDlg.h"
#include "afxdialogex.h"



// CBasic 对话框

IMPLEMENT_DYNAMIC(CBasicInfoDlg, CBCGPDialog)

CBasicInfoDlg::CBasicInfoDlg(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CBasicInfoDlg::IDD, pParent)
{
	EnableVisualManagerStyle(TRUE, TRUE);

}

CBasicInfoDlg::~CBasicInfoDlg()
{
}

void CBasicInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBasicInfoDlg, CBCGPDialog)
END_MESSAGE_MAP()


// CBasic 消息处理程序
