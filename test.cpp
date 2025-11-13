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
            P.insert(c,cursor,0);
            cursor++;
            
        }else if(c=='5'){       // backspace
            //P.deletion(cursor);
            if( P.GlobalIndex > 0){
                if(P.state!=2)P.delCount=0;
                P.delCount++;
                P.deletion(cursor,0);
                cursor = P.GlobalIndex;  // Sync with internal index instead of just decrementing
            }

        }else if(c=='4'){       // left
            cursor--;
            P.GlobalIndex--;
            if(P.state==2){P.weightUpdator2(P.head,P.currIndex);P.delCount=0;P.current_piece=NULL;}
            if(P.state==1){
                P.weightUpdator(P.head , P.currIndex);
                P.current_piece=NULL;
            }
            if(P.lastStepLength>0) P.undo.push(new laststep(P.undoType  , P.lastStepLength , P.cursorStart , P.charStack));
            P.lastStepLength=0;
            
            P.state=0;
        }else if(c=='6'){       //right
            cursor++;
            P.GlobalIndex++;
            if(P.state==2){P.weightUpdator2(P.head,P.currIndex);P.delCount=0;P.current_piece=NULL;}
            if(P.state==1){
                P.weightUpdator(P.head , P.currIndex);
                P.current_piece=NULL;
            }
            if(P.lastStepLength>0) P.undo.push(new laststep(P.undoType , P.lastStepLength , P.cursorStart , P.charStack));
            P.lastStepLength=0;

            P.state=0;
        }else if(c=='8'){        //undo
            if(P.lastStepLength>0) P.undo.push(new laststep(P.undoType  , P.lastStepLength , P.cursorStart , P.charStack));
            P.lastStepLength=0;
            if(P.state==2){P.weightUpdator2(P.head,P.currIndex);P.delCount=0;P.current_piece=NULL;}
            if(P.state==1){
                P.weightUpdator(P.head , P.currIndex);
                P.current_piece=NULL;
            }
            P.state=0;
            
            P.undofn();
            if(P.state==2){
                P.weightUpdator2(P.head,P.currIndex);P.delCount=0;P.current_piece=NULL;
            }
            if(P.state==1){
                P.weightUpdator(P.head , P.currIndex);
                P.current_piece=NULL;
            }
            P.state=0;
            cursor=P.GlobalIndex;
        }else if(c=='9'){       //redo
            if(P.lastStepLength>0) P.undo.push(new laststep(P.undoType  , P.lastStepLength , P.cursorStart , P.charStack));
            P.lastStepLength=0;
            if(P.state==2){P.weightUpdator2(P.head,P.currIndex);P.delCount=0;P.current_piece=NULL;}
            if(P.state==1){
                P.weightUpdator(P.head , P.currIndex);
                P.current_piece=NULL;
            }
            P.state=0;
            
            P.redofn();
            if(P.state==2){
                P.weightUpdator2(P.head,P.currIndex);P.delCount=0;P.current_piece=NULL;
            }
            if(P.state==1){
                P.weightUpdator(P.head , P.currIndex);
                P.current_piece=NULL;
            }
            P.state=0;
            cursor=P.GlobalIndex;
        }
    }
}