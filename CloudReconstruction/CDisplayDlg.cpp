// CDisplayDlg.cpp: 实现文件
//

#include "pch.h"
#include "CloudReconstruction.h"
#include "CDisplayDlg.h"


// CDisplayDlg

IMPLEMENT_DYNCREATE(CDisplayDlg, CFormView)

CDisplayDlg::CDisplayDlg()
	: CFormView(IDD_DLG_DISPLAY)
{

}

CDisplayDlg::~CDisplayDlg()
{
}

void CDisplayDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDisplayDlg, CFormView)
END_MESSAGE_MAP()


// CDisplayDlg 诊断

#ifdef _DEBUG
void CDisplayDlg::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDisplayDlg::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDisplayDlg 消息处理程序


void CDisplayDlg::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	// TODO: 在此添加专用代码和/或调用基类
	CMainFrame* pMainFrm = (CMainFrame *)AfxGetApp()->GetMainWnd();
	pMainFrm->m_pdlgDisplay = this;

}
