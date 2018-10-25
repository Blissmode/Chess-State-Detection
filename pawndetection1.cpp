#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <iostream>
using namespace cv;
using namespace std;
struct template
{
  Mat img;
  int state;
  int count;
  char piece;
}
int match_method;
int main()
{
 template Arr[12];

 Mat img= imread("chesscropped.PNG",1);
 Arr[0].img = imread("sraja.PNG",1);
 Arr[1].img = imread("srani.PNG",1);
 Arr[2].img = imread("sghoda.PNG",1);
 Arr[3].img = imread("suunt.PNG",1);
 Arr[4].img = imread("shathi.PNG",1);
 Arr[5].img = imread("spyada.PNG",1);
 Arr[6].img = imread("kraja.PNG",1);
 Arr[7].img = imread("krani.PNG",1);
 Arr[8].img = imread("kghoda.PNG",1);
 Arr[9].img = imread("kuunt.PNG",1);
 Arr[10].img = imread("khathi.PNG",1);
 Arr[11].img = imread("kpyada.PNG",1);
 Arr[].piece={'k','q','n','b','r','p','k','q','n','b','r','p'};
 Arr[].state={1,1,1,1,1,1,0,0,0,0,0,0};
 Arr[].count={1,1,2,2,2,8,1,1,2,2,2,8};


 imshow("Cropped portion1",img);
waitKey(0);

 Mat img1(img.rows, img.cols, CV_8UC1, Scalar(0));
Mat  img2(img.rows, img.cols, CV_8UC1, Scalar(0));
int rmax = 133, rmin = 105, gmin = 140, gmax = 191, bmax = 101, bmin = 0;
 int i, j,k,x,y;
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
  for(k=0;k<12;k++)
  {
  Mat Arr[k].img1(Arr[k].img.rows, Arr[k].img.cols, CV_8UC1, Scalar(0));
   
	for (i = 0; i < Arr[k].img.rows; i++)
	{
	    for (j = 0; j < Arr[k].img.cols; j++)
		{
	    	 if ((Arr[k].img.at<Vec3b>(i, j)[2]>=rmin) && (Arr[k].img.at<Vec3b>(i, j)[2]<=rmax) && (Arr[k].img.at<Vec3b>(i, j)[1]>=gmin) && (Arr[k].img.at<Vec3b>(i, j)[1]<=gmax) &&          (Arr[k].img.at<Vec3b>(i, j)[0]>=bmin) && (Arr[k].img.at<Vec3b>(i, j)[0] <= bmax))
		    {
			    Arr[k].img.at<Vec3b>(i, j) ={255,255,255};
	     }
                  // if((Arr[k].img.at<Vec3b>(i, j)[2]+Arr[k].img.at<Vec3b>(i, j)[1]+Arr[k].img.at<Vec3b>(i, j)[0])/3>127)
                  //   Arr[k].img.at<uchar>(i,j)=255;
                  // else
                   //  Arr[k].img.at<uchar>(i,j)=0;
                  //}
  	}
  } 
//imshow("Cropped portion1",img1);
//waitKey(0);
//imshow("Cropped portion1",Arr[k].img1);
//waitKey(0);
for(i=0;i<12;i++)
  {
     for(j=1;j<=Arr[i].count;j++)
     {
  Point matchLoc;
  /// Create the result matrix
  int result_cols =  img1.cols - Arr[k].img.cols + 1;
  int result_rows = img1.rows - Arr[k].img.rows + 1;
 Mat result(img.rows,img.cols,CV_8UC1,Scalar(0));
  /// Do the Matching and Normalize
    matchTemplate( img1,Arr[k].img, result, CV_TM_SQDIFF );
  
  normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );
  /// Localizing the best match with minMaxLoc
  double minVal; double maxVal; Point minLoc; Point maxLoc;
  
  minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
    if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED )
    { matchLoc = minLoc; }
  else
    { matchLoc = maxLoc; }
  
  /// Show me what you go
  rectangle( img, matchLoc, Point( matchLoc.x + Arr[k].img.cols , matchLoc.y + Arr[k].img.rows ), Scalar::all(0), 2, 8, 0 );
  for(x=0;x<8;x++)
  {
    for(y=0;y<8;y++)
    {
      if(abs(board[x][y].ltop.first-matchLoc.x)<3 && abs(board[x][y].ltop.second-matchLoc.y)<3)
      {
        board[x][y].state=Arr[i].state;
        board[x][y].piece=Arr[i].piece;

      }
    }
   }

  }
  }
   for(x=0;x<8;x++)
  {
    for(y=0;y<8;y++)
    {
      
      cout<<x<<" ,"<<y<<endl;
      cout<<board[x][y].piece<<","<<board[x][y].state;      }
    }
   }


  //rectangle( result, matchLoc, Point( matchLoc.x + temp.cols , matchLoc.y + temp.rows ), Scalar::all(0), 2, 8, 0 );
imshow("Cropped portion1",img);
waitKey(0);
     
   
}