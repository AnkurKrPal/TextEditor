#include "PieceTable.h"
using namespace std;

using namespace std;

void PieceTable::insert(char c, int index) {
    if(state==0){
        current_piece.source = ADD;
        current_piece.start = addString.length();
        current_piece.length = 1;
        int i= nextIndex(index);
        addString.push_back(c);
        Pieces.insert(Pieces.begin()+i , current_piece);
        state = 1;
    }

    else if(state == 1){
        current_piece.length++;
        addString.push_back(c);
    }
}

void PieceTable::deletion(int index) {
    if(current_piece.length >0) current_piece.length--;
    else{
        int it = nextIndex(index);
        if(it==0)return ;
        current_piece = Pieces[it-1];
        while(current_piece.length==0){
            // Pieces.erase(Pieces.begin()+it-1);
            current_piece = Pieces[it-1];
            if(it>=1)it--;
        }
        current_piece.length--;
    }
    
    state =0 ;
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