g++ -frtti test_cast.cpp

#编译错误,error: use of dynamic_cast requires -frtti
#g++ -fno-rtti test_cast.cpp
