HEADERS = 
OBJECTS = main.cpp

default: program

%.o: %.cpp $(HEADERS)
	g++ -std=c++0x -c $< -o $@ -I/opt/intel/opencl/include -lOpenCL


program: $(OBJECTS)
	g++ -std=c++0x $(OBJECTS) -I/opt/intel/opencl/include -lOpenCL -o $@ -g3

clean:
	-rm -f program
	make
