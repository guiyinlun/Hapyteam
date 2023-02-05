//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a sample for BCGControlBar Library Professional Edition
// Copyright (C) 1998-2022 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//*******************************************************************************
//
// VisualManager.h: interface for the CVisualManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VISUALMANAGER_H__23C4AAB0_1938_4B6E_81A1_70497653D68A__INCLUDED_)
#define AFX_VISUALMANAGER_H__23C4AAB0_1938_4B6E_81A1_70497653D68A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CHAPYTheme : public CBCGPVisualManager2016
{
	DECLARE_DYNCREATE(CHAPYTheme)

public:
	enum Style
	{
		HAPY_White,
		HAPY_Black,
	};
	static void SetStyle(Style style);
	static Style GetStyle() { return m_Style; }
	CHAPYTheme();
	virtual ~CHAPYTheme();
	virtual void SetupColors();
	virtual void ModifyGlobalColors ();
	static Style	m_Style;
};
#endif // !defined(AFX_VISUALMANAGER_H__23C4AAB0_1938_4B6E_81A1_70497653D68A__INCLUDED_)
