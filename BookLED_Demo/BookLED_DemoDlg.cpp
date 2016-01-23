
// BookLED_DemoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BookLED_Demo.h"
#include "BookLED_DemoDlg.h"
#include "afxdialogex.h"
#include <Windows.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define TID_TIMER1	1
#define TID_TIMER2	2


/*�����ʽ
PC��������ָ����յ�����Ӧ�𣬵�һ����������Ӧ�𣬵ڶ�����ִ�зֻ���Ӧ��
//3�ŷֻ��ĵ�5�������
81 00 03 05 44
|  |  |  |  |___Ч���ֽڣ�(Ч����0x3B) + 0x80 + 0x00 + 0x03 + 0x05  = 0xC4,����Ҫ & 0x7F,���Ծͱ�� 0x44
|  |  |  |______1���ֽڵĵ���ź�����ָʾ,B0~B5Ϊ�����,B6Ϊ����ָʾ,B7����Ч�ģ���ΪҪ & 0x7F
|  |  |_________2���ֽڵķֻ��ţ�ֻ��14λ����Ч�ģ���Ϊ����Ҫ & 0x7F
|  |____________
|_______________����0,�������Ҫ����0x80

//3�ŷֻ��ĵ�5������
81 00 03 45 04

//2�ŷֻ���ȫ������
80 00 02 00 3D

//2�ŷֻ���ȫ������
80 00 02 01 3E

//500�ŷֻ���40�ŵ���
81 74 03 68 1B
500--->0x1F4 ;
40---->0x28 ;
bffer[0] = 1 | 0x80 ;
bffer[1] = 0x1F4 & 0x7F ;
bffer[2] = (0x1F4 >> 7) & 0x7F ;
bffer[3] = 0x28 & 0x7F ;
bffer[3] = 0x40 ;//����
bffer[4] = 1B ;

*/

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CBookLED_DemoDlg �Ի���



CBookLED_DemoDlg::CBookLED_DemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBookLED_DemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBookLED_DemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_OPENCOM, m_Button_OpenCOM);
	DDX_Control(pDX, IDC_COMBO_PORTLIST, m_ComboBox_PortList);
	DDX_Control(pDX, IDC_ED_LEDNUM, m_EdLEDNum);
	DDX_Control(pDX, IDC_ED_MACNO, m_EdMACNum);
	DDX_Control(pDX, IDC_ED_LEDNUMBERS, m_EdAssignNumber);
	DDX_Control(pDX, IDC_STA_ERROR, m_StaEdError);
	DDX_Control(pDX, IDC_BU_OPEN, m_BuOpenBit);
	DDX_Control(pDX, IDC_BU_CLOSE, m_BuCloseBit);
	DDX_Control(pDX, IDC_BU_OPENALL, m_BuOpenAll);
	DDX_Control(pDX, IDC_BU_CLOSEALL, m_BuCloseAll);
	DDX_Control(pDX, IDC_BU_ASSIGNOPEN, m_BuOpenAssign);
	DDX_Control(pDX, IDC_BU_ASSIGNCLOSE, m_BuCloseAssign);
}

BEGIN_MESSAGE_MAP(CBookLED_DemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPENCOM, &CBookLED_DemoDlg::OnBnClickedOpenCommonPort)
	ON_CBN_SELCHANGE(IDC_COMBO_PORTLIST, &CBookLED_DemoDlg::OnCbnSelchangeComboPortlist)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BU_OPEN, &CBookLED_DemoDlg::OnBnClickedBuOpen)
	ON_BN_CLICKED(IDC_BU_CLOSE, &CBookLED_DemoDlg::OnBnClickedBuClose)
	ON_BN_CLICKED(IDC_BU_OPENALL, &CBookLED_DemoDlg::OnBnClickedBuOpenall)
	ON_BN_CLICKED(IDC_BU_CLOSEALL, &CBookLED_DemoDlg::OnBnClickedBuCloseall)
	ON_BN_CLICKED(IDC_BU_ASSIGNOPEN, &CBookLED_DemoDlg::OnBnClickedBuAssignopen)
	ON_BN_CLICKED(IDC_BU_ASSIGNCLOSE, &CBookLED_DemoDlg::OnBnClickedBuAssignclose)
