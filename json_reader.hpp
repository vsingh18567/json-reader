#include "parser.hpp"
#include <optional>

class JSONReader {
public:
    static std::optional<JSON> read(const std::string& s) {
        Tokenizer tokenizer(s);
        if (!tokenizer.succeeded()) {
            std::cout << "Failed to tokenize" << std::endl;
            return std::nullopt;
        }
        Parser parser(tokenizer);
        return parser.parse();
    }

    static std::optional<JSON> read(std::ifstream& ifs) {
        std::string s((std::istreambuf_iterator<char>(ifs)),
                      (std::istreambuf_iterator<char>()));
        return read(s);
    }

};