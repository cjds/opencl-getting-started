/**
 * @copyright 2018 Fetch Robotics Inc.
 * @author Carl Saldanha
*/


#ifndef OPENCL_PROGRAM_PROGRAM_MANAGER_H
#define OPENCL_PROGRAM_PROGRAM_MANAGER_H

// OpenCL Include
#include <CL/cl.hpp>
#include <memory>

// Common Functions
#include <opencl/common.h>

namespace opencl {

namespace program {

class ProgramManager {
  public:
  ProgramManager(const std::vector<cl::Device> devices, const std::string opencl_source_file): 
    device_{devices[0]},
    context_{devices},
    program_{opencl::common::createProgramFromSource(context_, devices, opencl_source_file)},
    queue(context_, device_)
  {
  }

  template<typename T>
  inline void runProgram(const std::vector<T> input)
  {
    return opencl::common::enqueueWriteBuffer<T>(queue, context_, input);
  }

  template<typename T>
  inline cl::Buffer enqueueWriteBuffer(std::vector<T> input)
  {
    static_assert(! std::is_same<T, bool>::value, "Bool cannot be used to enqueue write buffer");
    return opencl::common::enqueueWriteBuffer(queue, context_, input);
  }

  inline cl::Buffer createBuffer(std::size_t size, std::size_t length){
    return cl::Buffer(context_,CL_MEM_READ_WRITE, size*length);
  }

  inline cl::Kernel createKernel(std::string program_name){
    return cl::Kernel(program_, program_name.c_str());
  }

  private:
    cl::Device device_;
    cl::Context context_;
    cl::Program program_;
  public:
    cl::CommandQueue queue;
};

}
}
#endif //  OPENCL_PROGRAM_PROGRAM_MANAGER_H
