#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

int main() {
    constexpr int num_ops = 100000;
    std::unordered_map<std::string, int> table;
    // table.reserve(num_ops * 2);

    // Тест вставки
    for (int i = 0; i < num_ops; ++i) {
        table["key_" + std::to_string(i)] = i;
    }

    // Тест читання
    long long sum = 0;
    for (int i = 0; i < num_ops; ++i) {
        auto it = table.find("key_" + std::to_string(i));
        if (it != table.end()) sum += it->second;
    }

    // Тест видалення
    for (int i = 0; i < num_ops; i += 2) {
        table.erase("key_" + std::to_string(i));
    }

    std::cout << "Final sum: " << sum << std::endl;
    return 0;
}