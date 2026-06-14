#include "HashTable.hpp"


template<typename K, typename V, template <typename,typename> typename BucketType>
float HashTable<K,V,BucketType>::loadFactor() { return (float)count / tableSize; }
template<typename K, typename V, template <typename,typename> typename BucketType>
void HashTable<K,V,BucketType>::rehash(){
    float loadF = loadFactor();
    std::size_t newSize;
    if(loadF<0.8f && loadF>0.2f || loadF<0.2f && tableSize<=MIN_SIZE) return;
    if(loadF>=0.8f) newSize = nextPrime(tableSize*2);
    else            newSize = nextPrime(tableSize/2);
    BucketType<K,V>** newTable = new BucketType<K,V>*[newSize];
    for(std::size_t i = 0; i < newSize; i++)
        newTable[i] = new BucketType<K,V>();

    std::vector<std::pair<K,V>> oldEntries;
    for(std::size_t i = 0; i < tableSize; i++){
        if(table[i]){
            for(auto& p: table[i]->getAll())
                oldEntries.push_back(p);
        }
    }

    for(std::size_t i = 0; i < tableSize; i++)
        delete table[i];
    delete[] table;

    table = newTable;
    tableSize = newSize;
    count = 0;
    for(auto&[k,v] : oldEntries){ insert(k,v);}
}
template<typename K, typename V, template <typename,typename> typename BucketType>
HashTable<K,V,BucketType>::HashTable(std::size_t size, std::function<std::size_t(const K&, std::size_t)> hashFn)
    : tableSize(nextPrime(size)), count(0), hashFn(hashFn) {
    table = new BucketType<K,V>*[tableSize];
    for(std::size_t i = 0; i < tableSize; i++)
        table[i] = new BucketType<K,V>();
}

template<typename K, typename V, template <typename,typename> typename BucketType>
HashTable<K,V,BucketType>::HashTable(const HashTable<K,V,BucketType>& other)
    : tableSize(other.tableSize), count(other.count), hashFn(other.hashFn) {
    table = new BucketType<K,V>*[tableSize];
    for(std::size_t i = 0; i < tableSize; i++){
        table[i] = new BucketType<K,V>(*other.table[i]);
    }
}

