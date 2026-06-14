#include "LinkedList.hpp"


template <typename K, typename V, typename node_t>
LinkedList<K, V, node_t>::LinkedList(unsigned int num) : head(nullptr), _size(0), maxSize(num) {}
template <typename K, typename V, typename node_t>
LinkedList<K, V, node_t>::~LinkedList() {clear();}
template <typename K, typename V, typename node_t>
LinkedList<K, V, node_t>::LinkedList(const LinkedList<K, V, node_t>&) = delete;
template <typename K, typename V, typename node_t>
LinkedList<K, V, node_t>& LinkedList<K, V, node_t>::operator=(const LinkedList<K, V, node_t>&) = delete;
template <typename K, typename V, typename node_t>
void LinkedList<K, V, node_t>::push(K key, V val){ insert(key, val, _size);}
template <typename K, typename V, typename node_t>
void LinkedList<K, V, node_t>::insert(K key, V val, int idx){
    for(auto* node : *this)
        if(node->key == key) { node->value = val; return; }
    if (idx<0) idx += _size;
    if(idx < 0 || idx > _size) throw std::runtime_error("out of bounds");
    if(maxSize > 0 && _size==maxSize) throw std::runtime_error("List full!");
    node_t *newNode = new node_t{key,val,nullptr};
    if(idx==0){
        newNode->next = head;
        head = newNode;
    } else {
        
        node_t* current = head;
        for(int j = 0; j<idx-1; ++j){
            current = current->next; 
        }
        node_t* next = current->next;
        current->next = newNode;
        newNode->next = next;
    }
    _size++;
}
template <typename K, typename V, typename node_t>
node_t* LinkedList<K, V, node_t>::operator[](int i){
    if (i<0) i += _size;
    if(i < 0 || i >= static_cast<int>(_size)) throw std::runtime_error("out of bounds");
    node_t* current = head;
    for(int j = 0; j<i; ++j){
        current = current->next; 
    }
    return current;
}
template <typename K, typename V, typename node_t>
LinkedList<K, V, node_t>* LinkedList<K, V, node_t>::operator()(int i,int j){
    bool out_of_bounds;
    unsigned int _i, _j;
    if(i<0) _i=i+_size;
    if(j<0) _j=j+_size;
    if(_i < 0 || _i >= static_cast<int>(_size) || _j < 0 || _j >= static_cast<int>(_size)) throw std::runtime_error("out of bounds");
    unsigned int size;
    bool reverse = (i>0 && i<j || i>j && !(i<=0 && j<0));
    if (i<= j){                             // eg [0,1,2,3,4,5,6,7] _size:8
        if      (i==j)          size=0;     // i:1  , j:1  ->            0 -> 1: [1]         i==j f |sanity check| i to i+size: 1 to  1+ 0: 1 yes
        if else (0>i && 0<=j)   size=_i-j;  // i:-1 , j:3  ->  7 -  3  = 4 -> 5: [7,6,5,4,3] i<j  r |sanity check| i to i-size:-1 to -1- 4:-5 yes
        else                    size=j-i;   // i:1  , j:3  ->  3 -  1  = 2 -> 3: [1,2,3]     i<j  f |sanity check| i to i+size: 1 to  1+ 2: 3 yes
                                            // i:-3 , j:-1 -> -1 -(-3) = 2 -> 3; [5,6,7]     i<j  f |sanity check| i to i+size:-3 to -3+ 2:-1 yes
    } else if   (0<=i && 0>j)   size=_j-i;  // i:1  , j:-3 ->  5 -  1  = 4 -> 5; [1,2,3,4,5] i>j  f |sanity check| i to i+size: 1 to  1+ 4: 5 yes
    else                        size=i-j;   // i:3  , j:1  ->  3 -  1  = 2 -> 3: [3,2,1]     i>j  r |sanity check| i to i-size: 3 to  3- 2: 1 yes
                                            // i:-1 , j:-3 -> -1 -(-3) = 2 -> 3; [7,6,5]     i>j  r |sanity check| i to i-size:-1 to -1- 2:-3 yes
    size++;
    LinkedList<K, V, node_t>* result = new LinkedList<K, V, node_t>{size};
    for(int k = 0; k < size; k++){
        int _idx = (reverse) ? i-k : i+k;
        node_t* _tmp = (*this)[_idx];
        result->push(_tmp->key,_tmp->value);
    }
    return result;
}

