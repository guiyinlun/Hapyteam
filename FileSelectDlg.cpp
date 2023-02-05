
#include "stdafx.h"
#include "hapyteamclient.h"
#include "FileSelectDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "FolderListView.h"


IMPLEMENT_DYNAMIC(CFileSelectDlg, CBCGPDialog)

	CFileSelectDlg::CFileSelectDlg(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CFileSelectDlg::IDD, pParent)
{
	EnableVisualManagerStyle(TRUE, TRUE);
	EnableLayout();
	m_pfolderlistview=NULL;
}

CFileSelectDlg::~CFileSelectDlg()
{
}

void CFileSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFileSelectDlg, CBCGPDialog)
	ON_REGISTERED_MESSAGE(BCGM_GRID_HEADERCHECKBOX_CLICK, OnHeaderCheckClick)
	//ON_REGISTERED_MESSAGE(BCGM_GRID_ROW_CHECKBOX_CLICK, OnCheckClick)
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_FileSelectTree, &CFileSelectDlg::OnItemExpanding)
	ON_BN_CLICKED(IDC_OKFileSelect, &CFileSelectDlg::OnBnClickedOkfileselect)
	ON_BN_CLICKED(IDC_CancelFileSelect, &CFileSelectDlg::OnBnClickedCancelfileselect)
END_MESSAGE_MAP()


