#include <iostream>
#include <fstream>
#include "json_reader.hpp"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <json file>" << std::endl;
        return 1;
    }

    std::ifstream ifs(argv[1]);
    std::string s((std::istreambuf_iterator<char>(ifs)),
                 (std::istreambuf_iterator<char>()));

    Tokenizer tokenizer(s);
    if (!tokenizer.succeeded()) {
        std::cout << "Failed to tokenize" << std::endl;
        return 1;
    }
    
    auto js = JSONReader::read(s);
    if (!js) {
        std::cout << "Failed to parse" << std::endl;
        return 1;
    }
    std::cout << js->to_str() << std::endl;
    

}