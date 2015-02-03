#ifndef OCTREE_TYPES_3D_UNIT_H
#define OCTREE_TYPES_3D_UNIT_H

#include <iostream>
#include <cpptest.h>
#include <string>

#include "types_3d.h"

class TestSuite_TYPES_3D : public Test::Suite
{
public:
  TestSuite_TYPES_3D()
  {
    // 31 - 16 (+1)= 16 tests
    TEST_ADD(TestSuite_TYPES_3D::vector3_members)
    TEST_ADD(TestSuite_TYPES_3D::vector3_default_constructor)
    TEST_ADD(TestSuite_TYPES_3D::vector3_constructor)
    TEST_ADD(TestSuite_TYPES_3D::vector3_getters)
    TEST_ADD(TestSuite_TYPES_3D::vector3_operators_dot)
    TEST_ADD(TestSuite_TYPES_3D::vector3_operators_cross)
    TEST_ADD(TestSuite_TYPES_3D::vector3_operators_project)
    TEST_ADD(TestSuite_TYPES_3D::vector3_metric_distance)
    TEST_ADD(TestSuite_TYPES_3D::vector3_metric_length)
    TEST_ADD(TestSuite_TYPES_3D::vector3_transform_scale)
    TEST_ADD(TestSuite_TYPES_3D::vector3_transform_normalize)
    TEST_ADD(TestSuite_TYPES_3D::vector3_operators_plusequals)
    TEST_ADD(TestSuite_TYPES_3D::vector3_operators_minusequals)
    TEST_ADD(TestSuite_TYPES_3D::vector3_operators_plus)
    TEST_ADD(TestSuite_TYPES_3D::vector3_operators_minus)
    TEST_ADD(TestSuite_TYPES_3D::vector3_overload_stream_insertion)
    
    TEST_ADD(TestSuite_TYPES_3D::ray3_members)
    TEST_ADD(TestSuite_TYPES_3D::ray3_getters)
    TEST_ADD(TestSuite_TYPES_3D::ray3_constructor)
    TEST_ADD(TestSuite_TYPES_3D::ray3_stream_insertion)
    
    TEST_ADD(TestSuite_TYPES_3D::poly3_members)
    TEST_ADD(TestSuite_TYPES_3D::poly3_constructor)
    TEST_ADD(TestSuite_TYPES_3D::poly3_getters)
    TEST_ADD(TestSuite_TYPES_3D::poly3_stream_insert)
  }
  
private:
  // 51 - 36 (+1) = 16 tests
  void vector3_members();
  void vector3_default_constructor();
  void vector3_constructor();
  void vector3_getters();
  void vector3_operators_dot();
  void vector3_operators_cross();
  void vector3_operators_project();
  void vector3_metric_distance();
  void vector3_metric_length();
  void vector3_transform_scale();
  void vector3_transform_normalize();
  void vector3_operators_plusequals();
  void vector3_operators_minusequals();
  void vector3_operators_plus();
  void vector3_operators_minus();
  void vector3_overload_stream_insertion();
  
  void ray3_members();
  void ray3_getters();
  void ray3_constructor();
  void ray3_stream_insertion();
  
  void poly3_members();
  void poly3_constructor();
  void poly3_getters();
  void poly3_stream_insert();
};


void TestSuite_TYPES_3D::vector3_members()
{
  vector3_t t;
  TEST_ASSERT_MSG(sizeof(t.coord) == sizeof(double)*3, "coord not right size");
}

void TestSuite_TYPES_3D::vector3_default_constructor()
{
  auto absTol = double{1E-8};
  
  vector3_t t;
  
  TEST_ASSERT_MSG(std::abs(t.coord[0] - 0) < absTol, "test 1")
  TEST_ASSERT_MSG(std::abs(t.coord[1] - 0) < absTol, "test 2")
  TEST_ASSERT_MSG(std::abs(t.coord[2] - 0) < absTol, "test 3")
}

void TestSuite_TYPES_3D::vector3_constructor()
{
  auto absTol = double{1E-8};
  
  auto x = double{1.0};
  auto y = double{2.0};
  auto z = double{3.0};
  
  vector3_t t{x,y,z};
  
  TEST_ASSERT_MSG(std::abs(t.coord[0] - x) < absTol, "test 1")
  TEST_ASSERT_MSG(std::abs(t.coord[1] - y) < absTol, "test 2")
  TEST_ASSERT_MSG(std::abs(t.coord[2] - z) < absTol, "test 3") 
}

