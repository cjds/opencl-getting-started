#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <CL/cl.hpp>
#include <opencl/kernel/kernel_manager.h>

///
/// Run a program that checks if an array of points is within a circle
//
int main(int argc, char** argv)
{
    std::vector<cl::Device> devices= loadDevices();
    cl::Context context{devices};

    std::cout << "Save program binary for future run..." << std::endl;
    CreateBinaryFromProgram(context, devices, "HelloWorld.cl", "HelloWorld.cl.bin");


    std::cout << "Reading from binary..." << std::endl;
    devices = loadDevices();
    context = cl::Context{devices};
    cl::Program program = CreateProgramFromBinary(context, devices, "HelloWorld.cl.bin");

    cl::Buffer buffer_PointsX(context,CL_MEM_READ_WRITE,sizeof(int)*10);
    cl::Buffer buffer_PointsY(context,CL_MEM_READ_WRITE,sizeof(int)*10);
    cl::Buffer buffer_Center(context,CL_MEM_READ_WRITE,sizeof(int)*2);
    cl::Buffer buffer_Extends(context,CL_MEM_READ_WRITE,sizeof(int)*2);
    cl::Buffer buffer_Output(context,CL_MEM_READ_WRITE,sizeof(bool)*10);
 
    int PointsX[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int PointsY[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int Center[] = {0, 0};
    int Extends[] = {1, 2};
    bool output[]{false, false, false, false, false, false, false, false, false, true};
 
    //create queue to which we will push commands for the device.
    cl::CommandQueue queue(context,devices[0]);
 
    //write arrays A and B to the device
    queue.enqueueWriteBuffer(buffer_PointsX,CL_TRUE,0,sizeof(int)*10,PointsX);
    queue.enqueueWriteBuffer(buffer_PointsY,CL_TRUE,0,sizeof(int)*10,PointsY);
    queue.enqueueWriteBuffer(buffer_Center,CL_TRUE,0,sizeof(int)*2,Center);
    queue.enqueueWriteBuffer(buffer_Extends,CL_TRUE,0,sizeof(int)*2,Extends);
 
  
    std::cout << "Running program..." << std::endl;
    //run the kernel
    cl::Kernel kernel_add=cl::Kernel(program,"point_within");
    kernel_add.setArg(0,buffer_PointsX);
    kernel_add.setArg(1,buffer_PointsY);
    kernel_add.setArg(2,buffer_Center);
    kernel_add.setArg(3,buffer_Extends);
    kernel_add.setArg(4,buffer_Output);
    queue.enqueueNDRangeKernel(kernel_add, cl::NullRange, cl::NDRange(10), cl::NDRange(2));
    queue.finish();
 
    int C[10];
    //read result C from the device to array C
    queue.enqueueReadBuffer(buffer_Output,CL_TRUE,0,sizeof(bool)*10,output);
 
    std::cout<<" result: \n";
    for(int i=0;i<10;i++)
        std::cout<<output[i]<<" ";
    std::cout << "\n";
    return 0;
}
