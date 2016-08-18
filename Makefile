CXX=g++
CXXFLAGS=-Wall -O3 -g -std=c++11 -ISFML-2.3.2/include
LDFLAGS=-llua5.2 -LSFML-2.3.2/lib -lsfml-graphics -lsfml-window -lsfml-system
EXEC=Cursed-Crown2
SRC= $(wildcard *.cpp)
OBJ= $(SRC:.cpp=.o)

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
			
