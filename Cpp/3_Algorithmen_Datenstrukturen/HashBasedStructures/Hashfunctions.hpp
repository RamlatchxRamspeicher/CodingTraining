//djb2, FNV-1a fuer strings
//murmurhash3 fuer general purpose
// integer hashing als basic basic
#include <string>
#include <cmath>
#include <vector>
//int float double long

template <typename T>
std::vector<std::byte> to_bytes(const T& obj) {
    std::vector<std::byte> bytes(sizeof(T));
    std::memcpy(bytes.data(), &obj, sizeof(T));
    return bytes;
}

template<typename K>
struct Hasher{
    static std::size_t modHash(const K& key, std::size_t tableSize);
    static std::size_t multiplyHash(const K& key, std::size_t tableSize);
    static std::size_t fnv1aHash(const K& key, std::size_t tableSize);
};

//strings
template<>
struct Hasher<std::string>{
    static std::size_t djb2(const std::string& key, std::size_t tableSize);
    static std::size_t fnv1a(const std::string& key, std::size_t tableSize);
};
