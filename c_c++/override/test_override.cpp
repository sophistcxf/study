class A
{
public:
#ifdef NOVIRTUAL
    void fun1() {}
#endif

    virtual void fun2() {}

#ifdef DIFFPARAM
    virtual void fun3(int n) {}
#endif
};

class B : public A
{
public:
#ifdef NOVIRTUAL
    // 由于A::fun1() 不是 virtual，所以会报错
    // error: ‘void B::fun1()’ marked override, but does not override
    void fun1() override {} 
#endif

    virtual void fun2() override {}

#ifdef DIFFPARAM
    // 与 A::fun3(int n) 参数类型不同，所以是 overload 了
    // 并没有 override
    virtual void fun3(double n) override {}
#endif
};

int main()
{
    return 0;
}
