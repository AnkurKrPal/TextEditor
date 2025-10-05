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

// A utility function to left rotate
// subtree rooted with x
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

int PieceTable::weightUpdator(pieceNode* node){
    if(node==current_piece){
        return current_piece->length-1;
    }
    int weightUpdation;
    if (currIndex <= node->weight)
    {
        weightUpdation = weightUpdator(node->left);
        node->weight += weightUpdation;
    }
    else if (currIndex >= node->weight + node->length)
    {
        weightUpdation = weightUpdator(node->right);
    }
    return weightUpdation;
}
void PieceTable::insert(char c, int index)
{
    
    if (state == 0)
    {
        
        if(current_piece){
            weightUpdator(head);
        }
        currIndex = index ;
        head = createInsert(head, c, index, 1, 0);
        state = 1;
    }

    else if (state == 1)
    {
        current_piece->length++;
        addString.push_back(c);
    }
}
// type(0) : normal single character insertion
// type(1) : pieceNode part insertion
pieceNode *PieceTable::createInsert(pieceNode *node, char c, int index, int weightUpdation, int type)
{
    
    if (node == NULL)
    {
        pieceNode *newNode = new pieceNode(ADD, addString.length(), weightUpdation);
        current_piece = newNode;
        if (type == 0)
        {
            addString.push_back(c);
        }
        return newNode;
    }
    if (index <= node->weight)
    {
        node->left = createInsert(node->left, c, index, weightUpdation, type);
        node->weight += weightUpdation;
    }
    else if (index >= node->weight + node->length)
    {
        node->right = createInsert(node->right, c, index - node->weight - node->length, weightUpdation, type);
    }
    else
    {

        //////////////////////////CREATE LEFT CHILD/////////////////////////////

        node->left = createInsert(node->left, c, index, index - node->weight, 1);
        current_piece->source = node->source;
        current_piece->start = node->start;
        current_piece->left = NULL;
        current_piece->right = NULL;
        current_piece->height = 1;
        current_piece->weight = 0;

        

        //////////////////////////CREATE RIGHT CHILD////////////////////////////

        node->right = createInsert(node->right, c, index - node->weight - node->length, node->length + node->weight - index, 1);
        current_piece->source = node->source;
        current_piece->start = node->start + index - node->weight;
        current_piece->left = NULL;
        current_piece->right = NULL;
        current_piece->height = 1;
        current_piece->weight = 0;

        node->weight += index - node->weight;

        current_piece = node;
        current_piece->start = addString.length();
        current_piece->length = 1;
        addString.push_back(c);
    }
    node->height = 1 + max(height(node->left), height(node->right));
    int balance = getBalance(node);

    /////////conditions need to be confirmed

    // Left Left Case
    if (balance > 1 && index <= node->left->weight)
    {
        pieceNode *toReturn = rightRotate(node);
        node->weight -= toReturn->weight + toReturn->length;
        return toReturn;
    }
    // Right Right Case
    if (balance < -1 && index > node->right->length + node->right->weight)
    {
        pieceNode *toReturn = leftRotate(node);
        toReturn->weight += node->weight + node->length;
        return toReturn;
    }
    // Left Right Case
    if (balance > 1 && index > node->left->length + node->left->weight)
    {
        node->left = leftRotate(node->left);
        pieceNode *toReturn = rightRotate(node);
        toReturn->weight += toReturn->left->weight + toReturn->left->length;
        node->weight -= toReturn->weight + toReturn->length;
        return toReturn;
    }

    // Right Left Case
    if (balance < -1 && index <= node->right->weight)
    {
        node->right = rightRotate(node->right);
        pieceNode *toReturn = leftRotate(node);
        toReturn->right->weight -= toReturn->length + toReturn->weight;
        toReturn->weight += node->length + node->weight;
        return toReturn;
    }
    return node;
};

