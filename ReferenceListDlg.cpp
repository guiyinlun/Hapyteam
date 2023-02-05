#include "stdafx.h"
#include "hapyteamclient.h"
#include "ReferenceListDlg.h"
#include "afxdialogex.h"
#include"FileDetailsLayoutView.h"
#include <Core/api.h>


int nSel;

// CQuoteDlg 对话框
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


// CQuoteDlg 消息处理程序
BOOL CReferenceListDlg::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();
	m_Combo.SetWindowPos(this,5,5,150,30,SWP_NOZORDER);
	UINT nTreeStyles = WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | TVS_LINESATROOT |
		TVS_HASLINES | //有线
		TVS_HASBUTTONS | //有按钮
		TVS_SHOWSELALWAYS|
		TVS_FULLROWSELECT; //整行选中
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

	//设置
	m_treeGrid.EnableHeader (TRUE, BCGP_GRID_HEADER_SELECT);
	m_treeGrid.EnableColumnAutoSize(TRUE); //设置列自适应宽度
	m_treeGrid.EnableGridLines(TRUE);//显示gridline
	m_treeGrid.EnableAlternateRows(FALSE);//启用交替行
	m_treeGrid.SetReadOnly ();
	m_treeGrid.SetWholeRowSel ();//整行
	m_treeGrid.SetCustomRowHeight(35,35);//设置行宽，第二个参数决定

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
	m_treeGrid.InsertColumn(listname, _T("文件名称"), m_width);
	m_treeGrid.InsertColumn(listtype, _T("文件类型"), m_width);
	m_treeGrid.InsertColumn(listsize, _T("文件大小"), m_width);
	m_treeGrid.InsertColumn(listauthor , _T("作者"), m_width);
	m_treeGrid.InsertColumn(listcreatetime, _T("创建时间"), m_width);
	m_treeGrid.InsertColumn(listversion , _T("版本"), m_width);	
	m_treeGrid.InsertColumn(listrevision, _T("版次"), m_width);
	m_treeGrid.InsertColumn(listlife , _T("生命周期状态"), m_width);
	m_treeGrid.InsertColumn(listout , _T("检出人"), m_width);
	m_treeGrid.InsertColumn(listouttime , _T("检出时间"), m_width);
	m_treeGrid.InsertColumn(listin, _T("检入人"), m_width);
	m_treeGrid.InsertColumn(listintime , _T("检入时间"), m_width);
	m_treeGrid.InsertColumn(listnote , _T("备注"), m_width);*/
	m_treeGrid.SetColumnVisible(listcheck,FALSE);
	m_treeGrid.SetColumnVisible(listicon,FALSE);
	m_treeGrid.SetColumnVisible(listout,FALSE);
	m_treeGrid.SetColumnVisible(listouttime,FALSE);

	//m_treeGrid.LoadState (_T("Referencelist"));
	m_treeGrid.AdjustLayout();
	m_treeGrid.RedrawWindow ();
	return TRUE;  
}

//combo选择
void CReferenceListDlg::OnCbnSelchangequotecombo()
{
	// TODO: 在此添加控件通知处理程序代码
	nSel= m_Combo.GetCurSel();  
	m_treeGrid.DeleteAllItems();
	BeginWaitCursor();
	LoadData(m_pselectfile,nSel+1);
	EndWaitCursor();
}

void CReferenceListDlg::ResetComboAndTree(){
	//清空
	this->m_treeGrid.DeleteAllItems();
	this->m_Combo.ResetContent();	
}
void CReferenceListDlg::LoadCombo(HapyProjectFile* file){
	this->m_pselectfile=file;
	//添加combo
	for(int i=1;i<=file->FileVersion();i++){
		CString str(StringUtils::Int2String(i).c_str());
		this->m_Combo.AddString(str); 
	}	
	this->m_Combo.SetCurSel(file->FileVersion()-1);  //不会触发OnCbnSelchangequotecombo();
}
void CReferenceListDlg::AddOneData(HapyProjectFile* file,CBCGPGridRow* parentR){
	if(file!=NULL)
	{
			CBCGPGridRow* pchild=  this->m_treeGrid.CreateRow ( this->m_treeGrid.GetColumnCount());
			ASSERT_VALID (pchild);
			pchild->AllowSubItems ();
			pchild->SetData((DWORD_PTR)file);
			//名称
			CString filename (file->Name().c_str());
			pchild->GetItem (listname)->SetValue ((LPCTSTR)filename);
			//类型
			CString fileextension (file->Extension().c_str());
			pchild->GetItem (listtype)->SetValue ((LPCTSTR)fileextension);
			//大小
			CString filelen;
			filelen.Format(_T("%dKB"),file->FileSize()/1024+1);
			pchild->GetItem (listsize)->SetValue ((LPCTSTR)filelen);
			//作者
			CString createname (file->CreateName().c_str());
			pchild->GetItem (listauthor)->SetValue ((LPCTSTR)createname);
			//创建时间
			CString createtime (file->CreateTime().c_str());
			customMethods::timeFormatter(createtime);
			pchild->GetItem (listcreatetime)->SetValue ((LPCTSTR)createtime);
			//版本
			CString version;
			version.Format(_T("%d"),file->FileVersion());
			pchild->GetItem (listversion)->SetValue ((LPCTSTR)version);
			//版次
			CString fileReversionLabel (file->FileReversionLabel().c_str());
			pchild->GetItem (listrevision)->SetValue ((LPCTSTR)fileReversionLabel);
			//生命周期状态
			CString statusname (file->StateName().c_str());
			pchild->GetItem (listlife)->SetValue ((LPCTSTR)statusname);
			//检出人
			CString checkoutname (file->CheckoutName().c_str());
			pchild->GetItem (listout)->SetValue ((LPCTSTR)checkoutname);
			//检出时间
			CString filecheckouttime (file->FileCheckouttime().c_str());
			customMethods::timeFormatter(filecheckouttime);
			pchild->GetItem (listouttime)->SetValue ((LPCTSTR)filecheckouttime);
			//检入人
			CString checkinname (file->CheckinName().c_str());
			pchild->GetItem (listin)->SetValue ((LPCTSTR)checkinname);
			//检入时间
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
	//得到当前版本文件
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
			pRoot->GetItem (listname)->SetValue (_T("无数据"));
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
	//如果是收缩就返回
	if ( (pNMTreeView->action & TVE_EXPAND) != TVE_EXPAND )
	{
		return;
	}
	HapyProjectFile* thisfile=(HapyProjectFile*)this->m_treeGrid.GetItemData(hItem);
	//展开时
	HTREEITEM hchild=this->m_treeGrid.GetChildItem(hItem);
	if(hchild!=NULL){/*如果有子项*/
		if(this->m_treeGrid.GetItemText(hchild)=="placeholder")/*如果是占位行*/{
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

	// TODO: 在此处添加消息处理程序代码
}
