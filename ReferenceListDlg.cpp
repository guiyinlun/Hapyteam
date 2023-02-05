#include "stdafx.h"
#include "hapyteamclient.h"
#include "ReferenceListDlg.h"
#include "afxdialogex.h"
#include"FileDetailsLayoutView.h"
#include <Core/api.h>


int nSel;

// CQuoteDlg �Ի���
IMPLEMENT_DYNAMIC(CReferenceListDlg, CBCGPDialog)

	CReferenceListDlg::CReferenceListDlg(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CReferenceListDlg::IDD, pParent)
{
	EnableVisualManagerStyle(TRUE, TRUE);
	EnableLayout();
	m_pselectfile=NULL;
}

CReferenceListDlg::~CReferenceListDlg()
{
}

void CReferenceListDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ReferenceCombo, m_Combo);
}


BEGIN_MESSAGE_MAP(CReferenceListDlg, CBCGPDialog)
	ON_CBN_SELCHANGE(IDC_ReferenceCombo, &CReferenceListDlg::OnCbnSelchangequotecombo)
	//ON_NOTIFY(TVN_GETDISPINFO, IDC_ReferenceTree, &CReferenceListDlg::GetDispInfo)
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_ReferenceTree, &CReferenceListDlg::OnItemExpanding)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CQuoteDlg ��Ϣ�������
BOOL CReferenceListDlg::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();
	m_Combo.SetWindowPos(this,5,5,150,30,SWP_NOZORDER);
	UINT nTreeStyles = WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | TVS_LINESATROOT |
		TVS_HASLINES | //����
		TVS_HASBUTTONS | //�а�ť
		TVS_SHOWSELALWAYS|
		TVS_FULLROWSELECT; //����ѡ��
	m_treeGrid.CreateOnPlaceHolder(this, IDC_ReferenceList, 0, nTreeStyles);
	CRect thisrect,viewrect;
	this->GetClientRect(&viewrect);
	thisrect.SetRect(-2,44,viewrect.right+2,viewrect.bottom+2);
	m_treeGrid.MoveWindow(&thisrect);
	CBCGPStaticLayout* pLayout = (CBCGPStaticLayout*)GetLayout();
	if (pLayout != NULL)
	{
		pLayout->AddAnchor(m_treeGrid.GetSafeHwnd(), CBCGPStaticLayout::e_MoveTypeNone, CBCGPStaticLayout::e_SizeTypeBoth);
		pLayout->AddAnchor(IDC_ReferenceCombo, CBCGPStaticLayout::e_MoveTypeNone, CBCGPStaticLayout::e_SizeTypeNone);
	}

	//����
	m_treeGrid.EnableHeader (TRUE, BCGP_GRID_HEADER_SELECT);
	m_treeGrid.EnableColumnAutoSize(TRUE); //����������Ӧ���
	m_treeGrid.EnableGridLines(TRUE);//��ʾgridline
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
	/*m_treeGrid.InsertColumn(listcheck, _T(""), m_width);
	m_treeGrid.InsertColumn(listicon, _T(""), m_width);
	m_treeGrid.InsertColumn(listname, _T("�ļ�����"), m_width);
	m_treeGrid.InsertColumn(listtype, _T("�ļ�����"), m_width);
	m_treeGrid.InsertColumn(listsize, _T("�ļ���С"), m_width);
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
	m_treeGrid.SetColumnVisible(listout,FALSE);
	m_treeGrid.SetColumnVisible(listouttime,FALSE);

	//m_treeGrid.LoadState (_T("Referencelist"));
	m_treeGrid.AdjustLayout();
	m_treeGrid.RedrawWindow ();
	return TRUE;  
}

//comboѡ��
void CReferenceListDlg::OnCbnSelchangequotecombo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	nSel= m_Combo.GetCurSel();  
	m_treeGrid.DeleteAllItems();
	BeginWaitCursor();
	LoadData(m_pselectfile,nSel+1);
	EndWaitCursor();
}

void CReferenceListDlg::ResetComboAndTree(){
	//���
	this->m_treeGrid.DeleteAllItems();
	this->m_Combo.ResetContent();	
}
void CReferenceListDlg::LoadCombo(HapyProjectFile* file){
	this->m_pselectfile=file;
	//���combo
	for(int i=1;i<=file->FileVersion();i++){
		CString str(StringUtils::Int2String(i).c_str());
		this->m_Combo.AddString(str); 
	}	
	this->m_Combo.SetCurSel(file->FileVersion()-1);  //���ᴥ��OnCbnSelchangequotecombo();
}
void CReferenceListDlg::AddOneData(HapyProjectFile* file,CBCGPGridRow* parentR){
	if(file!=NULL)
	{
			CBCGPGridRow* pchild=  this->m_treeGrid.CreateRow ( this->m_treeGrid.GetColumnCount());
			ASSERT_VALID (pchild);
			pchild->AllowSubItems ();
			pchild->SetData((DWORD_PTR)file);
			//����
			CString filename (file->Name().c_str());
			pchild->GetItem (listname)->SetValue ((LPCTSTR)filename);
			//����
			CString fileextension (file->Extension().c_str());
			pchild->GetItem (listtype)->SetValue ((LPCTSTR)fileextension);
			//��С
			CString filelen;
			filelen.Format(_T("%dKB"),file->FileSize()/1024+1);
			pchild->GetItem (listsize)->SetValue ((LPCTSTR)filelen);
			//����
			CString createname (file->CreateName().c_str());
			pchild->GetItem (listauthor)->SetValue ((LPCTSTR)createname);
			//����ʱ��
			CString createtime (file->CreateTime().c_str());
			customMethods::timeFormatter(createtime);
			pchild->GetItem (listcreatetime)->SetValue ((LPCTSTR)createtime);
			//�汾
			CString version;
			version.Format(_T("%d"),file->FileVersion());
			pchild->GetItem (listversion)->SetValue ((LPCTSTR)version);
			//���
			CString fileReversionLabel (file->FileReversionLabel().c_str());
			pchild->GetItem (listrevision)->SetValue ((LPCTSTR)fileReversionLabel);
			//��������״̬
			CString statusname (file->StateName().c_str());
			pchild->GetItem (listlife)->SetValue ((LPCTSTR)statusname);
			//�����
			CString checkoutname (file->CheckoutName().c_str());
			pchild->GetItem (listout)->SetValue ((LPCTSTR)checkoutname);
			//���ʱ��
			CString filecheckouttime (file->FileCheckouttime().c_str());
			customMethods::timeFormatter(filecheckouttime);
			pchild->GetItem (listouttime)->SetValue ((LPCTSTR)filecheckouttime);
			//������
			CString checkinname (file->CheckinName().c_str());
			pchild->GetItem (listin)->SetValue ((LPCTSTR)checkinname);
			//����ʱ��
			CString filecheckintime (file->FileCheckintime().c_str());
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
void CReferenceListDlg::LoadData(HapyProjectFile* file,int selectversion){ 
	CString timecstr;
	//�õ���ǰ�汾�ļ�
	//t1=GetTickCount();
	vector<HapyProjectFile*>* historyFiles=file->FileHistory();
	if(historyFiles->size()>0){
		this->m_treeGrid.SetRedraw(FALSE);
		vector<HapyProjectFile*>* referenceFiles=historyFiles->at(file->FileVersion()-selectversion)->References();
		//t2=GetTickCount();
		CBCGPGridRow* pRoot = this->m_treeGrid.CreateRow (this->m_treeGrid.GetColumnCount());
		ASSERT_VALID (pRoot);
		pRoot->AllowSubItems();
		
		//timecstr.Format(L"time:%d ms",t2-t1);
		//customMethods::customMessagebox(L"",timecstr,MB_OK|MB_ICONEXCLAMATION);

		if(!referenceFiles->size()>0){
			pRoot->GetItem (listname)->SetValue (_T("������"));
			this->m_treeGrid.AddRow (pRoot, FALSE);
		}else{
			for (vector<HapyProjectFile*>::iterator	it =referenceFiles->begin();it!=referenceFiles->end();it++)
			{
				HapyProjectFile* refreencefile=*it;				
				AddOneData(refreencefile,NULL);	
			}	

		}

		this->m_treeGrid.ExpandAll(FALSE);
		this->m_treeGrid.SetRedraw(TRUE);
		this->m_treeGrid.AdjustLayout();
	}	
}
void CReferenceListDlg::OnItemExpanding(NMHDR* pNMHDR, LRESULT* pResult){

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
					AddOneData(refreencefile,pselectRow);
					
				}
		}
		this->m_treeGrid.SetRedraw(TRUE);
		EndWaitCursor();
		this->m_treeGrid.AdjustLayout();
	}
}
void CReferenceListDlg::OnDestroy()
{
	m_treeGrid.SaveState (_T("Referencelist"));
	CBCGPDialog::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
}
