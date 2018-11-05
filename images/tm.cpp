#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <bits/stdc++.h>
#include <stdio.h>

using namespace std;
using namespace cv;

/// Global Variables
Mat imgz; Mat templ; Mat result;
char* image_window = "Source Image";
char* result_window = "Result window";

int match_method=2;
int max_Trackbar = 5;

/// Function Headers
void MatchingMethod( int, void* );

/** @function main */
int main( int argc, char** argv )
{
  /// Load image and template
  string templateName="wpawn.png";
  imgz = imread( "templateThreshold.png", 1 );
  templ = imread( templateName, 1 );

  /// Create windows
  namedWindow( image_window, CV_WINDOW_AUTOSIZE );
  namedWindow( result_window, CV_WINDOW_AUTOSIZE );

  /// Create Trackbar
  char* trackbar_label = "Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED";
  createTrackbar( trackbar_label, image_window, &match_method, max_Trackbar, MatchingMethod );

  MatchingMethod( 0, 0 );

  waitKey(0);
  return 0;
}

/**
 * @function MatchingMethod
 * @brief Trackbar callback
 */
void MatchingMethod( int, void* )
{
      /// Source image to display
      Mat img_display;
      imgz.copyTo( img_display );

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
      imshow( image_window, img_display );
      imshow( result_window, result );

      return;
    }