//An Chess Engine for predicting next move using a Screenshot//


#include <opencv2/opencv.hpp>
#include <bits/stdc++.h>
using namespace cv;
using namespace std;

int rmax = 133, rmin = 105, gmin = 140, gmax = 191, bmax = 101, bmin = 0;
Mat img;

struct square
{
	pair <int,int> ltop;
	pair <int,int> rbottom;
	char piece;
	int state;
};

square board[8][8];

int main()
{
    img = imread("images/startpos3.png", 1);
    Mat img1(img.rows, img.cols, CV_8UC1, Scalar(0));
	int i, j;
	//Colour Detection to remove the unwanted parts in the image
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
	//To remove the salt and pepper noise left after thresholding
    medianBlur( img1, img1, 3 );
	
	
    int x1,y1,x2,y2;

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
 	// circle( img, Point( y1, x1 ), 32.0, Scalar( 0, 0, 255 ), 1, 8 );
	// circle( img, Point( y2, x2 ), 32.0, Scalar( 0, 0, 255 ), 1, 8 ); 
  	// imshow("The required points for cropping out",img);

	int startX=y2,startY=x1,height=x2-x1,width=y1-y2;

    Mat ROI(img, Rect(startX,startY,width,height));
    Mat croppedImage;
    ROI.copyTo(croppedImage);
    // imshow("Cropped portion",croppedImage);

	Mat src=croppedImage;
	int rows=src.rows;
	int cols=src.cols;
	int rowIncrement=rows/8;
	int colIncrement=cols/8;

	board[0][0].ltop.first=0;
	board[0][0].ltop.second=0;
	board[0][0].rbottom.first=rowIncrement;
	board[0][0].rbottom.second=colIncrement;

	for(int i=1;i<8;i++)
	{
		board[i][0].ltop.first=board[i-1][0].ltop.first;
		board[i][0].ltop.second=board[i-1][0].ltop.second+colIncrement;
		board[i][0].rbottom.first=board[i-1][0].rbottom.first;
		board[i][0].rbottom.second=board[i-1][0].rbottom.second+colIncrement;
	}

	cout<<endl;
	for(int i=0;i<8;i++)
	{
		for(int j=1;j<8;j++)
		{
			board[i][j].ltop.first=board[i][j-1].ltop.first+rowIncrement;
			board[i][j].rbottom.first=board[i][j-1].rbottom.first+rowIncrement;
			board[i][j].ltop.second=board[i][j-1].ltop.second;
			board[i][j].rbottom.second=board[i][j-1].rbottom.second;
		}
		cout<<endl;
	}

	for(int i=0;i<8;i++)
	{
		for(int j=0;j<8;j++)
		{
			circle( src, Point( board[i][j].ltop.first,board[i][j].ltop.second ), 32.0, Scalar( 0, 0, 255 ), 1, 8 );
			circle( src, Point( board[i][j].rbottom.first,board[i][j].rbottom.second ), 32.0, Scalar( 0, 0, 255 ), 1, 8 );
			cout<<board[i][j].ltop.first<<" "<<board[i][j].ltop.second<<"           ";
		}
		cout<<endl;
	}


	imshow("point detection part",src);

	waitKey(0);
}