END_MESSAGE_MAP()


// CBookLED_DemoDlg ��Ϣ�������

BOOL CBookLED_DemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_Button_OpenCOM.SetWindowTextW(L"��") ;
	m_EdLEDNum.SetWindowTextW(L"0");
	m_EdMACNum.SetWindowTextW(L"0");
	m_EdAssignNumber.SetWindowTextW(L"0");
	ScanCommonPort(&m_ComboBox_PortList) ;
	m_ComboBox_PortList.SetCurSel(0) ;
	if (m_ComboBox_PortList.GetCount()==0)
	{
		MessageBox(L"����û�д���", L"����", MB_OK | MB_ICONWARNING);
	}
	portOpen = FALSE ;
	m_hSerialThread = (SerialCommunicationThread *)AfxBeginThread(RUNTIME_CLASS(SerialCommunicationThread), 0, 0, CREATE_SUSPENDED);
	m_ThreadDataStruct.hSerial = m_hSerialThread;
	m_ThreadDataStruct.ackCount = 0;
	m_ThreadDataStruct.error = 0;
	m_ThreadDataStruct.timeout = 0;
	m_pThreadMain = AfxBeginThread((AFX_THREADPROC)BookLEDDemo_Main, &m_ThreadDataStruct, THREAD_PRIORITY_NORMAL,0, CREATE_SUSPENDED, NULL);
	//m_pThreadMain->ResumeThread();
	m_BuOpenBit.EnableWindow(FALSE);
	m_BuCloseBit.EnableWindow(FALSE);
	m_BuOpenAll.EnableWindow(FALSE);
	m_BuCloseAll.EnableWindow(FALSE);
	m_BuOpenAssign.EnableWindow(FALSE);
	m_BuCloseAssign.EnableWindow(FALSE);


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CBookLED_DemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CBookLED_DemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CBookLED_DemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CBookLED_DemoDlg::ScanCommonPort(CComboBox *combobox)
{
	#define MAX_KEY_LENGTH   255
	#define MAX_VALUE_NAME  16383
	HKEY hTestKey;
	if(ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"), 0, KEY_READ, &hTestKey) )
	{
		TCHAR   achClass[MAX_PATH] = TEXT(""); // buffer for class name
		DWORD   cchClassName = MAX_PATH; // size of class string
		DWORD   cSubKeys=0;                  // number of subkeys
		DWORD   cbMaxSubKey;             // longest subkey size
		DWORD   cchMaxClass;             // longest class string
		DWORD   cValues;             // number of values for key
		DWORD   cchMaxValue;         // longest value name
		DWORD   cbMaxValueData;      // longest value data
		DWORD   cbSecurityDescriptor; // size of security descriptor
		FILETIME ftLastWriteTime;     // last write time
 
		DWORD i, retCode;
		//TCHAR achValue[MAX_VALUE_NAME];
		CHAR achValue[MAX_VALUE_NAME];
		DWORD cchValue = MAX_VALUE_NAME;
		LONG lResult ;
 
		// Get the class name and the value count.
		retCode = RegQueryInfoKey(
					hTestKey,                   // key handle
					achClass,               // buffer for class name
					&cchClassName,          // size of class string
					NULL,                   // reserved
					&cSubKeys,              // number of subkeys
					&cbMaxSubKey,           // longest subkey size
					&cchMaxClass,           // longest class string
					&cValues,               // number of values for this key
					&cchMaxValue,           // longest value name
					&cbMaxValueData,        // longest value data
					&cbSecurityDescriptor,  // security descriptor
					&ftLastWriteTime);      // last write time
		if (cValues > 0)
		{
			int nIndex = -1;
			for (i=0; i<cValues; i++)
			{
				cchValue = MAX_VALUE_NAME; 
				achValue[0] = '\0';
				lResult = RegEnumValueA(hTestKey, i, achValue, &cchValue, NULL, NULL, NULL, NULL) ;
				if (ERROR_SUCCESS == lResult)
				{
					BYTE strDSName[50];
					//memset(strDSName, 0, 100);
					strDSName[0] = '\n' ;
					DWORD  nBuffLen = 50;
					//if (ERROR_SUCCESS == RegQueryValueEx(hTestKey, (LPCTSTR)achValue, NULL,&nValueType, strDSName, &nBuffLen))
					if (ERROR_SUCCESS == RegQueryValueExA(hTestKey, achValue, NULL,NULL, strDSName, &nBuffLen))
					{	
						CString str(strDSName) ;
						combobox->AddString(str) ;
					}
				}
				else
				{
					TRACE("error\r\n") ;
				}
			}
		}
		else
		{
			//AfxMessageBox(_T("PC��û��COM��....."));
		}
	}
	RegCloseKey(hTestKey);
}


void CBookLED_DemoDlg::OnBnClickedOpenCommonPort()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if ( portOpen == FALSE )
	{
		CString str;
		WCHAR text_buff[255];
		m_ComboBox_PortList.GetLBText(m_ComboBox_PortList.GetCurSel(), text_buff);
		str = text_buff;
		//if ( OpenCommonPort(str) == FALSE )
		if ( m_hSerialThread->ConnectSerialPort(str) == FALSE )
		{
			MessageBox(L"�򿪴���ʧ��",L"",MB_OK | MB_ICONERROR) ;
		}
		else
		{
			//MessageBox(L"�򿪴��ڳɹ�",L"",MB_OK) ;
			PortOpenDeal();
			//m_Button_OpenCOM.SetWindowTextW(L"�ر�") ;
			//portOpen = TRUE ;
			//m_hSerialThread->ClearBuffer();
			//m_hSerialThread->ResumeThread();
			//m_pThreadMain->ResumeThread();
			//m_TimerID = SetTimer(TID_TIMER1, 1, 0);
		}
	}
	else
	{
		PortCloseDeal();
		//KillTimer(m_TimerID);
		//m_pThreadMain->SuspendThread();
		//m_hSerialThread->CloseSerialPort();
		//m_hSerialThread->SuspendThread();
		//portOpen = FALSE;
		//m_Button_OpenCOM.SetWindowTextW(L"��");
	}
}


