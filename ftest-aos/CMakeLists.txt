# Define the executable for functional tests in ftest-aos
add_executable(ftest-aos
    ftest_aos.cpp
)

# Link the necessary libraries
target_link_libraries(ftest-aos
    PRIVATE
    img-aos
    GTest::gtest_main
    Microsoft.GSL::GSL
)

# Add the test to CTest
add_test(NAME ftest-aos COMMAND ftest-aos)