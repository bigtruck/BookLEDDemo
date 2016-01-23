
// BookLED_DemoDlg.h : 头文件
//

#pragma once
#include "afxwin.h"

#include "SerialCommunicationThread.h"

#define ERR_NONE			0x00
#define ERR_NOACK			0x01
#define ERR_MAIN_TIMEOUT	0x02
#define ERR_EXT_TIMEOUT		0x03

typedef struct
{
	SerialCommunicationThread *hSerial;
	unsigned char   ackCount;
	unsigned long   timeout;
	unsigned char 	error;

	BOOL level;
	UINT ledNumbers[100];
	UINT ledMax;

}__ThreadMainData;

// CBookLED_DemoDlg 对话框
class CBookLED_DemoDlg : public CDialogEx
{
// 构造
public:
	CBookLED_DemoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_BOOKLED_DEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	__ThreadMainData m_ThreadDataStruct;
	BOOL portOpen;
	CWinThread *m_pThreadMain;
	CWinThread *m_pThreadAssignLED;
	HANDLE *m_hThread;
	SerialCommunicationThread *m_hSerialThread;
	UINT m_TimerID;

	static UINT WINAPI BookLEDDemo_Main(LPVOID lParam);
	void ScanCommonPort(CComboBox *combobox);
	static UINT WINAPI CtrlAssignLED(LPVOID lParam);
	void PortOpenDeal(void);
	void PortCloseDeal(void);

public:
	CButton m_Button_OpenCOM;
	CComboBox m_ComboBox_PortList;
	afx_msg void OnBnClickedOpenCommonPort();
	afx_msg void OnCbnSelchangeComboPortlist();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CEdit m_EdLEDNum;
	CEdit m_EdMACNum;
	CEdit m_EdAssignNumber;
	afx_msg void OnBnClickedBuOpen();
	afx_msg void OnBnClickedBuClose();
	afx_msg void OnBnClickedBuOpenall();
	afx_msg void OnBnClickedBuCloseall();
	afx_msg void OnBnClickedBuAssignopen();
	afx_msg void OnBnClickedBuAssignclose();
	CStatic m_StaEdError;
	CButton m_BuOpenBit;
	CButton m_BuCloseBit;
	CButton m_BuOpenAll;
	CButton m_BuCloseAll;
	CButton m_BuOpenAssign;
	CButton m_BuCloseAssign;
};
