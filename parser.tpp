

Object Parser::expect_object() {
  if (tokenizer.tokens().size() == 0) {
    std::cout << "Error: empty input" << std::endl;
    exit(1);
  }
  auto &token = tokenizer[idx];
  if (token.type != TokenType::OPEN_BRACE) {
    std::cout << "Error: expected '{' at line " << token.line << ", col "
              << token.col << std::endl;
    exit(1);
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
        std::cout << "Error: expected ':' at line " << token.line << ", col "
                  << token.col << std::endl;
        exit(1);
      }
      idx++;
      auto val = expect_value();
      obj.elements.insert({key, val});
      if (tokenizer[idx].type == TokenType::COMMA) {
        idx++;
      }
    } else {
      std::cout << "Error: expected STRING at line " << token.line << ", col "
                << token.col << std::endl;
      exit(1);
    }
  }
  std::cout << "Error: expected '}' at line " << token.line << ", col "
            << token.col << std::endl;
  exit(1);
};

Value Parser::expect_value() {

  if (tokenizer.tokens().size() == 0) {
    std::cout << "Error: empty input" << std::endl;
    exit(1);
  }
  auto &token = tokenizer[idx];
  if (token.type == TokenType::STRING) {
    idx++;
    return Value(ValueType::STRING, std::make_shared<std::string>(token.s_val));
  } else if (token.type == TokenType::INT) {
    idx++;
    return Value(ValueType::INT, std::make_shared<int>(token.int_val));
  } else if (token.type == TokenType::OPEN_BRACE) {
    depth += 4;
    auto val =
        Value(ValueType::OBJECT, std::make_shared<Object>(expect_object()));
    depth -= 4;
    return val;
  } else if (token.type == TokenType::TRUE) {
    idx++;
    return Value(ValueType::BOOL, std::make_shared<bool>(true));
  } else if (token.type == TokenType::FALSE) {
    idx++;
    return Value(ValueType::BOOL, std::make_shared<bool>(false));
  } else if (token.type == TokenType::LEFT_BRACKET) {
    depth += 4;
    auto val = Value(ValueType::ARRAY, std::make_shared<Array>(expect_array()));
    depth -= 4;
    return val;
  } else {
    std::cout << "Error: expected value at line " << token.line << ", col "
              << token.col << std::endl;
    exit(1);
  }
}

Array Parser::expect_array() {
  if (tokenizer.tokens().size() == 0) {
    std::cout << "Error: empty input" << std::endl;
    exit(1);
  }
  auto &token = tokenizer[idx];
  if (token.type != TokenType::LEFT_BRACKET) {
    std::cout << "Error: expected '[' at line " << token.line << ", col "
              << token.col << std::endl;
    exit(1);
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
  std::cout << "Error: expected ']' at line " << token.line << ", col "
            << token.col << std::endl;
  exit(1);
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

template <typename T> sPtr<T> Object::get(const std::string &key) {
  if (std::get_if<sPtr<T>>(&elements.at(key).value)) {
    return std::get<sPtr<T>>(elements.at(key).value);
  }
  throw std::runtime_error("Error: expected type " +
                           std::string(typeid(T).name()) + " at key " + key);
}

template <typename T>
sPtr<T> Object::get(const std::string &key, const T default_val) {
  if (elements.find(key) == elements.end()) {
    return default_val;
  }
  if (std::get_if<sPtr<T>>(&elements.at(key).value)) {
    return std::get<sPtr<T>>(elements.at(key).value);
  } else {
    return default_val;
  }
}

template <typename T> sPtr<T> Object::try_get(const std::string &key) {
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

/*



STATES:
- waiting for {
- waiting for key
- waiting for :
- waiting for value


*/