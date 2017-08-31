
// HostAppDlg.h : ヘッダー ファイル
//

#pragma once
#include <atlfile.h>
#include "afxwin.h"

// CHostAppDlg ダイアログ
class CHostAppDlg : public CDialog
{
// コンストラクション
public:
	CHostAppDlg(CWnd* pParent = NULL);	// 標準コンストラクター

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HOSTAPP_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;
	CAtlFileMapping<TCHAR>	m_fileMap;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	afx_msg BOOL OnCopyData( CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct );
};
