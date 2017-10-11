CC=g++
CFLAGS=-c -Wall -std=c++11
LDFLAGS=
INCLUDEDIRS="../include/"
SOURCES=evaluate.cpp ga.cpp individual.cpp population.cpp random.cpp utils.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=ga

all: $(SOURCES) $(EXECUTABLE)


$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) -I$(INCLUDEDIRS) $< -o $@

clean:
	rm *.o $(EXECUTABLE) *~

