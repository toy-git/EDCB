#pragma once
#include "../BonCtrl/PacketInit.h"
#include "../BonCtrl/SendUDP.h"
#include "../BonCtrl/SendTCP.h"
#include "../BonCtrl/CreatePATPacket.h"
#include "TSPacketUtil.h"
#include "ThreadUtil.h"

class CTimeShiftUtil
{
public:
	CTimeShiftUtil(void);
	~CTimeShiftUtil(void);

	//UDP/TCP送信を行う
	//戻り値：
	// TRUE（成功）、FALSE（失敗）
	//引数：
	// val		[IN/OUT]送信先情報
	BOOL Send(
		NWPLAY_PLAY_INFO* val
		);

	//タイムシフト用ファイルを開く
	//戻り値：
	// TRUE（成功）、FALSE（失敗）
	//引数：
	// filePath		[IN]タイムシフト用バッファファイルのパス
	// fileMode		[IN]録画済みファイル再生モード
	BOOL OpenTimeShift(
		LPCWSTR filePath_,
		BOOL fileMode_
		);

	//タイムシフト送信を開始する
	//戻り値：
	// TRUE（成功）、FALSE（失敗）
	BOOL StartTimeShift();

	//タイムシフト送信を停止する
	//戻り値：
	// TRUE（成功）、FALSE（失敗）
	BOOL StopTimeShift();

	//現在の送信位置と有効なファイルサイズを取得する
	//引数：
	// filePos		[OUT]ファイル位置
	// fileSize		[OUT]ファイルサイズ
	void GetFilePos(__int64* filePos, __int64* fileSize);

	//送信開始位置を変更する
	//引数：
	// filePos		[IN]ファイル位置
	void SetFilePos(__int64 filePos);

protected:
	recursive_mutex_ utilLock;
	recursive_mutex_ ioLock;
	CSendUDP sendUdp;
	CSendTCP sendTcp;
	wstring sendIP[2];
	HANDLE portMutex[2];

	wstring filePath;
	WORD PCR_PID;

	BOOL fileMode;
	int seekJitter;
	__int64 currentFilePos;

	thread_ readThread;
	atomic_bool_ readStopFlag;
	HANDLE readFile;
	HANDLE seekFile;
protected:
	static void ReadThread(CTimeShiftUtil* sys);
	__int64 GetAvailableFileSize() const;
};

