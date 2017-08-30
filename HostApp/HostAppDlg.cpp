
// HostAppDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "HostApp.h"
#include "HostAppDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHostAppDlg ダイアログ



CHostAppDlg::CHostAppDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_HOSTAPP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHostAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CHostAppDlg, CDialog )
	ON_WM_COPYDATA()
END_MESSAGE_MAP()


// CHostAppDlg メッセージ ハンドラー

BOOL CHostAppDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。
	CString param;
	HWND hwndOwner = *this;
	param.Format( _T( "%p" ), hwndOwner );
	SetDlgItemText( ID_HOST_HWND, param );

	auto hRes = m_fileMap.MapSharedMem( 1024, _T( "SampleFileMapping" ) );
	if( FAILED( hRes ) )
	{
		CString msg;
		msg.Format( _T( "Failed:CAtlFileMapping::MapSharedMem():%d" ), SCODE_CODE( hRes ) );
		AfxMessageBox( msg );
	}

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

void CHostAppDlg::OnOK()
{
	TCHAR	modulePath[MAX_PATH];
	GetModuleFileName( nullptr, modulePath, MAX_PATH );
	*PathFindFileName( modulePath ) = _T( '\0' );
	PathCombine( modulePath, modulePath, _T( "ClientApp.exe" ) );
	CString param;
	HWND hwndOwner = *this;
	param.Format( _T( "%p" ), hwndOwner );
	SHELLEXECUTEINFO	info;
	ZeroMemory( &info, sizeof( info ) );
	info.cbSize = sizeof( info );
	info.fMask = SEE_MASK_NOCLOSEPROCESS;
	info.hwnd = *this;
	info.lpVerb = _T( "open" );
	info.lpFile = modulePath;
	info.lpParameters = param;
	info.nShow = SW_SHOWNORMAL;
	if( ShellExecuteEx( &info ) )
	{
		//	更新通知を受け取るためのイベントタスクを起こしておく...
		CloseHandle( info.hProcess );
	}
	//ShellExecute( hwndOwner, nullptr, modulePath, param, nullptr, SW_SHOWNORMAL );
}

BOOL CHostAppDlg::OnCopyData( CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct )
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	if( pCopyDataStruct->dwData == MAKEFOURCC( 'F', 'M', 'M', 'T' ) )
	{
		CString msg( _T( "Received. WM_COPYDATA." ) );
		msg.Format( _T( "Received. WM_COPYDATA.\"%s\"" ), static_cast<LPCTSTR>(pCopyDataStruct->lpData) );
	
		SetDlgItemText( IDC_STC_RECEIVE_DATA, msg );
		return TRUE;
	}
	return CDialog::OnCopyData( pWnd, pCopyDataStruct );
}
