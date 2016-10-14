// ATM_DEMO.h : main header file for the ATM_DEMO application
//

#if !defined(AFX_ATM_DEMO_H__EFAC8DEA_D2B9_4A66_AA1B_99678568DFCE__INCLUDED_)
#define AFX_ATM_DEMO_H__EFAC8DEA_D2B9_4A66_AA1B_99678568DFCE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CATM_DEMOApp:
// See ATM_DEMO.cpp for the implementation of this class
//

class CATM_DEMOApp : public CWinApp
{
public:
	CATM_DEMOApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CATM_DEMOApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CATM_DEMOApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATM_DEMO_H__EFAC8DEA_D2B9_4A66_AA1B_99678568DFCE__INCLUDED_)
