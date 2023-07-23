cmake_minimum_required(VERSION 3.25)

project(untitled1)

add_executable(untitled1 main.cpp)

# specify a target specific minimum here instead of globablly specifying the choice
target_compile_features(untitled1 PRIVATE cxx_std_17)

# add the following list of libraries to the libraries linked to target untitled1:
#   * aa
target_link_libraries(untitled1 PRIVATE aa)