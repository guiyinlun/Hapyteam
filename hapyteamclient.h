
// hapyteamclient.h : hapyteamclient Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"

#endif

#include "resource.h"       // ������
#include"CustomMethods.h"  
#include <Core/StringUtils.h>
#include "MainFrm.h"
#include"FolderListView.h"
#include"ProjectTreeView.h"


// ChapyteamclientApp:
// �йش����ʵ�֣������ hapyteamclient.cpp
//

class ChapyteamclientApp : public CBCGPWinApp
{
public:
	ChapyteamclientApp();

	virtual void PreLoadState ();
public:
	//�����ļ�·��
	CString	m_configpath;
	//�����ַ���
	CString m_languagecstr;
	string	m_thumbnailpath;
	//��¼״̬
	bool	m_loginstatus;
	//���⣨��0��1��
	int		m_theme;
	//���ԣ���0Ӣ1��
	int		m_language;
	//����
	CFont	m_font; 
	//�����ź�
	BOOL	g_bRestart;
private:
	//��������dll
	HINSTANCE m_hLangDLL;	
	
// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//��������
	void SetTheme(BOOL bRead);
	//������һ�ιر�ʱ����Ŀ
	void LoadLastProject();
	// ���ڶԻ���
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern ChapyteamclientApp theApp;
