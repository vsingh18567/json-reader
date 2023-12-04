#include "parser.hpp"
#include <optional>

namespace json_reader {
static std::optional<JSON> read_json(const std::string &s) {
  Tokenizer tokenizer(s);
  if (!tokenizer.succeeded()) {
    std::cout << "Failed to tokenize" << std::endl;
    return std::nullopt;
  }
  Parser parser(tokenizer);
  return parser.parse();
}

static std::optional<JSON> read_json(std::ifstream &ifs) {
  std::string s((std::istreambuf_iterator<char>(ifs)),
                (std::istreambuf_iterator<char>()));
  return read_json(s);
}

} // namespace json_reader
