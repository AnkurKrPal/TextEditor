#include <bits/stdc++.h>
using namespace std ;
typedef struct piece{
    string source;
    int start;
    int length;
}piece;
class pieceTable{

    private:
        vector<piece> Pieces;
        int state=0;
        piece current_piece=NULL;
        string add="";
        string original="";

    public:
        int nextIndex(int index){
            int originalIndex=index;
            int i=0;
            if(index==0)return 0;
            for(auto &it:Pieces){
                i++;
                index-=it.length;
                if(index==0){
                    return i;
                }
                if(index<0){
                    int n=it.start;
                    int len=it.length;
                    it.length=originalIndex-n;
                    piece newPiece=new piece;
                    newPiece.source=it.source;
                    newPiece.start=originalIndex;
                    newPiece.length=len+n-originalIndex;
                    Pieces.insert(i,newPiece);
                    return i;
                }
            }
            return Pieces.length();
        }
        void insert(char c, int index){
            if(state==0){
                current_piece.source = "add";
                current_piece.start = add.length();
                current_piece.length = 1;

                add.push_back(c);
                Pieces.insert(Pieces.begin()+k , current_piece);
                state = 1;
            }

            else if(state == 1){
                current_piece.length++;
                add.push_back(c);
            }
        }
};
int main(){
    
    char c; //decided from keyboard
    int k; //decided from cursor

    //if (cursor_change || arrow_key_press)state=0;


    //insertion

    
    

    //deletion

    // while(true){
    //     if(current_piece.length==0){
    //         for(int i=0;i<Pieces.size();i++){
    //             if(Pieces[i].start == current_piece.start && Pieces[i].source == current_piece.source){
    //                 Pieces.erase(Pieces.begin()+i);
    //                 current_piece = Pieces[i-1];
    //             }
    //         }
    //     }
    //     current_piece.length--;
    // }
}