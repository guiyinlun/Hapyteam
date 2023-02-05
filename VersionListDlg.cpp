// VersionsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "hapyteamclient.h"
#include "VersionListDlg.h"
#include "afxdialogex.h"
#include"CustomMethods.h"
#include"FileDetailsLayoutView.h"


// CVersionsDlg 对话框
IMPLEMENT_DYNAMIC(CVersionListDlg, CBCGPDialog)

	CVersionListDlg::CVersionListDlg(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CVersionListDlg::IDD, pParent)
{
	EnableVisualManagerStyle(TRUE, TRUE);
	EnableLayout();
}

CVersionListDlg::~CVersionListDlg()
{
}

void CVersionListDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CVersionListDlg, CBCGPDialog)
	ON_WM_DESTROY()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_MENU_Download, &CVersionListDlg::OnMenuDownload)
END_MESSAGE_MAP()


// CVersionsDlg 消息处理程序


BOOL CVersionListDlg::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	// 位置大小
	UINT nTreeStyles = WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER |
		TVS_SHOWSELALWAYS|
		TVS_FULLROWSELECT; //整行选中

	m_listGrid.CreateOnPlaceHolder(this, IDC_versionsLIST, 0, nTreeStyles);
	CRect thisrect,viewrect;
	this->GetClientRect(&viewrect);
	thisrect.SetRect(-2,-2,viewrect.right+2,viewrect.bottom+2);
	m_listGrid.MoveWindow(&thisrect);
	CBCGPStaticLayout* pLayout = (CBCGPStaticLayout*)GetLayout();
	if (pLayout != NULL)
	{
		pLayout->AddAnchor(m_listGrid.GetSafeHwnd(), CBCGPStaticLayout::e_MoveTypeNone, CBCGPStaticLayout::e_SizeTypeBoth);
	}

	//设置
	m_listGrid.EnableHeader (TRUE, BCGP_GRID_HEADER_SELECT);
	m_listGrid.EnableColumnAutoSize(TRUE); //设置列自适应宽度
	m_listGrid.EnableGridLines(TRUE);//显示gridline
	m_listGrid.EnableAlternateRows(FALSE);//启用交替行
	m_listGrid.EnableRowHeader(TRUE);
	m_listGrid.SetReadOnly ();
	m_listGrid.SetWholeRowSel ();//整行
	m_listGrid.SetSingleSel(TRUE);
	m_listGrid.SetCustomRowHeight(80,80);//设置行宽

	m_listGrid.SetVisualManagerColorTheme(TRUE);
	m_listGrid.SetFont(&theApp.m_font);
	m_listGrid.SetImageList(&m_versionlistImageList);

	m_listGrid.RemoveAll();
	m_listGrid.DeleteAllColumns();
	int onewidth=100;
	CString cstr;
	m_listGrid.InsertColumn(listcheck, _T(""), 18);
	cstr.LoadString(IDS_Thumbnail);
	m_listGrid.InsertColumn(listicon,cstr , 90);
	for(int i=103;i<116;i++){
		cstr.LoadString(i);
		m_listGrid.InsertColumn(i-101,cstr,onewidth);
	}
	/*m_listGrid.InsertColumn(listcheck, _T(""), 18);
	m_listGrid.InsertColumn(listicon, _T("缩略图"), onewidth);
	m_listGrid.InsertColumn(listname, _T("文件名称"), onewidth);
	m_listGrid.InsertColumn(listtype, _T("文件类型"), onewidth);
	m_listGrid.InsertColumn(listsize, _T("文件大小"), onewidth);
	m_listGrid.InsertColumn(listauthor , _T("作者"), onewidth);
	m_listGrid.InsertColumn(listcreatetime, _T("创建时间"), onewidth);
	m_listGrid.InsertColumn(listversion , _T("版本"), onewidth);	
	m_listGrid.InsertColumn(listrevision, _T("版次"), onewidth);
	m_listGrid.InsertColumn(listlife , _T("生命周期状态"), onewidth);
	m_listGrid.InsertColumn(listout , _T("检出人"), onewidth);
	m_listGrid.InsertColumn(listouttime , _T("检出时间"), onewidth);
	m_listGrid.InsertColumn(listin, _T("检入人"), onewidth);
	m_listGrid.InsertColumn(listintime , _T("检入时间"), onewidth);
	m_listGrid.InsertColumn(listnote , _T("备注"), onewidth);*/

	m_listGrid.SetColumnVisible(listcheck,FALSE);
	m_listGrid.SetColumnLocked(listicon,TRUE);
	//m_list.SetColumnLocked(listicon);
	//m_list.LoadState (_T("versionsLIST"));
	m_listGrid.AdjustLayout();
	m_listGrid.RedrawWindow ();
	return TRUE;  
	// 异常: OCX 属性页应返回 FALSE
}

