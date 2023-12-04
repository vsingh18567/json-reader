#ifndef PARSER_HPP
#define PARSER_HPP
#include "tokenizer.hpp"
#include <iostream>
#include <memory>
#include <unordered_map>
#include <variant>

enum class ValueType {
  STRING,
  INT,
  OBJECT,
  BOOL,
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

  void insert(const std::string key, const Value val);

  Value &operator[](const std::string key);

  template <typename T> T &get(const std::string &key);

  template <typename T> T &get(const std::string &key, const T default_val);

  template <typename T> std::optional<T &> try_get(const std::string &key);
};

using val_t = std::variant<int, bool, std::string, Object>;

struct Value {
  ValueType type;
  val_t value;
  Value() {}
  Value(int value) : type(ValueType::INT), value(value) {}
  Value(bool value) : type(ValueType::BOOL), value(value) {}
  Value(std::string value) : type(ValueType::STRING), value(value) {}
  Value(Object value) : type(ValueType::OBJECT), value(value) {}
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

  template <typename T> T &cast() { return std::get<T>(value); }

  template <typename T> std::optional<T> &try_cast() {
    if (const T *val = std::get_if<T>(value)) {
      return *val;
    } else {
      return std::nullopt;
    }
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

public:
  Parser(Tokenizer &tokenizer) : tokenizer(tokenizer) {}
  JSON parse();
};
// namespace json_reader

#include "parser.tpp"
#endif