#ifndef OCTREE_COLLISIONS_H
#define OCTREE_COLLISIONS_H

#include <stdexcept>
#include "types_3d.h"

class Collisions
{
public:
  static inline bool sphere3_vector3(const vector3_t& c, double r, const vector3_t p)
  {
    // distance between point and sphere center is less than radius -> point in sphere
    auto v1 = c - p;
    return v1.length() <= r;
  }
  
  static inline bool sphere3_sphere3(const vector3_t& c1, double r1, const vector3_t& c2, double r2)
  {
    // distance between sphere center1 and sphere center2 <= radius1 + radius2 -> spheres collide
    auto v1 = c1 - c2;
    return v1.length() <= (r1 + r2);
  }
  
  static bool sphere3_ray3(const vector3_t& c, double r, const ray3_t& y);
  
  static bool sphere3_ray3_time(const vector3_t& c, double r, const ray3_t& y)
  {
    // Avoid 'unused variable' warnings
    (void)c;
    (void)r;
    (void)y;
    
    // TODO: Implement this function
    throw std::logic_error("Not implemented");
    return false;
  }
  
  static bool poly3_vector3(const poly3_t& t, const vector3_t& v);
  static bool sphere3_poly3(const vector3_t& c, double r, const poly3_t& t);
  static bool poly3_ray3(const poly3_t& p, const ray3_t r);
};

#endif //OCTREE_COLLISIONS_H