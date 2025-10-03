#include "PieceTable.h"
#include <bits/stdc++.h>
using namespace std;
typedef pair<pieceNode*,int> pack;//node,sum of all lengths
void PieceTable::insert(char c, int index)
{
    if (state == 0)
    {
        createInsert(c,index);
        state=1;
    }

    else if (state == 1)
    {
        current_piece->length++;
        addString.push_back(c);
    }
}
pack PieceTable::createInsert(pieceNode* node,char c, int index){
    if(node==NULL){
        pieceNode* newNode=new pieceNode;
        current_piece=newNode;
        current_piece->start=addString.length();
        current_piece->length=1;
        addString.push_back(c);
        return {newNode,1};
    }
    pack packet;
    if(index<= node->weight){
        packet=createInsert(node->left,c,index);
        node->left= packet.first;
    }else if(index>=node->weight+node->length){
        packet=createInsert(node->right,c, index - node->weight - node->length);
        node->right= packet.first;
    }else{

        //////////////////////////CREATE LEFT CHILD/////////////////////////////

        packet=createInsert(node->left,c,index);
        node->left= packet.first;
        current_piece->source=node->source;
        current_piece->start=node->start;
        current_piece->length=index-node->weight;
        current_piece->left=NULL;
        current_piece->right=NULL;
        current_piece->height=1;
        current_piece->weight=0;

        //////////////////////////CREATE RIGHT CHILD////////////////////////////

        packet=createInsert(node->right,c, index - node->weight - node->length);
        node->right= packet.first;
        current_piece->source=node->source;
        current_piece->start=node->start+index-node->weight;
        current_piece->length=node->length+node->weight-index;
        current_piece->left=NULL;
        current_piece->right=NULL;
        current_piece->height=1;
        current_piece->weight=0;

        current_piece=node;
    }
};
int PieceTable::nextIndex(int index, pieceNode *root)
{
    if (index < weight)
    {
        return nextIndex(index, root->left);
    }
    else if (index < weight + root->length)
    {
        pieceNode *newPiece = new pieceNode;
        newPiece->source = root->source;
        newPiece->start = root->start;
        newPiece->length = index - root->weight;
        newPiece->weight = root->weight;
        newPiece->left = root->left;
        newPiece->right = NULL;
        newPiece->height=1+newPiece->left->height;

        root->length = root->length + root->weight - index;
        root->start = newPiece->start + newPiece->length;
        root->left = newPiece;
        root->weight += newPiece->length;
        root->
    }
    else
    {
        return nextIndex(index - weight - root->length, root->right);
    }
    // for(auto &it:Pieces){
    //     i++;
    //     index-=it->length;
    //     if(index==0){
    //         return i;
    //     }
    //     if(index<0){
    //         int n=it->start;
    //         int len=it->length;
    //         it->length=originalIndex-n;
    //         piece* newPiece=new piece;
    //         newPiece->source=it->source;
    //         newPiece->start=originalIndex;
    //         newPiece->length=len+n-originalIndex;
    //         Pieces.insert(Pieces.begin() + i, newPiece);
    //         return i;
    //     }
    // }
    // return Pieces.size();
}
void PieceTable::deletion(int index)
{
    // for first backspace execute if or execute else for more than once back to back
    if (current_piece == NULL)
    {
        int it = nextIndex(index);

        // if the cursor is at the first index of the first piece, then backspace isn't allowed
        if (it == 0)
            return;

        // else return the address of the previous
        current_piece = Pieces[it - 1];

        // skip all the pieces having string length 0 and delete that piece
        while (current_piece->length == 0)
        {
            if (it >= 1)
                it--;
            else
                return;
            current_piece = Pieces[it - 1];
            Pieces.erase(Pieces.begin() + it);
        }
        // first encounter of a piece having string and decrement length by one
        current_piece->length--;

        // check if after deleting the current piece has a string
        if (current_piece->length == 0)
        {
            Pieces.erase(Pieces.begin() + it - 1);
            if (it - 2 >= 0)
                current_piece = Pieces[it - 2];
            else
                current_piece = NULL;
        }
    }
    else
    {
        /*
            This block executes after more than once backspace one after the other
            if block will execute till the string has more than 1 character
            else block will execute so as to delete the block and check again for valid pieces having string
        */
        if (current_piece->length > 1)
            current_piece->length--;
        else
        {
            int it = nextIndex(index);
            if (it == 0)
                return;
            current_piece = Pieces[it - 1];
            while (current_piece->length == 0)
            {
                if (it >= 1)
                    it--;
                else
                    return;
                current_piece = Pieces[it - 1];
                Pieces.erase(Pieces.begin() + it);
            }
            current_piece->length--;

            if (current_piece->length == 0)
            {
                Pieces.erase(Pieces.begin() + it - 1);
                if (it - 2 >= 0)
                    current_piece = Pieces[it - 2];
                else
                    current_piece = NULL;
            }
        }
    }

    // set state to 0 so as to activate the block for creating new piece
    state = 0;
}

void PieceTable::view()
{
    for (auto it : Pieces)
    {
        if (it->source == ADD)
        {
            for (int i = 0; i < it->length; i++)
            {
                cout << addString[i + it->start];
            }
        }
        else
        {
            for (int i = 0; i < it->length; i++)
            {
                cout << originalString[i + it->start];
            }
        }
    }
    cout << endl;
}



void PieceTable::start(){
    //input from file to string
    ifstream inFile("org.txt");
    char ch;
    while (inFile.get(ch)) {
        originalString += ch;
    }
    inFile.close();

    //clearing the file
    ofstream outFile("org.txt", ios::out);
    outFile.close();

    //making a new single piece
    piece* p = new piece;
    p->source = ORIGINAL;
    p->start = 0;
    p->length = originalString.length();
    Pieces.push_back(p);

    state=0;
    current_piece=NULL;
}

void PieceTable::end(){
    //traversing through all pieces and storing in file
    ofstream outFile("org.txt");
    for (auto it :Pieces) {
        if (it->source ==ADD) {
            outFile.write( &addString[it->start] , it->length);
        } else {
            outFile.write( &originalString[it->start] , it->length);
        }
    }
    outFile.close();

    //clearing all pieces and string
    for (auto p : Pieces) delete p;
    Pieces.clear();
    originalString.clear();
    addString.clear();

}