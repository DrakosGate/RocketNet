# Create the RocketNet library
add_library(RocketNet Source/library.cpp)
# Link against RakNet
target_link_libraries(RocketNet PUBLIC RakNetLibStatic)
# Add RakNet include path(s)
target_include_directories(RocketNet PUBLIC ${RAKNET_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/Include)