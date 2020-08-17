#include <map>
#include <iostream>

int main()
{
    std::map<std::string, int> m;
    m.insert(std::make_pair("zhangsan", 0));
    m.insert(std::make_pair("lisi", 1));
    m.insert(std::make_pair("wangwu", 2));
    m.insert(std::make_pair("zhuliu", 3));
    m.insert(std::make_pair("zhuoqi", 4));

    // 这时候，p 是 m 中元素的一个副本，并不真的是其引用
    // 修改 p2.first，并不会修改到 m 中的 key
    // 如果使用 &p，得到的是临时变量的地址
    for (const std::pair<std::string, int>& p : m) {
        std::pair<std::string, int>& p2 = const_cast<std::pair<std::string, int>&>(p);
        p2.first = p2.first + "_new";
    }
    for (const auto& p : m) {
        std::cout << p.first << std::endl;
    }

    // auto 被推导为 const std::pair<const std::basic_string<char>, int>
    // 这时候，p 是 m 中元素的引用，修改 p.first，会修改到 key
    for (const auto& p : m) {
        std::string& key = const_cast<std::string&>(p.first);   // 强行修改 p
        key = key + "_new";
    }
    for (const auto& p : m) {
        std::cout << p.first << std::endl;
    }

    return 0;
}
