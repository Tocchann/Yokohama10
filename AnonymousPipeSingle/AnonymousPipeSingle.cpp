// AnonymousPipeSingle.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

#include <ppltasks.h>

#define __send_int__

static void WriterTask( HANDLE hPipe, LPCTSTR text = nullptr )
{
	_putts( _T( "Start:WriterTask()" ) );
	DWORD written;
#ifdef __send_int__
	int arr[] ={ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	auto result = WriteFile( hPipe, arr, sizeof( arr ), &written, nullptr );
#else
	const TCHAR msg[] = _T("日本語を含む文字列を表現しておくよ");
	int length = sizeof( msg );
	if( text == nullptr )
	{
		text = msg;
	}
	else if( *text != _T( '\0' ) )
	{
		length = (lstrlen( text )+1)*sizeof( TCHAR );
	}
	else if( *text == _T( '\0' ) )
	{
		length = sizeof( TCHAR );
	}
	auto result = WriteFile( hPipe, text, length, &written, nullptr );
#endif
	if( result )
	{
		_tprintf( _T("Succeeded:WriteFile(): write at %u\n" ), written );
	}
	else
	{
		auto errorCode = GetLastError();
		_tprintf( _T( "Failed: WriteFile(), %u(0x%08X)\n" ), errorCode, errorCode );
	}
	_putts( _T( "End:WriterTask()" ) );
}

static void ReaderTask( HANDLE hPipe )
{
	_putts( _T( "Start:ReaderTask()" ) );
	DWORD readed;
#ifdef __send_int__
	int arr[1024];
	auto result = ReadFile( hPipe, &arr, sizeof( arr ), &readed, nullptr );
	if( result )
	{
		_tprintf( _T( "Succeeded:ReadFile(): read at %u\n" ), readed );
		for( DWORD index = 0 ; index < readed/sizeof( int ) ; index++ )
		{
			_tprintf( _T( "%d, " ), arr[index] );
		}
		_putts( _T( "" ) );
	}
	else
	{
		auto errorCode = GetLastError();
		_tprintf( _T( "Failed: ReadFile(), %u(0x%08X)\n" ), errorCode, errorCode );
	}
#else
	TCHAR msg[4096];
	TCHAR* prev = nullptr;
	TCHAR* curr = msg;

	for(;;)
	{
		auto result = ReadFile( hPipe, curr, sizeof( TCHAR ), &readed, nullptr );
		if( !result )
		{
			auto errorCode = GetLastError();
			if( errorCode != ERROR_MORE_DATA )
			{
				_tprintf( _T( "Failed: ReadFile(), %u(0x%08X)\n" ), errorCode, errorCode );
				break;	//	もう終わり
			}
		}
		if( prev != nullptr && *prev == _T('\0') && *prev == *curr )
		{
			break;	// \0 が2つ並んだら終わり。
		}
		prev = curr;
		if( *curr == _T( '\0' ) )
		{
			_tprintf( _T( "Succeeded:ReadFile():%s\n" ), msg );
			curr = msg;
		}
		else
		{
			curr++;
			if( curr-msg >= 4096-sizeof( TCHAR ) )
			{
				*curr = _T( '\0' );
				_tprintf( _T( "Succeeded:ReadFile():%s\n" ), msg );	//	バッファフルになるので切る
				curr = msg;
			}
		}
	}
	//_tprintf( _T( "Succeeded:ReadFile():%s\n" ), msg );
	//auto result = ReadFile( hPipe, msg, sizeof( msg ), &readed, nullptr );
	//if( result )
	//{
	//	_tprintf( _T( "Succeeded:ReadFile(): read at %u\n%s\n" ), readed, msg );
	//}
	//else
	//{
	//	auto errorCode = GetLastError();
	//	_tprintf( _T( "Failed: ReadFile(), %u(0x%08X)\n" ), errorCode, errorCode );
	//}
#endif
	_putts( _T( "End:ReaderTask()" ) );
}

int _tmain( int argc, TCHAR* argv[] )
{
	_tsetlocale( LC_ALL, _T("") );
	HANDLE	hRead, hWrite;
	//	hWrite に書き込んだ内容が hRead で読める。相互にストリーム構成を決めておけば、やり取りは任意に行える
	auto result = CreatePipe( &hRead, &hWrite, nullptr, 4096 );
	if( !result )
	{
		auto errorCode = GetLastError();
		_tprintf( _T( "Failed: CreatePipe(), %u(0x%08X)\n" ), errorCode, errorCode );
		return errorCode;
	}
#ifdef __async_read__
	//	読み取り処理を非同期にする(作成側から送信のイメージ)
	auto taskRead = concurrency::create_task( [hRead]() { ReaderTask( hRead );  } );
	if( argc > 1 )
	{
		for( int index = 1 ; index < argc ; index++ )
		{
			WriterTask( hWrite, argv[index] );
		}
	}
	else
	{
		WriterTask( hWrite );
	}
	WriterTask( hWrite, _T( "" ) );
	taskRead.wait();
#else
	//	書き込みを非同期に処理する(読み込みが終わって始めて処理が終わるので、書き込みの終了待機は不要)
	concurrency::create_task( [argc, argv, hWrite]()
	{
		if( argc > 1 )
		{
			for( int index = 1 ; index < argc ; index++ )
			{
				WriterTask( hWrite, argv[index] );
			}
		}
		else
		{
			WriterTask( hWrite );
		}
		WriterTask( hWrite, _T( "" ) );
	} );
	ReaderTask( hRead );
#endif
	//auto taskWrite = concurrency::create_task( [hWrite]() { WriterTask( hWrite ); } );
	//ReaderTask( hRead );
	//WriterTask( hWrite );

	//taskWrite.wait();
	//auto writerTask = concurrency::create_task( [hWrite]() { WriterTask( hWrite ); } );
	//writerTask.then( [hRead]() { ReaderTask( hRead ); } );
	//writerTask.wait();
	CloseHandle( hRead );
	CloseHandle( hWrite );
	return 0;
}
