#include "treeNodes.hpp"
#include <vector>
#include <cmath>
/* template<typename node_t, typename T>
class BinarySearchTree {
    protected:
        node_t *stump;

    public:
        BinarySearchTree();
        ~BinarySearchTree();
        void _add(T v);                         //
        void add(T v);                          //
        node_t* find(node_t *current, T v);     //
        void remove(T v);                       //
        node_t* findMin(node_t *node);          //
        node_t* findMax(node_t *node);          //
        void inOrder(node_t* current);          //
        void _inOrder();                        //
        void preOrder(node_t* current);         //
        void postOrder(node_t* current);        //
        int height(node_t *node);               //
        void clear(node_t *current);            //
        bool isEmpty();                         //
        int size(node_t* node);                 //
}; */

template< typename K, typename V, typename node_t = kvuNode<K,V>>
class BinarySearchTree {
    protected:
        node_t *stump;

    public:
        BinarySearchTree();
        ~BinarySearchTree();
        void _add(V v);                    //
        void add(K key, V v);                     //
        node_t* find(node_t *current, K key);//
        void remove(K key);                  //
        node_t* findMin(node_t *node);          //
        node_t* findMax(node_t *node);          //
        void inOrder(node_t* current);          //
        void _inOrder();                        //
        void preOrder(node_t* current);         //
        void postOrder(node_t* current);        //
        int height(node_t *node);               //
        void clear(node_t *current);            //
        bool isEmpty();                         //
        int size(node_t* node);                 //
};