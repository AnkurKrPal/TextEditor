#ifndef PIECETABLE_H
#define PIECETABLE_H

#include <vector>
#include <string>
using namespace std;

class PieceTable {
public:
    
    int state=0;
    enum BufferType {
        ORIGINAL,
        ADD
    };

    typedef struct Piece {
        BufferType source;
        size_t start;
        size_t length;
    }piece;

    void insert(char c, int index);
    void deletion(int index);
    int nextIndex(int index);
    void view();
    piece* current_piece = NULL;
    
private:
    string originalString="";
    string addString="";
    vector<Piece> Pieces;
    
};

#endif