add_library(RocketNet library.cpp)
target_include_directories(RocketNet PUBLIC ${RAKNET_INCLUDE})
target_link_libraries(RocketNet RakNet)
message(STATUS "goodbye ${RAKNET_INCLUDE}")