void CBookLED_DemoDlg::OnCbnSelchangeComboPortlist()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//m_hSerialThread->SuspendThread();
	//m_hSerialThread->CloseSerialPort();
	//portOpen = FALSE;
	//m_Button_OpenCOM.SetWindowTextW(L"��");
	PortCloseDeal();
}
void CBookLED_DemoDlg::PortOpenDeal(void)
{
	m_BuOpenBit.EnableWindow(TRUE);
	m_BuCloseBit.EnableWindow(TRUE);
	m_BuOpenAll.EnableWindow(TRUE);
	m_BuCloseAll.EnableWindow(TRUE);
	m_BuOpenAssign.EnableWindow(TRUE);
	m_BuCloseAssign.EnableWindow(TRUE);
	m_Button_OpenCOM.SetWindowTextW(L"�ر�");
	portOpen = TRUE;
	m_hSerialThread->ClearBuffer();
	m_hSerialThread->ResumeThread();
	m_pThreadMain->ResumeThread();
	m_TimerID = 0;
	m_TimerID = SetTimer(TID_TIMER1, 1, 0);

}

void CBookLED_DemoDlg::PortCloseDeal(void)
{
	m_BuOpenBit.EnableWindow(FALSE);
	m_BuCloseBit.EnableWindow(FALSE);
	m_BuOpenAll.EnableWindow(FALSE);
	m_BuCloseAll.EnableWindow(FALSE);
	m_BuOpenAssign.EnableWindow(FALSE);
	m_BuCloseAssign.EnableWindow(FALSE);
	if (m_TimerID != 0)
	{
		KillTimer(m_TimerID);
	}
	m_pThreadMain->SuspendThread();
	m_hSerialThread->CloseSerialPort();
	m_hSerialThread->SuspendThread();
	portOpen = FALSE;
	m_Button_OpenCOM.SetWindowTextW(L"��");

}





