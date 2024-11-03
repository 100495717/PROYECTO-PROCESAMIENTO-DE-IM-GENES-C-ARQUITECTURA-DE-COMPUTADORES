#include "../common/progargs.hpp"

#include "gtest/gtest.h"
#include <stdexcept>
#include <string>
#include <vector>

// NOLINTBEGIN(cert-err58-cpp)
// NOLINTBEGIN(cppcoreguidelines-owning-memory)
// NOLINTBEGIN(cppcoreguidelines-avoid-goto)
// NOLINTBEGIN(modernize-type-traits)
// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
// NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast,-warnings-as-errors)
namespace common {

  TEST(InputArgs, cero_args) {
    std::vector<std::string> const argv = {"path/to/program"};
    try {
      InputArgs const input_args(argv);
    } catch (std::invalid_argument const & err) {
      EXPECT_EQ(err.what(), std::string("Error: Invalid number of arguments: 0"));
    }
  }

  TEST(InputArgs, un_arg) {
    std::vector<std::string> const argv = {"path/to/program", "archivo.ppm"};
    try {
      InputArgs const input_args(argv);
    } catch (std::invalid_argument const & err) {
      EXPECT_EQ(err.what(), std::string("Error: Invalid number of arguments: 1"));
    }
  }

  TEST(InputArgs, dos_args) {
    std::vector<std::string> const argv = {"path/to/program", "archivo.ppm", "salida.ppm"};
    try {
      InputArgs const input_args(argv);
    } catch (std::invalid_argument const & err) {
      EXPECT_EQ(err.what(), std::string("Error: Invalid number of arguments: 2"));
    }
  }

  TEST(InputArgs, op_no_valida) {
    std::vector<std::string> const argv = {"path/to/program", "archivo.ppm", "salida.ppm",
                                           "op_no_valida"};
    try {
      InputArgs const input_args(argv);
    } catch (std::invalid_argument const & err) {
      EXPECT_EQ(err.what(), std::string("Error: Invalid option: op_no_valida"));
    }
  }

  TEST(InputArgs, info) {
    std::vector<std::string> const argv = {"path/to/program", "archivo.ppm", "salida.ppm", "info"};
    InputArgs const input_args(argv);
    ASSERT_EQ(input_args.getInput(), "archivo.ppm");
    ASSERT_EQ(input_args.getOutput(), "salida.ppm");
    ASSERT_EQ(input_args.getCommand(), InputArgs::Command::Info);
  }

  TEST(InputArgs, info_1_extra_arg) {
    std::vector<std::string> const argv = {"path/to/program", "archivo.ppm", "salida.ppm", "info",
                                           "100"};
    try {
      InputArgs const input_args(argv);
    } catch (std::invalid_argument const & err) {
      EXPECT_EQ(err.what(), std::string("Error: Invalid extra arguments for info: 100"));
    }
  }

  TEST(InputArgs, info_2_extra_arg) {
    std::vector<std::string> const argv = {
      "path/to/program", "archivo.ppm", "salida.ppm", "info", "100", "200"};
    try {
      InputArgs const input_args(argv);
    } catch (std::invalid_argument const & err) {
      EXPECT_EQ(err.what(), std::string("Error: Invalid extra arguments for info: 100 200"));
    }
  }

  TEST(InputArgs, maxlevel_falta_1_arg) {
    std::vector<std::string> const argv = {"path/to/program", "archivo.ppm", "salida.ppm",
                                           "maxlevel"};
    try {
      InputArgs const input_args(argv);
    } catch (std::invalid_argument const & err) {
      EXPECT_EQ(err.what(),
                std::string("Error: Invalid number of extra arguments for maxlevel: 0"));
    }
  }

  TEST(InputArgs, maxlevel) {
    std::vector<std::string> const argv = {"path/to/program", "archivo.ppm", "salida.ppm",
                                           "maxlevel", "100"};
    InputArgs const input_args(argv);
    ASSERT_EQ(input_args.getInput(), "archivo.ppm");
    ASSERT_EQ(input_args.getOutput(), "salida.ppm");
    ASSERT_EQ(input_args.getCommand(), InputArgs::Command::MaxLevel);
    auto maxLevel = std::get<InputArgs::MaxLevelData>(input_args.values);
    ASSERT_EQ(maxLevel.getLevel(), 100);
  }

