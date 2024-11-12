#Add to this list all files related to imgaos library
add_library(imgsoa
    image-operations-soa.cpp 
    ppm-soa.cpp
)

#Use this line only if you have dependencies from this library to GSL
target_link_libraries (imgsoa PRIVATE common gtest)