all: main

clean: 
	rm -f shell

main: main.cpp
	g++ -o shell commands.cpp main.cpp -Wall