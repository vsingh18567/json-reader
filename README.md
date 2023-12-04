# JSON reader


## Description
This is a simple JSON reader library that can read a JSON file and provide a way to access the data.

Key features:
- Tokenizer for JSON files
- Parser for JSON files
- Interface to access the data
- JSON writer (through the JSON object's `to_str()` method)

Benchmarked against [nlohmann](https://json.nlohmann.me/integration/)'s JSON-reader. It is about 2x slower and less feature-rich, so this is simply a proof of concept / toy project.

## Usage
```cpp
    std::ifstream ifs("simple.json");
    JSON js = read_json(ifs).value();
    Value nested = js["nested"]; 
    std::shared_ptr<Object> nested_obj = nested.cast<Object>();

    std::shared_ptr<Object> nested2 = js.root.get<Object>("nested"); // same as above

    std::shared_ptr<Object> nested3 = js.root.try_get<Object>("nested"); // same as above, but returns nullptr if the key is not found

    js.root.insert("new_key", Value("new_value")); // insert a new key-value pair

    Object obj;
    obj.insert("key", Value("value"));
    obj.insert("key2", 2); // don't need to wrap primitive types in a Value object
    js.root.insert("new_obj", obj); // insert a new key-value pair with an object as the value

    Value &val = js.root["new_obj"]["key2"]; // access a value by chaining the keys

```

## Some cpp features used
- `std::variant`
- `std::shared_ptr`
- templates
  - template specialization 
- operator overloading (`operator[]` and `operator<<`)