
// ClientAppDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "ClientApp.h"
#include "ClientAppDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CClientAppDlg �_�C�A���O



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


// CClientAppDlg ���b�Z�[�W �n���h���[

BOOL CClientAppDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���̃_�C�A���O�̃A�C�R����ݒ肵�܂��B�A�v���P�[�V�����̃��C�� �E�B���h�E���_�C�A���O�łȂ��ꍇ�A
	//  Framework �́A���̐ݒ�������I�ɍs���܂��B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R���̐ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R���̐ݒ�

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
		msg.Format( _T( "�t�@�C���}�b�v�̍쐬�Ɏ��s���܂����B(%d)" ), SCODE_CODE( hRes ) );
		SetDlgItemText( IDC_EDT_FILEMAP, msg );
	}
	else
	{
		SetDlgItemText( IDC_EDT_FILEMAP, m_fileMap );
	}
	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
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
	// �I�[�i�[�E�B���h�E��WM_COPYDATA�Ń��[�U�[�̓��͂����f�[�^�𑗂�
	CWnd* pwndOwner = GetOwner();
	ASSERT( pwndOwner->GetSafeHwnd() != nullptr );
	if( pwndOwner->GetSafeHwnd() != nullptr )
	{
		COPYDATASTRUCT data;
		data.dwData = MAKEFOURCC( 'F', 'M', 'M', 'T' );
		data.cbData = (m_sendData.GetLength()+1)*sizeof( TCHAR );
		data.lpData = m_sendData.GetBuffer();	//	CString �� LPCTSTR ������������o���Ȃ��̂ŁALPTSTR�����o��
		HWND hwnd = *this;
		pwndOwner->SendMessage( WM_COPYDATA, reinterpret_cast<WPARAM>(hwnd), reinterpret_cast<LPARAM>(&data) );
		m_sendData.ReleaseBuffer();
	}
}
