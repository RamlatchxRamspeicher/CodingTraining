#include "AVLTree.hpp"

template< typename T>
bool nearlyEqual(T a, T b) {
    return std::abs(a - b) < 1e-9;
}


template<typename node_t, typename T>
void AVLTree<node_t, T>::updateBF(node_t *current){
    if (!current) return;
    int leftHeight  = current->childLeft  ? current->childLeft->height  : 0;
    int rightHeight = current->childRight ? current->childRight->height : 0;
    current->height        = 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
    current->balanceFactor = rightHeight - leftHeight;
    if (current->balanceFactor > 1 || current->balanceFactor < -1)
        rebalance(current);
}

template<typename node_t, typename T>
node_t* AVLTree<node_t, T>::_rotate(node_t* root, Rotation direction){
    node_t *rChild = (direction == LEFT) ? root->childRight : root->childLeft;
    node_t *swap = nullptr;
    
    swap = (direction == LEFT) ? rChild->childLeft : rChild->childRight;
    rChild->parent = root->parent;
    if(swap) swap->parent = root;
    root->parent = rChild;
    if(direction == LEFT){
        rChild->childLeft = root;
        root->childRight = swap;
    }else{
        rChild->childRight = root;
        root->childLeft = swap;
    }
    return rChild;
}
template<typename node_t, typename T>
void AVLTree<node_t, T>::rebalance(node_t *root){
    //save position of root in parent
    node_t *parentRoot = root->parent;
    node_t* newRoot = nullptr;
    if(root->balanceFactor < -1){
        if(root->childLeft->balanceFactor <= 0){//Left Left -> rotate Right
            newRoot = _rotate(root, RIGHT);
            int bF = newRoot->balanceFactor;
            newRoot->balanceFactor             = (bF == 0) ?  1 : 0;
            newRoot->childRight->balanceFactor = (bF == 0) ? -1 : 0;
        } else {//Left Right -> T rotate LeftRight
            root->childLeft = _rotate(root->childLeft, LEFT);
            root->childLeft->parent = root;
            newRoot = _rotate(root, RIGHT);
            if (newRoot->balanceFactor == 0) {
                newRoot->childLeft->balanceFactor   = 0;
                newRoot->childRight->balanceFactor  = 0;
            } else if (newRoot->balanceFactor > 0) {
                newRoot->childLeft->balanceFactor   = 0;
                newRoot->childRight->balanceFactor  =+1;
            } else {
                newRoot->childLeft->balanceFactor   =-1;
                newRoot->childRight->balanceFactor  = 0;
            }
        }
    } else if (root->balanceFactor > 1){        
        if(root->childRight->balanceFactor >= 0){//Right Right -> rotate Left
            newRoot = _rotate(root,LEFT);
            int bF = newRoot->balanceFactor;
            newRoot->balanceFactor             = (bF == 0) ? -1 : 0;
            newRoot->childLeft->balanceFactor  = (bF == 0) ?  1 : 0;
        } else {//Right Left -> T rotate RightLeft
            root->childRight = _rotate(root->childRight, RIGHT);
            root->childRight->parent = root;
            newRoot = _rotate(root, LEFT);
            if (newRoot->balanceFactor == 0) {
                newRoot->childRight->balanceFactor = 0;
                newRoot->childLeft->balanceFactor  = 0;
            } else if (newRoot->balanceFactor > 0) {
                newRoot->childRight->balanceFactor = 0;
                newRoot->childLeft->balanceFactor  =+1;
            } else {
                newRoot->childRight->balanceFactor =-1;
                newRoot->childLeft->balanceFactor  = 0;
            }
        }
    }
    if (parentRoot) {
    if (parentRoot->childLeft == root)
        parentRoot->childLeft = newRoot;
    else
        parentRoot->childRight = newRoot;
    }
}

template<typename node_t, typename T>
void AVLTree<node_t, T>::add(T v){
    node_t *current = _add(v);
    current = current->parent;
    while (current !=nullptr)
    {
        updateBF(current);
        if (current->balanceFactor == 0) break;
        current = current->parent;
    }
}
template<typename node_t, typename T>
void AVLTree<node_t, T>::remove(T v) {
    node_t *node = find(stump, v);
    if(!node)return;
    
    node_t* rebalanceStart = _remove(node);
    while (rebalanceStart != nullptr) {
        updateBF(rebalanceStart);
        if (current->balanceFactor == 0) break;
        rebalanceStart = rebalanceStart->parent;
    }
}
template<typename node_t, typename T>
node_t* AVLTree<node_t, T>::_add(T v){
    node_t** current = &stump;
    node_t* parent = nullptr;
    
    while(*current){
        parent = *current;
        if (v < (*current)->value)      current = &(*current)->childLeft;
        else if (v > (*current)->value) current = &(*current)->childRight;
        else                            return;
    }
    
    *current= new node_t{parent, nullptr, nullptr, v, 0, 0};
    return *current;
}
template<typename node_t, typename T>
node_t* AVLTree<node_t, T>::_remove(node_t* node){
    if(!node->childLeft || !node->childRight) {
        node_t *child = node->childLeft ? node->childLeft : node->childRight;
        node_t* parent = node->parent;
        if (!node->parent)                                          stump = child;
        else if (node->parent->childLeft == node) node->parent->childLeft = child;
        else                                      node->parent->childRight= child;
        if(child) child->parent=node->parent;
        delete node;
        return parent;
    }
    else {
        node_t *successor = findMin(node->childRight);
        node_t* successorParent = successor->parent;
        node->value = successor->value;
        
        if (successor->parent->childLeft == successor)  successor->parent->childLeft = successor->childRight;
        else                                            successor->parent->childRight= successor->childRight;
        
        if (successor->childRight) successor->childRight->parent = successor->parent;
        
        delete successor;
        return successorParent;
    }
}

