#include <bits/stdc++.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    Mat img=imread("images/startpos.png"),img2;
    imshow("Chess board",img);
    waitKey(0);
    img2=img;
    int x=1,prevx=0;

    
    while(true)
    {
        createTrackbar("Kernal size","Chess state",&x,50);
        imshow("Chess state",img2);
    if(x%2 && prevx!=x)
      {
          prevx=x;
        GaussianBlur(img,img2,Size(x,x),0,0);  // Have the index as 0,1,2 for blue, greem and red respectively 
      }
      int press=waitKey(50);
      if(press==27)
      break;
    }
    destroyAllWindows();
}
