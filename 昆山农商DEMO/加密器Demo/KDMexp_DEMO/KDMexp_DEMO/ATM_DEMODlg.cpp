// ATM_DEMODlg.cpp : implementation file
//

#include "stdafx.h"
#include "ATM_DEMO.h"
#include "ATM_DEMODlg.h"

#include "KDMexp.h"
#include "des.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma comment(lib,"KDMexp.lib") 



char g_bComNum[5];		// �˿ں�"COM1"

struct TMK{
	char termid[21];	// �ն˱��
	char kval[49];		// �ն���Կ
	int  klen;			// ��Կ����
	char kchk[7];		// У��ֵ
}tmk;

char g_sAtmCode[21];	
char g_strTmk[49];		// 
int  g_sLen = 0;
unsigned char g_TransKey[24]; // ��¼����������ն˱�ŵĺϳ���Կ
unsigned char g_tLen = 0;

unsigned char g_ThreadState; // 0 �̹߳ر� 1 �߳��Զ��ر� 2�߳�����

HANDLE	m_hThreadEvent;
HANDLE	hComHand;
HWND	hWnd;

CEdit* g_editLog = NULL;
CEdit* g_check = NULL;
CEdit* g_rand = NULL;
CEdit* g_term = NULL;
CEdit* g_len = NULL;

CWinThread* pThread;

BOOL	b_AtmGoing;


void Log(CString info);
void ShowTmkInfo();


// �ַ�ת��
int StringToHex(unsigned char *hex,unsigned char *str,unsigned int len)
{
	unsigned int i;
	unsigned char tempH,tempL;

	for(i=0; i<len; i++)
	{
	   tempH = *str++;
	   tempL = *str++;
	   if(tempH >= '0' && tempH <= '9') tempH -= '0';
	   else
	   if(tempH >= 'A' && tempH <= 'F') tempH = tempH - 'A' + 0x0a;
	   else return -1;
	   if(tempL >= '0' && tempL <= '9') tempL -= '0';
	   else
	   if(tempL >= 'A' && tempL <= 'F') tempL = tempL - 'A' + 0x0a;
	   else return -1;
	   *hex++ = (tempH << 4) | tempL;
	}
	return 0;
}

void HexToString(unsigned char *hex,unsigned char *str,unsigned int len)
{
	unsigned int i;
	unsigned char temp;
	unsigned char tmp;

	for(i=0; i<len; i++)
	{
		temp = *hex++;
		tmp  = temp >> 4;
		temp&= 0x0f;
		if(tmp < 0x0a) *str++ = tmp + '0';
		else		   *str++ = tmp + 'A'-0x0a;
		if(temp < 0x0a) *str++ = temp + '0';
		else		    *str++ = temp + 'A'-0x0a;
	}
}


