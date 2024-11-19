#ifndef SOFTWAREENGINEERINGPROJECT_SINGLETON_H
#define SOFTWAREENGINEERINGPROJECT_SINGLETON_H

template <typename T>
class Singleton {
public:
    static T& getInstance() {
        static T instance;
        return instance;
    }

protected:
    Singleton() {}
    ~Singleton() {}

private:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
};

#endif // SOFTWAREENGINEERINGPROJECT_SINGLETON_H