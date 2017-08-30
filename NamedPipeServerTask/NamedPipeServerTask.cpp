// NamedPipeServerTask.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

#include <ppltasks.h>
#include <agents.h>
#include <concrt.h>

#include <list>

const DWORD bufSize = 512;
LPCTSTR pipeName = _T( "\\\\.\\pipe\\Wankuma\\Yokohama\\10\\Pipe" );

HANDLE WaitConnectPipe();
void CommunicationToClient( HANDLE hPipe );
CString GetAnswerToRequest( LPCTSTR readStr );

int main()
{
	_tsetlocale( LC_ALL, _T( "" ) );
	//concurrency::call<HANDLE>	receiver( []( HANDLE pipe )
	//{
	//	//	このメソッドの処理が終了しないと次のタスクを処理できない
	//	CommunicationToClient( pipe );
	//	//concurrency::create_task( [pipe]() { CommunicationToClient( pipe ); } );
	//} );
	for(;;){
		//	接続待機
		HANDLE pipe = WaitConnectPipe();
		if( pipe == nullptr )
		{
			break;	//	パイプが作れなかったらその時点で終了
		}
		//	作ったサーバーはクライアントタスクに流します(流しっぱなしであとは考慮しない)
		//concurrency::send( receiver, pipe );
		//concurrency::asend( receiver, pipe );	//	非同期に呼び出しているのだが。。。
		//	処理は投げっぱなしなので、作り捨ての軽量タスクでよい
		concurrency::CurrentScheduler::ScheduleTask( CommunicationToClient, pipe );
		//concurrency::create_task( [pipe]() { CommunicationToClient( pipe ); } );
	}
}
//	名前付きパイプを作って、接続待ちする。
HANDLE WaitConnectPipe()
{
	_putts( _T( "Create and Connecting Pipe." ) );
	HANDLE pipe = CreateNamedPipe(
		pipeName,					//	パイプの名前
		PIPE_ACCESS_DUPLEX,			// アクセスモード(双方向)
		PIPE_TYPE_MESSAGE |			// 書き込みモード(メッセージモードはワード単位)
		PIPE_READMODE_MESSAGE |		// 読み取りモード
		PIPE_WAIT,					// ブロッキングモード(PIPE_NOWAITは今は使わないので必ずブロッキングモードにする)
		PIPE_UNLIMITED_INSTANCES,	// 最大インスタンス数は無制限
		bufSize,					// 読み取り用バッファサイズ
		bufSize,					// 書き込み用バッファサイズ
		0,							// クライアントがWaitNamedPipe で NMPWAIT_USE_DEFAULT_WAIT を指定した時の待ち時間
		nullptr );				// セキュリティ記述子(SECURITY_ATTRIBUTES*)
		
	if( pipe == INVALID_HANDLE_VALUE )
	{
		_tprintf( _T( "Failed:CreateNamedPipe():%d\n" ), GetLastError() );
	}
	else
	{
		auto pipeConnected = ConnectNamedPipe( pipe, nullptr );
		if( !pipeConnected )
		{
			auto errorCode = GetLastError();
			//	CreateNamedPipeした時点でパイプにつながっていた場合(クライアント側が作るほうが先の場合)
			if( errorCode == ERROR_PIPE_CONNECTED )
			{
				pipeConnected = TRUE;
			}
			else
			{
				_tprintf( _T( "Failed:ConnectNamedPipe():%d\n" ), errorCode );
			}
		}
		if( !pipeConnected )
		{
			CloseHandle( pipe );
			pipe = nullptr;
		}
	}
	if( pipe != nullptr )
	{
		ULONG processId;
		if( !GetNamedPipeClientProcessId( pipe, &processId ) )
		{
			auto errorCode = GetLastError();
			_tprintf( _T( "Failed:GetNamedPipeClientProcessId():%d\n" ), errorCode );
		}
		_tprintf( _T( "Connect Client.  ProcessId=%d\n" ), processId );
	}
	return pipe;
}
void CommunicationToClient( HANDLE hPipe )
{
	//	ここで、あらかじめ決めておいた読み書きが行われる。今回は、MSDNのサンプルと同じ段取り
	TCHAR	readbuf[bufSize];
	if( hPipe == nullptr )
	{
		_putts( _T( "\nError Fail To Create or Connect" ) );
		return;
	}
	CAtlFile	pipe( hPipe );
	_putts( _T( "Receiving and processing messages." ) );

	for(;;)
	{
		DWORD readed;
		auto result = pipe.Read( readbuf, sizeof(readbuf), readed );
		if( FAILED( result ) || readed == 0 )
		{
			if( HRESULT_CODE( result ) == ERROR_BROKEN_PIPE )
			{
				_putts( _T( "Client Disconnected." ) );
			}
			else
			{
				_tprintf( _T( "Fail:ReadFile():%d\n" ), HRESULT_CODE( result ) );
			}
			break;
		}
		DWORD written;
		auto resultStr = GetAnswerToRequest( readbuf );
		LPCTSTR msg = resultStr;
		DWORD length = (resultStr.GetLength()+1) * sizeof(TCHAR);
		result = pipe.Write( msg, length, &written );
		if( FAILED( result ) || length != written )
		{
			_tprintf( _T( "Fail:WriteFile():%d\n" ), HRESULT_CODE( result ) );
			break;
		}
	}
	pipe.Flush();
	DisconnectNamedPipe( pipe );

	_putts( _T( "Exitting Task." ) );
}
CString GetAnswerToRequest( LPCTSTR readStr )
{
	_tprintf( TEXT( "Client Request String:\"%s\"\n" ), readStr );

	CString result;
	result.Format( _T( "Received From \"%s\"" ), readStr );
	return result;
}
