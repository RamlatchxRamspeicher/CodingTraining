#include "listNode.hpp"



template <typename K, typename V, typename node_t>
class LinkedList {
    private:
        node_t *head;
        unsigned int _size, maxSize;
    public:
        LinkedList(unsigned int num);
        ~LinkedList();
        LinkedList(const LinkedList&) = delete;
        LinkedList& operator=(const LinkedList&) = delete;
        void push(K key, V val);
        void insert(K key, V val, int idx);
        node_t* operator[](int i);
        LinkedList* operator()(int i,int j);
        V pop();
        V* find(K key);
        V remove(K key);
        bool isEmpty();
        node_t* getHead()const;
        unsigned int getSize()const;
        void clear();
        struct Iterator{
            node_t* current;

            Iterator(node_t* node) : current(node) {}

            node_t* operator*() {return current;}
            Iterator& operator++() {current = current->next; return *this;}
            bool operator!=(const Iterator& other) { return current != other.current; }
        };
        Iterator begin() { return Iterator(head); }
        Iterator end()   { return Iterator(nullptr); }
        
};

template <typename K, typename V>
class DoublyLinkedList: public LinkedList<K, V, dllNode> {
private:
    dllNode<K,V> *head;
    dllNode<K,V> *tail;
    int _size;

public:
    DoublyLinkedList();

    ~DoublyLinkedList();

    //add Nodes
    void push_back(K k, V v);

    void push_front(K k, V v);

    //find Nodes
    dllNode<K,V>* find(K k);
    dllNode<K,V>* rfind(K k);

    //delete Node
    void remove(K k);

    //print list and print list reverse order
    void print() const;
    void rprint() const;

    //clear List
    void clear();

    int getSize() const;
};