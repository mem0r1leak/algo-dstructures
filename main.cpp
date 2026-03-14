#include <iostream>
#include "hash-table.h"

template <typename V>
class StringHashMap : public HashTable<std::string, V> {
public:
    StringHashMap(size_t size) : HashTable<std::string, V>(size) {}

    void printInfo() {
        std::cout << "Nodes: " << std::endl;
        for (size_t i = 0; i < this->table_size; i++) {
            if (this->nodes[i].inUse) {
                std::cout << "|next: " << this->nodes[i].next << ", curr:" << this->nodes+i << "| " << this->nodes[i].bucket.key << ": " << this->nodes[i].bucket.value << std::endl;
            }
        }
    }

    size_t items_c() {
        return this->items_count;
    }

protected:
    size_t hash(const std::string &key) override {
        return std::hash<std::string>()(key);
    }
};

// int main() {
//     StringHashMap<int> table(5);
//
//     table.put("Vlad", 2000);
//     table.put("Stas", 1000);
//     table.put("a", 5);
//     table.put("a", 6);
//     table.put("Banana", 0);
//
//     std::cout << table["Vlad"] << std::endl;
//     std::cout << table["Stas"] << std::endl;
//     std::cout << table["a"] << std::endl;
//     std::cout << table["Banana"] << std::endl;
//     table["Banana"] = 6;
//     std::cout << table["Banana"] << std::endl;
//     std::cout << "items count: " << table.items_c() << std::endl;
//     std::cout << "fill level: " << table.fill_level() << std::endl;
//     return 0;
// }
