#include <type_traits>
#include <iostream>

template <class _Tp> struct _LIBCPP_TEMPLATE_VIS my_is_array
: public std::false_type {};

template <class _Tp> struct _LIBCPP_TEMPLATE_VIS my_is_array<_Tp[]>
: public std::true_type {};

template <class _Tp, size_t _Np> struct _LIBCPP_TEMPLATE_VIS my_is_array<_Tp[_Np]>
: public std::true_type {};

int main() {
  std::cout << std::boolalpha;
  std::cout << "is_array:" << std::endl;
  std::cout << "int: " << my_is_array<int>::value << std::endl;
  std::cout << "int[3]: " << my_is_array<int[3]>::value << std::endl;
  std::cout << "array<int,3>: " << my_is_array<std::array<int,3>>::value << std::endl;
  std::cout << "string: " << my_is_array<std::string>::value << std::endl;
  std::cout << "string[3]: " << my_is_array<std::string[3]>::value << std::endl;
  return 0;
}