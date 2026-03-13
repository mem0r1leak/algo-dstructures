#include <functional>
#include "gtest/gtest.h"
#include "../hash-table.h"

template<typename K, typename V>
class UniversalMock : public HashTable<K, V> {
public:
    bool forceCollisions = false;

    UniversalMock(size_t size) : HashTable<K, V>(size) {
    }

    size_t hash(const K &key) override {
        return forceCollisions ? 0 : std::hash<K>{}(key);
    }

    size_t getCount() const { return this->items_count; }
    size_t getSize() const { return this->table_size; }

    auto *getNodes() { return this->nodes; }
};

class HashTableExtremeTest : public ::testing::Test {
protected:
    UniversalMock<std::string, int> table{2};
};

TEST_F(HashTableExtremeTest, CollisionAvalancheAndResize) {
    table.forceCollisions = true;
    constexpr int limit = 10;
    table.resize(limit);

    for (int i = 0; i < limit; ++i) {
        table.put("key_" + std::to_string(i), i);
    }

    table.resize(20);

    for (int i = 0; i < limit; ++i) {
        ASSERT_TRUE(table.contains("key_" + std::to_string(i)))
            << "Data lost after resizing a heavy collision chain at index " << i;
    }
    EXPECT_EQ(table.getCount(), limit);
}

TEST_F(HashTableExtremeTest, InfiniteValueUpdate) {
    for (int i = 0; i < 1000; ++i) {
        table.put("static_key", i);
    }
    EXPECT_EQ(table.getCount(), 1);
    EXPECT_TRUE(table.contains("static_key"));
}

TEST_F(HashTableExtremeTest, MultipleResizeIntegrity) {
    table.put("a", 1);
    table.resize(100);
    table.put("b", 2);
    table.resize(2);
    table.resize(50);

    EXPECT_TRUE(table.contains("a"));
    EXPECT_TRUE(table.contains("b"));
    EXPECT_EQ(table.getCount(), 2);
}

TEST_F(HashTableExtremeTest, FindNodeSafety) {
    table.put("real", 1);
    EXPECT_FALSE(table.contains("ghost"));
    EXPECT_FALSE(table.contains(""));
}

TEST_F(HashTableExtremeTest, FullTableSaturation) {
    UniversalMock<int, int> smallTable(5);
    for (int i = 0; i < 100; ++i) {
        ASSERT_NO_THROW(smallTable.put(i, i*10));
    }
}

template<typename K, typename V>
class HashTableNormalMock : public HashTable<K, V> {
public:
    HashTableNormalMock(size_t size) : HashTable<K, V>(size) {
    }

    size_t hash(const K &key) override {
        return std::hash<K>{}(key);
    }

    size_t getCount() const {
        return this->items_count;
    }
};

class HashTableGetTest : public testing::Test {
protected:
    UniversalMock<std::string, int> table{10};
};

TEST_F(HashTableGetTest, RetrievesCorrectValue) {
    table.put("apple", 100);
    auto *bucket = table.get("apple");

    ASSERT_NE(bucket, nullptr);
    EXPECT_EQ(bucket->key, "apple");
    EXPECT_EQ(bucket->value, 100);
}

TEST_F(HashTableGetTest, RetrievesValueInCollisionChain) {
    table.forceCollisions = true;
    table.put("k1", 10);
    table.put("k2", 20);
    table.put("k3", 30);

    auto *b1 = table.get("k1");
    auto *b2 = table.get("k2");
    auto *b3 = table.get("k3");

    ASSERT_NE(b1, nullptr);
    ASSERT_NE(b2, nullptr);
    ASSERT_NE(b3, nullptr);

    EXPECT_EQ(b1->value, 10);
    EXPECT_EQ(b2->value, 20);
    EXPECT_EQ(b3->value, 30);
}

TEST_F(HashTableGetTest, CanModifyValueThroughReturnedBucket) {
    table.put("mutable", 50);
    auto *bucket = table.get("mutable");

    ASSERT_NE(bucket, nullptr);
    bucket->value = 999;


    EXPECT_EQ(table.get("mutable")->value, 999);
}

TEST_F(HashTableGetTest, ReturnsNullptrForNonExistentKey) {
    table.put("existing", 1);
    EXPECT_EQ(table.get("non_existent"), nullptr);
}

TEST_F(HashTableGetTest, WorksWithComplexValues) {
    HashTableNormalMock<int, std::string> stringTable(5);
    stringTable.put(1, "one");

    auto *bucket = stringTable.get(1);
    ASSERT_NE(bucket, nullptr);
    EXPECT_EQ(bucket->value, "one");
}

class HashTableGetCombinedTest : public ::testing::Test {
protected:
    UniversalMock<std::string, int> table{10};
};

TEST_F(HashTableGetCombinedTest, RetrievesValuesNormally) {
    table.forceCollisions = false;
    table.put("A", 1);
    table.put("B", 2);

    auto *bA = table.get("A");
    auto *bB = table.get("B");

    ASSERT_NE(bA, nullptr);
    EXPECT_EQ(bA->value, 1);
    ASSERT_NE(bB, nullptr);
    EXPECT_EQ(bB->value, 2);
}

