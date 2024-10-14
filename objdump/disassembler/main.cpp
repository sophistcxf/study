#include <iostream>

class Zoo {
};

enum class AnimalType {
    kUndefined,
    kChordata,
};

class Animal {
public:
    Animal(AnimalType type) : type_(type) {}
    virtual void run() = 0;
    virtual void stop() = 0;
private:
    AnimalType type_ = AnimalType::kUndefined; 
};

class Elephant : public Animal {
public:
    Elephant() : Animal(AnimalType::kChordata) {}

    virtual void run() {}
    virtual void stop() {}
};

int main() {
    Elephant elephant;
    elephant.run();
    return 0;
}
