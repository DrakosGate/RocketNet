# Create the RocketNet library
add_library(RocketNet SHARED Source/library.cpp Source/library.extern.cpp)
target_precompile_headers(RocketNet PUBLIC library.pch)

# Link against RakNet
target_link_libraries(RocketNet PUBLIC RakNetLibStatic)
# Add RakNet include path(s)
target_include_directories(RocketNet PUBLIC ${EXPORT_PATH} ${RAKNET_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/Include)

# Generate an API file
generate_export_header(RocketNet
        EXPORT_MACRO_NAME ROCKETNET_API
        EXPORT_FILE_NAME ${EXPORT_PATH}/RocketNetApi.h)

set_output_directory(RocketNet)