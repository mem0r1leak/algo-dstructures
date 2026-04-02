#include <gtest/gtest.h>
#include "../priority_queue.h"

// Фікстура для тестів, щоб не дублювати об'єкт
class PriorityQueueTest : public ::testing::Test {
protected:
    PriorityQueue<int> pq;
};

// 1. Перевірка базової логіки: найменший пріоритет виходить першим
TEST_F(PriorityQueueTest, CorrectOrdering) {
    pq.push(10, 3);
    pq.push(20, 1); // Найвищий пріоритет (найменше число)
    pq.push(30, 2);

    EXPECT_EQ(pq.pop(), 20); // Пріоритет 1
    EXPECT_EQ(pq.pop(), 30); // Пріоритет 2
    EXPECT_EQ(pq.pop(), 10); // Пріоритет 3
    EXPECT_TRUE(pq.is_empty());
}

// 2. Перевірка стабільності (FIFO для однакових пріоритетів)
// Це важливо: той, хто прийшов раніше з таким же пріоритетом, має вийти раніше
TEST_F(PriorityQueueTest, StabilityFIFO) {
    pq.push(100, 10);
    pq.push(200, 10);
    pq.push(300, 10);

    EXPECT_EQ(pq.pop(), 100);
    EXPECT_EQ(pq.pop(), 200);
    EXPECT_EQ(pq.pop(), 300);
}

// 3. Перевірка Peek
TEST_F(PriorityQueueTest, PeekWorks) {
    pq.push(42, 1);
    EXPECT_EQ(pq.peek(), 42);
    EXPECT_FALSE(pq.is_empty()); // Peek не має видаляти
    EXPECT_EQ(pq.pop(), 42);
}

// 4. Перевірка обробки помилок (Exceptions)
TEST_F(PriorityQueueTest, ThrowsOnEmpty) {
    EXPECT_THROW(pq.pop(), std::out_of_range);
    EXPECT_THROW(pq.peek(), std::out_of_range);
}

// 5. Тест на "витривалість" (багато пушів/попів упереміш)
TEST_F(PriorityQueueTest, InterleavedOperations) {
    pq.push(1, 10);
    pq.push(2, 1);
    EXPECT_EQ(pq.pop(), 2);

    pq.push(3, 5);
    EXPECT_EQ(pq.pop(), 3);
    EXPECT_EQ(pq.pop(), 1);
    EXPECT_TRUE(pq.is_empty());
}
