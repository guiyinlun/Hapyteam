#pragma once


// CBasic �Ի���

class CBasicInfoDlg : public CBCGPDialog
{
	DECLARE_DYNAMIC(CBasicInfoDlg)

public:
	CBasicInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBasicInfoDlg();

// �Ի�������
	enum { IDD = IDD_BasicDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
