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
            cursor--;
        }else if(c=='4'){       // left
            cursor--;
            P.state=0;
        }else if(c=='6'){       //right
            cursor++;
            P.state=0;
        }
        P.view(P.head);
        cout<<endl;
    }
}