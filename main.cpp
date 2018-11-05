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
void MatchingMethod( int, void*,int,string,int,char);








////////////////////////////// MOVE DETECTION PART DECLARATION////////////////////////////

struct pieceMovex
{
 int weight;
 char piece;
 pair<int,int> start,end;
};


int attackMatrix[8][8];
int moveOf;


void initialize();
void findAttack();
void setKing(int,int);
void setQueen(int,int);
void setRook(int,int);
void setKnight(int,int);
void setBishop(int,int);
void setPawn(int,int);
bool checkDetect();
void checkResponse();
void bestMove();
void moveQueen();
void enlistMoves();
bool attackOnQueen();


void enlistMoves();
/*/////////////////////////////////////////////////////////////////////////
                WEIGHTS RULE:
    +100 for each possible exchange position
    -50 for each value on attack matrix
/////////////////////////////////////////////////////////////////////////*/
void enlistRook(int,int);
void enlistKnight(int,int);
void enlistBishop(int,int);
void enlistPawn(int,int);
unordered_map <char,int> piecePoints;


bool Comp(pieceMovex s1,pieceMovex s2)
{
    return s1.weight > s2.weight;
};

vector <pieceMovex> MiniMax;

vector < pair<int,int> > kingMoves;







int main()
{
	initialize();
    img = imread("images/startpos1.png", 1);
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
	for(int i=0;i<8;i++)
	{
		for(int j=0;j<8;j++)
		{
			// circle( src, Point( board[i][j].ltop.first,board[i][j].ltop.second ), 32.0, Scalar( 0, 0, 255 ), 1, 8 );
			// circle( src, Point( board[i][j].rbottom.first,board[i][j].rbottom.second ), 32.0, Scalar( 0, 0, 255 ), 1, 8 );
			printf("(%d,%d),(%d,%d)  ",board[i][j].ltop.first,board[i][j].ltop.second,board[i][j].rbottom.first,board[i][j].rbottom.second);
		}
		cout<<endl;
	}

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

    MatchingMethod( 0, 0 ,3,"images/bpawn.png",0,'p');
	MatchingMethod( 0, 0 ,3,"images/wpawn.png",1,'p');
	MatchingMethod( 0, 0 ,3,"images/bknight.png",0,'n');
	MatchingMethod( 0, 0 ,1,"images/wknight.png",1,'n');
	MatchingMethod( 0, 0 ,3,"images/bqueen.png",0,'q');
	MatchingMethod( 0, 0 ,1,"images/wqueen.png",1,'q');
	MatchingMethod( 0, 0 ,3,"images/bking.png",0,'k');
	MatchingMethod( 0, 0 ,1,"images/wking.png",1,'k');
	MatchingMethod( 0, 0 ,3,"images/bbishop.png",0,'b');
	MatchingMethod( 0, 0 ,1,"images/wbishop.png",1,'b');
	MatchingMethod( 0, 0 ,3,"images/brook.png",0,'r');
	MatchingMethod( 0, 0 ,1,"images/wrook.png",1,'r');


	cout<<"\n\nThe stored chess board\n"<<endl;
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            if(board[i][j].state==1)
            cout<<"w";
            else if(board[i][j].state==0)
            cout<<"b";
            cout<<board[i][j].piece<<" ";
        }
        cout<<endl;
    }
	char s[10];
	cout<<endl<<"This turn is the move of (white/black):";
	cin>>s;
    moveOf=strcmp(s,"white")?0:1;
    if(moveOf)
    cout<<"White to play"<<endl;
    else
    cout<<"Black to play"<<endl;

    findAttack();
    if(checkDetect())
    {
        cout<<"Check detected"<<endl;
        checkResponse();
    }
    else
    {
        cout<<"No Checks detected proceeding with the best move"<<endl;
        bestMove();
    }

    waitKey(0);
    return 0;
}














