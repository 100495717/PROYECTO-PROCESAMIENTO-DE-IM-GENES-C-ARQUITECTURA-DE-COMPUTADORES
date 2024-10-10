add library (imgsoa
    imagesoa.cpp
    ppm.cpp
)

target_link_libraries(imgsoa PRIVATE common Microsoft.GSL::GSL)
target_include_directories(imgsoa PUBLIC ${CMAKE_SOURCE_DIR}/include/imgsoa)