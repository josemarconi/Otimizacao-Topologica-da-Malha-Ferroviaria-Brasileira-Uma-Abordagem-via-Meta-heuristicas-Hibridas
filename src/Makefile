TARGET = Upcv
CXX = g++
LIBS = -lm

CXXFLAGS = -O2 -Wall

SOURCES = $(wildcard *.cpp)
HEADERS = $(wildcard *.h)
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@ echo 'Building binary using G++ linker: $@'
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)
	@ echo 'Finished building binary: $@'
	@ echo ' '

%.o: %.cpp %.h
	@ echo 'Building target using G++ compiler: $<'
	$(CXX) $(CXXFLAGS) -o $@ -c $<
	@ echo ' '

clean:
	@ echo 'Cleaning project:'
	rm -f $(TARGET) $(OBJECTS) $(TARGET).exe
	@ echo ' '

.PHONY: all clean