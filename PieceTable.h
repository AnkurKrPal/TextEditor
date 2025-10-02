#ifndef PIECETABLE_H
#define PIECETABLE_H

#include <vector>
#include <string>
using namespace std;

enum BufferType {
        ORIGINAL,
        ADD
    };
typedef struct pieceNode {
        BufferType source;
        size_t start;
        size_t length;
        pieceNode* left;
        pieceNode* right;
        int weight;
        int height;
}pieceNode;

class PieceTable {

    public:
        
        int state=0;
        void insert(char c, int index);
        void deletion(int index);
        int nextIndex(int index);
        void view();
        piece* current_piece = NULL;
        string originalString="";
        string addString="";
        vector<Piece*> Pieces;    
    
};

#endif