void initialize()
{

	for(int i=0;i<8;i++)
	{
		for(int j=0;j<8;j++)
		{
			board[i][j].state=-1;
			board[i][j].piece='-';
		}
	}

    // inititalize king moves;
    kingMoves.push_back(make_pair(1,0));
    kingMoves.push_back(make_pair(1,1));
    kingMoves.push_back(make_pair(0,1));
    kingMoves.push_back(make_pair(1,0));
    kingMoves.push_back(make_pair(-1,1));
    kingMoves.push_back(make_pair(-1,0));
    kingMoves.push_back(make_pair(-1,-1));
    kingMoves.push_back(make_pair(0,-1));

    sort(kingMoves.begin(),kingMoves.end());

    piecePoints['k']=-INT_MAX/2;
    piecePoints['q']=9;
    piecePoints['r']=5;
    piecePoints['b']=3;
    piecePoints['n']=3;
    piecePoints['p']=1;
}




void MatchingMethod( int, void* ,int match_method,string name,int state,char piece)
{
      Mat img_display;
      imgz.copyTo( img_display );

	  templ=imread(name,1);
      int result_cols =  imgz.cols - templ.cols + 1;
      int result_rows = imgz.rows - templ.rows + 1;

      result.create( result_cols, result_rows, CV_32FC1 );
      matchTemplate( imgz, templ, result, match_method );
      normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

      Point minLoc; Point maxLoc;
      Point matchLoc;
      double minVal; double maxVal;

      for(int k=1;k<=8;k++)
      {
		  minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
          // cout<<"Match #"<<k<<" occurs at:"<<result.at<float>(minLoc.x,minLoc.y)<<endl;
    	  cout<<"Loacation:"<<matchLoc.x<<","<<matchLoc.y<<endl;
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

          if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED )
          matchLoc = minLoc;
          else
          matchLoc = maxLoc;

      	  rectangle( img_display, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar(0,0,255), 2, 8, 0 );
      	  rectangle( result, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar(0,0,255), 2, 8, 0 );
	  
	  	  for(int i=0;i<8;i++)
	  	  {
	  	  	for(int j=0;j<8;j++)
	  	  	{
	  	  		if(board[i][j].ltop.first<=matchLoc.x && board[i][j].ltop.second<=matchLoc.y && board[i][j].rbottom.first>=matchLoc.x && board[i][j].rbottom.second>=matchLoc.y)
		  		{
		  			board[i][j].state=state;
		  			board[i][j].piece=piece;
		  		}
	  	  	}
	       }
      }
      imshow( name, img_display );

      return;
    }


bool isvalid(int i,int j)
{
    if(i<8 && j<8 && i>=0 && j>=0)
    return true;
    else
    return false;
}


void findAttack()
{
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            if(board[i][j].state==(!moveOf))
            {
                switch(board[i][j].piece)
                {
                    case 'k':
                    setKing(i,j);
                    break;
                    case 'q':
                    setQueen(i,j);
                    break;
                    case 'r':
                    setRook(i,j);
                    break;
                    case 'n':
                    setKnight(i,j);
                    break;
                    case 'b':
                    setBishop(i,j);
                    break;
                    case 'p':
                    setPawn(i,j);
                }
            }
        }
    }

    //Display the attack array

    cout<<endl<<"Attack Matrix"<<endl;

    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            cout<<attackMatrix[i][j]<<" ";
        }
        cout<<endl;
    }
}




