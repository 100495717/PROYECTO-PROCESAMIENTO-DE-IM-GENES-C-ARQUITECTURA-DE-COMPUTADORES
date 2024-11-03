#include "progargs.hpp"

#include <cstdlib>
#include <exception>
#include <stdexcept>
#include <string>
#include <vector>

namespace common {

  constexpr int MAX_MAXLEVEL      = 65535;
  constexpr int ARGC_MIN          = 4;
  constexpr int ARGC_INFO         = 4;
  constexpr int ARGC_MAXLEVEL     = 5;
  constexpr int ARGC_RESIZE       = 6;
  constexpr int ARGC_CUTFREQ      = 5;
  constexpr int ARGC_COMPRESS     = 4;
  constexpr int POSITION_MAXLEVEL = 4;
  constexpr int POSITION_WIDTH    = 4;
  constexpr int POSITION_HEIGHT   = 5;
  constexpr int POSITION_FREQ     = 4;

  InputArgs::InputArgs(std::vector<std::string> const & argv_strings) {
    int const argc = static_cast<int>(argv_strings.size());

    if (argc < ARGC_MIN) {
      throw std::invalid_argument("Error: Invalid number of arguments: " +
                                  std::to_string(argc - 1));
    }

    input                           = argv_strings[1];
    output                          = argv_strings[2];
    std::string const & command_str = argv_strings[3];
    if (command_str == "info") {
      validate_info(argc, argv_strings);
      command = Command::Info;
    } else if (command_str == "maxlevel") {
      validate_maxlevel(argc, argv_strings);
      command = Command::MaxLevel;
    } else if (command_str == "resize") {
      validate_resize(argc, argv_strings);
      command = Command::Resize;
    } else if (command_str == "cutfreq") {
      validate_cutfreq(argc, argv_strings);
      command = Command::CutFreq;
    } else if (command_str == "compress") {
      validate_compress(argc, argv_strings);
      command = Command::Compress;
    } else {
      throw std::invalid_argument("Error: Invalid option: " + command_str);
    }
  }

  void InputArgs::validate_info(int argc, std::vector<std::string> const & argv_strings) {
    if (argc != ARGC_INFO) {
      std::string extra_args;
      for (int i = ARGC_MIN; i < argc; i++) {
        extra_args += argv_strings[static_cast<size_t>(i)];
        if (i < argc - 1) { extra_args += " "; }
      }
      throw std::invalid_argument("Error: Invalid extra arguments for info: " + extra_args);
    }
  }

  void InputArgs::validate_maxlevel(int argc, std::vector<std::string> const & argv_strings) {
    if (argc != ARGC_MAXLEVEL) {
      throw std::invalid_argument("Error: Invalid number of extra arguments for maxlevel: " +
                                  std::to_string(argc - ARGC_MIN));
    }
    std::string const & level_str = argv_strings[POSITION_MAXLEVEL];

    try {
      int const level = std::stoi(level_str);
      if (level <= 0 || level > MAX_MAXLEVEL) {
        throw std::invalid_argument("Error: Invalid maxlevel: " + level_str);
      }
      values = MaxLevelData(level);
    } catch (std::exception const & e) {
      throw std::invalid_argument("Error: Invalid maxlevel: " + level_str);
    }
  }

  void InputArgs::validate_resize(int argc, std::vector<std::string> const & argv_strings) {
    if (argc != ARGC_RESIZE) {
      throw std::invalid_argument("Error: Invalid number of extra arguments for resize: " +
                                  std::to_string(argc - ARGC_MIN));
    }
    std::string const & width_str = argv_strings[POSITION_WIDTH];
    int width                     = 0;
    try {
      width = std::stoi(width_str);
    } catch (std::exception const & e) {
      throw std::invalid_argument("Error: Invalid resize width: " + width_str);
    }
    if (width <= 0) { throw std::invalid_argument("Error: Invalid resize width: " + width_str); }
    std::string const & height_str = argv_strings[POSITION_HEIGHT];

    int height = 0;
    try {
      height = std::stoi(height_str);
    } catch (std::exception const & e) {
      throw std::invalid_argument("Error: Invalid resize height: " + height_str);
    }
    if (height <= 0) { throw std::invalid_argument("Error: Invalid resize height: " + height_str); }
    values = ResizeData(Width(width), Height(height));
  }

  void InputArgs::validate_cutfreq(int argc, std::vector<std::string> const & argv_strings) {
    if (argc != ARGC_CUTFREQ) {
      throw std::invalid_argument("Error: Invalid number of extra arguments for cutfreq: " +
                                  std::to_string(argc - ARGC_MIN));
    }
    std::string const & freq_str = argv_strings[POSITION_FREQ];

    int freq = 0;
    try {
      freq = std::stoi(freq_str);
    } catch (std::exception const & e) {
      throw std::invalid_argument("Error: Invalid cutfreq: " + freq_str);
    }
    if (freq <= 0) { throw std::invalid_argument("Error: Invalid cutfreq: " + freq_str); }
    values = CutFreqData(freq);
  }

  void InputArgs::validate_compress(int argc, std::vector<std::string> const & argv_strings) {
    if (argc != ARGC_COMPRESS) {
      std::string extra_args;
      for (int i = ARGC_MIN; i < argc; i++) {
        extra_args += argv_strings[static_cast<size_t>(i)];
        if (i < argc - 1) { extra_args += " "; }
      }
      throw std::invalid_argument("Error: Invalid extra arguments for compress: " + extra_args);
    }
  }

  std::string InputArgs::getCommandAsString() const {
    switch (command) {
      case InputArgs::Command::Info:
        return {"info"};
      case InputArgs::Command::MaxLevel:
        return {"maxlevel"};
      case InputArgs::Command::Resize:
        return {"resize"};
      case InputArgs::Command::CutFreq:
        return {"cutfreq"};
      case InputArgs::Command::Compress:
        return {"compress"};
      default:
        return {"unreachable"};
    }
  }

  InputArgs::MaxLevelData InputArgs::getMaxLevelData() const {
    if (command != InputArgs::Command::MaxLevel) {
      throw std::invalid_argument("Error: Invalid command for maxlevel");
    }
    return std::get<InputArgs::MaxLevelData>(values);
  }

  InputArgs::ResizeData InputArgs::getResizeData() const {
    if (command != InputArgs::Command::Resize) {
      throw std::invalid_argument("Error: Invalid command for resize");
    }
    return std::get<InputArgs::ResizeData>(values);
  }

  InputArgs::CutFreqData InputArgs::getCutFreqData() const {
    if (command != InputArgs::Command::CutFreq) {
      throw std::invalid_argument("Error: Invalid command for cutfreq");
    }
    return std::get<InputArgs::CutFreqData>(values);
  }

  std::string InputArgs::getInput() const {
    return input;
  }

  std::string InputArgs::getOutput() const {
    return output;
  }

  InputArgs::Command InputArgs::getCommand() const {
    return command;
  }

}  // namespace common