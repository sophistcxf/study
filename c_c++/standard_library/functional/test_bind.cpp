// bind example
#include <iostream>     // std::cout
#include <functional>   // std::bind
                        //
namespace sph = std::placeholders;

// a function: (also works with function object: std::divides<double> my_divide;)
double my_divide (double x, double y) {return x/y;}

struct MyPair {
  double a,b;
  double multiply() {return a*b;}
};

void test1() {
  // binding functions:
  auto fn_five = std::bind (my_divide,10,3);               // returns 10/2
  std::cout << fn_five() << '\n';                          // 3.3333
  auto fn_five2 = std::bind<int>(my_divide,10,3);
  std::cout << fn_five2() << '\n';                         // 3

  // 这个输出 3.33333
  fn_five = std::bind<int>(my_divide,10,3);
  std::cout << fn_five() << '\n';
                

  auto fn_half = std::bind (my_divide, sph::_1,2);               // returns x/2
  std::cout << fn_half(10) << '\n';                        // 5

  auto fn_invert = std::bind (my_divide, sph::_2, sph::_1);            // returns y/x
  std::cout << fn_invert(10,2) << '\n';                    // 0.2

  auto fn_rounding = std::bind<int> (my_divide, sph::_1, sph::_2);     // returns int(x/y)
  std::cout << fn_rounding(10,3) << '\n';                  // 3

  MyPair ten_two {10,2};

  // binding members:
  auto bound_member_fn = std::bind (&MyPair::multiply, sph::_1); // returns x.multiply()
  std::cout << bound_member_fn(ten_two) << '\n';           // 20

  auto bound_member_data = std::bind (&MyPair::a,ten_two); // returns ten_two.a
  std::cout << bound_member_data() << '\n';                // 10
}

void increment(int& value) {
    value++;
}

void test2() {
    auto incre = std::bind(increment, sph::_1);

    int value = 0;
    incre(value);
    std::cout << value << std::endl;
}

int main() {
    test2();
}
