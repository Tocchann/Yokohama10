
// HostAppDlg.h : �w�b�_�[ �t�@�C��
//

#pragma once
#include <atlfile.h>
#include "afxwin.h"

// CHostAppDlg �_�C�A���O
class CHostAppDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CHostAppDlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^�[

// �_�C�A���O �f�[�^
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HOSTAPP_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g


// ����
protected:
	HICON m_hIcon;
	CAtlFileMapping<TCHAR>	m_fileMap;

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	afx_msg BOOL OnCopyData( CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct );
};
