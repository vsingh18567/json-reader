#include "tokenizer.hpp"
#include <memory>
#include <variant>
#include <unordered_map>


#define uPtr std::unique_ptr
#define mk_uPtr std::make_unique
enum class ValueType {
    STRING,
    INT,
    OBJECT,
    BOOL,
};

struct JSONValue;

struct JSONObject {
    int depth = 0;
    std::unordered_map<std::string, JSONValue> elements;
    std::string to_str() const; 

    JSONObject() {}
    JSONObject(int depth) : depth(depth) {}
    friend std::ostream& operator<<(std::ostream& os, const JSONObject& jo);
};

using val_t = std::variant<int, bool, std::string, JSONObject>;


struct JSONValue {
    ValueType type;
    val_t value;
    JSONValue() {}
    JSONValue(ValueType t, val_t value ) : type(t), value(value) {}
    std::string to_str() const {
        switch (type) {
            case ValueType::STRING:
                return "\"" + std::get<std::string>(value) + "\"";
            case ValueType::INT:
                return std::to_string(std::get<int>(value));
            case ValueType::OBJECT:
                return std::get<JSONObject>(value).to_str();
            case ValueType::BOOL:
                return std::get<bool>(value) ? "true" : "false";
        }
    
    }
    friend std::ostream& operator<<(std::ostream& os, const JSONValue& jv);

};


struct JSON {

    JSONObject root;    
    std::string to_str() const {
        return root.to_str();
    }

    friend std::ostream& operator<<(std::ostream& os, const JSON& json);

};

class Parser {
private:
    Tokenizer& tokenizer;
    int idx = 0;
    int depth = 0;
    JSON object;
    JSONObject expect_object();
    JSONValue expect_value();
    
public:
    Parser(Tokenizer& tokenizer) : tokenizer(tokenizer) { }
    JSON parse();
};