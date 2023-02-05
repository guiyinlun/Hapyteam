#include"stdafx.h"
#include"CustomMethods.h"
#include "hapyteamclient.h"
#include "MainFrm.h"
#include"ProjectTreeView.h"
#include"FolderListView.h"
#include"FileDetailsLayoutView.h"
#include"VersionListDlg.h"
#include"ReferenceListDlg.h"
#include"ByReferenceListDlg.h"
#include"RevisionListDlg.h"
#include"AnnotateDlg.h"
#include "LoginDlg.h"
#include "ProjectListDlg.h"
#include<string>
#include <Core/StringUtils.h>
#include <Interface/HapyUserManager.h>
#include"UserInfoDlg.h"
#include <Core/FileDownloader.h>
#include <Core/Utility.h>
#include <map>
void CHAPYTreeCtrl::InitColors(){

	// call the base implementation
	CBCGPTreeCtrlEx::InitColors();

	COLORREF clrLineDefault = !globalData.IsHighContrastMode() ? globalData.clrBtnFace : globalData.clrBarShadow;

	// Recreate pen for vertical line
	if (m_penVLine.GetSafeHandle() != NULL)
	{
		m_penVLine.DeleteObject();
	}

	m_penVLine.CreatePen(PS_NULL, 1,
		m_ColorData.m_clrVertLine != (COLORREF)-1 ? m_ColorData.m_clrVertLine : clrLineDefault);

}
CHAPYListCtrl::CHAPYListCtrl(){
	
}
void CHAPYListCtrl::InitColors(){

	// call the base implementation
	CBCGPGridCtrl::InitColors();

	COLORREF clrLineDefault = !globalData.IsHighContrastMode() ? globalData.clrBtnFace : globalData.clrBarShadow;

	// Recreate pen for horizontal line
	/*if (m_penHLine.GetSafeHandle() != NULL)
	{
		m_penHLine.DeleteObject();
	}

	m_penHLine.CreatePen(PS_NULL, 1,RGB(202,81,0));
		//m_ColorData.m_clrHorzLine != (COLORREF)1 ? m_ColorData.m_clrHorzLine : clrLineDefault);
		//m_ColorData.m_clrHorzLine != (COLORREF)1 ? m_ColorData.m_clrHorzLine : clrLineDefault);*/
	// Recreate pen for vertical line
	if (m_penVLine.GetSafeHandle() != NULL)
	{
		m_penVLine.DeleteObject();
	}

	m_penVLine.CreatePen(PS_NULL, 1,
		m_ColorData.m_clrVertLine != (COLORREF)-1 ? m_ColorData.m_clrVertLine : clrLineDefault);

}






