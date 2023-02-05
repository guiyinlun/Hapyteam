#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include"CustomMethods.h"


extern int nSel;
// CQuoteDlg �Ի���

class CReferenceListDlg : public CBCGPDialog
{
	DECLARE_DYNAMIC(CReferenceListDlg)

public:
	CReferenceListDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CReferenceListDlg();

// �Ի�������
	enum { IDD = IDD_ReferenceDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CBCGPComboBox	m_Combo;
	CHAPYTreeCtrl	m_treeGrid;
	HapyProjectFile* m_pselectfile;
protected:
	virtual BOOL OnInitDialog();
	//ѡ�иı������б��
	afx_msg void OnCbnSelchangequotecombo();
	//�ڵ�չ�����𼶼��أ�
	afx_msg void OnItemExpanding(NMHDR* pNMHDR, LRESULT* pResult);
	//�ر�
	afx_msg void OnDestroy();
public:
	//����
	void ResetComboAndTree();
	//���������б��
	void LoadCombo(HapyProjectFile* file);
	//����������ڵ�
	void AddOneData(HapyProjectFile* file,CBCGPGridRow* parentR);
	//���ݰ汾combonum��������
	void LoadData(HapyProjectFile* file,int combonum);
};
