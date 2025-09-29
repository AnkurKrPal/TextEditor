#include <bits/stdc++.h>
using namespace std ;

struct piece{
    string source;
    int start;
    int length;
};

int main(){
    vector<piece> Pieces;
    int state;
    piece current_piece;
    string add;
    char c; //decided from keyboard
    int k; //decided from cursor
    bool backspace; //decided from keyboard

    //if (cursor_change || arrow_key_press)state=0;


    //insertion

    if(state==0){
        current_piece.source = add;
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
    

    //deletion

    while(backspace){
        if(current_piece.length==0){
            for(int i=1;i<Pieces.size();i++){
                if(Pieces[i].start == current_piece.start && Pieces[i].source == current_piece.source){
                    Pieces.erase(Pieces.begin()+i);
                    current_piece = Pieces[i-1];
                }
            }
        }
        if(current_piece.length >0) current_piece.length--;
    }
}