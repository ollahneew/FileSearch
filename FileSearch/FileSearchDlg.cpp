
// FileSearchDlg.cpp : implementation file

#include "pch.h"
#include "framework.h"
#include "FileSearch.h"
#include "FileSearchDlg.h"
#include "afxdialogex.h"
#include "AclAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// CFileSearchDlg dialog
CFileSearchDlg::CFileSearchDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FILESEARCH_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFileSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, path_val);
	DDX_Control(pDX, IDC_EDIT2, file_name_val);
	DDX_Control(pDX, IDC_BUTTON1, button_search);
	DDX_Control(pDX, IDC_LIST3, result_listCtrl);
}

BEGIN_MESSAGE_MAP(CFileSearchDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT1, &CFileSearchDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON1, &CFileSearchDlg::OnBnClickedButton1)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST3, &CFileSearchDlg::OnLvnItemchangedList3)
END_MESSAGE_MAP()


// CFileSearchDlg message handlers
BOOL CFileSearchDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	int screenX = GetSystemMetrics(SM_CXSCREEN);
	int screenY = GetSystemMetrics(SM_CYSCREEN);

	int newWidth = screenX * 0.8;
	int newHeight = screenY * 0.8;

	MoveWindow(0, 0, newWidth, newHeight);
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	// Ask Mfc to create/inseart a column
	/*
	result_listCtrl.InsertColumn(
		0, // Rank/order of item 
		L"ID", // Caption for this header
		LVCFMT_LEFT, // Relative position of items under header
		30); // Width of items under header
	*/
	result_listCtrl.InsertColumn(0, L"File name", LVCFMT_CENTER, 120);
	result_listCtrl.InsertColumn(1, L"Size", LVCFMT_CENTER, 120);
	result_listCtrl.InsertColumn(2, L"Full path", LVCFMT_CENTER, 120);
	result_listCtrl.InsertColumn(3, L"Create date", LVCFMT_CENTER, 120);
	result_listCtrl.InsertColumn(4, L"Edit date", LVCFMT_CENTER, 120);
	result_listCtrl.InsertColumn(5, L"Owner", LVCFMT_CENTER, 120);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFileSearchDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFileSearchDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFileSearchDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFileSearchDlg::OnEnChangeEdit1()
{
}


void CFileSearchDlg::OnBnClickedButton1()
{

	UpdateData(TRUE);

	CString strPath, strFileName;
	path_val.GetWindowText(strPath);
	file_name_val.GetWindowText(strFileName);

	CFileFind finder;
	BOOL bWorking = finder.FindFile(strPath + _T("\\") + strFileName + _T(".*"));
	result_listCtrl.DeleteAllItems();

	while (bWorking) {
		bWorking = finder.FindNextFile();
		if (!finder.IsDots())
		{
			CTime timeCreate, timeEdit;
			finder.GetCreationTime(timeCreate);
			finder.GetLastWriteTime(timeEdit);

			int nItem = result_listCtrl.InsertItem(result_listCtrl.GetItemCount(), finder.GetFileName());
			result_listCtrl.SetItemText(nItem, 1, GetFileSizeString(finder.GetLength()));
			result_listCtrl.SetItemText(nItem, 2, finder.GetFilePath());
			result_listCtrl.SetItemText(nItem, 3, timeCreate.Format("%Y-%m-%d %H:%M:%S"));
			result_listCtrl.SetItemText(nItem, 4, timeEdit.Format("%Y-%m-%d %H:%M:%S"));
			result_listCtrl.SetItemText(nItem, 5, GetFileOwner(finder.GetFilePath()));
		}

	}
	
}

CString CFileSearchDlg::GetFileSizeString(ULONGLONG nFileSize)
{
	CString strResult;
	double dFileSize = static_cast<double>(nFileSize);

	const LPCTSTR szUnits[] = {
		_T("bytes"), _T("KB"), _T("MB"), _T("GB"), _T("TB")
	};
	int nUnitIndex = 0;

	while (dFileSize >= 1024.0 && nUnitIndex < _countof(szUnits) - 1)
	{
		dFileSize /= 1024.0;
		nUnitIndex++;
	}

	strResult.Format(_T("%.2f %s"), dFileSize, szUnits[nUnitIndex]);
	return strResult;
}

CString CFileSearchDlg::GetFileOwner(const CString& strFilePath)
{
	DWORD dwRes = 0;
	PSID pSidOwner = NULL;
	LPTSTR AcctName = NULL;
	LPTSTR DomainName = NULL;
	DWORD dwAcctName = 0, dwDomainName = 0;
	SID_NAME_USE eUse = SidTypeUnknown;

	// get the owner SID of the file
	dwRes = GetNamedSecurityInfo(
		(LPCTSTR)strFilePath,   
		SE_FILE_OBJECT,
		OWNER_SECURITY_INFORMATION,
		&pSidOwner,
		NULL, NULL, NULL, NULL
	);

	if (dwRes != ERROR_SUCCESS)
	{
		return _T("N/A");
	}


	LookupAccountSid(
		NULL,            // name of local or remote computer, default = localhost
		pSidOwner,
		NULL,            // account name buffer
		&dwAcctName,
		NULL,            // domain name
		&dwDomainName,   //size of domain name buffer
		&eUse			//SID type
	);


	AcctName = (LPTSTR)LocalAlloc(LMEM_ZEROINIT, dwAcctName * sizeof(TCHAR));
	DomainName = (LPTSTR)LocalAlloc(LMEM_ZEROINIT, dwDomainName * sizeof(TCHAR));

	if (AcctName == NULL || DomainName == NULL)
	{
		if (AcctName != NULL) LocalFree(AcctName);
		if (DomainName != NULL) LocalFree(DomainName);
		LocalFree(pSidOwner);
		return _T("N/A");
	}

	if (!LookupAccountSid(
		NULL,
		pSidOwner,
		AcctName,
		&dwAcctName,
		DomainName,
		&dwDomainName,
		&eUse
	))
	{

		return _T("N/A");
	}

	CString strOwnerName = AcctName;
	
	return strOwnerName;
}



void CFileSearchDlg::OnLvnItemchangedList3(NMHDR* pNMHDR, LRESULT* pResult)
{
LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
// TODO: Add your control notification handler code here
*pResult = 0;
}
