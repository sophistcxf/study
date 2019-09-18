#include <cmath>
#include <iostream>

void logistic()
{
	float x = 10;
	static const double e = 2.718281828459;
    std::cout << 1.0 / (1 + pow(e, x)) << std::endl;
}

int main(int argc, char* argv[])
{
    logistic();
    return 0;
}
