// NamedPipeServerTask.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"

//	�������s�����^�C���𗘗p�����Ăяo���p�^�[��
//	call ���b�Z�[�W�u���b�N
//#define USE_CALL_SYNC_MODE

//#define USE_CALL_ASYNC_MODE	
//#define USE_TASK
#define USE_LIGHT_THREAD

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

#if defined(USE_CALL_SYNC_MODE) || defined(USE_CALL_ASYNC_MODE)
	concurrency::call<HANDLE>	receiver( []( HANDLE pipe )
	{
		CommunicationToClient( pipe );
	} );
#endif
	for(;;){
		//	�ڑ��ҋ@
		HANDLE pipe = WaitConnectPipe();
		if( pipe == nullptr )
		{
			break;	//	�p�C�v�����Ȃ������炻�̎��_�ŏI��
		}
		//	������T�[�o�[�̓N���C�A���g�^�X�N�ɗ����܂�(�������ςȂ��ł��Ƃ͍l�����Ȃ�)
#if defined(USE_CALL_SYNC_MODE)
		concurrency::send( receiver, pipe );
#elif defined(USE_CALL_ASYNC_MODE)
		concurrency::asend( receiver, pipe );	//	�񓯊��ɌĂяo���Ă���̂����B�B�B
#elif defined(USE_TASK)
		concurrency::create_task( [pipe]() { CommunicationToClient( pipe ); } );
#elif defined(USE_LIGHT_THREAD)
		//	�����͓������ςȂ��Ȃ̂ŁA���̂Ă̌y�ʃ^�X�N�ł悢
		concurrency::CurrentScheduler::ScheduleTask( CommunicationToClient, pipe );
#endif
	}
}
//	���O�t���p�C�v������āA�ڑ��҂�����B
HANDLE WaitConnectPipe()
{
	_putts( _T( "Create and Connecting Pipe." ) );
	HANDLE pipe = CreateNamedPipe(
		pipeName,					//	�p�C�v�̖��O
		PIPE_ACCESS_DUPLEX,			// �A�N�Z�X���[�h(�o����)
		PIPE_TYPE_MESSAGE |			// �������݃��[�h(���b�Z�[�W���[�h�̓��[�h�P��)
		PIPE_READMODE_MESSAGE |		// �ǂݎ�胂�[�h
		PIPE_WAIT,					// �u���b�L���O���[�h(PIPE_NOWAIT�͍��͎g��Ȃ��̂ŕK���u���b�L���O���[�h�ɂ���)
		PIPE_UNLIMITED_INSTANCES,	// �ő�C���X�^���X���͖�����
		bufSize,					// �ǂݎ��p�o�b�t�@�T�C�Y
		bufSize,					// �������ݗp�o�b�t�@�T�C�Y
		0,							// �N���C�A���g��WaitNamedPipe �� NMPWAIT_USE_DEFAULT_WAIT ���w�肵�����̑҂�����
		nullptr );				// �Z�L�����e�B�L�q�q(SECURITY_ATTRIBUTES*)
		
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
			//	CreateNamedPipe�������_�Ńp�C�v�ɂȂ����Ă����ꍇ(�N���C�A���g�������ق�����̏ꍇ)
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
	//	�����ŁA���炩���ߌ��߂Ă������ǂݏ������s����B����́AMSDN�̃T���v���Ɠ����i���
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
