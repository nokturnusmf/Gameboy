CXX			:= g++
CXX_FLAGS	:= -std=c++17 -Wall -Wextra
REL_FLAGS	:= -O3 -flto -march=native
DBG_FLAGS	:= -g

LIBS 		:= -lglfw -lGL -lGLEW

SRC 		:= $(shell find src/ -name "*.cpp")
RELEASE_OBJ	:= $(patsubst src/%.cpp, bin/obj/release/%.o, $(SRC))
DEBUG_OBJ	:= $(patsubst src/%.cpp, bin/obj/debug/%.o, $(SRC))

.PHONY: all release debug clean

release: bin/gb

debug: bin/debug

all: release debug

clean:
	rm -rf bin

bin:
	mkdir -p bin bin/obj/release bin/obj/debug

DEPENDS := $(RELEASE_OBJ:.o=.d) $(DEBUG_OBJ:.o=.d)
-include $(DEPENDS)

bin/obj/release/%.o: src/%.cpp | bin
	$(CXX) -c -MMD -o $@ $< $(CXX_FLAGS) $(REL_FLAGS)

bin/obj/debug/%.o: src/%.cpp | bin
	$(CXX) -c -MMD -o $@ $< $(CXX_FLAGS) $(DBG_FLAGS)

bin/gb: $(RELEASE_OBJ)
	$(CXX) -o $@ $^ $(CXX_FLAGS) $(REL_FLAGS) $(LIBS)

bin/debug: $(DEBUG_OBJ)
	$(CXX) -o $@ $^ $(CXX_FLAGS) $(DBG_FLAGS) $(LIBS)
