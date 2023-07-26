class Base {
public:
    virtual void Show(int x) final; // 虚函数
};

class Derived : public Base {
public:
    virtual void Show(int x) override; // 重写提示错误  
};

int main() {
    return 0;
}
