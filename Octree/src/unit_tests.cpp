#include <memory>
#include <iostream>
#include <fstream>
#include <cpptest.h>

#include "types_3d_unit.h"
#include "collisions_unit.h"

int main()
{
  Test::Suite ts;
  
  #pragma clang diagnostic ignored "-Wdeprecated-declarations"
  ts.add(std::auto_ptr<Test::Suite>(new TestSuite_TYPES_3D));
  ts.add(std::auto_ptr<Test::Suite>(new TestSuite_COLLISIONS));
  
  //Test::TextOutput output(Test::TextOutput::Verbose);
  Test::HtmlOutput output;

  // run the tests
  auto result = ts.run(output);
  
  std::ofstream f;
  f.open("results.html");
  //output.generate(std::cout, true, "MyTest"); // can output directly to terminal
  output.generate(f, true, "Octree");

  return result;
}