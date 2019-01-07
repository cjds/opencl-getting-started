/**
 * @author Carl Saldanha
 *
 * @file aabb.h
 * @brief Defines axis-aligned bounding box utility functions
 */
#ifndef GEOMETRY_AABB_H
#define GEOMETRY_AABB_H



namespace geometry
{
/**
 * @brief Ohmigod we're going to do such a coooool alogrithm
 */
namespace aabb
{

s
/**
 * @brief Check if 2D point is within a 2D axis-aligned bounding box
 *
 * @tparam PointT   (deduced) point type
 * @tparam ScalarT  (deduced) an arithmetic scalar coordinate type
 *
 * @param point  query point
 * @param center  center of the box
 * @param extents  positive half side lengths
 *
 * @retval true  if \p point is within bounding box defined by \p center and \p extents
 * @retval false  otherwise
 */
template<typename PointT, typename ScalarT>
inline bool point_within(const PointT& point,
                         const Coords<ScalarT>& center,
                         const Coords<ScalarT>& extents)
{
  return std::abs(point.x - center.x) <= extents.x and
         std::abs(point.y - center.y) <= extents.y;
}



}  // namespace aabb
}  // geometry

#endif  // GEOMETRY_AABB_H