void setKing(int x,int y)
{
    int i,j;
    i=x-1;
    j=y;
    //north
    if(i>=0)
    {
        if(board[i][j].state==(moveOf) || board[i][j].state==-1)
        attackMatrix[i][j]++;
    }

    i=x-1;
    j=y-1;
    //north west
    if(i>=0 && j>=0)
    {
        if(board[i][j].state==(moveOf) || board[i][j].state==-1)
        attackMatrix[i][j]++;
    }

    i=x;
    j=y-1;
    //west
    if(j>=0)
    {
        if(board[i][j].state==(moveOf) || board[i][j].state==-1)
        attackMatrix[i][j]++;
    }

    i=x+1;
    j=y-1;
    //south west
    if(i<8 && j>=0)
    {
        if(board[i][j].state==(moveOf) || board[i][j].state==-1)
        attackMatrix[i][j]++;
    }

    i=x+1;
    j=y;
    //south
    if(i<8)
    {
        if(board[i][j].state==(moveOf) || board[i][j].state==-1)
        attackMatrix[i][j]++;
    }

    i=x+1;
    j=y+1;
    //south east
    if(i<8 && j<8)
    {
        if(board[i][j].state==(moveOf) || board[i][j].state==-1)
        attackMatrix[i][j]++;
    }

    i=x;
    j=y+1;
    //east
    if(j<8)
    {
        if(board[i][j].state==(moveOf) || board[i][j].state==-1)
        attackMatrix[i][j]++;
    }

    i=x-1;
    j=y+1;
    //north east
    if(i>=0 && j<8)
    {
        if(board[i][j].state==(moveOf) || board[i][j].state==-1)
        attackMatrix[i][j]++;
    }
    
}



void setQueen(int x,int y)
{
    setRook(x,y);
    setBishop(x,y);
}



void setRook(int x,int y)
{
     int i,j;

    //up
    i=x-1;
    j=y;
    while(i>=0)
    {
        if(board[i][j].state==(moveOf))
        {
            attackMatrix[i][j]++;
            break;
        }
        else if(board[i][j].state==(!moveOf))
        break;
        else
        attackMatrix[i][j]++;
        i--;
    }

    //left
    i=x;
    j=y-1;
    while(j>=0)
    {
        if(board[i][j].state==(moveOf))
        {
            attackMatrix[i][j]++;
            break;
        }
        else if(board[i][j].state==(!moveOf))
        break;
        else
        attackMatrix[i][j]++;
        j--;
    }

    //bottom
    i=x+1;
    j=y;
    while(i<8)
    {
        if(board[i][j].state==(moveOf))
        {
            attackMatrix[i][j]++;
            break;
        }
        else if(board[i][j].state==(!moveOf))
        break;
        else
        attackMatrix[i][j]++;
        i++;
    }

    //right
    i=x;
    j=y+1;
    while(j<8)
    {
        if(board[i][j].state==(moveOf))
        {
            attackMatrix[i][j]++;
            break;
        }
        else if(board[i][j].state==(!moveOf))
        break;
        else
        attackMatrix[i][j]++;
        j++;
    }   
}



void setKnight(int x,int y)
{
    int i,j;
    i=x-2;
    j=y+1;
    //minus plus
    if(i>=0 && j<8)
    {
        if(board[i][j].state==(moveOf) || board[i][j].state==-1)
        attackMatrix[i][j]++;
    }

    i=x-1;
    j=y+2;
    if(i>=0 && j<8)
    {
        if(board[i][j].state==(moveOf) || board[i][j].state==-1)
        attackMatrix[i][j]++;
    }

    i=x+1;
    j=y+2;
    //plus plus
    if(i<8 && j<8)
    {
        if(board[i][j].state==(moveOf) || board[i][j].state==-1)
        attackMatrix[i][j]++;
    }

    i=x+2;
    j=y+1;
    if(i<8 && j<8)
    {
        if(board[i][j].state==(moveOf) || board[i][j].state==-1)
        attackMatrix[i][j]++;
    }

    i=x+1;
    j=y-2;
    //plus minus
    if(i<8 && j>=0)
    {
        if(board[i][j].state==(moveOf) || board[i][j].state==-1)
        attackMatrix[i][j]++;
    }

    i=x+2;
    j=y-1;
    if(i<8 && j>=0)
    {
        if(board[i][j].state==(moveOf) || board[i][j].state==-1)
        attackMatrix[i][j]++;
    }

    i=x-1;
    j=y-2;
    //minus minus
    if(i>=0 && j>=0)
    {
        if(board[i][j].state==(moveOf) || board[i][j].state==-1)
        attackMatrix[i][j]++;
    }

    i=x-2;
    j=y-1;
    if(i>=0 && j>=0)
    {
        if(board[i][j].state==(moveOf) || board[i][j].state==-1)
        attackMatrix[i][j]++;
    }
}



