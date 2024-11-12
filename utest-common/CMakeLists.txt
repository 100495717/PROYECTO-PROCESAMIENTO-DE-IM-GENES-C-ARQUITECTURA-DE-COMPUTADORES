# Executable for all unit tests with list of sources
# For example, you mas have one *_test.cpp fpr each *.cpp in common
add_executable(utest-common
    binaryio_test.cpp
    progargs_test.cpp
)

# Library dependencies
target_link_libraries (utest-common 
    PRIVATE 
    common 
    GTest::gtest_main 
    Microsoft.GSL::GSL
)

# Add test to CMake
add_test(
    NAME utest-common
    COMMAND utest-common
)