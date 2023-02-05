#include "stdafx.h"
#include "hapyteamclient.h"
#include "AnnotateDlg.h"
#include "afxdialogex.h"
#include <io.h>
#include <vector>

// CReadcircle �Ի���

IMPLEMENT_DYNAMIC(CAnnotateDlg, CBCGPDialog)

CAnnotateDlg::CAnnotateDlg(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CAnnotateDlg::IDD, pParent)
{
	EnableVisualManagerStyle(TRUE, TRUE);
	EnableLayout();
}

CAnnotateDlg::~CAnnotateDlg()
{
}

void CAnnotateDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAnnotateDlg, CBCGPDialog)

END_MESSAGE_MAP()


// CReadcircle ��Ϣ�������
BOOL CAnnotateDlg::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	UINT nTreeStyles = WS_CHILD | WS_VISIBLE | WS_TABSTOP |WS_BORDER |
		TVS_SHOWSELALWAYS|
		TVS_FULLROWSELECT; //����ѡ��
	m_listGrid.CreateOnPlaceHolder(this, IDC_readcirclelist, 0, nTreeStyles);
	CRect thisrect,viewrect;
	this->GetClientRect(&viewrect);
	thisrect.SetRect(-2,-2,viewrect.right+2,viewrect.bottom+2);
	m_listGrid.MoveWindow(&thisrect);
	CBCGPStaticLayout* pLayout = (CBCGPStaticLayout*)GetLayout();
	if (pLayout != NULL)
	{
		pLayout->AddAnchor(m_listGrid.GetSafeHwnd(), CBCGPStaticLayout::e_MoveTypeNone, CBCGPStaticLayout::e_SizeTypeBoth);
	}

	
	
	m_ImageList.Create(80 ,80, ILC_COLOR32 | ILC_MASK, 3, 1);
	/*CBitmap bitmap;
	CImage image;
	image.Load(_T("D:\\Desktop\\testdown\\The_thumbnail\\ֱ����.bmp"));
	HBITMAP hBitmap = (HBITMAP)image;
	bitmap.Attach(hBitmap);
	m_ImageList.Add(&bitmap, RGB(0xff, 0xff, 0xff));*/


	CBitmap m_backBitmap;
	HBITMAP bitmap;
	bitmap=(HBITMAP)LoadImage(AfxGetInstanceHandle(),L"D:\\Desktop\\testdown\\2.bmp",
		IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	m_backBitmap.DeleteObject();
	if(!m_backBitmap.Attach(bitmap))
	{
		//MessageBox("���뱳��ͼʧ��!","��ʾ",MB_OK);
	}
	else
	{
		int index = m_ImageList.Add(&m_backBitmap, nullptr);
		m_backBitmap.DeleteObject();
	}
	 
	m_listGrid.SetImageList(&m_ImageList);
	//����
	m_listGrid.EnableHeader (TRUE, BCGP_GRID_HEADER_SELECT);
	m_listGrid.EnableColumnAutoSize(TRUE); //����������Ӧ���
	m_listGrid.EnableGridLines(TRUE);//��ʾgridline
	m_listGrid.EnableAlternateRows(FALSE);//���ý�����
	m_listGrid.EnableRowHeader(TRUE);
	m_listGrid.SetReadOnly ();
	m_listGrid.SetWholeRowSel ();//����
	m_listGrid.SetCustomRowHeight(128,128);//�����п�

	m_listGrid.SetVisualManagerColorTheme(TRUE);
	m_listGrid.SetFont(&theApp.m_font);
	//Column
	m_listGrid.DeleteAllColumns();
	int m_width=1000;
	m_listGrid.InsertColumn(0, _T("����"), m_width);
	m_listGrid.InsertColumn(1, _T("����"), m_width);
	m_listGrid.InsertColumn(2, _T("��С"), m_width);
	m_listGrid.InsertColumn(3 , _T("����"), m_width);
	m_listGrid.InsertColumn(4, _T("����ʱ��"), m_width);
	m_listGrid.InsertColumn(5 , _T("�汾"), m_width);	
	m_listGrid.InsertColumn(6, _T("���"), m_width);
	for(int i=0;i<10;i++){
		CBCGPGridRow* pchild= m_listGrid.CreateRow (m_listGrid.GetColumnCount());
		ASSERT_VALID (pchild);
		pchild->GetItem (0)->SetImage(0);
		pchild->GetItem (1)->SetValue(L"test");
		pchild->GetItem (2)->SetValue(L"test");
		m_listGrid.AddRow (pchild, FALSE);

	}
	m_listGrid.AdjustLayout();
	m_listGrid.RedrawWindow ();	
	return TRUE;  
	
}
void CAnnotateDlg::LoadData(HapyProjectFile* file){
	this->m_listGrid.RemoveAll();

}


