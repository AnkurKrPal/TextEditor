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
int PieceTable::weightUpdator(pieceNode* node, int index){
    if(node==current_piece){
        return current_piece->length-1;
    }
    int weightUpdation;
    if (index <= node->weight)
    {
        weightUpdation = weightUpdator(node->left, index);
        node->weight += weightUpdation;
    }
    else if (index >= node->weight + node->length)
    {
        weightUpdation = weightUpdator(node->right, index-node->weight-node->length);
    }
    return weightUpdation;
}
void PieceTable::weightUpdator2(pieceNode* node, int index){
    if(delCount==0||node==current_piece){
        return;
    }
    if (index <= node->weight)
    {
        weightUpdator2(node->left,index);
        node->weight -= delCount;
    }
    else if (index >= node->weight + node->length)
    {
        weightUpdator2(node->right, index-node->weight-node->length);
    }
}
void PieceTable::insert(char c, int index){
    if (state != 1){   
        if(delCount>0){
            weightUpdator2(head,index);
        }else if(current_piece){
            weightUpdator(head,index);
        }
        currIndex = index ;
        head = createInsert(head, c, index, 1, 0);
        state = 1;
    }

    else if (state == 1)
    {
        current_piece->length++;
        GlobalIndex++;
        addString.push_back(c);
    }
}
pieceNode *PieceTable::createInsert(pieceNode *node, char c, int index, int weightUpdation, int type)
{
    
    if (node == NULL)
    {
        pieceNode *newNode = new pieceNode(ADD, addString.length(), weightUpdation);
        current_piece = newNode;
        GlobalIndex++;
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

   return balanceFunction(node, index);
};
void PieceTable::predecessor(pieceNode* node, pieceNode* &t, int i){
    if (i < 0) return;
    if(node==current_piece){
        if(node->left){
            node=node->left;
            while(node->right)node=node->right;
            t=node;
        }
        
    }else if (currIndex <= node->weight)
    {   
        
        predecessor(node->left,t,i);
    }
    else if (currIndex >= node->weight + node->length)
    {
        t=node;
        predecessor(node->right,t,i-node->weight + node->length);
    }

}
void PieceTable::deletion(int index){
    if(state==1){
        weightUpdator(head,index);
        state=2;
    }
    if(state == 2){
        current_piece->length--;
        GlobalIndex--;
        if(current_piece->length ==0){
            pieceNode* temp;
            predecessor(head,temp,currIndex-1);
            head=AVLDeletion(head,index-1);
            current_piece=temp;
            currIndex=GlobalIndex;
            delCount=0;
        }
    }else{
        newDeletion(head,index);
        state=2;
    }
}
pieceNode* PieceTable::newDeletion(pieceNode* node, int index){
    if (!node) return NULL;      
    if (index <= node->weight){
        node->left = newDeletion(node->left, index);
    }
    else if (index > node->weight + node->length){
        node->right = newDeletion(node->right, index - node->weight - node->length);
    }else if(index == node->weight + node->length){
        current_piece=node;
        current_piece->length--;
        GlobalIndex--;
        currIndex=GlobalIndex;
        if(current_piece->length ==0){
            pieceNode* temp;
            predecessor(head,temp,currIndex);
            head=AVLDeletion(head,currIndex);
            current_piece=temp;
            delCount=0;
        }
    }else{
        node->left=createInsert(node->left,'a',index,index-node->weight,1);
        current_piece->source = node->source;
        current_piece->start = node->start;
        current_piece->left = NULL;
        current_piece->right = NULL;
        current_piece->height = 1;
        current_piece->weight = 0;
        currIndex=GlobalIndex;
        node->start=node->start + index - node->weight;
        node->length=node->length + node->weight - index;
        node->weight += index - node->weight;
        current_piece->length--;
        GlobalIndex--;
        currIndex=GlobalIndex;
        if(current_piece->length ==0){
            pieceNode* temp;
            predecessor(head,temp,currIndex);
            head=AVLDeletion(head,currIndex);
            current_piece=temp;
            delCount=0;
        }
    }
    return balanceFunction(node, index);
}
pieceNode *minValueNode(pieceNode *node)
{
    pieceNode *current = node;
    while (current->left){
        current = current->left;
    }
    return current;
}

pieceNode *PieceTable::AVLDeletion(pieceNode *node, int index){
    if (!node) return NULL;      
    if (index <= node->weight){
        node->weight-=delCount;
        node->left = AVLDeletion(node->left, index);
    }
    else if (index > node->weight + node->length){
        node->right = AVLDeletion(node->right, index - node->weight - node->length);
    }
    else
    {
        if ((node->left == nullptr) || 
            (node->right == nullptr)) {
            pieceNode *temp = node->left ? 
                         node->left : node->right;

            // No child case
            if (temp == nullptr) {
                temp = node;
                node = nullptr;
            } else // One child case
                *node = *temp; // Copy the contents of 
                               // the non-empty child
            free(temp);
        } else {
            // node with two children: Get the 
            // inorder successor (smallest in 
            // the right subtree)
            pieceNode* temp = minValueNode(node->right);

            // Copy the inorder successor's 
            // data to this node
            node->source = temp->source;
            node->length = temp->length;////////this block can be optimized
            node->start = temp->start;
            node->source = temp->source;
            node->source = temp->source;

            // Delete the inorder successor
            delCount=node->length;
            node->right = AVLDeletion(node->right, index-node->weight-node->length);
        }
    
    }
    return balanceFunction(node, index);
}

pieceNode *PieceTable::balanceFunction(pieceNode *node, int index)
{
    if(!node)return NULL;
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



