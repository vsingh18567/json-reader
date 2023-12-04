#include "json_reader.hpp"
#include "nlohmann/nlohmann_json.hpp"
#include <chrono>
#include <fstream>
#include <iostream>
#include <tuple>

using namespace std::chrono;
#define TRIES 20

void time_taken(std::string path) {
  long long total = 0;
  for (int i = 0; i < TRIES; i++) {
    std::ifstream ifs(path);
    auto start = high_resolution_clock::now();
    auto js = read_json(ifs);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    total += duration.count();
  }
  long long average_1 = total / TRIES;
  total = 0;
  for (int i = 0; i < TRIES; i++) {
    std::ifstream ifs2(path);
    auto start = high_resolution_clock::now();
    auto j = nlohmann::json::parse(ifs2);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    total += duration.count();
  }
  long long average_2 = total / TRIES;
  std::string unit = "us";
  if (average_1 > 10000 || average_2 > 10000) {
    average_1 /= 1000;
    average_2 /= 1000;
    unit = "ms";
  }
  std::cout << "JSON Reader: " << average_1 << unit << std::endl;
  std::cout << "Nlohmann JSON: " << average_2 << unit << std::endl;
}

void benchmark() {
  std::cout << "Benchmarking..." << std::endl;
  std::cout << "400 lines:" << std::endl;
  time_taken("400.json");
  std::cout << "700 lines:" << std::endl;
  time_taken("700.json");
  std::cout << "2000 lines:" << std::endl;
  time_taken("2000.json");
  std::cout << "5000 lines" << std::endl;
  time_taken("5000.json");
  std::cout << "10000 lines" << std::endl;
  time_taken("10000.json");
}

int main(int argc, char **argv) {
  benchmark();

  std::ifstream ifs("simple.json");
  auto js = read_json(ifs).value();
  std::cout << js << std::endl;
  auto nested = js["nested"];
  auto nested_obj = nested.cast<Object>();
  std::cout << *nested_obj->get<std::string>("foo") << std::endl;

  auto nested2 = js.root.get<Object>("nested");
  std::cout << (nested2 == nested_obj) << std::endl;
  js.root.insert("new_key", Value("new_value")); // insert a new key-value pair

  Object obj;
  obj.insert("key", Value("value"));
  obj.insert("key2", Value(2));
  js.root.insert(
      "new_obj",
      obj); // insert a new key-value pair with an object as the value
  std::cout << js << std::endl;

  Value &val = js.root["new_obj"]["key2"];

  std::cout << js.root["new_obj"]["key2"] << std::endl; // access a value
}
