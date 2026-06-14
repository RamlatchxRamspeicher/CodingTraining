#include "Queue.hpp"


template <typename T>
Queue<T>::Queue(unsigned int length): len(0), maxlen(length), first(nullptr), last(nullptr){if (length <= 0) throw std::invalid_argument("length must be > 0");}

template <typename T>
void Queue<T>::queue(T val){
    if(len==maxlen) throw std::runtime_error("queue full, dequeue first!");
    queueNode* newNode = new queueNode{val,nullptr}; 
    if(len==0){
        first = newNode;
        last = newNode;
    } else {
        last->next = newNode;
        last = newNode;
    }
    len++;
}

template <typename T>
T Queue<T>::dequeue(){
    if(len==0) throw std::runtime_error("empty queue");
    T value = first->value;
    if(len==1) {delete first; first = last = nullptr;}
    else {
        queueNode* tmp = first->next;
        delete first;
        first= tmp;
    }
    len--;
    return value;
}

template <typename T>
Queue<T>::~Queue(){
    while (first) {
        queueNode* tmp = first->next;
        delete first;
        first = tmp;
    }
}
template <typename T>
PriorityQueue<T>::PriorityQueue(unsigned int length): len(0), maxlen(length), first(nullptr), last(nullptr){if (length <= 0) throw std::invalid_argument("length must be > 0");}
template <typename T>
PriorityQueue<T>::~PriorityQueue(){
    while (first) {
        pqNode* tmp = first->next;
        delete first;
        first = tmp;
    }
}

template <typename T>
void PriorityQueue<T>::queue(T val, Priority p){
    if(len==maxlen) throw std::runtime_error("queue full, dequeue first!");
    pqNode* newNode = new pqNode{val,p}; 
    if(!first || p > first->priority){
        newNode->next = first;
        first = newNode;
        if (len == 0) last = newNode;
    } else {
        pqNode* current = first;
        while(current->next && current->next->priority >= p)
        {   current= current->next;}
        newNode->next = current->next;
        current->next = newNode;
        if (!newNode->next) last = newNode;
        }
    len++;
}

template <typename T>
T PriorityQueue<T>::dequeue(){
    if(len==0) throw std::runtime_error("empty queue");
    T value = first->value;
    if(len==1) {delete first; first = last = nullptr;}
    else {
        pqNode* tmp = first->next;
        delete first;
        first= tmp;
    }
    len--;
    return value;
}

template <typename T>
bool Queue<T>::isEmpty(){return first==nullptr;}
template <typename T>
bool PriorityQueue<T>::isEmpty(){return first==nullptr;}