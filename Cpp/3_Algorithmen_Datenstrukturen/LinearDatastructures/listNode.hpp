#include "includes.hpp"
template<typename T>
struct baseNode{
    T value;
    baseNode(T v): value(v){} = default;
    operator=(const T &val){this->value = val; }
    operator+=(const T &val){this->value += val;}
    operator-=(const T &val){this->value -= val;}
    operator*=(const T &val){this->value *= val;}
    operator/=(const T &val){this->value /= val;}
};
template<typename T, typename P>
struct priorityNode: baseNode<T>{
    P priority;
    priorityNode(T val, P prio): baseNode<T>(val), priority(prio){}
};
template<typename K, typename V>
struct keyValNode: baseNode<V>{
    K key;
    keyValNode(K key, V val): baseNode<V>(val), key(key){}
};

template<typename T, typename node_t>
struct _listBase: baseNode<T>{
    node_t *next;
    _listBase(T val): baseNode<T>(val), next(nullptr){}
};
template<typename K, typename V, typename node_t>
struct _listBaseKV: _listBase<V, node_t>{
    K key;
    _listBaseKV(K key, V val): _listBase<V, node_t>(val){}
};
template<typename T>
struct stackNode: baseNode<T>{
    stackNode *previous;
    stackNode(T val): baseNode<T>(val), previous(nullptr){}
};
template<typename T>
struct queueNode: _listBase<T,queueNode>{};

template<typename T, typename P>
struct pqNode: _listBase<T,pqNode>{
    P priority;
    pqNode(T val, P prio): baseNode<T>(val), priority(prio){}
};


template<typename K, typename V>
struct listNode: _listBaseKV<K, V, listNode<K,V>>{
    listNode(K key, V val): _listBaseKV<K, V, listNode<K,V>>(key, val){}
};

template<typename K, typename V>
struct dllNode: _listBaseKV<K, V, dllNode<K, V>>{
    dllNode *previous;
    dllNode(K key, V val): _listBaseKV<K, V, dllNode<K, V>>(key, val), previous(nullptr){}
};
#include <vector>
template<typename K, typename V>
struct skipNode: keyValNode<K,V>{
    std::vector<skipNode*> next;
    skipNode(K k, V v, int levels) : keyValNode<K,V>(k,v), next(levels, nullptr) {}
};
// isnt it possible to do node_t *next;?