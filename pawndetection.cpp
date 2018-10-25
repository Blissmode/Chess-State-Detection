#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <math.h>

using namespace cv;



int match_method;

int main()
{
Mat img1 = imread("chesscropped.PNG",1);
Mat pawn1 = imread ("pawnele.PNG", 1); 


Point matchLoc;
  /// Create the result matrix
  int result_cols =  img1.cols - pawn1.cols + 1;
  int result_rows = img1.rows - pawn1.rows + 1;

 Mat result(result_rows,result_cols,CV_8UC1,Scalar(0));

  /// Do the Matching and Normalize
  matchTemplate( img1,pawn1, result, CV_TM_CCORR );
 // normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

  /// Localizing the best match with minMaxLoc
  double minVal; double maxVal; Point minLoc; Point maxLoc;
  

  minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
  if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED )
    { matchLoc = minLoc; }
  else
    { matchLoc = maxLoc; }


  
  /// Show me what you got
  rectangle( img1, matchLoc, Point( matchLoc.x + pawn1.cols , matchLoc.y + pawn1.rows ), Scalar::all(0), 2, 8, 0 );
  //rectangle( result, matchLoc, Point( matchLoc.x + pawn1.cols , matchLoc.y + pawn1.rows ), Scalar::all(0), 2, 8, 0 );
imshow("Cropped portion1",img1);
waitKey(0);
     
   
}


