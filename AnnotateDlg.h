#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include"CustomMethods.h"

#define  THUMBNAIL_WIDTH 50
#define  THUMBNAIL_HEIGHT 50
// CReadcircle 对话框
//圈阅批注对话框
class CAnnotateDlg : public CBCGPDialog
{
	DECLARE_DYNAMIC(CAnnotateDlg)

public:
	CAnnotateDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAnnotateDlg();

	enum { IDD = IDD_ReadCircleAnnotateDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CHAPYListCtrl	m_listGrid;
	CImageList		m_ImageList;
public:
	virtual BOOL OnInitDialog();
	void LoadData(HapyProjectFile* file);
};
