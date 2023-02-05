#include "stdafx.h"
#include "hapyteamclient.h"
#include "hapyteamclientDoc.h"
#include "FolderListView.h"
#include "MainFrm.h"
#include"ProjectTreeView.h"
#include "AttachedFileDlg.h"
#include"ProjectListDlg.h"
#include"FileSelectDlg.h"
#include"CustomMethods.h"
#include <Core/FileUploader.h>
#include <Core/Utility.h>
#include<Interface/HapyProjectManager.h>
#include <boost/filesystem.hpp>
#include"CCppOfficeConvertPdf.h"


BOOL CHAPYBreadcrumb::DeleteAllItem ()
{
	return (BOOL)SendMessage (BCCM_RESETCONTENT, 0, 0);
}

IMPLEMENT_DYNCREATE(CFolderListView, CBCGPFormView)

	CFolderListView::CFolderListView()
	: CBCGPFormView(CFolderListView::IDD)
{
	EnableVisualManagerStyle();
	EnableLayout();
	m_pselectFile=NULL;
	isRefresh=FALSE;
	m_pprojecttreeview=NULL ;
	m_pdetailsView=NULL;
	m_pversionDlg=NULL ;
	m_preferenceDlg=NULL;
	m_pbyrelerenceDlg=NULL;
	m_previsionDlg=NULL;
	m_pannotateDlg=NULL;
	//string str[]={"123"};
	string str[]={".dwg",".dxf",".ipt",".iam",".idw",".ide",
		".ipn",".sldprt",".sldasm",".slddrw",".sldxml",
		".drw",".asm",".prt",".rvt",".rfa",".adsk",".rte"};
	for(int i=0;i<sizeof(str)/sizeof(str[0]);i++){
		m_filefilter.push_back(str[i]);
	}
}

CFolderListView::~CFolderListView()
{

}

void CFolderListView::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BREADCRUMB, m_Breadcrumb);
	DDX_Control(pDX, IDC_ListUpdate, m_listUpdate);
}

BEGIN_MESSAGE_MAP(CFolderListView, CBCGPFormView)
	ON_MESSAGE(WM_INITDIALOG, HandleInitDialog)
	ON_WM_CONTEXTMENU()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_REGISTERED_MESSAGE(BCGM_GRID_ITEM_DBLCLICK, &CFolderListView::OnGridDoubleClicked)
	ON_REGISTERED_MESSAGE(BCGM_GRID_SEL_CHANGED, &CFolderListView::OnSelchangedList)
	ON_NOTIFY(BCCN_SELECTIONCHANGED, IDC_BREADCRUMB, &CFolderListView::OnBredcrumbSelectionChanged)
	ON_COMMAND(ID_Menu_CheckIn, &CFolderListView::OnCheckIn)
	ON_COMMAND(ID_Menu_CheckOut, &CFolderListView::OnCheckOut)
	ON_COMMAND(ID_Menu_UndoCheckOut, &CFolderListView::OnUndoCheckOut)
	ON_COMMAND(ID_MENU_Download, &CFolderListView::OnDownload)
	ON_COMMAND(ID_CTRLF, &CFolderListView::OnCtrlf)
	ON_BN_CLICKED(IDC_ListUpdate, &CFolderListView::OnBnClickedListupdate)
	//ON_COMMAND(ID_Menu_Look, &CFolderListView::OnMenuLook)
END_MESSAGE_MAP()
// CMyListView ���

#ifdef _DEBUG
void CFolderListView::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFolderListView::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}
#endif
#endif //_DEBUG


