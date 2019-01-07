HEADERS = 
OBJECTS = main.cpp

default: program

%.o: %.cpp $(HEADERS)
	g++ -std=c++11 -c $< -o $@ -I/opt/intel/opencl/include -lOpenCL


program: $(OBJECTS)
	g++ -std=c++11 $(OBJECTS) -I/opt/intel/opencl/include -lOpenCL -o $@

clean:
	-rm -f $(OBJECTS)
	-rm -f program
