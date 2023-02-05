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
// CMyListView 诊断

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


//CMyListView 消息处理程序
LRESULT CFolderListView::HandleInitDialog(WPARAM wParam, LPARAM lParam)
{
	BOOL bRes = (BOOL)CBCGPFormView::HandleInitDialog(wParam, lParam);	

	UINT nListStyles = WS_CHILD | WS_VISIBLE  | WS_BORDER | 
		TVS_SHOWSELALWAYS|
		TVS_FULLROWSELECT; //整行选中
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
//初始化
void CFolderListView::OnInitialUpdate()
{
	CBCGPFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	//快捷键相关
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
	m_listUpdate.SetTooltip(L"刷新");

	//HICON m_hicn1=AfxGetApp()->LoadIcon(IDI_outlook); 
	//m_listUpdate.SetIcon(m_hicn1);
	//ListCtrl设置////////////////////////////////////////////////////////////////////////////
	m_listGrid.EnableHeader (TRUE, BCGP_GRID_HEADER_SORT);
	m_listGrid.EnableColumnAutoSize(TRUE); //设置列自适应宽度
	m_listGrid.EnableGridLines(TRUE);//显示gridline
	m_listGrid.EnableRowHeader(FALSE);
	m_listGrid.EnableAlternateRows(FALSE);//启用交替行
	m_listGrid.SetReadOnly ();
	m_listGrid.SetWholeRowSel ();//整行
	m_listGrid.SetCustomRowHeight(38,38);//设置行宽，第二个参数决定


	//主题，字体，图标
	m_listGrid.SetVisualManagerColorTheme(TRUE);
	m_listGrid.SetFont(&theApp.m_font);
	CMainFrame*pMainFrame = DYNAMIC_DOWNCAST(CMainFrame,AfxGetMainWnd());
	m_listGrid.SetImageList(&pMainFrame->m_ImageList);

	//添加表头
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
	/*m_listGrid.InsertColumn(listname, _T("文件名称"), onewidth);
	m_listGrid.InsertColumn(listtype, _T("文件类型"), onewidth);
	m_listGrid.InsertColumn(listsize, _T("文件大小"), onewidth);
	m_listGrid.InsertColumn(listauthor , _T("作者"), onewidth);
	m_listGrid.InsertColumn(listcreatetime, _T("创建时间"), onewidth);
	m_listGrid.InsertColumn(listversion , _T("版本"), onewidth);	
	m_listGrid.InsertColumn(listedition, _T("版次"), onewidth);
	m_listGrid.InsertColumn(listlife , _T("生命周期状态"), onewidth);
	m_listGrid.InsertColumn(listout , _T("检出人"), onewidth);
	m_listGrid.InsertColumn(listouttime , _T("检出时间"), onewidth);
	m_listGrid.InsertColumn(listin, _T("检入人"), onewidth);
	m_listGrid.InsertColumn(listintime , _T("检入时间"), onewidth);
	m_listGrid.InsertColumn(listnote , _T("备注"), onewidth);*/
	//锁定居中
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
//右键菜单
void CFolderListView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CMenu menu;
	menu.LoadMenuW(IDR_FolderMenu);
	CMenu *contextMenu = menu.GetSubMenu(0);
	ASSERT(contextMenu != NULL);
	/*menu.GetSubMenu(0)->TrackPopupMenu
	(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, GetParent());//创建一个浮动的菜单	*/
	//首先得到点击的位置
	CBCGPGridRow* pchild= this->m_listGrid.GetCurSel();
	if(pchild==NULL) return;	
	UINT i=m_listGrid.GetSelectedItemCount()/this->m_listGrid.GetColumnCount();
	//单选
	if(i==1){
		HapyProjectNode* selectNode=(HapyProjectNode*)pchild->GetData();
		if(selectNode==NULL) return;
		//文件
		if(selectNode->IsFile()){
			HapyProjectFile* selectFile= dynamic_cast<HapyProjectFile*>(selectNode);
			//如果是检出状态
			if(selectFile->NodeStatus()==CheckoutBySelf||selectFile->NodeStatus()==CheckoutBySomeone){
				contextMenu->RemoveMenu(ID_Menu_CheckOut, MF_BYCOMMAND);//检出
				contextMenu->RemoveMenu(ID_Menu_AttachedFile, MF_BYCOMMAND );//附加文件
			}
			//本地文件
			else if(selectFile->NodeStatus()==LocalFile){	
				//过滤不可检入文件
				for (vector<string>::iterator it=m_filefilter.begin();it!=m_filefilter.end();it++)
				{
					if(BoostHelper::getExtension(selectFile->Name())==*it){
						return;
					}
				}	
				contextMenu->RemoveMenu(ID_Menu_CheckOut, MF_BYCOMMAND);//检出
				contextMenu->RemoveMenu(ID_Menu_UndoCheckOut, MF_BYCOMMAND);//撤销检出
				contextMenu->RemoveMenu(ID_MENU_Download, MF_BYCOMMAND );//获取
				contextMenu->RemoveMenu(ID_Menu_AttachedFile, MF_BYCOMMAND );//附加文件


			}else if(selectFile->NodeStatus()==Archived){	
				contextMenu->RemoveMenu(ID_Menu_CheckIn, MF_BYCOMMAND);//检入
				contextMenu->RemoveMenu(ID_Menu_CheckOut, MF_BYCOMMAND);//检出
				contextMenu->RemoveMenu(ID_Menu_UndoCheckOut, MF_BYCOMMAND);//撤销检出
				contextMenu->RemoveMenu(ID_Menu_AttachedFile, MF_BYCOMMAND );//附加文件	
			}
			//检入的状态
			else {
				contextMenu->RemoveMenu(ID_Menu_CheckIn, MF_BYCOMMAND);//检入
				contextMenu->RemoveMenu(ID_Menu_UndoCheckOut, MF_BYCOMMAND);//撤销检出
				contextMenu->RemoveMenu(ID_Menu_AttachedFile, MF_BYCOMMAND );//附加文件
			}
		}//目录
		else {	
			contextMenu->RemoveMenu(ID_Menu_CheckIn, MF_BYCOMMAND);//检入
			//contextMenu->RemoveMenu(ID_Menu_AttachedFile, MF_BYCOMMAND );//附加文件
		}
	}
	//多选
	else if(i>1)
	{
		contextMenu->RemoveMenu(ID_Menu_AttachedFile, MF_BYCOMMAND );//附加文件
	}
	CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;
	pPopupMenu->Create (this, point.x, point.y, contextMenu->Detach (),FALSE,TRUE);
	pPopupMenu->SetForceMenuFocus(TRUE);
	pPopupMenu->SetAutoDestroy(TRUE);
	pPopupMenu->ActivatePopupMenu((CBCGPFrameWnd *)AfxGetMainWnd(),pPopupMenu);
	::SetForegroundWindow(pPopupMenu->m_hWnd);


}
//选中
LRESULT CFolderListView::OnSelchangedList(WPARAM wParam, LPARAM lParam)
{
	//获取选中的文件或目录
	CBCGPGridRow* pchild= m_listGrid.GetCurSel();
	if(pchild==NULL) return FALSE;
	//重置
	CMainFrame*pMainFrame = DYNAMIC_DOWNCAST(CMainFrame,AfxGetMainWnd());
	pMainFrame->m_wndPropBar.ResetList();
	m_pversionDlg->ResetList();
	m_preferenceDlg->ResetComboAndTree();
	//获取结点绑定指针
	HapyProjectNode* selectNode=(HapyProjectNode*)pchild->GetData();
	if(selectNode==NULL) return FALSE;
	//文件
	if(selectNode->IsFile()&&selectNode->NodeStatus()!=LocalFile){
		HapyProjectFile* selectFile= dynamic_cast<HapyProjectFile*>(selectNode);
		m_pselectFile=selectFile;
		m_pdetailsView->ResetIsLoad();
		BeginWaitCursor();
		//属性表
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
	}//目录
	else{ 
		//customMethods::customMessagebox(L"提示",L"目录！",MB_OK|MB_ICONERROR);
	}
	this->m_listGrid.SetFocus();
	return TRUE;
}
//路径栏改变
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
//双击打开
LRESULT CFolderListView::OnGridDoubleClicked(WPARAM wParam, LPARAM lParam)
{
	//获取选中的文件或目录
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
//检入
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
				//errorstr=errorstr+selectFile->Name()+" 未知文件不可检入!\n";
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
				//errorstr=errorstr+selectFile->Name()+" 归档文件不可检入!\n";
				//errorstr=errorstr+selectFile->Name()+"\n";
				filenames.push_back(selectFile->Name());
				cstr=L"checkinArchived";
				filemessages.push_back(cstr);
			}
			else  if(status==CheckoutBySomeone){
				//errorstr=errorstr+selectFile->Name()+" 文件他人检出，不可操作该文件!\n";
				//errorstr=errorstr+selectFile->Name()+"\n";
				filenames.push_back(selectFile->Name());
				cstr=L"checkinCheckoutBySomeone";
				filemessages.push_back(cstr);

			}
			else if(status==Checkined) {
				//errorstr=errorstr+selectFile->Name()+" 文件未检出不可检入!\n";
				//errorstr=errorstr+selectFile->Name()+"\n";
				filenames.push_back(selectFile->Name());
				cstr=L"checkinCheckined";
				filemessages.push_back(cstr);

			}
			else {
				//过滤不可检入文件
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
					//给word，excel，ppt文件添加pdf附件后检入
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
			//customMethods::customMessagebox(L"提示",cstr,MB_OK|MB_ICONERROR);
			//customMethods::customMessagebox(errorstr,L"Operationfailure",MB_OK|MB_ICONERROR);
			customMethods::customMessagebox(filenames,filemessages,MB_OK|MB_ICONERROR);

		}else{
			//customMethods::customMessagebox(L"提示",L"操作成功!",MB_OK|MB_ICONASTERISK);
			customMethods::customMessagebox(L"Operationsucceeded",MB_OK|MB_ICONASTERISK);
		}
		//List更新
		HTREEITEM hItem=m_pprojecttreeview->m_treeGrid.GetSelectedItem();	
		HapyProjectFolder* selectTreeFolder=(HapyProjectFolder*)m_pprojecttreeview->m_treeGrid.GetItemData(hItem);		
		//selectTreeFolder->Refresh();
		this->ResetList();
		this->LoadData(selectTreeFolder);
	}else{
		customMethods::customMessagebox(L"Nofile",MB_OK|MB_ICONEXCLAMATION);
	}	
}
//检出-
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
				//errorstr=errorstr+thisfile->Name()+" 未知文件不可检出!\n";
				filenames.push_back(selectFile->Name());
				cstr=L"checkoutUnknow";
				filemessages.push_back(cstr);
			}
			if(status==Archived){
				//errorstr=errorstr+thisfile->Name()+" 文件已归档不可检出!\n";
				filenames.push_back(selectFile->Name());
				cstr=L"checkoutArchived";
				filemessages.push_back(cstr);
			}
			else if(status==LocalFile||status==CheckoutBySelf||status==CheckoutBySomeone){
				//errorstr=errorstr+thisfile->Name()+" 未检入文件不可检出!\n";
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
						//errorstr=errorstr+thisfile->Name()+" 检出失败!\n";
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
			//customMethods::customMessagebox(L"提示",L"操作成功!",MB_OK|MB_ICONASTERISK);
			customMethods::customMessagebox(L"Operationsucceeded",MB_OK|MB_ICONASTERISK);
		}
	}
	//List更新
	HTREEITEM hItem=m_pprojecttreeview->m_treeGrid.GetSelectedItem();	
	HapyProjectFolder* selectTreeFolder=(HapyProjectFolder*)m_pprojecttreeview->m_treeGrid.GetItemData(hItem);	
	//selectTreeFolder->Refresh();
	HapyProjectManager::GetInstance()->CurrentProject()->Refresh();
	this->ResetList();
	this->LoadData(selectTreeFolder);
}
//撤销检出-
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
				//errorstr=errorstr+thisfile->Name()+" 未知文件不可撤销检出!\n";
				filenames.push_back(selectFile->Name());
				cstr=L"undocheckoutUnknow";
				filemessages.push_back(cstr);
			}else  if(status==Archived){
				//errorstr=errorstr+thisfile->Name()+" 文件已归档不可撤销检出!\n";
				filenames.push_back(selectFile->Name());
				cstr=L"undocheckoutArchived";
				filemessages.push_back(cstr);
			}
			else if(status==Checkined){
				//errorstr=errorstr+thisfile->Name()+" 未检出文件不可撤销检出!\n";
				filenames.push_back(selectFile->Name());
				cstr=L"undocheckoutCheckined";
				filemessages.push_back(cstr);
			}
			else if(status==CheckoutBySomeone){
				//errorstr=errorstr+thisfile->Name()+" 他人检出的文件不可撤销检出!\n";
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
						//errorstr=errorstr+thisfile->Name()+" 撤销检出失败!\n";
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
			//customMethods::customMessagebox(L"提示",L"操作成功!",MB_OK|MB_ICONASTERISK);
			customMethods::customMessagebox(L"Operationsucceeded",MB_OK|MB_ICONASTERISK);
		}
		//List更新
		HTREEITEM hItem=m_pprojecttreeview->m_treeGrid.GetSelectedItem();	
		HapyProjectFolder* selectTreeFolder=(HapyProjectFolder*)m_pprojecttreeview->m_treeGrid.GetItemData(hItem);	
		this->ResetList();
		this->LoadData(selectTreeFolder);
	}	
}
//获取
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
		//List更新
		HTREEITEM hItem=m_pprojecttreeview->m_treeGrid.GetSelectedItem();	
		HapyProjectFolder* selectTreeFolder=(HapyProjectFolder*)m_pprojecttreeview->m_treeGrid.GetItemData(hItem);	
		this->ResetList();
		this->LoadData(selectTreeFolder);
	}
}

