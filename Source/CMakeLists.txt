# Create the RocketNet library
add_library(RocketNet library.cpp)
# Link against RakNet
target_link_libraries(RocketNet PUBLIC RakNetLibStatic)
# Add RakNet include path(s)
target_include_directories(RocketNet PUBLIC ${RAKNET_INCLUDE_DIRS})

# -----------------------------------------------------------------
# Tests

# Create the RocketNet test executable
add_executable(RocketNetTest main.cpp)
target_link_libraries(RocketNetTest RocketNet)

# Define a function to help with adding new tests
function (add_test_ex name)
    add_test(NAME ${name} COMMAND RocketNetTest ${ARGN})
    set_property(TEST ${name} PROPERTY FAIL_REGULAR_EXPRESSION "error")
endfunction()

# Include any tests found in the Tests.cmake file
include(${CMAKE_SOURCE_DIR}/Tests.cmake)