void setBishop(int x,int y)
{
    int i,j;

    //upper right
    i=x-1;
    j=y+1;
    while(j<8 && i>=0)
    {
        if(board[i][j].state==(moveOf))
        {
            attackMatrix[i][j]++;
            break;
        }
        else if(board[i][j].state==(!moveOf))
        break;
        else
        attackMatrix[i][j]++;
        i--;
        j++;
    }

    //upper left
    i=x-1;
    j=y-1;
    while(i>=0 && j>=0)
    {
        if(board[i][j].state==(moveOf))
        {
            attackMatrix[i][j]++;
            break;
        }
        else if(board[i][j].state==(!moveOf))
        break;
        else
        attackMatrix[i][j]++;
        i--;
        j--;
    }

    //bottom right
    i=x+1;
    j=y+1;
    while(i<8 && j<8)
    {
        if(board[i][j].state==(moveOf))
        {
            attackMatrix[i][j]++;
            break;
        }
        else if(board[i][j].state==(!moveOf))
        break;
        else
        attackMatrix[i][j]++;
        i++;
        j++;
    }

    //bottom left
    i=x+1;
    j=y-1;
    while(i<8 && j>=0)
    {
        if(board[i][j].state==(moveOf))
        {
            attackMatrix[i][j]++;
            break;
        }
        else if(board[i][j].state==(!moveOf))
        break;
        else
        attackMatrix[i][j]++;
        i++;
        j--;
    }
    
}

void setPawn(int x,int y)
{
    int i,j;

    if(moveOf)
    {   
        i=x+1;
        j=y-1;
        if(i<8 && j>=0)
        {
            if(board[i][j].state==(moveOf) || board[i][j].state==-1)
            attackMatrix[i][j]++;
        }

        i=x+1;
        j=y+1;
        if(i<8 && j<8)
        {
            if(board[i][j].state==(moveOf) || board[i][j].state==-1)
            attackMatrix[i][j]++;
        }
    }
    else
    {
        i=x-1;
        j=y-1;
        if(i>=0 && j>=0)
        {
            if(board[i][j].state==(moveOf) || board[i][j].state==-1)
            attackMatrix[i][j]++;
        }

        i=x-1;
        j=y+1;
        if(i>=0 && j<8)
        {
            if(board[i][j].state==(moveOf) || board[i][j].state==-1)
            attackMatrix[i][j]++;
        }
    }
}

bool checkDetect()
{
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            if(board[i][j].state==moveOf)
            {
                if(board[i][j].piece=='k')
                {
                    if(attackMatrix[i][j])
                    return true;
                    else
                    return false;
                }
            }
        }
    }

    return false;
}


//!!!!Needs fixing!!!!!
void checkResponse()
{
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            if(board[i][j].state==moveOf)
            {
                if(board[i][j].piece=='k')
                {
                    for(int k=0;k<kingMoves.size();k++)
                    {
                        if(isvalid(i+kingMoves[k].first,j+kingMoves[k].second))
                        {
                            if(board[i+kingMoves[k].first][j+kingMoves[k].second].state==-1 && attackMatrix[i+kingMoves[k].first][j+kingMoves[k].second]==0)
                            {
                                cout<<"King to "<<i+kingMoves[k].first<<","<<MiniMax[i].end.second<<": "<<j+kingMoves[k].second<<" "<<endl;
                                return;
                            }
                        }
                    }
                    return;
                }
            }
        }
    }
}


void bestMove()
{
    if(attackOnQueen())
    moveQueen();
    else
    enlistMoves();
}

bool attackOnQueen()
{
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            if(board[i][j].state==moveOf)
            if(board[i][j].piece=='q')
            {
                if(attackMatrix[i][j])
                return true;
                else
                return false;
            }
        }
    }
    return false;
}

void moveQueen()
{
    cout<<"This is a beta version and attack on the queen is detected. Generally there is a simpler way to escape than this algorithm can predict."<<endl;
}


