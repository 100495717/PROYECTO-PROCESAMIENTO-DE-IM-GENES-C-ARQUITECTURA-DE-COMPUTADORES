# Add to this list all files related to common library
add_library(common
    progargs.cpp
    binaryio.cpp
    header_ppm.cpp
    )

# Use this line only if you have dependencies from this library to GSL
target_link_libraries (common PRIVATE Microsoft.GSL::GSL)
target_include_directories(common PUBLIC ${CMAKE_SOURCE_DIR}/include/common)
