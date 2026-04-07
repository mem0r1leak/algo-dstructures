#include <iostream>
#include "priority_queue.h"
#include "queue.h"

int main() {
    PriorityQueue<int> queue;

    queue.push(5, 6);
    queue.push(1, 1);
    queue.push(4, 2);
    queue.push(3, 1);
    queue.push(2, 2);
    queue.push(6, 9);

    // std::cout << queue.peek() << std::endl;
    std::cout << queue.pop() << std::endl;
    std::cout << queue.pop() << std::endl;
    std::cout << queue.pop() << std::endl;
    std::cout << queue.pop() << std::endl;
    std::cout << queue.pop() << std::endl;
    std::cout << queue.pop() << std::endl;

    return 0;
}
