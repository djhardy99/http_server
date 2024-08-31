.PHONY: build clean test run_server run_client

# Directories
BUILD_DIR = build
CLIENT_SRC_DIR = code/client/src
CLIENT_INCLUDE_DIR = code/client/include
SERVER_SRC_DIR = code/server/src
SERVER_INCLUDE_DIR = code/server/include

# Executable Names
CLIENT_EXE = $(BUILD_DIR)/client.exe
SERVER_EXE = $(BUILD_DIR)/server.exe

# Find all source files
CLIENT_SRCS = $(shell find $(CLIENT_SRC_DIR) -name '*.cpp') 
SERVER_SRCS = $(shell find $(SERVER_SRC_DIR) -name '*.cpp')

# Build targets
build: clean $(CLIENT_EXE) $(SERVER_EXE)

# Link client executable
$(CLIENT_EXE): $(CLIENT_SRCS)
	@mkdir -p $(BUILD_DIR)
	g++ -I$(CLIENT_INCLUDE_DIR) $(CLIENT_SRCS) -o $@

# Link server executable
$(SERVER_EXE): $(SERVER_SRCS)
	@mkdir -p $(BUILD_DIR)
	g++ -I$(SERVER_INCLUDE_DIR) $(SERVER_SRCS) -o $@

# Clean up build directory
clean:
	rm -rf $(BUILD_DIR)

# Testing placeholder
test:
	echo "WIP"

# Run executables
run_server: $(SERVER_EXE)
	./$(SERVER_EXE)

run_client: $(CLIENT_EXE)
	./$(CLIENT_EXE)
