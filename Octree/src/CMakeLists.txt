file(GLOB SOURCES *.cpp)
add_executable (unit_tests ${SOURCES})

find_package(CppTest REQUIRED)
include_directories(${CPPTEST_INCLUDE_DIRS})
target_link_libraries(unit_tests ${CPPTEST_LIBRARIES})