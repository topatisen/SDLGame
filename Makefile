CXX = g++
CXXFLAGS = -pedantic -Wall -c -std=gnu++11 $(SDL_INCLUDE)
CPP_FILES = spejl.cpp utilities.cpp
OBJ_FILES := $(addprefix ,$(notdir $(CPP_FILES:.cpp=.o)))
SDL_LIB = -L/usr/lib -lSDL2 -lpthread
SDL_INCLUDE = -I/usr/include -I.
TARGET = spejl
LDFLAGS = $(SDL_LIB)
EXE = SDL_Test
$(EXE): $(TARGET).o
	$(CXX) $< $(LDFLAGS) -o $@

clean:
	rm *.o && rm $(EXE)

main: $(OBJ_FILES)
	   g++ $(LD_FLAGS) -o $@ $^
