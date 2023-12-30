cmake_minimum_required(VERSION 3.25)
project(bee_io)

# ------------------------------ Project Settings --------------------------- #

# Set the C++ standard
set(CMAKE_CXX_STANDARD 23)

# Set the binary output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ------------------------------ Environment Variables ---------------------- #

# Set the http proxy
set(ENV{http_proxy} "http://192.168.49.1:8282")
set(ENV{https_proxy} "http://192.168.49.1:8282")

# ------------------------------ Project Directories ------------------------ #

# Create build directories
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Create include and src directories
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src)

# ------------------------------ Dependencies ------------------------------- #

# ------------------------------ Create Executables ------------------------- #

# Add executables
add_executable(bee_io main.cpp)
add_executable(bee_io_test test.cpp)

# ------------------------------ Create Libraries --------------------------- #

# Add libraries
add_library(bee_io_lib STATIC)

# ------------------------------ Gather Headers ----------------------------- #

# Main headers
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

# ------------------------------ Gather Sources ----------------------------- #

# Main sources
file(GLOB_RECURSE SOURCES main.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# Test sources
file(GLOB_RECURSE TEST_SOURCES test.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# ------------------------------ Include Directories ------------------------ #

# Main include directories
target_include_directories(bee_io PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(bee_io PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/compression)
target_include_directories(bee_io PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/file_formats)
target_include_directories(bee_io PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/file_formats/png)
target_include_directories(bee_io PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/image)

# Library include directories
target_include_directories(bee_io_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(bee_io_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/compression)
target_include_directories(bee_io_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/file_formats)
target_include_directories(bee_io_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/file_formats/png)
target_include_directories(bee_io_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/image)

# Test include directories
target_include_directories(bee_io_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(bee_io_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/compression)
target_include_directories(bee_io_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/file_formats)
target_include_directories(bee_io_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/file_formats/png)
target_include_directories(bee_io_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/image)

# ------------------------------ Add Sources -------------------------------- #

# Main sources
target_sources(bee_io PRIVATE ${SOURCES})

# Library sources
target_sources(bee_io_lib PRIVATE ${SOURCES})

# Test sources
target_sources(bee_io_test PRIVATE ${TEST_SOURCES})

# ------------------------------ Link Libraries ----------------------------- #

# Link zlib
target_link_libraries(bee_io z)
target_link_libraries(bee_io_lib z)
target_link_libraries(bee_io_test z)

# Link ws2_32
target_link_libraries(bee_io ws2_32)
target_link_libraries(bee_io_lib ws2_32)
target_link_libraries(bee_io_test ws2_32)

# ------------------------------ Setup bee_io_lib --------------------------- #

# MAKE SURE TO RELOAD CMAKE AFTER BUILDING THE LIBRARY

# Set the output directory for the library
set_target_properties(bee_io_lib PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/bee_io_lib)

# Delete the headers already in the include directory
file(REMOVE_RECURSE ${CMAKE_BINARY_DIR}/lib/bee_io_lib/include)

# Copy the headers to the include directory
file(COPY ${HEADERS} DESTINATION ${CMAKE_BINARY_DIR}/lib/bee_io_lib/include)