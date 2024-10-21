# Executable for all unit tests with list of sources
# For example, you mas have one *_test.cpp fpr each *.cpp in img-soa
add_executable(utest-imgsoa
    test_imgsoa.cpp
)

# Library dependencies
target_link_libraries(utest-imgsoa
    PRIVATE
    img-soa
    GTest::gtest_main
    Microsoft.GSL::GSL
)