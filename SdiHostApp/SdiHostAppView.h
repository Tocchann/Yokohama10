
// SdiHostAppView.h : CSdiHostAppView �N���X�̃C���^�[�t�F�C�X
//

#pragma once


class CSdiHostAppView : public CView
{
protected: // �V���A��������̂ݍ쐬���܂��B
	CSdiHostAppView();
	DECLARE_DYNCREATE(CSdiHostAppView)

// ����
public:
	CSdiHostAppDoc* GetDocument() const;

// ����
public:

// �I�[�o�[���C�h
public:
	virtual void OnDraw(CDC* pDC);  // ���̃r���[��`�悷�邽�߂ɃI�[�o�[���C�h����܂��B
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	CString m_receiveData;
// ����
public:
	virtual ~CSdiHostAppView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnCopyData( CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct );
};

#ifndef _DEBUG  // SdiHostAppView.cpp �̃f�o�b�O �o�[�W����
inline CSdiHostAppDoc* CSdiHostAppView::GetDocument() const
   { return reinterpret_cast<CSdiHostAppDoc*>(m_pDocument); }
#endif

