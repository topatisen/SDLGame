CXX = g++
CXXFLAGS = -pedantic -Wall -c -std=gnu++11 $(SDL_INCLUDE)
CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))
SDL_LIB = -L/usr/lib -lSDL2 -lpthread
SDL_INCLUDE = -I/usr/include -I./utilities.h
TARGET = spejl
LDFLAGS = $(SDL_LIB)
EXE = SDL_Test
$(EXE): $(TARGET).o
	$(CXX) $< $(LDFLAGS) -o $@

main.o: $(CPP_FILES)
	$(CXX) $(CXXFLAGS) $(CPP_FILESC)

clean:
	rm *.o && rm $(EXE)



main: $(OBJ_FILES)
	   g++ $(LD_FLAGS) -o $@ $^

obj/%.o: %.cpp
	   g++ $(CC_FLAGS) -c -o $@ $<