  TEST(InputArgs, maxlevel_sobra_1_arg) {
    std::vector<std::string> const argv = {"path/to/program", "archivo.ppm", "salida.ppm",
                                           "maxlevel",        "100",         "200"};
    try {
      InputArgs const input_args(argv);
    } catch (std::invalid_argument const & err) {
      EXPECT_EQ(err.what(),
                std::string("Error: Invalid number of extra arguments for maxlevel: 2"));
    }
  }

  TEST(InputArgs, maxlevel_negativo) {
    std::vector<std::string> const argv = {"path/to/program", "archivo.ppm", "salida.ppm",
                                           "maxlevel", "-1"};
    try {
      InputArgs const input_args(argv);
    } catch (std::invalid_argument const & err) {
      EXPECT_EQ(err.what(), std::string("Error: Invalid maxlevel: -1"));
    }
  }

  TEST(InputArgs, maxlevel_grande) {
    std::vector<std::string> const argv = {"path/to/program", "archivo.ppm", "salida.ppm",
                                           "maxlevel", "70000"};
    try {
      InputArgs const input_args(argv);
    } catch (std::invalid_argument const & err) {
      EXPECT_EQ(err.what(), std::string("Error: Invalid maxlevel: 70000"));
    }
  }

  TEST(InputArgs, maxlevel_texto) {
    std::vector<std::string> const argv = {"path/to/program", "archivo.ppm", "salida.ppm",
                                           "maxlevel", "texto"};
    try {
      InputArgs const input_args(argv);
    } catch (std::invalid_argument const & err) {
      EXPECT_EQ(err.what(), std::string("Error: Invalid maxlevel: texto"));
    }
  }

  TEST(InputArgs, resize_faltan_2_args) {
    std::vector<std::string> const argv = {"path/to/program", "archivo.ppm", "salida.ppm",
                                           "resize"};
    try {
      InputArgs const input_args(argv);
    } catch (std::invalid_argument const & err) {
      EXPECT_EQ(err.what(), std::string("Error: Invalid number of extra arguments for resize: 0"));
    }
  }

  TEST(InputArgs, resize_falta_1_arg) {
    std::vector<std::string> const argv = {"path/to/program", "archivo.ppm", "salida.ppm", "resize",
                                           "100"};
    try {
      InputArgs const input_args(argv);
    } catch (std::invalid_argument const & err) {
      EXPECT_EQ(err.what(), std::string("Error: Invalid number of extra arguments for resize: 1"));
    }
  }

  TEST(InputArgs, resize) {
    std::vector<std::string> const argv = {"path/to/program", "archivo.ppm", "salida.ppm",
                                           "resize",          "100",         "200"};
    InputArgs const input_args(argv);
    ASSERT_EQ(input_args.getInput(), "archivo.ppm");
    ASSERT_EQ(input_args.getOutput(), "salida.ppm");
    ASSERT_EQ(input_args.getCommand(), InputArgs::Command::Resize);
    auto resize = std::get<InputArgs::ResizeData>(input_args.values);
    ASSERT_EQ(resize.getWidth(), 100);
    ASSERT_EQ(resize.getHeight(), 200);
  }

  TEST(InputArgs, resize_sobra_1_arg) {
    std::vector<std::string> const argv = {
      "path/to/program", "archivo.ppm", "salida.ppm", "resize", "100", "200", "300"};
    try {
      InputArgs const input_args(argv);
    } catch (std::invalid_argument const & err) {
      EXPECT_EQ(err.what(), std::string("Error: Invalid number of extra arguments for resize: 3"));
    }
  }

  TEST(InputArgs, resize_width_negativo) {
    std::vector<std::string> const argv = {"path/to/program", "archivo.ppm", "salida.ppm",
                                           "resize",          "-100",        "200"};
    try {
      InputArgs const input_args(argv);
    } catch (std::invalid_argument const & err) {
      EXPECT_EQ(err.what(), std::string("Error: Invalid resize width: -100"));
    }
  }

