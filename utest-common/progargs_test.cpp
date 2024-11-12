#include "../common/progargs.hpp"
#include <cstdint>
#include <fstream>
#include <gtest/gtest.h>

#include <stdexcept>
#include <vector>
#include <string>

TEST(progargs, no_valido) {
    int argc = 1;
    char* argv[] = {const_cast<char*>("imtool-aos")};
    ASSERT_THROW(parse_arguments(argc, argv), std::invalid_argument);
}




TEST(progargs, info_valido) {
    int argc = 4;
    char* argv[] = {const_cast<char*>("imtool-aos"), const_cast<char*>("deer-small.ppm"), const_cast<char*>("deer-small-mod.ppm"), const_cast<char*>("info")};
    std::vector<std::string> args = parse_arguments(argc, argv);
    std::vector<std::string> expected = {"imtool-aos", "deer-small.ppm", "deer-small-mod.ppm", "info"};
    ASSERT_EQ(args, expected);
}


TEST(progargs, maxlevel_valido) {
    int argc = 5;
    char* argv[] = {const_cast<char*>("imtool-aos"), const_cast<char*>("deer-small.ppm"), const_cast<char*>("deer-small-mod.ppm"), const_cast<char*>("maxlevel"), const_cast<char*>("100")};
    std::vector<std::string> args = parse_arguments(argc, argv);
    std::vector<std::string> expected = {"imtool-aos", "deer-small.ppm", "deer-small-mod.ppm", "maxlevel", "100"};
    ASSERT_EQ(args, expected);
}
TEST(progargs, maxlevel_no_valido) {
    int argc = 5;
    char* argv[] = {const_cast<char*>("imtool-aos"), const_cast<char*>("deer-small.ppm"), const_cast<char*>("deer-small-mod.ppm"), const_cast<char*>("maxlevel"), const_cast<char*>("0")};
    ASSERT_THROW(parse_arguments(argc, argv), std::invalid_argument);
}

TEST(progargs, resize_valido) {
    int argc = 6;
    char* argv[] = {const_cast<char*>("imtool-aos"), const_cast<char*>("deer-small.ppm"), const_cast<char*>("deer-small-mod.ppm"), const_cast<char*>("resize"), const_cast<char*>("100"), const_cast<char*>("100")};
    std::vector<std::string> args = parse_arguments(argc, argv);
    std::vector<std::string> expected = {"imtool-aos", "deer-small.ppm", "deer-small-mod.ppm", "resize", "100", "100"};
    ASSERT_EQ(args, expected);
}

TEST(progargs, resize_no_valido) {
    int argc = 5;
    char* argv[] = {const_cast<char*>("imtool-aos"), const_cast<char*>("deer-small.ppm"), const_cast<char*>("deer-small-mod.ppm"), const_cast<char*>("resize"), const_cast<char*>("10")};
    ASSERT_THROW(parse_arguments(argc, argv), std::invalid_argument);
}

TEST(progargs, cutfreq_valido) {
    int argc = 5;
    char* argv[] = {const_cast<char*>("imtool-aos"), const_cast<char*>("deer-small.ppm"), const_cast<char*>("deer-small-mod.ppm"), const_cast<char*>("cutfreq"), const_cast<char*>("100")};
    std::vector<std::string> args = parse_arguments(argc, argv);
    std::vector<std::string> expected = {"imtool-aos", "deer-small.ppm", "deer-small-mod.ppm", "cutfreq", "100"};
    ASSERT_EQ(args, expected);
}

TEST(progargs, cutfreq_no_valido) {
    int argc = 5;
    char* argv[] = {const_cast<char*>("imtool-aos"), const_cast<char*>("deer-small.ppm"), const_cast<char*>("deer-small-mod.ppm"), const_cast<char*>("cutfreq"), const_cast<char*>("0")};
    ASSERT_THROW(parse_arguments(argc, argv), std::invalid_argument);
}

TEST(progargs, compress_valido) {
    int argc = 4;
    char* argv[] = {const_cast<char*>("imtool-aos"), const_cast<char*>("deer-small.ppm"),const_cast<char*>("deer-small-mod.ppm"), const_cast<char*>("compress")};
    std::vector<std::string> args = parse_arguments(argc, argv);
    std::vector<std::string> expected = {"imtool-aos", "deer-small.ppm", "deer-small-mod.ppm", "compress"};
    ASSERT_EQ(args, expected);
}

TEST(progargs, compress_novalido) {
    int argc = 3;
    char* argv[] = {const_cast<char*>("imtool-aos"), const_cast<char*>("deer-small.ppm"), const_cast<char*>("compress")};

    ASSERT_THROW(parse_arguments(argc, argv), std::invalid_argument);
}
