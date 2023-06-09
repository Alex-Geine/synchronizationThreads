﻿
// synchronizationThreadsDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "synchronizationThreads.h"
#include "synchronizationThreadsDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Диалоговое окно CsynchronizationThreadsDlg



CsynchronizationThreadsDlg::CsynchronizationThreadsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SYNCHRONIZATIONTHREADS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CsynchronizationThreadsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, Text);
}

BEGIN_MESSAGE_MAP(CsynchronizationThreadsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CsynchronizationThreadsDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDCANCEL2, &CsynchronizationThreadsDlg::OnBnClickedCancel2)
	ON_EN_CHANGE(IDC_EDIT1, &CsynchronizationThreadsDlg::OnEnChangeEdit1)
END_MESSAGE_MAP()

	
//поток, ждущий сообщений
DWORD WINAPI WhaitFor(PVOID param) {	
	Info* inf = (Info*)param;
	HANDLE mas[2] = { inf->mes, inf->syn };
	
	for (;;) {
		if (!inf->IsMain) {			
			DWORD id = WaitForMultipleObjects(2, mas, FALSE, INFINITE);
						
			//обработчик изменения текста
			if (id == 0) {
				//MessageBoxW(NULL, (LPCTSTR)"1", NULL, NULL);
				CString* str = (CString*)MapViewOfFile(inf->mem, FILE_MAP_ALL_ACCESS, 0, 0, 1024);
				//MessageBoxW(NULL, (LPCTSTR)str, NULL,NULL);
				inf->txt->SetWindowTextW((LPCTSTR)str);
			}
			//обработчик закрытия главного окна
			if(id == 1) {				
				inf->IsMain = true;
				inf->txt->SetReadOnly(FALSE);
			}
		}
		else {			
			return 0;
		}			
	}	
}

// Обработчики сообщений CsynchronizationThreadsDlg

BOOL CsynchronizationThreadsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию
	inf.IsMain = false;
	inf.txt = &Text;

	//хэндл синхронизации 
	inf.syn = OpenEventW(EVENT_ALL_ACCESS, FALSE, LPWSTR("CUR"));

	//хэндл сообщения 
	inf.mes = OpenEventW(EVENT_ALL_ACCESS, FALSE, LPWSTR("SIMB"));

	//хэндл памяти
	inf.mem = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, LPWSTR("Mem"));
	inf.txt->SetReadOnly(TRUE);

	if (inf.syn == NULL) {
		//синхронизация
		inf.syn = CreateEvent(NULL, FALSE, FALSE, LPWSTR("CUR"));
		//сообщения 
		inf.mes = CreateEvent(NULL, TRUE, FALSE, LPWSTR("SIMB"));
		//память
		inf.mem = CreateFileMapping(
			INVALID_HANDLE_VALUE,
			NULL,
			PAGE_READWRITE,
			0,
			1024,
			LPWSTR("Mem")
		);
		inf.IsMain = true;
		inf.txt->SetReadOnly(FALSE);
	}

	CreateThread(NULL, NULL, WhaitFor, &inf, 0, NULL);
	
	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void CsynchronizationThreadsDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CsynchronizationThreadsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CsynchronizationThreadsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//кнопка выхода
void CsynchronizationThreadsDlg::OnBnClickedCancel()
{	
	
	SetEvent(OpenEventW(EVENT_ALL_ACCESS, FALSE, LPCWSTR("CUR")));
	CDialogEx::OnCancel();
}





void CsynchronizationThreadsDlg::OnBnClickedCancel2()
{
	
}


//обработчик изменения текст эдита
void CsynchronizationThreadsDlg::OnEnChangeEdit1()
{	
	if (inf.IsMain) {
		CString buf;
		Text.GetWindowTextW(buf);
		
		CString* str = (CString*)MapViewOfFile(inf.mem, FILE_MAP_ALL_ACCESS, 0, 0, 1024);
		CopyMemory((PVOID)str, buf,1024);
				
		if (inf.mes != NULL) 
			PulseEvent(inf.mes);			
	}
	
}
