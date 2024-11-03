# Add to this list all files related to common library
add_library(common
aux.cpp
progargs.cpp
binaryio.cpp
parsebinary.cpp
validatefile.cpp
)
# Use this line only if you have dependencies from this library to GSL
# target_link_libraries (common PRIVATE Microsoft.GSL::GSL)
target_link_libraries (common PRIVATE gtest)