
// ClientAppDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ClientApp.h"
#include "ClientAppDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CClientAppDlg ダイアログ



CClientAppDlg::CClientAppDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_CLIENTAPP_DIALOG, pParent)
	, m_strOwnerHwnd( _T( "" ) )
	, m_sendData( _T( "" ) )
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange( pDX );
	DDX_Text( pDX, IDC_OWNER_HWND, m_strOwnerHwnd );
	DDX_Text( pDX, IDC_EDIT_SEND_DATA, m_sendData );
}

BEGIN_MESSAGE_MAP(CClientAppDlg, CDialog )
	ON_BN_CLICKED( IDC_BTN_SEND_MSG, &CClientAppDlg::OnBnClickedBtnSendMsg )
END_MESSAGE_MAP()


// CClientAppDlg メッセージ ハンドラー

BOOL CClientAppDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	CWnd* pwndOwner = GetOwner();
	if( pwndOwner->GetSafeHwnd() == nullptr )
	{
		GetDlgItem( IDC_BTN_SEND_MSG )->EnableWindow( FALSE );
	}
	auto hRes = m_fileMap.OpenMapping( _T( "Wankuma-Yokohama#10-SharedMem" ), 1024 );
	if( FAILED( hRes ) )
	{
		GetDlgItem( IDC_EDT_FILEMAP )->EnableWindow( FALSE );
		CString msg;
		msg.Format( _T( "ファイルマップの作成に失敗しました。(%d)" ), SCODE_CODE( hRes ) );
		SetDlgItemText( IDC_EDT_FILEMAP, msg );
	}
	else
	{
		SetDlgItemText( IDC_EDT_FILEMAP, m_fileMap );
	}
	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}


void CClientAppDlg::OnBnClickedBtnSendMsg()
{
	if( !UpdateData() )
	{
		return;
	}
	if( GetDlgItem( IDC_EDT_FILEMAP )->IsWindowEnabled() )
	{
		GetDlgItemText( IDC_EDT_FILEMAP, m_fileMap, m_fileMap.GetMappingSize() );
	}
	// オーナーウィンドウにWM_COPYDATAでユーザーの入力したデータを送る
	CWnd* pwndOwner = GetOwner();
	ASSERT( pwndOwner->GetSafeHwnd() != nullptr );
	if( pwndOwner->GetSafeHwnd() != nullptr )
	{
		COPYDATASTRUCT data;
		data.dwData = MAKEFOURCC( 'F', 'M', 'M', 'T' );
		data.cbData = (m_sendData.GetLength()+1)*sizeof( TCHAR );
		data.lpData = m_sendData.GetBuffer();	//	CString は LPCTSTR しか引っ張り出せないので、LPTSTRを作り出す
		HWND hwnd = *this;
		pwndOwner->SendMessage( WM_COPYDATA, reinterpret_cast<WPARAM>(hwnd), reinterpret_cast<LPARAM>(&data) );
		m_sendData.ReleaseBuffer();
	}
}
