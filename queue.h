#pragma once

template <typename T>
class Queue {
    public:
    struct Node {
        T data;
        Node* next;

        Node(T& data) : data(data), next(nullptr) { }
    };

    Queue() : head(nullptr), tail(nullptr) {  }

    void push(T data) {
        Node* new_node = new Node(data);
        if (head == nullptr) {
            head = new_node;
            tail = new_node;
            return;
        }
        tail->next = new_node;
        tail = tail->next;
    }

    T peek() {
        return head->data;
    }

    T pop() {
        if (head == nullptr) throw std::runtime_error("Pop from empty q");
        auto head_ptr = head;
        auto data = head_ptr->data;
        head = head->next;
        delete head_ptr;
        return data;
    }

private:
    Node* head;
    Node* tail;
};
