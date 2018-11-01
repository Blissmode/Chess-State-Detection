#include <bits/stdc++.h>
using namespace std;

struct square
{
	pair <int,int> ltop;
	pair <int,int> rbottom;
	char piece;
    int state;
};

square board[8][8];
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
bool attackOnQueen();
void printAns(int,int);


void enlistMoves();
/*/////////////////////////////////////////////////////////////////////////
                xS RULE:
    +100 for each possible exchange position
    -50 for each value on attack matrix
/////////////////////////////////////////////////////////////////////////*/
void enlistRook(int,int);
void enlistKnight(int,int);
void enlistBishop(int,int);
void enlistPawn(int,int);


struct pieceMovex
{
 int weight;
 char piece;
 pair<int,int> start,end;
};

struct Comp
{
   bool operator()(const pieceMovex& s1, const pieceMovex& s2)
   {
       return s1.x < s2.x;
   }
};

vector <pieceMovex> MiniMax;

vector < pair<int,int> > kingMoves,queenMoves,bishopMoves,rookMoves,knightMoves;

int main()
{

    initialize();

    char s[10];
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            attackMatrix[i][j]=0;
            cin>>board[i][j].state;
            if(board[i][j].state!=-1)
            cin>>board[i][j].piece;
            else
            board[i][j].piece='-';
        }
    }

    cout<<"The stored chess board "<<endl;
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
        cout<<"No Checks yeah!!!!"<<endl;
        bestMove();
    }
}

bool isvalid(int i,int j)
{
    if(i<8 && j<8 && i>=0 && j>=0)
    return true;
    else
    return false;
}


void initialize()
{

    // inititalize king moves;
    kingMoves.push_back(make_pair(1,0));
    kingMoves.push_back(make_pair(1,1));
    kingMoves.push_back(make_pair(0,1));
    kingMoves.push_back(make_pair(1,0));
    kingMoves.push_back(make_pair(-1,1));
    kingMoves.push_back(make_pair(-1,0));
    kingMoves.push_back(make_pair(-1,-1));
    kingMoves.push_back(make_pair(0,-1));
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
}

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
                    for(int k=0;knightMoves.size();k++)
                    {
                        if(isvalid(i+kingMoves[k].first,j+kingMoves[k].second))
                        {
                            if(board[i+kingMoves[k].first][j+kingMoves[k].second].state==-1 && attackMatrix[i+kingMoves[k].first][j+kingMoves[k].second]==0)
                            {
                                printAns(i+kingMoves[k].first,j+kingMoves[k].second);
                            }
                        }
                    }
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

}

void moveQueen()
{

}


void enlistMoves()
{
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            if(board[i][j].state==(!moveOf))
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

    make_heap(vec.begin(), vec.end(), Comp());
}








void enlistRook(int x,int y)
{
    int i,j;

    pieceMovex temp;

    temp.piece='r';
    temp.start=make_pair(i,j);
    //up
    i=x-1;
    j=y;
    while(i>=0)
    {
        if(board[i][j].state==(!moveOf))
        {
            attackMatrix[i][j]++;
            break;
        }
        else if(board[i][j].state==(moveOf))
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
        else if(board[i][j].state==(moveOf))
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
        else if(board[i][j].state==(moveOf))
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
        else if(board[i][j].state==(moveOf))
        break;setRook
        else
        attackMatrix[i][j]++;
        j++;
    }   
}

void printAns(int x, int y)
{
    cout<<"The best move is "<<x<<" "<<y<<endl;
}