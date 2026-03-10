#include <gtest/gtest.h>
#include "../implementation.h"

class BinaryTreeTest : public testing::Test {
protected:
    BinaryTree<Car> tree;

    bool IsEmpty() const {
        return tree.head == nullptr;
    }
};

// --- Тести вставки та пошуку ---

TEST_F(BinaryTreeTest, InsertAndFindSingleElement) {
    tree.insert(Car{2015, "Tesla"});
    const auto node = tree.find(Car{2015, ""});
    
    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->value.year, 2015);
    EXPECT_STREQ(node->value.name.c_str(), "Tesla");
}

TEST_F(BinaryTreeTest, FindNonExistentReturnsNull) {
    tree.insert(Car{2020, "Porsche"});
    EXPECT_EQ(tree.find(Car{1999, ""}), nullptr);
}

TEST_F(BinaryTreeTest, InsertMultipleMaintainsBSTStructure) {
    tree.insert(Car{2015, "Root"});
    tree.insert(Car{2010, "Left"});
    tree.insert(Car{2020, "Right"});

    ASSERT_NE(tree.head, nullptr);
    EXPECT_EQ(tree.head->value.year, 2015);
    EXPECT_EQ(tree.head->left->value.year, 2010);
    EXPECT_EQ(tree.head->right->value.year, 2020);
}

// --- Тести видалення (твоя логіка "Subtree Wipeout") ---

TEST_F(BinaryTreeTest, RemoveLeafNode) {
    tree.insert(Car{2015, "Root"});
    tree.insert(Car{2010, "Leaf"});
    
    tree.remove(Car{2010, ""});
    
    EXPECT_EQ(tree.find(Car{2010, ""}), nullptr);
    EXPECT_NE(tree.find(Car{2015, ""}), nullptr); // Корінь має лишитися
}

TEST_F(BinaryTreeTest, RemoveNodeWithSubtreeDeletesEverythingBelow) {
    // Будуємо гілку: 2015 -> 2010 -> (2008, 2012)
    tree.insert(Car{2015, "Root"});
    tree.insert(Car{2010, "Parent"});
    tree.insert(Car{2008, "Child1"});
    tree.insert(Car{2012, "Child2"});

    // Видаляємо 2010. 2008 та 2012 мають "померти" разом з ним
    tree.remove(Car{2010, ""});

    EXPECT_EQ(tree.find(Car{2010, ""}), nullptr);
    EXPECT_EQ(tree.find(Car{2008, ""}), nullptr);
    EXPECT_EQ(tree.find(Car{2012, ""}), nullptr);
    EXPECT_NE(tree.find(Car{2015, ""}), nullptr); // Корінь живий
}

TEST_F(BinaryTreeTest, RemoveRootMakesTreeEmpty) {
    tree.insert(Car{2015, "Root"});
    tree.insert(Car{2010, "L"});
    tree.insert(Car{2020, "R"});

    tree.remove(Car{2015, ""});

    EXPECT_TRUE(IsEmpty());
}

// --- Тести на специфічні випадки (Edge Cases) ---

TEST_F(BinaryTreeTest, RemoveFromEmptyTreeDoesNotCrash) {
    EXPECT_NO_THROW(tree.remove(Car{2022, "Ghost"}));
}

TEST_F(BinaryTreeTest, OverwriteExistingNodeLeakCheck) {
    // Цей тест перевіряє твою поточну логіку перезапису в find
    tree.insert(Car{2010, "Old"});
    tree.insert(Car{2010, "New"});
    
    auto node = tree.find(Car{2010, ""});
    EXPECT_EQ(node->value.name, "New");
}

// --- Глибока рекурсія (Стек та пам'ять) ---

TEST_F(BinaryTreeTest, DeepUnbalancedTreeStress) {
    // Створюємо "дерево-палицю" (вироджене дерево)
    // 1000 елементів в одну лінію
    for(int i = 0; i < 1000; ++i) {
        tree.insert(Car{i, "Car " + std::to_string(i)});
    }

    // Видаляємо корінь (0) — це має спровокувати рекурсивне видалення
    // всіх 1000 вузлів. Перевірка на переповнення стека.
    EXPECT_NO_THROW(tree.remove(Car{0, ""}));
    EXPECT_TRUE(IsEmpty());
}

