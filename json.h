#ifndef JSON_H
#define JSON_H

#include <map>
#include <memory>
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

    void for_each(std::function<void(JSON)> f);
    void for_each(std::function<void(std::string, JSON)> f);

    virtual std::optional<double> to_num();
    virtual std::optional<std::string> to_str();

    static JSON parse(std::string);

protected:
    class JSONBase {
    public:
        virtual ~JSONBase() = default;
        virtual std::shared_ptr<JSONBase> operator[](std::size_t index);
        virtual std::shared_ptr<JSONBase> operator[](std::string index);

        void for_each(std::function<void(JSON)> f);
        void for_each(std::function<void(std::string, JSON)> f);

        virtual std::optional<double> to_num();
        virtual std::optional<std::string> to_str();
    };

    class JSONArray : public JSONBase {
        std::vector<std::shared_ptr<JSON::JSONBase>> array;

    public:
        virtual std::shared_ptr<JSONBase> operator[](std::size_t index) override;
        static std::shared_ptr<JSONBase> parse(const char*&);
    };

    struct JSONMap : public JSONBase {
        std::map<std::string, std::shared_ptr<JSONBase>> map;

    public:
        virtual std::shared_ptr<JSONBase> operator[](std::string index) override;
        static std::shared_ptr<JSONBase> parse(const char*&);
    };

    struct JSONNumber : public JSONBase {
        double number;

    public:
        virtual std::optional<double> to_num() override;
        static std::shared_ptr<JSONBase> parse(const char*&);
    };

    struct JSONString : public JSONBase {
        std::string string;

    public:
        virtual std::optional<std::string> to_str() override;
        static std::shared_ptr<JSONBase> parse(const char*&);
    };

    JSON(std::shared_ptr<JSONBase> impl) : impl(impl) {}

    static std::shared_ptr<JSONBase> parse(const char*& i);

    static std::shared_ptr<JSONBase> invalidBase;

private:
    std::shared_ptr<JSONBase> impl;
};

#endif
