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
        vector<pieceNode *> Pieces;
        int currIndex ;
        int weightUpdator(pieceNode* node);
        void nodeDeletion(pieceNode* node);
        pieceNode* preNode = NULL ;
        int delCount = 0;

        void weightUpdator2(pieceNode* node);
        void insert(char c, int index);
        pieceNode *deletion(pieceNode *node, int index, int weightUpdation);
        pieceNode *balanceFunction(pieceNode *node, int index, bool &retFlag);
        void handleBackSpace(int cursor_pos);
        pieceNode *createInsert(pieceNode *node, char c, int index, int weightUpdation, int type);
        void view(pieceNode* node);

};

#endif