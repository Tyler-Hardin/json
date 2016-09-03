#include "json.h"

JSON::JSONBase JSON::invalidBase;

JSON::JSONBase* JSON::JSONBase::operator[](std::size_t index) {
    return this;
}

JSON::JSONBase* JSON::JSONBase::operator[](std::string index) {
    return this;
}

std::optional<double> JSON::JSONBase::to_num() {
    return std::optional<double>();
}

std::optional<std::string> JSON::JSONBase::to_str() {
    return std::optional<std::string>();
}

JSON::JSONBase* JSON::JSONArray::operator[](std::size_t index) {
    if (index < array.size())
        return array[index];
    else
        return &invalidBase;
}

JSON::JSONBase* JSON::JSONMap::operator[](std::string index) {
    auto itr = map.find(index);
    if (itr != map.end())
        return itr->second;
    else
        return &invalidBase;
}

std::optional<double> JSON::JSONNumber::to_num() {
    return std::optional<double>(number);
}

std::optional<std::string> JSON::JSONString::to_str() {
    return std::optional<std::string>(string);
}

JSON JSON::operator[](std::size_t index) {
    return JSON((*impl)[index]);
}

JSON JSON::operator[](std::string index) {
    return JSON((*impl)[index]);
}

std::optional<double> JSON::to_num() {
    return impl->to_num();
}

std::optional<std::string> JSON::to_str() {
    return impl->to_str();
}
