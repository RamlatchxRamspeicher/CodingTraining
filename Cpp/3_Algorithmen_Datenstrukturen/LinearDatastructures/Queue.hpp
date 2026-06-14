#include "listNode.hpp"

template <typename T>
class Queue{
    unsigned int len, maxlen;
    queueNode* first, *last;
    public:
        Queue(unsigned int length);
        ~Queue();
        void queue(T val);
        T dequeue();
        bool isEmpty();
};
enum Priority{
    HIGH=3,
    MEDIUM=2,
    LOW=1
};
template <typename T>
class PriorityQueue{
    unsigned int len, maxlen;
    pqNode* first, *last;
    public:
        PriorityQueue(unsigned int length);
        ~PriorityQueue();
        void queue(T val, Priority p);
        bool isEmpty();
        T dequeue();
};
