#include "SkipList.hpp"

template <typename K, typename V>
SkipList<K,V>::SkipList(int maxLevel): maxLevel(maxLevel), currentLevel(0), _size(0){
    head = new skipNode<K,V>(K{}, V{}, maxLevel);
}
template <typename K, typename V>
int SkipList<K,V>::_randLevel(){
    int rL=0;
    while(rand()%2 == 0 && rL < maxLevel) rL++;
    return rL;
}
template <typename K, typename V>
V* SkipList<K,V>::operator[](const K& key){return find(key);}
template <typename K, typename V>
bool SkipList<K,V>::isEmpty()const{return head->next[0]==nullptr;}
template <typename K, typename V>
unsigned int SkipList<K,V>::getSize()const{return _size;}
template <typename K, typename V>
skipNode<K,V>* SkipList<K,V>::getHead()const{return head;}
template <typename K, typename V>
SkipList<K,V>::~SkipList(){clear(); delete head;}

template <typename K, typename V>
void SkipList<K,V>::clear(){
    skipNode<K,V>* current = head->next[0];
    while(current){
        skipNode<K,V>* tmp = current->next[0];
        delete current;
        current = tmp;
    }
    for(int i = 0; i < maxLevel; i++)
        head->next[i] = nullptr;
    currentLevel = 0;
    _size = 0;
}

template <typename K, typename V>
void SkipList<K,V>::insert(K key, V value){
    std::vector<skipNode<K,V>*> update(maxLevel, head);
    skipNode<K,V>* current = head;

    for(int i = currentLevel; i >= 0; i--){
        while(current->next[i] && current->next[i]->key < key)
            current = current->next[i];
        update[i] = current;
    }

    if(update[0]->next[0] && update[0]->next[0]->key == key){
        update[0]->next[0]->value = value;
        return;
    }

    int randLvl = _randLevel();
    if(randLvl > currentLevel){
        for(int i = currentLevel+1; i <= randLvl; i++)
            update[i] = head;
        currentLevel = randLvl;
    }

    skipNode<K,V>* newNode = new skipNode<K,V>{key, value, randLvl+1};
    for(int i = 0; i <= randLvl; i++){
        newNode->next[i] = update[i]->next[i];
        update[i]->next[i] = newNode;
    }
    _size++;
}
template <typename K, typename V>
V* SkipList<K,V>::find(K key)const{
    skipNode<K,V>* current = head;
    for(int lvl = currentLevel; lvl >= 0; lvl--){
        while(current->next[lvl] && current->next[lvl]->key < key)
            current = current->next[lvl];
    }
    if(current->next[0] && current->next[0]->key == key){ return &current->next[0]->value;}
    return nullptr;
};
template <typename K, typename V>
void SkipList<K,V>::_updateLevel(){
    for(int lvl=maxLevel; lvl>=0; lvl--){
        if(head->next[lvl]){currentLevel=lvl; return;}
    }
}
template <typename K, typename V>
V SkipList<K,V>::remove(K key){
    std::vector<skipNode<K,V>*> update(maxLevel, head);
    skipNode<K,V>* current = head;
    for(int lvl = currentLevel; lvl >= 0; lvl--){
        while(current->next[lvl] && current->next[lvl]->key < key)
            current = current->next[lvl];
        update[lvl] = current;
    }

    if(update[0]->next[0] && update[0]->next[0]->key == key){
        skipNode<K,V>* target = update[0]->next[0];
        for(int lvl = 0; lvl<=currentLevel; lvl++){
            if(update[lvl]->next[lvl]!=target) break;
            update[lvl]->next[lvl]=target->next[lvl];
        }
        V val = target->value;
        delete current;
        _updateLevel();
        _size--;
        return val;
    } else {throw std::runtime_error("key not in list");}
};
