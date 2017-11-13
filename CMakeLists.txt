project(particle_filter_in_cpp)

cmake_minimum_required (VERSION 3.5)

add_definitions(-std=c++11)
set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

set(gaussian_sampling_sources src/gaussian_sampling.cpp)
add_executable(gaussian_sampling ${gaussian_sampling_sources})

set(car_map_transformation_sources src/car_map_transformation.cpp)
add_executable(car_map_transformation ${car_map_transformation_sources})