void TestSuite_TYPES_3D::vector3_getters()
{
  auto absTol = double{1E-8};
  
  auto x = double{1.0};
  auto y = double{2.0};
  auto z = double{3.0};
  
  vector3_t t{x,y,z};
  
  TEST_ASSERT_MSG(std::abs(t.x() - x) < absTol, "test 1")
  TEST_ASSERT_MSG(std::abs(t.y() - y) < absTol, "test 2")
  TEST_ASSERT_MSG(std::abs(t.z() - z) < absTol, "test 3") 
}

void TestSuite_TYPES_3D::vector3_operators_dot()
{
  auto absTol = double{1E-8};
  
  auto x = double{1.0};
  auto y = double{2.0};
  auto z = double{3.0}; 
  
  auto i = double{4.0};
  auto j = double{5.0};
  auto k = double{6.0};
      
  vector3_t v1{x,y,z};
  vector3_t v2{i,j,k};
  
  double r = x*i + y*j + z*k;
  auto d = v1.dot(v2);
  
  TEST_ASSERT(std::abs(d-r) < absTol) 
}

void TestSuite_TYPES_3D::vector3_operators_cross()
{
  auto absTol = double{1E-8};
  
  auto x = double{1.0};
  auto y = double{2.0};
  auto z = double{3.0}; 
  
  auto i = double{4.0};
  auto j = double{5.0};
  auto k = double{6.0};
      
  auto a = y*k - z*j;
  auto b = z*i - x*k;
  auto c = x*j - y*i;
  
  vector3_t v1{x,y,z};
  vector3_t v2{i,j,k};
  vector3_t v3{a,b,c};
  
  auto p = v1.cross(v2);
  
  TEST_ASSERT( (std::abs(v3.x()-p.x()) < absTol) && 
               (std::abs(v3.y()-p.y()) < absTol) && 
               (std::abs(v3.z()-p.z()) < absTol)    )  
}

void TestSuite_TYPES_3D::vector3_operators_project()
{
  auto absTol = double{1E-8};
  
  auto x = double{2.0};
  auto y = double{3.0};
  auto z = double{4.0}; 
  
  auto i = double{1.0};
  auto j = double{0.0};
  auto k = double{0.0};
  
  double r = x*i + y*j + z*k;
  
  auto a = r * i;
  auto b = r * j;
  auto c = r * k;
    
  vector3_t v1{x,y,z};
  vector3_t v2{i,j,k};
  vector3_t v3{a,b,c};
  
  auto p = v1.project(v2);
  
  TEST_ASSERT( (std::abs(v3.x()-p.x()) < absTol) && 
               (std::abs(v3.y()-p.y()) < absTol) && 
               (std::abs(v3.z()-p.z()) < absTol)    )
}

void TestSuite_TYPES_3D::vector3_metric_distance()
{
  auto absTol = double{1E-8};
  
  auto x = double{1.0};
  auto y = double{2.0};
  auto z = double{3.0}; 
  
  auto i = double{4.0};
  auto j = double{5.0};
  auto k = double{6.0};
  
  auto d = sqrt((x-i)*(x-i) + (y-j)*(y-j) + (z-k)*(z-k));
  
  vector3_t v1{x,y,z};
  vector3_t v2{i,j,k};
  
  auto p = v1.distance(v2);
  
  TEST_ASSERT(std::abs(d-p) < absTol)
}

void TestSuite_TYPES_3D::vector3_metric_length()
{
  auto absTol = double{1E-8};
  
  auto x = double{1.0};
  auto y = double{2.0};
  auto z = double{3.0}; 
  
  auto r = sqrt(x*x + y*y + z*z);
  
  vector3_t v1{x,y,z};
  auto p = v1.length();
  
  TEST_ASSERT(std::abs(p-r) < absTol) 
}

void TestSuite_TYPES_3D::vector3_transform_scale()
{
  auto absTol = double{1E-8};
  
  auto s = double{1.0};
  
  auto x = double{1.0};
  auto y = double{2.0};
  auto z = double{3.0};
  
  auto a = x*s;
  auto b = y*s;
  auto c = z*s;
  
  vector3_t v1{x,y,z};
  v1.scale(s);
  
  TEST_ASSERT( (std::abs(v1.x()-a) < absTol) && 
               (std::abs(v1.y()-b) < absTol) && 
               (std::abs(v1.z()-c) < absTol)    )
}

