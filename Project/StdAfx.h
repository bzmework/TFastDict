// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__3E6AF3BF_D1CD_4F19_A457_8AB9187D6B94__INCLUDED_)
#define AFX_STDAFX_H__3E6AF3BF_D1CD_4F19_A457_8AB9187D6B94__INCLUDED_

/***��Ȩ��ʶ**********************************/
#define COPYRIGHT _T("��Ȩ����")
#define ABOUTME   _T("��������")
static const char *COMPANY = "elong.tech"; 
static const char *AUTHOR = "denglf";
/*********************************************/

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif

#define _ATL_APARTMENT_THREADED

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__3E6AF3BF_D1CD_4F19_A457_8AB9187D6B94__INCLUDED)
