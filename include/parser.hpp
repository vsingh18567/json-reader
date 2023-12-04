#include "tokenizer.hpp"
#include <memory>
#include <unordered_map>
#include <variant>

namespace json_reader {

#define uPtr std::unique_ptr
#define mk_uPtr std::make_unique

enum class ValueType {
  STRING,
  INT,
  OBJECT,
  BOOL,
};

struct Value;

struct Object {
  int depth = 0;
  std::unordered_map<std::string, Value> elements;
  std::string to_str() const;

  Object() {}
  Object(int depth) : depth(depth) {}
  friend std::ostream &operator<<(std::ostream &os, const Object &jo);
};

using val_t = std::variant<int, bool, std::string, Object>;

struct Value {
  ValueType type;
  val_t value;
  Value() {}
  Value(ValueType t, val_t value) : type(t), value(value) {}
  std::string to_str() const {
    switch (type) {
    case ValueType::STRING:
      return "\"" + std::get<std::string>(value) + "\"";
    case ValueType::INT:
      return std::to_string(std::get<int>(value));
    case ValueType::OBJECT:
      return std::get<Object>(value).to_str();
    case ValueType::BOOL:
      return std::get<bool>(value) ? "true" : "false";
    }
  }
  friend std::ostream &operator<<(std::ostream &os, const Value &jv);
};

struct JSON {

  Object root;
  std::string to_str() const { return root.to_str(); }

  friend std::ostream &operator<<(std::ostream &os, const JSON &json);
};

class Parser {
private:
  json_reader::Tokenizer &tokenizer;
  int idx = 0;
  int depth = 0;
  JSON object;
  Object expect_object();
  Value expect_value();

public:
  Parser(json_reader::Tokenizer &tokenizer) : tokenizer(tokenizer) {}
  JSON parse();
};
} // namespace json_reader
