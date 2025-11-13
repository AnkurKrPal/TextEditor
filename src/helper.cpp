#include "PieceTable.h"
#include <bits/stdc++.h>
using namespace std;

int height(pieceNode *N)
{
    if (N == nullptr)
        return 0;
    return N->height;
}

int getBalance(pieceNode *N)
{
    if (N == nullptr)
        return 0;
    return height(N->left) - height(N->right);
}

pieceNode *rightRotate(pieceNode *y)
{
    pieceNode *x = y->left;
    pieceNode *T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = 1 + max(height(y->left), height(y->right));
    x->height = 1 + max(height(x->left), height(x->right));
    return x;
}

pieceNode *leftRotate(pieceNode *x)
{
    pieceNode *y = x->right;
    pieceNode *T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = 1 + max(height(x->left), height(x->right));
    y->height = 1 + max(height(y->left), height(y->right));
    return y;
}

std::string PieceTable::printTrial(pieceNode *node)
{
    if (!node)
        return "";
    std::string str = (node->source == ADD) ? addString.substr(node->start, node->length) : originalString.substr(node->start, node->length);
    return printTrial(node->left) + str + printTrial(node->right);
}

void insertChar(PieceTable &P, char c, int &cursor)
{
    P.insert(c, cursor, 0);
    cursor++;
}

void deleteChar(PieceTable &P, int &cursor)
{
    if (cursor > 0 && P.GlobalIndex > 0)
    {
        if (P.state != 2)
            P.delCount = 0;
        P.delCount++;
        P.deletion(cursor, 0);
        cursor = P.GlobalIndex;
    }
}

void performUndo(PieceTable &P, int &cursor)
{
    if (P.lastStepLength > 0)
        P.undo.push(new laststep(P.undoType, P.lastStepLength, P.cursorStart, P.charStack));
    P.lastStepLength = 0;
    if (P.state == 2)
    {
        P.weightUpdator2(P.head, P.currIndex);
        P.delCount = 0;
        P.current_piece = NULL;
    }
    if (P.state == 1)
    {
        P.weightUpdator(P.head, P.currIndex);
        P.current_piece = NULL;
    }
    P.state = 0;

    P.undofn();

    if (P.state == 2)
    {
        P.weightUpdator2(P.head, P.currIndex);
        P.delCount = 0;
        P.current_piece = NULL;
    }
    if (P.state == 1)
    {
        P.weightUpdator(P.head, P.currIndex);
        P.current_piece = NULL;
    }
    P.state = 0;

    cursor = P.GlobalIndex;
}

void performRedo(PieceTable &P, int &cursor)
{
    if (P.lastStepLength > 0)
        P.undo.push(new laststep(P.undoType, P.lastStepLength, P.cursorStart, P.charStack));
    P.lastStepLength = 0;
    if (P.state == 2)
    {
        P.weightUpdator2(P.head, P.currIndex);
        P.delCount = 0;
        P.current_piece = NULL;
    }
    if (P.state == 1)
    {
        P.weightUpdator(P.head, P.currIndex);
        P.current_piece = NULL;
    }
    P.state = 0;

    P.redofn();

    if (P.state == 2)
    {
        P.weightUpdator2(P.head, P.currIndex);
        P.delCount = 0;
        P.current_piece = NULL;
    }
    if (P.state == 1)
    {
        P.weightUpdator(P.head, P.currIndex);
        P.current_piece = NULL;
    }
    P.state = 0;

    cursor = P.GlobalIndex;
}