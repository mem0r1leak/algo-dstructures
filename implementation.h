#pragma once
#include <string>

class Car {
    public:
    int year;
    std::string name;

    bool operator==(const Car &other) const {
        return this->year == other.year;
    }

    bool operator <(const Car &other) const {
        return this->year < other.year;
    }

    bool operator >(const Car &other) const {
        return this->year > other.year;
    }

    friend std::ostream& operator <<(std::ostream& s, const Car& c) {
        return s << c.name << ", " << c.year;
    }
};

template<typename T>
class BinaryTree {
public:
    class Node {
    public:
        T value;
        Node *left;
        Node *right;

        Node(T&& value, Node *left = nullptr, Node *right = nullptr) : value(value), left(left), right(right) {
        }

        Node(T& value, Node *left = nullptr, Node *right = nullptr) : value(value), left(left), right(right) {
        }
    };

    Node *head = nullptr;

    ~BinaryTree() {
        this->remove(&this->head);
    }

    void insert(T&& value) {
        if (head == nullptr) {
            head = new Node(value);
            return;
        }
        find(value) = new Node(value);
    }

    void remove(T& value) {
        auto node = &find(value);
        remove(node);
        *node = nullptr;
    }

    void remove(T&& value) {
        auto node = &find(value);
        remove(node);
        *node = nullptr;
    }

    void remove(Node **node) {
        if (*node == nullptr) { return; }
        if ((*node)->left == nullptr and (*node)->right == nullptr) {
            delete *node;
            *node = nullptr;
            return;
        }
        remove(&(*node)->left);
        remove(&(*node)->right);
        delete *node;
        *node = nullptr;
    }

    Node*& find(T&& value) {
        Node **node = &head;
        while (*node) {
            if (value == (*node)->value) {
                return *node;
            }
            if (value > (*node)->value) {
                node = &(*node)->right;
            } else { node = &(*node)->left; }
        }
        return *node;
    }

    Node*& find(T& value) {
        Node **node = &head;
        while (*node) {
            if (value == (*node)->value) {
                return *node;
            }
            if (value > (*node)->value) {
                node = &(*node)->right;
            } else { node = &(*node)->left; }
        }
        return *node;
    }
};