void CBookLED_DemoDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//TRACE("0.5s\r\n");
	if (m_ThreadDataStruct.timeout)
	{
		m_ThreadDataStruct.timeout++;
		if (m_ThreadDataStruct.ackCount == 0)//û���յ�������Ӧ��
		{
			if (m_ThreadDataStruct.timeout >= 100)
			{
				TRACE("main no ack\r\n");
				m_ThreadDataStruct.error = ERR_MAIN_TIMEOUT;
				m_ThreadDataStruct.timeout = 0;
			}
		}
		else if (m_ThreadDataStruct.ackCount == 1)//�յ�������Ӧ��,��û�յ�ִ�зֻ���Ӧ��
		{
			if (m_ThreadDataStruct.timeout >= 300)
			{
				TRACE("ext no ack\r\n");
				m_ThreadDataStruct.error = ERR_EXT_TIMEOUT;
				m_ThreadDataStruct.timeout = 0;
			}
		}
		else
		{
			m_ThreadDataStruct.timeout = 0;
		}
	}
	
	CDialogEx::OnTimer(nIDEvent);
}


const unsigned char	c_Parm0[] = {
	3 ,	//0
	3 ,	//1
	3 ,	//2
};

#define	GETSIZE			1024
#define	VERIFY_CODE0	0x3B
#define	OUTBUFSIZE0		56

#define CMD0_SETALL     0
#define CMD0_SETBIT     1
#define CMD0_ACK        2

UINT CBookLED_DemoDlg::BookLEDDemo_Main(LPVOID lParam)
{
	//AfxMessageBox(L"led");
	unsigned char	c1, c2, c3, c4, c5;
	unsigned int	macNo;
	unsigned char	err;
	unsigned char	CmdIn;
	unsigned char	CmdBuf[512];

	__ThreadMainData *pStruct = (__ThreadMainData*)lParam;
	SerialCommunicationThread *hSerial = pStruct->hSerial ;
	
	TRACE("main thread run...\r\n");
	CmdIn = 0;
	while (hSerial)
	{
		while (hSerial->UART_Count())//����Ƿ����յ���������
		{
			//�����ݣ�����
			c1 = hSerial->UART_GetData();//��һ���ֽ�
			if ((c1 & 0x80) > 0)
			{
				c1 &= 0x7F;
				CmdIn = 0;
				if (c1 < sizeof(c_Parm0))
				{
					CmdBuf[0] = c1;
					CmdIn = 1;
				}
			}
			else
			{
				if (CmdIn > 0)
				{
					CmdBuf[CmdIn] = c1;
					c2 = CmdBuf[0];
					c3 = c_Parm0[c2] + 2;
					if (++CmdIn >= c3)
					{
						CmdIn = 0;
						c3--;
						c4 = VERIFY_CODE0;
						for (c5 = 0; c5 < c3; c5++)
						{
							c4 += CmdBuf[c5];
						}
						if ((c4 & 0x7F) == CmdBuf[c5])
						{
							//У����ȷ
							switch (c2)
							{
							case CMD0_ACK:
								macNo = CmdBuf[2] & 0x7F;
								macNo |= CmdBuf[1] & 0x7F;
								err = CmdBuf[3] & 0x7F;
								pStruct->ackCount++;
								if (pStruct->ackCount == 1)//��һ������������Ӧ
								{
									pStruct->timeout = 1;//��Ϊ1�����¿�ʼ����
									TRACE("main ack\r\n");
								}
								else if (pStruct->ackCount == 2)//�ڶ�����ִ�зֻ���Ӧ��
								{
									pStruct->timeout = 0;
									if (err == ERR_NOACK)
									{
										//pStruct->error = ERR_NOACK;
										TRACE("mac no ack\r\n");
										CString str;
										str.Format(L"�ֻ�[%d]��Ӧ��", macNo);
										AfxMessageBox(str);
									}
									else
									{
										TRACE("ok\r\n");
									}
									pStruct->error = ERR_NONE;
									
								}
								break;
							}
						}
					}
				}
			}
		}
		if (pStruct->error != ERR_NONE)
		{
			if (pStruct->error == ERR_MAIN_TIMEOUT)
			{
				AfxMessageBox(L"����Ӧ��ʱ",MB_OK | MB_ICONWARNING);
			}
			else if (pStruct->error == ERR_EXT_TIMEOUT)
			{
				AfxMessageBox(L"�ֻ�Ӧ��ʱ", MB_OK | MB_ICONWARNING);
			}
			pStruct->error = ERR_NONE;
		}
		Sleep(2);
	}
	TRACE("main thread over\r\n");
	AfxEndThread(0);
	return 0;
}

