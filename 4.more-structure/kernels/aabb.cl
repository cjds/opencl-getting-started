void kernel point_within_int(global const int* pointsX, global const int* pointsY, global int* center, global int* extents, global bool* output) {
  output[get_global_id(0)] = (abs(pointsX[get_global_id(0)] - center[0]) <= extents[0]) && \
                              (abs(pointsY[get_global_id(0)] - center[1]) < extents[1]); 
};


void kernel point_within_float(global const float* pointsX, global const float* pointsY, global float* center, global float* extents, global bool* output) {
  output[get_global_id(0)] = (fabs(pointsX[get_global_id(0)] - center[0]) <= extents[0]) && \
                              (fabs(pointsY[get_global_id(0)] - center[1]) < extents[1]); 
};

void kernel point_within_double(global const double* pointsX, global const double* pointsY, global double* center, global double* extents, global bool* output) {
  output[get_global_id(0)] = (fabs(pointsX[get_global_id(0)] - center[0]) <= extents[0]) && \
                              (fabs(pointsY[get_global_id(0)] - center[1]) < extents[1]); 
};
