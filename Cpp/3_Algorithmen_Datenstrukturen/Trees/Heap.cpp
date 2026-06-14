#include "Heap.hpp"
#include <cmath>


void MinHeap::insert(int value){
    kvNode* inserted = CompleteBinaryTree::insert(value);
    bubbleUp(inserted);
}
void MaxHeap::insert(int value){
    kvNode* inserted = CompleteBinaryTree::insert(value);
    bubbleUp(inserted);
}
void MinHeap::bubbleUp(kvNode* node){
    while(true){
        if(!node->parent) break;
        if(node->value>=node->parent->value) break;
        _swapV(node,node->parent);
        node=node->parent;
    }
}
void MaxHeap::bubbleUp(kvNode* node){
    while(true){
        if(!node->parent) break;
        if(node->value<=node->parent->value) break;
        _swapV(node,node->parent);
        node=node->parent;
    }
}
int MinHeap::extractMin(){
    if(isEmpty()) throw std::runtime_error("empty!");
    _swapV(getRoot(),getLast());
    kvNode* min = removeLast();
    heapifyDown(getRoot());
    return min->value;
}
int MaxHeap::extractMax(){
    if(isEmpty()) throw std::runtime_error("empty!");
    _swapV(getRoot(),getLast());
    kvNode* max = removeLast();
    heapifyDown(getRoot());
    return max->value;
}
void MinHeap::heapifyDown(kvNode* node){
    if(!node->cLeft) return;

    kvNode* smaller = node->cLeft;
    if(node->cRight &&
       node->cRight->value < node->cLeft->value)

       smaller = node->cRight;

    if(smaller->value < node->value){
        _swapV(node, smaller);
        heapifyDown(smaller);
    };
}
void MaxHeap::heapifyDown(kvNode* node){
    if(!node->cLeft) return;

    kvNode* larger = node->cLeft;
    if(node->cRight &&
       node->cRight->value > node->cLeft->value)

       larger = node->cRight;

    if(larger->value > node->value){
        _swapV(node, larger);
        heapifyDown(larger);
    };
}

int MinMaxHeap::_getLevel(int key){return (int)std::log2(key+1);}
bool MinMaxHeap::isMinLevel(kvNode* node){return _getLevel(node->key)%2==0;}
void MinMaxHeap::bubbleUp(kvNode* node, bool isMin){
    if(!node->parent) return;
    if(!node->parent->parent) return;
    if(isMin){
        if(node->value > node->parent->value){
            _swapV(node,node->parent);
            bubbleUp(node->parent, !isMin);
        } else if (node->value < node->parent->parent->value) {
            _swapV(node,node->parent->parent);
            bubbleUp(node->parent->parent, isMin);
        } 
    } else {
        if(node->value < node->parent->value){
            _swapV(node,node->parent);
            bubbleUp(node->parent, !isMin);
        } else if (node->value > node->parent->parent->value) {
            _swapV(node,node->parent->parent);
            bubbleUp(node->parent->parent, isMin);
        } 
    }
    return;
}
int MinMaxHeap::extractMin(){
    if(isEmpty()) throw std::runtime_error("empty!");
    _swapV(getRoot(),getLast());
    kvNode* min = removeLast();
    heapifyDown(getRoot(), true);
    return min->value;
}
int MinMaxHeap::extractMax(){
    if(isEmpty()) throw std::runtime_error("empty!");
    kvNode* root = getRoot();
    if(!root->cLeft || !root->cRight) return removeLast()->value;
    kvNode* childMax = (root->cLeft->value > root->cRight->value) ? root->cLeft : root->cRight;
    _swapV(childMax,getLast());
    kvNode* max = removeLast();
    heapifyDown(childMax, false);
    return max->value;
}

kvNode* MinMaxHeap::_getLargestDescendant(kvNode* node){
    kvNode* largest = nullptr;
    if(node->cLeft)largest = node->cLeft;
    if(node->cRight && (!largest || node->cRight->value > largest->value)) largest=node->cRight;
    auto check = [&](kvNode* child){
        if(!child) return;
        if(child->cLeft  && child->cLeft->value  > largest->value) largest = child->cLeft;
        if(child->cRight && child->cRight->value > largest->value) largest = child->cRight;
    };
    check(node->cLeft);
    check(node->cRight);
    return largest;
}
kvNode* MinMaxHeap::_getSmallestDescendant(kvNode* node){
    kvNode* smallest = nullptr;
    if(node->cLeft)smallest = node->cLeft;
    if(node->cRight && (!smallest || node->cRight->value < smallest->value)) smallest=node->cRight;
    auto check = [&](kvNode* child){
        if(!child) return;
        if(child->cLeft  && child->cLeft->value  < smallest->value) smallest = child->cLeft;
        if(child->cRight && child->cRight->value < smallest->value) smallest = child->cRight;
    };
    check(node->cLeft);
    check(node->cRight);
    return smallest;
}

void MinMaxHeap::heapifyDown(kvNode* node, bool isMin){
    if(!node->cLeft) return;
    
    kvNode* candidate = isMin ? _getSmallestDescendant(node) 
                            : _getLargestDescendant(node);
    if(!candidate) return;
    bool isGrandchild = (candidate->parent !=node);
    if(isMin){
        if (candidate->value > node->value) return;
        _swapV(node,candidate);
        if (isGrandchild){
            if(candidate->value > candidate->parent->value)
                _swapV(candidate,candidate->parent);
            heapifyDown(candidate, isMin);
        }
    } else {
        if (candidate->value < node->value) return;
        _swapV(node,candidate);
        if (isGrandchild){
            if(candidate->value < candidate->parent->value)
                _swapV(candidate,candidate->parent);
            heapifyDown(candidate, isMin);
        }
    }
}