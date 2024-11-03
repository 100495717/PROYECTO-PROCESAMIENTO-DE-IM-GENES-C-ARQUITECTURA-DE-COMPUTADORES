#include "../common/binaryio.hpp"
#include "../common/progargs.hpp"
#include "../imgaos/imageaos.hpp"

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace common;
using namespace imgaos;

namespace {
  void info(InputArgs const & args) {
    std::vector<uint8_t> const data = BinaryIO::read_binary(args.getInput());
    const AOS image(data);

    std::cout << "Input: " << args.getInput() << "\n";
    std::cout << "Output: " << args.getOutput() << "\n";
    std::cout << "Operation: " << args.getCommandAsString() << "\n";
    std::cout << "Image size: " << image.getWidth() << "x" << image.getHeight() << "\n";
    std::cout << "Max level: " << image.getMaxLevel() << "\n";
  }

  void max_level(InputArgs const & args) {
    AOS image(BinaryIO::read_binary(args.getInput()));
    auto const & data = args.getMaxLevelData();
    image.max_level(data.getLevel());
    BinaryIO::write_binary(image.toBinary(), args.getOutput());
  }

  void resize(InputArgs const & args) {
    AOS image(BinaryIO::read_binary(args.getInput()));
    auto const & data = args.getResizeData();
    image.resize(data.getWidth(), data.getHeight());
    BinaryIO::write_binary(image.toBinary(), args.getOutput());
  }

  void cut_freq(InputArgs const & args) {
    AOS image(BinaryIO::read_binary(args.getInput()));
    auto const & data = args.getCutFreqData();
    image.cut_freq(data.getFreq());
    BinaryIO::write_binary(image.toBinary(), args.getOutput());
  }
}  // namespace

int main(int argc, char const * argv[]) {
  std::vector<std::string> const argvector(argv, argv + argc);
  try {
    InputArgs const args(argvector);
    switch (args.getCommand()) {
      case InputArgs::Command::Info:
        info(args);
        break;
      case InputArgs::Command::MaxLevel:
        max_level(args);
        break;
      case InputArgs::Command::Resize:
        resize(args);
        break;
      case InputArgs::Command::CutFreq:
        cut_freq(args);
        break;
      /*
    case InputArgs::Command::Compress:
      compress(args);
      break;
      */
      default:
        throw std::invalid_argument("no he programado eso aun");
    }

    return 0;
  } catch (std::invalid_argument const & err) {
    std::cerr << err.what() << "\n";
    exit(-1);
  } catch (std::runtime_error const & err) {
    std::cerr << err.what() << "\n";
    exit(-1);
  }
}
