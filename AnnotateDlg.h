#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include"CustomMethods.h"

#define  THUMBNAIL_WIDTH 50
#define  THUMBNAIL_HEIGHT 50
// CReadcircle �Ի���
//Ȧ����ע�Ի���
class CAnnotateDlg : public CBCGPDialog
{
	DECLARE_DYNAMIC(CAnnotateDlg)

public:
	CAnnotateDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAnnotateDlg();

	enum { IDD = IDD_ReadCircleAnnotateDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CHAPYListCtrl	m_listGrid;
	CImageList		m_ImageList;
public:
	virtual BOOL OnInitDialog();
	void LoadData(HapyProjectFile* file);
};
