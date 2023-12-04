all: json-reader

json-reader: main.cpp
	g++ -g -Wall -std=c++20 main.cpp tokenizer.cpp parser.cpp -o json-reader
clean:
	rm json-reader
