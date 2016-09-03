#include "json.h"

std::shared_ptr<JSON::JSONBase> JSON::invalidBase;

std::shared_ptr<JSON::JSONBase> JSON::JSONBase::operator[](std::size_t index) {
    return invalidBase;
}

std::shared_ptr<JSON::JSONBase> JSON::JSONBase::operator[](std::string index) {
    return invalidBase;
}

bool JSON::JSONBase::for_each(std::function<void(JSON)> f) const {
    return false;
}

bool JSON::JSONBase::for_each(std::function<void(std::string, JSON)> f) const {
    return false;
}

std::optional<double> JSON::JSONBase::get_num() {
    return std::optional<double>();
}

std::optional<std::string> JSON::JSONBase::get_str() {
    return std::optional<std::string>();
}

std::string JSON::JSONBase::to_string() const {
    return "null";
}

std::shared_ptr<JSON::JSONBase> JSON::JSONArray::operator[](std::size_t index) {
    if (index < array.size())
        return array[index];
    else
        return invalidBase;
}

bool JSON::JSONArray::for_each(std::function<void(JSON)> f) const {
    for (const auto& i : array) {
        f(i);
    }
    return true;
}

std::string JSON::JSONArray::to_string() const {
    bool first = true;
    std::string s("[");
    for (const auto& i : array) {
        if (!first) {
            s.append(",");
        }
        first = false;
        s.append(i->to_string());
    }
    s.append("]");
    return s;
}

std::shared_ptr<JSON::JSONBase> JSON::JSONMap::operator[](std::string index) {
    auto itr = map.find(index);
    if (itr != map.end())
        return itr->second;
    else
        return invalidBase;
}

bool JSON::JSONMap::for_each(std::function<void(std::string, JSON)> f) const {
    for (const auto& i : map) {
        f(i.first, i.second);
    }
    return true;
}

std::string JSON::JSONMap::to_string() const {
    bool first = true;
    std::string s("[");
    for (const auto& pair : map) {
        if (!first) {
            s.append(",");
        }
        first = false;
        s.append('"' + pair.first + "\":" + pair.second->to_string());
    }
    s.append("}");
    return s;
}

std::optional<double> JSON::JSONNumber::get_num() {
    return std::optional<double>(number);
}

std::string JSON::JSONNumber::to_string() const {
    return std::to_string(number);
}

std::optional<std::string> JSON::JSONString::get_str() {
    return std::optional<std::string>(string);
}

std::string JSON::JSONString::to_string() const {
    return '"' + string + '"';
}

JSON JSON::operator[](std::size_t index) {
    return JSON((*impl)[index]);
}

JSON JSON::operator[](std::string index) {
    return JSON((*impl)[index]);
}

std::optional<double> JSON::get_num() {
    return impl->get_num();
}

std::optional<std::string> JSON::get_str() {
    return impl->get_str();
}

std::string JSON::to_string() const {
    return impl->to_string();
}
