#include "huffman_tree.h"

#include <queue>

using namespace std;

HuffmanTree::HuffmanTree(const HuffmanTree::CharStat& stat) {
  vector<NodePtr> pq;

  for (auto it : stat) {
    NodePtr node = make_unique<Node>();
    node->sym = it.first;
    node->count = it.second;
    pq.push_back(move(node));
  }

  auto comp = [](const NodePtr& lhs, const NodePtr& rhs) {
    return lhs->count > rhs->count;
  };
  make_heap(pq.begin(), pq.end(), comp);

  while (pq.size() > 1) {
    NodePtr node = make_unique<Node>();
    node->left = move(pq.front());
    node->count += node->left->count;
    pop_heap(pq.begin(), pq.end(), comp);
    pq.pop_back();

    node->right = move(pq.front());
    node->count += node->right->count;
    pop_heap(pq.begin(), pq.end(), comp);
    pq.pop_back();

    pq.push_back(move(node));
    push_heap(pq.begin(), pq.end(), comp);
  }
  root = move(pq.front());
}

namespace {
void BuildMinimalCode(const HuffmanTree::NodePtr& node,
                      HuffmanTree::SymBitmask& sym_bitmask,
                      std::string& bitmask) {
  if (!node->left && !node->right) {
    sym_bitmask[node->sym] = bitmask;
    return;
  }
  bitmask.push_back('0');
  BuildMinimalCode(node->left, sym_bitmask, bitmask);
  bitmask.back() = '1';
  BuildMinimalCode(node->right, sym_bitmask, bitmask);
  bitmask.pop_back();
}
}  // namespace

HuffmanTree::SymBitmask HuffmanTree::BuildSymBitmask() const {
  SymBitmask sym_bitmask;
  string bitmask;
  BuildMinimalCode(root, sym_bitmask, bitmask);
  return sym_bitmask;
}
