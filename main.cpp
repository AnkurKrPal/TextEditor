#include <bits/stdc++.h>
using namespace std ;
typedef struct piece{
    string source;
    int start;
    int length;

    piece(){
        source = "add";
        start =0;
        length=0;
    }
}piece;
class pieceTable{

    private:
        vector<piece> Pieces;
        int state=0;
        piece current_piece ;
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
                    piece* newPiece=new piece;
                    newPiece->source=it.source;
                    newPiece->start=originalIndex;
                    newPiece->length=len+n-originalIndex;
                    Pieces.insert(Pieces.begin() + i, *newPiece);
                    return i;
                }
            }
            return Pieces.size();
        }
        void insert(char c, int index){
            if(state==0){
                current_piece.source = "add";
                current_piece.start = add.length();
                current_piece.length = 1;
                int i= nextIndex(index);
                add.push_back(c);
                Pieces.insert(Pieces.begin()+i , current_piece);
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
    bool backspace; //decided from keyboard

    //if (cursor_change || arrow_key_press)state=0;


    //insertion

    
    

    //deletion

    // while(backspace){
    //     if(current_piece.length==0){
    //         for(int i=1;i<Pieces.size();i++){
    //             if(Pieces[i].start == current_piece.start && Pieces[i].source == current_piece.source){
    //                 Pieces.erase(Pieces.begin()+i);
    //                 current_piece = Pieces[i-1];
    //             }
    //         }
    //     }
    //     if(current_piece.length >0) current_piece.length--;
    // }
}