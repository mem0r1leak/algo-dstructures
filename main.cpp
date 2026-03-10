#include <iostream>
#include "implementation.h"

void printTable(const BinaryTree<Car>::Node* table) {
    if (table == nullptr) return;
    std::cout << table->value << std::endl;
    printTable(table->left);
    printTable(table->right);
}

int main() {
    BinaryTree<Car> tree{};
    tree.insert(Car{.year = 2015, .name = "Tesla Model S"});
    tree.insert(Car{.year = 2012, .name = "BMW M3"});
    tree.insert(Car{.year = 2010, .name = "Toyota Camry"});
    tree.insert(Car{.year = 2020, .name = "Porsche Taycan"});
    tree.insert(Car{.year = 2022, .name = "Lucid Air"});

    tree.remove(Car{.year = 2010, .name = ""});

    printTable(tree.head);

    const auto valueAfterRemove = tree.find(Car{.year=2020, .name = ""});
    if (valueAfterRemove != nullptr) {
        std::cout << valueAfterRemove->value.name << std::endl;
    }
    return 0;
}
