#include "huffman_code.h"

#include <algorithm>
#include <bitset>

#include "utils.h"

using namespace std;
using CodeInfo = HuffmanCode::CodeInfo;

namespace {
  void BuildCannonicalCodesLengths(vector<CodeInfo>& result, const HuffmanTree::Node::Ptr& root, uint code_length) {
    if (!root->left && !root->right) {
      result.push_back(CodeInfo{code_length, root->sym});
      return;
    }
    BuildCannonicalCodesLengths(result, root->left, code_length + 1);
    BuildCannonicalCodesLengths(result, root->right, code_length + 1);
  }
}  // namespace

HuffmanCode::HuffmanCode(const HuffmanTree& tree) {
  vector<CodeInfo> code_infos;
  BuildCannonicalCodesLengths(code_infos, tree.GetRoot(), 0);
  BuildSymbolInfos(move(code_infos));
}

void HuffmanCode::BuildSymbolInfos(vector<CodeInfo> code_infos) {
  sort(code_infos.begin(), code_infos.end(), [](const CodeInfo& lhs, const CodeInfo& rhs) {
    return make_pair(lhs.length, lhs.sym) < make_pair(rhs.length, rhs.sym);
  });

  uint current_code = 0, current_length = 0, next_length = 0;
  for (size_t i = 0; i < code_infos.size(); i++) {
    current_length = code_infos[i].length;

    {
      SymbolInfo symbol_info{current_code, current_length};
      symbol_infos_[code_infos[i].sym] = symbol_info;
      inverted_symbol_infos_[symbol_info] = code_infos[i].sym;
    }

    if (i < code_infos.size()) {
      next_length = code_infos[i + 1].length;
    } else {
      next_length = current_length;
    }
    current_code = (current_code + 1) << (next_length - current_length);
  }
}
const HuffmanCode::SymbolInfo& HuffmanCode::GetSymbolCompressionInfo(char sym) const { return symbol_infos_.at(sym); }

const char* HuffmanCode::FindSymbolByCode(SymbolInfo info) const {
  auto it = inverted_symbol_infos_.find(info);
  if (it == inverted_symbol_infos_.end()) {
    return nullptr;
  }
  return &it->second;
}

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
    string file_digest(digest_size, ' ');
    ReadPrimitive(in, &file_digest[0], file_digest.size());
    if (file_digest != "korotin.dev") {
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

const hash<uint> HuffmanCode::SymbolInfoHasher::hasher = hash<uint>();

bool HuffmanCode::SymbolInfo::operator==(const SymbolInfo& info) const {
  return code == info.code && length == info.length;
}

size_t HuffmanCode::SymbolInfoHasher::operator()(SymbolInfo info) const {
  size_t seed = hasher(info.code);
  size_t value = hasher(info.length);
  return value + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}