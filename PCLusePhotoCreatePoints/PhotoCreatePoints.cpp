#pragma warning (disable :4996)

#include <iostream>
#include <string>

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <pcl/point_types.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/io/io.h>
#include <pcl/io/pcd_io.h>
#include <vtkAutoInit.h>

using namespace std;
using namespace cv;

VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);
VTK_MODULE_INIT(vtkRenderingOpenGL);

int user_data;

void viewerOneOff(pcl::visualization::PCLVisualizer& viewer)
{
	viewer.setBackgroundColor(0.5, 0.5, 1.0);  //设置背景
	pcl::PointXYZ o;   //点云数据
	o.x = 1.0;
	o.y = 0;
	o.z = 0;
	viewer.addSphere(o, 0.25, "sphere", 0);  // 增加球体
	std::cout << "i only run once" << std::endl;
}

//显示字符
void viewerPsycho(pcl::visualization::PCLVisualizer& viewer)
{
	static unsigned count = 0;
	std::stringstream ss;
	ss << "Once per viewer loop: " << count++;
	viewer.removeShape("text", 0);
	viewer.addText(ss.str(), 200, 300, "text", 0);

	//FIXME: possible race condition here:
	user_data++;
}

// 定义点云类型
typedef pcl::PointXYZRGBA PointT;
typedef pcl::PointCloud<PointT> PointCloud;


// 相机内参
const double camera_factor = 1000;
const double camera_cx = 325.5;
const double camera_cy = 253.5;
const double camera_fx = 518.0;
const double camera_fy = 519.0;

// 主函数
int main(int argc, char** argv)
{
	// 读取./data/rgb.png和./data/depth.png，并转化为点云
	cv::Mat rgb, depth;
	rgb = cv::imread("D:\\testData\\left.png");	// rgb 图像是8UC3的彩色图像
	depth = cv::imread("D:\\testData\\disp_SGBM.png", -1); 	// depth 是16UC1的单通道图像，注意flags设置-1,表示读取原始数据不做任何修改
	
	PointCloud::Ptr cloud(new PointCloud);  // 使用智能指针，创建一个空点云。这种指针用完会自动释放。

    // 遍历深度图 m 行， n 列
	for (int m = 0; m < depth.rows; m++)
		for (int n = 0; n < depth.cols; n++)
		{
			ushort d = depth.ptr<ushort>(m)[n];  //深度图像第 m 行， 第 n 列的像素值
			// d 可能没有值，若如此，跳过此点
			if (d == 0)
				continue;
			// d 存在值，则向点云增加一个点
			PointT p;

			// 计算这个点的空间坐标（x, y, z)
			p.z = double(d) / camera_factor;
			p.x = (n - camera_cx) * p.z / camera_fx;
			p.y = (m - camera_cy) * p.z / camera_fy;

			// 从rgb图像中获取它的颜色
			// rgb是三通道的BGR格式图，所以按下面的顺序获取颜色
			p.b = rgb.ptr<uchar>(m)[n * 3];
			p.g = rgb.ptr<uchar>(m)[n * 3 + 1];
			p.r = rgb.ptr<uchar>(m)[n * 3 + 2];

			// 把p加入到点云中
			cloud->points.push_back(p);
		}
	// 设置并保存点云
	cloud->height = 1;
	cloud->width = cloud->points.size();
	cout << "point cloud size = " << cloud->points.size() << endl;
	cloud->is_dense = false;

	//pcl::io::savePCDFile("D:\\testData\\tskuPointCloud.pcd", *cloud);
	//// 清除数据并退出
	//cloud->points.clear();
	//cout << "Point cloud saved." << endl;



	//pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZRGBA>);
	//pcl::io::loadPCDFile("D:\\testData\\tskuPointCloud.pcd", *cloud);

	pcl::visualization::CloudViewer viewer("Cloud Viewer");



	//blocks until the cloud is actually rendered
	viewer.showCloud(cloud);

	//use the following functions to get access to the underlying more advanced/powerful
	//PCLVisualizer

	//This will only get called once
	viewer.runOnVisualizationThreadOnce(viewerOneOff);

	//This will get called once per visualization iteration
	viewer.runOnVisualizationThread(viewerPsycho);
	while (!viewer.wasStopped())
	{
		//you can also do cool processing here
		//FIXME: Note that this is running in a separate thread from viewerPsycho
		//and you should guard against race conditions yourself...
		user_data++;
	}

	return 0;
}
