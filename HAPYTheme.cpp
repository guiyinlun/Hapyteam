#include "stdafx.h"
#include "hapyteamclient.h"
#include "HAPYTheme.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CHAPYTheme, CBCGPVisualManager2016)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CHAPYTheme::Style CHAPYTheme::m_Style = CHAPYTheme::HAPY_Black;
CHAPYTheme::CHAPYTheme()
{
	CBCGPVisualManager2016::m_Style = (m_Style == HAPY_Black) ? 
		CBCGPVisualManager2016::Office2016_Black : CBCGPVisualManager2016::Office2016_White;
}

CHAPYTheme::~CHAPYTheme()
{
}

void CHAPYTheme::SetStyle(Style style)
{
	m_Style = style;
	CBCGPVisualManager2016::m_Style = (m_Style == HAPY_Black) ? 
		CBCGPVisualManager2016::Office2016_Black : CBCGPVisualManager2016::Office2016_White;
	switch (m_Style)
	{
	case HAPY_Black:
		CBCGPVisualManager2016::SetStyle(CBCGPVisualManager2016::Office2016_Black);
		break;

	case HAPY_White:
		CBCGPVisualManager2016::SetStyle(CBCGPVisualManager2016::Office2016_White);
		break;
	}
}
void CHAPYTheme::SetupColors()
{
	m_bUseVS2012AccentColors = (m_Style == HAPY_Black);
	CBCGPVisualManager2016::SetupColors();

	m_brCaption.DeleteObject();
	m_brCloseButtonHighlighted.DeleteObject();
	m_brCloseButtonPressed.DeleteObject();

	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContrastMode())
	{
		return;
	}
	m_brCloseButtonHighlighted.CreateSolidBrush(RGB(232, 17, 35));
	m_brCloseButtonPressed.CreateSolidBrush(RGB(241, 112, 122));
	m_bCheckedRibbonButtonFrame = TRUE;

	if (m_Style == HAPY_White)
	{
		m_clrCaption = globalData.clrBarFace;
		m_brCaption.CreateSolidBrush(m_clrCaption);
		m_clrState=RGB(255,255,255);//状态栏颜色

	}else
	if (m_Style == HAPY_Black){
		m_bCheckedRibbonButtonFrame = FALSE;
		m_clrRibbonTabs = IsColoredRibbonTabsArea() ? RGB(255, 255, 255) : globalData.clrBarText;
		m_clrTextDisabled =RGB(183,181,186);

		m_clrMenuLight = m_clrRibbonCategoryFill = globalData.clrBarFace;
		m_clrMenuSeparator = globalData.clrBarLight;
		m_clrMenuBorder = globalData.clrBarDkShadow;

		m_clrControl = globalData.clrBarFace;
		m_clrSeparator = globalData.clrBarLight;

		m_clrHighlight = m_clrHighlightMenuItem = RGB(48,57,65);
		m_clrHighlightDn = globalData.clrBarLight;
		m_clrHighlightChecked = RGB(24, 24, 24);

		m_clrHighlightGradientLight = m_clrHighlight;
		m_clrHighlightGradientDark = m_clrHighlight;

		m_clrCaption = RGB(49,49,60);
		m_brCaption.CreateSolidBrush(m_clrCaption);//标题栏颜色
		m_clrDlgBackground = globalData.clrBarFace;

		m_clrEditBoxBorder = RGB(106, 106, 106);
		m_clrEditBoxBorderDisabled = RGB(97, 97, 97);
		m_clrBorderActive = RGB(153, 153, 153);

		m_clrHighlightNC = RGB(100, 100, 100);
		m_clrHighlightDnNC = RGB(70, 70, 70);

		m_clrNcTextActive = globalData.clrBarText;
		m_clrNcTextInactive = globalData.clrBarText;
		m_clrNcBorder = m_clrCaption;

		m_clrScrollButtonHot = m_clrControl;
		m_clrScrollButton = globalData.clrBarHilite;

		m_clrPressedButtonBorder = globalData.clrBarDkShadow;
		m_clrState=RGB(49,49,60);//状态栏颜色
		m_clrPlannerTextHeader = globalData.clrBarText;
		m_clrPlannerTextHeaderHilite = globalData.clrBarText;
		m_clrPlannerTextHeaderPrep = globalData.clrBarFace;
		m_clrPlannerTextHeaderPrepHilite = globalData.clrBarFace;
		m_clrPlannerTextApp = globalData.clrBarFace;
		m_clrMenuItemBorder = m_clrHighlight;
		m_clrPalennerLine= m_clrBorderActive;
		m_clrPlannerTodayLine = m_clrPlannerTodayFill;
		//m_brCaption.CreateSolidBrush(m_clrCaption);
	}
}
	

void CHAPYTheme::ModifyGlobalColors (){
	CBCGPVisualManager2016::ModifyGlobalColors();
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContrastMode())
	{
		return;
	}
	if(m_Style==HAPY_Black){
		globalData.clrBarFace = RGB(35,35,45);
		globalData.clrBarText = RGB(255,255,255);
		globalData.clrBarShadow = RGB(49,49,60);//一些线条阴影
		globalData.clrBarHilite = RGB (49,49,60);
		globalData.clrBarDkShadow =RGB(120,120,120);//表头分割线条，菜单子项边框，树折叠按钮
		globalData.clrBarLight = RGB(49,49,60);//边框	
	}
	globalData.brBarFace.DeleteObject ();
	globalData.brBarFace.CreateSolidBrush (globalData.clrBarFace);
}