template<typename node_t, typename T>
void AVLTree<node_t, T>::collectInOrder(node_t* root, std::set<T>& set){
    if (!root)
        return;

    collectInOrder(root->childLeft, set);
    set.insert(root->value);
    collectInOrder(root->childRight, set);
}

template<typename node_t, typename T>
std::set<T> AVLTree<node_t, T>::toSet(node_t* root) const{
    std::set<T> result;
    collectInOrder(root,result);
    return result;
}
template<typename node_t, typename T>
node_t* AVLTree<node_t, T>::fromSet(std::set<T> set){
    node_t* root = nullptr;
    for(T val : set){
        root = add(val);
    }
    return root;
};

template<typename node_t, typename T>
AVLTree<node_t, T> AVLTree<node_t, T>::getUnion(AVLTree* other) const {
    std::set<T> setSelf = toSet(stump);
    std::set<T> setOther = toSet(other->stump);
    std::set<T> unionTree;
    AVLTree *newTree = new AVLTree;
    std::set_union(setSelf.begin(), setSelf.end(), setOther.begin(), setOther.end(), unionTree.begin());
    newTree->stump = fromSet(unionTree);
    return newTree;
};
template<typename node_t, typename T>
AVLTree<node_t, T> AVLTree<node_t, T>::getIntersection(AVLTree* other) const{
    std::set<T> setSelf = toSet(stump);
    std::set<T> setOther = toSet(other->stump);
    std::set<T> intersection;
    AVLTree *newTree = new AVLTree;
    std::set_intersection(setSelf.begin(), setSelf.end(), setOther.begin(), setOther.end(), intersection.begin());
    newTree->stump = fromSet(intersection);
    return newTree;
};
template<typename node_t, typename T>
AVLTree<node_t, T> AVLTree<node_t, T>::getDifference(AVLTree* other) const{
    std::set<T> setSelf = toSet(stump);
    std::set<T> setOther = toSet(other->stump);
    std::set<T> difference;
    AVLTree *newTree = new AVLTree;
    std::set_difference(setSelf.begin(), setSelf.end(), setOther.begin(), setOther.end(), difference.begin());
    newTree->stump = fromSet(difference);
    return newTree;
};

template<typename K, typename V>
V* AVLTreeKV<K,V>::operator[](const K& key){
    return find(stump, key);
}
template<typename K, typename V>
void AVLTreeKV<K,V>::updateBF(stdNode* current){
        if (!current) return;
        int leftHeight  = current->childLeft  ? current->childLeft->height  : 0;
        int rightHeight = current->childRight ? current->childRight->height : 0;
        current->height        = 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
        current->balanceFactor = rightHeight - leftHeight;
        if (current->balanceFactor > 1 || current->balanceFactor < -1)
            rebalance(current);
    }

