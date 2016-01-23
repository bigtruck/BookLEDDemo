#pragma once

#define MAX_GET_BUFF	(1024*10)
#define MAX_SEND_BUFF	(1024*10)

// SerialCommunicationThread

class SerialCommunicationThread : public CWinThread
{
	DECLARE_DYNCREATE(SerialCommunicationThread)

protected:
	SerialCommunicationThread();           // 动态创建所使用的受保护的构造函数
	virtual ~SerialCommunicationThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	CSemaphore *m_pSemaphore;
	OVERLAPPED osRead;
	OVERLAPPED osWrite;
	OVERLAPPED ShareEvent;

	UCHAR m_GetBuff[MAX_GET_BUFF];
	UINT32 m_GetIndex;
	UINT32 m_ReadIndex;

public:
	virtual int Run();
	virtual BOOL OnIdle(LONG lCount);
	BOOL ConnectSerialPort(CString portName);
	BOOL CloseSerialPort(void);
	BOOL WriteData(PBYTE pOutData, int nDataLen);
	DWORD ReadData(PBYTE pInData);
	UINT32 UART_Count(void);
	UCHAR UART_GetData(void);
	void ClearBuffer(void);

	HANDLE COMFile;
};


