TARGET = Upcv
# Alterado de CC (C Compiler) para CXX (C++ Compiler)
CXX = g++
LIBS = -lm
# Alterado de CFLAGS para CXXFLAGS (padrão para C++)
CXXFLAGS = -g -Wall
SOURCES = $(wildcard *.cpp)
HEADERS = $(wildcard *.h)
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

# Regra de Linkagem
$(TARGET): $(OBJECTS)
	@ echo 'Building binary using G++ linker: $@'
	# Aqui usamos $(CXX) ao invés de $(CC)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)
	@ echo 'Finished building binary: $@'
	@ echo ' '

# Regra de Compilação
%.o: %.cpp %.h
	@ echo 'Building target using G++ compiler: $<'
	$(CXX) $(CXXFLAGS) -o $@ -c $<
	@ echo ' '

clean:
	@ echo 'Cleaning project:'
	rm -f $(TARGET) $(OBJECTS)
	@ echo ' '