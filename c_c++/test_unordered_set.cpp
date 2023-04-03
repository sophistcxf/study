#include <unordered_set>

enum E {
    E_0 = 0,
};


int main() {
    std::unordered_set<E, std::hash<int> > e;
    return 0;
}
