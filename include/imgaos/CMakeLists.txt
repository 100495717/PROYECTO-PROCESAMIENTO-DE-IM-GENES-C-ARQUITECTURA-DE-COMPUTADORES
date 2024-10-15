add_library(imgaos
    image-operations.cpp
)

target_link_libraries(imgaos PRIVATE common Microsoft.GSL::GSL)
target_include_directories(imgaos PUBLIC ${CMAKE_SOURCE_DIR}/include/imgaos)