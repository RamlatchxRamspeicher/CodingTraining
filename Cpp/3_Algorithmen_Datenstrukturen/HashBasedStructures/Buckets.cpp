#include "Buckets.hpp"

template<typename K, typename V>
State SlotBucket<K,V>::getState()const{return state;}
template<typename K, typename V>
void SlotBucket<K,V>::insert(K k, V v){
    if(state==OCCUPIED){
        if(k==key) value = v;
        else throw std::runtime_error("occupied");
    } else {key = k; value = v; state = OCCUPIED;}
}
template<typename K, typename V>
V* SlotBucket<K,V>::get(K k)const{
    if(state==OCCUPIED&&k==key) return &value;
    return nullptr;
}
template<typename K, typename V>
bool SlotBucket<K,V>::remove(K k){
    if(state==OCCUPIED&&k==key){
        state=TOMBSTONE; return true;
    } else return false;
}
template<typename K, typename V>
bool SlotBucket<K,V>::isEmpty()const{return (state!=OCCUPIED);}
template<typename K, typename V>
std::vector<std::pair<K,V>> SlotBucket<K,V>::getAll()const{
    if(!isEmpty()){
        return {{this->key, this->value}};
    } else return {};
}
template<typename K, typename V>
SlotBucket<K,V>::SlotBucket():state(EMPTY){}
template<typename K, typename V>
SlotBucket<K,V>::SlotBucket(const SlotBucket<K,V>& other): state(other.state), key(other.key), value(other.value) {}
template<typename K, typename V>
SlotBucket<K,V>& SlotBucket<K,V>::operator=(const SlotBucket<K,V>& other){
    if(this == &other) return *this;
    state = other.state;
    key   = other.key;
    value = other.value;
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////

template<typename K, typename V>
TreeBucket<K,V>::TreeBucket():items(){}
template<typename K, typename V>
TreeBucket<K,V>::TreeBucket(const TreeBucket& other):items(){
    for(auto& [k,v] : other.getAll())
        this->insert(k, v);
}
template<typename K, typename V>
TreeBucket<K,V>& TreeBucket<K,V>::operator=(const TreeBucket<K,V>& other){
    if(this==&other) return *this; //selbstzuweisung -> siehe n"achste zeile
    items.clear();
    for(auto& [k,v] : other.getAll())
        this->insert(k, v);
    return *this;
}
template<typename K, typename V>
void TreeBucket<K,V>::insert(const K& key, const V& value){items.add(key,value);}
template<typename K, typename V>
V* TreeBucket<K,V>::get(const K& key) const {auto* node = items.find(key);if(!node)return nullptr; return &node->value;}
template<typename K, typename V>
bool TreeBucket<K,V>::remove(const K& key){
    try { items.remove(key); return true; }
    catch(...) { return false; }
}
template<typename K, typename V>
bool TreeBucket<K,V>::isEmpty() const {return items.isEmpty();}
template<typename K, typename V>
TreeBucket<K,V>::~TreeBucket(){}
template<typename K, typename V>
std::vector<std::pair<K,V>> TreeBucket<K,V>::getAll() const {return items.toVector();}

/////////////////////////////////////////////////////////////////////////////////

template<typename K, typename V>
ChainBucket<K,V>::ChainBucket():items(0){}
template<typename K, typename V>
ChainBucket<K,V>::ChainBucket(const ChainBucket<K,V>& other):items(0){
    for(auto& [k,v] : other.getAll())
        this->insert(k, v);
}
template<typename K, typename V>
void ChainBucket<K,V>::insert(const K& key, const V& value){items.push(key,value);}
template<typename K, typename V>
V* ChainBucket<K,V>::get(const K& key)const{return items.find(key);}
template<typename K, typename V>
bool ChainBucket<K,V>::remove(const K& key){
    try { items.remove(key); return true; }
    catch(...) { return false; }
}
template<typename K, typename V>
bool ChainBucket<K,V>::isEmpty()const{return items.isEmpty();}
template<typename K, typename V>
ChainBucket<K,V>::~ChainBucket(){}
template<typename K, typename V>
std::vector<std::pair<K,V>> ChainBucket<K,V>::getAll()const{
    std::vector<std::pair<K,V>> result;
    for(auto* node : items)
        result.push_back({node->key, node->value});
    return result;
}
template<typename K, typename V>
ChainBucket<K,V>& ChainBucket<K,V>::operator=(const ChainBucket<K,V>& other){
    if(this==&other) return *this; //selbstzuweisung -> siehe n"achste zeile
    items.clear();
    for(auto& [k,v] : other.getAll())
        this->insert(k, v);
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////

template<typename K, typename V>
void SortedBucket<K,V>::insert(K key, V val){
    listNode<K,V>* previous = nullptr;
    unsigned int iSize = this->items.getSize();
    if(iSize==0) {this->items.push(key,val); return;}
    for(std::size_t idx = 0; idx<iSize; idx++){
        if (items[idx]->key>=key) {
            items.insert(key, val, idx);
            break;
        }        
    }
    this->items.push(key, val);
}

template<typename K, typename V>
V* SortedBucket<K,V>::get(K key)const{
    for(auto* node: this->items){
        if(node->key==key) return &node->value;
        if(node->key > key) return nullptr;
    }
    return nullptr; 
}

template<typename K, typename V>
bool SortedBucket<K,V>::remove(K key){
    for(auto* node : this->items){
        if(node->key == key) return (bool)this->items.remove(key);
        if(node->key > key)  break;
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////////////

template<typename K, typename V>
SkipListBucket<K,V>::SkipListBucket(int maxLvl):items(maxLvl){}
template<typename K, typename V>
SkipListBucket<K,V>::SkipListBucket(const SkipListBucket& other):items(16){
    for(auto& [k,v] : other.getAll())
        this->insert(k, v);
}
template<typename K, typename V>
SkipListBucket<K,V>& SkipListBucket<K,V>::operator=(const SkipListBucket<K,V>& other){
    if(this==&other) return *this; //selbstzuweisung -> siehe n"achste zeile
    items.clear();
    for(auto& [k,v] : other.getAll())
        this->insert(k, v);
    return *this;
}
template<typename K, typename V>
void SkipListBucket<K,V>::insert(const K& key, const V& value){items.insert(key,value);}
template<typename K, typename V>
V* SkipListBucket<K,V>::get(const K& key)const{return items.find(key);}
template<typename K, typename V>
bool SkipListBucket<K,V>::remove(const K& key){
    try { items.remove(key); return true; }
    catch(...) { return false; }
}
template<typename K, typename V>
bool SkipListBucket<K,V>::isEmpty()const{return items.isEmpty();}
template<typename K, typename V>
std::vector<std::pair<K,V>> SkipListBucket<K,V>::getAll()const{
    std::vector<std::pair<K,V>> result;
    for(auto* node : items)
        result.push_back({node->key, node->value});
    return result;
}