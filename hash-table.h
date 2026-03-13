#pragma once

template<typename K, typename V>
class HashTable {
public:
    class Bucket {
    public:
        K key;
        V value;

        Bucket(K key, V value) : key(key), value(value) {
        }

        Bucket() : key(), value() {
        }
    };

    HashTable(const size_t size = 10) {
        this->items_count = 0;
        this->table_size = size;
        this->nodes = new Node[this->table_size];
    }

    virtual ~HashTable() = default;

    size_t fill_level() const {
        return items_count * 100 / table_size;
    }

    void resize(const size_t size) {
        auto old_nodes = nodes;
        const auto old_table_size = table_size;
        this->table_size = size;
        this->nodes = new Node[this->table_size];
        this->items_count = 0;
        for (size_t i = 0; i < old_table_size; i++) {
            if (old_nodes[i].inUse)
                put(old_nodes[i].bucket.key, old_nodes[i].bucket.value);
        }
    }

    bool contains(const K &key) {
        auto node = nodeByKey(key);
        return node != nullptr and node->inUse;
    }

    void put(const K &key, const V &value) {
        if (fill_level() > 60) resize(this->table_size*2);
        if (Node *node = nodeByHash(key); !node->inUse) {
            // Звичайна вставка
            node->use(Bucket{key, value});
            ++items_count;
        }
        else {
            // Колізія
            if (auto node_by_key = nodeByKey(key)) {
                node_by_key->use(Bucket{key, value});
            }
            else {
                auto new_node = anyFreeNode();
                if (new_node == nullptr) throw std::runtime_error("Not enough nodes");
                new_node->use(Bucket{key, value});
                node->add_next(new_node);
                ++items_count;
            }
        }
    }

    void remove(const K &key) {
        if (contains(key)) {
            auto node = nodeByHash(key);
            if (node->next == nullptr) {
                // Звичайне видалення
                node->free();
                --items_count;
                return;
            }
            // Якщо елемент мав колізію
            if (node->bucket.key == key and node->next and node->next->next == nullptr) {
                node->bucket = node->next->bucket;
                return;
            }
            auto prev = node;
            while (node->bucket.key != key) {
                prev = node;
                node = node->next;
            }
            prev->next = node->next;
            node->free();
        }
    }

    Bucket *get(const K &key) {
        auto node = nodeByKey(key);
        if (node == nullptr) return nullptr;
        return &node->bucket;
    }

    V& operator[](const K &key) {
        if (get(key) == nullptr) put(key, V());
        return get(key)->value;
    }

protected:
    class Node {
    public:
        Bucket bucket;
        Node *next;
        bool inUse = false;

        Node() {
            next = nullptr;
        }

        Node(const Bucket &bucket, Node *next = nullptr) {
            this->bucket = bucket;
            this->next = next;
        }

        void use(const Bucket &b) {
            this->bucket = b;
            this->inUse = true;
        }

        void add_next(Node* node) {
            if (next == nullptr) {
                next = node;
                return;
            }
            auto node_ptr = next;
            while (node_ptr->next) {
                node_ptr = node_ptr->next;
            }
            node_ptr->next = node;
        }

        void free() {
            this->inUse = false;
            this->next = nullptr;
        }
    };

    size_t items_count;
    size_t table_size;

    Node *nodes;

    virtual size_t hash(const K &key) = 0;

    // дістаємо будь яку вільну ноду.
    Node *anyFreeNode() {
        for (size_t i = 0; i < table_size; i++) {
            if (auto node = nodes[i]; !node.inUse) return nodes + i;
        }
        return nullptr;
    }

    // Шукає ноду(можливо не активну) за ключем, включаючи ті що мають колізії.
    // Повертає nullptr якщо ноди з ключем не існує.
    Node *nodeByKey(const K &key) {
        auto node = nodeByHash(key);
        while (node and node->bucket.key != key) {
            node = node->next;
        }
        return node;
    }

    Node *nodeByHash(const K &key) {
        return nodes + hash(key) % table_size;
    }
};
