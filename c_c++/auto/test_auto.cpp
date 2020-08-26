
#ifdef NOINIT
void auto_without_init()
{
    auto i; // error, auto 声明的变量必需要初始化，否则无法推导出类型
}
#endif

int main()
{
#ifdef NOINIT
    auto_without_init();
#endif
    return 0;
}
