
// MFCDialogDlg.h: 头文件
//

#pragma warning (disable:4996)
#pragma once

#undef min 
#undef max 

#include <iostream>
#include <pcl/io/io.h>
#include <pcl/io/pcd_io.h>
#include "pcl/visualization/pcl_visualizer.h" 

#include "pcl\PCLHeader.h" 
#include "pcl\point_cloud.h" 
#include "pcl\point_types.h" 

#include "vtkSmartPointer.h" 
#include "vtkActor.h" 
#include "vtkRenderWindow.h" 
#include "vtkRenderer.h" 
#include "vtkWin32OpenGLRenderWindow.h" 
#include "vtkWin32RenderWindowInteractor.h" 
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);
VTK_MODULE_INIT(vtkRenderingOpenGL);

using namespace pcl;

// CMFCDialogDlg 对话框
class CMFCDialogDlg : public CDialogEx
{
// 构造
public:
	CMFCDialogDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCDIALOG_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	CString strFolderPath;
	CString EndName;    //扩展名


public:
	afx_msg void OnBnClickedOpenpcd();
};
