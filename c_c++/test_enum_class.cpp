/*!
 * scoped enums
 */

enum class Turn
{
    LEFT = 0,
    RIGHT
};

/* 使用 scoped enums，枚举值可以同名 */
enum class E2
{
    ERROR = 0,
    RIGHT
};

int main()
{
    Turn t = Turn::LEFT;
    //t == 0;   // scoped enums 不能与整数比较了
    t == Turn::RIGHT;
    E2 e = E2::RIGHT;
    //e == t;   // 不同的枚举类型间不能隐式转换，必须要强制转换
    e == (E2)t;
    return 0;
}
