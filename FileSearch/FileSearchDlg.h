
// FileSearchDlg.h : header file
//

#pragma once


// CFileSearchDlg dialog
class CFileSearchDlg : public CDialogEx
{
// Construction
public:
	CFileSearchDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILESEARCH_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg CString GetFileSizeString(ULONGLONG nFileSize);
	afx_msg CString GetFileOwner(const CString& strFilePath);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnLvnItemchangedList3(NMHDR* pNMHDR, LRESULT* pResult);
	
protected:
	CEdit path_val;
	CEdit file_name_val;
	CButton button_search;
public:
	CListCtrl result_listCtrl;
};
