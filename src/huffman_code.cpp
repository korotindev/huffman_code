#include "huffman_code.h"

#include <algorithm>

using namespace std;

namespace {
  struct CanonicalCodeInfo {
    uint length;
    char sym;
  };

  void BuildCannonicalCodesLegths(vector<CanonicalCodeInfo>& result, const HuffmanTree::Node::Ptr& root,
                                  uint code_length) {
    if (!root->left && !root->right) {
      result.push_back(CanonicalCodeInfo{code_length, root->sym});
      return;
    }
    BuildCannonicalCodesLegths(result, root->left, code_length + 1);
    BuildCannonicalCodesLegths(result, root->right, code_length + 1);
  }
}  // namespace

HuffmanCode::HuffmanCode(const HuffmanTree& tree) {
  vector<CanonicalCodeInfo> code_infos;
  BuildCannonicalCodesLegths(code_infos, tree.GetRoot(), 0);

  sort(code_infos.begin(), code_infos.end(), [](const CanonicalCodeInfo& lhs, const CanonicalCodeInfo& rhs) {
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

void HuffmanCode::SerializeTo(ostream& out) const {
  string digest = "korotin.dev";
  out << digest.size();
  out << digest;
  out << symbol_infos_.size();
  for(const auto &info : symbol_infos_) {
    out << info.first;
    out << info.second.length;
  }
}