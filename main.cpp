#include <iostream>
#include "priority_queue.h"

int main() {
    PriorityQueue<int> queue;

    queue.push(5, 2);
    queue.push(1, 3);
    queue.push(4, 4);
    queue.push(3, 5);
    queue.push(2, 2);
    queue.push(6, 1);

    // std::cout << queue.peek() << std::endl;
    std::cout << queue.pop() << std::endl;
    std::cout << queue.pop() << std::endl;
    std::cout << queue.pop() << std::endl;
    std::cout << queue.pop() << std::endl;
    std::cout << queue.pop() << std::endl;
    std::cout << queue.pop() << std::endl;

    return 0;
}
