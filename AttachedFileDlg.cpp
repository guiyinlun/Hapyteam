// addReference.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "hapyteamclient.h"
#include "AttachedFileDlg.h"
#include "afxdialogex.h"
#include <Interface/HapyProjectManager.h>


// addReference �Ի���

IMPLEMENT_DYNAMIC(CAttachedFileDlg, CBCGPDialog)

CAttachedFileDlg::CAttachedFileDlg(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CAttachedFileDlg::IDD, pParent)
{
	EnableVisualManagerStyle(TRUE, TRUE);
	EnableLayout();
}

CAttachedFileDlg::~CAttachedFileDlg()
{
}

void CAttachedFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAttachedFileDlg, CBCGPDialog)
	ON_BN_CLICKED(IDOK, &CAttachedFileDlg::OnBnClickedOk)
	ON_NOTIFY(TVN_SELCHANGED, IDC_addTREE, &CProjectTreeView::OnSelchangedTree)
END_MESSAGE_MAP()


// addReference ��Ϣ�������

BOOL CAttachedFileDlg::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();
	//tree
	UINT nTreeStyles = WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | TVS_LINESATROOT |
		TVS_HASLINES | //����
		TVS_HASBUTTONS | //�а�ť
		TVS_SHOWSELALWAYS|
		TVS_TRACKSELECT |
		TVS_FULLROWSELECT; //����ѡ��
	m_treeGrid.CreateOnPlaceHolder(this, IDC_AddreferenceTree, 0, nTreeStyles);

	//List
	UINT nListStyles = WS_CHILD | WS_VISIBLE  | WS_BORDER | 
		TVS_SHOWSELALWAYS|
		TVS_FULLROWSELECT; //����ѡ��
	m_listGrid.CreateOnPlaceHolder(this, IDC_AddReferenceList, 0, nListStyles);
	CBCGPStaticLayout* pLayout = (CBCGPStaticLayout*)GetLayout();
	if (pLayout != NULL)
	{
		pLayout->AddAnchor(m_treeGrid.GetSafeHwnd(), 
			CBCGPStaticLayout::e_MoveTypeNone, CBCGPStaticLayout::e_SizeTypeBoth);
		pLayout->AddAnchor(m_listGrid.GetSafeHwnd(), 
			CBCGPStaticLayout::e_MoveTypeNone, CBCGPStaticLayout::e_SizeTypeBoth);
	}
	//Tree
	m_treeGrid.EnableHeader(FALSE,  BCGP_GRID_HEADER_SORT);
	m_treeGrid.EnableColumnAutoSize(TRUE);
	m_treeGrid.EnableGridLines(FALSE);
	m_treeGrid.EnableAlternateRows(FALSE);
	m_treeGrid.SetCustomRowHeight(38,38);//�����п��ڶ�����������
	m_treeGrid.SetFont(&theApp.m_font);
	m_treeGrid.SetVisualManagerColorTheme(TRUE);
	//List
	m_listGrid.EnableHeader (TRUE, BCGP_GRID_HEADER_SELECT);
	m_listGrid.EnableColumnAutoSize(TRUE); //����������Ӧ���
	m_listGrid.EnableGridLines(TRUE);//��ʾgridline
	m_listGrid.EnableRowHeader(TRUE);
	m_listGrid.EnableAlternateRows(FALSE);//���ý�����
	m_listGrid.SetReadOnly ();
	m_listGrid.SetWholeRowSel ();//����
	m_listGrid.SetCustomRowHeight(38,38);//�����п��ڶ�����������
	m_listGrid.SetFont(&theApp.m_font);
	m_listGrid.SetVisualManagerColorTheme(TRUE);
	//��ӱ�ͷ
	m_listGrid.RemoveAll();
	m_listGrid.DeleteAllColumns();
	int onewidth=100;
	m_listGrid.InsertColumn(0, _T("�ļ�����"), onewidth);
	m_listGrid.InsertColumn(1 , _T("��������״̬"), onewidth);
	m_listGrid.InsertColumn(2 , _T("�汾"), onewidth);	
	m_listGrid.InsertColumn(3, _T("���"), onewidth);	


	m_treeGrid.AdjustLayout();
	m_listGrid.AdjustLayout();
	m_treeGrid.RedrawWindow ();
	m_listGrid.RedrawWindow ();
	return TRUE;  
}

void CAttachedFileDlg::ResetTree(){
	this->m_treeGrid.DeleteAllItems();
	this->m_treeGrid.DeleteAllColumns();
	this->m_treeGrid.InsertColumn(0, _T("��Ŀ"), 100);
	/*this->m_treeGrid.InsertColumn(1, _T(""), 20);
	this->m_treeGrid.SetColumnVisible(1,FALSE);*/
}

void CAttachedFileDlg::AddTreeNode(HapyProjectFolder* projectFolder,HTREEITEM parentItem)
{
	/*CString	 name(projectFolder->Name().c_str());
	HTREEITEM m_item = this->m_treeGrid.InsertItem(name,Wfolder,Wfolder,parentItem);

	this->m_treeGrid.SetItemData(m_item,(DWORD_PTR)projectFolder);
	vector<HapyProjectFolder*> subFolders=projectFolder->SubFolders();
	if(subFolders.size()>0){
		for (vector<HapyProjectFolder*>::iterator it =subFolders.begin();it!=subFolders.end();it++ )
		{
			HapyProjectFolder* childFolder= *it;
			AddTreeNode(childFolder,m_item);
		}
		this->m_treeGrid.Expand(m_item,TVE_COLLAPSE);
	}*/
}
void CAttachedFileDlg::LoadTreeData(){
	ResetTree();
	AddTreeNode(HapyProjectManager::GetInstance()->CurrentProject()->ProjectRoot(),NULL);
}
void CAttachedFileDlg::LoadListData(){}
//ѡ��
void CAttachedFileDlg::OnTvnSelchangedaddtree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
		*pResult = 0;

}
void CAttachedFileDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CBCGPDialog::OnOK();
}