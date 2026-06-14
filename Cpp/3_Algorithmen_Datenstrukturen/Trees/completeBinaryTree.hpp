#include "treeNodes.hpp"
#include "..\LinearDatastructures\Queue.hpp"

class CompleteBinaryTree {
    kvNode* stump;
    kvNode* lastInserted;
    protected:
        int maxKey;
        void destroyTree(kvNode* node);
        kvNode* getRoot();
        kvNode* getLast();
        kvNode* findLast();
        void _swapV(kvNode* n1, kvNode* n2);
    public:
        CompleteBinaryTree();
        ~CompleteBinaryTree();
        int size();
        bool isEmpty();

        kvNode* insert(int value);
        kvNode* removeLast();
        
};