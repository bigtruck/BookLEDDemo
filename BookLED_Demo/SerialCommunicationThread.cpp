// SerialCommunicationThread.cpp : 实现文件
//

#include "stdafx.h"
#include "BookLED_Demo.h"
#include "SerialCommunicationThread.h"


// SerialCommunicationThread

IMPLEMENT_DYNCREATE(SerialCommunicationThread, CWinThread)

SerialCommunicationThread::SerialCommunicationThread()
{
	memset(&osRead, 0, sizeof(osRead));
	memset(&osWrite, 0, sizeof(osWrite));
	memset(&ShareEvent, 0, sizeof(ShareEvent));
	COMFile = NULL;

	osRead.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (osRead.hEvent == NULL) {
		AfxMessageBox(_T("建立事件失败!"));
	}

	osWrite.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (osWrite.hEvent == NULL) {
		AfxMessageBox(_T("建立事件失败!"));
	}

	ShareEvent.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (ShareEvent.hEvent == NULL) {
		AfxMessageBox(_T("建立事件失败!"));
	}

	//semapObj.~CSemaphore(1, 3);
	m_pSemaphore = new CSemaphore(0, 2);
}

SerialCommunicationThread::~SerialCommunicationThread()
{
	delete m_pSemaphore;
}

BOOL SerialCommunicationThread::InitInstance()
{
	// TODO:    在此执行任意逐线程初始化
	
	return TRUE;
}

int SerialCommunicationThread::ExitInstance()
{
	// TODO:    在此执行任意逐线程清理

	CloseHandle(osRead.hEvent);
	memset(&osRead, 0, sizeof(osRead));;
	CloseHandle(osWrite.hEvent);
	memset(&osWrite, 0, sizeof(osWrite));
	CloseHandle(ShareEvent.hEvent);
	memset(&ShareEvent, 0, sizeof(ShareEvent));

	

	return CWinThread::ExitInstance();
}

int SerialCommunicationThread::Run()
{
	while (1)
	{
		/*
		DWORD nResutl = WaitForMultipleObjectsEx(1, &osRead.hEvent, FALSE, 1, TRUE);
		if (1 == nResutl)
		{
			TRACE("\r\nwat\r\n");
		}
		*/

		DWORD dwEvtMask = 0;
		WaitCommEvent(COMFile, &dwEvtMask, &ShareEvent);//等待串口事件
		if ((dwEvtMask & EV_RXCHAR) == EV_RXCHAR)
		{
			//TRACE("\r\naa\r\n");
			DWORD  dwLen;
			dwLen = ReadData(m_GetBuff + m_GetIndex);
			m_GetIndex += (UINT32)dwLen;
			if (m_GetIndex >= MAX_GET_BUFF)
			{
				m_GetIndex -= MAX_GET_BUFF;
			}
			//TRACE("\r\nreaddata:%d\r\n",dwLen);
		}

	}
	//子线程结束
	CloseHandle(osRead.hEvent);
	CloseHandle(osWrite.hEvent);
	CloseHandle(ShareEvent.hEvent);

	return 0;
}

BOOL SerialCommunicationThread::OnIdle(LONG lCount)
{
	return 0;
}