map<string,int> filetype_map;
void  initmap(){
	filetype_map[".zip"]=zip;
	filetype_map[".ipt"]=ipt;
	filetype_map[".doc"]=doc;
	filetype_map[".docx"]=docx;
	filetype_map[".vsd"]=vsd;
	filetype_map[".txt"]=txt;
	filetype_map[".ses"]=ses;
	filetype_map[".sldprt"]=sldprt;
	filetype_map[".sldasm"]=sldasm;
	filetype_map[".rvt"]=rvt;
	filetype_map[".rvm"]=rvm;
	filetype_map[".rtf"]=rtf;
	filetype_map[".rfa"]=rfa;
	filetype_map[".prt"]=prt;
	filetype_map[".ppt"]=ppt;
	filetype_map[".pptx"]=ppt;
	filetype_map[".png"]=png;
	filetype_map[".one"]=one;
	filetype_map[".pdf"]=pdf;
	filetype_map[".nwf"]=nwf;
	filetype_map[".nwc"]=nwc;
	filetype_map[".mpp"]=mpp;
	filetype_map[".jpg"]=jpg;
	filetype_map[".ipj"]=ipj;
	filetype_map[".gif"]=gif;
	filetype_map[".xlsx"]=xlsx;
	filetype_map[".xls"]=xls;
	filetype_map[".dxf"]=dxf;
	filetype_map[".dwg"]=dwg;
	filetype_map[".dgn"]=dgn;
	filetype_map[".csv"]=csv;
	filetype_map[".catproduct"]=catproduct;
	filetype_map[".catpart"]=catpart;
	filetype_map[".assembly"]=assembly;
	filetype_map[".asm"]=myasm;
	filetype_map[".iam"]=iam;
	filetype_map[".idw"]=idw;
	filetype_map[".slddrw"]=slddrw;
}
UINT  DownloadThread(LPVOID lparam){
	SetThreadUILanguage(MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT));//�����߳�UI����ΪӢ��
	DownloadInfo* threadol=(DownloadInfo*)lparam;
	customMethods::customDownload(threadol->m_downloadtype,threadol->m_selectfileid,
		threadol->m_projectid,threadol->m_downloadpath);
	delete threadol;
	threadol=NULL;
	return 0;
}
void customMethods::createProjectTree(int projectid,CHAPYTreeCtrl* Tree,CBCGPBreadcrumb* breadcrumb)
{
	Tree->DeleteAllItems();
	Tree->DeleteAllColumns();

	Tree->InsertColumn(0, _T("��Ŀ"), 100);
	st_exec_errors error;
	vector<ProjectDir*> proyidir=apiProject::getProjectDirs(projectid,error);
	CString str(proyidir.at(0)->FolderName().c_str());/*��ȡ��Ŀ��*/	

	HTREEITEM m_hRoot= Tree->InsertItem(str,Wfolder,Wfolder);	
	Tree->SetItemData(m_hRoot,proyidir.at(0)->Id());
	//Tree->getitem
	HBREADCRUMBITEM hRoot = breadcrumb->GetRootItem ();
	breadcrumb->DeleteItem(hRoot);
	breadcrumb->SetItemText (hRoot, str);
	breadcrumb->SetItemData(hRoot,proyidir.at(0)->Id());
	customMethods::addDataToProjectTreeAndBreadcrumb(proyidir.at(0),error,m_hRoot,hRoot,Tree,breadcrumb);
	//չ��

	Tree->ExpandAll(FALSE);
	Tree->Expand(m_hRoot,TVE_EXPAND); 
	Tree->AdjustLayout ();
	Tree->RedrawWindow ();
	breadcrumb->RedrawWindow();
	initmap();
	Tree->SelectItem(m_hRoot);

	for (vector<ProjectDir*>::iterator it = proyidir.begin(); it != proyidir.end(); ++it) 
	{ 
		delete *it; 
		*it=NULL;
	}
}
void customMethods::addDataToProjectTreeAndBreadcrumb(ProjectDir* dir, st_exec_errors& error,HTREEITEM treeChild,HBREADCRUMBITEM breadhChild,CHAPYTreeCtrl* Tree,CBCGPBreadcrumb* breadcrumb) 
{
	int id = dir->Id();
	int projId = dir->ProjectId();	
	if (dir->Children().size() > 0)//�������Ŀ¼
	{
		for (size_t i = 0; i < dir->Children().size(); i++)
		{
			ProjectDir* subdir = dir->Children().at(i);
			CString str(subdir->FolderName().c_str());
			treeChild=Tree->InsertItem(str,Wfolder,Wfolder,treeChild,TVI_LAST);
			breadhChild=breadcrumb->InsertItem (breadhChild, str);

			if (treeChild != NULL)
			{
				Tree->SetItemData(treeChild,subdir->Id());
				breadcrumb->SetItemData(breadhChild,subdir->Id());
			}
			addDataToProjectTreeAndBreadcrumb(subdir, error,treeChild,breadhChild,Tree,breadcrumb);
			treeChild = Tree->GetParentItem(treeChild);
			breadhChild=breadcrumb->GetItemParent(breadhChild);
		}
	}
}
void customMethods::addDirsToAttachedFileTree(vector<AttachmentFile*> AttachmentFiles,CBCGPGridRow* pRoot,CHAPYTreeCtrl* m_wndTree) 
{
	if(AttachmentFiles.size()>0)
	{
		vector<AttachmentFile*> Afiles;
		for (size_t i = 0; i < AttachmentFiles.size(); i++)
		{
			CString name(AttachmentFiles.at(i)->AttachFile()->FileName().c_str());
			CString str1,str2;
			ProjectFile* thisfile= (ProjectFile*)AttachmentFiles.at(i)->AttachFile();
			int thisAfileid =thisfile->FileId();
			CBCGPGridRow* pchild= m_wndTree->CreateRow (m_wndTree->GetColumnCount());
			ASSERT_VALID (pchild);
			pchild->AllowSubItems ();
			pRoot->AddSubItem (pchild, FALSE);
			//����
			pchild->GetItem (0)->SetValue ((LPCTSTR)name);
			//����
			CString fileextension (thisfile->Extension().c_str());
			pchild->GetItem (1)->SetValue ((LPCTSTR)fileextension);
			//��С
			CString filelen;
			filelen.Format(_T("%dKB"),thisfile->FileSize()/1024+1);
			pchild->GetItem (2)->SetValue ((LPCTSTR)filelen);
			//����
			CString createname (thisfile->CreateName().c_str());
			pchild->GetItem (3)->SetValue ((LPCTSTR)createname);
			//����ʱ��
			CString createtime (thisfile->CreateTime().c_str());
			str1=createtime.Left(10);
			str2=createtime.Mid(11,5);
			createtime=str1+_T(" ")+str2;
			pchild->GetItem (4)->SetValue ((LPCTSTR)createtime);
			//�汾
			CString version;
			version.Format(_T("%d"),thisfile->FileVersion());
			pchild->GetItem (5)->SetValue ((LPCTSTR)version);
			//���
			CString fileReversionLabel (thisfile->FileReversionLabel().c_str());
			pchild->GetItem (6)->SetValue ((LPCTSTR)fileReversionLabel);
			//��������״̬
			CString statusname (thisfile->StateName().c_str());
			pchild->GetItem (7)->SetValue ((LPCTSTR)statusname);
			//�����
			CString checkoutname (thisfile->CheckoutName().c_str());
			pchild->GetItem (8)->SetValue ((LPCTSTR)checkoutname);
			//���ʱ��
			CString filecheckouttime (thisfile->FileCheckouttime().c_str());
			str1=filecheckouttime.Left(10);
			str2=filecheckouttime.Mid(11,5);
			filecheckouttime=str1+_T(" ")+str2;
			pchild->GetItem (9)->SetValue ((LPCTSTR)filecheckouttime);
			//������
			CString checkinname (thisfile->CheckinName().c_str());
			pchild->GetItem (10)->SetValue ((LPCTSTR)checkinname);
			//����ʱ��
			CString filecheckintime (thisfile->FileCheckintime().c_str());
			str1=filecheckintime.Left(10);
			str2=filecheckintime.Mid(11,5);
			filecheckintime=str1+_T(" ")+str2;
			pchild->GetItem (11)->SetValue ((LPCTSTR)filecheckintime);

			st_exec_errors error;
			Afiles=apiProject::getAttachmentFiles("0",thisAfileid,error);
			addDirsToAttachedFileTree(Afiles,pchild,m_wndTree);
		}
		for (vector<AttachmentFile*>::iterator it = Afiles.begin(); it != Afiles.end(); ++it)
		{ 
			delete *it; 
			*it=NULL;
		}
	}	
}
void customMethods::addDataToReferenceList( int thisid,CHAPYTreeCtrl* tree,CBCGPComboBox* combo,int combonum){
	//���
	//tree->DeleteAllItems();
	//combo->ResetContent();
	if(thisid==0) return;
	
	//���combo
	st_exec_errors error;
	ProjectFile* thisfile=apiProject::getProjectFile(thisid,error);
	int versionmax=thisfile->FileVersion();
	for(int i=1;i<=versionmax;i++){
		CString str(StringUtils::Int2String(i).c_str());
		combo->AddString(str); 
	}	
	combo->SetCurSel(combonum-1);   
	//�õ���ǰ�汾�ļ���fileid
	int thisnum=versionmax-combonum;
	
	vector<ProjectFile*> files=apiProject::getProjectFileHistroy(thisfile->FileMasterId(),error);
	vector<AttachmentFile*> AttachmentFiles=apiProject::getAttachmentFiles("0",files.at(thisnum)->FileId(),error);
	/////////////
	CBCGPGridRow* pRoot = tree->CreateRow (tree->GetColumnCount());
	ASSERT_VALID (pRoot);
	pRoot->AllowSubItems ();
	if(AttachmentFiles.size()>0){
		CString cname(thisfile->FileName().c_str());
		cname+=L"������";
		pRoot->GetItem (0)->SetValue ((LPCTSTR)cname);
	}
	else 
		pRoot->GetItem (0)->SetValue (_T("������"));
	tree->AddRow (pRoot, FALSE);
	addDirsToAttachedFileTree(AttachmentFiles,pRoot,tree);
	//tree->AdjustLayout();
	for (vector<ProjectFile*>::iterator it = files.begin(); it != files.end(); ++it) 
	{ 
		delete *it; 
		*it=NULL;
	}
	for (vector<AttachmentFile*>::iterator it = AttachmentFiles.begin(); it != AttachmentFiles.end(); ++it) 
	{ 
		delete *it; 
		*it=NULL;
	}
	delete thisfile;
	thisfile=NULL;
}
//
void customMethods::addSubDirsToFolderList(int folderId, ProjectDir* Dir,CHAPYListCtrl* List)
{
	st_exec_errors error;
	int projId=Dir->ProjectId();
	ProjectDir* thisdir=apiProject::getChildren(folderId,projId,error);
	if (folderId == Dir->Id()) {
		vector<ProjectDir*> children=Dir->Children();
		if (children.size() > 0){

			for (size_t i = 0; i <children.size(); i++)
			{
				CBCGPGridRow* pchild= List->CreateRow (List->GetColumnCount());
				ASSERT_VALID (pchild);
				pchild->SetData(children.at(i)->Id());	
				//�ļ���,icon
				CString filename (children.at(i)->FolderName().c_str());
				pchild->GetItem (listicon)->SetImage(Wfolder);
				pchild->GetItem (listname)->SetValue ((LPCTSTR)filename);
				//����
				CString createname (children.at(i)->CreateName().c_str());
				pchild->GetItem (listauthor)->SetValue ((LPCTSTR)createname);
				//����ʱ��
				CString createtime (children.at(i)->CreateTime().c_str());
				CString str1=createtime.Left(10);
				CString str2=createtime.Mid(11,5);
				createtime=str1+_T(" ")+str2;
				pchild->GetItem (listcreatetime)->SetValue ((LPCTSTR)createtime);
				List->AddRow (pchild, FALSE);
			}
		}
	}

	if (Dir->Children().size() > 0)
	{
		vector<ProjectDir*> children = Dir->Children();
		for (size_t i = 0; i < children.size(); i++)
		{
			addSubDirsToFolderList(folderId, children.at(i),List);
		}
	}
}
void customMethods::addFilesToFolderList(int thisid,int projectid, CHAPYListCtrl* List)
{
	st_exec_errors error;
	ProjectDir* thisdir=apiProject::getChildren(thisid,projectid,error);
	vector<ProjectDir*> dirs=thisdir->Children();
	vector<ProjectFile*> files=thisdir->Files();
	if (dirs.size() > 0){
		for (size_t i = 0; i <dirs.size(); i++)
		{
			CBCGPGridRow* pchild= List->CreateRow (List->GetColumnCount());
			ASSERT_VALID (pchild);
			pchild->SetData(dirs.at(i)->Id());	
			//�ļ���,icon
			CString filename (dirs.at(i)->FolderName().c_str());
			pchild->GetItem (listicon)->SetImage(Wfolder);
			pchild->GetItem (listname)->SetValue ((LPCTSTR)filename);
			//����
			CString createname (dirs.at(i)->CreateName().c_str());
			pchild->GetItem (listauthor)->SetValue ((LPCTSTR)createname);
			//����ʱ��
			CString createtime (dirs.at(i)->CreateTime().c_str());
			CString str1=createtime.Left(10);
			CString str2=createtime.Mid(11,5);
			createtime=str1+_T(" ")+str2;
			pchild->GetItem (listcreatetime)->SetValue ((LPCTSTR)createtime);
			List->AddRow (pchild, FALSE);
		}
	}
	if (files.size() > 0){
		for (size_t i = 0; i < files.size(); i++)
		{
			CString str1;
			CString str2;
			int stateicon=empty;
			//�½�һ�У�����id
			CBCGPGridRow* pchild= List->CreateRow (List->GetColumnCount ());
			ASSERT_VALID (pchild);
			pchild->SetData(files.at(i)->ProjectFileId());	
			//�ļ������ļ�ͼ��
			CString filename (files.at(i)->FileName().c_str());
			string strname=customMethods::getFileSuffixByFileName(filename);
			strname.assign(strname);
			std::transform(strname.begin(),strname.end(),strname.begin(),tolower); 
			int fileicon=filetype_map[strname];
			pchild->GetItem (listicon)->SetImage(fileicon);
			pchild->GetItem (listname)->SetValue ((LPCTSTR)filename);
			//
			CMainFrame*pMainFrame = DYNAMIC_DOWNCAST(CMainFrame,AfxGetMainWnd());
			CFolderListView*plistview = DYNAMIC_DOWNCAST(CFolderListView,pMainFrame->m_wndSplitter2.GetPane(0,0));
			//����״̬ͼ��
			string strpath;
			strpath=pMainFrame->m_workfolderpath+'\\'+
				StringUtils::CStringToString(plistview->m_Breadcrumb.GetSelectedPath(_T('\\')));
			strpath=strpath+"\\"+StringUtils::CStringToString(filename);
			if(files.at(i)->CheckoutName()!=""||files.at(i)->FileCheckouttime()!=""){
				stateicon= checkoutBySelf;
			}else 
				stateicon=customMethods::fileLocalStatus(strpath,files.at(i)->FileMD5());
			pchild->GetItem (listcheck)->SetImage(stateicon);

			//����
			CString fileextension (files.at(i)->Extension().c_str());
			pchild->GetItem (listtype)->SetValue ((LPCTSTR)fileextension);
			//��С
			//filelen.Format(_T("%dKB"),file.at(i)->FileSize()/1024+1);
			/*float filesize=(float)file.at(i)->FileSize()/1024;
			filelen.Format(_T("%.2f"),filesize);*/
			CString filelen=customMethods::fileSizeUnit(files.at(i)->FileSize());
			pchild->GetItem (listsize)->SetValue ((LPCTSTR)filelen);
			//pchild->GetItem (listsize)->SetValue ((LPCTSTR)file.at(i)->FileSize());
			//����
			CString createname (files.at(i)->CreateName().c_str());
			pchild->GetItem (listauthor)->SetValue ((LPCTSTR)createname);
			//����ʱ��
			CString createtime (files.at(i)->CreateTime().c_str());
			str1=createtime.Left(10);
			str2=createtime.Mid(11,5);
			createtime=str1+_T(" ")+str2;
			pchild->GetItem (listcreatetime)->SetValue ((LPCTSTR)createtime);
			//�汾
			CString version;
			version.Format(L"%d",files.at(i)->FileVersion());
			pchild->GetItem (listversion)->SetValue ((LPCTSTR)version);
			//���
			CString fileReversionLabel (files.at(i)->FileReversionLabel().c_str());
			pchild->GetItem (listrevision)->SetValue ((LPCTSTR)fileReversionLabel);
			//��������״̬
			CString statusname (files.at(i)->StateName().c_str());
			pchild->GetItem (listlife)->SetValue ((LPCTSTR)statusname);
			//�����
			CString checkoutname (files.at(i)->CheckoutName().c_str());
			pchild->GetItem (listout)->SetValue ((LPCTSTR)checkoutname);
			//���ʱ��
			CString filecheckouttime (files.at(i)->FileCheckouttime().c_str());
			str1=filecheckouttime.Left(10);
			str2=filecheckouttime.Mid(11,5);
			filecheckouttime=str1+_T(" ")+str2;
			pchild->GetItem (listouttime)->SetValue ((LPCTSTR)filecheckouttime);
			//������
			CString checkinname (files.at(i)->CheckinName().c_str());
			pchild->GetItem (listin)->SetValue ((LPCTSTR)checkinname);
			//����ʱ��
			CString filecheckintime (files.at(i)->FileCheckintime().c_str());
			str1=filecheckintime.Left(10);
			str2=filecheckintime.Mid(11,5);
			filecheckintime=str1+_T(" ")+str2;
			pchild->GetItem (listintime)->SetValue ((LPCTSTR)filecheckintime);
			//��ע
			pchild->GetItem (listnote)->SetValue (_T(""));
			List->AddRow (pchild, FALSE);

		}
	}
	delete thisdir;
	thisdir=NULL;
	for (vector<ProjectDir*>::iterator it = dirs.begin(); it != dirs.end(); ++it) 
	{ 
		delete *it; 
		*it=NULL;
	}
	for (vector<ProjectFile*>::iterator it = files.begin(); it != files.end(); ++it) 
	{ 
		delete *it; 
		*it=NULL;
	}
}
void customMethods::addFilesToAttachedFileList(int thisid,int projectid, CListCtrl& List,std::string thisname)
{
	st_exec_errors error;
	vector<ProjectFile*> file=apiProject::getFilesByFolder(thisid,projectid,error);
	if (file.size() > 0)
	{
		for (size_t i = 0; i < file.size(); i++)
		{
			if(file.at(i)->CheckoutName()!=""||file.at(i)->FileName()==thisname)continue;

			CString str1;
			CString str2;
			int Index = List.InsertItem(0xffff, _T(""));
			List.SetItemData(Index,file.at(i)->ProjectFileId());	
			//�ļ���
			CString filename (file.at(i)->FileName().c_str());
			List.SetItemText(Index, 0, filename);
			//��������״̬
			CString statusname (file.at(i)->StateName().c_str());
			List.SetItemText(Index, 1,statusname);
			//�汾
			CString version;
			version.Format(L"%d",file.at(i)->FileVersion());
			List.SetItemText(Index, 2,version);
			//���
			CString fileReversionLabel (file.at(i)->FileReversionLabel().c_str());
			List.SetItemText(Index, 3,fileReversionLabel);
		}
	}
	for (vector<ProjectFile*>::iterator it = file.begin(); it != file.end(); ++it) 
	{ 
		delete *it; 
		*it=NULL;
	}
}
void customMethods::addDataToVersionList(int thisid,CHAPYListCtrl* List)
{	
	//List->RemoveAll();
	if(thisid==0) return;
	
	st_exec_errors error;
	FileDownloader* download=new FileDownloader();
	string downloadpath=theApp.m_thumbnailpath;

	/*C:\Users\gan\AppData\Local\Temp\
	wchar_t OutPath[MAX_PATH];
	int Strlen = GetTempPath(MAX_PATH,(LPWSTR)OutPath);
	customMethods::customMessagebox(L"��ʾ",OutPath,MB_OK|MB_ICONEXCLAMATION);*/
	ProjectFile* thisfile=apiProject::getProjectFile(thisid,error);
	vector<ProjectFile*> file=apiProject::getProjectFileHistroy(thisfile->FileMasterId(),error); 
	vector<AttachmentFile*> AttachmentFiles;
	CMainFrame*pMainFrame = DYNAMIC_DOWNCAST(CMainFrame,AfxGetMainWnd());
	CFileDetailsLayoutView* pdetailsView = (CFileDetailsLayoutView*)pMainFrame->m_wndSplitter2.GetPane(1,0);
	CVersionListDlg*pversionDlg =&pdetailsView->m_pversionDlg;
	if (file.size() > 0)
	{
		
		pversionDlg->m_versionlistImageList.DeleteImageList();
		pversionDlg->m_versionlistImageList.Create(80, 80, ILC_COLOR32|ILC_MASK,1,1);
		for (int i = 0; i < file.size(); i++)	
		{
			//if(!List->exitflag) break;
			AttachmentFiles=apiProject::getAttachmentFiles("4",file.at(i)->FileId(),error);
			if(AttachmentFiles.size()>0){
				string versionnum=StringUtils::Int2String(i);

				versionnum=downloadpath+"\\"+versionnum+AttachmentFiles.at(0)->AttachFile()->FileName();
				CString cdownloadpath(versionnum.c_str());
				bool isupload=TRUE;
				//
				if(5!=customMethods::fileLocalStatus(versionnum,AttachmentFiles.at(0)->AttachFile()->FileMD5()))
				{
					if(!download->Download(AttachmentFiles.at(0)->AttachFile()->FileId(),AttachmentFiles.at(0)->AttachmentId(),versionnum,error)){
						pversionDlg->m_versionlistImageList.Add(theApp.LoadIcon(IDI_empty));
						isupload=FALSE;
					}
				}
				if(isupload){
					CBitmap bitmap;
					CImage image,imDest;
					image.Load(cdownloadpath);
					int imageW=image.GetWidth();
					int imageH=image.GetHeight();
					//int imDestW=imageW*80/imageH;
					int imDestW=80;
					int imDestH=80;
					imDest.Create(80, 80, ILC_COLOR32);
					if (image.IsDIBSection()){

						HDC destDC = imDest.GetDC();
						//SetStretchBltMode(destDC, HALFTONE);
						image.StretchBlt(destDC, 0, 0, imDestW, imDestH,SRCCOPY);
						imDest.ReleaseDC();
					}
					HBITMAP hBitmap = (HBITMAP)imDest;
					bitmap.Attach(hBitmap);
					pversionDlg->m_versionlistImageList.Add(&bitmap, RGB(0xff, 0xff, 0xff));	
					bitmap.DeleteObject();
				}
			}				
		}
		CBCGPGridRow* pchild=NULL;
		for (int i = 0; i < file.size(); i++)
		{
			//if(!List->exitflag) break;
			CString str1;
			CString str2;
			pchild= List->CreateRow (List->GetColumnCount ());
			ASSERT_VALID (pchild);
			//����ͼ
			pchild->GetItem (listicon)->SetImage (i);
			//�ļ���
			CString filename (file.at(i)->FileName().c_str());
			pchild->GetItem (listname)->SetValue ((LPCTSTR)filename);
			//����
			CString fileextension (file.at(i)->Extension().c_str());
			pchild->GetItem (listtype)->SetValue ((LPCTSTR)fileextension);
			//��С
			/*CString filelen;
			filelen.Format(_T("%dKB"),file.at(i)->FileSize()/1024+1);
			pchild->GetItem (listsize)->SetValue ((LPCTSTR)filelen);*/
			CString filelen=customMethods::fileSizeUnit(file.at(i)->FileSize());
			pchild->GetItem (listsize)->SetValue ((LPCTSTR)filelen);
			//����
			CString createname (file.at(i)->CreateName().c_str());
			pchild->GetItem (listauthor)->SetValue ((LPCTSTR)createname);
			//����ʱ��
			CString createtime (file.at(i)->CreateTime().c_str());
			str1=createtime.Left(10);
			str2=createtime.Mid(11,5);
			createtime=str1+_T(" ")+str2;
			pchild->GetItem (listcreatetime)->SetValue ((LPCTSTR)createtime);
			//�汾
			CString version;
			version.Format(L"%d",file.at(i)->FileVersion());
			pchild->GetItem (listversion)->SetValue ((LPCTSTR)version);
			//���
			CString fileReversionLabel (file.at(i)->FileReversionLabel().c_str());
			pchild->GetItem (listrevision)->SetValue ((LPCTSTR)fileReversionLabel);
			//��������״̬
			CString statusname (file.at(i)->StateName().c_str());
			pchild->GetItem (listlife)->SetValue ((LPCTSTR)statusname);
			//�����
			CString checkoutname (file.at(i)->CheckoutName().c_str());
			pchild->GetItem (listout)->SetValue ((LPCTSTR)checkoutname);
			//���ʱ��
			CString filecheckouttime (file.at(i)->FileCheckouttime().c_str());
			str1=filecheckouttime.Left(10);
			str2=filecheckouttime.Mid(11,5);
			filecheckouttime=str1+_T(" ")+str2;
			pchild->GetItem (listouttime)->SetValue ((LPCTSTR)filecheckouttime);
			//������
			CString checkinname (file.at(i)->CheckinName().c_str());
			pchild->GetItem (listin)->SetValue ((LPCTSTR)checkinname);
			//����ʱ��
			CString filecheckintime (file.at(i)->FileCheckintime().c_str());
			str1=filecheckintime.Left(10);
			str2=filecheckintime.Mid(11,5);
			filecheckintime=str1+_T(" ")+str2;
			pchild->GetItem (listintime)->SetValue ((LPCTSTR)filecheckintime);
			//��ע
			pchild->GetItem (listnote)->SetValue (_T(""));
			List->AddRow (pchild, FALSE);
		}
		//List->AdjustLayout();
		//List->RedrawWindow ();
		pchild=NULL;
	}
	delete download;
	download=NULL;
	delete thisfile;
	thisfile=NULL;
	for (vector<ProjectFile*>::iterator it = file.begin(); it != file.end(); ++it) 
	{ 
		delete *it; 
		*it=NULL;
	}
	for (vector<AttachmentFile*>::iterator it = AttachmentFiles.begin(); it != AttachmentFiles.end(); ++it) 
	{ 
		delete *it; 
		*it=NULL;
	}
}
void customMethods::addDataToFilePropertiesList(CBCGPPropList* m_property,CHAPYListCtrl* List){
	m_property->ResetOriginalValues	(TRUE);
	CBCGPProp* pPropone=m_property->GetProperty(0);
	CBCGPProp* pProp=NULL;

	CBCGPGridRow* pchild= List->GetCurSel();
	CString cstr;
	for(int i=listname;i<=listintime;i++){
		cstr=(CString)pchild->GetItem (i)->GetValue();
		pProp=pPropone->GetSubItem(i-2);
		pProp->SetValue((_variant_t)(LPCTSTR)cstr);	
	}

	////�ļ���
	//cstr=(CString)pchild->GetItem (listname)->GetValue();
	//pProp=pPropone->GetSubItem(listname-2);
	//pProp->SetValue((_variant_t)(LPCTSTR)cstr);
	////����
	//cstr=(CString)pchild->GetItem (listtype)->GetValue();
	//pProp=pPropone->GetSubItem(listtype-2);
	//pProp->SetValue((_variant_t)(LPCTSTR)cstr);
	////��С
	//cstr=(CString)pchild->GetItem (listsize )->GetValue();
	//pProp=pPropone->GetSubItem(listsize-2);
	//pProp->SetValue((_variant_t)(LPCTSTR)cstr);
	////����
	//cstr=(CString)pchild->GetItem ( listauthor)->GetValue();
	//pProp=pPropone->GetSubItem(listauthor-2);
	//pProp->SetValue((_variant_t)(LPCTSTR)cstr);
	////����ʱ��
	//cstr=(CString)pchild->GetItem ( listcreatetime )->GetValue();
	//pProp=pPropone->GetSubItem(listcreatetime-2);
	//pProp->SetValue((_variant_t)(LPCTSTR)cstr);
	////�汾
	//cstr=(CString)pchild->GetItem ( listversion )->GetValue();
	//pProp=pPropone->GetSubItem(listversion-2);
	//pProp->SetValue((_variant_t)(LPCTSTR)cstr);
	////���
	//cstr=(CString)pchild->GetItem ( listedition )->GetValue();
	//pProp=pPropone->GetSubItem(listedition-2);
	//pProp->SetValue((_variant_t)(LPCTSTR)cstr);
	////��������״̬
	//cstr=(CString)pchild->GetItem ( listlife )->GetValue();
	//pProp=pPropone->GetSubItem(listlife-2);
	//pProp->SetValue((_variant_t)(LPCTSTR)cstr);
	////�����
	//cstr=(CString)pchild->GetItem ( listout)->GetValue();
	//pProp=pPropone->GetSubItem(listout-2);
	//pProp->SetValue((_variant_t)(LPCTSTR)cstr);
	////���ʱ��
	//cstr=(CString)pchild->GetItem ( listouttime )->GetValue();
	//pProp=pPropone->GetSubItem(listouttime-2);
	//pProp->SetValue((_variant_t)(LPCTSTR)cstr);
	////������
	//cstr=(CString)pchild->GetItem ( listin )->GetValue();
	//pProp=pPropone->GetSubItem(listin-2);
	//pProp->SetValue((_variant_t)(LPCTSTR)cstr);
	////����ʱ��
	//cstr=(CString)pchild->GetItem ( listintime)->GetValue();
	//pProp=pPropone->GetSubItem(listintime-2);
	//pProp->SetValue((_variant_t)(LPCTSTR)cstr);


}
void customMethods::updateFolderList(int thisid,int projectid,CHAPYListCtrl* List){
	//List����
	st_exec_errors error;
	//��������
	List->SetRedraw(FALSE);
	List->RemoveAll();
	customMethods::addFilesToFolderList( thisid,projectid,List);
	List->SetRedraw(TRUE);
	List->AdjustLayout();
	List->RedrawWindow ();
}
//
void customMethods::updateFileDetailsByTabNum(int tabnum,int thisid){
	int combonum=0;
	if(thisid!=0){
		st_exec_errors error;
		ProjectFile* thisfile=apiProject::getProjectFile(thisid,error);
		combonum=thisfile->FileVersion();
		delete thisfile;
		thisfile=NULL;
		
	}
	CMainFrame*pMainFrame = DYNAMIC_DOWNCAST(CMainFrame,AfxGetMainWnd());
	CFileDetailsLayoutView* pdetailsView = (CFileDetailsLayoutView*)pMainFrame->m_wndSplitter2.GetPane(1,0);
	CVersionListDlg*	pversionDlg =&pdetailsView->m_pversionDlg;
	CReferenceListDlg*	preferenceDlg =&pdetailsView->m_preferenceDlg;
	switch(tabnum){
	case 0:	
		
		customMethods::addDataToVersionList(thisid,&pversionDlg->m_listGrid);
		break;
	case 1:	
		customMethods::addDataToReferenceList(thisid,&preferenceDlg->m_treeGrid,&preferenceDlg->m_Combo,combonum);
		break;
	case 2:
		//CByReferenceListDlg*pbyrelerenceDlg =pdetailsView->m_byreferencepage;
		break;
	case 3:
		//CRevisionListDlg*previsionDlg =pdetailsView->m_revisionpage;
		break;
	case 4:
		//CReadCircleAnnotateDlg* preadandcircleDlg =pdetailsView->m_readcircleAnnotatepage;
		break;
	}

}
std::string customMethods::openDirSelectDlg()
{
	LPITEMIDLIST rootLoation;
	SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &rootLoation);
	//���öԻ���
	BROWSEINFO bInfo;
	ZeroMemory(&bInfo, sizeof(bInfo));
	bInfo.lpszTitle = _T("��ѡ��·��: ");
	bInfo.ulFlags = BIF_RETURNONLYFSDIRS;

	bInfo.pidlRoot = rootLoation; // �ļ��жԻ���֮��Ŀ¼����ָ���Ļ���Ϊ�ҵĵ���

	//��ʾ�Ի���
	LPITEMIDLIST lpDlist; //�������淵����Ϣ��IDList
	lpDlist = SHBrowseForFolder(&bInfo); //��ʾѡ��Ի���
	CString strPath;
	if (lpDlist != NULL)  //�û�����ȷ����ť
	{
		TCHAR chPath[255]; //�����洢·�����ַ���
		SHGetPathFromIDList(lpDlist, chPath);//����Ŀ��ʶ�б�ת�����ַ���
		strPath = chPath; //��TCHAR���͵��ַ���ת��ΪCString���͵��ַ���
	}
	string str="";
	str=StringUtils::CStringToString(strPath);
	return str;
}
CString customMethods::getFileNameByPath(CString strPath){
	int nPos= strPath.ReverseFind('\\'); 
	CString thisfilename = strPath.Right(strPath.GetLength() - nPos - 1 ); 
	return thisfilename;
}
string customMethods::getFileSuffixByFileName(CString name){
	int nPos= name.ReverseFind('.'); 
	CString thisfilesuffix = name.Right(name.GetLength() - nPos ); 	
	return StringUtils::CStringToString(thisfilesuffix);
}
bool customMethods::isFileById(int thisid,string thisname)
{
	st_exec_errors error;
	bool is=FALSE;
	ProjectFile* thisfile=apiProject::getProjectFile(thisid,error);	

	//������ļ���
	if(thisfile!=NULL)
		if(thisfile->FileName()==thisname)  
			is= TRUE;

	delete thisfile;
	thisfile=NULL;
	return is;
}

