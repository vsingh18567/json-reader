#include "json_reader.hpp"
#include <fstream>
#include <iostream>

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " <json file>" << std::endl;
    return 1;
  }

  std::ifstream ifs(argv[1]);

  auto js = read_json(ifs).value();
  auto nested = js.root.get<Object>("nested").try_get<std::string>("nest");
  // nested.insert("number2", Value(42));
  std::cout << js << std::endl;
}
