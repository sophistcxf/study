// 标识函数被废弃，使用会报warning
void __attribute__((deprecated)) foo() {
}

int main() {
    foo();
    return 0;
}
