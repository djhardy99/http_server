.PHONY: all client server clean preprocess assemble disassemble run_server run_client

CXX = g++
CXXFLAGS = -Wall -Icode/client/include -Icode/server/include
OBJ_DIR = build/bin
CLIENT_OBJ_DIR = $(OBJ_DIR)/client
SERVER_OBJ_DIR = $(OBJ_DIR)/server
TARGET_DIR = build/target
CLIENT_TARGET = $(TARGET_DIR)/client.exe
SERVER_TARGET = $(TARGET_DIR)/server.exe

# Directories for assembly outputs
ASSEMBLY_DIR = build/assembly
PREPROCESSED_DIR = $(ASSEMBLY_DIR)/preprocessed
ASSEMBLED_DIR = $(ASSEMBLY_DIR)/assembled
DISASSEMBLED_DIR = $(ASSEMBLY_DIR)/disassembled

# Default target
all: client server preprocess assemble disassemble

# Create necessary directories
$(OBJ_DIR) $(CLIENT_OBJ_DIR) $(SERVER_OBJ_DIR) $(TARGET_DIR) \
$(PREPROCESSED_DIR) $(ASSEMBLED_DIR) $(DISASSEMBLED_DIR):
	mkdir -p $@

# Collect source files
CLIENT_SRCS = $(wildcard code/client/src/*.cpp)
SERVER_SRCS = $(wildcard code/server/src/*.cpp)

# Convert source files to object files
CLIENT_OBJS = $(CLIENT_SRCS:code/client/src/%.cpp=$(CLIENT_OBJ_DIR)/%.o)
SERVER_OBJS = $(SERVER_SRCS:code/server/src/%.cpp=$(SERVER_OBJ_DIR)/%.o)

# Compile client sources into object files
$(CLIENT_OBJ_DIR)/%.o: code/client/src/%.cpp | $(OBJ_DIR) $(CLIENT_OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile server sources into object files
$(SERVER_OBJ_DIR)/%.o: code/server/src/%.cpp | $(OBJ_DIR) $(SERVER_OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link client executable
client: $(TARGET_DIR) $(CLIENT_TARGET)

$(CLIENT_TARGET): $(CLIENT_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(CLIENT_OBJS)

# Link server executable
server: $(TARGET_DIR) $(SERVER_TARGET)

$(SERVER_TARGET): $(SERVER_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(SERVER_OBJS)

# Preprocess code
preprocess: $(patsubst code/client/src/%.cpp,$(PREPROCESSED_DIR)/code/client/src/%.cpp,$(CLIENT_SRCS)) \
            $(patsubst code/server/src/%.cpp,$(PREPROCESSED_DIR)/code/server/src/%.cpp,$(SERVER_SRCS))

$(PREPROCESSED_DIR)/code/client/src/%.cpp: code/client/src/%.cpp | $(PREPROCESSED_DIR)
	mkdir -p $(dir $@)
	$(CXX) -E $< -o $@

$(PREPROCESSED_DIR)/code/server/src/%.cpp: code/server/src/%.cpp | $(PREPROCESSED_DIR)
	mkdir -p $(dir $@)
	$(CXX) -E $< -o $@

# Assemble code
assemble: $(patsubst code/client/src/%.cpp,$(ASSEMBLED_DIR)/code/client/src/%.s,$(CLIENT_SRCS)) \
          $(patsubst code/server/src/%.cpp,$(ASSEMBLED_DIR)/code/server/src/%.s,$(SERVER_SRCS))

$(ASSEMBLED_DIR)/code/client/src/%.s: code/client/src/%.cpp | $(ASSEMBLED_DIR)
	mkdir -p $(dir $@)
	$(CXX) -S $< -o $@

$(ASSEMBLED_DIR)/code/server/src/%.s: code/server/src/%.cpp | $(ASSEMBLED_DIR)
	mkdir -p $(dir $@)
	$(CXX) -S $< -o $@

# Disassemble object files
disassemble: $(patsubst $(CLIENT_OBJ_DIR)/%.o,$(DISASSEMBLED_DIR)/code/client/src/%.txt,$(CLIENT_OBJS)) \
              $(patsubst $(SERVER_OBJ_DIR)/%.o,$(DISASSEMBLED_DIR)/code/server/src/%.txt,$(SERVER_OBJS))

$(DISASSEMBLED_DIR)/code/client/src/%.txt: $(CLIENT_OBJ_DIR)/%.o | $(DISASSEMBLED_DIR)
	mkdir -p $(dir $@)
	objdump -d $< > $@

$(DISASSEMBLED_DIR)/code/server/src/%.txt: $(SERVER_OBJ_DIR)/%.o | $(DISASSEMBLED_DIR)
	mkdir -p $(dir $@)
	objdump -d $< > $@

# Clean up build and target directories
clean:
	rm -rf build

run_client:
	./build/target/client.exe

run_server:
	./build/target/server.exe
