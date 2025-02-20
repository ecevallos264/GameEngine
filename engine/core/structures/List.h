//
// Created by eceva on 2/2/2025.
//

#ifndef GAMEENGINE_LIST_H
#define GAMEENGINE_LIST_H

template <typename T>
class List {
private:
    std::vector<T> queue;
public:
    List() {}

    void add(T item) {
        queue.push_back(item);
    }

    T getNextElement() {
        T item = queue.front();
        queue.erase(queue.begin());
        return item;
    }

    T peekNextElement() {
        return queue.front();
    }

    bool empty() {
        return queue.empty();
    }

    bool contains(T object) {
        return std::find(queue.begin(), queue.end(), object) != queue.end();
    }

    void remove(T object) {
        auto it = std::find(queue.begin(), queue.end(), object);
        if (it != queue.end()) {
            queue.erase(it);
        }
    }

    void removeAt(size_t index) {
        if (index < queue.size()) {
            queue.erase(queue.begin() + index);
        }
    }

    typename std::vector<T>::iterator begin() {
        return queue.begin();
    }

    typename std::vector<T>::iterator end() {
        return queue.end();
    }

    typename std::vector<T>::const_iterator begin() const {
        return queue.begin();
    }

    typename std::vector<T>::const_iterator end() const {
        return queue.end();
    }

    T& operator[](size_t index) {
        return queue[index];
    }

    const T& operator[](size_t index) const {
        return queue[index];
    }

    unsigned int size() {
        return queue.size();
    }

    void clear() {
        queue.clear();
    }
};

#endif //GAMEENGINE_LIST_H
