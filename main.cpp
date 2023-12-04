#include <iostream>
#include <fstream>
#include "json_reader.hpp"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <json file>" << std::endl;
        return 1;
    }

    std::ifstream ifs(argv[1]);
    
    auto js = JSONReader::read(ifs);
    if (!js) {
        std::cout << "Failed to parse" << std::endl;
        return 1;
    }
    std::cout << js.value() << std::endl;
    

}