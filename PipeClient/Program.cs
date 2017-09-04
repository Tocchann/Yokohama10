using Microsoft.VisualBasic.FileIO;
using System;
using System.IO;
using System.IO.Pipes;
using System.Text;

class PipeClient
{
	static void Main( string[] args )
	{
		//\\\\.\\pipe\\Wankuma\\Yokohama\\10\\Pipe
		using( var pipeClient = new NamedPipeClientStream( ".", @"Wankuma\Yokohama\10\Pipe", PipeDirection.InOut ) )
		{

			// Connect to the pipe or wait until the pipe is available.
			Console.Write( "Attempting to connect to pipe..." );
			pipeClient.Connect();

			Console.WriteLine( "Connected to pipe." );
			Console.WriteLine( "There are currently {0} pipe server instances open.", pipeClient.NumberOfServerInstances );
			var encoder = new UnicodeEncoding();

			

			//	mynamedpipe は、TCHAR(wchar_t)の文字列を想定してデータの受け渡しを行う。
			var sendMsg = (args.Length > 0) ? args[0] : ".NET 版クライアントのデフォルトデータ";
			sendMsg += '\0';    //	サーバー側がデリミタ込みを期待しているためデリミタを追加する
			var writeBuf = encoder.GetBytes( sendMsg );

			Console.WriteLine( $"送信バイト数:{writeBuf.Length}\n内容:{sendMsg}");
			pipeClient.Write( writeBuf, 0, writeBuf.Length );
			Console.WriteLine( "\nMessage sent to server, receiving reply as follows:" );

			var readBuf = new byte[512*2];
			int readed = pipeClient.Read( readBuf, 0, readBuf.Length );
			var msg = encoder.GetString( readBuf, 0, readed );
			Console.WriteLine( $"\"{msg}\"" );

			Console.Write( "Press Enter to continue..." );
			Console.ReadLine();
		}
	}
}
