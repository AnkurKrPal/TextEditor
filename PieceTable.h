#ifndef PIECETABLE_H
#define PIECETABLE_H

#include <vector>
#include <string>
using namespace std;

enum BufferType {
        ORIGINAL,
        ADD
    };
struct pieceNode {
        BufferType source;
        size_t start;
        size_t length;
        pieceNode* left;
        pieceNode* right;
        int weight;
        int height;
        pieceNode(BufferType src, size_t s, size_t l){
            source=src;
            start=s;
            length=l;
            left=NULL;
            right=NULL;
            height=1;
            weight=0;
        }
};

class PieceTable {

    public:
        pieceNode* current_piece = NULL;
        string originalString="";
        string addString="";
        vector<Piece*> Pieces; 
        int state=0;
        void insert(char c, int index);
        void deletion(int index);
        int nextIndex(int index);
        pieceNode* createInsert(pieceNode* node,char c, int index);
        void view();
           
    
};

#endif