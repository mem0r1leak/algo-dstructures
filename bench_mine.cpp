#include <iostream>
#include <string>
#include <vector>
#include "hash-table.h"

// Реалізація з твоїм хешем для рядків
template <typename V>
class MyHashMap : public HashTable<std::string, V> {
public:
    MyHashMap(size_t size) : HashTable<std::string, V>(size) {}
protected:
    size_t hash(const std::string &key) override {
        return std::hash<std::string>{}(key);
    }
};

int main() {
    constexpr int num_ops = 100000;
    MyHashMap<int> table(num_ops * 2);
    // table.resize(num_ops * 2);

    // Тест вставки
    for (int i = 0; i < num_ops; ++i) {
        table.put("key_" + std::to_string(i), i);
    }

    // Тест читання
    long long sum = 0;
    for (int i = 0; i < num_ops; ++i) {
        auto b = table.get("key_" + std::to_string(i));
        if (b) sum += b->value;
    }

    // Тест видалення
    for (int i = 0; i < num_ops; i += 2) {
        table.remove("key_" + std::to_string(i));
    }

    std::cout << "Final sum: " << sum << std::endl;
    return 0;
}