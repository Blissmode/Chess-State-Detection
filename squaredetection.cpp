#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <iostream>
using namespace cv;
using namespace std;


struct sqlocation
{
 int x1;
 int y1;
 int x2;
 int y2;
 int n;
 };
struct sqstatus
 {
 int n;
 char pawn;
 };
 int main()
{
  int startX=0,startY=0,endX,endY;
 Mat chessboard = imread("chesscropped.PNG",1);
 struct sqlocation matrix[64];
 int flag=0;
 int SQUARE_SIZEX = chessboard.rows/8;
 int SQUARE_SIZEY = chessboard.cols/8;
 cout<<chessboard.rows<<endl;
 cout<<chessboard.cols<<endl;
 cout<<SQUARE_SIZEY<<endl;
 for (int i=0;i<8;i++)
    { 

     for (int j=0;j<8;j++)
      {
        startX =  SQUARE_SIZEX*i;
        startY =  SQUARE_SIZEX*j; 
        matrix[flag].x1=startX;
        matrix[flag].y1=startY;
        endX = startX + SQUARE_SIZEY;
        endY = startY + SQUARE_SIZEY;
        matrix[flag].x2=endX;
        matrix[flag].y2=endY;
        matrix[flag].n=flag;
        flag++;
        rectangle(chessboard, Point(startX,startY), Point(endX,endY), Scalar(255,0,0), 3, 8, 0);
      }
    }
  for (int i=0;i<63;i++)
    {
      cout<< matrix[i].x1<<"  "<<matrix[i].y1<<endl;
      cout<< matrix[i].x2<<"  "<<matrix[i].y2<<endl;
    }
 imshow("chessboard", chessboard);
 
 waitKey(0); 
 return 0; 
}


