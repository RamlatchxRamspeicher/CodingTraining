#include "completeBinaryTree.hpp"

class MinHeap: public CompleteBinaryTree{
    void bubbleUp(kvNode* node);
    void heapifyDown(kvNode* node);
    public:
        void insert(int val);
        int extractMin();
};

class MaxHeap: public CompleteBinaryTree{
    void bubbleUp(kvNode* node);
    void heapifyDown(kvNode* node);
    public:
        void insert(int val);
        int extractMax();
};

class MinMaxHeap: public CompleteBinaryTree{
    int _getLevel(int key);
    kvNode* _getSmallestDescendant(kvNode* node);
    kvNode* _getLargestDescendant(kvNode* node);
    bool isMinLevel(kvNode* node);
    void bubbleUp(kvNode* node, bool isMin);
    void heapifyDown(kvNode* node, bool isMin);
    public:
        int extractMin();
        int extractMax();

};