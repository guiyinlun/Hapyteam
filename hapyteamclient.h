
// hapyteamclient.h : hapyteamclient 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"

#endif

#include "resource.h"       // 主符号
#include"CustomMethods.h"  
#include <Core/StringUtils.h>
#include "MainFrm.h"
#include"FolderListView.h"
#include"ProjectTreeView.h"


// ChapyteamclientApp:
// 有关此类的实现，请参阅 hapyteamclient.cpp
//

class ChapyteamclientApp : public CBCGPWinApp
{
public:
	ChapyteamclientApp();

	virtual void PreLoadState ();
public:
	//配置文件路径
	CString	m_configpath;
	//语言字符串
	CString m_languagecstr;
	string	m_thumbnailpath;
	//登录状态
	bool	m_loginstatus;
	//主题（黑0白1）
	int		m_theme;
	//语言（中0英1）
	int		m_language;
	//字体
	CFont	m_font; 
	//重启信号
	BOOL	g_bRestart;
private:
	//加载语言dll
	HINSTANCE m_hLangDLL;	
	
// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//设置主题
	void SetTheme(BOOL bRead);
	//加载上一次关闭时的项目
	void LoadLastProject();
	// 关于对话框
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern ChapyteamclientApp theApp;