  TEST(InputArgs, resize_height_negativo) {
    std::vector<std::string> const argv = {"path/to/program", "archivo.ppm", "salida.ppm",
                                           "resize",          "100",         "-200"};
    try {
      InputArgs const input_args(argv);
    } catch (std::invalid_argument const & err) {
      EXPECT_EQ(err.what(), std::string("Error: Invalid resize height: -200"));
    }
  }

  TEST(InputArgs, resize_texto) {
    std::vector<std::string> const argv = {"path/to/program", "archivo.ppm", "salida.ppm",
                                           "resize",          "100",         "texto"};
    try {
      InputArgs const input_args(argv);
    } catch (std::invalid_argument const & err) {
      EXPECT_EQ(err.what(), std::string("Error: Invalid resize height: texto"));
    }
  }

  TEST(InputArgs, cutfreq_faltan_1_arg) {
    std::vector<std::string> const argv = {"path/to/program", "archivo.ppm", "salida.ppm",
                                           "cutfreq"};
    try {
      InputArgs const input_args(argv);
    } catch (std::invalid_argument const & err) {
      EXPECT_EQ(err.what(), std::string("Error: Invalid number of extra arguments for cutfreq: 0"));
    }
  }

  TEST(InputArgs, cutfreq) {
    std::vector<std::string> const argv = {"path/to/program", "archivo.ppm", "salida.ppm",
                                           "cutfreq", "100"};
    InputArgs const input_args(argv);
    ASSERT_EQ(input_args.getInput(), "archivo.ppm");
    ASSERT_EQ(input_args.getOutput(), "salida.ppm");
    ASSERT_EQ(input_args.getCommand(), InputArgs::Command::CutFreq);
    auto cutfreq = std::get<InputArgs::CutFreqData>(input_args.values);
    ASSERT_EQ(cutfreq.getFreq(), 100);
  }

  TEST(InputArgs, cutfreq_sobra_1_arg) {
    std::vector<std::string> const argv = {"path/to/program", "archivo.ppm", "salida.ppm",
                                           "cutfreq",         "100",         "200"};
    try {
      InputArgs const input_args(argv);
    } catch (std::invalid_argument const & err) {
      EXPECT_EQ(err.what(), std::string("Error: Invalid number of extra arguments for cutfreq: 2"));
    }
  }

  TEST(InputArgs, cutfreq_freq_negativa) {
    std::vector<std::string> const argv = {"path/to/program", "archivo.ppm", "salida.ppm",
                                           "cutfreq", "-100"};
    try {
      InputArgs const input_args(argv);
    } catch (std::invalid_argument const & err) {
      EXPECT_EQ(err.what(), std::string("Error: Invalid cutfreq: -100"));
    }
  }

  TEST(InputArgs, cutfreq_texto) {
    std::vector<std::string> const argv = {"path/to/program", "archivo.ppm", "salida.ppm",
                                           "cutfreq", "texto"};
    try {
      InputArgs const input_args(argv);
    } catch (std::invalid_argument const & err) {
      EXPECT_EQ(err.what(), std::string("Error: Invalid cutfreq: texto"));
    }
  }

  TEST(InputArgs, compress) {
    std::vector<std::string> const argv = {"path/to/program", "archivo.ppm", "salida.ppm",
                                           "compress"};
    InputArgs const input_args(argv);
    ASSERT_EQ(input_args.getInput(), "archivo.ppm");
    ASSERT_EQ(input_args.getOutput(), "salida.ppm");
    ASSERT_EQ(input_args.getCommand(), InputArgs::Command::Compress);
  }

  TEST(InputArgs, compress_sobra_1_arg) {
    std::vector<std::string> const argv = {"path/to/program", "archivo.ppm", "salida.ppm",
                                           "compress", "100"};
    try {
      InputArgs const input_args(argv);
      FAIL() << "Expected std::invalid_argument";
    } catch (std::invalid_argument const & err) {
      EXPECT_EQ(err.what(), std::string("Error: Invalid extra arguments for compress: 100"));
    }
  }

