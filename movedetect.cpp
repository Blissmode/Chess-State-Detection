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

int main()
{
    string s;
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            cin>>board[i][j].state;
            if(board[i])>>board[i][j].piece;
        }
    }

    cout<<"The stored chess board "
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            if(board[i][j].state==0)
            cout<<"w";
            else if(board[i][j]==1)
            cout<<"b";
            cout<<board[i][j].piece<<" ";
        }
        cout<<endl;
    }
}