UINT CBookLED_DemoDlg::CtrlAssignLED(LPVOID lParam)
{
	UINT num, macNo;
	UCHAR sendBuff[10], c1, c2, ledNo;
	__ThreadMainData *pStruct = (__ThreadMainData*)lParam;
	SerialCommunicationThread *hSerial = pStruct->hSerial;
	for (UINT i = 0; i < pStruct->ledMax; i++)
	{
		num = pStruct->ledNumbers[i];
		macNo = num / 44;
		ledNo = num % 44;
		sendBuff[0] = CMD0_SETBIT | 0x80;
		sendBuff[1] = macNo & 0x7F;
		sendBuff[2] = (macNo >> 7) & 0x7F;
		sendBuff[3] = ledNo & 0x3F;
		if (pStruct->level)
		{
			sendBuff[3] |= 0x40;
		}
		c1 = VERIFY_CODE0;
		for (c2 = 0; c2 < c_Parm0[CMD0_SETBIT] + 1; c2++)
		{
			c1 += sendBuff[c2];
		}
		sendBuff[c2] = c1 & 0x7F;

		hSerial->WriteData(sendBuff, c_Parm0[CMD0_SETBIT] + 2);

		pStruct->ackCount = 0;
		pStruct->error = ERR_NONE;
		pStruct->timeout = 1;
		while (pStruct->timeout >1)
		{
			if (pStruct->error != ERR_NONE)
			{
				break;
			}
		}
		if (pStruct->error != ERR_NONE)
		{
			AfxMessageBox(L"error");
			break;
		}
		Sleep(5);
	}
	AfxEndThread(0);
	return 0;
}



//UCHAR	testBuff[] = "Hello\r\n";


//�����Ʋ���
void CBookLED_DemoDlg::OnBnClickedBuOpen()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	UINT num, macNo;
	UCHAR sendBuff[10],c1,c2,ledNo;
	WCHAR textBuff[255];

	m_EdLEDNum.GetWindowTextW(textBuff, m_EdLEDNum.GetWindowTextLengthW()+1);
	num = _wtoi(textBuff);//�õ��ƺ�
	macNo = num / 44;//�õ��ֻ���
	ledNo = num % 44;//�õ��ֻ��ϵ�LED���
	sendBuff[0] = CMD0_SETBIT | 0x80;//���� | 0x80
	sendBuff[1] = macNo & 0x7F;	//2���ֽڵķֻ��Ų���
	sendBuff[2] = (macNo >> 7) & 0x7F;
	sendBuff[3] = ledNo & 0x3F;//һ���ֽڵĵ����,B0~B5��Ч
	sendBuff[3] |= 0x40; //B6����\���־
	c1 = VERIFY_CODE0;
	for (c2 = 0; c2 < c_Parm0[CMD0_SETBIT] + 1; c2++)
	{
		c1 += sendBuff[c2];
	}
	sendBuff[c2] = c1 & 0x7F;

	m_hSerialThread->WriteData(sendBuff, c_Parm0[CMD0_SETBIT] + 2);

	m_ThreadDataStruct.timeout = 1;
	m_ThreadDataStruct.ackCount = 0;

}


void CBookLED_DemoDlg::OnBnClickedBuClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UINT num, macNo;
	UCHAR sendBuff[10], c1, c2, ledNo;
	WCHAR textBuff[255];

	m_EdLEDNum.GetWindowTextW(textBuff, m_EdLEDNum.GetWindowTextLengthW() + 1);
	num = _wtoi(textBuff);
	macNo = num / 44;
	ledNo = num % 44;
	sendBuff[0] = CMD0_SETBIT | 0x80;
	sendBuff[1] = macNo & 0x7F;
	sendBuff[2] = (macNo >> 7) & 0x7F;
	sendBuff[3] = ledNo & 0x3F;
	sendBuff[3] |= 0x00;
	c1 = VERIFY_CODE0;
	for (c2 = 0; c2 < c_Parm0[CMD0_SETBIT] + 1; c2++)
	{
		c1 += sendBuff[c2];
	}
	sendBuff[c2] = c1 & 0x7F;

	m_hSerialThread->WriteData(sendBuff, c_Parm0[CMD0_SETBIT] + 2);

	m_ThreadDataStruct.timeout = 1;
	m_ThreadDataStruct.ackCount = 0;

}