TEST_F(HashTableGetCombinedTest, RetrievesValuesInDeepCollision) {
    table.forceCollisions = true;
    table.put("key1", 100);
    table.put("key2", 200);
    table.put("key3", 300);

    EXPECT_EQ(table.get("key1")->value, 100);
    EXPECT_EQ(table.get("key2")->value, 200);
    EXPECT_EQ(table.get("key3")->value, 300);
}

TEST_F(HashTableGetCombinedTest, GetReflectsUpdatedValuesInCollision) {
    table.forceCollisions = true;
    table.put("same_bucket", 10);
    table.put("other_key", 20);
    table.put("same_bucket", 99);

    auto *res = table.get("same_bucket");
    ASSERT_NE(res, nullptr);
    EXPECT_EQ(res->value, 99);
}

TEST_F(HashTableGetCombinedTest, ReturnsNullptrWhenKeyMissingInChain) {
    table.forceCollisions = true;
    table.put("existing1", 1);
    table.put("existing2", 2);
    EXPECT_EQ(table.get("not_here"), nullptr);
}

class HashTableRemoveTest : public ::testing::Test {
protected:
    UniversalMock<std::string, int> table{5};
};

TEST_F(HashTableRemoveTest, SimpleRemoveAndContains) {
    table.put("apple", 1);
    EXPECT_TRUE(table.contains("apple"));

    table.remove("apple");
    EXPECT_FALSE(table.contains("apple"));
    EXPECT_EQ(table.getCount(), 0);
}

TEST_F(HashTableRemoveTest, DoubleRemoveIsSafe) {
    table.put("apple", 1);
    table.remove("apple");

    // Повторне видалення не має викликати Segmentation Fault або виключення
    // (залежно від твоєї логіки, зазвичай це просто ігнорується або повертає false)
    EXPECT_NO_THROW(table.remove("apple"));
    EXPECT_EQ(table.getCount(), 0);
}

TEST_F(HashTableRemoveTest, CanPutAgainAfterRemove) {
    table.put("apple", 1);
    table.remove("apple");

    // Перевірка, чи вільне місце можна зайняти знову
    EXPECT_NO_THROW(table.put("apple", 2));
    EXPECT_TRUE(table.contains("apple"));
    EXPECT_EQ(table.get("apple")->value, 2);
}

TEST_F(HashTableRemoveTest, RemoveMiddleElementInChain) {
    table.forceCollisions = true; // Всі в 0-й бакет
    table.put("first", 10);
    table.put("middle", 20);
    table.put("last", 30);

    // Видаляємо середній
    table.remove("middle");

    EXPECT_FALSE(table.contains("middle"));
    // КРИТИЧНО: "last" має бути доступним!
    EXPECT_TRUE(table.contains("first"));
    EXPECT_TRUE(table.contains("last"));
    EXPECT_EQ(table.get("last")->value, 30);
}

TEST_F(HashTableRemoveTest, RemoveHeadOfChain) {
    table.forceCollisions = true;
    table.put("head", 1);
    table.put("tail", 2);

    table.remove("head");

    EXPECT_FALSE(table.contains("head"));
    EXPECT_TRUE(table.contains("tail"));
    EXPECT_EQ(table.get("tail")->value, 2);
}

TEST_F(HashTableRemoveTest, StressRemoveAndResize) {
    table.forceCollisions = false;
    constexpr int count = 20;

    // 1. Заповнюємо
    for (int i = 0; i < count; ++i) table.put("k" + std::to_string(i), i);

    // 2. Видаляємо парні
    for (int i = 0; i < count; i += 2) table.remove("k" + std::to_string(i));

    // 3. Робимо Resize
    table.resize(50);

    // 4. Перевіряємо цілісність
    for (int i = 0; i < count; ++i) {
        if (i % 2 == 0) {
            EXPECT_FALSE(table.contains("k" + std::to_string(i)));
        } else {
            ASSERT_TRUE(table.contains("k" + std::to_string(i)));
            EXPECT_EQ(table.get("k" + std::to_string(i))->value, i);
        }
    }
}


class HashTableNormalUsageTest : public ::testing::Test {
protected:
    HashTableNormalMock<std::string, int> table{5};
};

TEST_F(HashTableNormalUsageTest, BracketOperatorCreatesDefaultValues) {
    // Для int дефолт має бути 0
    int value = table["non_existent_yet"];
    EXPECT_EQ(value, 0);
    EXPECT_TRUE(table.contains("non_existent_yet"));

    // Для рядків (якщо змінити тип V)
    HashTableNormalMock<std::string, std::string> stringTable(5);
    std::string str = stringTable["missing_string"];
    EXPECT_EQ(str, "");
}

TEST_F(HashTableNormalUsageTest, BracketOperatorCanBeUsedForAssignment) {
    table["new_key"] = 42;
    // Викликається put(key, V()), а потім повертається посилання на 0, яке ми перезаписуємо на 42
    EXPECT_EQ(table.get("new_key")->value, 42);
}
