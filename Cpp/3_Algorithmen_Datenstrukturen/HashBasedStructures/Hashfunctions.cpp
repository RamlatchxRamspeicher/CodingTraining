//djb2, FNV-1a fuer strings
//murmurhash3 fuer general purpose
// integer hashing als basic basic
#include "Hashfunctions.hpp"
//int float double long



template<typename K>
std::size_t Hasher<K>::modHash(const K& key, std::size_t tableSize){
    std::size_t hash;
    if constexpr (std::is_floating_point<K>::value) //sizeFloat == sizeInt = 4 -> constexpr
        std::memcpy(&hash, &key, sizeof(key));
    else
        hash = static_cast<std::size_t>(std::abs(key));
    return hash % tableSize;
}
template<typename K>
std::size_t Hasher<K>::multiplyHash(const K& key, std::size_t tableSize){
    std::size_t hash;
    const float A = 0.6180339887;
    hash = std::floor(std::fmod(key*A,1)*tableSize);
    return hash;
}
template<typename K>
std::size_t Hasher<K>::fnv1aHash(const K& key, std::size_t tableSize){
    const std::size_t offsetBasis = 2166136261;
    const std::size_t prime = 16777619;
    auto byteRep = to_bytes(key);
    std::size_t hash = offsetBasis;
    for(std::byte b : byteRep){
        hash ^=std::to_integer<std::size_t>(b);
        hash *=prime;
    }
    return hash % tableSize;
}

//strings
std::size_t Hasher<std::string>::fnv1a(const std::string& key, std::size_t tableSize){
    const std::size_t offsetBasis = 2166136261;
    const std::size_t prime = 16777619;
    std::size_t hash = offsetBasis;
    for(char c : key){
        hash ^=static_cast<std::size_t>(c);
        hash *=prime;
    }
    return hash % tableSize;
}
std::size_t Hasher<std::string>::djb2(const std::string& key, std::size_t tableSize){
    std::size_t hash = 5381;
    for(char c : key){
        hash = hash * 33 + c;
    }
    return hash % tableSize;
}