template<typename K, typename V, template <typename,typename> typename BucketType>
HashTable<K,V,BucketType>& HashTable<K,V,BucketType>::operator=(const HashTable<K,V,BucketType>& other){
    if(this==&other) return *this;
    for(std::size_t i = 0; i < tableSize; i++){
        delete table[i];
    }
    delete[] table;
    tableSize=other.tableSize;
    count=other.count;
    hashFn=other.hashFn;
    table = new BucketType<K,V>*[tableSize];
    for(std::size_t i = 0; i < tableSize; i++){
        table[i] = new BucketType<K,V>(*other.table[i]);
    }
    return *this;
}
template<typename K, typename V, template <typename,typename> typename BucketType>
V& HashTable<K,V,BucketType>::operator[](const K& key){
    V* val = get(key);
    if(!val){
        insert(key,V{});
        return *get(key);
    }
    return *val;
}
template<typename K, typename V, template <typename,typename> typename BucketType>
std::vector<std::pair<K,V>> HashTable<K,V,BucketType>::getAll()const{
    std::vector<std::pair<K,V>> result;
    for(std::size_t i = 0; i < tableSize; i++)
        for(auto& p : this->table[i]->getAll())
            result.push_back(p);
    return result;
}
template<typename K, typename V, template <typename,typename> typename BucketType>
BaseStats HashTable<K,V,BucketType>::_baseStats(){
    std::size_t empty = 0;
    for(std::size_t i = 0; i < tableSize; i++)
        if(table[i]->isEmpty()) empty++;
    return{tableSize, count, loadFactor(), empty};
}
template<typename K, typename V, template <typename,typename> typename BucketType>
HashTable<K,V,BucketType>::~HashTable() {
    for(std::size_t i = 0; i < tableSize; i++)
        delete table[i];
    delete[] table;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename K, typename V, template <typename,typename> class BucketType>
void HashTableChain<K,V,BucketType>::insert(const K& key, const V& value){
    std::size_t hash = this->hashFn(key,this->tableSize);
    this->table[hash]->insert(key,value);
    this->count++;
    this->rehash();
}
template<typename K, typename V, template <typename,typename> class BucketType>
V* HashTableChain<K,V,BucketType>::get(const K& key){
    return this->table[this->hashFn(key,this->tableSize)]->get(key);
}
template<typename K, typename V, template <typename,typename> class BucketType>
bool HashTableChain<K,V,BucketType>::remove(const K& key){
    if(!this->table[this->hashFn(key,this->tableSize)]->remove(key)) return false;
    this->count--;
    return true;
}
template<typename K, typename V, template <typename,typename> class BucketType>
HashTableChain<K,V,BucketType>::HashTableChain(std::size_t size, std::function<std::size_t(const K&, std::size_t)> hashFn):HashTable<K,V,BucketType>(size, hashFn){}
template<typename K, typename V, template <typename,typename> typename BucketType>
ChainStats HashTableChain<K,V,BucketType>::stats(){
    std::size_t longestChain = 0; std::size_t collisions = 0; std::size_t filled = 0;
    for(std::size_t i = 0; i<this->tableSize; i++){
        unsigned int currentSize = table[i]->getAll().size();
        if(currentSize>0) filled++;
        if(currentSize>1) collisions++;
        if(currentSize>longestChain) longestChain=currentSize;
    }
    if(filled==0)filled=1; //avoid div by 0
    return{BaseStats{_baseStats()},longestChain,(float)this->count/filled};
}
//////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename K, typename V, template <typename,typename> class BucketType>
HashTableOA<K,V,BucketType>::HashTableOA(std::size_t size, ProbeStrategy strat, std::function<std::size_t(const K&, std::size_t)> hashFn):HashTable<K,V,BucketType>(size,hashFn), _strategy(strat), hashFn2(hashFn){}
template<typename K, typename V, template <typename,typename> class BucketType>
HashTableOA<K,V,BucketType>::HashTableOA(std::size_t size, std::function<std::size_t(const K&, std::size_t)> hashFn, std::function<std::size_t(const K&, std::size_t)> hashFn2):HashTable<K,V,BucketType>(size,hashFn), _strategy(DOUBLE_HASH), hashFn2(hashFn2){}
template<typename K, typename V, template <typename,typename> typename BucketType>
HashTableOA<K,V,BucketType>::HashTableOA(const HashTableOA<K,V,BucketType>& other): HashTable<K,V,BucketType>(other), hashFn2(other.hashFn2), _strategy(other._strategy) {}
template<typename K, typename V, template <typename,typename> typename BucketType>
HashTableOA<K,V,BucketType>& HashTableOA<K,V,BucketType>::operator=(const HashTableOA<K,V,BucketType>& other){
    if(this==&other) return *this;
    for(std::size_t i = 0; i < this->tableSize; i++){
        delete this->table[i];
    }
    delete[] this->table;
    this->tableSize=other.tableSize;
    this->count=other.count;
    this->hashFn=other.hashFn;
    this->hashFn2=other.hashFn2;
    this->_strategy=other._strategy;
    this->table = new BucketType<K,V>*[this->tableSize];
    for(std::size_t i = 0; i < this->tableSize; i++){
        this->table[i] = new BucketType<K,V>(*other.table[i]);
    }
    return *this;
}
template<typename K, typename V, template <typename,typename> class BucketType>
std::size_t HashTableOA<K,V,BucketType>::probe(std::size_t h1, std::size_t h2, int i){
    std::size_t hash;
    switch (_strategy){
    case DOUBLE_HASH:   hash = doubleHashProbe(h1,h2,i,this->tableSize);break;
    case LINEAR:        hash = linearProbe(h1,i,this->tableSize);break;
    case QUADRATIC:     hash = quadraticProbe(h1,i,this->tableSize);break;
    }
    return hash;
}
template<typename K, typename V, template <typename,typename> class BucketType>
void HashTableOA<K,V,BucketType>::insert(const K& key, const V& value){
    std::size_t hash1   = this->hashFn(key, this->tableSize);
    std::size_t hash2   = hashFn2(key, this->tableSize);
    std::size_t hash;
    int firstTombstone = -1;
    for (int i = 0; i<this->tableSize; i++){
        hash = probe(hash1, hash2, i);
        State bState=this->table[hash]->getState();

        if (bState==EMPTY){
            std::size_t insertAt = (firstTombstone != -1) ? firstTombstone : hash;
            this->table[insertAt]->insert(key,value);
            this->count++;
            this->rehash();
            return;
        }

        if(bState==TOMBSTONE && firstTombstone ==-1) firstTombstone = hash;

        if(bState== OCCUPIED && this->table[hash]->get(key)!=nullptr){
            this->table[hash]->insert(key, value); // update
            return;
        }
    }

    if(firstTombstone != -1){
        this->table[firstTombstone]->insert(key, value);
        this->count++;
        this->rehash();
    } else throw std::runtime_error("table full");
}
template<typename K, typename V, template <typename,typename> class BucketType>
V* HashTableOA<K,V,BucketType>::get(const K& key){
    std::size_t hash1   = this->hashFn(key, this->tableSize);
    std::size_t hash2   = hashFn2(key, this->tableSize);
    std::size_t hash;
    for (int i = 0; i<this->tableSize; i++){
        hash = probe(hash1, hash2, i);
        State bState=this->table[hash]->getState();
        if(bState==TOMBSTONE) continue;
        if(bState==OCCUPIED) return this->table[hash]->get(key);
        else if (bState==EMPTY) throw std::runtime_error("no such key");
    }
    return nullptr;
}
template<typename K, typename V, template <typename,typename> class BucketType>
bool HashTableOA<K,V,BucketType>::remove(const K& key){
    std::size_t hash1   = this->hashFn(key, this->tableSize);
    std::size_t hash2   = hashFn2(key, this->tableSize);
    std::size_t hash;
    for (int i = 0; i<this->tableSize; i++){
        hash = probe(hash1, hash2, i);
        State bState=this->table[hash]->getState();
        if(bState==TOMBSTONE) continue;
        if(bState==OCCUPIED){
            if(this->table[hash]->get(key) != nullptr)
                return this->table[hash]->remove(key);
        } 
        else if (bState==EMPTY) return false;
    }
    return false;
}

template<typename K, typename V, template <typename,typename> class BucketType>
OAStats HashTableOA<K,V,BucketType>::stats(){
    std::size_t longestCluster = 0; std::size_t tombstones = 0; std::size_t currentCluster = 0;
    for(std::size_t i = 0; i<this->tableSize; i++){
        State currentState = table[i]->getState();
        if(currentState==TOMBSTONE) tombstones++;
        if(currentState == OCCUPIED){
            currentCluster++;
            if(currentCluster > longestCluster) longestCluster = currentCluster;
        } else {
            currentCluster = 0;
        }
    }
    OAStats result={BaseStats{_baseStats()},tombstones,longestCluster};
    result.emptyBuckets = this->tableSize - this->count - tombstones;
    return result;
}

template<typename K, typename V, template <typename,typename> class BucketType>
std::size_t HashTableOA<K,V,BucketType>::linearProbe(std::size_t hash, int i, std::size_t tableSize){
    return (hash + i) % tableSize;
}
template<typename K, typename V, template <typename,typename> class BucketType>
std::size_t HashTableOA<K,V,BucketType>::quadraticProbe(std::size_t hash, int i, std::size_t tableSize){
    return (hash + i*i) % tableSize;
}
template<typename K, typename V, template <typename,typename> class BucketType>
std::size_t HashTableOA<K,V,BucketType>::doubleHashProbe(std::size_t hash, std::size_t hash2, int i, std::size_t tableSize){
    hash2 = (hash2==0) ? 1 : hash2;
    return (hash + i * hash2) % tableSize;
}