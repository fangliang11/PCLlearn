
// CloudReconstructionView.cpp: CCloudReconstructionView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "CloudReconstruction.h"
#endif

#include "CloudReconstructionDoc.h"
#include "CloudReconstructionView.h"

//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif


// CCloudReconstructionView

IMPLEMENT_DYNCREATE(CCloudReconstructionView, CView)

BEGIN_MESSAGE_MAP(CCloudReconstructionView, CView)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CCloudReconstructionView 构造/析构

CCloudReconstructionView::CCloudReconstructionView() noexcept
{
	// TODO: 在此处添加构造代码
	m_cloud.reset((new pcl::PointCloud<pcl::PointXYZ>));
	m_viewer.reset(new pcl::visualization::PCLVisualizer("viewer", false));//初始化viewer对象
	
	m_iren = vtkSmartPointer<vtkRenderWindowInteractor>::New(); //初始化vtkwindow交互的对象 

}

CCloudReconstructionView::~CCloudReconstructionView()
{
	// 此处的内存释放无用，网上说为 interactor 无法释放， release下正常
	//m_iren->SetRenderWindow(nullptr);
	//m_iren->SetInteractorStyle(nullptr);
	//m_iren->Delete();
	
}

BOOL CCloudReconstructionView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CCloudReconstructionView 绘图

void CCloudReconstructionView::OnDraw(CDC* /*pDC*/)
{
	CCloudReconstructionDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码

}

// CCloudReconstructionView 诊断

#ifdef _DEBUG
void CCloudReconstructionView::AssertValid() const
{
	CView::AssertValid();
}

void CCloudReconstructionView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCloudReconstructionDoc* CCloudReconstructionView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCloudReconstructionDoc)));
	return (CCloudReconstructionDoc*)m_pDocument;
}
#endif //_DEBUG


// CCloudReconstructionView 消息处理程序


void CCloudReconstructionView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	//m_viewer.reset(new pcl::visualization::PCLVisualizer("viewer", false));//初始化viewer对象
	//m_viewer->addCoordinateSystem();           //设置对应的坐标系
	m_viewer->setBackgroundColor(0, 0, 0);     //设置背景颜色
	m_viewer->initCameraParameters();          //初始化相机的参数
	m_rwnd = m_viewer->getRenderWindow();       //将view中的渲染窗口的句柄传递给vtk window
	//m_iren = vtkRenderWindowInteractor::New(); //初始化vtkwindow交互的对象 
	m_viewer->resetCamera();                   //使点云显示在屏幕中间，并绕中心操作

	LoadPCDfile();                             //载入点云数据：m_viewer->addPointCloud

	CRect rect;
	this->GetClientRect(&rect);                //实时获取MFC窗口大小
	m_rwnd->SetSize(rect.right - rect.left, rect.bottom - rect.top); //根据当前窗口的大小设置vtk 窗口的大小
	m_rwnd->SetParentId(this->m_hWnd);          //设置vtk窗口的句柄
	m_rwnd->SetPosition(0, 0);
	m_iren->SetRenderWindow(m_rwnd);            //将vtk交互对象与vtk window绑定 
	m_viewer->createInteractor();
	//m_viewer->setupInteractor(m_iren, m_win);
	m_rwnd->Render();                           //开始渲染
	
}


void CCloudReconstructionView::LoadPCDfile()
{
	std::string filename = "rabbit.pcd";
	if (pcl::io::loadPCDFile(filename, *m_cloud) == -1) {
		PCL_ERROR("Couldn't read file");
		return;
	}
	m_viewer->setBackgroundColor(0.0, 0.0, 0.0);
	m_viewer->removeAllPointClouds();//该函数不加则不显示
	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> single_color(m_cloud, 0, 255, 0);  //设置点云颜色
	m_viewer->addPointCloud<pcl::PointXYZ >(m_cloud, single_color, "sample cloud");
	m_viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "sample cloud");
}


void CCloudReconstructionView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: 在此处添加消息处理程序代码

}
