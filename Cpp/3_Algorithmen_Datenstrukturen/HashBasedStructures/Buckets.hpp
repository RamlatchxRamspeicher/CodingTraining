#include "../LinearDatastructures/LinkedList.hpp"
#include "../LinearDatastructures/SkipList.hpp"
#include "../Trees/AVLTree.hpp"



template<typename K, typename V>
class BucketBase{
    public:
        virtual void insert(const K& key, const V& value) = 0;
        virtual V* get(const K& key) const = 0;
        virtual bool remove(const K& key) = 0;
        virtual bool isEmpty() const = 0;
        virtual std::vector<std::pair<K,V>> getAll() const = 0;
        virtual ~BucketBase() = default;
        };

template<typename K, typename V>
class ChainBucket: public  BucketBase<K,V>{
    LinkedList<K, V, listNode<K, V>> items;
    public:
        ChainBucket();
        ChainBucket(const ChainBucket& other);
        void insert(const K& key, const V& value) override;
        V* get(const K& key)const override;
        bool remove(const K& key) override;
        bool isEmpty()const override;
        std::vector<std::pair<K,V>> getAll()const override;
        ~ChainBucket();
        ChainBucket& operator=(const ChainBucket& other);
};



template<typename K, typename V>
class TreeBucket: public  BucketBase<K,V>{
    AVLTreeKV<K, V> items;
    public:
        TreeBucket();
        TreeBucket(const TreeBucket& other);
        void insert(const K& key, const V& value) override;
        V* get(const K& key)const override;
        bool remove(const K& key) override;
        bool isEmpty()const override;
        std::vector<std::pair<K,V>> getAll()const;
        ~TreeBucket();
        TreeBucket& operator=(const TreeBucket& other);
};



enum State{EMPTY,OCCUPIED,TOMBSTONE};
template<typename K, typename V>
class SlotBucket: public  BucketBase<K,V>{
    State state;
    K key;
    V value;
    public:
        SlotBucket();
        SlotBucket(const SlotBucket& other);
        State getState()const;
        void insert(K key, V val) override;
        V* get(K key)const override;
        bool remove(K key) override;
        bool isEmpty()const override;
        std::vector<std::pair<K,V>> getAll()const override;
        SlotBucket& operator=(const SlotBucket& other);
};

template<typename K, typename V>
class SortedBucket: public ChainBucket<K,V>{
    static_assert(std::is_arithmetic<K>::value || std::is_same<K, std::string>::value, "SortedBucket requires comparable keys");
    public:
        void insert(K key, V val) override;
        V* get(K key)const override;
        bool remove(K key) override;
};

template<typename K, typename V>
class SkipListBucket: public  BucketBase<K,V>{
    SkipList<K,V> items;
    public:
        SkipListBucket(int maxLevel = 16);
        SkipListBucket(const SkipListBucket& other);
        SkipListBucket& operator=(const SkipListBucket& other);
        void insert(const K& key, const V& value) override;
        V* get(const K& key)const override;
        bool remove(const K& key) override;
        bool isEmpty()const override;
        std::vector<std::pair<K,V>> getAll()const override;
        ~SkipListBucket() override = default;
};

