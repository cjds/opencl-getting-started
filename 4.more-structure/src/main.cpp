#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <CL/cl.hpp>
#include <geometry/aabb.h>

///
/// Run a program that checks if an array of points is within a circle
//
int main(int argc, char** argv)
{
    geometry::aabb::PointWithin p{};
    std::vector<int> x{1, 2, 3, 4, 5, 100};
    std::vector<int> y{1, 2, 3, 4, 5, 100};
    std::vector<int> center = {1, 2};
    std::vector<int> extends = {2, 2};
    std::vector<bool> output = p.points_within(x, y, center, extends);
   
    std::cout<<"Are points in the circle"<<std::endl;
    for(auto val: output) {
      std::cout<<std::to_string(val).c_str()<<std::endl;
    }  
    return 0;
}
