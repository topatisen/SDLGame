CXX = g++

SDL_LIB = -L/usr/lib -lSDL2 -lpthread
SDL_INCLUDE = -I/usr/include -I./
TARGET = spejl
SRC = $(TARGET).cpp ./utilities.cpp 
CXXFLAGS = -pedantic -Wall -c -std=gnu++11 $(SDL_INCLUDE)
LDFLAGS = $(SDL_LIB)
EXE = SDL_Test
all: $(EXE)

$(EXE): $(TARGET).o
	$(CXX) $< $(LDFLAGS) -o $@

main.o: $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC)

clean:
	rm *.o && rm $(EXE)
