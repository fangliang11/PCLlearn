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
	viewer.setBackgroundColor(0.5, 0.5, 1.0);  //���ñ���
	pcl::PointXYZ o;   //��������
	o.x = 1.0;
	o.y = 0;
	o.z = 0;
	viewer.addSphere(o, 0.25, "sphere", 0);  // ��������
	std::cout << "i only run once" << std::endl;
}

//��ʾ�ַ�
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

// �����������
typedef pcl::PointXYZRGBA PointT;
typedef pcl::PointCloud<PointT> PointCloud;


// ����ڲ�
const double camera_factor = 1000;
const double camera_cx = 325.5;
const double camera_cy = 253.5;
const double camera_fx = 518.0;
const double camera_fy = 519.0;

// ������
int main(int argc, char** argv)
{
	// ��ȡ./data/rgb.png��./data/depth.png����ת��Ϊ����
	cv::Mat rgb, depth;
	rgb = cv::imread("D:\\testData\\left.png");	// rgb ͼ����8UC3�Ĳ�ɫͼ��
	depth = cv::imread("D:\\testData\\disp_SGBM.png", -1); 	// depth ��16UC1�ĵ�ͨ��ͼ��ע��flags����-1,��ʾ��ȡԭʼ���ݲ����κ��޸�
	
	PointCloud::Ptr cloud(new PointCloud);  // ʹ������ָ�룬����һ���յ��ơ�����ָ��������Զ��ͷš�

    // �������ͼ m �У� n ��
	for (int m = 0; m < depth.rows; m++)
		for (int n = 0; n < depth.cols; n++)
		{
			ushort d = depth.ptr<ushort>(m)[n];  //���ͼ��� m �У� �� n �е�����ֵ
			// d ����û��ֵ������ˣ������˵�
			if (d == 0)
				continue;
			// d ����ֵ�������������һ����
			PointT p;

			// ���������Ŀռ����꣨x, y, z)
			p.z = double(d) / camera_factor;
			p.x = (n - camera_cx) * p.z / camera_fx;
			p.y = (m - camera_cy) * p.z / camera_fy;

			// ��rgbͼ���л�ȡ������ɫ
			// rgb����ͨ����BGR��ʽͼ�����԰������˳���ȡ��ɫ
			p.b = rgb.ptr<uchar>(m)[n * 3];
			p.g = rgb.ptr<uchar>(m)[n * 3 + 1];
			p.r = rgb.ptr<uchar>(m)[n * 3 + 2];

			// ��p���뵽������
			cloud->points.push_back(p);
		}
	// ���ò��������
	cloud->height = 1;
	cloud->width = cloud->points.size();
	cout << "point cloud size = " << cloud->points.size() << endl;
	cloud->is_dense = false;

	//pcl::io::savePCDFile("D:\\testData\\tskuPointCloud.pcd", *cloud);
	//// ������ݲ��˳�
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