BOOL CFileSelectDlg::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();
	UINT nTreeStyles = WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | TVS_LINESATROOT |
		TVS_HASLINES | //����
		TVS_HASBUTTONS | //�а�ť
		TVS_CHECKBOXES  |//
		TVS_SHOWSELALWAYS|
		TVS_FULLROWSELECT; //����ѡ��
	m_treeGrid.CreateOnPlaceHolder(this, IDC_FileSelectTree, 0, nTreeStyles);
	CBCGPStaticLayout* pLayout = (CBCGPStaticLayout*)GetLayout();
	if (pLayout != NULL)
	{
		pLayout->AddAnchor(m_treeGrid.GetSafeHwnd(), CBCGPStaticLayout::e_MoveTypeNone, CBCGPStaticLayout::e_SizeTypeBoth);
		pLayout->AddAnchor(IDC_OKFileSelect, CBCGPStaticLayout::e_MoveTypeBoth, CBCGPStaticLayout::e_SizeTypeNone);
		pLayout->AddAnchor(IDC_CancelFileSelect, CBCGPStaticLayout::e_MoveTypeBoth, CBCGPStaticLayout::e_SizeTypeNone);

	}

	//����
	m_treeGrid.EnableHeader (TRUE, BCGP_GRID_HEADER_SELECT);
	m_treeGrid.EnableColumnAutoSize(TRUE); //����������Ӧ���
	m_treeGrid.EnableGridLines(False);//��ʾgridline
	m_treeGrid.EnableAlternateRows(FALSE);//���ý�����
	m_treeGrid.SetReadOnly ();
	m_treeGrid.SetWholeRowSel ();//����
	m_treeGrid.SetCustomRowHeight(35,35);//�����п��ڶ�����������

	m_treeGrid.SetVisualManagerColorTheme(TRUE);
	m_treeGrid.SetFont(&theApp.m_font);

	//Column
	m_treeGrid.DeleteAllItems();
	m_treeGrid.DeleteAllColumns();
	int m_width=100;
	CString cstr;
	m_treeGrid.InsertColumn(listcheck, _T(""), m_width);
	m_treeGrid.InsertColumn(listicon, _T(""), m_width);
	for(int i=103;i<116;i++){
		cstr.LoadString(i);
		m_treeGrid.InsertColumn(i-101,cstr,m_width);
	}
	/*m_treeGrid.InsertColumn(listcheck, _T(""), m_width*2);
	m_treeGrid.InsertColumn(listicon, _T(""), m_width);
	m_treeGrid.InsertColumn(listname, _T("����"), m_width);
	m_treeGrid.InsertColumn(listtype, _T("����"), m_width);
	m_treeGrid.InsertColumn(listsize, _T("��С"), m_width);
	m_treeGrid.InsertColumn(listauthor , _T("����"), m_width);
	m_treeGrid.InsertColumn(listcreatetime, _T("����ʱ��"), m_width);
	m_treeGrid.InsertColumn(listversion , _T("�汾"), m_width);	
	m_treeGrid.InsertColumn(listrevision, _T("���"), m_width);
	m_treeGrid.InsertColumn(listlife , _T("��������״̬"), m_width);
	m_treeGrid.InsertColumn(listout , _T("�����"), m_width);
	m_treeGrid.InsertColumn(listouttime , _T("���ʱ��"), m_width);
	m_treeGrid.InsertColumn(listin, _T("������"), m_width);
	m_treeGrid.InsertColumn(listintime , _T("����ʱ��"), m_width);
	m_treeGrid.InsertColumn(listnote , _T("��ע"), m_width);*/
	m_treeGrid.SetColumnVisible(listcheck,FALSE);
	m_treeGrid.SetColumnVisible(listicon,FALSE);
	m_treeGrid.SetColumnVisible(listcreatetime,FALSE);
	//m_treeGrid.SetColumnVisible(listout,FALSE);
	//m_treeGrid.SetColumnVisible(listouttime,FALSE);
	m_treeGrid.SetColumnVisible(listnote,FALSE);
	CMainFrame*pMainFrame = DYNAMIC_DOWNCAST(CMainFrame,AfxGetMainWnd());
	m_pfolderlistview = DYNAMIC_DOWNCAST(CFolderListView,pMainFrame->m_wndSplitter2.GetPane(0,0));
	this->m_treeGrid.DeleteAllItems();
	this->LoadAllData(m_pfolderlistview->m_pselectFiles);
	//���
	vector<HapyProjectFile*>().swap(m_pfolderlistview->m_pselectFiles);
	m_treeGrid.ExpandAll(FALSE);
	m_treeGrid.AdjustLayout();
	m_treeGrid.RedrawWindow ();
	return TRUE;  
}
void CFileSelectDlg::ResetTree(){
	this->m_treeGrid.DeleteAllItems();
}
void CFileSelectDlg::LoadAllData(vector<HapyProjectFile*> thisfiles){
	for (vector<HapyProjectFile*>::iterator it =thisfiles.begin();it!=thisfiles.end();it++ ){
		HapyProjectFile* thisfile=*it;
		LoadData(thisfile,NULL);
	}

}
void CFileSelectDlg::LoadData(HapyProjectFile* thisfile,CBCGPGridRow* parentR){

	if(thisfile!=NULL)
	{
		if(thisfile->NodeStatus()==LocalFile) return;
		CBCGPGridRow* pchild=  this->m_treeGrid.CreateRow ( this->m_treeGrid.GetColumnCount());
		ASSERT_VALID (pchild);
		pchild->AllowSubItems ();
		pchild->SetData((DWORD_PTR)thisfile);
		//����
		CString filename (thisfile->Name().c_str());
		pchild->GetItem (listname)->SetValue ((LPCTSTR)filename);
		//����
		CString fileextension (thisfile->Extension().c_str());
		pchild->GetItem (listtype)->SetValue ((LPCTSTR)fileextension);
		//��С
		CString filelen;
		filelen.Format(_T("%dKB"),thisfile->FileSize()/1024+1);
		//CString filelen(StringUtils::Int2String(thisfile->NodeStatus()).c_str());
		pchild->GetItem (listsize)->SetValue ((LPCTSTR)filelen);
		//����
		CString createname (thisfile->CreateName().c_str());
		pchild->GetItem (listauthor)->SetValue ((LPCTSTR)createname);
		//����ʱ��
		CString createtime (thisfile->CreateTime().c_str());
		customMethods::timeFormatter(createtime);
		pchild->GetItem (listcreatetime)->SetValue ((LPCTSTR)createtime);
		//�汾
		CString version;
		version.Format(_T("%d"),thisfile->FileVersion());
		pchild->GetItem (listversion)->SetValue ((LPCTSTR)version);
		//���
		CString fileReversionLabel (thisfile->FileReversionLabel().c_str());
		pchild->GetItem (listrevision)->SetValue ((LPCTSTR)fileReversionLabel);
		//��������״̬
		CString statusname (thisfile->StateName().c_str());
		pchild->GetItem (listlife)->SetValue ((LPCTSTR)statusname);
		//�����
		CString checkoutname (thisfile->CheckoutName().c_str());
		pchild->GetItem (listout)->SetValue ((LPCTSTR)checkoutname);
		//���ʱ��
		CString filecheckouttime (thisfile->FileCheckouttime().c_str());
		customMethods::timeFormatter(filecheckouttime);
		pchild->GetItem (listouttime)->SetValue ((LPCTSTR)filecheckouttime);
		//������
		CString checkinname (thisfile->CheckinName().c_str());
		pchild->GetItem (listin)->SetValue ((LPCTSTR)checkinname);
		//����ʱ��
		CString filecheckintime (thisfile->FileCheckintime().c_str());
		customMethods::timeFormatter(filecheckintime);
		pchild->GetItem (listintime)->SetValue ((LPCTSTR)filecheckintime);
		if(parentR==NULL){
			this->m_treeGrid.AddRow (pchild, FALSE);
		}else{
			parentR->AddSubItem (pchild, FALSE);
		}
		CBCGPGridRow* pplaceholder=  this->m_treeGrid.CreateRow ( this->m_treeGrid.GetColumnCount());
		ASSERT_VALID (pplaceholder);
		pplaceholder->GetItem (listcheck)->SetValue (L"placeholder");
		pchild->AddSubItem (pplaceholder, FALSE);
		pchild->Expand(FALSE);
	}	
}
void CFileSelectDlg::OnItemExpanding(NMHDR* pNMHDR, LRESULT* pResult){

	NMTREEVIEW *pNMTreeView = (NMTREEVIEW*)pNMHDR;
	HTREEITEM hItem=pNMTreeView->itemNew.hItem;
	//����������ͷ���
	if ( (pNMTreeView->action & TVE_EXPAND) != TVE_EXPAND )
	{
		return;
	}

	HapyProjectFile* thisfile=(HapyProjectFile*)this->m_treeGrid.GetItemData(hItem);
	//չ��ʱ
	HTREEITEM hchild=this->m_treeGrid.GetChildItem(hItem);
	if(hchild!=NULL){/*���������*/
		if(this->m_treeGrid.GetItemText(hchild)=="placeholder")/*�����ռλ��*/{
			BeginWaitCursor();
			this->m_treeGrid.SetRedraw(FALSE);
			this->m_treeGrid.DeleteItem(hchild);
			CBCGPGridRow* pselectRow=this->m_treeGrid.GetCurSel();
			ASSERT_VALID (pselectRow);
			pselectRow->AllowSubItems ();
			HapyProjectFile* pselectFile=(HapyProjectFile*)pselectRow->GetData();
			vector<HapyProjectFile*>* referenceFiles=pselectFile->References();
			for (vector<HapyProjectFile*>::iterator	it =referenceFiles->begin();it!=referenceFiles->end();it++)
			{
				HapyProjectFile* refreencefile=*it;
				LoadData(refreencefile,pselectRow);
			}
		}
		this->m_treeGrid.SetRedraw(TRUE);
		EndWaitCursor();
		this->m_treeGrid.AdjustLayout();
	}
}
//OK
void CFileSelectDlg::OnBnClickedOkfileselect()
{
	HTREEITEM hitem=m_treeGrid.GetRootItem();
	while(hitem)
	{
		this->GetCheckNode(hitem);
		hitem=m_treeGrid.GetNextSiblingItem(hitem);
	}
	CBCGPDialog::OnOK(); //�رյ�½����
}
//Cancel
void CFileSelectDlg::OnBnClickedCancelfileselect()
{
	CBCGPDialog::OnCancel(); // �ر�
}
LRESULT CFileSelectDlg::OnHeaderCheckClick(WPARAM, LPARAM)
{
	BOOL bChecked = m_treeGrid.GetColumnsInfo().GetCheckBoxState();
	m_treeGrid.GetColumnsInfo().SetCheckBoxState(!bChecked);
	m_treeGrid.CheckAll(!bChecked);
	return TRUE; // disable the default implementation
}
LRESULT CFileSelectDlg::OnCheckClick(WPARAM, LPARAM lp)
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
void CFileSelectDlg::GetCheckNode( HTREEITEM hRoot )
{
	if(TRUE == m_treeGrid.GetCheck(hRoot))
	{
		HapyProjectFile* selectfile=(HapyProjectFile*)m_treeGrid.GetItemData(hRoot);
		if(selectfile!=NULL)
			m_pfolderlistview->m_pselectFiles.push_back(selectfile);
	}
	HTREEITEM hCurItem = m_treeGrid.GetChildItem(hRoot);
	HTREEITEM hNextItem;
	while(hCurItem)
	{
		hNextItem = hCurItem;
		GetCheckNode(hNextItem);
		hCurItem = m_treeGrid.GetNextSiblingItem(hCurItem);
	}
}