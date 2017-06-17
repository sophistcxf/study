#include <iostream>
#include <vector>
#include <string>
#include <boost/algorithm/searching/boyer_moore.hpp>
using namespace std;

int main() {
	std::string corpus("here is a simple example");
	std::string pattern("example");

  std::string pattern1("hhhohhhoo");
	boost::algorithm::boyer_moore<std::string::const_iterator>
		search(pattern.begin(), pattern.end());
	boost::algorithm::boyer_moore<std::string::const_iterator>
		search2(pattern1.begin(), pattern1.end());

	if (corpus.end() != search(corpus.begin(), corpus.end())) {
		std::cout << "pattern found" << std::endl;
	} else {
		std::cout << "pattern found" << std::endl;
	}
	return 0;
}
