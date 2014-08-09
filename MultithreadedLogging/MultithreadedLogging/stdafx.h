#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN   //exclude rarely used stuff from windows headers
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS   //some CString constructors will be explicit

#define _AFX_ALL_WARNINGS 

#include <afxwin.h> //MFC core and standard components
#include <afxext.h>  // MFC extensions
#include<Windows.h>