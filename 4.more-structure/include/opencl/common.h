/**
 * @copyright 2018 Fetch Robotics Inc.
 * @author Carl Saldanha
 */


#ifndef OPENCL_COMMON_H
#define OPENCL_COMMON_H

// OpenCL Include
#include <CL/cl.hpp>
#include <memory>

namespace opencl {

namespace common {


/**
 * @brief An OpenCL Exception
 */
class OpenCLException : public std::exception
{
  public:
  OpenCLException(std::string string_exception):
  string_exception_(string_exception)
  {}

  OpenCLException(cl_int cl_exception):
  cl_exception_(cl_exception)
  {}

  const char * what () const throw()
  {
	std::string s = std::to_string(cl_exception_);
 	return (string_exception_ + ": " + s).c_str();
  }
  private:
  cl_int cl_exception_ = -1000;
  std::string string_exception_ = "Internal OpenCL Exception";
};

/**
 * @brief Returns a list of OpenCL devices on the system
 * @throws OpenCLException if no devices are found
 */
std::vector<cl::Device> loadDevices()
{
    std::vector<cl::Platform> all_platforms;
    cl::Platform::get(&all_platforms);
    if(all_platforms.size()==0){
	throw OpenCLException("No Platforms found. Check OpenCL installation!");
    }
    
    std::vector<cl::Device> all_devices{};
    for (cl::Platform platform: all_platforms) {
      std::vector<cl::Device> devices;
      platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);

      all_devices.insert(all_devices.end(), devices.begin(), devices.end());
    }
    if(all_devices.size()==0){
	throw OpenCLException("No Devices found. Check OpenCL installation!");
    }
    return all_devices;
};


inline cl::Program createProgramFromSource(const cl::Context context,const std::vector<cl::Device> devices, const std::string readFileName)
{
    std::ifstream file(readFileName.c_str(), std::ios::binary | std::ios::ate | std::ios::in);
    uint32_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    char* buffer = new char[size];
    file.read(buffer, size);
    file.close();
    if (buffer && !buffer[0])
    {
	throw OpenCLException("You loaded a blank kernel file: " + readFileName);
    }
    cl::Program::Sources sources;
    std::string kernel_code(buffer);
    sources.push_back({kernel_code.c_str(),kernel_code.length()});
    cl::Program program{context,sources};
    if(program.build(devices)!=CL_SUCCESS){
        std::string error_log;
        for (auto device: devices) {
          error_log += program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device);
        }
    //    std::cout << error_log <<std::endl;
  	throw OpenCLException("Error building: " + error_log);
    }
    return program;
}

///
//  Attempt to create the program object from a cached binary.  Note that
//  on first run this will fail because the binary has not yet been created.
//
inline cl::Program createProgramFromBinary(const cl::Context context,const std::vector<cl::Device> devices, const std::string fileName)
{
    std::ifstream file(fileName.c_str(),  std::ios::binary | std::ios::in | std::ios::ate);

    uint32_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    char* buffer = new char[size];
    file.read(buffer, size);
    file.close();
    cl::Program::Binaries bin{{buffer, size}};

    std::vector<cl_int> binaryStatus;
    cl_int err = 0;
    cl::Program program = cl::Program{context, devices, bin, &binaryStatus, &err};

    if(err != CL_SUCCESS) {
       throw OpenCLException(" Error loading: " + err);
    }
    if(program.build(devices)!=CL_SUCCESS){
       throw OpenCLException(" Error building: " + program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[0]));
    }
    delete[] buffer;
    return program;
}   


template<class T>
inline cl::Buffer enqueueWriteBuffer(cl::CommandQueue& queue, cl::Context& context, std::vector<T> input)
{
    cl::Buffer buffer(context,CL_MEM_READ_WRITE,sizeof(T)*input.size());
    queue.enqueueWriteBuffer(buffer,CL_TRUE,0,sizeof(T)*input.size(), &input[0]);
    return buffer;
} 

}
}
#endif // OPENCL_COMMON_H
