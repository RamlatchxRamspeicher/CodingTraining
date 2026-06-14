#include "BinarySearchTree.hpp"
#include <set>

enum Rotation {
    LEFT,
    RIGHT
};

template<typename node_t, typename T>
class AVLTree: BinarySearchTree<node_t, T> {
    protected:
        node_t *stump;
        void updateBF(node_t* node);
        node_t* _add(T v);
        node_t* _remove(node_t* node);
        node_t* _rotate(node_t* node, Rotation direction);
        void rebalance(node_t* root);
        void collectInOrder(node_t* root, std::set<T>& set);
        node_t* fromSet(std::set<T> set);
    public:
        void add(T v);
        void remove(T v);
        std::set<T> toSet(node_t* root) const;
        AVLTree<node_t, T> getUnion(AVLTree* other) const;
        AVLTree<node_t, T> getIntersection(AVLTree* other) const;
        AVLTree<node_t, T> getDifference(AVLTree* other) const;
};

template<typename K, typename V>
class AVLTreeKV: public BinarySearchTree<K,V> {
    using stdNode = kvuNode<K,V>;
    protected:
        stdNode *stump;
        void updateBF(stdNode* node);
        stdNode* _add(K key, V val);
        stdNode* _remove(stdNode* node);
        stdNode* _rotate(stdNode* node, Rotation direction);
        void rebalance(stdNode* root);
        void collectInOrder(stdNode* root, std::vector<std::pair<K,V>>& vec);
    public:
        void add(K key, V val);
        void remove(K key);
        void clear();
        std::vector<std::pair<K,V>> toVector() const;
        V* operator[](const K& key);
};