pieceNode *minValueNode(pieceNode *node)
{
    pieceNode *current = node;
    while (current && current->left)
        current = current->left;
    return current;
}

pieceNode *PieceTable::deletion(pieceNode *node, int index, int weightUpdation)
{

    if (!node) return node;      
    if (index <= node->weight)
    {
        node->left = deletion(node->left, index, weightUpdation);
        node->weight -= weightUpdation;
    }
    else if (index > node->weight + node->length)
    {
        node->right = deletion(node->right, index - node->weight - node->length, weightUpdation);
    }
    else
    {
        current_piece = node;
        int relativeIndex = index - node->weight;

        if (node->length == 1)
        {
            if (node->left == NULL || node->right == NULL)
            {
                pieceNode *temp = node->left ? node->left : node->right;
                delete node;
                node = temp;
                current_piece = NULL;
            }
            else
            {
                pieceNode *successor = minValueNode(node->right);
                node->source = successor->source;
                node->start = successor->start;
                node->length = successor->length;
                node->right = deletion(node->right, 0, weightUpdation);
            }
        }
        else if (relativeIndex == 1)
        {
            node->start++;
            node->length--;
        }
        else if (relativeIndex == node->length)
        {
            node->length--;
        }
        else
        {
            pieceNode *right_part = new pieceNode(node->source, node->start + relativeIndex, node->length - relativeIndex);
            node->length = relativeIndex - 1;

            if (node->right == NULL)
            {
                node->right = right_part;
            }
            else
            {
                pieceNode *successor_parent = minValueNode(node->right);
                successor_parent->left = right_part;
            }
        }
    }

    if (node == NULL)
    {
        return node;
    }

    bool retFlag;
    pieceNode *retVal = balanceFunction(node, index, retFlag);
    if (retFlag)
        return retVal;
    return node;
}

pieceNode *PieceTable::balanceFunction(pieceNode *node, int index, bool &retFlag)
{
    retFlag = true;
    node->height = 1 + max(height(node->left), height(node->right));
    int balance = getBalance(node);

    /////////conditions need to be confirmed

    // Left Left Case
    if (balance > 1 && index <= node->left->weight)
    {
        pieceNode *toReturn = rightRotate(node);
        node->weight -= toReturn->weight + toReturn->length;
        return toReturn;
    }
    // Right Right Case
    if (balance < -1 && index > node->right->length + node->right->weight)
    {
        pieceNode *toReturn = leftRotate(node);
        toReturn->weight += node->weight + node->length;
        return toReturn;
    }
    // Left Right Case
    if (balance > 1 && index > node->left->length + node->left->weight)
    {
        node->left = leftRotate(node->left);
        pieceNode *toReturn = rightRotate(node);
        toReturn->weight += toReturn->left->weight + toReturn->left->length;
        node->weight -= toReturn->weight + toReturn->length;
        return toReturn;
    }

    // Right Left Case
    if (balance < -1 && index <= node->right->weight)
    {
        node->right = rightRotate(node->right);
        pieceNode *toReturn = leftRotate(node);
        toReturn->right->weight -= toReturn->length + toReturn->weight;
        toReturn->weight += node->length + node->weight;
        return toReturn;
    }
    retFlag = false;
    return node;
};


void PieceTable::handleBackSpace(int index)
{

    if (current_piece != NULL && index == last_cursor_pos - 1 && current_piece->length > 0)
    {
        if (current_piece && current_piece->length > 0)
        {
            current_piece->length--;
            if (current_piece->length == 0)
            {
                current_piece = NULL;
            }
        }
    }
    else
    {
        head = deletion(head, index + 1, 0);
    }
    last_cursor_pos = index;
}

// int PieceTable::nextIndex(int index, pieceNode *root)
// {
//     if (index < weight)
//     {
//         return nextIndex(index, root->left);
//     }
//     else if (index < weight + root->length)
//     {
//         pieceNode *newPiece = new pieceNode;
//         newPiece->source = root->source;
//         newPiece->start = root->start;
//         newPiece->length = index - root->weight;
//         newPiece->weight = root->weight;
//         newPiece->left = root->left;
//         newPiece->right = NULL;
//         newPiece->height = 1 + newPiece->left->height;

