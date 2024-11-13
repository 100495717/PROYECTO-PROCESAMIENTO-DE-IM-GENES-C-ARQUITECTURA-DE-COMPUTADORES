#include "../common/progargs.hpp"
#include <cstdint>
#include <fstream>
#include <gtest/gtest.h>
#include <sstream>
#include <iostream>

#include <stdexcept>
#include <vector>
#include <string>

class ProgArgsTest : public ::testing::Test {
protected:
    std::stringstream buffer;
    std::streambuf* old;

    void SetUp() override {
        old = std::cerr.rdbuf(buffer.rdbuf());
    }

    void TearDown() override {
        std::cerr.rdbuf(old);
    }
};

TEST_F(ProgArgsTest, info_valido) {
    int argc = 4;
    char* argv[] = {const_cast<char*>("imtool-aos"), const_cast<char*>("deer-small.ppm"), const_cast<char*>("deer-small-mod.ppm"), const_cast<char*>("info")};
    std::vector<std::string> args = parse_arguments(argc, argv);
    std::vector<std::string> expected = {"imtool-aos", "deer-small.ppm", "deer-small-mod.ppm", "info"};
    ASSERT_EQ(args, expected);
}

TEST_F(ProgArgsTest, maxlevel_valido) {
    int argc = 5;
    char* argv[] = {const_cast<char*>("imtool-aos"), const_cast<char*>("deer-small.ppm"), const_cast<char*>("deer-small-mod.ppm"), const_cast<char*>("maxlevel"), const_cast<char*>("100")};
    std::vector<std::string> args = parse_arguments(argc, argv);
    std::vector<std::string> expected = {"imtool-aos", "deer-small.ppm", "deer-small-mod.ppm", "maxlevel", "100"};
    ASSERT_EQ(args, expected);
}

TEST_F(ProgArgsTest, maxlevel_no_valido) {
    int argc = 5;
    char* argv[] = {const_cast<char*>("imtool-aos"), const_cast<char*>("deer-small.ppm"), const_cast<char*>("deer-small-mod.ppm"), const_cast<char*>("maxlevel"), const_cast<char*>("0")};
    std::vector<std::string> args(argv, argv + argc);
    ASSERT_EQ(validate_arguments(args, argc), -1);
    std::string output = buffer.str();
    ASSERT_NE(output.find("Error: Invalid maxlevel: 0"), std::string::npos);
}

TEST_F(ProgArgsTest, resize_valido) {
    int argc = 6;
    char* argv[] = {const_cast<char*>("imtool-aos"), const_cast<char*>("deer-small.ppm"), const_cast<char*>("deer-small-mod.ppm"), const_cast<char*>("resize"), const_cast<char*>("100"), const_cast<char*>("100")};
    std::vector<std::string> args = parse_arguments(argc, argv);
    std::vector<std::string> expected = {"imtool-aos", "deer-small.ppm", "deer-small-mod.ppm", "resize", "100", "100"};
    ASSERT_EQ(args, expected);
}

TEST_F(ProgArgsTest, resize_no_valido) {
    int argc = 5;
    char* argv[] = {const_cast<char*>("imtool-aos"), const_cast<char*>("deer-small.ppm"), const_cast<char*>("deer-small-mod.ppm"), const_cast<char*>("resize"), const_cast<char*>("10")};
    std::vector<std::string> args(argv, argv + argc);
    ASSERT_EQ(validate_arguments(args, argc), -1);
    std::string output = buffer.str();
    ASSERT_NE(output.find("Error: Invalid number of extra arguments for resize: 1"), std::string::npos);
}

TEST_F(ProgArgsTest, cutfreq_valido) {
    int argc = 5;
    char* argv[] = {const_cast<char*>("imtool-aos"), const_cast<char*>("deer-small.ppm"), const_cast<char*>("deer-small-mod.ppm"), const_cast<char*>("cutfreq"), const_cast<char*>("100")};
    std::vector<std::string> args = parse_arguments(argc, argv);
    std::vector<std::string> expected = {"imtool-aos", "deer-small.ppm", "deer-small-mod.ppm", "cutfreq", "100"};
    ASSERT_EQ(args, expected);
}

TEST_F(ProgArgsTest, cutfreq_no_valido) {
    int argc = 5;
    char* argv[] = {const_cast<char*>("imtool-aos"), const_cast<char*>("deer-small.ppm"), const_cast<char*>("deer-small-mod.ppm"), const_cast<char*>("cutfreq"), const_cast<char*>("0")};
    std::vector<std::string> args(argv, argv + argc);
    ASSERT_EQ(validate_arguments(args, argc), -1);
    std::string output = buffer.str();
    ASSERT_NE(output.find("Error: Invalid cutfreq: 0"), std::string::npos);
}

TEST_F(ProgArgsTest, compress_valido) {
    int argc = 4;
    char* argv[] = {const_cast<char*>("imtool-aos"), const_cast<char*>("deer-small.ppm"),const_cast<char*>("deer-small-mod.ppm"), const_cast<char*>("compress")};
    std::vector<std::string> args = parse_arguments(argc, argv);
    std::vector<std::string> expected = {"imtool-aos", "deer-small.ppm", "deer-small-mod.ppm", "compress"};
    ASSERT_EQ(args, expected);
}

TEST_F(ProgArgsTest, compress_novalido) {
    int argc = 5;
    char* argv[] = {const_cast<char*>("imtool-aos"), const_cast<char*>("deer-small.ppm"), const_cast<char*>("deer-small-mod.ppm"), const_cast<char*>("compress"), const_cast<char*>("200")};
    std::vector<std::string> args(argv, argv + argc);
    ASSERT_EQ(validate_arguments(args, argc), -1);
    std::string output = buffer.str();
    ASSERT_NE(output.find("Error: Invalid extra arguments for compress"), std::string::npos);
}