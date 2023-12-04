
#include <string>
#include <vector>

enum class TokenType {
  OPEN_BRACE,
  CLOSE_BRACE,
  STRING,
  NUMBER,
  COLON,
  COMMA,
  TRUE,
  FALSE,
  LEFT_BRACKET,
  RIGHT_BRACKET,
};

struct Token {
  TokenType type;
  int int_val;
  std::string s_val;
  int line;
  int col;
  Token(TokenType t, int line, int col) : type(t), line(line), col(col) {}
  Token(TokenType t, std::string val, int line, int col)
      : type(t), s_val(val), line(line), col(col) {}
  Token(TokenType t, int val, int line, int col)
      : type(t), int_val(val), line(line), col(col) {}
  std::string to_str();
};

class Tokenizer {
private:
  bool _succeeded;
  std::vector<Token> _tokens;
  std::string s;
  void tokenize();

public:
  Tokenizer(std::string s);
  bool succeeded();
  std::vector<Token> &tokens();
  void print_tokens();
  Token &operator[](int idx) { return _tokens[idx]; }
};