void CBookLED_DemoDlg::OnBnClickedBuOpenall()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UINT num, macNo;
	UCHAR sendBuff[10], c1, c2;
	WCHAR textBuff[255];

	m_EdMACNum.GetWindowTextW(textBuff, m_EdMACNum.GetWindowTextLengthW() + 1);
	num = _wtoi(textBuff);
	macNo = num ;
	sendBuff[0] = CMD0_SETALL | 0x80;
	sendBuff[1] = macNo & 0x7F;
	sendBuff[2] = (macNo >> 7) & 0x7F;
	sendBuff[3] = 1 & 0x7F;
	c1 = VERIFY_CODE0;
	for (c2 = 0; c2 < c_Parm0[CMD0_SETALL] + 1; c2++)
	{
		c1 += sendBuff[c2];
	}
	sendBuff[c2] = c1 & 0x7F;

	m_hSerialThread->WriteData(sendBuff, c_Parm0[CMD0_SETALL] + 2);

	m_ThreadDataStruct.timeout = 1;
	m_ThreadDataStruct.ackCount = 0;

}


void CBookLED_DemoDlg::OnBnClickedBuCloseall()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UINT num, macNo;
	UCHAR sendBuff[10], c1, c2 ;
	WCHAR textBuff[255];

	m_EdMACNum.GetWindowTextW(textBuff, m_EdMACNum.GetWindowTextLengthW() + 1);
	num = _wtoi(textBuff);
	macNo = num;
	sendBuff[0] = CMD0_SETALL | 0x80;
	sendBuff[1] = macNo & 0x7F;
	sendBuff[2] = (macNo >> 7) & 0x7F;
	sendBuff[3] = 0 & 0x7F;
	c1 = VERIFY_CODE0;
	for (c2 = 0; c2 < c_Parm0[CMD0_SETALL] + 1; c2++)
	{
		c1 += sendBuff[c2];
	}
	sendBuff[c2] = c1 & 0x7F;

	m_hSerialThread->WriteData(sendBuff, c_Parm0[CMD0_SETALL] + 2);

	m_ThreadDataStruct.timeout = 1;
	m_ThreadDataStruct.ackCount = 0;

}


void CBookLED_DemoDlg::OnBnClickedBuAssignopen()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//��Ҫ���Ƶĵ�������뵽buffer
	m_ThreadDataStruct.ledNumbers[0] = 0;
	m_ThreadDataStruct.ledNumbers[1] = 1;
	m_ThreadDataStruct.ledNumbers[2] = 20;
	m_ThreadDataStruct.ledNumbers[3] = 25;
	m_ThreadDataStruct.ledNumbers[4] = 50;
	m_ThreadDataStruct.ledNumbers[5] = 45;
	m_ThreadDataStruct.ledNumbers[6] = 70;
	//Ҫ���ƵĵƵĸ���
	m_ThreadDataStruct.ledMax = 7;
	//��\��
	m_ThreadDataStruct.level = 1;
	//��һ���߳�������
	m_pThreadAssignLED = AfxBeginThread((AFX_THREADPROC)CtrlAssignLED, &m_ThreadDataStruct, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
	m_pThreadAssignLED->ResumeThread();
}


void CBookLED_DemoDlg::OnBnClickedBuAssignclose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_ThreadDataStruct.ledNumbers[0] = 0;
	m_ThreadDataStruct.ledNumbers[1] = 1;
	m_ThreadDataStruct.ledNumbers[2] = 20;
	m_ThreadDataStruct.ledNumbers[3] = 25;
	m_ThreadDataStruct.ledNumbers[4] = 50;
	m_ThreadDataStruct.ledNumbers[5] = 45;
	m_ThreadDataStruct.ledNumbers[6] = 70;
	m_ThreadDataStruct.ledMax = 7;

	m_ThreadDataStruct.level = 0;
	m_pThreadAssignLED = AfxBeginThread((AFX_THREADPROC)CtrlAssignLED, &m_ThreadDataStruct, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
	m_pThreadAssignLED->ResumeThread();

}
