#include "huffman_code.h"

#include <algorithm>
#include <type_traits>

using namespace std;
using CodeInfo = HuffmanCode::CodeInfo;

namespace {
  void BuildCannonicalCodesLegths(vector<CodeInfo>& result, const HuffmanTree::Node::Ptr& root, uint code_length) {
    if (!root->left && !root->right) {
      result.push_back(CodeInfo{code_length, root->sym});
      return;
    }
    BuildCannonicalCodesLegths(result, root->left, code_length + 1);
    BuildCannonicalCodesLegths(result, root->right, code_length + 1);
  }
}  // namespace

HuffmanCode::HuffmanCode(const HuffmanTree& tree) {
  vector<CodeInfo> code_infos;
  BuildCannonicalCodesLegths(code_infos, tree.GetRoot(), 0);
  BuildSymbolInfos(move(code_infos));
}

void HuffmanCode::BuildSymbolInfos(vector<CodeInfo> code_infos) {
  sort(code_infos.begin(), code_infos.end(), [](const CodeInfo& lhs, const CodeInfo& rhs) {
    return make_pair(lhs.length, lhs.sym) < make_pair(rhs.length, rhs.sym);
  });

  uint current_code = 0, current_length = 0, next_length = 0;
  for (size_t i = 0; i < code_infos.size(); i++) {
    current_length = code_infos[i].length;
    symbol_infos_[code_infos[i].sym] = SymbolInfo{current_code, current_length};
    if (i + 1 < code_infos.size()) {
      next_length = current_length;
    } else {
      next_length = code_infos[i + 1].length;
    }
    current_code = (current_code + 1) << (next_length - current_length);
  }
}
const HuffmanCode::SymbolInfo& HuffmanCode::GetSymbolCompressionInfo(char sym) const { return symbol_infos_.at(sym); }

namespace {
  template <typename T>
  void WritePrimitive(ostream& out, T t, size_t n = sizeof(T)) {
    static_assert(is_integral<T>::value, "Integral required.");
    out.write(reinterpret_cast<const char*>(&t), n);
  }

  template <typename T>
  void WritePrimitive(ostream& out, const T* t, size_t n = sizeof(T)) {
    static_assert(is_integral<T>::value, "Integral required.");
    out.write(reinterpret_cast<const char*>(t), n);
  }

  template <typename T>
  void ReadPrimitive(istream& in, T& t, size_t n = sizeof(T)) {
    static_assert(is_integral<T>::value, "Integral required.");
    in.read(reinterpret_cast<char*>(&t), n);
  }

  template <typename T>
  void ReadPrimitive(istream& in, T* t, size_t n = sizeof(T)) {
    static_assert(is_integral<T>::value, "Integral required.");
    in.read(reinterpret_cast<char*>(t), n);
  }
}  // namespace

void HuffmanCode::SerializeTo(ostream& out) const {
  {
    string digest = "korotin.dev";
    WritePrimitive(out, digest.size());
    WritePrimitive(out, digest.c_str(), digest.size());
  }
  WritePrimitive(out, symbol_infos_.size());
  for (const auto& info_pair : symbol_infos_) {
    WritePrimitive(out, info_pair.first);
    WritePrimitive(out, info_pair.second.length);
  }
}

HuffmanCode HuffmanCode::DeserializeFrom(istream& in) {
  {
    size_t digest_size;
    ReadPrimitive(in, digest_size);
    std::vector<char> file_digest(' ', digest_size);
    ReadPrimitive(in, file_digest.data(), file_digest.size());
    string digest = "korotin.dev";
    string file_digest_str(file_digest.begin(), file_digest.end());
    if (digest != file_digest_str) {
      throw runtime_error("Digest is missing!");
    }
  }

  size_t table_size;
  ReadPrimitive(in, table_size);
  std::vector<CodeInfo> code_infos;
  for (size_t i = 0; i < table_size; i++) {
    CodeInfo info;
    ReadPrimitive(in, info.sym);
    ReadPrimitive(in, info.length);
    code_infos.push_back(info);
  }

  HuffmanCode code;
  code.BuildSymbolInfos(move(code_infos));
  return code;
}