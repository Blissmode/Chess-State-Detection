#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <iostream>
using namespace cv;
using namespace std;

int match_method;
int main()
{
 Mat img= imread("chesscropped.PNG",1);
 Mat pawn = imread("raja.PNG",1);
 imshow("Cropped portion1",img);
waitKey(0);
 Mat img1(img.rows, img.cols, CV_8UC1, Scalar(0));
Mat img2(img.rows, img.cols, CV_8UC1, Scalar(0));
Mat pawn1(pawn.rows, pawn.cols, CV_8UC1, Scalar(0));
 int rmax = 133, rmin = 105, gmin = 140, gmax = 191, bmax = 101, bmin = 0;
 int i, j;
	for (i = 0; i < img.rows; i++)
	{
	    for (j = 0; j < img.cols; j++)
		{
	    	 if ((img.at<Vec3b>(i, j)[2]>=rmin) && (img.at<Vec3b>(i, j)[2]<=rmax) && (img.at<Vec3b>(i, j)[1]>=gmin) && (img.at<Vec3b>(i, j)[1]<=gmax) &&          (img.at<Vec3b>(i, j)[0]>=bmin) && (img.at<Vec3b>(i, j)[0] <= bmax))
		    {
			    img.at<Vec3b>(i, j) ={255,255,255};
	             }
                   if((img.at<Vec3b>(i, j)[2]+img.at<Vec3b>(i, j)[1]+img.at<Vec3b>(i, j)[0])/3>127)
                     img1.at<uchar>(i,j)=255;
                   else
                     img1.at<uchar>(i,j)=0;
                  }
  	}
	for (i = 0; i < pawn.rows; i++)
	{
	    for (j = 0; j < pawn.cols; j++)
		{
	    	 if ((pawn.at<Vec3b>(i, j)[2]>=rmin) && (pawn.at<Vec3b>(i, j)[2]<=rmax) && (pawn.at<Vec3b>(i, j)[1]>=gmin) && (pawn.at<Vec3b>(i, j)[1]<=gmax) &&          (pawn.at<Vec3b>(i, j)[0]>=bmin) && (pawn.at<Vec3b>(i, j)[0] <= bmax))
		    {
			    pawn.at<Vec3b>(i, j) ={255,255,255};
	             }
                   if((pawn.at<Vec3b>(i, j)[2]+pawn.at<Vec3b>(i, j)[1]+pawn.at<Vec3b>(i, j)[0])/3>127)
                     pawn1.at<uchar>(i,j)=255;
                   else
                     pawn1.at<uchar>(i,j)=0;
                  }
  	} 
imshow("Cropped portion1",img1);
waitKey(0);
imshow("Cropped portion1",pawn1);
waitKey(0);
 
Point matchLoc;
  /// Create the result matrix
  int result_cols =  img1.cols - pawn1.cols + 1;
  int result_rows = img1.rows - pawn1.rows + 1;
 Mat result(img.rows,img.cols,CV_8UC1,Scalar(0));
  /// Do the Matching and Normalize
  matchTemplate( img1,pawn1, result, CV_TM_SQDIFF );
  normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );
  /// Localizing the best match with minMaxLoc
  double minVal; double maxVal; Point minLoc; Point maxLoc;
  
  minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
    if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED )
    { matchLoc = minLoc; }
  else
    { matchLoc = maxLoc; }
  
  /// Show me what you got
  rectangle( img, matchLoc, Point( matchLoc.x + pawn1.cols , matchLoc.y + pawn1.rows ), Scalar::all(0), 2, 8, 0 );
  //rectangle( result, matchLoc, Point( matchLoc.x + temp.cols , matchLoc.y + temp.rows ), Scalar::all(0), 2, 8, 0 );
imshow("Cropped portion1",img);
waitKey(0);
     
   
}