// An Chess Engine for predicting next move using a Screenshot//

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
using namespace cv;
using namespace std;

int rmax = 133, rmin = 105, gmin = 140, gmax = 191, bmax = 101, bmin = 0;
Mat img;
int match_method;
struct square {
  pair<int, int> ltop;
  pair<int, int> rbottom;
  char piece;
  int state;
};
struct Template {
  Mat img;
  Mat img1;
  int state;
  int count;
  char piece;
};

square board[8][8];

int main() {
  img = imread("images/startpos3.png", 1);
  Mat img1(img.rows, img.cols, CV_8UC1, Scalar(0));
  int i, j, x, y, k;
  Template Arr[12];

  Arr[0].img = imread("images/sraja.PNG", 1);
  Arr[1].img = imread("images/srani.PNG", 1);
  Arr[2].img = imread("images/sghoda.PNG", 1);
  Arr[3].img = imread("images/suunt.PNG", 1);
  Arr[4].img = imread("images/shathi.PNG", 1);
  Arr[5].img = imread("images/spyada.PNG", 1);
  Arr[6].img = imread("images/kraja.PNG", 1);
  Arr[7].img = imread("images/krani.PNG", 1);
  Arr[8].img = imread("images/kghoda.PNG", 1);
  Arr[9].img = imread("images/kuunt.PNG", 1);
  Arr[10].img = imread("images/khathi.PNG", 1);
  Arr[11].img = imread("images/kpyada.PNG", 1);
  Arr[0].img1 = imread("images/sraja.PNG", 0);
  Arr[1].img1 = imread("images/srani.PNG", 0);
  Arr[2].img1 = imread("images/sghoda.PNG", 0);
  Arr[3].img1 = imread("images/suunt.PNG", 0);
  Arr[4].img1 = imread("images/shathi.PNG", 0);
  Arr[5].img1 = imread("images/spyada.PNG", 0);
  Arr[6].img1 = imread("images/kraja.PNG", 0);
  Arr[7].img1 = imread("images/krani.PNG", 0);
  Arr[8].img1 = imread("images/kghoda.PNG", 0);
  Arr[9].img1 = imread("images/kuunt.PNG", 0);
  Arr[10].img1 = imread("images/khathi.PNG", 0);
  Arr[11].img1 = imread("images/kpyada.PNG", 0);
  Arr[0].piece = Arr[6].piece = 'k';
  Arr[1].piece = Arr[7].piece = 'q';
  Arr[2].piece = Arr[8].piece = 'n';
  Arr[3].piece = Arr[9].piece = 'b';
  Arr[4].piece = Arr[10].piece = 'r';
  Arr[5].piece = Arr[11].piece = 'p';
  Arr[0].state = Arr[1].state = Arr[2].state = Arr[3].state = Arr[4].state =
      Arr[5].state = 1;
  Arr[6].state = Arr[7].state = Arr[8].state = Arr[9].state = Arr[10].state =
      Arr[11].state = 0;
  Arr[0].count = Arr[1].count = Arr[6].count = Arr[7].count = 1;
  Arr[2].count = Arr[3].count = Arr[4].count = Arr[8].count = Arr[9].count =
      Arr[10].count = 2;
  Arr[5].count = Arr[11].count = 8;

  // Colour Detection to remove the unwanted parts in the image
  for (i = 0; i < img.rows; i++) {
    for (j = 0; j < img.cols; j++) {
      if ((img.at<Vec3b>(i, j)[2] >= rmin) &&
          (img.at<Vec3b>(i, j)[2] <= rmax) &&
          (img.at<Vec3b>(i, j)[1] >= gmin) &&
          (img.at<Vec3b>(i, j)[1] <= gmax) &&
          (img.at<Vec3b>(i, j)[0] >= bmin) &&
          (img.at<Vec3b>(i, j)[0] <= bmax)) {
        img1.at<uchar>(i, j) = 255;
      } else
        img1.at<uchar>(i, j) = 0;
    }
  }
  // To remove the salt and pepper noise left after thresholding
  medianBlur(img1, img1, 3);

  int x1, y1, x2, y2;

  // (x1,y1) is uppermost rightmost point
  // (x2,y2) is lowermost leftmost point
  int flag = 0;

  for (i = 0; i < img1.rows; i++) {
    for (j = img1.cols - 1; j; j--) {
      if (img1.at<uchar>(i, j) == 255) {
        x1 = i;
        y1 = j;
        flag = 1;
        break;
      }
    }

    if (flag)
      break;
  }

  flag = 0;

  for (i = img.rows - 1; i; i--) {
    for (j = 0; j < img.cols; j++) {
      if (img1.at<uchar>(i, j) == 255) {
        x2 = i;
        y2 = j;
        flag = 1;
        break;
      }
    }

    if (flag)
      break;
  }

  // cout<<x1<<" "<<y1<<endl;
  // cout<<y1<<" "<<y2<<endl;
  // cout<<img.rows<<" "<<img.cols<<endl;

  // Displaying the point obtained above
  // circle( img, Point( y1, x1 ), 32.0, Scalar( 0, 0, 255 ), 1, 8 );
  // circle( img, Point( y2, x2 ), 32.0, Scalar( 0, 0, 255 ), 1, 8 );
  // imshow("The required points for cropping out",img);

  int startX = y2, startY = x1, height = x2 - x1, width = y1 - y2;

  Mat ROI(img, Rect(startX, startY, width, height));
  Mat croppedImage;
  ROI.copyTo(croppedImage);
  // imshow("Cropped portion",croppedImage);

  Mat src = croppedImage;
  int rows = src.rows;
  int cols = src.cols;
  int rowIncrement = rows / 8;
  int colIncrement = cols / 8;

  board[0][0].ltop.first = 0;
  board[0][0].ltop.second = 0;
  board[0][0].rbottom.first = rowIncrement;
  board[0][0].rbottom.second = colIncrement;

  for (int i = 1; i < 8; i++) {
    board[i][0].ltop.first = board[i - 1][0].ltop.first;
    board[i][0].ltop.second = board[i - 1][0].ltop.second + colIncrement;
    board[i][0].rbottom.first = board[i - 1][0].rbottom.first;
    board[i][0].rbottom.second = board[i - 1][0].rbottom.second + colIncrement;
  }

  cout << endl;
  for (int i = 0; i < 8; i++) {
    for (int j = 1; j < 8; j++) {
      board[i][j].ltop.first = board[i][j - 1].ltop.first + rowIncrement;
      board[i][j].rbottom.first = board[i][j - 1].rbottom.first + rowIncrement;
      board[i][j].ltop.second = board[i][j - 1].ltop.second;
      board[i][j].rbottom.second = board[i][j - 1].rbottom.second;
    }
    // cout<<endl;
  }

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      // circle( src, Point( board[i][j].ltop.first,board[i][j].ltop.second
      // ), 32.0, Scalar( 0, 0, 255 ), 1, 8 ); circle( src, Point(
      // board[i][j].rbottom.first,board[i][j].rbottom.second ), 32.0, Scalar(
      // 0, 0, 255 ), 1, 8 );
      //	cout<<board[i][j].ltop.first<<" "<<board[i][j].ltop.second<<" ";
    }
    // cout<<endl;
  }

  imshow("point detection part", src);

  /*Mat img= imread("chesscropped.PNG",1);
Mat pawn = imread("pawnele.PNG",1);
Mat img1(img.rows, img.cols, CV_8UC1, Scalar(0));
Mat img2(img.rows, img.cols, CV_8UC1, Scalar(0));
Mat pawn1(pawn.rows, pawn.cols, CV_8UC1, Scalar(0));
int rmax = 133, rmin = 105, gmin = 140, gmax = 191, bmax = 101, bmin = 0;
int i, j;
  for (i = 0; i < img.rows; i++)
  {
      for (j = 0; j < img.cols; j++)
          {
           if ((img.at<Vec3b>(i, j)[2]>=rmin) && (img.at<Vec3b>(i, j)[2]<=rmax)
&& (img.at<Vec3b>(i, j)[1]>=gmin) && (img.at<Vec3b>(i, j)[1]<=gmax) &&
(img.at<Vec3b>(i, j)[0]>=bmin) && (img.at<Vec3b>(i, j)[0] <= bmax))
              {
                      img.at<Vec3b>(i, j) ={255,255,255};
               }
             if((img.at<Vec3b>(i, j)[2]+img.at<Vec3b>(i, j)[1]+img.at<Vec3b>(i,
j)[0])/3>127) img1.at<uchar>(i,j)=255; else img1.at<uchar>(i,j)=0;
            }
  }
  for (i = 0; i < pawn.rows; i++)
  {
      for (j = 0; j < pawn.cols; j++)
          {
           if ((pawn.at<Vec3b>(i, j)[2]>=rmin) && (pawn.at<Vec3b>(i,
j)[2]<=rmax) && (pawn.at<Vec3b>(i, j)[1]>=gmin) && (pawn.at<Vec3b>(i,
j)[1]<=gmax) &&          (pawn.at<Vec3b>(i, j)[0]>=bmin) && (pawn.at<Vec3b>(i,
j)[0] <= bmax))
              {
                      pawn.at<Vec3b>(i, j) ={255,255,255};
               }
             if((pawn.at<Vec3b>(i, j)[2]+pawn.at<Vec3b>(i,
j)[1]+pawn.at<Vec3b>(i, j)[0])/3>127) pawn1.at<uchar>(i,j)=255; else
               pawn1.at<uchar>(i,j)=0;
            }
  }
imshow("Cropped portion1",img1);


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
rectangle( img1, matchLoc, Point( matchLoc.x + pawn1.cols , matchLoc.y +
pawn1.rows ), Scalar::all(0), 2, 8, 0 );
//rectangle( result, matchLoc, Point( matchLoc.x + temp.cols , matchLoc.y +
temp.rows ), Scalar::all(0), 2, 8, 0 ); imshow("Cropped portion1",result);

waitKey(0);
}
*/
  /*for(i=0;i<12;i++){
                  Mat Arr[i].img1.rows=Arr[k].img.rows;
                  Mat Arr[i].img1.cols=Arr[k].img.cols;
  }*/
  int rmax = 133, rmin = 105, gmin = 140, gmax = 191, bmax = 101, bmin = 0;

  for (i = 0; i < img.rows; i++) {
    for (j = 0; j < img.cols; j++) {
      if ((img.at<Vec3b>(i, j)[2] >= rmin) &&
          (img.at<Vec3b>(i, j)[2] <= rmax) &&
          (img.at<Vec3b>(i, j)[1] >= gmin) &&
          (img.at<Vec3b>(i, j)[1] <= gmax) &&
          (img.at<Vec3b>(i, j)[0] >= bmin) &&
          (img.at<Vec3b>(i, j)[0] <= bmax)) {
        img.at<Vec3b>(i, j) = {255, 255, 255};
      }
      if ((img.at<Vec3b>(i, j)[2] + img.at<Vec3b>(i, j)[1] +
           img.at<Vec3b>(i, j)[0]) /
              3 >
          127)
        img1.at<uchar>(i, j) = 255;
      else
        img1.at<uchar>(i, j) = 0;
    }
  }
  for (k = 0; k < 12; k++) {
    // Mat Arr[k].img(Arr[k].img.rows, Arr[k].img.cols, CV_8UC1, Scalar(0));

    for (i = 0; i < Arr[k].img.rows; i++) {
      for (j = 0; j < Arr[k].img.cols; j++) {
        if ((Arr[k].img.at<Vec3b>(i, j)[2] >= rmin) &&
            (Arr[k].img.at<Vec3b>(i, j)[2] <= rmax) &&
            (Arr[k].img.at<Vec3b>(i, j)[1] >= gmin) &&
            (Arr[k].img.at<Vec3b>(i, j)[1] <= gmax) &&
            (Arr[k].img.at<Vec3b>(i, j)[0] >= bmin) &&
            (Arr[k].img.at<Vec3b>(i, j)[0] <= bmax)) {
          Arr[k].img.at<Vec3b>(i, j) = {255, 255, 255};
        }
        if ((Arr[k].img.at<Vec3b>(i, j)[2] + Arr[k].img.at<Vec3b>(i, j)[1] +
             Arr[k].img.at<Vec3b>(i, j)[0]) /
                3 >
            127)
          Arr[k].img1.at<uchar>(i, j) = 255;
        else
          Arr[k].img1.at<uchar>(i, j) = 0;
      }
    }
    // imshow("Cropped portion1",img1);
    // waitKey(0);
    // imshow("Cropped portion1",Arr[k].img1);
    // waitKey(0);
    for (i = 0; i < 12; i++) {
      for (j = 1; j <= Arr[i].count; j++) {
        Point matchLoc;
        /// Create the result matrix
        int result_cols = img1.cols - Arr[k].img.cols + 1;
        int result_rows = img1.rows - Arr[k].img.rows + 1;
        Mat result(img.rows, img.cols, CV_8UC1, Scalar(0));
        /// Do the Matching and Normalize
        matchTemplate(img1, Arr[k].img1, result, CV_TM_SQDIFF);

        normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());
        /// Localizing the best match with minMaxLoc
        double minVal;
        double maxVal;
        Point minLoc;
        Point maxLoc;

        minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
        if (match_method == CV_TM_SQDIFF ||
            match_method == CV_TM_SQDIFF_NORMED) {
          matchLoc = minLoc;
        } else {
          matchLoc = maxLoc;
        }

        /// Show me what you go
        rectangle(
            img, matchLoc,
            Point(matchLoc.x + Arr[k].img.cols, matchLoc.y + Arr[k].img.rows),
            Scalar::all(0), 2, 8, 0);
        for (x = 0; x < 8; x++) {
          for (y = 0; y < 8; y++) {
            if (abs(board[x][y].ltop.first - matchLoc.x) < 3 &&
                abs(board[x][y].ltop.second - matchLoc.y) < 3) {
              board[x][y].state = Arr[i].state;
              board[x][y].piece = Arr[i].piece;
            }
          }
        }
      }
    }
  }

  for (x = 0; x < 8; x++) {
    for (y = 0; y < 8; y++) {

      cout << "ome" << x << " ," << y << endl;
      cout << board[x][y].piece << "," << board[x][y].state << endl;
    }
  }

  // rectangle( result, matchLoc, Point( matchLoc.x + temp.cols , matchLoc.y +
  // temp.rows ), Scalar::all(0), 2, 8, 0 );
  imshow("Cropped portion1", img);
  waitKey(0);
}