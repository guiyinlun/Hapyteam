// ProjectList.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "hapyteamclient.h"
#include "hapyteamclientDoc.h"
#include "ProjectListDlg.h"
#include "afxdialogex.h"
#include <Interface/HapyProjectManager.h>

// CProjectList �Ի���

IMPLEMENT_DYNAMIC(CProjectList, CBCGPDialog)

	CProjectList::CProjectList(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CProjectList::IDD, pParent)
{
	EnableVisualManagerStyle(TRUE, TRUE);
	//EnableLayout();
}

CProjectList::~CProjectList()
{
}

void CProjectList::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_projectLIST, m_prolist);
}


BEGIN_MESSAGE_MAP(CProjectList, CBCGPDialog)
	ON_NOTIFY(NM_DBLCLK, IDC_projectLIST, &CProjectList::OnNMDblclkprojectlist)
END_MESSAGE_MAP()

// CProjectList ��Ϣ�������
BOOL CProjectList::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();
	//���ͼ��
	m_ImageList.DeleteImageList();
	m_ImageList.Create(64, 64, ILC_COLOR32|ILC_MASK,0,0);//����
	if(theApp.m_theme)
		m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_profolderblack));
	else
		m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_profolderwhite));//���
	m_prolist.SetImageList(&m_ImageList, LVSIL_NORMAL);//����
	
	//huangc
	HapyProjectManager::GetInstance()->Refresh();
	vector<HapyProject*> projs=HapyProjectManager::GetInstance()->GetProjects();
	if (projs.size()>0)
	{
		std::string str;
		for(int i=0;i<projs.size();i++)
		{
			HapyProject* proj=projs.at(i);
			CString cstr(proj->ProjectName().c_str());
		    int nindex= m_prolist.InsertItem(i, cstr,0);
			m_prolist.SetItemData(nindex,(DWORD_PTR)proj);
		}
	}
	return TRUE;  
}
void CProjectList::OnNMDblclkprojectlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	//��ȡѡ����Ŀ��ID
	POSITION pos = m_prolist.GetFirstSelectedItemPosition();
	if (pos!=NULL)
	{
		int item_index =(int)m_prolist.GetNextSelectedItem(pos);
		HapyProject* proj = (HapyProject*)m_prolist.GetItemData(item_index);
		HapyProjectManager::GetInstance()->SwitchProject(proj->ProjectName());	
		CBCGPDialog::OnOK();
	}	
	*pResult = 0;
}	

