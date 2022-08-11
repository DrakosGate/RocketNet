# Global Project Setup
cmake_minimum_required(VERSION 3.22)
project(RocketNet)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Unit Testing
enable_testing()

# Generate API headers, even if we're not building DLLs
include(GenerateExportHeader)

# Specify export/output paths
set(EXPORT_PATH ${CMAKE_BINARY_DIR}/exports)
set(OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

function(set_output_directory name)
    set_target_properties(${name}
            PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_PATH}
            ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_PATH}
            RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_PATH})
endfunction()

#-------------------- GLOBAL CONFIGURATION ENDS HERE --------------------#
#-------------------- GLOBAL CONFIGURATION ENDS HERE --------------------#
#-------------------- GLOBAL CONFIGURATION ENDS HERE --------------------#

# Add Source Folder(s)
add_subdirectory(ThirdParty)
add_subdirectory(Source)