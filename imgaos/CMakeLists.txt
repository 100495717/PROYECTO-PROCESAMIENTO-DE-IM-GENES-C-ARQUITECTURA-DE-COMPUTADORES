# Add to this list all files related to imgaos library
add_library(imgaos
    image-operations-aos.cpp
)




# Use this line only if you have dependencies from this library to GSL
target_link_libraries (imgaos PRIVATE common gtest)
