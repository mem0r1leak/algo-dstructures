#include <gtest/gtest.h>
#include "../queue.h"
#include <string>

class QueueTest : public ::testing::Test {
protected:
    Queue<int> q;
};

// Перевірка базових операцій
TEST_F(QueueTest, PushPopInt) {
    q.push(1);
    q.push(2);
    q.push(3);

    EXPECT_EQ(q.pop(), 1);
    EXPECT_EQ(q.pop(), 2);
    EXPECT_EQ(q.pop(), 3);
}

// Перевірка виключення при спробі pop з порожньої черги
TEST_F(QueueTest, ThrowsOnEmpty) {
    EXPECT_THROW({
        q.pop();
    }, std::runtime_error);
}

// Перевірка роботи з рядками (динамічна пам'ять)
TEST_F(QueueTest, StringSupport) {
    Queue<std::string> sq;
    sq.push("vlad");
    sq.push("zero_trust");

    EXPECT_STREQ(sq.pop().c_str(), "vlad");
    EXPECT_EQ(sq.pop(), "zero_trust");
}

// Стрес-тест: перевірка черги на великій кількості ітерацій
TEST_F(QueueTest, StressTest) {
    for (int i = 0; i < 1000; ++i) {
        q.push(i);
    }
    for (int i = 0; i < 1000; ++i) {
        ASSERT_EQ(q.pop(), i);
    }
}

// Перевірка консистентності head/tail після вичерпання черги
TEST_F(QueueTest, Reusability) {
    q.push(100);
    q.pop();

    // Перевіряємо, чи можна знову користуватися чергою
    q.push(200);
    EXPECT_EQ(q.pop(), 200);
}