/**
 * @author Carl Saldanha
 *
 * @file aabb.h
 * @brief Defines axis-aligned bounding box utility functions
 */
#ifndef GEOMETRY_AABB_H
#define GEOMETRY_AABB_H

#include <opencl/program_manager.h>
#include <opencl/common.h>

namespace geometry
{
/**
 * @brief Ohmigod we're going to do such a coooool alogrithm
 */
namespace aabb
{

class PointWithin {
public:

PointWithin():
  program_manager_{opencl::common::loadDevices(), "kernels/aabb.cl"}
{
}

inline std::vector<bool> points_within(std::vector<int> pointsX, std::vector<int> pointsY, std::vector<int> center, std::vector<int> extends) {
    bool* output = new bool[pointsX.size()];
    //create queue to which we will push commands for the device.
    cl::Buffer p_x_buffer = program_manager_.enqueueWriteBuffer(pointsX);
    cl::Buffer p_y_buffer = program_manager_.enqueueWriteBuffer(pointsY);
    cl::Buffer center_buffer = program_manager_.enqueueWriteBuffer(center);
    cl::Buffer extends_buffer = program_manager_.enqueueWriteBuffer(extends);
 
    cl::Buffer output_buffer = program_manager_.createBuffer(sizeof(bool), pointsX.size());

    cl::Kernel kernel_add=program_manager_.createKernel("point_within_int");
    kernel_add.setArg(0, p_x_buffer);
    kernel_add.setArg(1, p_y_buffer);
    kernel_add.setArg(2, center_buffer);
    kernel_add.setArg(3, extends_buffer);
    kernel_add.setArg(4, output_buffer);
    program_manager_.queue.enqueueNDRangeKernel(kernel_add, cl::NullRange, cl::NDRange(pointsX.size()), cl::NDRange(center.size()));
    program_manager_.queue.finish();
 
    program_manager_.queue.enqueueReadBuffer(output_buffer,CL_TRUE,0,sizeof(bool)*pointsX.size(), output);
    std::vector<bool> out{};
    for (int i=0; i< pointsX.size(); i++)
    {
      out.push_back(output[i]); 
    }
    return out;
}

private:
  opencl::program::ProgramManager program_manager_;
};

}  // namespace aabb
}  // geometry

#endif  // GEOMETRY_AABB_H
