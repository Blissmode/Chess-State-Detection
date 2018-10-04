//An Chess Engine for predicting next move using a Screenshot//


#include <opencv2/opencv.hpp>
#include <bits/stdc++.h>
using namespace cv;
using namespace std;

int rmax = 133, rmin = 105, gmin = 140, gmax = 191, bmax = 101, bmin = 0;
Mat img;


int main()
{



    img = imread("images/startpos3.png", 1);
	// namedWindow("Image for thresholding", WINDOW_NORMAL);
	// createTrackbar("red max", "Image for thresholding", &rmax, 255);
	// createTrackbar("red min", "Image for thresholding", &rmin, 255);
	// createTrackbar("green max", "Image for thresholding", &gmax, 255);
	// createTrackbar("green min", "Image for thresholding", &gmin, 255);
	// createTrackbar("blue max", "Image for thresholding", &bmax, 255);
	// createTrackbar("blue mbx", "Image for thresholding", &bmin, 255);

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
	// namedWindow("Image for thresholding", WINDOW_NORMAL);
    medianBlur( img1, img1, 3 );
	// imshow("Image for thresholding", img1);
	// waitKey(0);

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

	cout<<x1<<" "<<y1<<endl;
	cout<<y1<<" "<<y2<<endl;
	cout<<img.rows<<" "<<img.cols<<endl;

	//Displaying the point obtained above
 	// circle( img, Point( y1, x1 ), 8.0, Scalar( 0, 0, 255 ), 1, 8 );
	// circle( img, Point( y2, x2 ), 8.0, Scalar( 0, 0, 255 ), 1, 8 ); 
  	// imshow("The required points for cropping out",img);

	int startX=y2,startY=x1,height=x2-x1,width=y1-y2;

    Mat ROI(img, Rect(startX,startY,width,height));
    Mat croppedImage;
    ROI.copyTo(croppedImage);
    imshow("Cropped portion",croppedImage);
    waitKey(0);    
}
