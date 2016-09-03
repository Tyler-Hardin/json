#include "json.h"

#include <cassert>
#include <iostream>

void test_parse() {
    JSON json = JSON::parse(
    "{"
        "\"a\" :\"A\", "
        "\"b\" : \"B\"   , \n"
        "\"c\": 57"
        "  \n, \"d\" : ["
        "    5,6, 7,\"zte\\\\ \"]"
    "}");
    assert(json["a"].get_str().value_or("") == "A");
    assert(json["b"].get_str().value_or("") == "B");
    assert(json["c"].get_num().value_or(0) == 57);
    assert(json["d"][0].get_num().value_or(0) == 5);
    assert(json["d"][3].get_str().value_or("") == "zte\\ ");
}

void test_serialize() {
    char *c[] = {"ABC", "DEF", "HGI", "XYZ" };
    std::map<std::string, std::vector<double>> m;

    int i = 0;
    for (auto s : c) {
        std::vector<double> v;
        for (int j = 0;j < i;j++)
            v.push_back(i);
        i++;
        m.emplace(std::string(s), v);
    }

    std::cout << JSON::to_json(m).to_string() << std::endl;
}

int main() {
    test_parse();
    test_serialize();
    return 0;
}
