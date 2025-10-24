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

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = 1 + max(height(y->left),
                        height(y->right));
    x->height = 1 + max(height(x->left),
                        height(x->right));

    // Return new root
    return x;
}
pieceNode *leftRotate(pieceNode *x)
{
    pieceNode *y = x->right;
    pieceNode *T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = 1 + max(height(x->left),
                        height(x->right));
    y->height = 1 + max(height(y->left),
                        height(y->right));

    // Return new root
    return y;
}
void PieceTable::printNode(pieceNode* node){
    if(!node){cout<<"NULL";return;}
    if (node->source == ADD){
        for (int i = 0; i < node->length; i++){
            cout << addString[i + node->start];
        }
    }
    else{
        for (int i = 0; i < node->length; i++){
            cout << originalString[i + node->start];
        }
    }
}

void PieceTable::view(pieceNode* node)
{
    if(!node)return ;
    view(node->left);
    printNode(node);
    cout<<"  Left :  ";
    printNode(node->left);
    cout<<"  Right :  ";
    printNode(node->right);
    cout<<" | weight : "<<node->weight<<" | height : "<<node->height<<" | length : "<<node->length<<" | start : "<<node->start<<"  |  Address : "<<node<<endl;

    view(node->right);
}