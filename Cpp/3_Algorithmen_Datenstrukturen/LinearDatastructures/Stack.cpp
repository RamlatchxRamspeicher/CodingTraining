#include "Stack.hpp"

template<typename T>
Stack<T>::Stack(int max): size(0), maxsize(max), items(nullptr){}
template<typename T>
Stack<T>::~Stack() {
    while (items) {
        stackNode<T>* prev = items->previous;
        delete items;
        items = prev;
    }
}
template<typename T>
bool Stack<T>::empty()const{return items==nullptr;}


template<typename T>
void Stack<T>::push(T val){
    if(size==maxsize) throw ("Stackoverflow!");
    items = new stackNode<T>{val,items};
    size++;
}
template<typename T>
T Stack<T>::peek() const{
    if (empty()) throw("Stack empty!");
    return items->value;
}

template<typename T>
T Stack<T>::pop(){
    if(empty()) throw("Stackunderflow!");
    T value = items->value;
    stackNode<T>* previous = items->previous;
    delete items;
    items = previous;
    size--;
    return value;
}
template<typename T>
int Stack<T>::getSize()const{return size;}

template<typename T>
void Stack<T>::duplicate(){
    push(peek());
}

template<typename T>
void Stack<T>::rotate(int n, bool right){
    if(n>size || n<=0) throw ("invalid n given");
    stackNode<T>* tmp = items;
    stackNode<T>* newTop = nullptr;
    if(n==2){
        newTop = items->previous;
        items->previous = newTop->previous;
        items = newTop;
    }
    else{
        if(right){
            // given stack (items=)"5"->4->3->2->1->0 & n=4
            newTop=items->previous;           //save 5->4 connection newTop = 4
            for(int i = 0; i<n-1; i++){tmp = tmp->previous;} //tmp=2 tmp->previous=1
            items->previous=tmp->previous;  // "5"->1->0  `4`->3->'2'->1->0
            tmp->previous=items;            // `4`->3->'2'->"5"->1->0
            items=newTop;                      // "4"->3->2->5->1->0
        } else {
            // given stack (items=)"5"->4->3->2->1->0 & n=4
            for(int i = 0; i<n-2; i++){tmp = tmp->previous;} //tmp=3 tmp->previous=2
            newTop = tmp->previous;         // "5"->4->'3'->`2`->1->0
            tmp->previous=newTop->previous; // "5"->4->'3'->1->0 `2`->1->0
            newTop->previous=items;            // `2`->"5"->4->'3'->1->0
            items=newTop;                      // "2"->5->4->3->1->0
        }
    }
}

template<typename T>
void Stack<T>::rotateRight(int n){
    rotate(n);
}

template<typename T>
void Stack<T>::rotateLeft(int n){
    rotate(n, false);
}

template<typename T>
void Stack<T>::swapTop(){
    rotate(2);
}
