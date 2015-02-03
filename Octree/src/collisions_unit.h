#ifndef OCTREE_COLLISIONS_UNIT_H
#define OCTREE_COLLISIONS_UNIT_H

#include "collisions.h"

class TestSuite_COLLISIONS : public Test::Suite
{
public:
  TestSuite_COLLISIONS()
  {
    TEST_ADD(TestSuite_COLLISIONS::collisions_sphere3_vector3)
    TEST_ADD(TestSuite_COLLISIONS::collisions_sphere3_sphere3)
    TEST_ADD(TestSuite_COLLISIONS::collisions_sphere3_ray3)
    TEST_ADD(TestSuite_COLLISIONS::collisions_sphere3_ray3_time)
    TEST_ADD(TestSuite_COLLISIONS::collisions_poly3_vector3)
    TEST_ADD(TestSuite_COLLISIONS::collisions_sphere3_poly3)
    TEST_ADD(TestSuite_COLLISIONS::collisions_poly3_ray3)
  }
  
private:
  void collisions_sphere3_vector3();
  void collisions_sphere3_sphere3();
  void collisions_sphere3_ray3();
  void collisions_sphere3_ray3_time();
  void collisions_poly3_vector3();
  void collisions_sphere3_poly3();
  void collisions_poly3_ray3();
};

void TestSuite_COLLISIONS::collisions_sphere3_vector3()
{
  vector3_t c1{1,1,1};
  vector3_t c2{-1,-1,-1};
  
  // add a small epsilon to the radius to be sure of double comparison on border case
  auto r = (c1-c2).length() + 1e-12;
  
  // border case
  TEST_ASSERT(Collisions::sphere3_vector3(c1, r, c2) == true)
  // radius is slightly too small
  TEST_ASSERT(Collisions::sphere3_vector3(c1, r-0.1, c2) == false)
  // radius is slightly larger than it needs to be
  TEST_ASSERT(Collisions::sphere3_vector3(c1, r+0.1, c2) == true)
}

void TestSuite_COLLISIONS::collisions_sphere3_sphere3()
{
  vector3_t c1{1,1,1};
  vector3_t c2{-1,-1,-1};
  
  // add a small epsilon to the radius to be sure of double comparison on border case
  auto r = (c1-c2).length()/2.0 + 1e-12;
  
  // border case
  TEST_ASSERT(Collisions::sphere3_sphere3(c1, r, c2, r) == true)
  // radius is slightly too small
  TEST_ASSERT(Collisions::sphere3_sphere3(c1, r-0.1, c2, r) == false)
  TEST_ASSERT(Collisions::sphere3_sphere3(c1, r, c2, r-0.1) == false)
  //radius is slightly larger than needed
  TEST_ASSERT(Collisions::sphere3_sphere3(c1, r+0.1, c2, r) == true)
  TEST_ASSERT(Collisions::sphere3_sphere3(c1, r, c2, r+0.1) == true)
}

void TestSuite_COLLISIONS::collisions_sphere3_ray3()
{
  vector3_t c;
  auto r = double{1};
  
  // forwards and backwards through center of sphere
  ray3_t r1{vector3_t{-2,0,0}, vector3_t{1,0,0}};
  ray3_t r2{vector3_t{2,0,0}, vector3_t{-1,0,0}};
  TEST_ASSERT(Collisions::sphere3_ray3(c, r, r1) == true)
  TEST_ASSERT(Collisions::sphere3_ray3(c, r, r2) == true)
  
  //tangent to sphere
  ray3_t r3{vector3_t{-1,1,0}, vector3_t{1,0,0}};
  TEST_ASSERT(Collisions::sphere3_ray3(c, r, r3) == true)
  
  // slightly miss sphere
  ray3_t r4{vector3_t{-1,1 + 1e-12,0}, vector3_t{1,0,0}};
  TEST_ASSERT(Collisions::sphere3_ray3(c, r, r4) == false)
  
  // pointing away from sphere
  ray3_t r5{vector3_t{-2,0,0}, vector3_t{-1,0,0}};
  TEST_ASSERT(Collisions::sphere3_ray3(c, r, r5) == false)
  
  // forwards and backwards through center of sphere, but tranlated in the z axis
  vector3_t c2;
  ray3_t r6{vector3_t{-2,0,1}, vector3_t{1,0,0}};
  ray3_t r7{vector3_t{2,0,1}, vector3_t{-1,0,0}};
  TEST_ASSERT(Collisions::sphere3_ray3(c2, r, r6) == true)
  TEST_ASSERT(Collisions::sphere3_ray3(c2, r, r7) == true)
  
  // slightly miss sphere translated in the z axis
  ray3_t r8{vector3_t{-1,1 + 1e-12, 1}, vector3_t{1,0,0}};
  TEST_ASSERT(Collisions::sphere3_ray3(c2, r, r8) == false)
}

void TestSuite_COLLISIONS::collisions_sphere3_ray3_time()
{
  TEST_ASSERT(false)
}

void TestSuite_COLLISIONS::collisions_poly3_vector3()
{
  TEST_ASSERT(false)
}

void TestSuite_COLLISIONS::collisions_sphere3_poly3()
{
  TEST_ASSERT(false)
}

void TestSuite_COLLISIONS::collisions_poly3_ray3()
{
  TEST_ASSERT(false)
}




#endif // OCTREE_COLLISIONS_UNIT_H