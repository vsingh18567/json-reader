

Object Parser::expect_object() {
  if (tokenizer.tokens().size() == 0) {
    throw std::runtime_error("Error: empty input");
  }
  auto &token = tokenizer[idx];
  if (token.type != TokenType::OPEN_BRACE) {
    throw std::runtime_error("Error: expected '{' at line " +
                             std::to_string(token.line) + ", col " +
                             std::to_string(token.col));
  }

  idx++;
  Object obj(depth);
  while (idx < tokenizer.tokens().size()) {
    auto &token = tokenizer[idx];

    if (token.type == TokenType::CLOSE_BRACE) {
      idx++;
      return obj;
    } else if (token.type == TokenType::STRING) {
      std::string key = token.s_val;
      idx++;
      if (tokenizer[idx].type != TokenType::COLON) {
        throw std::runtime_error("Error: expected ':' at line " +
                                 std::to_string(token.line) + ", col " +
                                 std::to_string(token.col));
      }
      idx++;
      auto val = expect_value();
      obj.elements.insert({key, val});
      if (tokenizer[idx].type == TokenType::COMMA) {
        idx++;
      }
    } else {
      throw std::runtime_error("Error: expected STRING at line " +
                               std::to_string(token.line) + ", col " +
                               std::to_string(token.col));
    }
  }
  throw std::runtime_error("Error: expected '}' at line " +
                           std::to_string(token.line) + ", col " +
                           std::to_string(token.col));
};

Value Parser::expect_value() {

  if (tokenizer.tokens().size() == 0) {
    throw std::runtime_error("Error: empty input");
  }
  auto &token = tokenizer[idx];
  Value val;
  switch (token.type) {
  case TokenType::STRING:
    idx++;
    return Value(ValueType::STRING, std::make_shared<std::string>(token.s_val));
  case TokenType::DOUBLE:
    idx++;
    return Value(ValueType::DOUBLE, std::make_shared<double>(token.double_val));
  case TokenType::INT:
    idx++;
    return Value(ValueType::INT, std::make_shared<int>(token.int_val));
  case TokenType::OPEN_BRACE:
    depth += 4;
    val = Value(ValueType::OBJECT, std::make_shared<Object>(expect_object()));
    depth -= 4;
    return val;
  case TokenType::TRUE:
    idx++;
    return Value(ValueType::BOOL, std::make_shared<bool>(true));
  case TokenType::FALSE:
    idx++;
    return Value(ValueType::BOOL, std::make_shared<bool>(false));
  case TokenType::LEFT_BRACKET:
    depth += 4;
    val = Value(ValueType::ARRAY, std::make_shared<Array>(expect_array()));
    depth -= 4;
    return val;
  default:
    throw std::runtime_error("Error: expected value at line " +
                             std::to_string(token.line) + ", col " +
                             std::to_string(token.col));
  }
}

Array Parser::expect_array() {
  if (tokenizer.tokens().size() == 0) {
    throw std::runtime_error("Error: empty input");
  }
  auto &token = tokenizer[idx];
  if (token.type != TokenType::LEFT_BRACKET) {
    throw std::runtime_error("Error: expected '[' at line " +
                             std::to_string(token.line) + ", col " +
                             std::to_string(token.col));
  }

  idx++;
  Array arr(depth);
  while (idx < tokenizer.tokens().size()) {
    auto &token = tokenizer[idx];

    if (token.type == TokenType::RIGHT_BRACKET) {
      idx++;
      return arr;
    } else {
      arr.elements.push_back(expect_value());
      if (tokenizer[idx].type == TokenType::COMMA) {
        idx++;
      }
    }
  }
  throw std::runtime_error("Error: expected ']' at line " +
                           std::to_string(token.line) + ", col " +
                           std::to_string(token.col));
}

JSON Parser::parse() {
  object.root = expect_object();
  return object;
}

std::string Object::to_str() const {
  std::string s = "{\n";
  for (auto &[key, val] : elements) {
    s += std::string(depth, ' ') + "\"" + key + "\": " + val.to_str() + ", \n";
  }
  s.pop_back();
  s.pop_back();
  s.pop_back();

  s += "\n" + std::string(std::max(depth - 4, 0), ' ') + "}";
  return s;
}

std::string Array::to_str() const {
  std::string s = "[\n";
  for (auto &val : elements) {
    s += std::string(depth, ' ') + val.to_str() + ", \n";
  }
  s.pop_back();
  s.pop_back();
  s.pop_back();

  s += "\n" + std::string(std::max(depth - 4, 0), ' ') + "]";
  return s;
}

std::ostream &operator<<(std::ostream &os, const Object &jo) {
  os << jo.to_str();
  return os;
}

std::ostream &operator<<(std::ostream &os, const Value &jv) {
  os << jv.to_str();
  return os;
}

std::ostream &operator<<(std::ostream &os, const JSON &json) {
  os << json.to_str();
  return os;
}

std::ostream &operator<<(std::ostream &os, const Array &ja) {
  os << ja.to_str();
  return os;
}

Value &Object::operator[](const std::string key) { return elements[key]; }

template <typename T> sPtr<T> Object::get(const std::string &key) const {
  if (std::get_if<sPtr<T>>(&elements.at(key).value)) {
    return std::get<sPtr<T>>(elements.at(key).value);
  }
  throw std::runtime_error("Error: expected type " +
                           std::string(typeid(T).name()) + " at key " + key);
}

template <typename T>
sPtr<T> Object::get(const std::string &key, const T default_val) const {
  if (elements.find(key) == elements.end()) {
    return default_val;
  }
  if (std::get_if<sPtr<T>>(&elements.at(key).value)) {
    return std::get<sPtr<T>>(elements.at(key).value);
  } else {
    return default_val;
  }
}

template <typename T> sPtr<T> Object::try_get(const std::string &key) const {
  if (elements.find(key) == elements.end()) {
    return nullptr;
  }
  if (std::get_if<sPtr<T>>(&elements.at(key).value)) {
    return std::get<sPtr<T>>(elements.at(key).value);
  } else {
    return nullptr;
  }
}

template <typename T> void Object::insert(const std::string key, T val) {
  elements.insert({key, Value(val)});
}

template <> void Object::insert(const std::string key, Value val) {
  elements.insert({key, val});
}

template <> void Object::insert(const std::string key, Object val) {
  val.depth = depth + 4;
  elements.insert({key, Value(val)});
}

std::string Value::to_str() const {
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
