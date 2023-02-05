// FilePropertiesViewBar.h : interface of the CBCGPPropBar class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CHAPYPropertyBar : public CBCGPDockingControlBar  
{
// Construction
public:
	CHAPYPropertyBar();
// Attributes
public:
	void SetVSDotNetLook (BOOL bSet)
	{
		m_propertyList.SetVSDotNetLook (bSet);
		m_propertyList.SetGroupNameFullWidth (bSet);
	}

public:
	CBCGPPropList	m_propertyList;

// Attributes
protected:

// Overrides

// Implementation
public:
	virtual ~CHAPYPropertyBar();

	// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void InitPropList ();
	void AdjustLayout ();
	void SetPropListFont();

	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg LRESULT OnCommandClicked(WPARAM, LPARAM);
	afx_msg LRESULT OnMenuItemSelected(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
public:
	void ResetList();
	void LoadData();
	
};
