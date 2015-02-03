#include "collisions.h"

bool Collisions::sphere3_ray3(const vector3_t& c, double r, const ray3_t& y)
{
  // get vector from ray start to center of sphere
  auto v1 = c - y.point();
  
  // test if ray is pointing towards sphere
  auto len  = v1.dot(y.dir());
  if (len < 0) {return false;}
  
  // get test point which is closest point on ray to center of sphere
  auto proj = y.dir();
  proj.scale(len);
  auto tp =  proj + y.point();
  
  // collision test with this point and the sphere
  return sphere3_vector3(c, r, tp);
}

bool Collisions::poly3_vector3(const poly3_t& t, const vector3_t& v)
{
  // NOTE: function assumes that the point is coplaner with the triangle
  
  // find the inverse of the area of the triangle (iat)
  auto ts1 = t.p1() - t.p2();
  auto ts2 = t.p1() - t.p3();
  auto iat = 1 / ts1.cross(ts2).length();
  
  // find vectors from the corners of the triangle to the point
  // used in calculating the barycentric coordinates of the point
  auto ta = t.p1() - v;
  auto tb = t.p2() - v;
  auto tc = t.p3() - v;
  
  // calculate the barycentric coordinates for the point
  // a->alpha, b->beta, g->gamma
  auto a = tb.cross(tc).length() * iat;
  auto b = tc.cross(ta).length() * iat;
  auto g = 1 - a - b;
  
  // if the coordinates are all between 0 and 1, the point lies within the triangle
  return (0 <= a && a <= 1) && (0 <= b && b <= 1) && (0 <= g && g <= 1);
}

bool Collisions::sphere3_poly3(const vector3_t& c, double r, const poly3_t& t)
{
  auto v1 = t.p1() - c;
  auto v2 = v1.project(t.n());
  auto q  = c + v2;
  
  // test if point is inside sphere and if point is inside triangle
  return sphere3_vector3(c, r, q) && poly3_vector3(t, q);
}

bool Collisions::poly3_ray3(const poly3_t& p, const ray3_t r)
{
  // NOTE: not sure what this does if ray and triangle are parallel
  
  // d in the equation of a plane, where d is 
  // (the normal of the plane) dot (a point on the plane)
  auto d = p.n().dot(p.p1());
  
  // t is a scalar which represents the length between 
  // (the point of the ray) and (the intersecting point on the plane)
  auto t = (r.point().dot(p.n()) + d) / r.dir().dot(p.n());
  
  // s is the vector which goes between the point of the ray and the intersecting point 
  // on the plane
  auto s = r.dir();
  s.scale(t);
  
  // q is the intersecting point on the plane
  auto q = r.point() + s;
  
  // test if the intersecting point is within the triangle
  return poly3_vector3(p, q);
}