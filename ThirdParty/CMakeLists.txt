#find_library(RakNet)
add_subdirectory(RakNet)
set(RAKNET_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/RakNet/include PARENT_SCOPE)
