#ifndef PIECETABLE_H
#define PIECETABLE_H

#include <vector>
#include <string>
using namespace std;

enum BufferType
{
    ORIGINAL,
    ADD
};
struct pieceNode
{
    BufferType source;
    size_t start;
    size_t length;
    pieceNode *left;
    pieceNode *right;
    int weight;
    int height;

    pieceNode(BufferType src, size_t s, size_t l)
    {
        source = src;
        start = s;
        length = l;
        left = NULL;
        right = NULL;
        height = 1;
        weight = 0;
    }
};

class PieceTable
{

    public:
        pieceNode *current_piece = NULL;
        string originalString = "";
        pieceNode *head=NULL;
        string addString = "";
        int state = 0;
        int GlobalIndex = 0;
        int currIndex ;
        int delCount = 0;

        int weightUpdator(pieceNode* node, int index);
        pieceNode * AVLDeletion(pieceNode* node, int index);
        void predecessor(pieceNode* node, pieceNode* &t, int i);
        void printNode(pieceNode* node);
        void weightUpdator2(pieceNode* node, int index);
        void insert(char c, int index);
        void deletion(int index);
        pieceNode* newDeletion(pieceNode *node, int index);
        pieceNode *balanceFunction(pieceNode *node, int index);
        pieceNode *createInsert(pieceNode *node, char c, int index, int weightUpdation, int type);
        void view(pieceNode* node);

};

#endif