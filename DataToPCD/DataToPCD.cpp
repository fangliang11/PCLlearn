#pragma warning (disable : 4996)

#include<iostream>
#include<fstream>
#include <string>
#include <vector>
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

using namespace std;

typedef struct txtPoint_3D {
	double x;
	double y;
	double z;
	double r;
}txtPoint_3D;

/*
	�� txt ��ʽ���ļ��ж�ȡ�������ݣ�Ȼ���ٴ洢Ϊpcd��ʽ�ļ�
	readPath ��ȡ txt ��ʽ�ļ�·��
	savePath д�� pcd ��ʽ�ļ�·��
*/
void bin2pcds(const char* readPath, const char* savePath) {

	FILE *fp_txt;
	txtPoint_3D txtPoint;
	vector<txtPoint_3D> txtPoints;
	fp_txt = fopen(readPath, "r");

	if (fp_txt) {
		while (fscanf(fp_txt, "%lf %lf %lf %lf", &txtPoint.x, &txtPoint.y, &txtPoint.z, &txtPoint.r) != EOF) {
			txtPoints.push_back(txtPoint);
		}
	}

	pcl::PointCloud<pcl::PointXYZ> cloud;

	cloud.width = txtPoints.size();
	cloud.height = 1;
	cloud.is_dense = false;
	cloud.points.resize(cloud.width * cloud.height);

	for (size_t i = 0; i < cloud.points.size(); ++i) {
		cloud.points[i].x = txtPoints[i].x;
		cloud.points[i].y = txtPoints[i].y;
		cloud.points[i].z = txtPoints[i].z;
	}
	pcl::io::savePCDFileASCII(savePath, cloud);
}



int main() {

	ifstream myfile("D:\\testData\\MYdata1126_source.dat");
	if (!myfile.is_open()) {
		cout << "Unable to open myfile";
		system("pause");
		exit(1);
	}

	vector<string> vec;
	string temp;

	while (getline(myfile, temp))                    //����getline������ȡÿһ�У���������Ϊ��λ���뵽vector
	{
		if (!temp.empty()) {
			if (temp[0] == 'x' || temp[0] == '#') {
				temp.clear();  //������ͷ
			}
			else vec.push_back(temp);
		}
	}
	myfile.close();

	vector<float> vectorX;
	vector<float> vectorY;
	vector<float> vectorZ;
	for (auto it = vec.begin(); it != vec.end(); it++)
	{
		istringstream is(*it);                    //��ÿһ�е����ݳ�ʼ��һ���ַ�����������
		string s;
		int pam = 1;                              //�ӵ�һ�п�ʼ

		while (is >> s)                          //�Կո�Ϊ�磬��istringstream������ȡ�����뵽����s��
		{
			if (pam == 1)                       //��ȡ�� P �е�����
			{
				float r = atof(s.c_str());     //����������ת������string����ת����float
				vectorX.push_back(r);
			}
			if (pam == 2) {
				float y = atof(s.c_str());
				vectorY.push_back(y);
			}
			if (pam == 3) {
				float z = atof(s.c_str());
				vectorZ.push_back(z);
			}

			pam++;

		}
	}

	pcl::PointCloud<pcl::PointXYZ> cloud;

	cloud.width = vec.size();
	cloud.height = 1;
	cloud.is_dense = false;
	cloud.points.resize(cloud.width * cloud.height);

	for (size_t i = 0; i < cloud.points.size(); ++i) {
		cloud.points[i].x = vectorX[i];
		cloud.points[i].y = vectorY[i];
		cloud.points[i].z = vectorZ[i] * 20;
	}
	pcl::io::savePCDFileASCII("D:\\testData\\MYdata1126_source.pcd", cloud);

	cout << "finish to convert" << endl;

	system("pause");
	return 1;
}
