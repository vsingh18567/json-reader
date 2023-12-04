#include "parser.hpp"
#include <iostream>



JSONObject Parser::expect_object() {
    if (tokenizer.tokens().size() == 0) {
        std::cout << "Error: empty input" << std::endl;
        exit(1);
    }
    auto& token = tokenizer[idx];
    if (token.type != TokenType::OPEN_BRACE) {
        std::cout << "Error: expected '{' at line " << token.line << ", col " << token.col << std::endl;
        exit(1);
    }

    idx++;
    JSONObject obj;
    while (idx < tokenizer.tokens().size()) {
        std::cout << "idx: " << idx << std::endl;
        auto& token = tokenizer[idx];
        std::cout << "token: " << token.to_str() << std::endl;

        if (token.type == TokenType::CLOSE_BRACE) {
            std::cout << "found close brace" << std::endl;
            idx++;
            return obj;
        } else if (token.type == TokenType::STRING) {
            std::string key = token.s_val;
            idx++;
            if (tokenizer[idx].type != TokenType::COLON) {
                std::cout << "Error: expected ':' at line " << token.line << ", col " << token.col << std::endl;
                exit(1);
            }
            idx++;
            auto val = expect_value();
            obj.elements.insert({key, val});
            if (tokenizer[idx].type == TokenType::COMMA) {
                idx++;
            }
        } else {
            std::cout << "Error: expected STRING at line " << token.line << ", col " << token.col << std::endl;
            exit(1);
        }
    }
    std::cout << "Error: expected '}' at line " << token.line << ", col " << token.col << std::endl;
    exit(1);
    
};


JSONValue Parser::expect_value() {

    if (tokenizer.tokens().size() == 0) {
        std::cout << "Error: empty input" << std::endl;
        exit(1);
    }
    auto& token = tokenizer[idx];
    if (token.type == TokenType::STRING) {
        idx++;
        return JSONValue(ValueType::STRING, token.s_val);
    } else if (token.type == TokenType::INT) {
        idx++;
        return JSONValue(ValueType::INT, token.int_val);
    } else if (token.type == TokenType::OPEN_BRACE) {
        return JSONValue(ValueType::OBJECT, expect_object());
    } else if (token.type == TokenType::TRUE) {
        idx++;
        return JSONValue(ValueType::BOOL, true );
    } else if (token.type == TokenType::FALSE) {
        idx++;
        return JSONValue(ValueType::BOOL, false );
    } else {
        std::cout << "Error: expected value at line " << token.line << ", col " << token.col << std::endl;
        exit(1);
    }
}

JSON Parser::parse() {
    object.root = expect_object();
    return object;
}

std::string JSONObject::to_str() {
        std::string s = "{";
        for (auto& [key, val] : elements) {
            s += "\"" + key + "\": " + val.to_str() + ", \n";
        }
        s.pop_back();
        s.pop_back();
        s.pop_back();
        s += "\n}";
        return s;
}

/*



STATES:
- waiting for {
- waiting for key
- waiting for :
- waiting for value


*/