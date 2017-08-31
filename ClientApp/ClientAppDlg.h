
// ClientAppDlg.h : �w�b�_�[ �t�@�C��
//

#pragma once
#include <atlfile.h>

// CClientAppDlg �_�C�A���O
class CClientAppDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CClientAppDlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^�[

// �_�C�A���O �f�[�^
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENTAPP_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g


// ����
protected:
	HICON m_hIcon;
	CAtlFileMapping<TCHAR>	m_fileMap;

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_strOwnerHwnd;
	afx_msg void OnBnClickedBtnSendMsg();
	CString m_sendData;
};
