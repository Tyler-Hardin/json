#ifndef JSON_H
#define JSON_H

#include <map>
#include <vector>

#include <experimental/optional>

namespace std {
    using std::experimental::optional;
    using std::experimental::make_optional;
}

class JSON {
public:
    JSON() : impl(nullptr) {}

    JSON operator[](std::size_t index);
    JSON operator[](std::string index);

    virtual std::optional<double> to_num();
    virtual std::optional<std::string> to_str();

protected:
    class JSONBase {
    public:
        virtual ~JSONBase() = default;
        virtual JSONBase* operator[](std::size_t index);
        virtual JSONBase* operator[](std::string index);

        virtual std::optional<double> to_num();
        virtual std::optional<std::string> to_str();
    }* impl;
    static JSONBase invalidBase;

    class JSONArray : public JSONBase {
        std::vector<JSONBase*> array;

    public:
        virtual JSONBase* operator[](std::size_t index) override;
    };

    struct JSONMap : public JSONBase {
        std::map<std::string, JSONBase*> map;

    public:
        virtual JSONBase* operator[](std::string index) override;
    };

    struct JSONNumber : public JSONBase {
        double number;

    public:
        virtual std::optional<double> to_num() override;
    };

    struct JSONString : public JSONBase {
        std::string string;

    public:
        virtual std::optional<std::string> to_str() override;
    };

    JSON(JSONBase* impl) : impl(impl) {}
};

#endif
