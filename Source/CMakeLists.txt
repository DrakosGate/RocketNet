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

    # TODO: add customization here (e.g. specifying a custom COMMAND for add_test)
    # cmake_parse_arguments(prefix, bool_args, value_args, list_args, ${ARGN})
    # always prefix with TEST_<NAME>
    cmake_parse_arguments(TEST "" "COMMAND" "" ${ARGN})

    if (TEST_COMMAND)
        add_test(NAME ${name} COMMAND ${TEST_COMMAND} ${ARGN})
    else()
        add_test(NAME ${name} COMMAND RocketNetTest ${ARGN})
    endif()

    # "error" in any test log counts as a failure
    set_tests_properties(${name}
        PROPERTIES
            FAIL_REGULAR_EXPRESSION "error;exception;assert;fail"
    )

endfunction()

# Include any tests found in the Tests.cmake file
include(${CMAKE_SOURCE_DIR}/Tests.cmake)
