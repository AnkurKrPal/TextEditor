#ifndef PIECETABLE_H
#define PIECETABLE_H

#include <bits/stdc++.h>
using namespace std;

enum BufferType
{
    ORIGINAL,
    ADD
};

enum undoBufferType
{
    addition,
    subtraction
};

struct laststep
{
    undoBufferType command;
    long long lastStepLength;
    long long cursorStart;
    vector<char> charStack;

    laststep(undoBufferType cmmd, long long lngth, long long cursorStrt, vector<char> charstack)
    {
        command = cmmd;
        lastStepLength = lngth;
        cursorStart = cursorStrt;
        charStack = charstack;
    }
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

    pieceNode *head = NULL;

    string addString = "";

    int state = 0;

    int GlobalIndex = 0;

    int currIndex;

    int delCount = 0;

    long long lastStepLength;

    long long cursorStart;

    vector<char> charStack;

    undoBufferType undoType;

    stack<laststep *> undo;

    stack<laststep *> redo;
    char deletedChar;

    int weightUpdator(pieceNode *node, int index);

    pieceNode *AVLDeletion(pieceNode *node, int index, pieceNode *type = NULL);

    int predecessor(pieceNode *node, pieceNode *&t, int i);

    void weightUpdator2(pieceNode *node, int index);

    void insert(char c, int index, int lastStepType);

    void deletion(int index, int lastStepType);

    pieceNode *newDeletion(pieceNode *node, int index);

    pieceNode *balanceFunction(pieceNode *node, int index);

    pieceNode *createInsert(pieceNode *node, char c, int index, int weightUpdation, int type);

    std::string printTrial(pieceNode *node);

    void undofn();

    void redofn();
};

int height(pieceNode *Node);

int getBalance(pieceNode *Node);

pieceNode *rightRotate(pieceNode *y);

pieceNode *leftRotate(pieceNode *x);

void insertChar(PieceTable &P, char c, int &cursor);

void deleteChar(PieceTable &P, int &cursor);

void performUndo(PieceTable &P, int &cursor);

void performRedo(PieceTable &P, int &cursor);

#endif