// --- Робота з тимчасовими об'єктами (Rvalue) ---

TEST_F(BinaryTreeTest, InsertRvalueCorrectness) {
    // Перевіряємо, чи твій метод void insert(T&& value)
    // коректно приймає тимчасові об'єкти без зайвого копіювання
    tree.insert({2025, "Future Car"});
    auto node = tree.find(Car{2025, ""});

    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->value.name, "Future Car");
}

// --- Специфічне видалення дублікатів ---

TEST_F(BinaryTreeTest, RemoveDuplicateOverwriteCheck) {
    // Ти вже знаєш, що твій insert затирає попередній вузол з тим самим ключем.
    // Перевіримо, чи після такого затирання remove працює коректно.
    tree.insert(Car{2000, "First"});
    tree.insert(Car{2000, "Second"});
    tree.insert(Car{2000, "Third"}); // Зараз у дереві лише Third (Node 3)

    tree.remove(Car{2000, ""});

    EXPECT_TRUE(IsEmpty());
    // Якщо тут спрацював AddressSanitizer — значить Node 1 та Node 2 потекли.
}

// --- Пошук у "майже порожньому" дереві ---

TEST_F(BinaryTreeTest, FindAfterTotalClear) {
    tree.insert(Car{10, "A"});
    tree.remove(Car{10, ""});

    // Після видалення єдиного вузла find має повертати nullptr,
    // а не сміття або стару адресу head
    EXPECT_EQ(tree.find(Car{10, ""}), nullptr);
}

// --- Константні посилання в remove ---

TEST_F(BinaryTreeTest, RemoveLvalueReference) {
    Car myCar{2021, "Referenced"};
    tree.insert(std::move(myCar));

    // Перевіряємо, чи спрацює remove(T& value)
    tree.remove(myCar);

    EXPECT_TRUE(IsEmpty());
}

TEST_F(BinaryTreeTest, ZigZagStructure) {
    // 50 -> 25 -> 37 -> 31 -> 34
    tree.insert(Car{50, "50"});
    tree.insert(Car{25, "25"});
    tree.insert(Car{37, "37"});
    tree.insert(Car{31, "31"});
    tree.insert(Car{34, "34"});

    tree.remove(Car{25, ""}); // Має видалити всю ламану гілку нижче 50

    EXPECT_EQ(tree.find(Car{34, ""}), nullptr);
    EXPECT_NE(tree.find(Car{50, ""}), nullptr);
}

TEST_F(BinaryTreeTest, MoveSemanticsTest) {
    Car c{2026, "MoveMe"};
    tree.insert(std::move(c));

    // Після move об'єкт 'c' у невизначеному стані,
    // але в дереві має бути копія або переміщений об'єкт
    ASSERT_NE(tree.find(Car{2026, ""}), nullptr);
    EXPECT_EQ(tree.find(Car{2026, ""})->value.name, "MoveMe");
}

TEST_F(BinaryTreeTest, SearchInEmptyAfterClearing) {
    tree.insert(Car{100, "100"});
    tree.remove(Car{100, ""});

    // head має бути nullptr, find не повинен намагатися розіменувати його
    EXPECT_EQ(tree.find(Car{100, ""}), nullptr);
}

// --- Складна ієрархія (видалення "середини") ---

TEST_F(BinaryTreeTest, RemoveMiddleNodeWithComplexSubtrees) {
    /* 20 (Root)
         /  \
        10   30
       /  \
      5    15
    */
    tree.insert(Car{20, "20"});
    tree.insert(Car{10, "10"});
    tree.insert(Car{30, "30"});
    tree.insert(Car{5, "5"});
    tree.insert(Car{15, "15"});

    // Видаляємо 10. Мають зникнути і 5, і 15.
    tree.remove(Car{10, ""});

    EXPECT_NE(tree.find(Car{20, ""}), nullptr);
    EXPECT_NE(tree.find(Car{30, ""}), nullptr);
    EXPECT_EQ(tree.find(Car{10, ""}), nullptr);
    EXPECT_EQ(tree.find(Car{5, ""}), nullptr);
    EXPECT_EQ(tree.find(Car{15, ""}), nullptr);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}