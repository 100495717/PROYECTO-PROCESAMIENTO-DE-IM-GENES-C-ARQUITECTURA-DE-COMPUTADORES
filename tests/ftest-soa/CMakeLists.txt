# Define the executable for functional tests in ftest-soa
add_executable(ftest-soa
    ftest_soa.cpp
)

# Link the necessary libraries
target_link_libraries(ftest-soa
    PRIVATE
    img-soa
    GTest::gtest_main
    Microsoft.GSL::GSL
)

# Add the test to CTest
add_test(NAME ftest-soa COMMAND ftest-soa)