//         root->length = root->length + root->weight - index;
//         root->start = newPiece->start + newPiece->length;
//         root->left = newPiece;
//         root->weight += newPiece->length;
//         root->
//     }
//     else
//     {
//         return nextIndex(index - weight - root->length, root->right);
//     }
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
// }
// void PieceTable::deletion(int index)
// {
//     // for first backspace execute if or execute else for more than once back to back
//     if (current_piece == NULL)
//     {
//         int it = nextIndex(index);

//         // if the cursor is at the first index of the first piece, then backspace isn't allowed
//         if (it == 0)
//             return;

//         // else return the address of the previous
//         current_piece = Pieces[it - 1];

//         // skip all the pieces having string length 0 and delete that piece
//         while (current_piece->length == 0)
//         {
//             if (it >= 1)
//                 it--;
//             else
//                 return;
//             current_piece = Pieces[it - 1];
//             Pieces.erase(Pieces.begin() + it);
//         }
//         // first encounter of a piece having string and decrement length by one
//         current_piece->length--;

//         // check if after deleting the current piece has a string
//         if (current_piece->length == 0)
//         {
//             Pieces.erase(Pieces.begin() + it - 1);
//             if (it - 2 >= 0)
//                 current_piece = Pieces[it - 2];
//             else
//                 current_piece = NULL;
//         }
//     }
//     else
//     {
//         /*
//             This block executes after more than once backspace one after the other
//             if block will execute till the string has more than 1 character
//             else block will execute so as to delete the block and check again for valid pieces having string
//         */
//         if (current_piece->length > 1)
//             current_piece->length--;
//         else
//         {
//             int it = nextIndex(index);
//             if (it == 0)
//                 return;
//             current_piece = Pieces[it - 1];
//             while (current_piece->length == 0)
//             {
//                 if (it >= 1)
//                     it--;
//                 else
//                     return;
//                 current_piece = Pieces[it - 1];
//                 Pieces.erase(Pieces.begin() + it);
//             }
//             current_piece->length--;

//             if (current_piece->length == 0)
//             {
//                 Pieces.erase(Pieces.begin() + it - 1);
//                 if (it - 2 >= 0)
//                     current_piece = Pieces[it - 2];
//                 else
//                     current_piece = NULL;
//             }
//         }
//     }

//     // set state to 0 so as to activate the block for creating new piece
//     state = 0;
// }

void PieceTable::view(pieceNode* node)
{
    if(!node)return ;

    view(node->left);
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

    view(node->right);
}

// void PieceTable::start()
// {
//     // input from file to string
//     ifstream inFile("org.txt");
//     char ch;
//     while (inFile.get(ch))
//     {
//         originalString += ch;
//     }
//     inFile.close();

//     // clearing the file
//     ofstream outFile("org.txt", ios::out);
//     outFile.close();

//     // making a new single piece
//     piece *p = new piece;
//     p->source = ORIGINAL;
//     p->start = 0;
//     p->length = originalString.length();
//     Pieces.push_back(p);

//     state = 0;
//     current_piece = NULL;
// }

// void PieceTable::end()
// {
//     // traversing through all pieces and storing in file
//     ofstream outFile("org.txt");
//     for (auto it : Pieces)
//     {
//         if (it->source == ADD)
//         {
//             outFile.write(&addString[it->start], it->length);
//         }
//         else
//         {
//             outFile.write(&originalString[it->start], it->length);
//         }
//     }
//     outFile.close();

//     // clearing all pieces and string
//     for (auto p : Pieces)
//         delete p;
//     Pieces.clear();
//     originalString.clear();
//     addString.clear();
// }