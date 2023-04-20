
// synchronizationThreadsDlg.h: файл заголовка
//

#pragma once
struct Info {
	CEdit* txt;
	bool IsMain;
	HANDLE
		mem,
		mes,
		syn;
};

// Диалоговое окно CsynchronizationThreadsDlg
class CsynchronizationThreadsDlg : public CDialogEx
{
// Создание
public:
	CsynchronizationThreadsDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SYNCHRONIZATIONTHREADS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	CEdit Text;
	
	

	Info inf;

	

	afx_msg void OnBnClickedCancel2();
	afx_msg void OnEnChangeEdit1();
};
