
// ClientAppDlg.h : ヘッダー ファイル
//

#pragma once
#include <atlfile.h>

// CClientAppDlg ダイアログ
class CClientAppDlg : public CDialog
{
// コンストラクション
public:
	CClientAppDlg(CWnd* pParent = NULL);	// 標準コンストラクター

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENTAPP_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;
	CAtlFileMapping<TCHAR>	m_fileMap;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_strOwnerHwnd;
	afx_msg void OnBnClickedBtnSendMsg();
	CString m_sendData;
};