void CVersionListDlg::OnDestroy()
{
	//m_list.SaveState (_T("versionsLIST"));
	CBCGPDialog::OnDestroy();
	// TODO: 在此处添加消息处理程序代码
}
void CVersionListDlg::ResetList(){
	this->m_listGrid.RemoveAll();
}
void CVersionListDlg::LoadData(HapyProjectFile* file)
{
	this->m_listGrid.SetRedraw(FALSE);
	vector<HapyProjectFile*>* historyFiles=file->FileHistory();
	if (historyFiles->size() > 0)
	{	
		this->m_versionlistImageList.DeleteImageList();
		this->m_versionlistImageList.Create(80, 80, ILC_COLOR32|ILC_MASK,1,1);
		for (int i = 0; i < historyFiles->size(); i++){
			HapyProjectFile* historyfile= historyFiles->at(i);
			//缩略图
			historyfile->ThumbnailFilePath(historyfile->GetThumbnailFromServer());
			CString thumbnailpath=StringUtils::StringToCString(historyfile->ThumbnailFilePath().c_str());
			if(thumbnailpath==""){
				this->m_versionlistImageList.Add(theApp.LoadIcon(IDI_empty));
				continue;
			}
			CBitmap bitmap;
			CImage image,imDest;
			image.Load(thumbnailpath);
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
			this->m_versionlistImageList.Add(&bitmap, RGB(0xff, 0xff, 0xff));	
			bitmap.DeleteObject();
		}
		for (int i = 0; i < historyFiles->size(); i++){
			HapyProjectFile* historyfile= historyFiles->at(i);
			CBCGPGridRow* pchild=this->m_listGrid.CreateRow ( this->m_listGrid.GetColumnCount ());
			ASSERT_VALID (pchild);
			pchild->SetData((DWORD_PTR)historyfile);	
			//缩略图
			pchild->GetItem (listicon)->SetImage (i);
			//文件名
			CString filename (historyfile->Name().c_str());		
			pchild->GetItem (listname)->SetValue ((LPCTSTR)filename);
			//类型
			CString fileextension (historyfile->Extension().c_str());
			pchild->GetItem (listtype)->SetValue ((LPCTSTR)fileextension);
			//大小
			CString filelen=customMethods::fileSizeUnit(historyfile->FileSize());
			pchild->GetItem (listsize)->SetValue ((LPCTSTR)filelen);			
			//作者
			CString createname (historyfile->CreateName().c_str());
			pchild->GetItem (listauthor)->SetValue ((LPCTSTR)createname);
			//创建时间
			CString createtime (historyfile->CreateTime().c_str());
			customMethods::timeFormatter(createtime);
			pchild->GetItem (listcreatetime)->SetValue ((LPCTSTR)createtime);
			//版本
			CString version;
			version.Format(L"%d",historyfile->FileVersion());
			pchild->GetItem (listversion)->SetValue ((LPCTSTR)version);
			//版次
			CString fileReversionLabel (historyfile->FileReversionLabel().c_str());
			pchild->GetItem (listrevision)->SetValue ((LPCTSTR)fileReversionLabel);
			//生命周期状态
			CString statusname (historyfile->StateName().c_str());
			pchild->GetItem (listlife)->SetValue ((LPCTSTR)statusname);
			//检出人
			CString checkoutname (historyfile->CheckoutName().c_str());
			pchild->GetItem (listout)->SetValue ((LPCTSTR)checkoutname);
			//检出时间
			CString filecheckouttime (historyfile->FileCheckouttime().c_str());
			customMethods::timeFormatter(filecheckouttime);
			pchild->GetItem (listouttime)->SetValue ((LPCTSTR)filecheckouttime);
			//检入人
			CString checkinname (historyfile->CheckinName().c_str());
			pchild->GetItem (listin)->SetValue ((LPCTSTR)checkinname);
			//检入时间
			CString filecheckintime (historyfile->FileCheckintime().c_str());
			customMethods::timeFormatter(filecheckintime);
			pchild->GetItem (listintime)->SetValue ((LPCTSTR)filecheckintime);
			//备注
			pchild->GetItem (listnote)->SetValue (_T(""));
			this->m_listGrid.AddRow (pchild, FALSE);
		}
		this->m_listGrid.SetRedraw(TRUE);
		this->m_listGrid.AdjustLayout();
	}
}
//右键
void CVersionListDlg::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CMenu menu;
	menu.LoadMenuW(IDR_FolderMenu);
	CMenu *contextMenu = menu.GetSubMenu(0);
	ASSERT(contextMenu != NULL);
	//首先得到点击的位置
	CBCGPGridRow* pchild= this->m_listGrid.GetCurSel();
	if(pchild==NULL) return;
	contextMenu->RemoveMenu(ID_Menu_CheckIn, MF_BYCOMMAND);//检入
	contextMenu->RemoveMenu(ID_Menu_CheckOut, MF_BYCOMMAND);//检出
	contextMenu->RemoveMenu(ID_Menu_UndoCheckOut, MF_BYCOMMAND);//撤销检出
	contextMenu->RemoveMenu(ID_Menu_AttachedFile, MF_BYCOMMAND );//附加文件
	CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;
	pPopupMenu->Create (this, point.x, point.y, contextMenu->Detach (),FALSE,TRUE);
	pPopupMenu->SetForceMenuFocus(TRUE);
	pPopupMenu->SetAutoDestroy(TRUE);
	pPopupMenu->ActivatePopupMenu((CBCGPFrameWnd *)AfxGetMainWnd(),pPopupMenu);
	::SetForegroundWindow(pPopupMenu->m_hWnd);
}


void CVersionListDlg::OnMenuDownload()
{
	// TODO: 在此添加命令处理程序代码
	CBCGPGridRow* pchild= this->m_listGrid.GetCurSel();
	HapyProjectFile* thisfile=(HapyProjectFile*)pchild->GetData();
	if(thisfile==NULL) return;
	try{
		thisfile->Download();
		}
	catch(HapyException* e){
		string errorstr=e->Message()+"\n";
		customMethods::customMessagebox(errorstr,L"",MB_OK|MB_ICONERROR);	
	}
}
