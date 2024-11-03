#ifndef PROGARGS_HPP
#define PROGARGS_HPP

#include "aux.hpp"

#include <cstdint>
#include <cstdlib>
#include <gtest/gtest_prod.h>
#include <string>
#include <variant>
#include <vector>

namespace common {

  struct InputArgs {
      FRIEND_TEST(InputArgs, maxlevel);
      FRIEND_TEST(InputArgs, resize);
      FRIEND_TEST(InputArgs, cutfreq);

    public:
      class MaxLevelData {
        public:
          MaxLevelData(int level) : level(level) { }

          [[nodiscard]] int getLevel() const { return level; }

        private:
          int level;
      };

      class ResizeData {
        public:
          ResizeData(Width width, Height height)
            : width(width.getValue()), height(height.getValue()) { }

          [[nodiscard]] int getWidth() const { return width; }

          [[nodiscard]] int getHeight() const { return height; }

        private:
          int width, height;
      };

      class CutFreqData {
        public:
          CutFreqData(int freq) : freq(freq) { }

          [[nodiscard]] int getFreq() const { return freq; }

        private:
          int freq;
      };
      enum class Command : uint8_t { Info, MaxLevel, Resize, CutFreq, Compress };

      InputArgs(std::vector<std::string> const & argv_strings);

      [[nodiscard]] std::string getInput() const;
      [[nodiscard]] std::string getOutput() const;
      [[nodiscard]] Command getCommand() const;
      [[nodiscard]] std::string getCommandAsString() const;
      [[nodiscard]] MaxLevelData getMaxLevelData() const;
      [[nodiscard]] ResizeData getResizeData() const;
      [[nodiscard]] CutFreqData getCutFreqData() const;

    private:
      std::string input;
      std::string output;

      Command command = Command::MaxLevel;  // default, to avoid a compiler warning

      std::variant<MaxLevelData, ResizeData, CutFreqData> values =
          MaxLevelData(0);  // default, to avoid a compiler warning

      static void validate_info(int argc, std::vector<std::string> const & argv_strings);
      void validate_maxlevel(int argc, std::vector<std::string> const & argv_strings);
      void validate_resize(int argc, std::vector<std::string> const & argv_strings);
      void validate_cutfreq(int argc, std::vector<std::string> const & argv_strings);
      static void validate_compress(int argc, std::vector<std::string> const & argv_strings);
  };

}  // namespace common

#endif