void enlistMoves()
{
    cout<<"Proceeding to enlist all possible moves available"<<endl;
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            if(board[i][j].state==(moveOf))
            {
                switch(board[i][j].piece)
                {
                    case 'r':
                    enlistRook(i,j);
                    break;
                    case 'n':
                    enlistKnight(i,j);
                    break;
                    case 'b':
                    enlistBishop(i,j);
                    break;
                    case 'p':
                    enlistPawn(i,j);
                }
            }
        }
    }

    cout<<endl<<"Lisitng out all the weights found"<<endl;
	sort(MiniMax.begin(),MiniMax.end(),Comp);
	cout<<"List of proper offensive moves available sorted by effectiveness:"<<endl;
    for(int i=0;i<MiniMax.size();i++)
    {
		// printf("#%d: %c from (%d,%d) to (%d,%d)\n",i+1,MiniMax[i].piece,MiniMax[i].start.first,MiniMax[i].start.second,MiniMax[i].end.first,MiniMax[i].end.second);
        cout<<MiniMax[i].piece<<" at "<<MiniMax[i].end.first<<","<<MiniMax[i].end.second<<": "<<MiniMax[i].weight<<" "<<endl;
    }
}








void enlistRook(int x,int y)
{
    int i,j;

    pieceMovex temp;
    temp.piece='r';
    temp.start=make_pair(x,y);
    //up
    i=x-1;
    j=y;
    while(i>=0)
    {
        if(board[i][j].state==(!moveOf))
        {
            temp.end=make_pair(i,j);
            temp.weight=100*(piecePoints[board[i][j].piece]-piecePoints[board[x][y].piece]);
            MiniMax.push_back(temp);
            break;
        }
        else if(board[i][j].state==(moveOf))
        break;
        else
        {
            temp.end=make_pair(i,j);
            temp.weight=attackMatrix[i][j]*(-50);
            MiniMax.push_back(temp);
        }
        i--;
    }

    //left
    i=x;
    j=y-1;
    while(j>=0)
    {
        if(board[i][j].state==(!moveOf))
        {
            temp.end=make_pair(i,j);
            temp.weight=100*(piecePoints[board[i][j].piece]-piecePoints[board[x][y].piece]);
            MiniMax.push_back(temp);
            break;
        }
        else if(board[i][j].state==(moveOf))
        break;
        else
        {
            temp.end=make_pair(i,j);
            temp.weight=attackMatrix[i][j]*(-50);
            MiniMax.push_back(temp);
        }
        j--;
    }

    //bottom
    i=x+1;
    j=y;
    while(i<8)
    {
        if(board[i][j].state==(!moveOf))
        {
            temp.end=make_pair(i,j);
            temp.weight=100*(piecePoints[board[i][j].piece]-piecePoints[board[x][y].piece]);
            MiniMax.push_back(temp);
            break;
        }
        else if(board[i][j].state==(moveOf))
        break;
        else
        {
            temp.end=make_pair(i,j);
            temp.weight=attackMatrix[i][j]*(-50);
            MiniMax.push_back(temp);
        }
        i++;
    }

    //right
    i=x;
    j=y+1;
    while(j<8)
    {
        if(board[i][j].state==(!moveOf))
        {
            temp.end=make_pair(i,j);
            temp.weight=100*(piecePoints[board[i][j].piece]-piecePoints[board[x][y].piece]);
            MiniMax.push_back(temp);
            break;
        }
        else if(board[i][j].state==(moveOf))
        break;
        else
        {
            temp.end=make_pair(i,j);
            temp.weight=attackMatrix[i][j]*(-50);
            MiniMax.push_back(temp);
        }
        j++;
    }   
}

