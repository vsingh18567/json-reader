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
    JSONObject obj(depth);
    while (idx < tokenizer.tokens().size()) {
        auto& token = tokenizer[idx];

        if (token.type == TokenType::CLOSE_BRACE) {
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
        depth += 4;
        auto val = JSONValue(ValueType::OBJECT, expect_object());
        depth -= 4;
        return val;
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

std::string JSONObject::to_str() const {
        std::string s =  "{\n";
        for (auto& [key, val] : elements) {
            s += std::string(depth, ' ')  + "\"" + key + "\": " + val.to_str() + ", \n";
        }
        s.pop_back();
        s.pop_back();
        s.pop_back();
        
        s += "\n" + std::string(std::max(depth - 4, 0), ' ') + "}";
        return s;
}

std::ostream& operator<<(std::ostream& os, const JSONObject& jo) {
    os << jo.to_str();
    return os;
}


std::ostream& operator<<(std::ostream& os, const JSONValue& jv) {
    os << jv.to_str();
    return os;
}

std::ostream& operator<<(std::ostream& os, const JSON& json) {
    os << json.to_str();
    return os;
}


/*



STATES:
- waiting for {
- waiting for key
- waiting for :
- waiting for value


*/