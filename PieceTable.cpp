#include "PieceTable.h"
#include <bits/stdc++.h>
using namespace std;

void PieceTable::insert(char c, int index) {
    if(state==0){
        current_piece->source = ADD;
        current_piece->start = addString.length();
        current_piece->length = 1;
        int i= nextIndex(index);
        addString.push_back(c);
        Pieces.insert(Pieces.begin()+i , *current_piece);
        state = 1;
    }

    else if(state == 1){
        current_piece->length++;
        addString.push_back(c);
    }
}

void PieceTable::deletion(int index) {
    // for first backspace execute if or execute else for more than once back to back
    if(current_piece == NULL){
        int it = nextIndex(index);

        // if the cursor is at the first index of the first piece, then backspace isn't allowed
        if(it == 0) return ;

        // else return the address of the previous
        current_piece = &Pieces[it-1];
        
        // skip all the pieces having string length 0 and delete that piece
        while(current_piece->length == 0){
            if( it >= 1) it--;
            else return;
            current_piece = &Pieces[it-1];
            Pieces.erase(Pieces.begin() + it);
        }
        // first encounter of a piece having string and decrement length by one
        current_piece->length--;
        
        // check if after deleting the current piece has a string
        if(current_piece->length == 0){
            Pieces.erase(Pieces.begin() + it - 1);
            if(it -2 >= 0) current_piece = &Pieces[it-2];
            else current_piece = NULL ;
        }
    }else{
        /*
            This block executes after more than once backspace one after the other
            if block will execute till the string has more than 1 character
            else block will execute so as to delete the block and check again for valid pieces having string
        */
        if(current_piece->length > 1) current_piece->length--;
        else{
            int it = nextIndex(index);
            if(it == 0)return ;
            current_piece = &Pieces[it-1];
            while(current_piece->length == 0){
                if( it >= 1) it--;
                else return;
                current_piece = &Pieces[it-1];
                Pieces.erase(Pieces.begin() + it);
            }
            current_piece->length--;
            
            if(current_piece->length == 0){
                Pieces.erase(Pieces.begin() + it - 1);
                if(it - 2 >= 0) current_piece = &Pieces[it-2];
                else current_piece = NULL ;
            }
        }
    }

    // set state to 0 so as to activate the block for creating new piece
    state = 0 ;
}

int PieceTable::nextIndex(int index){
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