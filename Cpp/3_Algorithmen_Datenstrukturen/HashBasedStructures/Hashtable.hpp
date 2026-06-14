#include "Buckets.hpp"
#include "Hashfunctions.hpp"


bool isPrime(std::size_t n) {
    if(n==2) return true;
    if(n<2 || n%2==0) return false;
    for(std::size_t i = 3; i*i <= n; i+=2){
        if(n % i == 0 ) return false;
    }
    return true;
}

std::size_t nextPrime(std::size_t n){
    if(isPrime(n)) return n;
    if(n%2==0) return nextPrime(n+1);
    else return nextPrime(n+2);
}

struct BaseStats {
    std::size_t tableSize;
    std::size_t count;
    float loadFactor;
    std::size_t emptyBuckets;
};
struct ChainStats : BaseStats {
    std::size_t longestChain;
    float avgChainLength;
    std::size_t collisions;
};
struct OAStats : BaseStats {
    std::size_t tombstones;
    std::size_t longestCluster;
};

static const std::size_t MIN_SIZE = 7;

template<typename K, typename V, template <typename,typename> typename BucketType>
class HashTable{
    protected:
        std::size_t tableSize;
        std::size_t count;
        BucketType<K,V>** table;
        std::function<std::size_t(const K&, std::size_t)> hashFn;
        virtual BaseStats stats() = 0;
    public:
        HashTable(std::size_t, std::function<std::size_t(const K&, std::size_t)> hashFn);
        HashTable(const HashTable& other);
        HashTable& operator=(const HashTable& other);
        virtual void insert(const K& key, const V& value) = 0;
        virtual V* get(const K& key) = 0;
        std::vector<std::pair<K,V>> getAll() const;
        virtual bool remove(const K& key) = 0;
        virtual ~HashTable();
        V& operator[](const K& key);
    protected:
        float loadFactor();
        virtual void rehash();
        BaseStats _baseStats()
};


template<typename K, typename V, template <typename,typename> class BucketType>
class HashTableChain: public HashTable<K,V,BucketType>{
    public:
        HashTableChain(std::size_t size, std::function<std::size_t(const K&, std::size_t)> hashFn);
        void insert(const K& key, const V& value) override;
        V* get(const K& key) override;
        bool remove(const K& key) override;
        ChainStats stats() override;
        ~HashTableChain() override;
        struct Iterator{
            BucketType<K,V>** table;
            std::size_t tableSize;
            std::size_t bucketIndex;
            listNode<K,V>* currentNode;
            std::pair<K,V> operator*(){return {currentNode->key, currentNode->value};}
            Iterator& operator++(){
                if(currentNode->next){currentNode=currentNode->next; return *this;}
                do {bucketIndex++;} while (bucketIndex<tableSize && table[bucketIndex]->isEmpty());
                currentNode = (bucketIndex < tableSize) ? table[bucketIndex]->getHead() : nullptr;
                return *this;
            }
            bool operator!=(const Iterator& other) {return table!=other.table || currentNode!=other.currentNode;}
        };
        Iterator begin() {
            std::size_t i = 0;
            while(i < this->tableSize && this->table[i]->isEmpty()) i++;
            listNode<K,V>* node = (i < this->tableSize) ? this->table[i]->getHead() : nullptr;
            return {this->table, this->tableSize, i, node};
        }
        Iterator end(){return{this->table,this->tableSize,this->tableSize,nullptr};}
        
};

enum ProbeStrategy { LINEAR, QUADRATIC, DOUBLE_HASH };
template<typename K, typename V, template <typename,typename> class BucketType>
class HashTableOA: public HashTable<K,V,BucketType>{
    ProbeStrategy _strategy;
    std::function<std::size_t(const K&, std::size_t)> hashFn2;
    protected:
        std::size_t linearProbe(std::size_t hash, int i, std::size_t tableSize);
        std::size_t quadraticProbe(std::size_t hash, int i, std::size_t tableSize);
        std::size_t doubleHashProbe(std::size_t hash, std::size_t hash2, int i, std::size_t tableSize);
        std::size_t probe(std::size_t h1, std::size_t h2, int i);
    public:
        HashTableOA(std::size_t size, std::function<std::size_t(const K&, std::size_t)> hashFn,std::function<std::size_t(const K&, std::size_t)> hashFn2);
        HashTableOA(std::size_t size, ProbeStrategy strategy, std::function<std::size_t(const K&, std::size_t)> hashFn);
        HashTableOA(const HashTableOA& other);
        HashTableOA& operator=(const HashTableOA& other);
        void insert(const K& key, const V& value) override;
        V* get(const K& key) override;
        bool remove(const K& key) override;
        OAStats stats() override;
        ~HashTableOA() override;
        struct Iterator{
            BucketType<K,V>** table;
            std::size_t tableSize;
            std::size_t bucketIndex;
            std::pair<K,V> operator*(){
                auto entries = table[bucketIndex]->getAll();
                return entries[0];
            }
            Iterator& operator++(){
                do {bucketIndex++;} while (bucketIndex<tableSize && table[bucketIndex]->getState()!=OCCUPIED);
                return *this;
            }
            bool operator!=(const Iterator& other) {return table!=other.table || bucketIndex!=other.bucketIndex;}
        };
        Iterator begin() {
            std::size_t i = 0;
            while(i < this->tableSize && this->table[i]->getState() != OCCUPIED) i++;
            return {this->table, this->tableSize, i};
        }
        Iterator end(){return{this->table,this->tableSize,this->tableSize};}
};
