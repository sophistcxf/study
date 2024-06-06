class A {
    public:
        virtual ~A() {}
};

class B : public virtual A {
    public:
        virtual ~B() {}
};

int main() {
    A* p = new B();
    // 编译失败，B虚继承于A，不能使用static_cast将A*转为B*
    B* pb = static_cast<B*>(p);
    return 0;
}
