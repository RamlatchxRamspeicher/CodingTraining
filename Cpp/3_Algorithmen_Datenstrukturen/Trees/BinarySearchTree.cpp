#include "BinarySearchTree.hpp"

template<typename V>
bool nearlyEqual(V a, V b) {
    return std::abs(a - b) < 1e-9;
}

template<typename K, typename V, typename node_t>
BinarySearchTree<K, V, node_t>::BinarySearchTree(): stump(nullptr) {}

template<typename K, typename V, typename node_t>
BinarySearchTree<K, V, node_t>::~BinarySearchTree() {clear(stump);}
        
/*
To add a node you must compare the values of the nodes,
then go left if the value is smaller or go right if the value is bigger than the current node
repeat until you hit a leaf node,
then insert 
*/
template<typename K, typename V, typename node_t>
void BinarySearchTree<K, V, node_t>::_add(V v) {
    node_t* newNode = new node_t{v,nullptr,nullptr,nullptr};
    if(!stump) {
        stump = newNode;
    }
    else {
        node_t* current = stump;
        while (true) {
            if (nearlyEqual(current->value, newNode->value)) {
                delete newNode;
                break;
            }
            else if (current->value > newNode->value){
                if(current->cLeft==nullptr){
                    newNode->parent=current;
                    current->cLeft=newNode;
                    break;
                } else current = current->cLeft;
            }
            else { 
                if(current->cRight == nullptr){
                    newNode->parent=current;
                    current->cRight=newNode;
                    break;
                } else current = current->cRight;
            }
        }
    }
}

//cleaner version without tracking
/* template<typename K, typename V, typename node_t>
void BinarySearchTree<node_t, K, V>::add(V v) { 
    node_t** current = &stump;
    node_t* parent = nullptr;
    
    while(*current){
        parent = *current;
        if (v < (*current)->value)      current = &(*current)->cLeft;
        else if (v > (*current)->value) current = &(*current)->cRight;
        else                            return;
    }
    
    *current= new node_t{v, parent, nullptr, nullptr};
} */
template<typename K, typename V, typename node_t>
void BinarySearchTree<K, V, node_t>::add(K key, V val) { 
    node_t** current = &stump;
    node_t* parent = nullptr;
    
    while(*current){
        parent = *current;
        if (key < (*current)->key)      current = &(*current)->cLeft;
        else if (key > (*current)->key) current = &(*current)->cRight;
        else                            return;
    }
    
    *current= new node_t{key, val, parent, nullptr, nullptr};
}

template<typename K, typename V, typename node_t>
node_t* BinarySearchTree<K, V, node_t>::findMin(node_t *node){
    while (node->cLeft)
    {
        node = node->cLeft;
    }
    return node;
}

template<typename K, typename V, typename node_t>
node_t* BinarySearchTree<K, V, node_t>::findMax(node_t *node){
    while (node->cRight)
    {
        node = node->cRight;
    }
    return node;
}

template<typename K, typename V, typename node_t>
/* node_t* BinarySearchTree<K, V, node_t>::find(node_t* current, V v) {
    if (!current)
        return nullptr;

    if (nearlyEqual(v, current->value)) return current;
    if (v < current->value)             return find(current->cLeft, v);
    else                                return find(current->cRight, v);
} */
node_t* BinarySearchTree<K, V, node_t>::find(node_t* current, K key) {
    if (!current)
        return nullptr;

    if (key == current->key)            return current;
    if (key < current->key)             return find(current->cLeft, key);
    else                                return find(current->cRight, key);
}

template<typename K, typename V, typename node_t>
/* void BinarySearchTree<K, V, node_t>::remove(V v) {
    node_t *node = find(stump, v);
    if(!node)return;

    if(!node->cLeft || !node->cRight) {
        node_t *child = node->cLeft ? node->cLeft : node->cRight;
        if (!node->parent)                                          stump = child;
        else if (node->parent->cLeft == node) node->parent->cLeft = child;
        else                                      node->parent->cRight= child;
        if(child) child->parent=node->parent;
        delete node;
    }
    else {
        node_t *successor = findMin(node->cRight);

        node->value = successor->value;

        if (successor->parent->cLeft == successor)  successor->parent->cLeft = successor->cRight;
        else                                            successor->parent->cRight= successor->cRight;

        if (successor->cRight) successor->cRight->parent = successor->parent;

        delete successor;
    }
} */
void BinarySearchTree<K, V, node_t>::remove(K key) {
    node_t *node = find(stump, key);
    if(!node)return;

    if(!node->cLeft || !node->cRight) {
        node_t *child = node->cLeft ? node->cLeft : node->cRight;
        if (!node->parent)                                          stump = child;
        else if (node->parent->cLeft == node) node->parent->cLeft = child;
        else                                      node->parent->cRight= child;
        if(child) child->parent=node->parent;
        delete node;
    }
    else {
        node_t *successor = findMin(node->cRight);

        node->key = successor->key;

        if (successor->parent->cLeft == successor)  successor->parent->cLeft = successor->cRight;
        else                                            successor->parent->cRight= successor->cRight;

        if (successor->cRight) successor->cRight->parent = successor->parent;

        delete successor;
    }
}

// left, root, right
template<typename K, typename V, typename node_t>
void BinarySearchTree<K, V, node_t>::inOrder(node_t* current) {
    if (!current)
        return;

    inOrder(current->cLeft);
    std::cout << current->value << " ";
    inOrder(current->cRight);
}

//meine buggy version
template<typename K, typename V, typename node_t>
void BinarySearchTree<K, V, node_t>::_inOrder(){
    if(!isEmpty()){
        node_t *node = stump;
        node_t *previous = nullptr;
        node_t *max = findMax(stump);
        while (true)
        {
            //break loop
            if (nearlyEqual(node->value,max->value)){
                std::cout<<node->value;
                break;
            }
            //go left and check next node
            if(node->cLeft && !(node->cLeft == previous)){
                node = node->cLeft;
                continue;
            } else {
                //push value since no child left -> currently lowest value
                std::cout<<node->value;
                //go right and check next node
                if(node->cRight && !(node->cRight == previous)){
                    node = node->cRight;
                    continue;
                } else { 
                    //go back if no children or all children traversed
                    previous = node;
                    node = node->parent; 
                }
            }
        }
    }
}

//root left right
template<typename K, typename V, typename node_t>
void BinarySearchTree<K, V, node_t>::preOrder(node_t* current) {
    if (!current)
        return;

    std::cout << current->value << " ";
    preOrder(current->cLeft);
    preOrder(current->cRight);
}

//left right root
template<typename K, typename V, typename node_t>
void BinarySearchTree<K, V, node_t>::postOrder(node_t* current) {
    if (!current)
        return;

    postOrder(current->cLeft);
    postOrder(current->cRight);
    std::cout << current->value << " ";
}
template<typename K, typename V, typename node_t>
bool BinarySearchTree<K, V, node_t>::isEmpty(){return stump==nullptr;}

template<typename K, typename V, typename node_t>
int BinarySearchTree<K, V, node_t>::height(node_t *node){
    if (!node) return 0;
    int leftHeight  = height(node->cLeft);
    int rightHeight = height(node->cRight);

    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}
template<typename K, typename V, typename node_t>
int BinarySearchTree<K, V, node_t>::size(node_t* node) {
    if (!node) return 0;
    return 1 + size(node->cLeft) + size(node->cRight);
}
template<typename K, typename V, typename node_t>
void BinarySearchTree<K, V, node_t>::clear(node_t *current){
    if (!current)
        return;

    clear(current->cLeft);
    clear(current->cRight);
    delete current;
}