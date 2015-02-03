# Find CPPTest for use in unit level testing
#
# This tries to find the CPPTest library. Once completed it will set:
# CPPTEST_INCLUDE_DIRS - include directories for compilation
# CPPTEST_LIBRARIES    - libraries needed to link against
# CPPTEST_LIBRARY_DIRS - directories where to find the libraries
# CPPTest_FOUND        - boolean set to true if found
#
################################################################################
# Copyright (c) 2013 David D. Marshall <ddmarsha@calpoly.edu>
#
# All rights reserved. This program and the accompanying materials
# are made available under the terms of the Eclipse Public License v1.0
# which accompanies this distribution, and is available at
# http://www.eclipse.org/legal/epl-v10.html
#
# Contributors:
#    David D. Marshall - initial code and implementation
################################################################################

if(CPPTEST_LIBRARIES)
  set(CPPTEST_FOUND TRUE)
else()
  find_path(CPPTEST_INCLUDE_DIRS cpptest.h
            PATHS
              /usr/include
              /usr/local/include
              /opt/local/include
              ${CMAKE_INSTALL_PREFIX}/include
              $ENV{CPPTESTDIR}
              ${INCLUDE_INSTALL_DIR})
  find_library(CPPTEST_LIBRARIES cpptest
               PATHS
                 $ENV{CPPTESTDIR}
                 /usr/local/lib
                 /opt/local/lib
                 ${LIB_INSTALL_DIR})
  get_filename_component(CPPTEST_LIBRARY_DIRS ${CPPTEST_LIBRARIES} PATH)

  mark_as_advanced(CPPTEST_INCLUDE_DIRS CPPTEST_LIBRARIES CPPTEST_LIBRARY_DIRS)

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(CPPTest DEFAULT_MSG CPPTEST_INCLUDE_DIRS CPPTEST_LIBRARIES CPPTEST_LIBRARY_DIRS)
endif()