BOOL SerialCommunicationThread::ConnectSerialPort(CString portName)
{
	DCB dcb;
	BOOL fRetVal;
	COMMTIMEOUTS CommTimeOuts;
	CString szCom;
	szCom = L"\\\\.\\" + portName;
	//COMFile = CreateFile(szCom.GetBuffer(50), GENERIC_READ | GENERIC_WRITE,//
	COMFile = CreateFile(szCom, GENERIC_READ | GENERIC_WRITE,//
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_OVERLAPPED,
		NULL);
	if (INVALID_HANDLE_VALUE == COMFile) {
		return (FALSE);
	}

	SetupComm(COMFile, 6000, 6000);
	SetCommMask(/*COMFileTemp*/COMFile, EV_RXCHAR);
	CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF;
	CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
	CommTimeOuts.ReadTotalTimeoutConstant = 1000;
	CommTimeOuts.WriteTotalTimeoutMultiplier = 2 * CBR_115200 / 115200;
	CommTimeOuts.WriteTotalTimeoutConstant = 0;
	SetCommTimeouts(/*COMFileTemp*/COMFile, &CommTimeOuts);

	dcb.DCBlength = sizeof(DCB);
	GetCommState(COMFile, &dcb);
	dcb.BaudRate = CBR_115200;
	dcb.StopBits = ONESTOPBIT;
	dcb.Parity = NOPARITY;
	dcb.ByteSize = 8;
	dcb.fBinary = TRUE;
	dcb.fOutxDsrFlow = 0;
	dcb.fDtrControl = DTR_CONTROL_ENABLE;
	dcb.fOutxCtsFlow = 0;
	dcb.fRtsControl = RTS_CONTROL_ENABLE;
	dcb.fInX = dcb.fOutX = 1;
	dcb.XonChar = 0X11;
	dcb.XoffChar = 0X13;
	dcb.XonLim = 100;
	dcb.XoffLim = 100;
	dcb.fParity = TRUE;

	fRetVal = SetCommState(/*COMFileTemp*/COMFile, &dcb);

	if (!fRetVal)
	{
		return FALSE;
	}

	PurgeComm( /*COMFileTemp*/COMFile, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	EscapeCommFunction( /*COMFileTemp*/COMFile, SETDTR);


	return TRUE;
}

BOOL SerialCommunicationThread::CloseSerialPort(void)
{
	//禁止串行端口所有事件
	SetCommMask(COMFile, 0);
	//清除数据终端就绪信号
	EscapeCommFunction(COMFile, CLRDTR);
	//丢弃通信资源的输出或输入缓冲区字符并终止在通信资源上挂起的读、写操操作
	PurgeComm(COMFile, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	CloseHandle(COMFile);
	COMFile = NULL;

	return TRUE;
}

BOOL SerialCommunicationThread::WriteData(PBYTE pOutData, int nDataLen)
{
	if (pOutData == NULL || nDataLen == 0)
	{
		return FALSE;
	}

	DWORD dwLen;

	if (!WriteFile(COMFile, pOutData, nDataLen, &dwLen, &osWrite))
	{
		if (WAIT_OBJECT_0 == WaitForSingleObject(osWrite.hEvent, 0xFFFFFF))
			ResetEvent(osWrite.hEvent);
		else
			ResetEvent(osWrite.hEvent);

		//DWORD nError = GetLastError();
		//if (997 != nError) {//997异步没完成
		//	char chError[256]; memset(chError, 0, 256);
		//	int nBuffLen = 256;
		//	if (GetErrorString((PBYTE)chError, &nBuffLen, nError)) {
		//		AfxMessageBox(chError);
		//	}
		//}
	}
	else
	{
		TRACE("\r\nwrite error\r\n");
	}

	return TRUE;
}

DWORD SerialCommunicationThread::ReadData(PBYTE pInData)
{
	DWORD dwErrorFlags;
	COMSTAT comStat;
	DWORD dwLen,dwOutBytes;

	ClearCommError(COMFile, &dwErrorFlags, &comStat);
	dwLen = comStat.cbInQue;
	if (dwLen > MAX_GET_BUFF)
	{
		dwLen = MAX_GET_BUFF;
	}
	if (dwLen > 0)
	{
		ReadFile(COMFile, pInData, dwLen, &dwOutBytes, &osRead);
	}

	return dwLen;
}

UINT32 SerialCommunicationThread::UART_Count(void)
{
	UINT32 len = 0;

//	CSingleLock singleLock(m_pSemaphore);
//	singleLock.Lock();
	if (m_GetIndex >= m_ReadIndex)
	{
		len = m_GetIndex - m_ReadIndex;
	}
	else
	{
		len = MAX_GET_BUFF - m_ReadIndex + m_GetIndex;
	}
//	singleLock.Unlock();
	return len;
}

UCHAR SerialCommunicationThread::UART_GetData(void)
{
	UCHAR	ch;

	ch = m_GetBuff[m_ReadIndex];
	if (++m_ReadIndex >= MAX_GET_BUFF)
	{
		m_ReadIndex = 0;
	}
	return ch;
}

void SerialCommunicationThread::ClearBuffer(void)
{
	m_GetIndex = 0;
	m_ReadIndex = 0;
}

BEGIN_MESSAGE_MAP(SerialCommunicationThread, CWinThread)
END_MESSAGE_MAP()


// SerialCommunicationThread 消息处理程序