char* customMethods::UTF8ToUnicode(const char* strSrc)
{
	//UTF8 to Unicode
	//Ԥת�����õ�����ռ�Ĵ�С
	int wcsLen = ::MultiByteToWideChar(CP_UTF8, NULL, strSrc, strlen(strSrc), NULL, 0);
	//����ռ�Ҫ��'\0'�����ռ䣬MultiByteToWideChar�����'\0'�ռ�
	wchar_t* wszString = new wchar_t[wcsLen + 1];
	//ת��
	::MultiByteToWideChar(CP_UTF8, NULL, strSrc, strlen(strSrc), wszString, wcsLen);
	//������'\0'
	wszString[wcsLen] = '\0';

	char *m_char;
	int len = WideCharToMultiByte(CP_ACP, 0, wszString, wcslen(wszString), NULL, 0, NULL, NULL);
	m_char = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, wszString, wcslen(wszString), m_char, len, NULL, NULL);
	m_char[len] = '\0';
	return m_char;
}
int customMethods::customMessagebox(CString message,DWORD dwStyle)
{
	BCGP_MSGBOXPARAMS params;
	CString cstr1,cstr2;
	//CString getmessage;
	wchar_t getmessage[MAX_PATH];
	cstr1.LoadString(IDS_TIPS);
	params.lpszCaption = cstr1;
	params.lpszHeader = L"";
	
	GetPrivateProfileString(theApp.m_languagecstr, message, L"",
				getmessage,MAX_PATH,theApp.m_configpath);
	params.lpszText = getmessage;
	params.dwStyle |= dwStyle;
	int i=BCGPMessageBoxIndirect (&params);
	return i;
}
int customMethods::customMessagebox(string name,CString message,DWORD dwStyle)
{
	BCGP_MSGBOXPARAMS params;
	CString cstr1,cstr2;
	CString  getmessage;
	cstr1.LoadString(IDS_TIPS);
	params.lpszCaption = cstr1;
	params.lpszHeader = L"";
	if(message!=""){
	GetPrivateProfileString(theApp.m_languagecstr, message, L"",
			getmessage.GetBuffer(MAX_PATH),MAX_PATH,theApp.m_configpath);
	}
	string total=name+"	"+StringUtils::CStringToString(getmessage);
	CString lpsztext(total.c_str());
	params.lpszText =lpsztext;
	params.dwStyle |= dwStyle;
	int i=BCGPMessageBoxIndirect (&params);
	return i;
}
int customMethods::customMessagebox(const vector<string>& name,const vector<CString>& message,DWORD dwStyle){

	BCGP_MSGBOXPARAMS params;
	CString tipstr;
	string total="";
	CString getmessage;
	string errorstr;
	tipstr.LoadString(IDS_TIPS);
	params.lpszCaption = tipstr;
	params.lpszHeader = L"";

	for(int i=0;i<name.size();i++){
		if(message.at(i)!=L""){
			GetPrivateProfileString(theApp.m_languagecstr, message.at(i), L"",
				getmessage.GetBuffer(MAX_PATH),MAX_PATH,theApp.m_configpath);
			errorstr=StringUtils::CStringToString(getmessage);
		}else{
			errorstr="";
		}
		total=total+name.at(i)+"     "+errorstr+"\n";
	}
	CString lpsztext(total.c_str());
	params.lpszText =lpsztext;
	params.dwStyle |= dwStyle;
	int i=BCGPMessageBoxIndirect (&params);
	return i;
}
int customMethods::customMessagebox(CString caption,CString message,DWORD dwStyle)
{
	BCGP_MSGBOXPARAMS params;
	CString cstr;
	cstr.LoadString(IDS_TIPS);
	params.lpszCaption = cstr;
	params.lpszHeader = L"";
	params.lpszText = message;
	params.dwStyle |= dwStyle;
	int i=BCGPMessageBoxIndirect (&params);
	return i;
}