void enlistKnight(int x,int y)
{
    int i,j;

    pieceMovex temp;
    temp.piece='n';
    temp.start=make_pair(x,y);


    i=x-2;
    j=y+1;
    //minus plus
    if(i>=0 && j<8)
    {
        if(board[i][j].state==(!moveOf))
        {
            temp.end=make_pair(i,j);
            temp.weight=100*(piecePoints[board[i][j].piece]-piecePoints[board[x][y].piece]);
            MiniMax.push_back(temp);
        }
        else if(board[i][j].state==-1)
        {
            temp.end=make_pair(i,j);
            temp.weight=attackMatrix[i][j]*(-50);
            MiniMax.push_back(temp);
        }
    }

    i=x-1;
    j=y+2;
    if(i>=0 && j<8)
    {
        if(board[i][j].state==(!moveOf))
        {
            temp.end=make_pair(i,j);
            temp.weight=100*(piecePoints[board[i][j].piece]-piecePoints[board[x][y].piece]);
            MiniMax.push_back(temp);
        }
        else if(board[i][j].state==-1)
        {
            temp.end=make_pair(i,j);
            temp.weight=attackMatrix[i][j]*(-50);
            MiniMax.push_back(temp);
        }
    }

    i=x+1;
    j=y+2;
    //plus plus
    if(i<8 && j<8)
    {
        if(board[i][j].state==(!moveOf))
        {
            temp.end=make_pair(i,j);
            temp.weight=100*(piecePoints[board[i][j].piece]-piecePoints[board[x][y].piece]);
            MiniMax.push_back(temp);
        }
        else if(board[i][j].state==-1)
        {
            temp.end=make_pair(i,j);
            temp.weight=attackMatrix[i][j]*(-50);
            MiniMax.push_back(temp);
        }
    }

    i=x+2;
    j=y+1;
    if(i<8 && j<8)
    {
        if(board[i][j].state==(!moveOf))
        {
            temp.end=make_pair(i,j);
            temp.weight=100*(piecePoints[board[i][j].piece]-piecePoints[board[x][y].piece]);
            MiniMax.push_back(temp);
        }
        else if(board[i][j].state==-1)
        {
            temp.end=make_pair(i,j);
            temp.weight=attackMatrix[i][j]*(-50);
            MiniMax.push_back(temp);
        }
    }

    i=x+1;
    j=y-2;
    //plus minus
    if(i<8 && j>=0)
    {
        if(board[i][j].state==(!moveOf))
        {
            temp.end=make_pair(i,j);
            temp.weight=100*(piecePoints[board[i][j].piece]-piecePoints[board[x][y].piece]);
            MiniMax.push_back(temp);
        }
        else if(board[i][j].state==-1)
        {
            temp.end=make_pair(i,j);
            temp.weight=attackMatrix[i][j]*(-50);
            MiniMax.push_back(temp);
        }
    }

    i=x+2;
    j=y-1;
    if(i<8 && j>=0)
    {
        if(board[i][j].state==(!moveOf))
        {
            temp.end=make_pair(i,j);
            temp.weight=100*(piecePoints[board[i][j].piece]-piecePoints[board[x][y].piece]);
            MiniMax.push_back(temp);
        }
        else if(board[i][j].state==-1)
        {
            temp.end=make_pair(i,j);
            temp.weight=attackMatrix[i][j]*(-50);
            MiniMax.push_back(temp);
        }
    }

    i=x-1;
    j=y-2;
    //minus minus
    if(i>=0 && j>=0)
    {
        if(board[i][j].state==(!moveOf))
        {
            temp.end=make_pair(i,j);
            temp.weight=100*(piecePoints[board[i][j].piece]-piecePoints[board[x][y].piece]);
            MiniMax.push_back(temp);
        }
        else if(board[i][j].state==-1)
        {
            temp.end=make_pair(i,j);
            temp.weight=attackMatrix[i][j]*(-50);
            MiniMax.push_back(temp);
        }
    }

    i=x-2;
    j=y-1;
    if(i>=0 && j>=0)
    {
        if(board[i][j].state==(!moveOf))
        {
            temp.end=make_pair(i,j);
            temp.weight=100*(piecePoints[board[i][j].piece]-piecePoints[board[x][y].piece]);
            MiniMax.push_back(temp);
        }
        else if(board[i][j].state==-1)
        {
            temp.end=make_pair(i,j);
            temp.weight=attackMatrix[i][j]*(-50);
            MiniMax.push_back(temp);
        }
    }
}

