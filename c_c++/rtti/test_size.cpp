class A
{
public:
    virtual void foo() {}
};

int main()
{
    A* pa = new A();
    delete pa;
    return 0;
}