//CMyListView ��Ϣ�������
LRESULT CFolderListView::HandleInitDialog(WPARAM wParam, LPARAM lParam)
{
	BOOL bRes = (BOOL)CBCGPFormView::HandleInitDialog(wParam, lParam);	

	UINT nListStyles = WS_CHILD | WS_VISIBLE  | WS_BORDER | 
		TVS_SHOWSELALWAYS|
		TVS_FULLROWSELECT; //����ѡ��
	m_listGrid.CreateOnPlaceHolder(this, IDC_FolderList, 0, nListStyles);

	CRect thisrect,viewrect;
	this->GetClientRect(&viewrect);
	thisrect.SetRect(-1,33,viewrect.right,viewrect.bottom);
	m_listGrid.MoveWindow(&thisrect);

	CBCGPStaticLayout* pLayout = (CBCGPStaticLayout*)GetLayout();
	if (pLayout != NULL)
	{
		pLayout->AddAnchor(m_listGrid.GetSafeHwnd(), 
			CBCGPStaticLayout::e_MoveTypeNone, CBCGPStaticLayout::e_SizeTypeBoth);
	}
	return bRes;

}
//��ʼ��
void CFolderListView::OnInitialUpdate()
{
	CBCGPFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	//��ݼ����
	m_hAccel = ::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR1)); 

	CRect thisrect,viewrect;
	this->GetClientRect(&viewrect);
	thisrect.SetRect(0,0,viewrect.right-35,35);
	m_Breadcrumb.MoveWindow(&thisrect);
	m_Breadcrumb.SetFont(&theApp.m_font);
	m_Breadcrumb.ModifyStyle (0, BCCS_SHOWROOTALWAYS);

	thisrect.SetRect(viewrect.right-36,0,viewrect.right,33);
	m_listUpdate.MoveWindow(&thisrect);
	m_listUpdate.SetMouseCursorHand();	
	m_listUpdate.SetTooltip(L"ˢ��");

	//HICON m_hicn1=AfxGetApp()->LoadIcon(IDI_outlook); 
	//m_listUpdate.SetIcon(m_hicn1);
	//ListCtrl����////////////////////////////////////////////////////////////////////////////
	m_listGrid.EnableHeader (TRUE, BCGP_GRID_HEADER_SORT);
	m_listGrid.EnableColumnAutoSize(TRUE); //����������Ӧ���
	m_listGrid.EnableGridLines(TRUE);//��ʾgridline
	m_listGrid.EnableRowHeader(FALSE);
	m_listGrid.EnableAlternateRows(FALSE);//���ý�����
	m_listGrid.SetReadOnly ();
	m_listGrid.SetWholeRowSel ();//����
	m_listGrid.SetCustomRowHeight(38,38);//�����п��ڶ�����������


	//���⣬���壬ͼ��
	m_listGrid.SetVisualManagerColorTheme(TRUE);
	m_listGrid.SetFont(&theApp.m_font);
	CMainFrame*pMainFrame = DYNAMIC_DOWNCAST(CMainFrame,AfxGetMainWnd());
	m_listGrid.SetImageList(&pMainFrame->m_ImageList);

	//��ӱ�ͷ
	m_listGrid.RemoveAll();
	m_listGrid.DeleteAllColumns();
	int onewidth=100;
	CString cstr;
	m_listGrid.InsertColumn(listcheck, _T(""), 18);
	m_listGrid.InsertColumn(listicon, _T(""), 18);
	for(int i=103;i<116;i++){
		cstr.LoadString(i);
		m_listGrid.InsertColumn(i-101,cstr,onewidth);
	}
	/*m_listGrid.InsertColumn(listname, _T("�ļ�����"), onewidth);
	m_listGrid.InsertColumn(listtype, _T("�ļ�����"), onewidth);
	m_listGrid.InsertColumn(listsize, _T("�ļ���С"), onewidth);
	m_listGrid.InsertColumn(listauthor , _T("����"), onewidth);
	m_listGrid.InsertColumn(listcreatetime, _T("����ʱ��"), onewidth);
	m_listGrid.InsertColumn(listversion , _T("�汾"), onewidth);	
	m_listGrid.InsertColumn(listedition, _T("���"), onewidth);
	m_listGrid.InsertColumn(listlife , _T("��������״̬"), onewidth);
	m_listGrid.InsertColumn(listout , _T("�����"), onewidth);
	m_listGrid.InsertColumn(listouttime , _T("���ʱ��"), onewidth);
	m_listGrid.InsertColumn(listin, _T("������"), onewidth);
	m_listGrid.InsertColumn(listintime , _T("����ʱ��"), onewidth);
	m_listGrid.InsertColumn(listnote , _T("��ע"), onewidth);*/
	//��������
	m_listGrid.SetColumnLocked(listcheck,TRUE);
	m_listGrid.SetColumnLocked(listicon,TRUE);
	m_listGrid.SetHeaderAlign(listcheck, HDF_CENTER);
	m_listGrid.SetColumnAlign(listcheck, HDF_CENTER);
	m_listGrid.SetHeaderAlign(listicon, HDF_CENTER);
	m_listGrid.SetColumnAlign(listicon, HDF_CENTER);
	m_listGrid.LoadState (_T("list"));

	m_listGrid.AdjustLayout();
	m_listGrid.RedrawWindow ();

	//huangc
	m_ExtensionAndIconMapping[".zip"]=zip;
	m_ExtensionAndIconMapping[".ipt"]=ipt;
	m_ExtensionAndIconMapping[".doc"]=doc;
	m_ExtensionAndIconMapping[".docx"]=docx;
	m_ExtensionAndIconMapping[".vsd"]=vsd;
	m_ExtensionAndIconMapping[".txt"]=txt;
	m_ExtensionAndIconMapping[".ses"]=ses;
	m_ExtensionAndIconMapping[".sldprt"]=sldprt;
	m_ExtensionAndIconMapping[".sldasm"]=sldasm;
	m_ExtensionAndIconMapping[".rvt"]=rvt;
	m_ExtensionAndIconMapping[".rvm"]=rvm;
	m_ExtensionAndIconMapping[".rtf"]=rtf;
	m_ExtensionAndIconMapping[".rfa"]=rfa;
	m_ExtensionAndIconMapping[".prt"]=prt;
	m_ExtensionAndIconMapping[".ppt"]=ppt;
	m_ExtensionAndIconMapping[".pptx"]=ppt;
	m_ExtensionAndIconMapping[".png"]=png;
	m_ExtensionAndIconMapping[".one"]=one;
	m_ExtensionAndIconMapping[".pdf"]=pdf;
	m_ExtensionAndIconMapping[".nwf"]=nwf;
	m_ExtensionAndIconMapping[".nwc"]=nwc;
	m_ExtensionAndIconMapping[".mpp"]=mpp;
	m_ExtensionAndIconMapping[".jpg"]=jpg;
	m_ExtensionAndIconMapping[".ipj"]=ipj;
	m_ExtensionAndIconMapping[".gif"]=gif;
	m_ExtensionAndIconMapping[".xlsx"]=xlsx;
	m_ExtensionAndIconMapping[".xls"]=xls;
	m_ExtensionAndIconMapping[".dxf"]=dxf;
	m_ExtensionAndIconMapping[".dwg"]=dwg;
	m_ExtensionAndIconMapping[".dgn"]=dgn;
	m_ExtensionAndIconMapping[".csv"]=csv;
	m_ExtensionAndIconMapping[".catproduct"]=catproduct;
	m_ExtensionAndIconMapping[".catpart"]=catpart;
	m_ExtensionAndIconMapping[".assembly"]=assembly;
	m_ExtensionAndIconMapping[".asm"]=myasm;
	m_ExtensionAndIconMapping[".iam"]=iam;
	m_ExtensionAndIconMapping[".idw"]=idw;
	m_ExtensionAndIconMapping[".slddrw"]=slddrw;
	m_ExtensionAndIconMapping[".ipn"]=ipn;
}
//�Ҽ��˵�
void CFolderListView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CMenu menu;
	menu.LoadMenuW(IDR_FolderMenu);
	CMenu *contextMenu = menu.GetSubMenu(0);
	ASSERT(contextMenu != NULL);
	/*menu.GetSubMenu(0)->TrackPopupMenu
	(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, GetParent());//����һ�������Ĳ˵�	*/
	//���ȵõ������λ��
	CBCGPGridRow* pchild= this->m_listGrid.GetCurSel();
	if(pchild==NULL) return;	
	UINT i=m_listGrid.GetSelectedItemCount()/this->m_listGrid.GetColumnCount();
	//��ѡ
	if(i==1){
		HapyProjectNode* selectNode=(HapyProjectNode*)pchild->GetData();
		if(selectNode==NULL) return;
		//�ļ�
		if(selectNode->IsFile()){
			HapyProjectFile* selectFile= dynamic_cast<HapyProjectFile*>(selectNode);
			//����Ǽ��״̬
			if(selectFile->NodeStatus()==CheckoutBySelf||selectFile->NodeStatus()==CheckoutBySomeone){
				contextMenu->RemoveMenu(ID_Menu_CheckOut, MF_BYCOMMAND);//���
				contextMenu->RemoveMenu(ID_Menu_AttachedFile, MF_BYCOMMAND );//�����ļ�
			}
			//�����ļ�
			else if(selectFile->NodeStatus()==LocalFile){	
				//���˲��ɼ����ļ�
				for (vector<string>::iterator it=m_filefilter.begin();it!=m_filefilter.end();it++)
				{
					if(BoostHelper::getExtension(selectFile->Name())==*it){
						return;
					}
				}	
				contextMenu->RemoveMenu(ID_Menu_CheckOut, MF_BYCOMMAND);//���
				contextMenu->RemoveMenu(ID_Menu_UndoCheckOut, MF_BYCOMMAND);//�������
				contextMenu->RemoveMenu(ID_MENU_Download, MF_BYCOMMAND );//��ȡ
				contextMenu->RemoveMenu(ID_Menu_AttachedFile, MF_BYCOMMAND );//�����ļ�


			}else if(selectFile->NodeStatus()==Archived){	
				contextMenu->RemoveMenu(ID_Menu_CheckIn, MF_BYCOMMAND);//����
				contextMenu->RemoveMenu(ID_Menu_CheckOut, MF_BYCOMMAND);//���
				contextMenu->RemoveMenu(ID_Menu_UndoCheckOut, MF_BYCOMMAND);//�������
				contextMenu->RemoveMenu(ID_Menu_AttachedFile, MF_BYCOMMAND );//�����ļ�	
			}
			//�����״̬
			else {
				contextMenu->RemoveMenu(ID_Menu_CheckIn, MF_BYCOMMAND);//����
				contextMenu->RemoveMenu(ID_Menu_UndoCheckOut, MF_BYCOMMAND);//�������
				contextMenu->RemoveMenu(ID_Menu_AttachedFile, MF_BYCOMMAND );//�����ļ�
			}
		}//Ŀ¼
		else {	
			contextMenu->RemoveMenu(ID_Menu_CheckIn, MF_BYCOMMAND);//����
			//contextMenu->RemoveMenu(ID_Menu_AttachedFile, MF_BYCOMMAND );//�����ļ�
		}
	}
	//��ѡ
	else if(i>1)
	{
		contextMenu->RemoveMenu(ID_Menu_AttachedFile, MF_BYCOMMAND );//�����ļ�
	}
	CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;
	pPopupMenu->Create (this, point.x, point.y, contextMenu->Detach (),FALSE,TRUE);
	pPopupMenu->SetForceMenuFocus(TRUE);
	pPopupMenu->SetAutoDestroy(TRUE);
	pPopupMenu->ActivatePopupMenu((CBCGPFrameWnd *)AfxGetMainWnd(),pPopupMenu);
	::SetForegroundWindow(pPopupMenu->m_hWnd);


}
//ѡ��
LRESULT CFolderListView::OnSelchangedList(WPARAM wParam, LPARAM lParam)
{
	//��ȡѡ�е��ļ���Ŀ¼
	CBCGPGridRow* pchild= m_listGrid.GetCurSel();
	if(pchild==NULL) return FALSE;
	//����
	CMainFrame*pMainFrame = DYNAMIC_DOWNCAST(CMainFrame,AfxGetMainWnd());
	pMainFrame->m_wndPropBar.ResetList();
	m_pversionDlg->ResetList();
	m_preferenceDlg->ResetComboAndTree();
	//��ȡ����ָ��
	HapyProjectNode* selectNode=(HapyProjectNode*)pchild->GetData();
	if(selectNode==NULL) return FALSE;
	//�ļ�
	if(selectNode->IsFile()&&selectNode->NodeStatus()!=LocalFile){
		HapyProjectFile* selectFile= dynamic_cast<HapyProjectFile*>(selectNode);
		m_pselectFile=selectFile;
		m_pdetailsView->ResetIsLoad();
		BeginWaitCursor();
		//���Ա�
		pMainFrame->m_wndPropBar.LoadData();	
		//
		m_pannotateDlg->LoadData(selectFile);
		//tab

		switch (m_pdetailsView->m_curSelTab)
		{
		case 0:

			m_pversionDlg->LoadData(selectFile);
			m_pdetailsView->isLoad[0]=TRUE;
			break;
		case 1:		
			m_preferenceDlg->LoadCombo(selectFile);		
			m_preferenceDlg->LoadData(selectFile,selectFile->FileVersion());
			m_pdetailsView->isLoad[1]=TRUE;
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		}	
		EndWaitCursor();
	}//Ŀ¼
	else{ 
		//customMethods::customMessagebox(L"��ʾ",L"Ŀ¼��",MB_OK|MB_ICONERROR);
	}
	this->m_listGrid.SetFocus();
	return TRUE;
}
//·�����ı�
void CFolderListView::OnBredcrumbSelectionChanged (NMHDR* pNotifyStruct, LRESULT* result)
{
	HBREADCRUMBITEM hItem=m_Breadcrumb.GetSelectedItem();
	HapyProjectFolder* FolderByBreadcrumb =(HapyProjectFolder*)m_Breadcrumb.GetItemData(hItem);
	HapyProjectFolder* FolderByTree=(HapyProjectFolder*)m_pprojecttreeview->m_treeGrid.GetItemData(m_pprojecttreeview->m_treeGrid.GetSelectedItem());
	if(FolderByBreadcrumb==FolderByTree||isRefresh==TRUE) {
		isRefresh=FALSE;
		return;
	}
	HBREADCRUMBITEM pathroot=m_Breadcrumb.GetRootItem();
	HTREEITEM htreenode=m_pprojecttreeview->m_treeGrid.GetRootItem();
	if(htreenode==NULL) return;
	if(hItem==pathroot){
		m_pprojecttreeview->m_treeGrid.SelectItem(htreenode);
	}
	else{	
		vector<HapyProjectFolder*> folders;
		while(hItem!=pathroot){
			HapyProjectFolder* thisid=(HapyProjectFolder*)m_Breadcrumb.GetItemData(hItem);
			folders.push_back(thisid);
			hItem=m_Breadcrumb.GetItemParent(hItem);
		}
		HapyProjectFolder* thisfolder;
		for(int i=folders.size()-1;i>=0;i--){
			htreenode=m_pprojecttreeview->m_treeGrid.GetChildItem(htreenode);
			thisfolder=(HapyProjectFolder*)m_pprojecttreeview->m_treeGrid.GetItemData(htreenode);
			while(thisfolder!=folders[i]&&thisfolder!=NULL){
				htreenode=m_pprojecttreeview->m_treeGrid.GetNextSiblingItem(htreenode);
				thisfolder=(HapyProjectFolder*)m_pprojecttreeview->m_treeGrid.GetItemData(htreenode);
			}
		}
		CString name=m_pprojecttreeview->m_treeGrid.GetItemText(htreenode);
		m_pprojecttreeview->m_treeGrid.SelectItem(htreenode);
	}
}
//˫����
LRESULT CFolderListView::OnGridDoubleClicked(WPARAM wParam, LPARAM lParam)
{
	//��ȡѡ�е��ļ���Ŀ¼
	CBCGPGridRow* pchild= this->m_listGrid.GetCurSel();
	if(pchild==NULL) return FALSE;
	HapyProjectNode* selectNode=(HapyProjectNode*)pchild->GetData();
	if(selectNode==NULL) return FALSE;
	if(selectNode->IsFolder()){
		HTREEITEM hTreeItem = m_pprojecttreeview->m_treeGrid.GetSelectedItem ();
		HapyProjectFolder* selectFolder= dynamic_cast<HapyProjectFolder*>(selectNode);
		HTREEITEM selectChildItem=m_pprojecttreeview->FindItemByData(hTreeItem,selectFolder);
		m_pprojecttreeview->m_treeGrid.SelectItem(selectChildItem);
	}
	return TRUE;
}
//����
void CFolderListView::OnCheckIn()
{
	BeginWaitCursor();
	this->GetAllSelectFiles();
	if(m_pselectFiles.size()>0){
		//string errorstr="";
		vector<string> filenames;
		vector<CString> filemessages;
		CString cstr;
		for (vector<HapyProjectFile*>::iterator it =m_pselectFiles.begin();it!=m_pselectFiles.end();it++ ){
			HapyProjectFile* selectFile=*it;
			ProjectNodeStatus status=selectFile->NodeStatus();
			if(status==Unknow){
				//errorstr=errorstr+selectFile->Name()+" δ֪�ļ����ɼ���!\n";
				//errorstr=errorstr+selectFile->Name()+"\n";
				filenames.push_back(selectFile->Name());
				cstr=L"checkinUnknow";
				filemessages.push_back(cstr);

			}else if(status==ServerFile){
				filenames.push_back(selectFile->Name());
				cstr=L"checkinServerFile";
				filemessages.push_back(cstr);

			}
			else  if(status==Archived){
				//errorstr=errorstr+selectFile->Name()+" �鵵�ļ����ɼ���!\n";
				//errorstr=errorstr+selectFile->Name()+"\n";
				filenames.push_back(selectFile->Name());
				cstr=L"checkinArchived";
				filemessages.push_back(cstr);
			}
			else  if(status==CheckoutBySomeone){
				//errorstr=errorstr+selectFile->Name()+" �ļ����˼�������ɲ������ļ�!\n";
				//errorstr=errorstr+selectFile->Name()+"\n";
				filenames.push_back(selectFile->Name());
				cstr=L"checkinCheckoutBySomeone";
				filemessages.push_back(cstr);

			}
			else if(status==Checkined) {
				//errorstr=errorstr+selectFile->Name()+" �ļ�δ������ɼ���!\n";
				//errorstr=errorstr+selectFile->Name()+"\n";
				filenames.push_back(selectFile->Name());
				cstr=L"checkinCheckined";
				filemessages.push_back(cstr);

			}
			else {
				//���˲��ɼ����ļ�
				bool ischeckin=TRUE;
				string extension=BoostHelper::getExtension(selectFile->Name());
				for (vector<string>::iterator it=m_filefilter.begin();it!=m_filefilter.end();it++)
				{
					if(extension==*it){
						filenames.push_back(selectFile->Name());
						cstr=L"checkinErrorformat";
						filemessages.push_back(cstr);
						ischeckin=FALSE;
						break;
					}
				}	
				if(ischeckin){
					//��word��excel��ppt�ļ����pdf���������
					if(extension==".doc"||extension==".docx"||extension==".xls"||extension==".xlsx"||extension==".pptx"||extension==".ppt"){
						wstring pdfPath = Utility::getTempPath();
						std::string sPdfPath = StringUtils::WString2String(pdfPath);
						sPdfPath.append("Pdfs\\");
						string onlyname = selectFile->Name().substr(0,selectFile->Name().rfind("."));
						sPdfPath.append(onlyname);
						sPdfPath.append(".pdf");
						BoostHelper::createDirectory(sPdfPath);

						if(ConvertPdf(extension,selectFile->LocalPath(),sPdfPath)){
							vector<string> fileViews=selectFile->Views();
							fileViews.push_back(sPdfPath);
							selectFile->Views(fileViews);
						}	
					}
					try
					{
						HapySyncParameter  parameter;
						if(selectFile->Checkin(parameter)){

							selectFile->References();
						}
					}
					catch(HapyException* e){
						string str=selectFile->Name()+"     "+e->Message();
						filenames.push_back(str);
						cstr=L"";
						filemessages.push_back(cstr);
					}
				}
			}

		}
		if(filenames.size()>0){
			//CString cstr=StringUtils::StringToCString(errorstr.c_str());
			//customMethods::customMessagebox(L"��ʾ",cstr,MB_OK|MB_ICONERROR);
			//customMethods::customMessagebox(errorstr,L"Operationfailure",MB_OK|MB_ICONERROR);
			customMethods::customMessagebox(filenames,filemessages,MB_OK|MB_ICONERROR);

		}else{
			//customMethods::customMessagebox(L"��ʾ",L"�����ɹ�!",MB_OK|MB_ICONASTERISK);
			customMethods::customMessagebox(L"Operationsucceeded",MB_OK|MB_ICONASTERISK);
		}
		//List����
		HTREEITEM hItem=m_pprojecttreeview->m_treeGrid.GetSelectedItem();	
		HapyProjectFolder* selectTreeFolder=(HapyProjectFolder*)m_pprojecttreeview->m_treeGrid.GetItemData(hItem);		
		//selectTreeFolder->Refresh();
		this->ResetList();
		this->LoadData(selectTreeFolder);
	}else{
		customMethods::customMessagebox(L"Nofile",MB_OK|MB_ICONEXCLAMATION);
	}	
}
//���-
void CFolderListView::OnCheckOut()
{
	BeginWaitCursor();
	this->GetAllSelectFiles();
	if(m_pselectFiles.size()>0){
		CFileSelectDlg dlg;
		if(dlg.DoModal()!=IDOK)return;
	}else{
		customMethods::customMessagebox(L"Nofile",MB_OK|MB_ICONEXCLAMATION);
		return;
	}
	BeginWaitCursor();
	if(m_pselectFiles.size()>0){
		//string errorstr="";
		vector<string> filenames;
		vector<CString> filemessages;
		CString cstr;
		for (vector<HapyProjectFile*>::iterator it =m_pselectFiles.begin();it!=m_pselectFiles.end();it++ ){
			HapyProjectFile* selectFile=*it;

			ProjectNodeStatus status=selectFile->NodeStatus();
			if(status==Unknow){
				//errorstr=errorstr+thisfile->Name()+" δ֪�ļ����ɼ��!\n";
				filenames.push_back(selectFile->Name());
				cstr=L"checkoutUnknow";
				filemessages.push_back(cstr);
			}
			if(status==Archived){
				//errorstr=errorstr+thisfile->Name()+" �ļ��ѹ鵵���ɼ��!\n";
				filenames.push_back(selectFile->Name());
				cstr=L"checkoutArchived";
				filemessages.push_back(cstr);
			}
			else if(status==LocalFile||status==CheckoutBySelf||status==CheckoutBySomeone){
				//errorstr=errorstr+thisfile->Name()+" δ�����ļ����ɼ��!\n";
				filenames.push_back(selectFile->Name());
				cstr=L"checkoutNotchecked";
				filemessages.push_back(cstr);
			}
			else if(status==ServerFile||status==LocalAndServerDifferent||status==Checkined){
				try
				{
					HapySyncParameter  parameter;
					if(selectFile->Checkout(parameter)){
						selectFile->References();
					}else {
						//errorstr=errorstr+thisfile->Name()+" ���ʧ��!\n";
						filenames.push_back(selectFile->Name());
						cstr=L"Operationfailure";
						filemessages.push_back(cstr);
					}
				}
				catch(HapyException* e){
					//errorstr=errorstr+thisfile->Name()+" "+e->Message()+"\n";
					string str=selectFile->Name()+"     "+e->Message();
					filenames.push_back(str);
					cstr=L"";
					filemessages.push_back(cstr);
				}

			}
		}
		if(filenames.size()>0){
			customMethods::customMessagebox(filenames,filemessages,MB_OK|MB_ICONERROR);

		}else{
			//customMethods::customMessagebox(L"��ʾ",L"�����ɹ�!",MB_OK|MB_ICONASTERISK);
			customMethods::customMessagebox(L"Operationsucceeded",MB_OK|MB_ICONASTERISK);
		}
	}
	//List����
	HTREEITEM hItem=m_pprojecttreeview->m_treeGrid.GetSelectedItem();	
	HapyProjectFolder* selectTreeFolder=(HapyProjectFolder*)m_pprojecttreeview->m_treeGrid.GetItemData(hItem);	
	//selectTreeFolder->Refresh();
	HapyProjectManager::GetInstance()->CurrentProject()->Refresh();
	this->ResetList();
	this->LoadData(selectTreeFolder);
}
//�������-
void CFolderListView::OnUndoCheckOut()
{
	BeginWaitCursor();
	this->GetAllSelectFiles();
	if(m_pselectFiles.size()>0){
		//string errorstr="";
		vector<string> filenames;
		vector<CString> filemessages;
		CString cstr;
		for (vector<HapyProjectFile*>::iterator it =m_pselectFiles.begin();it!=m_pselectFiles.end();it++ ){
			HapyProjectFile* selectFile=*it;
			ProjectNodeStatus status=selectFile->NodeStatus();
			if(status==Unknow){
				//errorstr=errorstr+thisfile->Name()+" δ֪�ļ����ɳ������!\n";
				filenames.push_back(selectFile->Name());
				cstr=L"undocheckoutUnknow";
				filemessages.push_back(cstr);
			}else  if(status==Archived){
				//errorstr=errorstr+thisfile->Name()+" �ļ��ѹ鵵���ɳ������!\n";
				filenames.push_back(selectFile->Name());
				cstr=L"undocheckoutArchived";
				filemessages.push_back(cstr);
			}
			else if(status==Checkined){
				//errorstr=errorstr+thisfile->Name()+" δ����ļ����ɳ������!\n";
				filenames.push_back(selectFile->Name());
				cstr=L"undocheckoutCheckined";
				filemessages.push_back(cstr);
			}
			else if(status==CheckoutBySomeone){
				//errorstr=errorstr+thisfile->Name()+" ���˼�����ļ����ɳ������!\n";
				filenames.push_back(selectFile->Name());
				cstr=L"undoCheckoutBySomeone";
				filemessages.push_back(cstr);
			}
			else {
				try
				{
					HapySyncParameter  parameter;
					if(selectFile->UndoCheckout(parameter)){
						selectFile->References();
					}else {
						//errorstr=errorstr+thisfile->Name()+" �������ʧ��!\n";
						filenames.push_back(selectFile->Name());
						cstr=L"Operationfailure";
						filemessages.push_back(cstr);

					}
				}
				catch(HapyException* e){

					//errorstr=errorstr+thisfile->Name()+" "+e->Message()+"\n";
					string str=selectFile->Name()+"     "+e->Message();
					filenames.push_back(str);
					cstr=L"";
					filemessages.push_back(cstr);
				}

			}
		}
		if(filenames.size()>0){
			customMethods::customMessagebox(filenames,filemessages,MB_OK|MB_ICONERROR);

		}else{
			//customMethods::customMessagebox(L"��ʾ",L"�����ɹ�!",MB_OK|MB_ICONASTERISK);
			customMethods::customMessagebox(L"Operationsucceeded",MB_OK|MB_ICONASTERISK);
		}
		//List����
		HTREEITEM hItem=m_pprojecttreeview->m_treeGrid.GetSelectedItem();	
		HapyProjectFolder* selectTreeFolder=(HapyProjectFolder*)m_pprojecttreeview->m_treeGrid.GetItemData(hItem);	
		this->ResetList();
		this->LoadData(selectTreeFolder);
	}	
}
//��ȡ
void CFolderListView::OnDownload()
{
	BeginWaitCursor();
	HapyProject* curProject= HapyProjectManager::GetInstance()->CurrentProject();
	if(curProject->LocalPath()==""){
		customMethods::customMessagebox(L"workDirIsempty",MB_OK|MB_ICONERROR);
		return;
	}	
	this->GetAllSelectFiles();
	if(m_pselectFiles.size()>0){
		//string errorstr="";
		vector<string> filenames;
		vector<CString> filemessages;
		CString cstr;

		for (vector<HapyProjectFile*>::iterator it =m_pselectFiles.begin();it!=m_pselectFiles.end();it++ ){
			HapyProjectFile* selectFile=*it;
			ProjectNodeStatus status=selectFile->NodeStatus();
			if(status==LocalFile||status==Unknow){
				continue;
			}else if(status==Checkined) {
				continue;
			}else {
				try{
					if(!selectFile->Download()){
						filenames.push_back(selectFile->Name());
						cstr=L"Operationfailure";
						filemessages.push_back(cstr);
					}
				}catch(HapyException* e){
					//errorstr=errorstr+thisfile->Name()+" "+e->Message()+"\n";
					string str=selectFile->Name()+"     "+e->Message();
					filenames.push_back(str);
					cstr=L"";
					filemessages.push_back(cstr);
				}
			}
		}				
		if(filenames.size()>0){
			customMethods::customMessagebox(filenames,filemessages,MB_OK|MB_ICONERROR);

		}else{
			customMethods::customMessagebox(L"Operationsucceeded",MB_OK|MB_ICONASTERISK);
		}
		//List����
		HTREEITEM hItem=m_pprojecttreeview->m_treeGrid.GetSelectedItem();	
		HapyProjectFolder* selectTreeFolder=(HapyProjectFolder*)m_pprojecttreeview->m_treeGrid.GetItemData(hItem);	
		this->ResetList();
		this->LoadData(selectTreeFolder);
	}
}

