
// MFCDialogDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCDialog.h"
#include "MFCDialogDlg.h"
#include "afxdialogex.h"

//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif


// CMFCDialogDlg 对话框



CMFCDialogDlg::CMFCDialogDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCDIALOG_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCDialogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCDialogDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPENPCD, &CMFCDialogDlg::OnBnClickedOpenpcd)
END_MESSAGE_MAP()


// CMFCDialogDlg 消息处理程序

BOOL CMFCDialogDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码



	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCDialogDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCDialogDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//打开点云文件
void CMFCDialogDlg::OnBnClickedOpenpcd()
{
	// TODO: 在此添加控件通知处理程序代码

	pcl::PointCloud<pcl::PointXYZ>::Ptr point_cloud_ptr(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::PointCloud<pcl::PointXYZ>& point_cloud = *point_cloud_ptr;
	pcl::PointCloud<pcl::PointWithViewpoint> far_ranges;
	Eigen::Affine3f scene_sensor_pose(Eigen::Affine3f::Identity());

	//打开文件 定义四种格式的文件bmp gif jpg tiff
	CString filter;
	filter = "所有文件(*.pcd,*ply)|*.pcd;*.ply| PCD(*.pcd)|*.pcd| PLY(*.ply)|*.ply||";
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter, NULL);

	//按下确定按钮 dlg.DoModal() 函数显示对话框
	if (dlg.DoModal() == IDOK)
	{
		//打开对话框获取图像信息
		strFolderPath = dlg.GetPathName();     //获取文件路径名   如D:\pic\abc.bmp
		EndName = dlg.GetFileExt();      //获取文件扩展名
		EndName.MakeLower();                     //将文件扩展名转换为一个小写字符
		std::string filename((LPCTSTR)strFolderPath);

		if (EndName.Compare(_T("ply")) == 0 || EndName.Compare(_T("pcd")) == 0)
		{
			//显示图像
			if (pcl::io::loadPCDFile<pcl::PointXYZ>(filename, point_cloud) == -1) //* load the file 
			{
				PCL_ERROR("Couldnot read file.\n");
				system("pause");
				return;
			}

			scene_sensor_pose = Eigen::Affine3f(Eigen::Translation3f(point_cloud.sensor_origin_[0],
				point_cloud.sensor_origin_[1],
				point_cloud.sensor_origin_[2])) *
			    Eigen::Affine3f(point_cloud.sensor_orientation_);

			pcl::visualization::PCLVisualizer viewer("", false); //viewer("", false);// ("3D Viewer"); 
			viewer.setBackgroundColor(0, 0, 0);
			viewer.addCoordinateSystem(1.0);
			pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> single_color(point_cloud_ptr, 128, 128, 228);

			viewer.addPointCloud<pcl::PointXYZ>(point_cloud_ptr, single_color, "sample cloud");
			viewer.setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "sample cloud");

			vtkRenderer* VisualiserRenderer = vtkRenderer::New();
			vtkRenderWindow* VisualiserWindow = viewer.getRenderWindow();
			vtkRenderWindowInteractor* InteractionVisualiserWindow = vtkRenderWindowInteractor::New();

			LPRECT rect = new CRect;
			CStatic* pclStatic = new CStatic();
			pclStatic = (CStatic*)GetDlgItem(IDC_STATIC);
			VisualiserWindow->SetParentId(pclStatic->m_hWnd);
			//VisualiserWindow->SetSize(rect->right - rect->left, rect->bottom - rect->top);
			VisualiserWindow->SetSize(800, 600);
			VisualiserWindow->SetPosition(10, 10);
			InteractionVisualiserWindow->SetRenderWindow(VisualiserWindow);
			VisualiserWindow->Render();
			InteractionVisualiserWindow->Render();

		}
		else return;
	}
	else return;




}
