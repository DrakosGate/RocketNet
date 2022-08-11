add_test_ex(HostSmokeTest RocketNetHost
            PARAMS test)
#add_test_ex(ClientSmokeTest RocketNetClient
#            PARAMS test)
add_test_ex(CSSmokeTest RocketNetCS
            PARAMS test
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/RocketNetCS/bin/Debug/net6.0")

# add_test_ex(ExampleTest COMMAND python multi_test.py)