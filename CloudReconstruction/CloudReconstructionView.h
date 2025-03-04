﻿
// CloudReconstructionView.h: CCloudReconstructionView 类的接口
//

#pragma once

//  PCL
#undef min 
#undef max 
#include <pcl/console/parse.h>
#include <pcl/io/io.h>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/common/io.h>
#include <pcl/visualization/pcl_visualizer.h>
//  VTK
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>


class CCloudReconstructionView : public CView
{
protected: // 仅从序列化创建
	CCloudReconstructionView() noexcept;
	DECLARE_DYNCREATE(CCloudReconstructionView)

// 特性
public:
	CCloudReconstructionDoc* GetDocument() const;

// 操作
public:
	void LoadPCDfile();

private:
	pcl::PointCloud<pcl::PointXYZ>::Ptr m_cloud;
	boost::shared_ptr<pcl::visualization::PCLVisualizer> m_viewer;//要共享指针类型的，要不然，显示窗口会跳出MFC界面
	vtkRenderWindow* m_win;   //vtk渲染的窗口句柄
	vtkRenderWindowInteractor* m_iren;//vtk交互的对象


// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CCloudReconstructionView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
};

#ifndef _DEBUG  // CloudReconstructionView.cpp 中的调试版本
inline CCloudReconstructionDoc* CCloudReconstructionView::GetDocument() const
   { return reinterpret_cast<CCloudReconstructionDoc*>(m_pDocument); }
#endif

