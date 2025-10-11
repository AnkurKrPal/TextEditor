#include <bits/stdc++.h>
#include "PieceTable.h"
using namespace std;
int main(){
    PieceTable P;
    int cursor=0;
    int queries;
    cin>>queries;
    while(queries--){
        char c;
        cin >> c;
        if(c>='a'&&c<='z'){
            P.insert(c,cursor);
            cursor++;
            
        }else if(c=='5'){       // backspace
            //P.deletion(cursor);
            if(P.state!=2)P.delCount=0;
            P.delCount++;
            P.deletion(cursor);
            cursor--;
        }else if(c=='4'){       // left
            cursor--;
            P.GlobalIndex--;
            if(P.delCount>0){P.weightUpdator2(P.head,P.currIndex);P.delCount=0;}
            if(P.state==1){
                P.weightUpdator(P.head , P.currIndex);
            }
            P.state=0;
        }else if(c=='6'){       //right
            cursor++;
            P.GlobalIndex++;
            if(P.delCount>0){P.weightUpdator2(P.head,P.currIndex);P.delCount=0;}
            if(P.state==1){
                P.weightUpdator(P.head , P.currIndex);
            }
            P.state=0;
        }
        P.view(P.head);
        cout<<" | state : "<<P.state<<" | currIndex : "<<P.currIndex<<endl;
        cout<<endl;
    }
}