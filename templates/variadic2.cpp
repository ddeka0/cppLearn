#include <bits/stdc++.h>
using namespace std;

// define binary tree structure and traverse helpers:
struct Node {
  int value;
  Node* left;
  Node* right;
  Node(int i=0) : value(i), left(nullptr), right(nullptr) {
  }
  friend int operator+(Node::left x)
  //...
};

auto _left = &Node::left;
auto _right = &Node::right;

// traverse tree, using fold expression:
template<typename T, typename... TP>
Node* traverse (T np, TP... paths) {
  return (np + ... + paths);      // np ->* paths1 ->* paths2 ...
}

int main()
{
  // init binary tree structure:
  Node* root = new Node{1};
  root->left = new Node{2};
  root->left->right = new Node{3};
  root->left->right->right = new Node{4};
  //...
  // traverse binary tree:
  Node* node = traverse(root, _left, _right,_right);
  //...
}
