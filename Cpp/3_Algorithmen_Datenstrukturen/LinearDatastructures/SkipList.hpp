#include "listNode.hpp"
#include <random>

template <typename K, typename V>
class SkipList{
    skipNode<K,V>* head;
    int maxLevel;
    int currentLevel;
    unsigned int _size;
    private:
        int _randLevel();
        void _updateLevel();
    public:
        SkipList(int maxLevel);
        SkipList(const SkipList& other) = delete;
        SkipList& operator=(const SkipList&) = delete;
        ~SkipList();
        void insert(K key, V val);
        V* operator[](const K& key);
        V* find(K key)const;
        V remove(K key);
        bool isEmpty() const;
        void clear();
        unsigned int getSize()const;
        skipNode<K,V>* getHead()const;
        struct Iterator {
            skipNode<K,V>* current;
            
            Iterator(skipNode<K,V>* node) : current(node) {}
            
            skipNode<K,V>* operator*()  { return current; }
            Iterator& operator++()      { current = current->next[0]; return *this; }
            bool operator!=(const Iterator& other) { return current != other.current; }
        };

        Iterator begin() { return Iterator(head->next[0]); } // ← erstes echtes Element
        Iterator end()   { return Iterator(nullptr); }
};