#include "types_3d.h"

// vector3_t functions
double vector3_t::dot(const vector3_t& v) const
{
  // start a, end a, start b, initial sum 
  auto d = std::inner_product(std::begin(coord),std::end(coord),std::begin(v.coord), 0.0);
  return d;
}

vector3_t vector3_t::cross(const vector3_t& v) const
{
  double x, y, z;
  x = coord[1]*v.coord[2] - coord[2]*v.coord[1];
  y = coord[2]*v.coord[0] - coord[0]*v.coord[2];
  z = coord[0]*v.coord[1] - coord[1]*v.coord[0];
  return vector3_t(x, y, z);
}

vector3_t vector3_t::project(vector3_t v) const
{
  // copy v as new vector to work with (do not pass in by reference)
  auto d = dot(v);
  v.scale(d);
  return v;
}

double vector3_t::distance(const vector3_t&  v) const
{
  auto s = *this - v;
  auto d = s.dot(s);
  return sqrt(d);    
}

double vector3_t::length() const
{
  return sqrt(dot(*this));
}

void vector3_t::scale(double s)
{
  coord[0] *= s;
  coord[1] *= s;
  coord[2] *= s;
}

void vector3_t::normalize()
{
  auto n = length();
  coord[0] /= n;
  coord[1] /= n;
  coord[2] /= n;
}

vector3_t& operator+= (vector3_t& v1, const vector3_t& v2)
{
  v1.coord[0] += v2.coord[0];
  v1.coord[1] += v2.coord[1];
  v1.coord[2] += v2.coord[2];
  
  return v1;
}

vector3_t& operator-= (vector3_t& v1, const vector3_t& v2)
{
  v1.coord[0] -= v2.coord[0];
  v1.coord[1] -= v2.coord[1];
  v1.coord[2] -= v2.coord[2];
  
  return v1;
}

vector3_t operator+ (const vector3_t& v1, const vector3_t& v2)
{
  auto xd = v1.coord[0] + v2.coord[0];
  auto yd = v1.coord[1] + v2.coord[1];
  auto zd = v1.coord[2] + v2.coord[2];
  
  return vector3_t(xd, yd, zd);
}

vector3_t operator- (const vector3_t& v1, const vector3_t& v2)
{
  auto xd = v1.coord[0] - v2.coord[0];
  auto yd = v1.coord[1] - v2.coord[1];
  auto zd = v1.coord[2] - v2.coord[2];
  
  return vector3_t(xd, yd, zd);
}

/*
bool operator== (const vector3_t& v1, const vector3_t& v2)
{
  return std::equal(&v1.coord[0], &v1.coord[2], &v2.coord[0]);
}

bool operator!= (const vector3_t& v1, const vector3_t& v2)
{
  return !(v1 == v2);
}
*/

std::ostream& operator<<(std::ostream& os, const vector3_t& v)
{
  os << "Vector(" << v.x() << ", " << v.y() << ", "  << v.z() << ")";
  return os;
}


// ray3_t functions
ray3_t::ray3_t(const vector3_t& point_, const vector3_t& dir_) : components{point_, dir_} // TODO: does this move or copy?
{
  dir().normalize();
}

std::ostream& operator<<(std::ostream& os, const ray3_t& r)
{
  os << "Ray(" << "Start:" << r.point() << ", Direction:" << r.dir() << ")";
  return os;
}


// poly3_t functions
void poly3_t::set_normal()
{
  auto e1 = points[2] - points[1];
  auto e2 = points[3] - points[1];
  points[4] = e1.cross(e2);
}

std::ostream& operator<<(std::ostream& os, const poly3_t& t)
{
  os << "Poly3(" << "p1:" << t.p1() << ", p2:" << t.p2() << ", p3:" << t.p3() << ", n:" << t.n() << ")";
  return os;
}