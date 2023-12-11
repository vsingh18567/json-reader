

json-derulo: tokenizer.cpp main.cpp
	g++ -g -Wall -std=c++17 tokenizer.cpp main.cpp -o json-derulo

clean:
	rm -f json-derulo	