HEADERS = include 
OBJECTS = src/main.cpp

default: program

%.o: %.cpp $(HEADERS)
	g++ -std=c++0x -c $< -o $@ -I/opt/intel/opencl/include -Iinclude -lOpenCL


program: $(OBJECTS)
	g++ -std=c++0x $(OBJECTS) -I/opt/intel/opencl/include -Iinclude -lOpenCL -o $@

clean:
	-rm -f $(OBJECTS)
	-rm -f program
