/**
 * @copyright 2018 Fetch Robotics Inc.
 * @author Carl Saldanha
 */


#ifndef KERNEL_KERNEL_MANAGER_H
#define KERNEL_KERNEL_MANAGER_H

// OpenCL Include
#include <CL/cl.hpp>
#include <memory>

namespace opencl {

namespace kernel {


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

  const char * what () const throw ()
  {
	std::string s = std::to_string(cl_exception_);
 	return (string_exception_ + ": " + s).c_str();
  }
  private:
  cl_int cl_exception_ = -1000;
  std::string string_exception_ = "Internal OpenCL Exception";
};

std::vector<cl::Device> loadDevices()
{
    std::vector<cl::Platform> all_platforms;
    cl::Platform::get(&all_platforms);
    if(all_platforms.size()==0){
	throw OpenCLException("No Platforms found. Check OpenCL installation!");
    }
    
    std::vector<cl::Device> all_devices{};
    for (cl::Platform platform: all_platforms) {
      std::vector<cl::Device> devices = platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
      all_devices.insert(all_devices.end(), devices.begin(), devices.end());
    }
    if(all_devices.size()==0){
	throw OpenCLException("No Devices found. Check OpenCL installation!");
    }
    return all_devices;
};

class KernelManager {

  KernelManager(std::string opencl_source_file) {

  }
  private:
    bool loadProgramFromSource(std::string file_name) {

    }

    virtual void runKernel() {
        throw OpenCLException("Running the Kernel must be implemented by the subclass");
    }

 
    bool writeProgramBinaryToFile(std::string file_name);
    bool loadProgramFromBinary(std::string file_name);

    std::shared_ptr<cl::Device> getDevice() {
      return &device_;
    }

    void setDevice(std::shared_ptr<cl::Device> device){
        device_ = device;
    }
    void loadContext();
    void loadKernel();

    cl::Program program_;
    cl::Kernel kernel_;
    std::shared_ptr<cl::Device> device_;
};

}
}
#endif //  KERNEL_KERNEL_MANAGER_H
