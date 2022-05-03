standard := c++11
objs := main.o
exe := main

all: $(objs)
	g++ $< -o $(exe) -framework GLUT -framework OpenGL

%.o: %.cpp
	g++ -c $? -o $@ -std=$(standard) -Wall -Wextra -Wno-deprecated-declarations -Werror -pedantic-errors -m64

.PHONY: clean
clean:
	rm -f $(objs) $(exe)