void TestSuite_TYPES_3D::vector3_transform_normalize()
{
  auto absTol = double{1E-8};
  
  auto x = double{1.0};
  auto y = double{2.0};
  auto z = double{3.0};
  
  vector3_t v1{x,y,z};
  
  auto l = v1.length();
  
  auto a = x/l;
  auto b = y/l;
  auto c = z/l;
  
  v1.normalize();
  
  TEST_ASSERT( (std::abs(v1.x()-a) < absTol) && 
               (std::abs(v1.y()-b) < absTol) && 
               (std::abs(v1.z()-c) < absTol)    )
}

void TestSuite_TYPES_3D::vector3_operators_plusequals()
{
  auto absTol = double{1E-8};
  
  auto x = double{1.0};
  auto y = double{2.0};
  auto z = double{3.0}; 
  
  auto i = double{4.0};
  auto j = double{5.0};
  auto k = double{6.0};
  
  vector3_t v1{x,y,z};
  vector3_t v2{i,j,k};
  
  v1 += v2;
  
  TEST_ASSERT( (std::abs((x+i)-v1.x()) < absTol) && 
               (std::abs((y+j)-v1.y()) < absTol) && 
               (std::abs((z+k)-v1.z()) < absTol)    )  
}
void TestSuite_TYPES_3D::vector3_operators_minusequals()
{
  auto absTol = double{1E-8};
  
  auto x = double{1.0};
  auto y = double{2.0};
  auto z = double{3.0}; 
  
  auto i = double{4.0};
  auto j = double{5.0};
  auto k = double{6.0};
  
  vector3_t v1{x,y,z};
  vector3_t v2{i,j,k};
  
  v1 -= v2;
  
  TEST_ASSERT( (std::abs((x-i)-v1.x()) < absTol) && 
               (std::abs((y-j)-v1.y()) < absTol) && 
               (std::abs((z-k)-v1.z()) < absTol)    )  
}

void TestSuite_TYPES_3D::vector3_operators_plus()
{
  auto absTol = double{1E-8};
  
  auto x = double{1.0};
  auto y = double{2.0};
  auto z = double{3.0}; 
  
  auto i = double{4.0};
  auto j = double{5.0};
  auto k = double{6.0};
  
  vector3_t v1{x,y,z};
  vector3_t v2{i,j,k};
  
  auto v3 = v1 + v2;
  
  TEST_ASSERT( (std::abs((x+i)-v3.x()) < absTol) && 
               (std::abs((y+j)-v3.y()) < absTol) && 
               (std::abs((z+k)-v3.z()) < absTol)    )  
}

void TestSuite_TYPES_3D::vector3_operators_minus()
{
  auto absTol = double{1E-8};
  
  auto x = double{1.0};
  auto y = double{2.0};
  auto z = double{3.0}; 
  
  auto i = double{4.0};
  auto j = double{5.0};
  auto k = double{6.0};
  
  vector3_t v1{x,y,z};
  vector3_t v2{i,j,k};
  
  auto v3 = v1 - v2;
  
  TEST_ASSERT( (std::abs((x-i)-v3.x()) < absTol) && 
               (std::abs((y-j)-v3.y()) < absTol) && 
               (std::abs((z-k)-v3.z()) < absTol)    )  
}

void TestSuite_TYPES_3D::vector3_overload_stream_insertion()
{
  auto x = double{1.0};
  auto y = double{2.0};
  auto z = double{3.0};
  
  vector3_t v1{x,y,z};
  std::stringstream si;
  si << v1;
  std::string correct = "Vector(1, 2, 3)";
  
  TEST_ASSERT(correct.compare(si.str()) == 0)  
}

//ray3 tests
void TestSuite_TYPES_3D::ray3_members()
{
  TEST_ASSERT(false)
}

void TestSuite_TYPES_3D::ray3_getters()
{
  TEST_ASSERT(false)
}

void TestSuite_TYPES_3D::ray3_constructor()
{
  TEST_ASSERT(false)
}

void TestSuite_TYPES_3D::ray3_stream_insertion()
{
  TEST_ASSERT(false)
}

//poly3 tests
void TestSuite_TYPES_3D::poly3_members()
{
  TEST_ASSERT(false)
}

void TestSuite_TYPES_3D::poly3_constructor()
{
  TEST_ASSERT(false)
}

void TestSuite_TYPES_3D::poly3_getters()
{
  TEST_ASSERT(false)
}

void TestSuite_TYPES_3D::poly3_stream_insert()
{
  TEST_ASSERT(false)
}

#endif //OCTREE_TYPES_3D_UNIT_H