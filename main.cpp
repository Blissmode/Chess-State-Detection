//A Chess Engine for predicting next move using a Screenshot//


#include <opencv2/opencv.hpp>
#include <bits/stdc++.h>
using namespace cv;
using namespace std;

int rmax = 133, rmin = 105, gmin = 140, gmax = 191, bmax = 101, bmin = 0;
int rmax1 = 140, rmin1 = 67, gmin1 = 0, gmax1 = 148, bmax1 = 156, bmin1 = 0;
Mat img;

struct square
{
	pair <int,int> ltop;
	pair <int,int> rbottom;
	char piece;
	int state;
};

square board[8][8];

Mat imgz;
Mat result;
Mat templ;
string image_window = "Source Image";
string result_window = "Result window";

int max_Trackbar = 5;

/// Function Headers
void MatchingMethod( int, void*,int,string );


int main()
{
    img = imread("images/absstart.png", 1);
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
	

	for(int i=0;i<8;i++)
	{
		for(int j=1;j<8;j++)
		{
			board[i][j].ltop.first=board[i][j-1].ltop.first+rowIncrement;
			board[i][j].rbottom.first=board[i][j-1].rbottom.first+rowIncrement;
			board[i][j].ltop.second=board[i][j-1].ltop.second;
			board[i][j].rbottom.second=board[i][j-1].rbottom.second;
		}
	}

	// UNCOMMENT FOR POINT DETECTION PART
	// for(int i=0;i<8;i++)
	// {
	// 	for(int j=0;j<8;j++)
	// 	{
	// 		circle( src, Point( board[i][j].ltop.first,board[i][j].ltop.second ), 32.0, Scalar( 0, 0, 255 ), 1, 8 );
	// 		circle( src, Point( board[i][j].rbottom.first,board[i][j].rbottom.second ), 32.0, Scalar( 0, 0, 255 ), 1, 8 );
	// 		cout<<board[i][j].ltop.first<<" "<<board[i][j].ltop.second<<"           ";
	// 	}
	// 	cout<<endl;

	// imshow("point detection part",src);
	imshow("Source after cropping",src);
	namedWindow("Source after Threshold", WINDOW_NORMAL);

	

	Mat src1(src.rows, src.cols, CV_8UC1, Scalar(0));
	cout<<src.rows<<" "<<src.cols<<endl;
	for (int i = 0; i < src.rows; i++)
	{
	    for (int j = 0; j < src.cols; j++)
		{
	    	if ((src.at<Vec3b>(i, j)[2]>=rmin1) && (src.at<Vec3b>(i, j)[2]<=rmax1) && (src.at<Vec3b>(i, j)[1]>=gmin1) && (src.at<Vec3b>(i, j)[1]<=gmax1) && (src.at<Vec3b>(i, j)[0]>=bmin1) && (src.at<Vec3b>(i, j)[0] <= bmax1))
		    {
			    src1.at<uchar>(i, j) = 255;
	    	}
		    else
			    src1.at<uchar>(i, j) = 0;
	    }
	}

	medianBlur( src1, src1, 3 );
	medianBlur( src1, src1, 3 );
	imshow("Source after Threshold", src1);

	imwrite("source.png",src1);
	
	//template matching initiate
	
	imgz = imread("source.png",1);
	




	//INTIATING TEMPLATE DETECTION

    MatchingMethod( 0, 0 ,3,"images/bpawn.png");
	MatchingMethod( 0, 0 ,3,"images/wpawn.png");
	MatchingMethod( 0, 0 ,3,"images/bknight.png");
	MatchingMethod( 0, 0 ,1,"images/wknight.png");
	MatchingMethod( 0, 0 ,3,"images/bqueen.png");
	MatchingMethod( 0, 0 ,1,"images/wqueen.png");
	MatchingMethod( 0, 0 ,3,"images/bking.png");
	MatchingMethod( 0, 0 ,1,"images/wking.png");
	MatchingMethod( 0, 0 ,3,"images/bbishop.png");
	MatchingMethod( 0, 0 ,1,"images/wbishop.png");
	MatchingMethod( 0, 0 ,3,"images/brook.png");
	MatchingMethod( 0, 0 ,1,"images/wrook.png");

    waitKey(0);
    return 0;
}



void MatchingMethod( int, void* ,int match_method,string name)
{
      /// Source image to display
      Mat img_display;
      imgz.copyTo( img_display );

	  templ=imread(name,1);

      /// Create the result matrix
      int result_cols =  imgz.cols - templ.cols + 1;
      int result_rows = imgz.rows - templ.rows + 1;

      result.create( result_cols, result_rows, CV_32FC1 );

      /// Do the Matching and Normalize
      matchTemplate( imgz, templ, result, match_method );
      normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

      /// Localizing the best match with minMaxLoc
      Point minLoc; Point maxLoc;
      Point matchLoc;
      double minVal; double maxVal;

      for(int k=1;k<=8;k++)
      {
        minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

        // cout<<"Match #"<<k<<" occurs at:"<<result.at<float>(minLoc.x,minLoc.y)<<endl;
        cout<<"Loacation:"<<matchLoc.x<<","<<matchLoc.y<<endl;
        // Lowest matches
        if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED )
        {
            result.at<float>(minLoc.x,minLoc.y)=1.0;
            result.at<float>(maxLoc.x,maxLoc.y)=1.0;
        }
        else
        {
            result.at<float>(minLoc.x,minLoc.y)=0.0;
            result.at<float>(maxLoc.x,maxLoc.y)=0.0;
        }

      /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
      if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED )
        { matchLoc = minLoc; }
      else
        { matchLoc = maxLoc; }

      /// Show me what you got
      rectangle( img_display, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar(0,0,255), 2, 8, 0 );
      rectangle( result, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar(0,0,255), 2, 8, 0 );
      }
      imshow( name, img_display );

      return;
    }