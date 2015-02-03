#ifndef OCTREE_TYPES_3D_H
#define OCTREE_TYPES_3D_H

#include <ostream>
#include <algorithm>

class vector3_t
{
public:
  double coord[3];
  
  // constructors
  vector3_t():coord{0.0,0.0,0.0} {}
  vector3_t(double x, double y, double z) : coord{x,y,z} {}
    
  // getters
  double inline x() const {return coord[0];} 
  double inline y() const {return coord[1];} 
  double inline z() const {return coord[2];} 
  
  // Vector operator functions (return new vector)
  double    dot(const vector3_t& v) const;
  vector3_t cross(const vector3_t& v) const;
  vector3_t project(vector3_t v) const;
  
  // Vector metric functions
  double distance(const vector3_t&  v) const;
  double length() const;
  
  // Transformation functions
  void scale(double s);
  void normalize();

  // Vector arithmatic functions
  friend vector3_t& operator+= (vector3_t& v1, const vector3_t& v2);
  friend vector3_t& operator-= (vector3_t& v1, const vector3_t& v2);
  
  friend vector3_t operator+ (const vector3_t& v1, const vector3_t& v2);
  friend vector3_t operator- (const vector3_t& v1, const vector3_t& v2);

  // Vector comparison functions
  // TODO remove these because we shouldn't do double comparisons like this
  //friend bool operator== (const vector3_t& v1, const vector3_t& v2);
  //friend bool operator!= (const vector3_t& v1, const vector3_t& v2);
  
  // cout operator overload
  friend std::ostream& operator<<(std::ostream& os, const vector3_t& v);
};


class ray3_t
{
public:
  vector3_t components[2];
  
  // getters (used in constructor, so defined first)
  auto inline point() const {return components[0];}
  auto inline dir()   const {return components[1];}

  // constructors
  ray3_t(const vector3_t& point_, const vector3_t& dir_);
  
  // cout operator overload
  friend std::ostream& operator<<(std::ostream& os, const ray3_t& r);
};


class poly3_t
{
public:
  vector3_t points[4];
  
  // constructors
  poly3_t(vector3_t p1, vector3_t p2, vector3_t p3)
  {
    // copy in points (do not pass in by reference)
    points[0] = p1;
    points[1] = p2;
    points[3] = p3;
    set_normal();    
  }
  
  // getters
  auto inline p1() const {return points[0];}
  auto inline p2() const {return points[1];}
  auto inline p3() const {return points[2];}
  auto inline n()  const {return points[3];}
  
  // cout operator overload
  friend std::ostream& operator<<(std::ostream& os, const poly3_t& t);
  
private:
  // helper functions
  void set_normal();
};

#endif //OCTREE_TYPES_3D_H