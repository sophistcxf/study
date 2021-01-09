# 使用 rtti 编译，比不使用 rtti 编译，多了192个字节
g++ -frtti test_size.cpp -o test_size_rtti
g++ -fno-rtti test_size.cpp -o test_size_nortti
