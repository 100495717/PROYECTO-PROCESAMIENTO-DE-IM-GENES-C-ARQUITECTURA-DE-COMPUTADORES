# Executable for all unit tests with list of sources
# For example, you mas have one *_test.cpp fpr each *.cpp in img-aos
add_executable(utest-imgaos
    test_imgaos.cpp
)

# Library dependencies
target_link_libraries(utest-imgaos
    PRIVATE
    img-aos
    GTest::gtest_main
    Microsoft.GSL::GSL
)