standard := c++14
objs := main.o Algorithms/Algorithm.o Algorithms/AntiAliasingAlgorithm.o Algorithms/MidPointAlgorithm.o
exe := main

all: $(objs)
	g++ $^ -o $(exe) -framework GLUT -framework OpenGL -L/usr/local/Cellar/freeglut/3.2.2/lib -lglut

check-address: $(objs)
	g++ $^ -o $(exe) -framework GLUT -framework OpenGL -L/usr/local/Cellar/freeglut/3.2.2/lib -lglut -fsanitize=address

%.o: %.cpp
	g++ -c $? -o $@ -std=$(standard) -Wall -Wextra -Wno-deprecated-declarations -Werror -pedantic-errors -m64

.PHONY: clean
clean:
	rm -f $(objs) $(exe)
