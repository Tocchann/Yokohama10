
// HostAppDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "HostApp.h"
#include "HostAppDlg.h"

#include <ppltasks.h>

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
	CDialog::DoDataExchange( pDX );
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
	SetDlgItemText( IDC_EDT_FILEMAP, _T( "転送テスト用FileMapテキスト" ) );
	auto hRes = m_fileMap.MapSharedMem( 1024, _T( "Wankuma-Yokohama#10-SharedMem" ) );
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
	GetDlgItemText( IDC_EDT_FILEMAP, m_fileMap, static_cast<int>(m_fileMap.GetMappingSize()) );
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
		//	別タスク(異なるスレッドコンテキスト)で、呼び出したクライアントアプリの終了を待つ
		HANDLE hProcess = info.hProcess;
		concurrency::create_task( [hProcess]()
		{
			WaitForSingleObject( hProcess, INFINITE );
			CloseHandle( hProcess );
		} ).then( [this]()
		{
			//	クライアントアプリが終了後に呼び出されるが、どのスレッドで動いているかは特定できない(メインスレッド以外も十分あり得る)
			this->SetDlgItemText( IDC_EDT_FILEMAP, m_fileMap );
			if( this->IsWindowEnabled() == FALSE )
			{
				this->EnableWindow( TRUE );	//	強制終了対策
			}
		} );
	}
	//ShellExecute( hwndOwner, nullptr, modulePath, param, nullptr, SW_SHOWNORMAL );
}

BOOL CHostAppDlg::OnCopyData( CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct )
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	if( pCopyDataStruct->dwData == MAKEFOURCC( 'F', 'M', 'M', 'T' ) )
	{
		CString msg;
		msg.Format( _T( "Received WM_COPYDATA.\"%s\"" ), static_cast<LPCTSTR>(pCopyDataStruct->lpData) );
	
		SetDlgItemText( IDC_STC_RECEIVE_DATA, msg );
		return TRUE;
	}
	return CDialog::OnCopyData( pWnd, pCopyDataStruct );
}
