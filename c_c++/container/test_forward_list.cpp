#include <iostream>
#include <forward_list>

int main() {
    std::forward_list<int> fl;
    fl.insert_after(1);
    fl.insert_after(2);
    return 0;
}
