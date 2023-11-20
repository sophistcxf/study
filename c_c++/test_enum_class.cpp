#include <iostream>

/*!
 * scoped enums
 */

enum class Turn
{
    LEFT = 0,
    RIGHT
};

enum E {
    ERROR = 0,
    RIGHT,

    //如果超过0xFFFFFFFF，则报错
    //error: enumerator value evaluates to 1099511627775, which cannot be narrowed to type 'int' [-Wc++11-narrowing]
    //E_MAX = 0xFFFFFFFFFF
};

/* 使用 scoped enums，枚举值可以同名 */
enum class E2
{
    ERROR = 0,
    RIGHT,
    //如果超过0xFFFFFFFF，则报错
    //error: enumerator value evaluates to 1099511627775, which cannot be narrowed to type 'int' [-Wc++11-narrowing]
    //E_MAX = 0xFFFFFFFFFF
};

// enum class 可以指定大小
enum class E3 : std::uint64_t {
    ERROR = 0,
    RIGHT,
    E_MAX = 0xFFFFFFFFFF
};

enum class EForwardDelare;

//enum不允许前置声明
//error: ISO C++ forbids forward references to 'enum' types
//enum EForwardDelare2;


int main()
{
    Turn t = Turn::LEFT;
    //t == 0;   // scoped enums 不能与整数比较了
    if (t == Turn::RIGHT) {

    }
    E2 e = E2::RIGHT;
    //e == t;   // 不同的枚举类型间不能隐式转换，必须要强制转换
    if (e == (E2)t) {

    }

    std::cout << sizeof(Turn) << std::endl;
    std::cout << sizeof(E) << "\t" << std::endl;
    std::cout << sizeof(E3) << "\t" << static_cast<std::uint64_t>(E3::RIGHT) << "\t" << static_cast<std::uint64_t>(E3::E_MAX) << std::endl;

    return 0;
}
