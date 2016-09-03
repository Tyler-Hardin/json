#ifndef JSON_H
#define JSON_H

#include <map>
#include <memory>
#include <vector>

#include <experimental/optional>

#include <iostream>

namespace std {
    using std::experimental::optional;
    using std::experimental::make_optional;
}

class JSON {
protected:
    class JSONBase {
    public:
        virtual ~JSONBase() = default;
        virtual std::shared_ptr<JSONBase> operator[](std::size_t index);
        virtual std::shared_ptr<JSONBase> operator[](std::string index);

        void for_each(std::function<void(JSON)> f);
        void for_each(std::function<void(std::string, JSON)> f);

        virtual std::optional<double> get_num();
        virtual std::optional<std::string> get_str();

        virtual std::string to_string() const;
    };

    class JSONArray : public JSONBase {
        std::vector<std::shared_ptr<JSON::JSONBase>> array;

    public:
        JSONArray() = default;
        JSONArray(const std::vector<std::shared_ptr<JSON::JSONBase>>& array) : array(array) {}
        virtual std::shared_ptr<JSONBase> operator[](std::size_t index) override;
        static std::shared_ptr<JSONBase> parse(const char*&);
        virtual std::string to_string() const override;
    };

    struct JSONMap : public JSONBase {
        std::map<std::string, std::shared_ptr<JSONBase>> map;

    public:
        JSONMap() = default;
        JSONMap(const std::map<std::string, std::shared_ptr<JSONBase>>& map) : map(map) {}
        virtual std::shared_ptr<JSONBase> operator[](std::string index) override;
        static std::shared_ptr<JSONBase> parse(const char*&);
        virtual std::string to_string() const override;
    };

    struct JSONNumber : public JSONBase {
        double number;

    public:
        JSONNumber() = default;
        JSONNumber(double number) : number(number) {}
        virtual std::optional<double> get_num() override;
        static std::shared_ptr<JSONBase> parse(const char*&);
        virtual std::string to_string() const override;
    };

    struct JSONString : public JSONBase {
        std::string string;

    public:
        JSONString() = default;
        JSONString(const std::string& string) : string(string) {}
        virtual std::optional<std::string> get_str() override;
        static std::shared_ptr<JSONBase> parse(const char*&);
        virtual std::string to_string() const override;
    };

    JSON(std::shared_ptr<JSONBase> impl) : impl(impl) {}

    static std::shared_ptr<JSONBase> parse(const char*& i);

    static std::shared_ptr<JSONBase> invalidBase;

private:
    std::shared_ptr<JSONBase> impl;

public:
    JSON() : impl(nullptr) {}

    JSON operator[](std::size_t index);
    JSON operator[](std::string index);
    std::optional<double> get_num();
    std::optional<std::string> get_str();

    std::string to_string() const;

    static JSON parse(std::string);

    static JSON to_json(double d) {
        return std::shared_ptr<JSONBase>(new JSONNumber(d));
    }

    static JSON to_json(const std::string& s) {
        return std::shared_ptr<JSONBase>(new JSONString(s));
    }

    template<typename T>
    static JSON to_json(const std::vector<T>& v) {
        std::vector<std::shared_ptr<JSONBase>> array;
        for (const auto& i : v) {
            array.emplace_back(std::move(to_json(i).impl));
        }
        return std::shared_ptr<JSONBase>(new JSONArray(array));
    }

    template<typename T>
    static JSON to_json(const std::map<std::string, T>& m) {
        std::map<std::string, std::shared_ptr<JSONBase>> map;
        for (const auto& p : m) {
            map.emplace(p.first, std::move(to_json(p.second).impl));
        }
        return std::shared_ptr<JSONBase>(new JSONMap(map));
    }
};

#endif
