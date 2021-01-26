#include "huffman_tree.h"

#include <algorithm>

using namespace std;

HuffmanTree::HuffmanTree(const HuffmanTree::CharStat& stat) {
  vector<Node::Ptr> pq;

  for (auto it : stat) {
    Node::Ptr node = make_unique<Node>();
    node->sym = it.first;
    node->count = it.second;
    pq.push_back(move(node));
  }

  auto comp = [](const Node::Ptr& lhs, const Node::Ptr& rhs) { return lhs->count > rhs->count; };
  make_heap(pq.begin(), pq.end(), comp);

  while (pq.size() > 1) {
    Node::Ptr node = make_unique<Node>();
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
  root_ = move(pq.front());
}

const HuffmanTree::Node::Ptr& HuffmanTree::GetRoot() const { return root_; }

HuffmanTree BuildHuffmanTreeFrom(istream& input) {
  HuffmanTree::CharStat stat;
  char c;
  while (input.get(c)) {
    stat[c]++;
  }
  return HuffmanTree(stat);
}