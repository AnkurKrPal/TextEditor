#include "PieceTable.h"
#include "helper.cpp"
#include <bits/stdc++.h>
using namespace std;


static int subChar(pieceNode* n){
    if(!n) return 0;
    return subChar(n->left)+n->length+subChar(n->right);
}

static void recomputeWeights(pieceNode* n){
    if(!n) return;
    recomputeWeights(n->left);
    recomputeWeights(n->right);
    n->weight=subChar(n->left);
    n->height=1+max(height(n->left),height(n->right));
}

int PieceTable::weightUpdator(pieceNode* node, int index){
    if(!node) return 0;  // FIX: Handle NULL node
    if(node==current_piece){
        return current_piece->length-1;
    }
    int weightUpdation = 0;  // FIX: Initialize to avoid undefined behavior
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
    if(delCount==0||node==current_piece||!node){  // FIX: Add NULL check
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

void PieceTable::insert(char c, int index , int lastStepType){
    if (state != 1){   
        
        if(delCount>0){
            weightUpdator2(head,currIndex);
            delCount=0;
        }else if(current_piece){
            weightUpdator(head,currIndex);
        }
        
        currIndex = index ;
        GlobalIndex++;
        head = createInsert(head, c, index, 1, 0);
        state = 1;

        if(lastStepType==0){
            if(lastStepLength>0)undo.push(new laststep(undoType  , lastStepLength , cursorStart , charStack));
            undoType = addition;
            lastStepLength = 1;
            cursorStart = GlobalIndex - 1;
            charStack.clear();
            charStack.push_back(c);

            while(!redo.empty()) { delete redo.top(); redo.pop(); }
        }
    }
    else if (state == 1)
    {
        current_piece->length++;
        GlobalIndex++;
        addString.push_back(c);

        if(lastStepType==0){
            lastStepLength++;
            charStack.push_back(c);
        }
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
        node->source=ADD;
        current_piece = node;
        currIndex=GlobalIndex;
        current_piece->start = addString.length();
        current_piece->length = 1;
        addString.push_back(c);
    }

   return balanceFunction(node, index);
}

int PieceTable::predecessor(pieceNode* node, pieceNode* &t, int i){
    if(!node){ t = NULL; return 1; }
    if(node==current_piece){
        int k=0;
        if(node->left){
            k=1;
            pieceNode* x=node->left;
            while(x->right)x=x->right;
            t=x;
        }
        return k;
    }else if (i <= node->weight)
    {   
        return predecessor(node->left,t,i);
    }
    else if (i >= node->weight + node->length)
    {
        int k= predecessor(node->right,t,i-node->weight - node->length);
        if(k==0)k=1;
        return k;
    }
    return 1;
}

void PieceTable::deletion(int index , int lastStepType){
    // CRITICAL FIX: Check if we can delete
    if(index <= 0 || !head || GlobalIndex <= 0){
        return;
    }
    
    if(state==1){
        if(!current_piece){
            state = 0;
            return;
        }
        
        weightUpdator(head,currIndex);
        state=2;
        
        if(current_piece->length ==1){
            pieceNode* temp=NULL;
            int tempCount=delCount;
            delCount=0;
            predecessor(head,temp,currIndex);
            
            current_piece->length--;
            deletedChar=(current_piece->source==ADD)?addString[current_piece->start+current_piece->length]:originalString[current_piece->start+current_piece->length];
            GlobalIndex--;
            currIndex=GlobalIndex;
            delCount=tempCount;
            head=AVLDeletion(head,index-1);
            current_piece=temp;
            delCount=0;
            
            // FIX: If predecessor is NULL, reset state to 0
            if(!current_piece){
                state = 0;
            }
        }else{
            current_piece->length--;
            deletedChar=(current_piece->source==ADD)?addString[current_piece->start+current_piece->length]:originalString[current_piece->start+current_piece->length];
            GlobalIndex--;
            currIndex--;
        }

        if(lastStepType==0){
            if(lastStepLength>0)undo.push(new laststep(undoType , lastStepLength , cursorStart, charStack));
            charStack.clear();
            undoType = subtraction;
            lastStepLength = 1;
            cursorStart = GlobalIndex+1;
            charStack.push_back(deletedChar);

            while(!redo.empty()) { delete redo.top(); redo.pop(); }
        }
    }
    else if(state == 2){
        if(!current_piece){
            // FIX: If current_piece is NULL, reset state and use newDeletion
            state = 0;
            head = newDeletion(head, index);
            
            if(lastStepType==0){
                if(lastStepLength>0)undo.push(new laststep(undoType , lastStepLength , cursorStart , charStack));
                charStack.clear();
                undoType = subtraction;
                lastStepLength = 1;
                cursorStart = GlobalIndex+1;
                charStack.push_back(deletedChar);

                while(!redo.empty()) { delete redo.top(); redo.pop(); }
            }
            return;
        }
        
        if(!head){
            state = 0;
            return;
        }
        
        if(current_piece->length ==1){
            pieceNode* temp=NULL;
            int tempCount=delCount;
            delCount=0;
            predecessor(head,temp,currIndex);
            current_piece->length--;
            deletedChar=(current_piece->source==ADD)?addString[current_piece->start+current_piece->length]:originalString[current_piece->start+current_piece->length];
            GlobalIndex--;
            currIndex=GlobalIndex;
            delCount=tempCount;
            head=AVLDeletion(head,index-1);
            current_piece=temp;
            delCount=0;
            
            // FIX: If predecessor is NULL, reset state to 0
            if(!current_piece){
                state = 0;
            }
        }else{
            current_piece->length--;
            deletedChar=(current_piece->source==ADD)?addString[current_piece->start+current_piece->length]:originalString[current_piece->start+current_piece->length];
            GlobalIndex--;
            currIndex=GlobalIndex;
        }

        if(lastStepType==0){
            lastStepLength++;
            charStack.push_back(deletedChar);
        }
    }else{
        head = newDeletion(head,index);
        state=2;

        if(lastStepType==0){
            if(lastStepLength>0)undo.push(new laststep(undoType , lastStepLength , cursorStart , charStack));
            charStack.clear();
            undoType = subtraction;
            lastStepLength = 1;
            cursorStart = GlobalIndex+1;
            charStack.push_back(deletedChar);

            while(!redo.empty()) { delete redo.top(); redo.pop(); }
        }
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
        if(current_piece->length ==1){
            pieceNode* temp=NULL;
            predecessor(head,temp,currIndex);
            current_piece->length--;
            deletedChar=(current_piece->source==ADD)?addString[current_piece->start+current_piece->length]:originalString[current_piece->start+current_piece->length];
            GlobalIndex--;
            currIndex=GlobalIndex;
            node=AVLDeletion(node,index-1);
            current_piece=temp;
            delCount=0;
            
            // FIX: If predecessor is NULL, reset state to 0
            if(!current_piece){
                state = 0;
            }
        }else{
            current_piece->length--;
            deletedChar=(current_piece->source==ADD)?addString[current_piece->start+current_piece->length]:originalString[current_piece->start+current_piece->length];
            GlobalIndex--;
            currIndex=GlobalIndex;
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
        deletedChar=(current_piece->source==ADD)?addString[current_piece->start+current_piece->length]:originalString[current_piece->start+current_piece->length];
        GlobalIndex--;
        currIndex=GlobalIndex;
        if(current_piece->length ==0){
            pieceNode* temp=NULL;
            predecessor(head,temp,currIndex);
            node->left=AVLDeletion(node->left,index-1);
            current_piece=temp;
            delCount=0;
            
            // FIX: If predecessor is NULL, reset state to 0
            if(!current_piece){
                state = 0;
            }
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

pieceNode* PieceTable::AVLDeletion(pieceNode *node, int index, pieceNode* type){
    if (!node) return NULL; 
    if(node->length==0||type==node){
        if ((node->left == nullptr) || (node->right == nullptr)) {
            pieceNode *temp = node->left ? node->left : node->right;
            if (temp == nullptr) {
                temp = node;
                node = NULL;
                delete temp;
                return NULL;
            } else{
                pieceNode* keep = temp;
                delete node;
                recomputeWeights(keep);
                return keep;
            }
        } else {
            pieceNode* temp = minValueNode(node->right);
            node->source = temp->source;
            node->length = temp->length;
            node->start = temp->start;
            node->source = temp->source;
            node->right = AVLDeletion(node->right, index-node->weight-node->length,temp);
        }
    }   
    else if (index <= node->weight){
        node->left = AVLDeletion(node->left, index, type);
    }
    else if (index >= node->weight + node->length){
        node->right = AVLDeletion(node->right, index - node->weight - node->length, type);
    }
    else {
        if(node->length > 0){
            node->length--;
        }
        if(node->length == 0){
            return AVLDeletion(node, index, node);
        }
    }
    
    node = balanceFunction(node, index);
    if(node){  // FIX: Add NULL check before accessing
        node->weight = subChar(node->left);
        node->height = 1 + max(height(node->left), height(node->right));
    }
    return node;
}

pieceNode *PieceTable::balanceFunction(pieceNode *node, int index)
{
    if(!node){return NULL;}
    node->height = 1 + max(height(node->left), height(node->right));
    int balance = getBalance(node);

    if (balance > 1){
        if(getBalance(node->left) < 0){
            node->left = leftRotate(node->left);
        }
        pieceNode *toReturn = rightRotate(node);
        recomputeWeights(toReturn);
        return toReturn;
    }
    if (balance < -1){
        if(getBalance(node->right) > 0){
            node->right = rightRotate(node->right);
        }
        pieceNode *toReturn = leftRotate(node);
        recomputeWeights(toReturn);
        return toReturn;
    }

    node->weight = subChar(node->left);
    node->height = 1 + max(height(node->left), height(node->right));
    return node;
}

void PieceTable::undofn()
{
    if(lastStepLength>0) undo.push(new laststep(undoType  , lastStepLength , cursorStart , charStack));
    lastStepLength=0;
    if(undo.empty())return;
    laststep* latest = undo.top();
    
    if(latest->command == addition){
        int cursor2 = latest->lastStepLength + latest->cursorStart ;
        GlobalIndex = cursor2;

        for(int i=0; i<latest->lastStepLength ;i++){
            if(state!=2)delCount=0;
            delCount++;
            deletion(cursor2,1);
            cursor2--;
        }
        redo.push(latest);
        undo.pop();
    }
    else if(latest->command == subtraction){
        int cursor2 = latest->cursorStart - latest->lastStepLength;
        GlobalIndex = cursor2;

        for(int i=0 ; i< latest->lastStepLength ; i++){
            insert(latest->charStack[latest->lastStepLength - 1 - i] , cursor2 ,1);
            cursor2++;
        }
        redo.push(latest);
        undo.pop();
    }
    lastStepLength=0;
}

void PieceTable::redofn()
{
    if(redo.empty())return;
    laststep* latest = redo.top();
    
    if(latest->command == subtraction){
        int cursor2 = latest->cursorStart ; 
        GlobalIndex = cursor2 ;

        for(int i=0; i<latest->lastStepLength ;i++){
            if(state!=2)delCount=0;
            delCount++;
            deletion(cursor2,1);
            cursor2--;
        }
        undo.push(latest);
        redo.pop();
    }
    else if(latest->command == addition){
        int cursor2 = latest->cursorStart ;
        GlobalIndex = cursor2;

        for(int i=0 ; i< latest->lastStepLength ; i++){
            insert(latest->charStack[i], cursor2 ,1);
            cursor2++;
        }

        undo.push(latest);
        redo.pop();
    }
}