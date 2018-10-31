#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <iostream>
#include <cstdlib>
using namespace cv;
using namespace std;


int main(){
	int i,j;
	Mat img = imread("images/krani.PNG",1);
	Mat img1(img.rows,img.cols,CV_8UC3,Scalar(255,255,255)); 
	img1 = img ;
	

	for (i = 0; i < img.rows; i++)
	{
	    for (j = 0; j < img.cols; j++)
		{ 
			if((img.at<Vec3b>(i, j)[2]+img.at<Vec3b>(i, j)[1]+img.at<Vec3b>(i, j)[0])/3<115)img1.at<Vec3b>(i, j)={240,240,240};

		}
	}
	imshow("you",img1);
	waitKey(0);
	imwrite("srani.PNG",img1);	
	
}