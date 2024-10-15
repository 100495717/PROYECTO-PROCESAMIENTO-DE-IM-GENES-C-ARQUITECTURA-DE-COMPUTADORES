# Add to this list all files related to imgaos library
add_library(imgaos
    ppm.cpp
)

# Use this line only if you have dependencies from this library to GSL
target_link_libraries (imgsoa PRIVATE common Microsoft.GSL:.GSL)
target_include_directories(imgsoa PUBLIC ${CMAKE_SOURCE_DIR}/include/imgsoa)