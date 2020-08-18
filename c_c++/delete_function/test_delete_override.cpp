void fun(int n) {}

#ifdef DELETE_OVERLOAD
void fun(double d) = delete;
void fun(char c) = delete;
#endif

int main()
{
    fun(10);

    // 如果声明 DELETE_OVERLOAD，则fun 对于 double 和 char 的重载版本将被
    // 删除，否则会调用重载版本
    fun(10.0);
    fun('a');
    return 0;
}
