

json-reader: tokenizer.cpp main.cpp
	g++ -g -Wall -std=c++17 tokenizer.cpp main.cpp -o json-reader

clean:
	rm -f json-reader	