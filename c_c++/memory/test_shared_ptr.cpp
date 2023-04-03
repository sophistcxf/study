#include <memory>
#include <iostream>

class C {
public:
    double d = 0.0;
    int* data = new int(10);
};

void test1() {
    std::shared_ptr<int> p1;
    std::cout << p1.use_count() << std::endl;

    // 使用 nullptr 初始化，use_count() 也为 0
    std::shared_ptr<int> p2(nullptr);
    std::cout << p2.use_count() << std::endl;

    std::shared_ptr<int> p3(new int);
    std::cout << p3.use_count() << std::endl;

    std::shared_ptr<int> p4 (new int, std::default_delete<int>());
    std::cout << "p4: " << p4.use_count() << std::endl;

    std::shared_ptr<int> p5 (new int, [](int* p){delete p;}, std::allocator<int>());
    std::cout << "p5: " << p5.use_count() << '\n';

    // p6 为 2
    std::shared_ptr<int> p6 (p5);
    std::cout << "p6: " << p6.use_count() << '\n';

    // p7.use_count() == 2
    std::shared_ptr<int> p7 (std::move(p6));
    std::cout << "p7: " << p7.use_count() << '\n';

    // p8.use_count() == 1
    std::shared_ptr<int> p8 (std::unique_ptr<int>(new int));
    std::cout << "p8: " << p8.use_count() << '\n';

    std::shared_ptr<C> obj (new C);

    // 这种构造方法叫做 aliasing constructor
    // 应用场景是，p9 想使用 obj 中的某个元素，这时 obj 也是不能销毁的
    std::shared_ptr<int> p9 (obj, obj->data);
    //std::shared_ptr<int> p10(obj);    // 这里只给 obj 是编译不过的
    // p9.use_count() == 2
    std::cout << "p9: " << p9.use_count() << '\n';
    // obj.use_count() == 2
    std::cout << "obj: " << obj.use_count() << std::endl;

    // p9 是使用 alias 构造的， p9.get() 与 obj.get() 并不相同
    std::cout << "obj.get(): " << obj.get() << std::endl;
    std::cout << "obj.get()->data: " << obj.get()->data << std::endl;
    std::cout << "p9.get(): " << p9.get() << ", *p9: " << *p9 << std::endl;

    std::shared_ptr<int> p11(p9);
    std::cout << p11.use_count() << std::endl;
}

int main() {
    test1();
    return 0;
}
