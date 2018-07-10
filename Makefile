OBJ_DIR := ./obj
SRC_DIR := ./src
SRC_FILES := $(wildcard $(SRC_DIR)/*/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

LDFLAGS := -lm
CPPFLAGS := -Wall
CXXFLAGS := -MMD
-include $(OBJ_FILES:.o=.d)

ssts: $(OBJ_FILES)
	g++ $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	g++ $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

.PHONY : clean
clean :
	-rm ssts $(OBJ_FILES) $(OBJ_FILES:.o=.d)
