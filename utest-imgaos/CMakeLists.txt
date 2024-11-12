# Executable for all unit tests with list of sources
# For example, you mas have one *_test.cpp fpr each *.cpp in img-aos
add_executable(utest-imgaos
    resize_test.cpp
    info_test.cpp
    compress_test.cpp
    cutfreq_test.cpp
    maxlevel_test.cpp
)

# Library dependencies
target_link_libraries(utest-imgaos
    PRIVATE
    imgaos
    GTest::gtest_main
    Microsoft.GSL::GSL
)


# Add test to CMake
add_test(
    NAME utest-imgaos
    COMMAND utest-imgaos
)