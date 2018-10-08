#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <CL/cl.hpp>

std::vector<cl::Device> loadDevices()
{
    std::vector<cl::Platform> all_platforms;
    cl::Platform::get(&all_platforms);
    if(all_platforms.size()==0){
        std::cout<<" No platforms found. Check OpenCL installation!\n";
        exit(1);
    }
    cl::Platform default_platform=all_platforms[0];
    std::cout << "Using platform: "<<default_platform.getInfo<CL_PLATFORM_NAME>()<<"\n";
 
    //get default device of the default platform
    std::vector<cl::Device> all_devices;
    default_platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
    if(all_devices.size()==0){
        std::cout<<" No devices found. Check OpenCL installation!\n";
        exit(1);
    }
    cl::Device default_device=all_devices[0];
    std::cout<< "Using device: "<<default_device.getInfo<CL_DEVICE_NAME>()<<"\n";
    return all_devices;
}

///
//  Create an OpenCL program from the kernel source file
//
inline cl::Program CreateBinaryFromProgram(const cl::Context context,const std::vector<cl::Device> devices, const char* readFileName, const char* writeFileName)
{
    std::ifstream file(readFileName, std::ios::binary | std::ios::ate | std::ios::in);

    uint32_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    char* buffer = new char[size];
    
    file.read(buffer, size);
    file.close();

    cl::Program::Sources sources;
 
    // kernel calculates for each element C=A+B
    std::string kernel_code(buffer);
    sources.push_back({kernel_code.c_str(),kernel_code.length()});
    cl::Program program{context,sources};
    if(program.build(devices)!=CL_SUCCESS){
       std::cout<<" Error building: "<<program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[0])<<"\n";
        exit(0);
    }
    std::vector<char*> output = program.getInfo<CL_PROGRAM_BINARIES>();
    std::vector<size_t> output_sizes = program.getInfo<CL_PROGRAM_BINARY_SIZES>();
    std::ofstream outFile (writeFileName, std::ios::out | std::ios::binary);
    outFile.write(output[0], output_sizes[0]);
    outFile.close();
    return program;
}

///
//  Attempt to create the program object from a cached binary.  Note that
//  on first run this will fail because the binary has not yet been created.
//
inline cl::Program CreateProgramFromBinary(cl::Context context,const std::vector<cl::Device> devices, const char* fileName)
{
    std::ifstream file(fileName, std::ios::binary | std::ios::ate | std::ios::in);

    uint32_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    char* buffer = new char[size];
    file.read(buffer, size);
    file.close();

    cl::Program::Binaries bin{{buffer, size}};

    std::vector<cl_int> binaryStatus;
    cl_int* err;
    cl::Program program = cl::Program{context, devices, bin, &binaryStatus, err};

    if(err != CL_SUCCESS) {
       std::cout<<" Error building: "<<program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[0])<<"\n";
        exit(0);
    }

    return program;
}


///
//	main() for HelloBinaryWorld example
//
int main(int argc, char** argv)
{
    std::vector<cl::Device> devices= loadDevices();
    cl::Context context{devices};

    std::cout << "Save program binary for future run..." << std::endl;
    CreateBinaryFromProgram(context, devices, "HelloWorld.cl", "HelloWorld.cl.bin");


    std::cout << "Read from binary..." << std::endl;
    devices= loadDevices();
    context = cl::Context{devices};
    cl::Program program = CreateProgramFromBinary(context, devices, "HelloWorld.cl.bin");

    cl::Buffer buffer_A(context,CL_MEM_READ_WRITE,sizeof(int)*10);
    cl::Buffer buffer_B(context,CL_MEM_READ_WRITE,sizeof(int)*10);
    cl::Buffer buffer_C(context,CL_MEM_READ_WRITE,sizeof(int)*10);
 
    int A[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int B[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
 
    //create queue to which we will push commands for the device.
    cl::CommandQueue queue(context,devices[0]);
 
    //write arrays A and B to the device
    queue.enqueueWriteBuffer(buffer_A,CL_TRUE,0,sizeof(int)*10,A);
    queue.enqueueWriteBuffer(buffer_B,CL_TRUE,0,sizeof(int)*10,B);
 
 
    //run the kernel
    cl::Kernel kernel_add=cl::Kernel(program,"simple_add");
    kernel_add.setArg(0,buffer_A);
    kernel_add.setArg(1,buffer_B);
    kernel_add.setArg(2,buffer_C);
    queue.enqueueNDRangeKernel(kernel_add,cl::NullRange,cl::NDRange(10),cl::NullRange);
    queue.finish();
 
    int C[10];
    //read result C from the device to array C
    queue.enqueueReadBuffer(buffer_C,CL_TRUE,0,sizeof(int)*10,C);
 
    std::cout<<" result: \n";
    for(int i=0;i<10;i++)
        std::cout<<C[i]<<" ";
    std::cout << "\n";
  //  Cleanup(context, commandQueue, program, kernel, memObjects);
//
    return 0;
}
