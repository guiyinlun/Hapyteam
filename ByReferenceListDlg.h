#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CReferenced �Ի���

class CByReferenceListDlg : public CBCGPDialog
{
	DECLARE_DYNAMIC(CByReferenceListDlg)

public:
	CByReferenceListDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CByReferenceListDlg();

// �Ի�������
	enum { IDD = IDD_ByReferenceList };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CTreeCtrl m_tree;
	CComboBox m_combo;
};
