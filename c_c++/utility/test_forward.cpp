#include <stdio.h>
#include <unistd.h>
#include <iostream>

class CData
{
public:
	CData() = delete;
	CData(const char* ch) : data(ch)
	{
		std::cout << "CData(const char* ch)" << std::endl;
	}
	CData(const std::string& str) : data(str)
	{
		std::cout << "CData(const std::string& str)" << std::endl;
	}
	CData(std::string&& str) : data(str)
	{
		std::cout << "CData(std::string&& str)" << std::endl;
	}
	~CData()
	{
		std::cout << "~CData()" << std::endl;
	}
private:
	std::string data;   // 表示类内部管理的资源
};

template<typename T>
CData* Creator(T&& t) { // 利用&&万能引用，引用折叠： T&& && -> T&&; T&& & -> T&
	return new CData(t);
}

template<typename T>
CData* Creator2(T&& t) { // 利用&&万能引用，引用折叠： T&& && -> T&&; T&& & -> T&
	return new CData(std::forward<T>(t));
}

int main(void) {
    std::string str1 = "hello";  
    std::string str2 = " world";
    CData* p1 = Creator(str1);         // 参数折叠为左值引用，调用CData构造函数
    CData* p2 = Creator(str1 + str2);  // 参数折叠为右值引用，但在Creator函数中t仍为左值，调用CData构造函数！！！
    CData* p3 = Creator2(str1 + str2); // 调用移动构造函数

    delete p2;
    delete p1;
    delete p3;

    return 0;
}