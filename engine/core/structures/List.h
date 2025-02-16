//
// Created by eceva on 2/2/2025.
//

#ifndef GAMEENGINE_LIST_H
#define GAMEENGINE_LIST_H

#include <queue>

template <typename T>
class List {
private:
    std::queue<T> queue;
public:
    List() {}
    void append(T item) {
        queue.push(item);
    }

    T getNextElement() {
        return queue.pop();
    }

    T peekNextElement() {
        return queue.front();
    }

    bool empty() {
        return queue.empty();
    }

    void remove(T object) {
        std::queue<T> newQueue;
        while (!queue.empty()) {
            T item = queue.front();
            queue.pop();
            if (item != object) {
                newQueue.push(item);
            }
        }
        queue = newQueue;
    }

    unsigned int size() {
        return queue.size();
    }
};

#endif //GAMEENGINE_LIST_H
