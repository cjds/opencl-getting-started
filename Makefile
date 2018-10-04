HEADERS = 
OBJECTS = main.cpp

default: program

%.o: %.cpp $(HEADERS)
	g++ -std=c++11 -c $< -o $@ -L/usr/lib/x86_64-linux-gnu -lOpenCL


program: $(OBJECTS)
	g++ -std=c++11 $(OBJECTS) -L/usr/lib/x86_64-linux-gnu -lOpenCL -o $@

clean:
	-rm -f $(OBJECTS)
	-rm -f program
