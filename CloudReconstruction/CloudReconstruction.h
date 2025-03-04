﻿
// CloudReconstruction.h: CloudReconstruction 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // 主符号


// CCloudReconstructionApp:
// 有关此类的实现，请参阅 CloudReconstruction.cpp
//

class CCloudReconstructionApp : public CWinAppEx
{
public:
	CCloudReconstructionApp() noexcept;


// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CCloudReconstructionApp theApp;
