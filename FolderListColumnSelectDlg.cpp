// Listcomsel.cpp : 实现文件
//

#include "stdafx.h"
#include "hapyteamclient.h"
#include "FolderListColumnSelectDlg.h"
#include "afxdialogex.h"


// CListcomsel 对话框

IMPLEMENT_DYNAMIC(CFolderListColumnSelectDlg, CBCGPDialog)

	CFolderListColumnSelectDlg::CFolderListColumnSelectDlg(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CFolderListColumnSelectDlg::IDD, pParent)
{
	EnableVisualManagerStyle(TRUE, TRUE);
}

CFolderListColumnSelectDlg::~CFolderListColumnSelectDlg()
{
}

void CFolderListColumnSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFolderListColumnSelectDlg, CBCGPDialog)
	ON_REGISTERED_MESSAGE(BCGM_GRID_HEADERCHECKBOX_CLICK, OnHeaderCheckClick)
	ON_REGISTERED_MESSAGE(BCGM_GRID_ROW_CHECKBOX_CLICK, OnCheckClick)
	ON_BN_CLICKED(IDC_reset, &CFolderListColumnSelectDlg::OnBnClickedreset)
	ON_BN_CLICKED(IDOK, &CFolderListColumnSelectDlg::OnBnClickedOk)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

BOOL CFolderListColumnSelectDlg::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化
	CRect thisrect;	
	// 位置大小
	UINT nTreeStyles = WS_CHILD | WS_VISIBLE |
		/*TVS_SHOWSELALWAYS|*/
		TVS_CHECKBOXES  |
		TVS_FULLROWSELECT; //整行选中
	m_treeGrid.CreateOnPlaceHolder(this, IDD_FolderListColumnSelectList, 0, nTreeStyles);

	m_treeGrid.EnableHeader (TRUE, BCGP_GRID_HEADER_SELECT);
	m_treeGrid.EnableColumnAutoSize(TRUE); //设置列自适应宽度
	m_treeGrid.EnableAlternateRows(TRUE);//启用交替行
	//m_list.EnableRowHeader(true,-1,-1);
	m_treeGrid.SetVisualManagerColorTheme(TRUE);
	m_treeGrid.SetFont(&theApp.m_font);
	m_treeGrid.DeleteAllItems();
	m_treeGrid.DeleteAllColumns();
	CString cstr;
	cstr.LoadString(IDS_selectAll);
	m_treeGrid.InsertColumn(0,cstr,20);
	cstr.LoadString(IDS_stateicon);
	m_treeGrid.InsertItem(cstr);
	cstr.LoadString(IDS_fileicon);
	m_treeGrid.InsertItem(cstr);
	for(int i=103;i<116;i++){
		cstr.LoadString(i);
		m_treeGrid.InsertItem(cstr);
	}
	/*m_treeGrid.InsertItem(_T("文件名称"));
	m_treeGrid.InsertItem(_T("文件类型"));
	m_treeGrid.InsertItem(_T("文件大小"));
	m_treeGrid.InsertItem(_T("作者"));
	m_treeGrid.InsertItem(_T("创建时间"));
	m_treeGrid.InsertItem(_T("版本"));
	m_treeGrid.InsertItem(_T("版次"));
	m_treeGrid.InsertItem(_T("生命周期状态"));
	m_treeGrid.InsertItem(_T("检出人"));
	m_treeGrid.InsertItem(_T("检出时间"));
	m_treeGrid.InsertItem(_T("检入人"));
	m_treeGrid.InsertItem(_T("检入时间"));
	m_treeGrid.InsertItem(_T("备注"));*/

	init();
	m_treeGrid.LoadState (_T("list"));
	m_treeGrid.AdjustLayout();
	m_treeGrid.RedrawWindow ();
	return TRUE;  
	// 异常: OCX 属性页应返回 FALSE
}
// CListcomsel 消息处理程序
void CFolderListColumnSelectDlg::init()
{
	CBCGPGridRow* pRow=NULL;
	CMainFrame*pMainFrame = DYNAMIC_DOWNCAST(CMainFrame,AfxGetMainWnd());
	CFolderListView*pfolderlistview = DYNAMIC_DOWNCAST(CFolderListView,pMainFrame->m_wndSplitter2.GetPane(0,0));
	for(int i=0;i<15;i++){
		pRow=m_treeGrid.GetRow(i);
		pRow->SetCheck(pfolderlistview->m_listGrid.GetColumnVisible(i));
	}
	pRow->UpdateParentCheckbox(TRUE);
	m_treeGrid.UpdateHeaderCheckbox();
}
LRESULT CFolderListColumnSelectDlg::OnHeaderCheckClick(WPARAM, LPARAM)
{
	BOOL bChecked = m_treeGrid.GetColumnsInfo().GetCheckBoxState();

	m_treeGrid.GetColumnsInfo().SetCheckBoxState(!bChecked);
	m_treeGrid.CheckAll(!bChecked);

	return TRUE; // disable the default implementation
}
LRESULT CFolderListColumnSelectDlg::OnCheckClick(WPARAM, LPARAM lp)
{
	CBCGPGridRow* pRow = (CBCGPGridRow*)lp;
	if (pRow == NULL)
	{
		return 0;
	}
	ASSERT_VALID(pRow);

	if (pRow->HasCheckBox())
	{
		BOOL bChecked = pRow->GetCheck();

		// check the row
		pRow->SetCheck(!bChecked);

		// check the subitems
		pRow->CheckSubItems(!bChecked);

		// update parent items
		pRow->UpdateParentCheckbox(TRUE/* use 3 state checkboxes */);

		// update the header control
		m_treeGrid.UpdateHeaderCheckbox();
	}

	return TRUE; // disable the default implementation
}
void CFolderListColumnSelectDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame*pMainFrame = DYNAMIC_DOWNCAST(CMainFrame,AfxGetMainWnd());
	CFolderListView*pfolderlistview = DYNAMIC_DOWNCAST(CFolderListView,pMainFrame->m_wndSplitter2.GetPane(0,0));
	CBCGPGridRow* pRow=NULL;
	for(int i=0;i<15;i++){
		pRow=m_treeGrid.GetRow(i);
		BOOL bChecked = pRow->GetCheck();
		pfolderlistview->m_listGrid.SetColumnVisible(i,bChecked);
	}
	pfolderlistview->m_listGrid.AdjustLayout();
	CBCGPDialog::OnOK();
}
void CFolderListColumnSelectDlg::OnBnClickedreset()
{
	// TODO: 在此添加控件通知处理程序代码
	bool bChecked=FALSE;
	m_treeGrid.GetColumnsInfo().SetCheckBoxState(!bChecked);
	m_treeGrid.CheckAll(!bChecked);
}




void CFolderListColumnSelectDlg::OnDestroy()
{
	m_treeGrid.SaveState (_T("list"));
	CBCGPDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}
