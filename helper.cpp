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

// void PieceTable::printNode(pieceNode *node)
// {
//     if(!node){return;
//         }
//     if (node->source == ADD){
//         for (int i = 0; i < node->length; i++){
//             cout << addString[i + node->start];
//         }
//     }
//     else{
//         for (int i = 0; i < node->length; i++){
//             cout << originalString[i + node->start];
//         }
//     }
// }

// void PieceTable::view(pieceNode* node)
// {
//     if(!node)return ;
//     view(node->left);
//     printNode(node);
//     cout<<"  Left :  ";
//     printNode(node->left);
//     cout<<"  Right :  ";
//     printNode(node->right);
//     cout<<" | weight : "<<node->weight<<" | height : "<<node->height<<" | length : "<<node->length<<" | start : "<<node->start<<"  |  Address : "<<node<<endl;

//     view(node->right);
// }

std::string PieceTable::printTrial(pieceNode* node) {
    if(!node)return "";
    std::string str=(node->source==ADD)?addString.substr(node->start,node->length):originalString.substr(node->start,node->length);
    return printTrial(node->left)+str+printTrial(node->right);
}
void insertChar(PieceTable &P, char c, int &cursor){
    P.insert(c,cursor,0);
    cursor++;
}
void deleteChar(PieceTable P, int &cursor){
     if(P.state!=2)P.delCount=0;
            P.delCount++;
            P.deletion(cursor,0);
            cursor--;
}

// --------------------- UNDO / REDO WRAPPERS -------------------------
void performUndo(PieceTable &P, int &cursor) {
    // finalize current editing session
    if(P.lastStepLength>0) P.undo.push(new laststep(P.undoType  , P.lastStepLength , P.cursorStart , P.charStack));
    P.lastStepLength=0;
    if(P.state==2){P.weightUpdator2(P.head,P.currIndex);P.delCount=0;P.current_piece=NULL;}
    if(P.state==1){
        P.weightUpdator(P.head , P.currIndex);
        P.current_piece=NULL;
    }
    P.state=0;

    P.undofn();

    if(P.state==2){P.weightUpdator2(P.head,P.currIndex);P.delCount=0;P.current_piece=NULL;}
    if(P.state==1){
        P.weightUpdator(P.head , P.currIndex);
        P.current_piece=NULL;
    }
    P.state=0;

    cursor = P.GlobalIndex;
}

void performRedo(PieceTable &P, int &cursor) {
    if(P.lastStepLength>0) P.undo.push(new laststep(P.undoType  , P.lastStepLength , P.cursorStart , P.charStack));
    P.lastStepLength=0;
    if(P.state==2){P.weightUpdator2(P.head,P.currIndex);P.delCount=0;P.current_piece=NULL;}
    if(P.state==1){
        P.weightUpdator(P.head , P.currIndex);
        P.current_piece=NULL;
    }
    P.state=0;

    P.redofn();

    if(P.state==2){P.weightUpdator2(P.head,P.currIndex);P.delCount=0;P.current_piece=NULL;}
    if(P.state==1){
        P.weightUpdator(P.head , P.currIndex);
        P.current_piece=NULL;
    }
    P.state=0;

    cursor = P.GlobalIndex;
}
