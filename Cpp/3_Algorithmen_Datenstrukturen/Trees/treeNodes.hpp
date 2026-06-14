#include "includes.hpp"
struct kvNode{
    int key;
    int value;
    kvNode* parent;
    kvNode* cLeft;
    kvNode* cRight;
};
//key value universal type Node
template<typename K,typename V>
struct kvuNode{
    K key;
    V value;
    kvuNode* parent;
    kvuNode* cLeft;
    kvuNode* cRight;
};
template<typename node_t, typename T>
struct binarySearchTreeNode{
    T value;
    node_t *parent;
    node_t *cLeft;
    node_t *cRight;
};
template<typename K, typename V>
struct AVLNode:kvuNode<K,V>{
    int balanceFactor;
    int height;
};

