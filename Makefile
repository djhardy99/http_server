.PHONY: run
run:
	g++ -Wall -Iinclude  src/*.cpp -o build/main.exe
	./build/main.exe
