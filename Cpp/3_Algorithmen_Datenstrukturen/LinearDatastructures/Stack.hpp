#include "listNode.hpp"

template<typename T>
class Stack{
    int size;
    int maxsize;
    stackNode<T>* items;
    void rotate(int n, bool right = true);
    public:
        Stack(int max);
        ~Stack();
        void push(T val);
        T pop();
        void duplicate();
        void rotateLeft(int n);
        void rotateRight(int n);
        void swapTop();
        T peek() const;
        bool empty() const;
        int getSize() const;
};