  TEST(getCommandAsString, Info) {
    std::vector<std::string> const argv = {"path/to/program", "archivo.ppm", "salida.ppm", "info"};
    InputArgs const input_args(argv);

    EXPECT_EQ("info", input_args.getCommandAsString());
  }

  TEST(getCommandAsString, MaxLevel) {
    std::vector<std::string> const argv = {"path/to/program", "archivo.ppm", "salida.ppm",
                                           "maxlevel", "100"};
    InputArgs const input_args(argv);

    EXPECT_EQ("maxlevel", input_args.getCommandAsString());
  }

  TEST(getCommandAsString, Resize) {
    std::vector<std::string> const argv = {"path/to/program", "archivo.ppm", "salida.ppm",
                                           "resize",          "100",         "200"};
    InputArgs const input_args(argv);
    EXPECT_EQ("resize", input_args.getCommandAsString());
  }

  TEST(getCommandAsString, CutFreq) {
    std::vector<std::string> const argv = {"path/to/program", "archivo.ppm", "salida.ppm",
                                           "cutfreq", "100"};
    InputArgs const input_args(argv);
    EXPECT_EQ("cutfreq", input_args.getCommandAsString());
  }

  TEST(getCommandAsString, Compress) {
    std::vector<std::string> const argv = {"path/to/program", "archivo.ppm", "salida.ppm",
                                           "compress"};
    InputArgs const input_args(argv);
    EXPECT_EQ("compress", input_args.getCommandAsString());
  }

  TEST(getMaxLevelData, normal) {
    std::vector<std::string> const argv = {"path/to/program", "archivo.ppm", "salida.ppm",
                                           "maxlevel", "100"};
    InputArgs const input_args(argv);
    EXPECT_EQ(100, input_args.getMaxLevelData().getLevel());
  }

  TEST(getMaxLevelData, invalid) {
    std::vector<std::string> const argv = {"path/to/program", "archivo.ppm", "salida.ppm",
                                           "resize",          "100",         "200"};
    InputArgs const input_args(argv);
    try {
      (void) input_args.getMaxLevelData();
      FAIL() << "Expected std::invalid_argument";
    } catch (std::invalid_argument const & err) {
      EXPECT_EQ(err.what(), std::string("Error: Invalid command for maxlevel"));
    }
  }

  TEST(getResizeData, normal) {
    std::vector<std::string> const argv = {"path/to/program", "archivo.ppm", "salida.ppm",
                                           "resize",          "100",         "200"};
    InputArgs const input_args(argv);
    EXPECT_EQ(100, input_args.getResizeData().getWidth());
    EXPECT_EQ(200, input_args.getResizeData().getHeight());
  }

  TEST(getResizeData, invalid) {
    std::vector<std::string> const argv = {"path/to/program", "archivo.ppm", "salida.ppm",
                                           "cutfreq", "100"};
    InputArgs const input_args(argv);
    try {
      (void) input_args.getResizeData();
      FAIL() << "Expected std::invalid_argument";
    } catch (std::invalid_argument const & err) {
      EXPECT_EQ(err.what(), std::string("Error: Invalid command for resize"));
    }
  }

  TEST(getCutFreqData, normal) {
    std::vector<std::string> const argv = {"path/to/program", "archivo.ppm", "salida.ppm",
                                           "cutfreq", "100"};
    InputArgs const input_args(argv);
    EXPECT_EQ(100, input_args.getCutFreqData().getFreq());
  }

  TEST(getCutFreqData, invalid) {
    std::vector<std::string> const argv = {"path/to/program", "archivo.ppm", "salida.ppm",
                                           "maxlevel", "100"};
    InputArgs const input_args(argv);
    try {
      (void) input_args.getCutFreqData();
      FAIL() << "Expected std::invalid_argument";
    } catch (std::invalid_argument const & err) {
      EXPECT_EQ(err.what(), std::string("Error: Invalid command for cutfreq"));
    }
  }
}  // namespace common

// NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast,-warnings-as-errors)
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
// NOLINTEND(modernize-type-traits)
// NOLINTEND(cppcoreguidelines-avoid-goto)
// NOLINTEND(cppcoreguidelines-owning-memory)
// NOLINTEND(cert-err58-cpp)