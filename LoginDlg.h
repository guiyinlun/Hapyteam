#pragma once
#include "afxwin.h"
#include <Core/StringUtils.h>
#include <string>

// CLoginDlg 对话框

class CLoginDlg : public CBCGPDialog
{
	DECLARE_DYNAMIC(CLoginDlg)

public:
	CLoginDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLoginDlg();

// 对话框数据
	enum { IDD = IDD_LoginDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CBCGPEdit		m_useredit;
	CBCGPEdit		m_passwdedit;
	CBCGPButton		m_ok;
	CString			m_cusername; 
	CString			m_cpassword; 
	std::string		m_username; 
	std::string		m_password;
	CFont			m_font;
	CStatic			m_welcomestatic;
protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	virtual void OnOK();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedexitbutton();
	afx_msg LRESULT OnNcHitTest(CPoint point);
};
