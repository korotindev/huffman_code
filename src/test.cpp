#include <algorithm>
#include <chrono>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "compressor.h"
#include "utils.h"

using namespace std;

namespace {
  void test(size_t id, const string& str) {
    cout << "Test " << id << ": ";
    istringstream in(str);
    stringstream binary;

    {
      auto tree = BuildHuffmanTreeFrom(in);
      HuffmanCode huffman_code(tree);
      huffman_code.SerializeTo(binary);
      Compressor compressor(huffman_code);
      in.clear();
      in.seekg(0);
      compressor.Encode(in, binary);
    }

    in.clear();
    in.seekg(0);
    binary.clear();
    binary.seekg(0);
    ostringstream out;
    {
      auto code = HuffmanCode::DeserializeFrom(binary);
      Compressor compressor(code);
      compressor.Decode(binary, out);
    }

    if (in.str() != out.str()) {
      cout << "FAILED\n"
           << "Expected: " << in.str() << " (size: " << in.str().size() << ")\n"
           << "Result: " << out.str() << " (size: " << out.str().size() << ")\n\n";
    } else {
      cout << "OK\n";
    }
  }

  void test_assert(size_t id, const string& str) {
    try {
      test(id, str);
    } catch (exception& ex) {
      cout << "OK\n";
      return;
    } catch (...) {
      exception_ptr p = std::current_exception();
      cout << "FAILED. \n"
           << "Expected: std::exception\n"
           << "Result: " << (p ? p.__cxa_exception_type()->name() : "null");
      return;
    }
    cout << "FAILED. std::exception expected\n";
  }

  std::string random_string(size_t length) {
    auto randchar = []() -> char {
      const char charset[] =
          "0123456789"
          "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
          "abcdefghijklmnopqrstuvwxyz";
      const size_t max_index = (sizeof(charset) - 1);
      return charset[rand() % max_index];
    };
    std::string str(length, 0);
    std::generate_n(str.begin(), length, randchar);
    return str;
  }

  void test_big_strings(size_t first_test_id) {
    using namespace std::chrono;

    cout << "Performance runs: \n";

    constexpr ulong string_size = 10000000;
     vector<string> test_strs{
        random_string(string_size),
        random_string(2 * string_size),
        random_string(10 * string_size),
    };

    cout << "tests generated\n";

    for (const auto& str : test_strs) {
      auto start = high_resolution_clock::now();
      test(first_test_id++, str);
      auto stop = high_resolution_clock::now();
      duration<float> duration = stop - start;
      cout << "(size:" << str.size() / (1024.0 * 1024.0) << "mb)";
      cout << "(test_time: " << duration.count() << "s)\n";
    }
  }
}  // namespace

int main() {
  vector<string> tests = {
      "L",
      "Lorem impsum",
      "Lorem impsum dolor sit amet",
      "Lorem ipsum dolor sit amet, consectetur adipiscing elit.",
  };
  tests.push_back("");

  for (int c = 0; c < 256; c++) {
    tests.back().push_back(c);
  }

  for (size_t i = 0; i < tests.size(); i++) {
    test(i, tests[i]);
  }

  test_assert(tests.size(), "");
  test_big_strings(tests.size() + 1);

  return 0;
}