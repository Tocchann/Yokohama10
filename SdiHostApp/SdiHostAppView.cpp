
// SdiHostAppView.cpp : CSdiHostAppView クラスの実装
//

#include "stdafx.h"
// SHARED_HANDLERS は、プレビュー、縮小版、および検索フィルター ハンドラーを実装している ATL プロジェクトで定義でき、
// そのプロジェクトとのドキュメント コードの共有を可能にします。
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

// CSdiHostAppView コンストラクション/デストラクション

CSdiHostAppView::CSdiHostAppView()
{
	// TODO: 構築コードをここに追加します。

}

CSdiHostAppView::~CSdiHostAppView()
{
}

BOOL CSdiHostAppView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return CView::PreCreateWindow(cs);
}

// CSdiHostAppView 描画

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


// CSdiHostAppView 診断

#ifdef _DEBUG
void CSdiHostAppView::AssertValid() const
{
	CView::AssertValid();
}

void CSdiHostAppView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSdiHostAppDoc* CSdiHostAppView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSdiHostAppDoc)));
	return (CSdiHostAppDoc*)m_pDocument;
}
#endif //_DEBUG


// CSdiHostAppView メッセージ ハンドラー


BOOL CSdiHostAppView::OnCopyData( CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct )
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	if( pCopyDataStruct->dwData == MAKEFOURCC( 'F', 'M', 'M', 'T' ) )
	{
		m_receiveData = static_cast<LPCTSTR>(pCopyDataStruct->lpData);
		Invalidate();
		return TRUE;
	}
	return CView::OnCopyData( pWnd, pCopyDataStruct );
}
