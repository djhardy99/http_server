.PHONY: all client server clean preprocess assemble disassemble

CXX = g++
CXXFLAGS = -Wall -Iinclude
OBJ_DIR = bin
CLIENT_OBJ_DIR = $(OBJ_DIR)/client
SERVER_OBJ_DIR = $(OBJ_DIR)/server
TARGET_DIR = target
CLIENT_TARGET = $(TARGET_DIR)/client.exe
SERVER_TARGET = $(TARGET_DIR)/server.exe

# Directories for assembly outputs
ASSEMBLY_DIR = assembly
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
CLIENT_SRCS = $(wildcard src/client/*.cpp)
SERVER_SRCS = $(wildcard src/server/*.cpp)

# Convert source files to object files
CLIENT_OBJS = $(CLIENT_SRCS:src/client/%.cpp=$(CLIENT_OBJ_DIR)/%.o)
SERVER_OBJS = $(SERVER_SRCS:src/server/%.cpp=$(SERVER_OBJ_DIR)/%.o)

# Compile client sources into object files
$(CLIENT_OBJ_DIR)/%.o: src/client/%.cpp | $(OBJ_DIR) $(CLIENT_OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile server sources into object files
$(SERVER_OBJ_DIR)/%.o: src/server/%.cpp | $(OBJ_DIR) $(SERVER_OBJ_DIR)
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
preprocess: $(patsubst src/client/%.cpp,$(PREPROCESSED_DIR)/src/client/%.cpp,$(CLIENT_SRCS)) \
            $(patsubst src/server/%.cpp,$(PREPROCESSED_DIR)/src/server/%.cpp,$(SERVER_SRCS))

$(PREPROCESSED_DIR)/src/client/%.cpp: src/client/%.cpp | $(PREPROCESSED_DIR)
	mkdir -p $(dir $@)
	$(CXX) -E $< -o $@

$(PREPROCESSED_DIR)/src/server/%.cpp: src/server/%.cpp | $(PREPROCESSED_DIR)
	mkdir -p $(dir $@)
	$(CXX) -E $< -o $@

# Assemble code
assemble: $(patsubst src/client/%.cpp,$(ASSEMBLED_DIR)/src/client/%.s,$(CLIENT_SRCS)) \
          $(patsubst src/server/%.cpp,$(ASSEMBLED_DIR)/src/server/%.s,$(SERVER_SRCS))

$(ASSEMBLED_DIR)/src/client/%.s: src/client/%.cpp | $(ASSEMBLED_DIR)
	mkdir -p $(dir $@)
	$(CXX) -S $< -o $@

$(ASSEMBLED_DIR)/src/server/%.s: src/server/%.cpp | $(ASSEMBLED_DIR)
	mkdir -p $(dir $@)
	$(CXX) -S $< -o $@

# Disassemble object files
disassemble: $(patsubst $(CLIENT_OBJ_DIR)/%.o,$(DISASSEMBLED_DIR)/$(CLIENT_OBJ_DIR)/%.txt,$(CLIENT_OBJS)) \
              $(patsubst $(SERVER_OBJ_DIR)/%.o,$(DISASSEMBLED_DIR)/$(SERVER_OBJ_DIR)/%.txt,$(SERVER_OBJS))

$(DISASSEMBLED_DIR)/$(CLIENT_OBJ_DIR)/%.txt: $(CLIENT_OBJ_DIR)/%.o | $(DISASSEMBLED_DIR)
	mkdir -p $(dir $@)
	objdump -d $< > $@

$(DISASSEMBLED_DIR)/$(SERVER_OBJ_DIR)/%.txt: $(SERVER_OBJ_DIR)/%.o | $(DISASSEMBLED_DIR)
	mkdir -p $(dir $@)
	objdump -d $< > $@

# Clean up build and target directories
clean:
	rm -rf $(OBJ_DIR) $(TARGET_DIR) $(ASSEMBLY_DIR)
