#include "PieceTable.h"
#include <bits/stdc++.h>
using namespace std;
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
            delCount=0;
        }else if(current_piece){
            cout<<"namaste"<<endl;
            weightUpdator(head,index);
        }
        
        currIndex = index ;
        GlobalIndex++;
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


   return balanceFunction(node, index);
};
int PieceTable::predecessor(pieceNode* node, pieceNode* &t, int i){
    cout<<"pred called for "<<node<<" at index "<<i<<" for current piece "<<current_piece<<endl;
    if (i < 0) return 1;
    if(node==current_piece){
        int k=0;
        if(node->left){
            k=1;
            node=node->left;
            while(node->right)node=node->right;
            t=node;
        }
        return k;
        
    }else if (i <= node->weight)
    {   
        cout<<"calling predessor left for index "<<i<<endl;
        int k=predecessor(node->left,t,i);
        if(k==0){node->weight-=delCount;cout<<"reducing weight of "<<node<<" in pred by "<<delCount<<endl;}
        return k;
    }
    else if (i >= node->weight + node->length)
    {
        t=node;
        cout<<"calling predessor right for index "<<i<<endl;
        int k= predecessor(node->right,t,i-node->weight - node->length);
        if(k==0)k=1;
        return k;
    }
    return 1;
}
void PieceTable::deletion(int index){
    if(state==1){
        weightUpdator(head,index);
        state=2;
    }
    if(state == 2){
        //current_piece->length--;
        //GlobalIndex--;
        //currIndex--;
        if(current_piece->length ==1){
            pieceNode* temp=NULL;
            int tempCount=delCount;
            delCount=0;
            predecessor(head,temp,currIndex);
            current_piece->length--;
            GlobalIndex--;
            currIndex=GlobalIndex;
            delCount=tempCount;
            head=AVLDeletion(head,index-1);
            current_piece=temp;
            delCount=0;
        }else{
            current_piece->length--;
            GlobalIndex--;
            currIndex--;
        }
    }else{
        newDeletion(head,index);
        state=2;
    }
}
pieceNode* PieceTable::newDeletion(pieceNode* node, int index){
    cout<<"newdel called for node"<<node<<endl;
    if (!node) return NULL;      
    if (index <= node->weight){
        node->left = newDeletion(node->left, index);
    }
    else if (index > node->weight + node->length){
        cout<<"hii"<<endl;
        node->right = newDeletion(node->right, index - node->weight - node->length);
    }else if(index == node->weight + node->length){
        cout<<"hiiiiii"<<endl;
        current_piece=node;
        if(current_piece->length ==1){
            pieceNode* temp=NULL;
            predecessor(head,temp,currIndex);
            current_piece->length--;
            GlobalIndex--;
            currIndex=GlobalIndex;
            node=AVLDeletion(node,index);
            current_piece=temp;
            delCount=0;
        }else{
            current_piece->length--;
            GlobalIndex--;
            currIndex=GlobalIndex;
        }
    }else{
        
        node->left=createInsert(node->left,'a',index,index-node->weight,1);
        //view(head);
        cout<<"biiiiii"<<endl;
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
        view(head);
        if(current_piece->length ==0){
            cout<<"biiiiiiiiiii"<<currIndex<<endl;
            pieceNode* temp=NULL;
            predecessor(head,temp,currIndex);
            cout<<"temp is "<<temp<<endl;

            node=AVLDeletion(node,index);
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
pieceNode *PieceTable::AVLDeletion(pieceNode *node, int index, pieceNode* type){
    cout<<"AVL deletion called at index "<<index<<endl;
    if (!node) return NULL; 
    if(node->length==0||type==node){
        if ((node->left == nullptr) || 
            (node->right == nullptr)) {
            pieceNode *temp = node->left ? 
                node->left : node->right;

            // No child case
            if (temp == nullptr) {
                cout<<"NO CHILD-----"<<endl;
                temp = node;
                node = NULL;
                delete temp;
                return NULL;
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
            cout<<"Copying data"<<endl;
            node->source = temp->source;
            node->length = temp->length;////////this block can be optimized
            node->start = temp->start;
            node->source = temp->source;
            node->source = temp->source;

            // Delete the inorder successor
            delCount=node->length;
            cout<<"Calling AVL deletion with different type"<<endl;
            node->right = AVLDeletion(node->right, index-node->weight-node->length,temp);
        }
    
    }     
    else if (index <= node->weight){
        node->weight-=delCount;
        cout<<"reducing weight of "<<node<<" in AVL del by "<<delCount<<endl;
        node->left = AVLDeletion(node->left, index, type);
    }
    else if (index >= node->weight + node->length){
        cout<<"calling node right for node "<<node<<endl;
        node->right = AVLDeletion(node->right, index - node->weight - node->length, type);
        cout<<"got node right as "<<node->right<<endl;
        printNode(node->right);
        cout<<endl;
    }
    
    return balanceFunction(node, index);
}
pieceNode *PieceTable::balanceFunction(pieceNode *node, int index)
{
    if(!node){return NULL;}
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
    if (balance < -1 && index >= node->right->length + node->right->weight)
    {
        pieceNode *toReturn = leftRotate(node);
        toReturn->weight += node->weight + node->length;
        return toReturn;
    }
    // Left Right Case
    if (balance > 1 && index >= node->left->length + node->left->weight)
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
