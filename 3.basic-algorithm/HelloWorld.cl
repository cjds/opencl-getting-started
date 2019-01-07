void kernel point_within(global const int* pointsX, global const int* pointsY, global int* center, global int* extents, global bool* output) {
  output[get_global_id(0)] = (abs(pointsX[get_global_id(0)] - center[0]) <= extents[0]) && \
                              (abs(pointsY[get_global_id(0)] - center[1]) < extents[1]); 
};
