#include <iostream>
#include <pthread.h>
#include <unistd.h>

class Singleton {
public:
    static Singleton& instance() {
        static Singleton s;
        return s;
    }
private:
    Singleton() {
        std::cout << "Singleton()" << std::endl;
        sleep(5);
    }
    Singleton(const Singleton&);
};

void* cb(void* index) {
    std::cout << "thread " << *(int*)index << std::endl;
    Singleton& s = Singleton::instance();
    return NULL;
}

int main() {
    pthread_t handler[2];
    int index[2];
    for (int i = 0; i < 2; ++i) {
        index[i] = i;
        pthread_create(&handler[i], NULL, cb, &index[i]);
    }
    for (int i = 0; i < 2; ++i) {
        pthread_join(handler[i], NULL);
    }
    return 0;
}