CString customMethods::fileSizeUnit(int filesize){
	int GB = 1024 * 1024 * 1024;
	int MB = 1024 * 1024;
	int KB = 1024;
	CString strfilesize;
	if (filesize / GB >= 1){
		strfilesize.Format(_T("%.2fGB"),filesize / (float) GB);
	}else if (filesize / MB >= 1){
		strfilesize.Format(_T("%.2fMB"),filesize / (float) MB);
	}else if (filesize / KB >= 1){
		strfilesize.Format(_T("%.2fKB"),filesize / (float) KB);
	} else {
		strfilesize.Format(_T("%dB"),filesize );
	}
	return strfilesize;
}
int customMethods::fileLocalStatus(const std::string& strpath,const std::string& fileMD5){
	/*�ж�strpath·���£��Ƿ��д��ļ�
	3:û�д��ļ�
	5:����ͬ�ļ�
	6:��ͬ����ͬ�ļ�
	*/
	int stateicon=empty;
	std::wstring wfilepath = StringUtils::String2WString(strpath);
	FileSlice *fileslice=new FileSlice();
	if(fileslice->open(wfilepath,eRead)){
		try
		{
			if (fileslice->Slice()){
				if(fileslice->FileMd5()==fileMD5){
					stateicon=filecheckin;
				}else  stateicon=localfilenotmatch;
			}else 
				customMethods::customMessagebox(L"��ʾ",L"�ļ��޷���Ƭ",MB_OK|MB_ICONASTERISK);
		}
		catch (CException* e)
		{

		}
		finally:{
			fileslice->close();
		};
	}
	delete fileslice;
	fileslice=NULL;
	return stateicon;
}
bool customMethods::createLocalFolder(const std::string& folderPath){
	CString strPath(folderPath.c_str());
	CString strSubPath;
	CString strMsg;
	int nCount = 0; 
	int nIndex = 0;

	//ͨ����\�����ָ�·�����Ӷ�����������Ŀ¼��
	do
	{
		nIndex = strPath.Find(_T("\\"),nIndex) + 1;
		nCount++;
	}while( (nIndex-1) != -1);
	nIndex = 0;
	//��飬������Ŀ¼
	while( (nCount-1) >= 0)
	{
		nIndex = strPath.Find(_T("\\"),nIndex) + 1;
		if( (nIndex - 1) == -1)
			strSubPath = strPath;
		else
			strSubPath = strPath.Left(nIndex);

		if(!PathFileExists(strSubPath))// - ���Ŀ¼�Ƿ����
		{
			if(!CreateDirectory(strSubPath,NULL))// -�������򴴽�Ŀ¼
			{
				strMsg.Format(_T("����Ŀ¼��%s��ʧ�ܣ�"),strSubPath);
				AfxMessageBox(strMsg,MB_OK);
				return FALSE;
			}
		}
		nCount--;
	}
	return TRUE;
}
void customMethods::customDownload(bool type,int thisid,int proJid,string downloadpath){
	if(type){
		//�ļ�
		st_exec_errors error;
		ProjectFile* thisfile=apiProject::getProjectFile(thisid,error);
		//�ļ�����
		if(5!=customMethods::fileLocalStatus(downloadpath,thisfile->FileMD5())){
			FileDownloader* download=new FileDownloader();
			if(download->Download(thisfile->FileId(),thisid,downloadpath,error)){
				customMethods::customMessagebox(L"��ʾ",_T("���سɹ�!"),MB_OK|MB_ICONASTERISK);
			}			
			else{
				customMethods::customMessagebox(L"��ʾ",_T("����ʧ��!"),MB_OK|MB_ICONERROR);
			}
			delete download;
			download=NULL;
		}else {
			customMethods::customMessagebox(L"��ʾ",_T("���سɹ�!"),MB_OK|MB_ICONASTERISK);
		}
		delete thisfile;
		thisfile=NULL;
	}else{
		//�ļ���
		vector<string>thisDirpath;
		thisDirpath.push_back(downloadpath);
		customMethods::recursiveDownloadDir(thisid,proJid,thisDirpath);	
		//���ؽ��
		if (thisDirpath.size()>1)
		{
			downloadpath="����ʧ��!\n";
			for(int i=1;i<thisDirpath.size();i++){
				downloadpath=downloadpath+thisDirpath.at(i)+"\n";
			}
			CString msg(downloadpath.c_str());
			customMethods::customMessagebox(L"��ʾ",msg,MB_OK|MB_ICONERROR);
		}else{
			CString str(downloadpath.c_str());
			str=customMethods::getFileNameByPath(str);
			customMethods::customMessagebox(L"��ʾ",str+L" ȫ�����سɹ���",MB_OK|MB_ICONASTERISK);
		}
	}
}
void customMethods::recursiveDownloadDir(int thisid,int projId,vector<string>&thisDirpath){
	st_exec_errors error;
	ProjectDir* thisdir=apiProject::getChildren(thisid,projId,error);
	//��������·��
	createLocalFolder(thisDirpath.at(0));
	if(thisdir->Files().size()>0){ 
		vector<ProjectFile*> childfiles=thisdir->Files();
		FileDownloader* download=new FileDownloader();
		for(int i=0;i<childfiles.size();i++){
			//��ȡ����·��
			string thisfilepath=thisDirpath.at(0)+"\\"+childfiles.at(i)->FileName();
			//�жϱ������޸��ļ���û�о�����
			if(5!=customMethods::fileLocalStatus(thisfilepath,childfiles.at(i)->FileMD5())){
				if(!download->Download(childfiles.at(i)->FileId(),childfiles.at(i)->ProjectFileId(),thisfilepath,error)){
					thisDirpath.push_back(thisfilepath);
				}
			}		
		}	
		delete download;
		download=NULL;
	}
	if(thisdir->Children().size()>0){ 
		vector<ProjectDir*> childdirs=thisdir->Children();
		string thisfolderpath=thisDirpath.at(0);
		for(int i=0;i<childdirs.size();i++){

			thisDirpath.at(0)=thisfolderpath+"\\"+childdirs.at(i)->FolderName();
			recursiveDownloadDir(childdirs.at(i)->Id(), projId, thisDirpath);
		}
	}	
}
void  customMethods::timeFormatter(CString &time) {
	time.Replace('T',' ');
	if(time.Mid(11, 1)=='0'){
		time.Delete(11,1);
	}
}