// �򿪴���
int ComOpen(HANDLE &hComHand,char * Comport,long iBaudRate,unsigned int time_set)
{
	hComHand = CreateFile(Comport,	// �˿ں�
				GENERIC_READ|GENERIC_WRITE, // �������д
				0, // ��ռ��ʽ
				NULL,
				OPEN_EXISTING, // �򿪶����Ǵ���
				0, // 0ͬ����ʽ ; FILE_FLAG_OVERLAPPED �첽��ʽ
				NULL);

	if(hComHand == INVALID_HANDLE_VALUE)
	{
		return -1;  // ��ʧ�ܣ�����-1
	}
	
	SetupComm(hComHand,19620,3000); //���ý��պͷ��仺����
	
	COMMTIMEOUTS TimeOuts;       //��ʱ��������
	//�趨����ʱ
	TimeOuts.ReadIntervalTimeout		= 6;
	TimeOuts.ReadTotalTimeoutMultiplier	= 6;
	TimeOuts.ReadTotalTimeoutConstant	= 100; // time_set
	
	//�趨д��ʱ
	TimeOuts.WriteTotalTimeoutMultiplier = 100;
	TimeOuts.WriteTotalTimeoutConstant   = 500;
	SetCommTimeouts(hComHand,&TimeOuts); //���ó�ʱ ms
	
	//ͨѶ��������
	DCB dcb;
	GetCommState(hComHand,&dcb);
	dcb.BaudRate = iBaudRate;	//������Ϊ9600
	dcb.ByteSize = 8;			//ÿ���ֽ���8λ
	dcb.Parity   = NOPARITY;	//����żУ��λ
	dcb.StopBits = 0;			//����ֹͣλ
	dcb.EvtChar  = (char)0xC0;	//�յ�0xC0�����¼�
	SetCommState(hComHand,&dcb);//����ͨѶ����

 	SetCommMask(hComHand,EV_RXCHAR); // ���ô��ڵȴ��¼�
	
	//��ֹĿǰ���ڽ���
	PurgeComm(hComHand, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	
	return 0;
}

// �жϴ����Ƿ��
BOOL IsOpen(HANDLE hComHand)  
{ 
	if (hComHand == INVALID_HANDLE_VALUE)
		return false; 	
	else
		return true; 
}

// �رմ���
int ComClose(HANDLE hComHand)
{
	if ( IsOpen(hComHand) )
	{
		PurgeComm(hComHand, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
		BOOL bSuccess = CloseHandle(hComHand);
		if(bSuccess)
		{ //�رճɹ�
			hComHand = INVALID_HANDLE_VALUE;
			return 0; 
		}
		else
			return -1;
	}
	else 
		return 0;	// COM�ڱ�����û�д�
}

// ��ȡһ���ֽ�
int ReadByte(HANDLE hComHand,unsigned char * data_buff)
{
	unsigned long fact_RDdata_count = 0;
	BOOL bReadStat;

	if( IsOpen(hComHand) )
	{
		bReadStat = ReadFile(hComHand, data_buff, 1, &fact_RDdata_count, NULL);
		if (!bReadStat || (fact_RDdata_count != 1))
		{
			return -1;	
		}
		return 0;
	}
	else 
	{//����δ��
		return -2;
	}
}

// �����ڻ�����
int ReadCom(HANDLE hComHand,unsigned char * data_buff,unsigned int data_count)
{
	unsigned long fact_RDdata_count = 0;
	BOOL bReadStat;
	
	if( IsOpen(hComHand) )
	{
		bReadStat = ReadFile(hComHand, &data_buff[0], data_count, &fact_RDdata_count, NULL);
		if (!bReadStat || (fact_RDdata_count != data_count))
		{
			PurgeComm(hComHand, PURGE_TXABORT | PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
			return -1;	
		}

	    PurgeComm(hComHand, PURGE_TXABORT | PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
		return 0;
	}
	else 
	{//����δ��
		return -2;
	}
}

// д���ڻ�����
int WriteCom(HANDLE hComHand,unsigned char * data_buff,unsigned int data_count)
{
	BOOL bWriteStat;
	unsigned long fact_WRdata_count = 0;
	
	if( IsOpen(hComHand) )
	{	
		//����շ���������ֹͣ��̨����д������
		//PurgeComm(hComHand, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
		PurgeComm(hComHand, PURGE_TXABORT | PURGE_TXCLEAR);
		bWriteStat = WriteFile(hComHand, data_buff,data_count,&fact_WRdata_count, NULL);
		if (!bWriteStat && ((unsigned int)fact_WRdata_count != data_count))
		{	
			return -1;	
		}
			
		return 0;//�ɹ�
	}
	else
		return -2;//����δ��
}

//DWORD WINAPI 
UINT ListenThread(
	 LPVOID lpParameter // thread data
);

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CATM_DEMODlg dialog

CATM_DEMODlg::CATM_DEMODlg(CWnd* pParent /*=NULL*/)
	: CDialog(CATM_DEMODlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CATM_DEMODlg)
	m_rand = _T("");
	m_Check = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CATM_DEMODlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CATM_DEMODlg)
	DDX_Control(pDX, IDC_TMK_LEN, m_tmklen);
	DDX_Control(pDX, IDC_TERM_CODE, m_TermCode);
	DDX_Control(pDX, IDC_CHECK, m_check);
	DDX_Control(pDX, IDC_RAND, m_Rand);
	DDX_Control(pDX, IDC_COMBO_COMM, m_Comm);
	DDX_Control(pDX, IDC_EXCUTE, m_bntExcute);
	DDX_Control(pDX, IDC_LOG, m_editLog);
	DDX_Text(pDX, IDC_CHECK, m_Check);
	DDV_MaxChars(pDX, m_Check, 6);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CATM_DEMODlg, CDialog)
	//{{AFX_MSG_MAP(CATM_DEMODlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_SELCHANGE(IDC_COMBO_COMM, OnSelchangeComm)
	ON_BN_CLICKED(IDC_EXCUTE, OnExcute)
	ON_BN_CLICKED(IDC_CLEAR_LOG, OnClearLog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CATM_DEMODlg message handlers

BOOL CATM_DEMODlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_Comm.SetCurSel(0);
	GetCommNum(g_bComNum);
	g_editLog = &m_editLog;
	g_check = &m_check;
	g_rand = &m_Rand;
	g_term = &m_TermCode;
	g_len = &m_tmklen;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CATM_DEMODlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CATM_DEMODlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CATM_DEMODlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CATM_DEMODlg::GetCommNum(char *com_buff)
{
	CString str;

	int index = m_Comm.GetCurSel();
	m_Comm.GetLBText(index,str);

	strncpy(com_buff,(char *)(LPCTSTR)str,4);
}

void CATM_DEMODlg::OnSelchangeComm() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	GetCommNum(g_bComNum);
}


void Write_Log(char filename[], char log[])
{
	HANDLE hFile;

	hFile = CreateFile(filename,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

	DWORD dwHigh;
	DWORD dwPos = GetFileSize(hFile,&dwHigh);
	SetFilePointer(hFile,dwPos,0,FILE_BEGIN);

	DWORD dwWrites;
	WriteFile(hFile,log,strlen(log),&dwWrites,NULL);
	WriteFile(hFile,"\r\n",strlen("\r\n"),&dwWrites,NULL);
	CloseHandle(hFile);
}

void CATM_DEMODlg::OnExcute() 
{
	// TODO: Add your control notification handler code here
	CString str;
 
	UpdateData(TRUE);
	if(GetDlgItemText(IDC_EXCUTE,str),str == "ֹͣģ��")
	{
		g_ThreadState = 1;
		Log("���ڼ����߳̽�����");
		if(ComClose(hComHand) == 0)
		{ // �ɹ��رմ���
			b_AtmGoing = FALSE;
			SetDlgItemText(IDC_EXCUTE,"��ʼģ��");
			GetDlgItem(IDC_COMBO_COMM)->EnableWindow(TRUE);
			Log("ATM�ն�ģ������ѱ���ֹ��");
		}
	}
	else
	{
		if( ComOpen(hComHand,g_bComNum,9600,9000) !=0)
		{
			Log("���ڴ�ʧ��!");
			return;
		}
		b_AtmGoing = TRUE;
		SetDlgItemText(IDC_EXCUTE,"ֹͣģ��");
		GetDlgItem(IDC_COMBO_COMM)->EnableWindow(FALSE);

		str = "��ʼ�����˿ڣ�";
		str += g_bComNum;
		Log(str);

		ClearRand(); // ���ź��ն���Կ��

		m_hThreadEvent = CreateEvent(NULL,FALSE,FALSE,NULL);

		hWnd = GetSafeHwnd();
		pThread = AfxBeginThread(ListenThread,hWnd);
		//pThread->m_bAutoDelete = FALSE;
		g_ThreadState = 2;
	}
}



void CATM_DEMODlg::ClearRand(void)
{
	CString str = "";

	g_tLen = 0;

	g_term->SetWindowText(str);
	g_len->SetWindowText(str);

	g_rand->GetWindowText(str);
	str.Empty();
	g_rand->SetWindowText(str);

	g_check->GetWindowText(str);
	str.Empty();
	g_check->SetWindowText(str);
}

void ClearRand(void)
{
	CATM_DEMODlg::ClearRand();
}

UINT ListenThread(
	 LPVOID lpParameter // thread data
)
{
	unsigned char buff[100];
	unsigned char cnt = 0;
	unsigned char temp;
	unsigned char f_slipHead = 0;
	unsigned char pack[100];
	unsigned char len = 0;

	CString s;
	CString str;
	CTime time;

	for(;;)
	{
		if(g_ThreadState == 1)
		{
			g_ThreadState = 0;
			return 0;
		}

		if(ReadByte(hComHand,&temp) != 0) continue;

		if(temp == 0xC0)
		{
			if(f_slipHead == 0 || cnt == 1)
			{	// ��һ��C0����������C0
				f_slipHead = 1;
				cnt = 0;
				buff[cnt++] = temp;
				continue;
			}
			else
			{	// �յ��������ݰ�
				buff[cnt++] = temp;
			}
		}
		else
		{
			if(f_slipHead == 1)
			{	// ��Ч����
				buff[cnt++] = temp;
			}
			continue;
		}

		memset(&tmk,0,sizeof(struct TMK));
		temp = dispatchData(cnt,buff,tmk.termid,tmk.kval);
		
		f_slipHead = 0; // �����ݰ���ʶ
		cnt = 0;

		if(temp == 0)
		{
			Log("�յ��ն�����Կ���ݰ�����!");
		}
		else
		{
			tmk.klen = strlen(tmk.kval);

			Log("�յ��ն�����Կ!");
			str = "��Կ����ֵ��";
			str += tmk.kval;
			Log(str);

			StringToHex(pack,(unsigned char *)tmk.kval,tmk.klen);
			CalculateCheck(pack,tmk.klen/2,(unsigned char *)tmk.kchk);
			str = "��ԿУ��ֵ��";
			str += tmk.kchk;
			Log(str);

			Log("������Կ�ɹ���");
			time = CTime::GetCurrentTime();
			s = time.Format(TEXT("%Y%m%d%H%M%S"));
			str = "����Ӧ��ʱ�䣺";
			str += s;
			Log(str);

			memcpy(buff,(char *)(LPCTSTR)s+2,12);
			len = assembleMsg(1,12,buff,pack);
			if(WriteCom(hComHand,pack,len) != 0)
			{
				Log("Ӧ����ʧ�ܣ�");
			}
			ShowTmkInfo();
		}
	}

	return 0;
}


void CATM_DEMODlg::Log(CString info)
{
	CString str;

	if(g_editLog)
	{
		g_editLog->GetWindowText(str);
		str += "--> ";
		str += info;
		str += "\r\n";
		g_editLog->SetWindowText(str);
	}
}


void Log(CString info)
{
	CATM_DEMODlg::Log(info);
}

void CATM_DEMODlg::OnClearLog() 
{
	// TODO: Add your control notification handler code here
	CString str;

	if(g_editLog)
	{
		g_editLog->GetWindowText(str);
		str.Empty();
		g_editLog->SetWindowText(str);
	}
}

void CATM_DEMODlg::ShowTmkInfo()
{
	CString str;

	str.Format("%s",tmk.termid);
	g_term->SetWindowText(str);
	str.Format("%s",tmk.kval);
	g_rand->SetWindowText(str);
	str.Format("%s",tmk.kchk);
	g_check->SetWindowText(str);
	str.Format("%d",tmk.klen);
	g_len->SetWindowText(str);
}

void ShowTmkInfo()
{
	CATM_DEMODlg::ShowTmkInfo();
}