//������С
void CFolderListView::OnSize(UINT nType, int cx, int cy)
{
	CBCGPFormView::OnSize(nType, cx, cy);
	// TODO: �ڴ˴������Ϣ����������
	if(m_Breadcrumb){
		CRect thisrect,viewrect;
		this->GetClientRect(&viewrect);
		thisrect.SetRect(-1,-1,viewrect.right-35,35);
		m_Breadcrumb.MoveWindow(&thisrect);
	}
	if(m_listUpdate){
		CRect thisrect,viewrect;
		this->GetClientRect(&viewrect);
		thisrect.SetRect(viewrect.right-36,0,viewrect.right,33);
		m_listUpdate.MoveWindow(&thisrect);
	}

}
//�ر�-2
void CFolderListView::OnClose()
{
	//customMethods::customMessagebox(L"��ʾ",L"�ر�close��",MB_OK|MB_ICONASTERISK); 
	CBCGPFormView::OnClose();
}
void CFolderListView::OnDestroy()
{
	m_listGrid.SaveState (_T("list"));
	CBCGPFormView::OnDestroy();
}
//�����б�
void CFolderListView::ResetList(){
	this->m_listGrid.RemoveAll();
}
//�����б�����
void CFolderListView::LoadData(HapyProjectFolder* hapyFolder){

	this->m_listGrid.SetRedraw(FALSE);
	vector<HapyProjectFolder*>* subFolders=hapyFolder->SubFolders();
	if (subFolders->size()>0)
	{
		for (vector<HapyProjectFolder*>::iterator	it =subFolders->begin();it!=subFolders->end();it++)
		{
			HapyProjectFolder* subFolder= *it;
			CString filename (subFolder->Name().c_str());
			if(filename=="_V")  continue;
			CBCGPGridRow* newRow= this->m_listGrid.CreateRow (this->m_listGrid.GetColumnCount());
			ASSERT_VALID (newRow);
			newRow->SetData((DWORD_PTR)subFolder);	
			//�ļ���,icon

			newRow->GetItem (listicon)->SetImage(Wfolder);
			newRow->GetItem (listname)->SetValue ((LPCTSTR)filename);
			//����
			CString createname (subFolder->CreateName().c_str());
			newRow->GetItem (listauthor)->SetValue ((LPCTSTR)createname);
			//����ʱ��
			CString createtime (subFolder->CreateDate().c_str());
			customMethods::timeFormatter(createtime);
			newRow->GetItem (listcreatetime)->SetValue ((LPCTSTR)createtime);
			this->m_listGrid.AddRow (newRow, FALSE);
		}
	}
	vector<HapyProjectFile*>* subFiles= hapyFolder->SubFiles();
	if (subFiles->size()>0)
	{
		for (vector<HapyProjectFile*>::iterator it = subFiles->begin();it!=subFiles->end();it++)
		{
			HapyProjectFile* projectFile= *it;
			if(BoostHelper::getExtension(projectFile->Name())==".bak") continue;
			CBCGPGridRow* pchild=this->m_listGrid.CreateRow ( this->m_listGrid.GetColumnCount ());
			ASSERT_VALID (pchild);
			this->m_listGrid.AddRow (pchild, FALSE);
			int stateicon=empty;
			pchild->SetData((DWORD_PTR)projectFile);	
			//���ü��stateͼ��
			switch (projectFile->NodeStatus())
			{
			case CheckoutBySelf:
				stateicon= checkoutBySelf;
				break;
			case CheckoutBySomeone:
				stateicon= checkoutBySomeOne;
				break;
			case LocalAndServerDifferent:
				stateicon= localfilenotmatch;
				break;
			case Checkined:
				stateicon= filecheckin;
				break;
			case Archived:
				stateicon= fileArchived;
				break;
			}
			pchild->GetItem (listcheck)->SetImage(stateicon);			
			//�ļ��� and icon
			CString filename (projectFile->Name().c_str());
			string extension=BoostHelper::getExtension(projectFile->Name());
			extension.assign(extension);
			std::transform(extension.begin(),extension.end(),extension.begin(),tolower); 
			int fileicon=m_ExtensionAndIconMapping[extension];
			pchild->GetItem (listicon)->SetImage(fileicon);
			pchild->GetItem (listname)->SetValue ((LPCTSTR)filename);

			if(projectFile->NodeStatus()==LocalFile) continue;
			//����
			CString fileextension (projectFile->Extension().c_str());
			pchild->GetItem (listtype)->SetValue ((LPCTSTR)fileextension);
			//��С
			CString filelen=customMethods::fileSizeUnit(projectFile->FileSize());
			pchild->GetItem (listsize)->SetValue ((LPCTSTR)filelen);			
			//����
			CString createname (projectFile->CreateName().c_str());
			pchild->GetItem (listauthor)->SetValue ((LPCTSTR)createname);
			//����ʱ��
			CString createtime (projectFile->CreateTime().c_str());
			customMethods::timeFormatter(createtime);
			pchild->GetItem (listcreatetime)->SetValue ((LPCTSTR)createtime);
			//�汾
			CString version;
			version.Format(L"%d",projectFile->FileVersion());
			pchild->GetItem (listversion)->SetValue ((LPCTSTR)version);
			//���
			CString fileReversionLabel (projectFile->FileReversionLabel().c_str());
			pchild->GetItem (listrevision)->SetValue ((LPCTSTR)fileReversionLabel);
			//��������״̬
			CString statusname (projectFile->StateName().c_str());
			pchild->GetItem (listlife)->SetValue ((LPCTSTR)statusname);
			//�����
			CString checkoutname (projectFile->CheckoutName().c_str());
			pchild->GetItem (listout)->SetValue ((LPCTSTR)checkoutname);
			//���ʱ��
			CString filecheckouttime (projectFile->FileCheckouttime().c_str());
			customMethods::timeFormatter(filecheckouttime);
			pchild->GetItem (listouttime)->SetValue ((LPCTSTR)filecheckouttime);
			//������
			CString checkinname (projectFile->CheckinName().c_str());
			pchild->GetItem (listin)->SetValue ((LPCTSTR)checkinname);
			//����ʱ��
			CString filecheckintime (projectFile->FileCheckintime().c_str());
			customMethods::timeFormatter(filecheckintime);
			pchild->GetItem (listintime)->SetValue ((LPCTSTR)filecheckintime);
			//��ע
			pchild->GetItem (listnote)->SetValue (_T(""));


		}
	}
	this->m_listGrid.SetRedraw(TRUE);
	this->m_listGrid.AdjustLayout();
}
//���·��������
void CFolderListView::AddBreadcrumb(HapyProjectFolder* folder,HBREADCRUMBITEM parentItem){

	vector<HapyProjectFolder *>* subFolders=folder->SubFolders();
	if(subFolders->size()>0){
		for (vector<HapyProjectFolder*>::iterator it =subFolders->begin();it!=subFolders->end();it++ )
		{
			HapyProjectFolder* childFolder= *it;
			CString	 name(childFolder->Name().c_str());

			HBREADCRUMBITEM hItem=this->m_Breadcrumb.InsertItem (parentItem, name);
			this->m_Breadcrumb.SetItemData(hItem,(DWORD_PTR)childFolder);

			AddBreadcrumb(childFolder, hItem);
			this->m_Breadcrumb.GetItemParent(hItem);
		}
	}
}
void CFolderListView::LoadBreadcrumb(HapyProjectFolder* folder){
	HBREADCRUMBITEM hroot=this->m_Breadcrumb.GetRootItem();
	CString	 name(folder->Name().c_str());
	//this->m_wndBreadcrumb.DeleteItem(hroot);
	this->m_Breadcrumb.SetItemText(hroot,name);
	this->m_Breadcrumb.SetItemData(hroot,(DWORD_PTR)folder);
	this->AddBreadcrumb(folder,hroot);
}
//����·����
void CFolderListView::ResetBreadcrumb(){

	/*HBREADCRUMBITEM hroot=this->m_Breadcrumb.GetRootItem();
	this->m_Breadcrumb.DeleteItem(hroot);*/
	this->m_Breadcrumb.DeleteAllItem();
}
//��ȡ�ļ����������ļ�
void CFolderListView::GetAllSubFiles(HapyProjectFolder* folder){
	vector<HapyProjectFile*>* subFiles=folder->SubFiles();
	vector<HapyProjectFolder*>* subFolders=folder->SubFolders();
	if(subFiles->size()>0){
		this->m_pselectFiles.insert(this->m_pselectFiles.end(),subFiles->begin(),subFiles->end());
	}
	if(subFolders->size()>0){
		for (vector<HapyProjectFolder*>::iterator it =subFolders->begin();it!=subFolders->end();it++ ){
			HapyProjectFolder* subfolder=*it;
			this->GetAllSubFiles(subfolder);
		}
	}
}
//��ȡѡ�е������ļ�
void CFolderListView::GetAllSelectFiles(){
	CList <CBCGPGridItem*, CBCGPGridItem*> lstSelected;
	m_listGrid.GetSelectedItems(lstSelected);
	POSITION pos=lstSelected.GetHeadPosition();
	if(pos==NULL)
	{
		customMethods::customMessagebox(L"��ʾ",L"������ѡ��һ�",MB_OK|MB_ICONEXCLAMATION);
		return;
	}
	//���
	vector<HapyProjectFile*>().swap(m_pselectFiles);
	BeginWaitCursor();
	//��ȡѡ�е������ļ�
	while(pos){
		CBCGPGridItem* nitem= lstSelected.GetNext(pos); 
		CBCGPGridRow* nrow=nitem->GetParentRow();
		HapyProjectNode* selectNode=(HapyProjectNode*)nrow->GetData();
		if(selectNode==NULL) return;
		//�ļ�
		if(selectNode->IsFile()){
			HapyProjectFile* selectFile= dynamic_cast<HapyProjectFile*>(selectNode);
			m_pselectFiles.push_back(selectFile);
		}else{
			HapyProjectFolder* selectFolder= dynamic_cast<HapyProjectFolder*>(selectNode);
			this->GetAllSubFiles(selectFolder);
		}
	}
}
//Ctrl+F
void CFolderListView::OnCtrlf()
{
	// TODO: �ڴ���������������
	this->m_listGrid.OpenFindReplaceDlg();
}
//�ȼ���Ϣ
BOOL CFolderListView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (m_hAccel && ::TranslateAccelerator(GetSafeHwnd(), m_hAccel, pMsg))
		return TRUE;

	return CBCGPFormView::PreTranslateMessage(pMsg);
}
//ˢ��
void CFolderListView::OnBnClickedListupdate()
{
	HTREEITEM hItem=m_pprojecttreeview->m_treeGrid.GetSelectedItem();
	if(hItem==NULL)return;
	bool isroot=FALSE;
	if(hItem==m_pprojecttreeview->m_treeGrid.GetRootItem())
		isroot=TRUE;
	HapyProjectFolder* selectTreeFolder=(HapyProjectFolder*)m_pprojecttreeview->m_treeGrid.GetItemData(hItem);	
	//selectTreeFolder->Refresh();
	if(selectTreeFolder==NULL) return;
	//
	CString thispath=this->m_Breadcrumb.GetSelectedPath(_T('\\'));
	//refresh
	HapyProjectManager::GetInstance()->Refresh();
	vector<HapyProject*> projs=HapyProjectManager::GetInstance()->GetProjects();
	string projectname=StringUtils::CStringToString
		(m_pprojecttreeview->m_treeGrid.GetItemText(m_pprojecttreeview->m_treeGrid.GetRootItem()));
	HapyProjectManager::GetInstance()->SwitchProject(projectname);
	HapyProject* curProject= HapyProjectManager::GetInstance()->CurrentProject();
	BeginWaitCursor();
	m_pprojecttreeview->m_treeGrid.SetRedraw(FALSE);
	this->m_listGrid.SetRedraw(FALSE);
	m_pprojecttreeview->ResetTree();
	m_pprojecttreeview->LoadData(curProject->ProjectRoot());
	this->ResetBreadcrumb();
	isRefresh=TRUE;
	this->LoadBreadcrumb(curProject->ProjectRoot());
	if(isroot) m_pprojecttreeview->m_treeGrid.SelectItem( m_pprojecttreeview->m_treeGrid.GetRootItem());
	else this->m_Breadcrumb.SelectPath(thispath,_T('\\'));
	EndWaitCursor();
}

