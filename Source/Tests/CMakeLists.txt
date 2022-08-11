# Create the RocketNet test executable
add_executable(RocketNetHost Host.cpp)
target_link_libraries(RocketNetHost RocketNet)
set_output_directory(RocketNetHost)

add_executable(RocketNetClient Client.cpp)
target_link_libraries(RocketNetClient RocketNet)
set_output_directory(RocketNetClient)

# Define a function to help with adding new tests
function (add_test_ex name command)

    # TODO: add customization here (e.g. specifying a custom COMMAND for add_test)
    # cmake_parse_arguments(prefix, bool_args, value_args, list_args, ${ARGN})
    # always prefix with TEST_<NAME>
    cmake_parse_arguments(TEST "" "WORKING_DIRECTORY" "PARAMS" ${ARGN})

    if (DEFINED TEST_WORKING_DIRECTORY)
        add_test(NAME ${name}
                COMMAND ${TEST_WORKING_DIRECTORY}/${command} ${TEST_PARAMS}
                WORKING_DIRECTORY "${TEST_WORKING_DIRECTORY}")
    else()
        add_test(NAME ${name}
                COMMAND ${command} ${TEST_PARAMS})
    endif()

    # "error" in any test log counts as a failure
    set_tests_properties(${name}
            PROPERTIES
            FAIL_REGULAR_EXPRESSION "error;exception;assert;fail"
            )

endfunction()

# Include any tests found in the Tests.cmake file
include(${CMAKE_CURRENT_SOURCE_DIR}/Tests.cmake)