void enlistBishop(int x,int y)
{
    int i,j;

    pieceMovex temp;
    temp.piece='b';
    temp.start=make_pair(x,y);


    //upper right
    i=x-1;
    j=y+1;
    while(j<8 && i>=0)
    {
        if(board[i][j].state==(!moveOf))
        {
            temp.end=make_pair(i,j);
            temp.weight=100*(piecePoints[board[i][j].piece]-piecePoints[board[x][y].piece]);
            MiniMax.push_back(temp);
            break;
        }
        else if(board[i][j].state==(moveOf))
        break;
        else
        {
            temp.end=make_pair(i,j);
            temp.weight=attackMatrix[i][j]*(-50);
            MiniMax.push_back(temp);
        }
        i--;
        j++;
    }

    //upper left
    i=x-1;
    j=y-1;
    while(i>=0 && j>=0)
    {
        if(board[i][j].state==(!moveOf))
        {
            temp.end=make_pair(i,j);
            temp.weight=100*(piecePoints[board[i][j].piece]-piecePoints[board[x][y].piece]);
            MiniMax.push_back(temp);
            break;
        }
        else if(board[i][j].state==(moveOf))
        break;
        else
        {
            temp.end=make_pair(i,j);
            temp.weight=attackMatrix[i][j]*(-50);
            MiniMax.push_back(temp);
        }
        i--;
        j--;
    }

    //bottom right
    i=x+1;
    j=y+1;
    while(i<8 && j<8)
    {
        if(board[i][j].state==(!moveOf))
        {
            temp.end=make_pair(i,j);
            temp.weight=100*(piecePoints[board[i][j].piece]-piecePoints[board[x][y].piece]);
            MiniMax.push_back(temp);
            break;
        }
        else if(board[i][j].state==(moveOf))
        break;
        else
        {
            temp.end=make_pair(i,j);
            temp.weight=attackMatrix[i][j]*(-50);
            MiniMax.push_back(temp);
        }
        i++;
        j++;
    }

    //bottom left
    i=x+1;
    j=y-1;
    while(i<8 && j>=0)
    {
        if(board[i][j].state==(!moveOf))
        {
            temp.end=make_pair(i,j);
            temp.weight=100*(piecePoints[board[i][j].piece]-piecePoints[board[x][y].piece]);
            MiniMax.push_back(temp);
            break;
        }
        else if(board[i][j].state==(moveOf))
        break;
        else
        {
            temp.end=make_pair(i,j);
            temp.weight=attackMatrix[i][j]*(-50);
            MiniMax.push_back(temp);
        }
        i++;
        j--;
    }
}

void enlistPawn(int x,int y)
{
    int i,j;
    pieceMovex temp;
    temp.piece='p';
    temp.start=make_pair(x,y);

    if(!moveOf)
    {   
        i=x+1;
        j=y-1;
        if(i<8 && j>=0)
        {
            if(board[i][j].state==(moveOf))
            {
                temp.end=make_pair(i,j);
                temp.weight=100*(piecePoints[board[i][j].piece]-piecePoints[board[x][y].piece]);
                MiniMax.push_back(temp);
            }
        }
            
        i=x+1;
        j=y+1;
        if(i<8 && j<8)
        {
            if(board[i][j].state==(moveOf))
            {
                temp.end=make_pair(i,j);
                temp.weight=100*(piecePoints[board[i][j].piece]-piecePoints[board[x][y].piece]);
                MiniMax.push_back(temp);
            }
        }
    }
    else
    {
        i=x-1;
        j=y-1;
        if(i>=0 && j>=0)
        {
            if(board[i][j].state==(!moveOf))
            {
                temp.end=make_pair(i,j);
                temp.weight=100*(piecePoints[board[i][j].piece]-piecePoints[board[x][y].piece]);
                MiniMax.push_back(temp);
            }
        }

        i=x-1;
        j=y+1;
        if(i>=0 && j<8)
        {
            if(board[i][j].state==(!moveOf))
            {
                temp.end=make_pair(i,j);
                temp.weight=100*(piecePoints[board[i][j].piece]-piecePoints[board[x][y].piece]);
                MiniMax.push_back(temp);
            }
        }
    }
}