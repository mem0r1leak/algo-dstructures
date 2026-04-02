#pragma once
#include <stdexcept>

template <typename T>
class PriorityQueue {
    struct Node {
        Node *next;

        T value;
        int priority;

        Node(T value, const int priority) : next(nullptr), value(value), priority(priority) {  }
    };

    Node *head = nullptr;

public:
    PriorityQueue(PriorityQueue& q) = delete;

    PriorityQueue() = default;

    ~PriorityQueue() {
        while (!is_empty()) pop();
    }

    bool is_empty() {
        return this->head == nullptr;
    }

    void push(T value, int priority) {
        auto node = &this->head;
        while (*node != nullptr and (*node)->priority <= priority) {
            node = &(*node)->next;
        }
        auto new_node = new Node(value, priority);
        new_node->next = *node;
        *node = new_node;
    }

    T peek() {
        if (is_empty()) throw std::out_of_range("Queue is empty");
        return this->head->value;
    }

    T pop() {
        if (is_empty()) throw std::out_of_range("Queue is empty");
        auto node = head;
        head = head->next;
        auto val = node->value;
        delete node;
        return val;
    }

    PriorityQueue operator =(PriorityQueue& q) = delete;
};
