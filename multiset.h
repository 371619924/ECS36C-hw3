#ifndef MULTISET_H_
#define MULTISET_H_

#include <cstddef>
#include <algorithm>
#include <iostream>
#include <memory>
#include <sstream>
#include <utility>

template <typename K>
class Multiset
{
public:
  //
  // Public API
  //

  // * Capacity
  // Returns number of items in multiset --O(1)
  size_t Size();
  // Returns true if multiset is empty --O(1)
  bool Empty();

  // * Modifiers
  // Inserts an item corresponding to @key in multiset --O(log N) on average
  void Insert(const K &key);
  // Removes an item corresponding to @key from multiset --O(log N) on average
  //  Throws exception if key doesn't exist
  void Remove(const K &key);

  // * Lookup
  // Return whether @key is found in multiset --O(log N) on average
  bool Contains(const K &key);
  // Returns number of items matching @key in multiset --O(log N) on average
  //  Throws exception if key doesn't exist
  size_t Count(const K &key);

  // Return greatest key less than or equal to @key --O(log N) on average
  //  Throws exception if multiset is empty or no floor exists for key
  const K &Floor(const K &key);
  // Return least key greater than or equal to @key --O(log N) on average
  //  Throws exception if multiset is empty or no ceil exists for key
  const K &Ceil(const K &key);

  // Return max key in multiset --O(log N) on average
  //  Throws exception if multiset is empty
  const K &Max();
  // Return min key in multiset --O(log N) on average
  //  Throws exception if multiset is empty
  const K &Min();

  Multiset() : root_(nullptr), size_(0) {}
  ~Multiset() = default;

private:
  //
  // @@@ The class's internal members below can be modified @@@
  //

  // Private member variables
  // ...To be completed...

  // Private constants
  // ...To be completed (if any)...

  // Private methods
  // ...To be completed (if any)...

  struct Node
  {
    K key;
    size_t count;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
    Node(const K &k) : key(k), count(1), left(nullptr), right(nullptr) {}
  };
  std::unique_ptr<Node> root;
  size_t size;

  void Insert(std::unique_ptr<Node> &node, const K &key)
  {
    if (!node)
    {
      node = std::make_unique<Node>(key);
    }
    else if (key < node->key)
    {
      Insert(node->left, key);
    }
    else if (node->key < key)
    {
      Insert(node->right, key);
    }
    else
    {
      node->count++;
    }
  }

  Node *Find(const std::unique_ptr<Node> &node, const K &key) const
  {
    if (!node)
      return nullptr;
    if (key < node->key)
      return Find(node->left, key);
    else if (node->key < key)
      return Find(node->right, key);
    else
      return node.get();
  }

  void Remove(std::unique_ptr<Node>& node, const K& key, bool &found) {
      if (!node) return;
      if (key < node->key) {
          Remove(node->left, key, found);
      } else if (node->key < key) {
          Remove(node->right, key, found);
      } else {         
          found = true;
          if (node->count > 1) {             
              node->count--;
              return;
          }      
          if (!node->left) {
              node = std::move(node->right);
          } else if (!node->right) {
              node = std::move(node->left);
          } else {            
              Node* succ = FindMin(node->right.get());             
              node->key = succ->key;           
              if (succ->count > 1) {
                  succ->count--;
              } else {
                  bool dummy = false;
                  Remove(node->right, succ->key, dummy);
              }
          }
      }
  }


  Node* Floor(Node* node, const K& key) const {
      if (!node) return nullptr;
      if (node->key == key)
          return node;
      if (node->key > key)
          return Floor(node->left.get(), key);
      
      Node* temp = Floor(node->right.get(), key);
      return (temp != nullptr) ? temp : node;
  }

  
  Node* Ceil(Node* node, const K& key) const {
      if (!node) return nullptr;
      if (node->key == key)
          return node;
      if (node->key < key)
          return Ceil(node->right.get(), key);
      
      Node* temp = Ceil(node->left.get(), key);
      return (temp != nullptr) ? temp : node;
  }

  Node* FindMin(Node* node) const {
      if (!node) return nullptr;
      while (node->left) {
          node = node->left.get();
      }
      return node;
  }

  
  Node* FindMax(Node* node) const {
      if (!node) return nullptr;
      while (node->right) {
          node = node->right.get();
      }
      return node;
  }
};

//
// Your implementation of the class should be located below
//

// ...To be completed...


template <typename K>
size_t Multiset<K>::Size() {
    return size_;
}

template <typename K>
bool Multiset<K>::Empty() {
    return size_ == 0;
}

template <typename K>
void Multiset<K>::Insert(const K &key) {
    Insert(root_, key);
    size_++;
}

template <typename K>
void Multiset<K>::Remove(const K &key) {
    bool found = false;
    Remove(root_, key, found);
    if (!found) {
        throw std::runtime_error("Invalid key");
    }
    size_--;
}

template <typename K>
bool Multiset<K>::Contains(const K& key) {
    return Find(root_, key) != nullptr;
}

template <typename K>
size_t Multiset<K>::Count(const K& key) {
    Node* node = Find(root_, key);
    if (!node) {
        throw std::runtime_error("Invalid key");
    }
    return node->count;
}

template <typename K>
const K& Multiset<K>::Floor(const K &key) {
    if (Empty()) {
        throw std::runtime_error("Empty multiset");
    }
    Node* candidate = Floor(root_.get(), key);
    if (!candidate || candidate->key > key) {
        throw std::runtime_error("Invalid key");
    }
    return candidate->key;
}

template <typename K>
const K& Multiset<K>::Ceil(const K &key) {
    if (Empty()) {
        throw std::runtime_error("Empty multiset");
    }
    Node* candidate = Ceil(root_.get(), key);
    if (!candidate || candidate->key < key) {
        throw std::runtime_error("Invalid key");
    }
    return candidate->key;
}

template <typename K>
const K& Multiset<K>::Max() {
    if (Empty()) {
        throw std::runtime_error("Empty multiset");
    }
    Node* maxNode = FindMax(root_.get());
    return maxNode->key;
}

template <typename K>
const K& Multiset<K>::Min() {
    if (Empty()) {
        throw std::runtime_error("Empty multiset");
    }
    Node* minNode = FindMin(root_.get());
    return minNode->key;
}

#endif // MULTISET_H_
