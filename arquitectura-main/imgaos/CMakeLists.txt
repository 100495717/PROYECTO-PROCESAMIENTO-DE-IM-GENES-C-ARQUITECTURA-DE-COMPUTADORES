# Add to this list all files related to imgaos library
add_library(imgaos
imageaos.cpp
resize.cpp
max_level.cpp
cut_freq.cpp
compress.cpp
#other2.cpp
)
target_link_libraries(imgaos PRIVATE common)
# Use this line only if you have dependencies from this library to GSL
# target_link_libraries (imgaos PRIVATE common Microsoft.GSL::GSL)
target_link_libraries (imgaos PRIVATE gtest)