#include "completeBinaryTree.hpp"

CompleteBinaryTree::CompleteBinaryTree(): stump(nullptr), lastInserted(nullptr), maxKey(0) {}


kvNode* CompleteBinaryTree::getRoot() {return stump;}
kvNode* CompleteBinaryTree::getLast() {return lastInserted;}
int CompleteBinaryTree::size() {return maxKey;}
bool CompleteBinaryTree::isEmpty() { return stump == nullptr;}
void CompleteBinaryTree::_swapV(kvNode* n1, kvNode* n2){
    int tmp = n1->value;
    n1->value=n2->value;
    n2->value = tmp;
}
kvNode* CompleteBinaryTree::insert(int value){
    kvNode* nNode = new kvNode{maxKey, value, nullptr, nullptr, nullptr};
    if(!stump) stump = nNode; 
    else {
        kvNode* current= stump;
        Queue<kvNode*>* q = new Queue<kvNode*>{maxKey+2};
        q->queue(current);
        while(current){
            if (!current->cLeft) {
                current->cLeft = nNode;
                nNode->parent =current;
                break;
            } else if (!current->cRight) {
                current->cRight = nNode;
                nNode->parent =current;
                break;
            } else {
                q->queue(current->cLeft);
                q->queue(current->cRight);
                current = q->dequeue();
            }
        }
        delete q;
    };
    maxKey++;
    lastInserted = nNode;
    return nNode;
};
kvNode* CompleteBinaryTree::findLast(){
    Queue<kvNode*> q{maxKey+2};
    q.queue(stump);
    kvNode* current = nullptr;
    while(!q.isEmpty()){
        current = q.dequeue();
        if (current->cLeft) q.queue(current->cLeft);
        if (current->cRight) q.queue(current->cRight);
    }
    return current;
}
void CompleteBinaryTree::destroyTree(kvNode* node) {
    if (!node) return;
    destroyTree(node->cLeft);
    destroyTree(node->cRight);
    delete node;
}
kvNode* CompleteBinaryTree::removeLast(){
    if (isEmpty()) throw std::runtime_error("empty!");
    kvNode* output = lastInserted;
    if (lastInserted == stump) {
        stump=nullptr;
    } else if(lastInserted->parent->cRight==lastInserted){
        lastInserted->parent->cRight=nullptr;
        lastInserted=output->parent->cLeft;
    } else {
        lastInserted->parent->cLeft = nullptr;
        lastInserted= findLast();
    }
    maxKey--;
    return output;
}
CompleteBinaryTree::~CompleteBinaryTree(){
    destroyTree(stump);
}