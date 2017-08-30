
// SdiHostApp.h : SdiHostApp アプリケーションのメイン ヘッダー ファイル
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"       // メイン シンボル


// CSdiHostAppApp:
// このクラスの実装については、SdiHostApp.cpp を参照してください。
//

class CSdiHostAppApp : public CWinApp
{
public:
	CSdiHostAppApp();


// オーバーライド
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 実装
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnExecClientApp();
};

extern CSdiHostAppApp theApp;
