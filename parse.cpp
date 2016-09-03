#include "json.h"

#include <cassert>
#include <set>

#include <iostream>

// TODO Currently, all syntax checking is done with asserts. Should probably make some sort of error reporting,
// but I'm not sure if exceptions are the way to do it. Maybe something else, like Rust's std::result?

static void skip_whitespace(const char*& i) {
    bool skip = true;
    do {
        switch (*i) {
        case ' ':
        case '\t':
        case '\f':
        case '\n':
            i++;
            break;
        default:
            skip = false;
        }
    } while(skip);
}

static char parse_escape(const char*& i) {
    switch (i[1]) {
    case '\\':
    case '"':
        i += 2;
        return i[-1];
    default:
        // TODO hex/octal
        assert(false);
        return '\0';
    }
}

static std::string parse_string(const char*& i) {
    std::string ret;
    assert(*i == '"');
    i++;
    while (*i != '"' && *i != '\0') {
        if (*i != '\\') {
            ret.push_back(*i);
            i++;
        } else {
            ret.push_back(parse_escape(i));
        }
    }
    assert(*i == '"');
    i++;
    return ret;
}

JSON JSON::parse(std::string s) {
    const char* i = s.c_str();
    return JSON(parse(i));
}

std::shared_ptr<JSON::JSONBase> JSON::parse(const char*& i) {
    skip_whitespace(i);

    switch (*i) {
    case '[':
        return JSONArray::parse(i);
    case '{':
        return JSONMap::parse(i);
    case '"':
        return JSONString::parse(i);
    default:
        if (*i >= '1' && *i <= '9')
            return JSONNumber::parse(i);
        else {
            std::cerr << *i << std::endl;
            assert(false);
            return invalidBase;
        }
    }
}

std::shared_ptr<JSON::JSONBase> JSON::JSONArray::parse(const char*& i) {
    JSON::JSONArray* ret = new JSON::JSONArray;
    assert(*i == '[');
    i++;
    while (*i != ']' && *i != '\0') {
        skip_whitespace(i);
        ret->array.push_back(JSON::parse(i));
        skip_whitespace(i);
        assert(*i == ',' || *i == ']');
        if (*i == ',')
            i++;
    }
    assert(*i == ']');
    i++;
    return std::shared_ptr<JSON::JSONBase>(ret);
}

std::shared_ptr<JSON::JSONBase> JSON::JSONMap::parse(const char*& i) {
    JSON::JSONMap* ret = new JSON::JSONMap;
    assert(*i == '{');
    i++;
    while (*i != '}' && i != '\0') {
        std::string key;
        std::shared_ptr<JSONBase> val;

        skip_whitespace(i);
        assert(*i == '"');
        key = parse_string(i);

        skip_whitespace(i);
        assert(*i == ':');
        i++;

        val = JSON::parse(i);
        skip_whitespace(i);

        if (*i == ',')
            i++;
        else if (*i != '}') {
            assert(false);
        }

        ret->map.emplace(key, val);
    }
    assert(*i == '}');
    i++;
    return std::shared_ptr<JSON::JSONBase>(ret);
}

std::shared_ptr<JSON::JSONBase> JSON::JSONNumber::parse(const char*& i) {
    JSON::JSONNumber* ret = new JSON::JSONNumber;
    std::string buffer;
    while (*i >= '0' && *i <= '9') {
        buffer.push_back(*i);
        i++;
    }
    ret->number = stod(buffer);
    return std::shared_ptr<JSON::JSONBase>(ret);
}

std::shared_ptr<JSON::JSONBase> JSON::JSONString::parse(const char*& i) {
    JSON::JSONString* ret = new JSON::JSONString;
    assert(*i == '"');
    ret->string = parse_string(i);
    return std::shared_ptr<JSON::JSONBase>(ret);
}
