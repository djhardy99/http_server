.PHONY: build client server
build:
	g++ -Wall -Iinclude  src/client/*.cpp -o build/client.exe
	g++ -Wall -Iinclude  src/server/*.cpp -o build/server.exe

client:
	./build/server.exe
server:
	./build/server.exe