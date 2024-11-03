# Executable for all unit tests with list of sources
# For example, you may have one ∗_test.cpp for each ∗.cpp in img−aos
add_executable(utest-imgaos
imageaos_test.cpp
imageaos_resize_test.cpp
imageaos_cut_freq_test.cpp
imageaos_max_level_test.cpp
imageaos_compress_test.cpp)
# Library dependencies
target_link_libraries (utest-imgaos
PRIVATE
imgaos
GTest::gtest_main
Microsoft.GSL::GSL)

add_test(NAME utest-imgaos COMMAND utest-imgaos)