# json

This project is licensed under the terms of the MIT license.

There are no build dependencies, but g++ 6.0+ is required for std::optional.

# Example usage: parsing

    JSON json = JSON::parse(
    "{"
        "\"a\" : \"A\", "
        "\"b\" : \"B\", "
        "\"c\" : 57"
        "\"d\" : ["
        "    5,6, 7,\"z\"]"
    "}");
    json["a"].get_str().value_or("")    // == "A"
    json["b"].get_str().value_or("")    // == "B"
    json["c"].get_num().value_or(0)     // == 57
    json["d"][0].get_num().value_or(0)  // ==  5
    json["d"][3].get_str().value_or("") // == "z"


To convert from C++-objects to JSON objects, use the `to_json` static method. It works with double, string, vector, map, and any nesting of those. (Maps must have std::string keys.)

From JSON to string, use the `to_string` method.

See test.cpp for explicit examples of `to_json` and `to_string`.
