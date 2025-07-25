#include <iostream>
#include "simdjson.h"

void test1() {
    simdjson::dom::parser parser;
    simdjson::dom::element tweets = parser.load("twitter.json");
    std::cout << tweets["search_metadata"]["count"] << " results." << std::endl;
}

void test2() {
    simdjson::dom::parser parser;
    simdjson::dom::element tweets = parser.load("twitter.json");
    std::cout << "ID: " << tweets["statuses"].at(0)["id"] << std::endl;
}

int main() {
    test2();
}