//调整大小
void CFolderListView::OnSize(UINT nType, int cx, int cy)
{
	CBCGPFormView::OnSize(nType, cx, cy);
	// TODO: 在此处添加消息处理程序代码
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
//关闭-2
void CFolderListView::OnClose()
{
	//customMethods::customMessagebox(L"提示",L"关闭close！",MB_OK|MB_ICONASTERISK); 
	CBCGPFormView::OnClose();
}
void CFolderListView::OnDestroy()
{
	m_listGrid.SaveState (_T("list"));
	CBCGPFormView::OnDestroy();
}
//重置列表
void CFolderListView::ResetList(){
	this->m_listGrid.RemoveAll();
}
//加载列表数据
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
			//文件名,icon

			newRow->GetItem (listicon)->SetImage(Wfolder);
			newRow->GetItem (listname)->SetValue ((LPCTSTR)filename);
			//作者
			CString createname (subFolder->CreateName().c_str());
			newRow->GetItem (listauthor)->SetValue ((LPCTSTR)createname);
			//创建时间
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
			//设置检出state图标
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
			//文件名 and icon
			CString filename (projectFile->Name().c_str());
			string extension=BoostHelper::getExtension(projectFile->Name());
			extension.assign(extension);
			std::transform(extension.begin(),extension.end(),extension.begin(),tolower); 
			int fileicon=m_ExtensionAndIconMapping[extension];
			pchild->GetItem (listicon)->SetImage(fileicon);
			pchild->GetItem (listname)->SetValue ((LPCTSTR)filename);

			if(projectFile->NodeStatus()==LocalFile) continue;
			//类型
			CString fileextension (projectFile->Extension().c_str());
			pchild->GetItem (listtype)->SetValue ((LPCTSTR)fileextension);
			//大小
			CString filelen=customMethods::fileSizeUnit(projectFile->FileSize());
			pchild->GetItem (listsize)->SetValue ((LPCTSTR)filelen);			
			//作者
			CString createname (projectFile->CreateName().c_str());
			pchild->GetItem (listauthor)->SetValue ((LPCTSTR)createname);
			//创建时间
			CString createtime (projectFile->CreateTime().c_str());
			customMethods::timeFormatter(createtime);
			pchild->GetItem (listcreatetime)->SetValue ((LPCTSTR)createtime);
			//版本
			CString version;
			version.Format(L"%d",projectFile->FileVersion());
			pchild->GetItem (listversion)->SetValue ((LPCTSTR)version);
			//版次
			CString fileReversionLabel (projectFile->FileReversionLabel().c_str());
			pchild->GetItem (listrevision)->SetValue ((LPCTSTR)fileReversionLabel);
			//生命周期状态
			CString statusname (projectFile->StateName().c_str());
			pchild->GetItem (listlife)->SetValue ((LPCTSTR)statusname);
			//检出人
			CString checkoutname (projectFile->CheckoutName().c_str());
			pchild->GetItem (listout)->SetValue ((LPCTSTR)checkoutname);
			//检出时间
			CString filecheckouttime (projectFile->FileCheckouttime().c_str());
			customMethods::timeFormatter(filecheckouttime);
			pchild->GetItem (listouttime)->SetValue ((LPCTSTR)filecheckouttime);
			//检入人
			CString checkinname (projectFile->CheckinName().c_str());
			pchild->GetItem (listin)->SetValue ((LPCTSTR)checkinname);
			//检入时间
			CString filecheckintime (projectFile->FileCheckintime().c_str());
			customMethods::timeFormatter(filecheckintime);
			pchild->GetItem (listintime)->SetValue ((LPCTSTR)filecheckintime);
			//备注
			pchild->GetItem (listnote)->SetValue (_T(""));


		}
	}
	this->m_listGrid.SetRedraw(TRUE);
	this->m_listGrid.AdjustLayout();
}
//添加路径栏数据
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
//重置路径栏
void CFolderListView::ResetBreadcrumb(){

	/*HBREADCRUMBITEM hroot=this->m_Breadcrumb.GetRootItem();
	this->m_Breadcrumb.DeleteItem(hroot);*/
	this->m_Breadcrumb.DeleteAllItem();
}
//获取文件夹下所有文件
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
//获取选中的所有文件
void CFolderListView::GetAllSelectFiles(){
	CList <CBCGPGridItem*, CBCGPGridItem*> lstSelected;
	m_listGrid.GetSelectedItems(lstSelected);
	POSITION pos=lstSelected.GetHeadPosition();
	if(pos==NULL)
	{
		customMethods::customMessagebox(L"提示",L"请至少选择一项！",MB_OK|MB_ICONEXCLAMATION);
		return;
	}
	//清空
	vector<HapyProjectFile*>().swap(m_pselectFiles);
	BeginWaitCursor();
	//获取选中的所有文件
	while(pos){
		CBCGPGridItem* nitem= lstSelected.GetNext(pos); 
		CBCGPGridRow* nrow=nitem->GetParentRow();
		HapyProjectNode* selectNode=(HapyProjectNode*)nrow->GetData();
		if(selectNode==NULL) return;
		//文件
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
	// TODO: 在此添加命令处理程序代码
	this->m_listGrid.OpenFindReplaceDlg();
}
//热键消息
BOOL CFolderListView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (m_hAccel && ::TranslateAccelerator(GetSafeHwnd(), m_hAccel, pMsg))
		return TRUE;

	return CBCGPFormView::PreTranslateMessage(pMsg);
}
//刷新
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

