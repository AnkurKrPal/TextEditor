//#include "PieceTable.h"
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
void PieceTable::weightUpdator2(pieceNode* node){
    if(node==current_piece){
        return;
    }
    if (currIndex <= node->weight)
    {
        weightUpdator(node->left);
        node->weight -= delCount;
    }
    else if (currIndex >= node->weight + node->length)
    {
        weightUpdator(node->right);
    }
}
void PieceTable::nodeDeletion(pieceNode* node){
    pieceNode* temp = NULL;
    if(node==current_piece){
        if(node->left){
            temp = node->left;
            if(!temp->right){
                preNode = temp;
            }else{
                while(temp->right->right){
                    temp = temp->right ;
                }
                preNode = temp->right ;
            }
        }
        
        if(node->left){
            node->source = preNode->source;
            node->length = preNode->length;
            node->start = preNode->start;
            node->weight -= preNode->length;
            pieceNode* t=preNode->left;
            if(preNode == node->left){
                delete node->left;
                node->left = t;
            }
            else{
                delete temp->right ;
                temp->right = NULL;
            }

        }else if(node->right){
            node->source = node->right->source;
            node->length = node->right->length;
            node->start = node->right->start;
            node->weight = node->right->weight;
            node->height = node->right->height;
            node->left=node->right->left;
            pieceNode*temp=node->right->right;
            delete node->right;
            node->right=temp;
        }else if(preNode){
            if(preNode->right==node){
                preNode->right=NULL;
            }else{
                pieceNode* curr=preNode->right;
                while(curr->left!=node){
                    curr=curr->left;
                }
                curr->left=NULL;
            }
            delete node;
            current_piece=preNode;
            currIndex=GlobalIndex;
        }else{
            if(previousNode){
               previousNode->left=NULL; 
               delete node;
               current_piece=NULL;
               currIndex=0;
            }
            state=0;
        }
        return;
    }

    if (currIndex <= node->weight)
    {
        previousNode=node;
        nodeDeletion(node->left);
        node->weight -= delCount;
    }
    else if (currIndex >= node->weight + node->length)
    {
        preNode = node ;
        previousNode=node;
        nodeDeletion(node->right);
    }
}

void PieceTable::insert(char c, int index){
    if (state != 1){     
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
        currIndex=GlobalIndex;
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
        currIndex=GlobalIndex;
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

pieceNode *minValueNode(pieceNode *node, int weightUp)
{
    pieceNode *current = node;
    while (current && current->left){
        current->weight+=weightUp;
        current = current->left;
    }
    return current;
}

pieceNode *PieceTable::deletion(pieceNode *node, int index, int weightUpdation)
{   if(state==1){
        weightUpdator(head);
        state=2;
    }
    if(state == 2){
        current_piece->length--;
        GlobalIndex--;
        if(current_piece->length ==0){
            nodeDeletion(head);
            delCount=0;
        }
        return node;
    }
    if (!node) return NULL;      
    if (index <= node->weight){
        node->left = deletion(node->left, index, weightUpdation);
    }
    else if (index > node->weight + node->length){
        preNode = node ;
        node->right = deletion(node->right, index - node->weight - node->length, weightUpdation);
    }
    else
    {
        current_piece = node;
        currIndex=GlobalIndex;
        int relativeIndex = index - node->weight;

        // if (node->length == 0)
        // {   
        //     if (!node->left || !node->right)
        //     {
        //         pieceNode *temp = node->left ? node->left : node->right;
        //         delete node;
        //         state = 0 ;
        //         return temp; 
        //     }

        //     pieceNode *successor = minValueNode(node->right);

        //     node->source = successor->source;
        //     node->start = successor->start;
        //     node->length = successor->length;

        //     node->right = deletion(node->right, 0, weightUpdation);
            
        //     state = 0 ;
        // }
        if (relativeIndex == 1)
        {
            node->start++;
            node->length--;
            GlobalIndex--;
            if(node->length==0) nodeDeletion(head);
            else{
                if(node->left){
                    pieceNode* temp = node->left;
                    while(temp->right){
                        temp = temp->right ;
                    }
                    preNode = temp ;
                }
                current_piece=preNode;
                currIndex=GlobalIndex;
                preNode=NULL;
            }
        }
        else if (relativeIndex == node->length)
        {
            node->length--;
            GlobalIndex--;
        }
        else
        {
            pieceNode *right_part = new pieceNode(node->source, node->start + relativeIndex, node->length - relativeIndex);
            
            if (node->right == NULL)
            {
                node->right = right_part;
            }
            else
            {
                pieceNode *successor_parent = minValueNode(node->right, node->length - relativeIndex);
                successor_parent->left = right_part;
            }
            
            node->length = relativeIndex -1;
            GlobalIndex--;
        }
    }
    state = 2;

    // if (node == NULL)
    // {
    //     return node;
    // }

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
    cout<<" | weight : "<<node->weight<<endl;

    view(node->right);
}

