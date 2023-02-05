// VersionsDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "hapyteamclient.h"
#include "VersionListDlg.h"
#include "afxdialogex.h"
#include"CustomMethods.h"
#include"FileDetailsLayoutView.h"


// CVersionsDlg �Ի���
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


// CVersionsDlg ��Ϣ�������


BOOL CVersionListDlg::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	// λ�ô�С
	UINT nTreeStyles = WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER |
		TVS_SHOWSELALWAYS|
		TVS_FULLROWSELECT; //����ѡ��

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

	//����
	m_listGrid.EnableHeader (TRUE, BCGP_GRID_HEADER_SELECT);
	m_listGrid.EnableColumnAutoSize(TRUE); //����������Ӧ���
	m_listGrid.EnableGridLines(TRUE);//��ʾgridline
	m_listGrid.EnableAlternateRows(FALSE);//���ý�����
	m_listGrid.EnableRowHeader(TRUE);
	m_listGrid.SetReadOnly ();
	m_listGrid.SetWholeRowSel ();//����
	m_listGrid.SetSingleSel(TRUE);
	m_listGrid.SetCustomRowHeight(80,80);//�����п�

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
	m_listGrid.InsertColumn(listicon, _T("����ͼ"), onewidth);
	m_listGrid.InsertColumn(listname, _T("�ļ�����"), onewidth);
	m_listGrid.InsertColumn(listtype, _T("�ļ�����"), onewidth);
	m_listGrid.InsertColumn(listsize, _T("�ļ���С"), onewidth);
	m_listGrid.InsertColumn(listauthor , _T("����"), onewidth);
	m_listGrid.InsertColumn(listcreatetime, _T("����ʱ��"), onewidth);
	m_listGrid.InsertColumn(listversion , _T("�汾"), onewidth);	
	m_listGrid.InsertColumn(listrevision, _T("���"), onewidth);
	m_listGrid.InsertColumn(listlife , _T("��������״̬"), onewidth);
	m_listGrid.InsertColumn(listout , _T("�����"), onewidth);
	m_listGrid.InsertColumn(listouttime , _T("���ʱ��"), onewidth);
	m_listGrid.InsertColumn(listin, _T("������"), onewidth);
	m_listGrid.InsertColumn(listintime , _T("����ʱ��"), onewidth);
	m_listGrid.InsertColumn(listnote , _T("��ע"), onewidth);*/

	m_listGrid.SetColumnVisible(listcheck,FALSE);
	m_listGrid.SetColumnLocked(listicon,TRUE);
	//m_list.SetColumnLocked(listicon);
	//m_list.LoadState (_T("versionsLIST"));
	m_listGrid.AdjustLayout();
	m_listGrid.RedrawWindow ();
	return TRUE;  
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CVersionListDlg::OnDestroy()
{
	//m_list.SaveState (_T("versionsLIST"));
	CBCGPDialog::OnDestroy();
	// TODO: �ڴ˴������Ϣ����������
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
			//����ͼ
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
			//����ͼ
			pchild->GetItem (listicon)->SetImage (i);
			//�ļ���
			CString filename (historyfile->Name().c_str());		
			pchild->GetItem (listname)->SetValue ((LPCTSTR)filename);
			//����
			CString fileextension (historyfile->Extension().c_str());
			pchild->GetItem (listtype)->SetValue ((LPCTSTR)fileextension);
			//��С
			CString filelen=customMethods::fileSizeUnit(historyfile->FileSize());
			pchild->GetItem (listsize)->SetValue ((LPCTSTR)filelen);			
			//����
			CString createname (historyfile->CreateName().c_str());
			pchild->GetItem (listauthor)->SetValue ((LPCTSTR)createname);
			//����ʱ��
			CString createtime (historyfile->CreateTime().c_str());
			customMethods::timeFormatter(createtime);
			pchild->GetItem (listcreatetime)->SetValue ((LPCTSTR)createtime);
			//�汾
			CString version;
			version.Format(L"%d",historyfile->FileVersion());
			pchild->GetItem (listversion)->SetValue ((LPCTSTR)version);
			//���
			CString fileReversionLabel (historyfile->FileReversionLabel().c_str());
			pchild->GetItem (listrevision)->SetValue ((LPCTSTR)fileReversionLabel);
			//��������״̬
			CString statusname (historyfile->StateName().c_str());
			pchild->GetItem (listlife)->SetValue ((LPCTSTR)statusname);
			//�����
			CString checkoutname (historyfile->CheckoutName().c_str());
			pchild->GetItem (listout)->SetValue ((LPCTSTR)checkoutname);
			//���ʱ��
			CString filecheckouttime (historyfile->FileCheckouttime().c_str());
			customMethods::timeFormatter(filecheckouttime);
			pchild->GetItem (listouttime)->SetValue ((LPCTSTR)filecheckouttime);
			//������
			CString checkinname (historyfile->CheckinName().c_str());
			pchild->GetItem (listin)->SetValue ((LPCTSTR)checkinname);
			//����ʱ��
			CString filecheckintime (historyfile->FileCheckintime().c_str());
			customMethods::timeFormatter(filecheckintime);
			pchild->GetItem (listintime)->SetValue ((LPCTSTR)filecheckintime);
			//��ע
			pchild->GetItem (listnote)->SetValue (_T(""));
			this->m_listGrid.AddRow (pchild, FALSE);
		}
		this->m_listGrid.SetRedraw(TRUE);
		this->m_listGrid.AdjustLayout();
	}
}
//�Ҽ�
void CVersionListDlg::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CMenu menu;
	menu.LoadMenuW(IDR_FolderMenu);
	CMenu *contextMenu = menu.GetSubMenu(0);
	ASSERT(contextMenu != NULL);
	//���ȵõ������λ��
	CBCGPGridRow* pchild= this->m_listGrid.GetCurSel();
	if(pchild==NULL) return;
	contextMenu->RemoveMenu(ID_Menu_CheckIn, MF_BYCOMMAND);//����
	contextMenu->RemoveMenu(ID_Menu_CheckOut, MF_BYCOMMAND);//���
	contextMenu->RemoveMenu(ID_Menu_UndoCheckOut, MF_BYCOMMAND);//�������
	contextMenu->RemoveMenu(ID_Menu_AttachedFile, MF_BYCOMMAND );//�����ļ�
	CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;
	pPopupMenu->Create (this, point.x, point.y, contextMenu->Detach (),FALSE,TRUE);
	pPopupMenu->SetForceMenuFocus(TRUE);
	pPopupMenu->SetAutoDestroy(TRUE);
	pPopupMenu->ActivatePopupMenu((CBCGPFrameWnd *)AfxGetMainWnd(),pPopupMenu);
	::SetForegroundWindow(pPopupMenu->m_hWnd);
}


void CVersionListDlg::OnMenuDownload()
{
	// TODO: �ڴ���������������
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