template <typename K, typename V, typename node_t>
V LinkedList<K, V, node_t>::pop() {
    if(_size == 0)    throw std::runtime_error("List empty");
    if(_size == 1){
        V val = head->value;
        delete head;
        head = nullptr;
        _size = 0;
        return val;
    }
    node_t* pre = (*this)[-2];
    node_t* last = pre->next;
    V val = last->value;
    pre->next = nullptr;
    delete last;
    --_size;
    return val;
}
template <typename K, typename V, typename node_t>
void LinkedList<K, V, node_t>::clear(){
    node_t* current= head;
    while(current){
        node_t* tmp = current->next;
        delete current;
        current = tmp;
    }
    head = nullptr;
    _size = 0;
}
template <typename K, typename V, typename node_t>
bool LinkedList<K, V, node_t>::isEmpty(){return head==nullptr;}
template <typename K, typename V, typename node_t>
node_t* LinkedList<K, V, node_t>::getHead()const{return head;}
template <typename K, typename V, typename node_t>
unsigned int LinkedList<K, V, node_t>::getSize()const{return _size;}

template <typename K, typename V, typename node_t>
V* LinkedList<K, V, node_t>::find(K key){
    if(isEmpty()) return nullptr;
    node_t* current = head;
    while(current && current->key!=key){current=current->next;}
    if(!current) return nullptr;
    return &current->value;
}
template <typename K, typename V, typename node_t>
V LinkedList<K, V, node_t>::remove(K key){
    if(isEmpty()) throw std::runtime_error("list empty");
    node_t* current = head;
    node_t* prev = nullptr;
    
    while(current && current->key != key) {
        prev = current;
        current = current->next;
    }
    if(!current) throw std::runtime_error("no such item");
    
    V value = current->value;
    
    if(!prev) head = current->next; 
    else prev->next = current->next;
    
    delete current;
    _size--;
    return value;
}



template <typename K, typename V>
DoublyLinkedList<K,V>::DoublyLinkedList() : head(nullptr), tail(nullptr), _size(0) {}

template <typename K, typename V>
DoublyLinkedList<K,V>::~DoublyLinkedList() {clear();}

//add Nodes
template <typename K, typename V>
void DoublyLinkedList<K,V>::push_back(K k, V v) {
    dllNode<K,V> *newNode = new dllNode<K,V>{k,v,nullptr,tail};

    if(tail)    tail->next=newNode;
    else        head = newNode;
    
    tail=newNode;
    _size++;
}

template <typename K, typename V>
void DoublyLinkedList<K,V>::push_front(K k, V v) {
    dllNode<K,V> *newNode = new dllNode<K,V>{k,v,head,nullptr};

    if(head)    head->previous = newNode;
    else        tail = newNode;
    
    head=newNode;
    _size++:
}

//find Nodes
template <typename K, typename V>
dllNode<K,V>* DoublyLinkedList<K,V>::find(K k) {
    dllNode<K,V> *current = head;
    while (current) {
        if (current->key == k)  return current;
        else                    current = current->next;
    }
    return nullptr;
}
template <typename K, typename V>
dllNode<K,V>* DoublyLinkedList<K,V>::rfind(K k) {
    dllNode<K,V> *current = tail;
    while (current) {
        if (current->key == k)  return current;
        else                    current = current->previous;
    }
    return nullptr;
}

//delete Node
template <typename K, typename V>
void DoublyLinkedList<K,V>::remove(K k) {
    dllNode<K,V>* node = find(k);

    if(!node)       throw std::runtime_error("didn't find key");

    if(node->previous)  node->previous->next = node->next;
    else            head = node->next;

    if(node->next)  node->next->previous = node->previous;
    else            tail = node->previous;

    delete node;
    _size--:
}

//print list and print list reverse order
template <typename K, typename V>
void DoublyLinkedList<K,V>::print() const {
    dllNode<K,V> *current = head;
    std::cout << "[";
    while (current) {
        std::cout << "{" << current->key << ": " << current->value << "}";
        if (current->next) std::cout << " <-> ";
        current = current->next;
    }
    std::cout << "]\n";
}
template <typename K, typename V>
void DoublyLinkedList<K,V>::rprint() const {
    dllNode<K,V> *current = tail;
    std::cout << "[";
    while (current) {
        std::cout << "{" << current->key << ": " << current->value << "}";
        if (current->previous) std::cout << " <-> ";
        current = current->previous;
    }
    std::cout << "]\n";
}

//clear List
template <typename K, typename V>
void DoublyLinkedList<K,V>::clear() {
    dllNode<K,V> *current = head;
    while (current)
    {
        dllNode<K,V> *next = current->next;
        delete current;
        current = next;
    }
    head = tail = nullptr;
    _size = 0;
}

template <typename K, typename V>
int DoublyLinkedList<K,V>::getSize() const {
    return _size;
}