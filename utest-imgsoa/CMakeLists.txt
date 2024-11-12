# Executable for all unit tests with list of sources
# For example, you mas have one *_test.cpp fpr each *.cpp in img-soa
add_executable(utest-imgsoa

    cutfreq_test.cpp
    resize_test.cpp
    info_test.cpp
    maxlevel_test.cpp
)

# Library dependencies
target_link_libraries(utest-imgsoa
    PRIVATE
    imgsoa
    GTest::gtest_main
    Microsoft.GSL::GSL
)

add_test(
    NAME utest-imgsoa
    COMMAND utest-imgsoa
)