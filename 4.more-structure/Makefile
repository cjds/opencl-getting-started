HEADERS = include 
OBJECTS = src/main.cpp

default: program

%.o: %.cpp $(HEADERS)
	g++ -std=c++0x -c  $< -o $@ -I/opt/intel/opencl/include -Iinclude -Wall -Werror -Wextra -WAll -WError -lOpenCL -g -O0


program: $(OBJECTS)
	g++ -std=c++0x $(OBJECTS) -I/opt/intel/opencl/include -Iinclude -lOpenCL -o  $@

clean:
	-rm -f program
	make
