# JSON reader

## Team members
Just me (Vikram)

## Description
This is a simple JSON reader library that can read a JSON file and provide a way to access the data.

Key features:
- Tokenizer for JSON files
- Parser for JSON files
- Interface to access the data


C++ features:
- smart pointers
- IO streams
- variant
- operator overloading
- benchmarking

The challenge of this project will be in the complexity of writing a JSON tokenizer and parser. The tokenizer will be able to read a JSON file and split it into tokens. The parser will be able to read the tokens and create a tree structure that can be used to access the data. We then provide an interface to access the data. This interface will likely be implemented by overloading the subscript `[]` operator.

We will use a relatively simple class structure.
- `JSONReader` will be the main class that will be used to read the JSON file and provide access to the data.
- `Token` will be a class that will represent a token. It will have a type and a value.
- `Tokenizer` will be a class that will read the JSON file and split it into tokens.
- `Parser` will be a class that will read the tokens and create a tree structure that can be used to access the data.

If we have time, we will add a `JSONWriter` class that will be able to write a JSON file from the data. This will be a good way to test the `JSONReader` class.

Whilst this project does not have a GUI / multi-threading, the complexity of this project comes from the difficult operations required to create a tokenizer and parser. These operations will require using a variety of C++ features learned in this class. To further increase the complexity of this project, we will also implement a benchmarking system to compare the performance of our JSON reader with [other JSON readers](https://json.nlohmann.me/integration/). It is expected that our JSON reader will be slower than other JSON readers, but we will try to make it as fast as possible (and hopefully faster than readers in other languages like Python).