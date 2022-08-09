# Global Project Setup
cmake_minimum_required(VERSION 3.22)
project(RocketNet)

# Unit Testing
include(CTest)
enable_testing()

# Add Source Folder(s)
add_subdirectory(ThirdParty)
add_subdirectory(Source)
