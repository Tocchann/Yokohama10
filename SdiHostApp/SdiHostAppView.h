
// SdiHostAppView.h : CSdiHostAppView クラスのインターフェイス
//

#pragma once


class CSdiHostAppView : public CView
{
protected: // シリアル化からのみ作成します。
	CSdiHostAppView();
	DECLARE_DYNCREATE(CSdiHostAppView)

// 属性
public:
	CSdiHostAppDoc* GetDocument() const;

// 操作
public:

// オーバーライド
public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画するためにオーバーライドされます。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	CString m_receiveData;
// 実装
public:
	virtual ~CSdiHostAppView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成された、メッセージ割り当て関数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnCopyData( CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct );
};

#ifndef _DEBUG  // SdiHostAppView.cpp のデバッグ バージョン
inline CSdiHostAppDoc* CSdiHostAppView::GetDocument() const
   { return reinterpret_cast<CSdiHostAppDoc*>(m_pDocument); }
#endif

