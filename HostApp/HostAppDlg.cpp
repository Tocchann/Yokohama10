
// HostAppDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "HostApp.h"
#include "HostAppDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHostAppDlg �_�C�A���O



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


// CHostAppDlg ���b�Z�[�W �n���h���[

BOOL CHostAppDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���̃_�C�A���O�̃A�C�R����ݒ肵�܂��B�A�v���P�[�V�����̃��C�� �E�B���h�E���_�C�A���O�łȂ��ꍇ�A
	//  Framework �́A���̐ݒ�������I�ɍs���܂��B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R���̐ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R���̐ݒ�

	// TODO: �������������ɒǉ����܂��B
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

	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
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
		//	�X�V�ʒm���󂯎�邽�߂̃C�x���g�^�X�N���N�����Ă���...
		CloseHandle( info.hProcess );
	}
	//ShellExecute( hwndOwner, nullptr, modulePath, param, nullptr, SW_SHOWNORMAL );
}

BOOL CHostAppDlg::OnCopyData( CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct )
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	if( pCopyDataStruct->dwData == MAKEFOURCC( 'F', 'M', 'M', 'T' ) )
	{
		CString msg( _T( "Received. WM_COPYDATA." ) );
		msg.Format( _T( "Received. WM_COPYDATA.\"%s\"" ), static_cast<LPCTSTR>(pCopyDataStruct->lpData) );
	
		SetDlgItemText( IDC_STC_RECEIVE_DATA, msg );
		return TRUE;
	}
	return CDialog::OnCopyData( pWnd, pCopyDataStruct );
}
