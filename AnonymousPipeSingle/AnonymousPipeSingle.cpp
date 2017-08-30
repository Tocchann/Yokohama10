// AnonymousPipeSingle.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <locale>

#define __send_int__

static void WriterTask( HANDLE hPipe )
{
	_putts( _T( "Start:WriterTask()" ) );
	DWORD written;
#ifdef __send_int__
	int arr[] ={ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	auto result = WriteFile( hPipe, arr, sizeof( arr ), &written, nullptr );
#else
	const TCHAR msg[] = _T("日本語を含む文字列を表現しておくよ\0");
	auto result = WriteFile( hPipe, msg, sizeof(msg), &written, nullptr );
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
	TCHAR msg[1024];
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
		curr++;
	}
	_tprintf( _T( "Succeeded:ReadFile():%s\n" ), msg );
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

int main()
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
	auto taskRead = concurrency::create_task( [hRead]() { ReaderTask( hRead );  } );
	WriterTask( hWrite );
	//auto taskWrite = concurrency::create_task( [hWrite]() { WriterTask( hWrite ); } );
	//ReaderTask( hRead );
	//WriterTask( hWrite );
	taskRead.wait();
	//taskWrite.wait();
	//auto writerTask = concurrency::create_task( [hWrite]() { WriterTask( hWrite ); } );
	//writerTask.then( [hRead]() { ReaderTask( hRead ); } );
	//writerTask.wait();
	CloseHandle( hRead );
	CloseHandle( hWrite );
	return 0;
}
