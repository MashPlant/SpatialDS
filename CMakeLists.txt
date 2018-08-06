cmake_minimum_required(VERSION 3.10)
project(SpatialDS)


FIND_PACKAGE(OpenMP REQUIRED)
if(OPENMP_FOUND)
    message("OPENMP FOUND")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

add_executable(TestMain TestMain.cpp Vec.hpp Common.hpp Point.hpp SpatialDS.hpp SpatialDSImpl/KDTree.hpp SpatialDSImpl/Brute.hpp SpatialDSImpl/SortSweep.hpp SpatialDSImpl/PowSplitTree.hpp SpatialDSImpl/UniformGrid.hpp SpatialDSImpl/ControlGroup.hpp)

add_executable(GlutMain GlutMain.cpp Vec.hpp Common.hpp Point.hpp SpatialDS.hpp SpatialDSImpl/KDTree.hpp SpatialDSImpl/Brute.hpp SpatialDSImpl/SortSweep.hpp SpatialDSImpl/PowSplitTree.hpp SpatialDSImpl/UniformGrid.hpp SpatialDSImpl/ControlGroup.hpp CmdLine/CmdLine.hpp)

target_link_libraries (GlutMain glut GLU GL)