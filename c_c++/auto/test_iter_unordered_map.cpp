#include <unordered_map>
#include <string>

class Key
{
public:
    Key(int k) : key(k) {}
    int key;
};

int main()
{
    std::unordered_map<std::string, int> hash;
    for (const auto& p : hash) {
    }
    for (const std::pair<std::string, int>& p : hash) {
    }
    for (const std::pair<const std::string, int>& p : has) {
    }
}
