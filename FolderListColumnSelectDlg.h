#pragma once


// CListcomsel �Ի���

class CFolderListColumnSelectDlg : public CBCGPDialog
{
	DECLARE_DYNAMIC(CFolderListColumnSelectDlg)

public:
	CFolderListColumnSelectDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFolderListColumnSelectDlg();

// �Ի�������
	enum { IDD = IDD_FolderListColumnSelectDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CBCGPTreeCtrlEx m_treeGrid;
public:
	void init();
	afx_msg LRESULT OnHeaderCheckClick(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnCheckClick(WPARAM wp, LPARAM lp);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedreset();
	afx_msg void OnBnClickedOk();
	afx_msg void OnDestroy();
};
