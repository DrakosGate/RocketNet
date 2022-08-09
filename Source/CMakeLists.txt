add_library(RocketNet library.cpp main.cpp)
target_include_directories(RocketNet PUBLIC ${RAKNET_INCLUDE})
target_link_libraries(RocketNet PUBLIC ${RAKNET_LIBRARY})

if (MSVC)
    target_link_libraries(RocketNet PUBLIC Ws2_32)
endif ()

add_executable(RocketNetTest main.cpp)
target_link_libraries(RocketNetTest RocketNet)