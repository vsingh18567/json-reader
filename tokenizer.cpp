#include "tokenizer.hpp"
#include <iostream>

std::string Token::to_str() {
  switch (type) {
  case TokenType::OPEN_BRACE:
    return "{";
  case TokenType::CLOSE_BRACE:
    return "}";
  case TokenType::STRING:
    return "STRING: " + s_val;
  case TokenType::NUMBER:
    return "NUMBER: " + std::to_string(int_val);
  case TokenType::COLON:
    return ":";
  case TokenType::COMMA:
    return ",";
  case TokenType::TRUE:
    return "TRUE";
  case TokenType::FALSE:
    return "FALSE";
  case TokenType::LEFT_BRACKET:
    return "[";
  case TokenType::RIGHT_BRACKET:
    return "]";
  }
}

Tokenizer::Tokenizer(std::string s) : s(s) { tokenize(); }

bool Tokenizer::succeeded() { return _succeeded; }

std::vector<Token> &Tokenizer::tokens() { return _tokens; }

void Tokenizer::tokenize() {
  int idx = 0;
  int line = 1;
  int col = 1;
  while (idx < s.length()) {
    if (s.substr(idx, 4) == "true") {
      _tokens.push_back(Token(TokenType::TRUE, line, col));
      idx += 4;
      col += 4;
    } else if (s.substr(idx, 5) == "false") {
      _tokens.push_back(Token(TokenType::FALSE, line, col));
      idx += 5;
      col += 5;
    } else if (s[idx] == '{') {
      _tokens.push_back(Token(TokenType::OPEN_BRACE, line, col));
      idx++;
      col++;
    } else if (s[idx] == '}') {
      _tokens.push_back(Token(TokenType::CLOSE_BRACE, line, col));
      idx++;
      col++;
    } else if (s[idx] == '[') {
      _tokens.push_back(Token(TokenType::LEFT_BRACKET, line, col));
      idx++;
      col++;
    } else if (s[idx] == ']') {
      _tokens.push_back(Token(TokenType::RIGHT_BRACKET, line, col));
      idx++;
      col++;
    } else if (s[idx] == '"') {
      int start = idx + 1;
      int end = start;
      while (end < s.length() && s[end] != '"') {
        end++;
        col++;
      }
      if (end == s.length()) {
        _succeeded = false;
        return;
      }
      _tokens.push_back(
          Token(TokenType::STRING, s.substr(start, end - start), line, col));
      idx = end + 1;
      col++;
    } else if (s[idx] == ':') {
      _tokens.push_back(Token(TokenType::COLON, line, col));
      idx++;
      col++;
    } else if (s[idx] == ',') {
      _tokens.push_back(Token(TokenType::COMMA, line, col));
      idx++;
      col++;
    } else if ((s[idx] >= '0' && s[idx] <= '9') || s[idx] == '-') {
      int start = idx;
      int end = start;
      bool found_decimal = false;
      while (end < s.length() && ((s[end] >= '0' && s[end] <= '9') ||
                                  (s[end] == '-' && end == start) ||
                                  (s[end] == '.' && !found_decimal))) {

        if (s[end] == '.') {
          found_decimal = true;
        }

        end++;
        col++;
      }
      _tokens.push_back(Token(TokenType::NUMBER,
                              std::stoi(s.substr(start, end - start)), line,
                              col));
      idx = end;
    } else if (s[idx] == '\n') {
      line++;
      col = 1;
      idx++;
    } else if (s[idx] == ' ' || s[idx] == '\t') {
      idx++;
    } else {
      _succeeded = false;
      std::cout << "Error: unexpected character '" << s[idx] << "' at line "
                << line << ", col " << col << std::endl;
      return;
    }
  }
  _succeeded = true;
}

void Tokenizer::print_tokens() {
  for (Token t : _tokens) {
    std::cout << t.to_str() << std::endl;
  }
}