template<typename K, typename V>
kvuNode<K,V>* AVLTreeKV<K,V>::_add(K key, V val){
    stdNode** current = &stump;
    stdNode* parent = nullptr;
    
    while(*current){
        parent = *current;
        if (key < (*current)->key)      current = &(*current)->childLeft;
        else if (key > (*current)->key) current = &(*current)->childRight;
        else                            return nullptr;
    }
    
    *current= new stdNode{key, val, parent, nullptr, nullptr, 0, 1};
    return *current;
}
template<typename K, typename V>
kvuNode<K,V>* AVLTreeKV<K,V>::_remove(stdNode* node){
    if(!node->cLeft || !node->cRight) {
        stdNode *child = node->cLeft ? node->cLeft : node->cRight;
        stdNode* parent = node->parent;
        if (!node->parent)                                          stump = child;
        else if (node->parent->cLeft == node) node->parent->cLeft = child;
        else                                      node->parent->cRight= child;
        if(child) child->parent=node->parent;
        delete node;
        return parent;
    }
    else {
        stdNode *successor = findMin(node->cRight);
        stdNode* successorParent = successor->parent;
        node->value = successor->value;
        node->key = successor->key;
        
        if (successor->parent->cLeft == successor)  successor->parent->cLeft = successor->cRight;
        else                                        successor->parent->cRight= successor->cRight;
        
        if (successor->cRight) successor->cRight->parent = successor->parent;
        
        delete successor;
        return successorParent;
    }
}
template<typename K, typename V>
kvuNode<K,V>* AVLTreeKV<K,V>::_rotate(stdNode* root, Rotation direction){
    stdNode *rChild = (direction == LEFT) ? root->cRight : root->cLeft;
    stdNode *swap = nullptr;
    
    swap = (direction == LEFT) ? rChild->cLeft : rChild->cRight;
    rChild->parent = root->parent;
    if(swap) swap->parent = root;
    root->parent = rChild;
    if(direction == LEFT){
        rChild->cLeft = root;
        root->cRight = swap;
    }else{
        rChild->cRight = root;
        root->cLeft = swap;
    }
    return rChild;
}
template<typename K, typename V>
void AVLTreeKV<K,V>::rebalance(stdNode* root){
    //save position of root in parent
    stdNode *parentRoot = root->parent;
    stdNode* newRoot = nullptr;
    if(root->balanceFactor < -1){
        if(root->cLeft->balanceFactor <= 0){//Left Left -> rotate Right
            newRoot = _rotate(root, RIGHT);
            int bF = newRoot->balanceFactor;
            newRoot->balanceFactor             = (bF == 0) ?  1 : 0;
            newRoot->cRight->balanceFactor = (bF == 0) ? -1 : 0;
        } else {//Left Right -> T rotate LeftRight
            root->cLeft = _rotate(root->cLeft, LEFT);
            root->cLeft->parent = root;
            newRoot = _rotate(root, RIGHT);
            if (newRoot->balanceFactor == 0) {
                newRoot->cLeft->balanceFactor   = 0;
                newRoot->cRight->balanceFactor  = 0;
            } else if (newRoot->balanceFactor > 0) {
                newRoot->cLeft->balanceFactor   = 0;
                newRoot->cRight->balanceFactor  =+1;
            } else {
                newRoot->cLeft->balanceFactor   =-1;
                newRoot->cRight->balanceFactor  = 0;
            }
        }
    } else if (root->balanceFactor > 1){        
        if(root->cRight->balanceFactor >= 0){//Right Right -> rotate Left
            newRoot = _rotate(root,LEFT);
            int bF = newRoot->balanceFactor;
            newRoot->balanceFactor             = (bF == 0) ? -1 : 0;
            newRoot->cLeft->balanceFactor  = (bF == 0) ?  1 : 0;
        } else {//Right Left -> T rotate RightLeft
            root->cRight = _rotate(root->cRight, RIGHT);
            root->cRight->parent = root;
            newRoot = _rotate(root, LEFT);
            if (newRoot->balanceFactor == 0) {
                newRoot->cRight->balanceFactor = 0;
                newRoot->cLeft->balanceFactor  = 0;
            } else if (newRoot->balanceFactor > 0) {
                newRoot->cRight->balanceFactor = 0;
                newRoot->cLeft->balanceFactor  =+1;
            } else {
                newRoot->cRight->balanceFactor =-1;
                newRoot->cLeft->balanceFactor  = 0;
            }
        }
    }
    if (parentRoot) {
    if (parentRoot->cLeft == root)
        parentRoot->cLeft = newRoot;
    else
        parentRoot->cRight = newRoot;
    } else {
        stump = newRoot;
    }
}
template<typename K, typename V>
void AVLTreeKV<K,V>::collectInOrder(stdNode* root, std::vector<std::pair<K,V>>& vec){
    if (!root)
        return;

    collectInOrder(root->cLeft, vec);
    vec.push_back({root->key,root->value});
    collectInOrder(root->cRight, vec);
}
template<typename K, typename V>
void AVLTreeKV<K,V>::add(K key, V val){
    stdNode *current = _add(key, val);
    if(!current) throw std::runtime_error("key exists");
    current = current->parent;
    while (current !=nullptr)
    {
        updateBF(current);
        if (current->balanceFactor == 0) break;
        current = current->parent;
    }
}
template<typename K, typename V>
void AVLTreeKV<K,V>::remove(K key){
    stdNode *node = find(stump, key);
    if(!node)return;
    
    stdNode* rebalanceStart = _remove(node);
    while (rebalanceStart != nullptr) {
        updateBF(rebalanceStart);
        if (rebalanceStart->balanceFactor == 0) break;
        rebalanceStart = rebalanceStart->parent;
    }
}
template<typename K, typename V>
std::vector<std::pair<K,V>> AVLTreeKV<K,V>::toVector() const{
    std::vector<std::pair<K,V>> result;
    collectInOrder(stump, result);
    return result;
}
template<typename K, typename V>
void AVLTreeKV<K,V>::clear(){
    auto deconstruct = [](auto& self, stdNode* node) -> void {
        if(node == nullptr) return;
        self(self, node->cLeft);
        self(self, node->cRight);
        delete node;
    };
    deconstruct(deconstruct, stump);
    stump = nullptr;
}