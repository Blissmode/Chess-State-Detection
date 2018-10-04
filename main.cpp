#include <opencv2/opencv.hpp>
#include <bits/stdc++.h>
using namespace cv;
using namespace std;

int rmax = 133, rmin = 105, gmin = 140, gmax = 191, bmax = 101, bmin = 0;
Mat img;


int main()
{



    img = imread("images/startpos.png", 1);
	namedWindow("abc", WINDOW_NORMAL);
	// createTrackbar("red max", "abc", &rmax, 255);
	// createTrackbar("red min", "abc", &rmin, 255);
	// createTrackbar("green max", "abc", &gmax, 255);
	// createTrackbar("green min", "abc", &gmin, 255);
	// createTrackbar("blue max", "abc", &bmax, 255);
	// createTrackbar("blue mbx", "abc", &bmin, 255);

    Mat img1(img.rows, img.cols, CV_8UC1, Scalar(0));
	int i, j;
	for (i = 0; i < img.rows; i++)
	{
	    for (j = 0; j < img.cols; j++)
		{
	    	if ((img.at<Vec3b>(i, j)[2]>=rmin) && (img.at<Vec3b>(i, j)[2]<=rmax) && (img.at<Vec3b>(i, j)[1]>=gmin) && (img.at<Vec3b>(i, j)[1]<=gmax) && (img.at<Vec3b>(i, j)[0]>=bmin) && (img.at<Vec3b>(i, j)[0] <= bmax))
		    {
			    img1.at<uchar>(i, j) = 255;
	    	}
		    else
			    img1.at<uchar>(i, j) = 0;
	    }
	}
	namedWindow("abc", WINDOW_NORMAL);
    medianBlur( img1, img1, 3 );
	imshow("abc", img1);
	waitKey(0);

    int x1=-INT_MAX,y1=-INT_MAX,x2,y2;

    // (x1,y1) is uppermost rightmost point
    // (x2,y2) is lowermost leftmost point
    int flag=0;
    for (i = 0; i < img1.rows; i++)
	{
	    for (j = img1.cols-1; j; j--)
		{
	    	if (img1.at<uchar>(i, j) == 255)
		    {
			  x1=i;
              y1=j;
              flag=1;
              break;
	    	}
	    }

        if(flag)
        break;
	}

    flag=0;
    for (i = img.rows-1; i ; i--)
	{
	    for (j = 0; j < img.cols ; j++)
		{
	    	if (img1.at<uchar>(i, j) == 255)
		    {
			  x2=i;
              y2=j;
              flag=1;
              break;
	    	}
	    }

        if(flag)
        break;
	}
    waitKey(0);    
}
