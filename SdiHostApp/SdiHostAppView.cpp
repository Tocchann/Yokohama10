
// SdiHostAppView.cpp : CSdiHostAppView �N���X�̎���
//

#include "stdafx.h"
// SHARED_HANDLERS �́A�v���r���[�A�k���ŁA����ь����t�B���^�[ �n���h���[���������Ă��� ATL �v���W�F�N�g�Œ�`�ł��A
// ���̃v���W�F�N�g�Ƃ̃h�L�������g �R�[�h�̋��L���\�ɂ��܂��B
#ifndef SHARED_HANDLERS
#include "SdiHostApp.h"
#endif

#include "SdiHostAppDoc.h"
#include "SdiHostAppView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSdiHostAppView

IMPLEMENT_DYNCREATE(CSdiHostAppView, CView)

BEGIN_MESSAGE_MAP(CSdiHostAppView, CView)
	ON_WM_COPYDATA()
END_MESSAGE_MAP()

// CSdiHostAppView �R���X�g���N�V����/�f�X�g���N�V����

CSdiHostAppView::CSdiHostAppView()
{
	// TODO: �\�z�R�[�h�������ɒǉ����܂��B

}

CSdiHostAppView::~CSdiHostAppView()
{
}

BOOL CSdiHostAppView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: ���̈ʒu�� CREATESTRUCT cs ���C������ Window �N���X�܂��̓X�^�C����
	//  �C�����Ă��������B

	return CView::PreCreateWindow(cs);
}

// CSdiHostAppView �`��

void CSdiHostAppView::OnDraw(CDC* pDC)
{
	CSdiHostAppDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if( !m_receiveData.IsEmpty() )
	{
		CRect	rc;
		GetClientRect( rc );
		pDC->DrawText( m_receiveData, rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE );
	}
}


// CSdiHostAppView �f�f

#ifdef _DEBUG
void CSdiHostAppView::AssertValid() const
{
	CView::AssertValid();
}

void CSdiHostAppView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSdiHostAppDoc* CSdiHostAppView::GetDocument() const // �f�o�b�O�ȊO�̃o�[�W�����̓C�����C���ł��B
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSdiHostAppDoc)));
	return (CSdiHostAppDoc*)m_pDocument;
}
#endif //_DEBUG


// CSdiHostAppView ���b�Z�[�W �n���h���[


BOOL CSdiHostAppView::OnCopyData( CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct )
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	if( pCopyDataStruct->dwData == MAKEFOURCC( 'F', 'M', 'M', 'T' ) )
	{
		m_receiveData = static_cast<LPCTSTR>(pCopyDataStruct->lpData);
		Invalidate();
		return TRUE;
	}
	return CView::OnCopyData( pWnd, pCopyDataStruct );
}
