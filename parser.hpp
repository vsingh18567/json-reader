#ifndef PARSER_HPP
#define PARSER_HPP
#include "tokenizer.hpp"
#include <iostream>
#include <memory>
#include <unordered_map>
#include <variant>

namespace json_reader {

#define sPtr std::shared_ptr

enum class ValueType {
  STRING,
  DOUBLE,
  INT,
  OBJECT,
  BOOL,
  ARRAY,
};

struct Value;

struct Object {
private:
  int depth = 0;

public:
  std::unordered_map<std::string, Value> elements;
  std::string to_str() const;

  Object() {}
  Object(int depth) : depth(depth) {}
  friend std::ostream &operator<<(std::ostream &os, const Object &jo);

  template <typename T> inline void insert(const std::string key, T val);

  Value &operator[](const std::string key);

  template <typename T> sPtr<T> get(const std::string &key);
  template <typename T>
  sPtr<T> get(const std::string &key, const T default_val);
  template <typename T> sPtr<T> try_get(const std::string &key);
};

struct Array {
private:
  int depth;

public:
  std::vector<Value> elements;
  std::string to_str() const;
  Array(int depth) : depth(depth) {}
  Value &operator[](int idx) { return elements[idx]; }
  friend std::ostream &operator<<(std::ostream &os, const Array &ja);
};

using val_t = std::variant<sPtr<int>, sPtr<double>, sPtr<bool>,
                           sPtr<std::string>, sPtr<Object>, sPtr<Array>>;

struct Value {
  ValueType type;
  val_t value;
  Value() {}
  Value(int value)
      : type(ValueType::INT), value(std::make_shared<int>(value)) {}
  Value(double value)
      : type(ValueType::DOUBLE), value(std::make_shared<double>(value)) {}
  Value(bool value)
      : type(ValueType::BOOL), value(std::make_shared<bool>(value)) {}
  Value(std::string value)
      : type(ValueType::STRING), value(std::make_shared<std::string>(value)) {}
  Value(Object value)
      : type(ValueType::OBJECT), value(std::make_shared<Object>(value)) {}
  Value(Array value)
      : type(ValueType::ARRAY), value(std::make_shared<Array>(value)) {}
  Value(ValueType t, val_t value) : type(t), value(value) {}
  std::string to_str() const {
    switch (type) {
    case ValueType::STRING:
      return "\"" + *std::get<sPtr<std::string>>(value) + "\"";
    case ValueType::DOUBLE:
      return std::to_string(*std::get<sPtr<double>>(value));
    case ValueType::OBJECT:
      return std::get<sPtr<Object>>(value)->to_str();
    case ValueType::BOOL:
      return std::get<sPtr<bool>>(value) ? "true" : "false";
    case ValueType::ARRAY:
      return std::get<sPtr<Array>>(value)->to_str();
    case ValueType::INT:
      return std::to_string(*std::get<sPtr<int>>(value));
    }
  }

  template <typename T> sPtr<T> cast() { return std::get<sPtr<T>>(value); }

  template <typename T> sPtr<T> try_cast() {
    if (std::get_if<sPtr<T>>(value)) {
      return std::get<sPtr<T>>(value);
    } else {
      return nullptr;
    }
  }

  Value &operator[](const std::string key) {
    return std::get<sPtr<Object>>(value)->operator[](key);
  }

  friend std::ostream &operator<<(std::ostream &os, const Value &jv);
};

struct JSON {

  Object root;
  std::string to_str() const { return root.to_str(); }

  friend std::ostream &operator<<(std::ostream &os, const JSON &json);
  Value &operator[](const std::string key) { return root[key]; }
};

class Parser {
private:
  Tokenizer &tokenizer;
  int idx = 0;
  int depth = 0;
  JSON object;
  Object expect_object();
  Value expect_value();
  Array expect_array();

public:
  Parser(Tokenizer &tokenizer) : tokenizer(tokenizer) {}
  JSON parse();
};

#include "parser.tpp"
#endif

} // namespace json_reader
