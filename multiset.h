#ifndef MULTISET_H_
#define MULTISET_H_

#include <cstddef>     // For size_t.
#include <algorithm>   // For std::min, std::max (if needed).
#include <iostream>    // For debugging output (if needed).
#include <memory>      // For std::unique_ptr.
#include <sstream>     // For string stream operations.
#include <utility>     // For std::move.

/*
Detailed Description:
This header defines a template class "Multiset" that implements a multiset (or bag) data
structure using a binary search tree. Each node stores a key and a count indicating the
number of occurrences of that key. The class supports logarithmic-time (on average) insertion,
removal, lookup, and various queries (floor, ceil, minimum, maximum). It also maintains the
total number of items in the multiset.
*/

template <typename K>
class Multiset {
public:
    // ---------------- Public API ----------------

    // Capacity:
    // Returns the total number of items stored in the multiset (O(1)).
    size_t Size();
    // Returns true if the multiset is empty (O(1)).
    bool Empty();

    // Modifiers:
    // Inserts an occurrence of key into the multiset (average O(log N)).
    void Insert(const K &key);
    // Removes one occurrence of key from the multiset.
    // Throws an exception if key is not present (average O(log N)).
    void Remove(const K &key);

    // Lookup:
    // Returns whether key is present in the multiset (average O(log N)).
    bool Contains(const K &key);
    // Returns the number of occurrences of key.
    // Throws an exception if key is not present (average O(log N)).
    size_t Count(const K &key);

    // Queries:
    // Returns the greatest key that is less than or equal to the given key (floor).
    // Throws an exception if the multiset is empty or no such key exists (average O(log N)).
    const K &Floor(const K &key);
    // Returns the least key that is greater than or equal to the given key (ceil).
    // Throws an exception if the multiset is empty or no such key exists (average O(log N)).
    const K &Ceil(const K &key);
    // Returns the maximum key in the multiset.
    // Throws an exception if the multiset is empty (average O(log N)).
    const K &Max();
    // Returns the minimum key in the multiset.
    // Throws an exception if the multiset is empty (average O(log N)).
    const K &Min();

    // Constructor: Initializes an empty multiset.
    Multiset() : root(nullptr), size(0) {}
    // Destructor: Uses default since unique_ptr handles cleanup.
    ~Multiset() = default;

private:
    // ---------------- Internal Members ----------------

    // Node structure representing a tree node in the BST.
    struct Node {
        K key;                          // The key stored in this node.
        size_t count;                   // Number of times key appears.
        std::unique_ptr<Node> left;     // Left subtree.
        std::unique_ptr<Node> right;    // Right subtree.
        // Constructor: Initializes a node with key and count set to 1.
        Node(const K &k) : key(k), count(1), left(nullptr), right(nullptr) {}
    };

    std::unique_ptr<Node> root;  // Root of the BST.
    size_t size;                 // Total number of items in the multiset.

    // ---------------- Internal Helper Methods ----------------



    

    // Recursively insert key into the BST starting at node.
    void Insert(std::unique_ptr<Node> &node, const K &key) {
        if (!node) {
            // If the subtree is empty, create a new node.
            node.reset(new Node(key));
        }
        else if (key < node->key) {
            // Insert into left subtree.
            Insert(node->left, key);
        }
        else if (node->key < key) {
            // Insert into right subtree.
            Insert(node->right, key);
        }
        else {
            // Key already exists: increment its count.
            node->count++;
        }
    }

    // Recursively search for key in the BST starting at node.
    // Returns a raw pointer to the node (or nullptr if not found).
    Node *Find(const std::unique_ptr<Node> &node, const K &key) const {
        if (!node)
            return nullptr;
        if (key < node->key)
            return Find(node->left, key);
        else if (node->key < key)
            return Find(node->right, key);
        else
            return node.get();
    }

    // Recursively remove one occurrence of key from the BST starting at node.
    // 'found' is set to true if key was found and removed.
    void Remove(std::unique_ptr<Node> &node, const K &key, bool &found) {
        if (!node)
            return;
        if (key < node->key) {
            Remove(node->left, key, found);
        }
        else if (node->key < key) {
            Remove(node->right, key, found);
        }
        else {
            // Key found.
            found = true;
            if (node->count > 1) {
                // More than one occurrence: just decrement count.
                node->count--;
                return;
            }
            // Node has count == 1: remove the node from the tree.
            if (!node->left) {
                // No left child: replace node with its right subtree.
                node = std::move(node->right);
            }
            else if (!node->right) {
                // No right child: replace node with its left subtree.
                node = std::move(node->left);
            }
            else {
                // Node has two children: find the in-order successor.
                Node *succ = FindMin(node->right.get());
                // Copy the successor's key into current node.
                node->key = succ->key;
                if (succ->count > 1) {
                    // If successor appears more than once, just decrement its count.
                    succ->count--;
                }
                else {
                    // Otherwise, remove the successor node.
                    bool dummy = false;
                    Remove(node->right, succ->key, dummy);
                }
            }
        }
    }

    // Returns the node with the greatest key that is <= key.
    // Recursively searches the BST.
    Node *Floor(Node *node, const K &key) const {
        if (!node)
            return nullptr;
        if (node->key == key)
            return node;
        if (node->key > key)
            return Floor(node->left.get(), key);
        // Else, key is greater than node->key. Try to find a closer candidate in the right subtree.
        Node *temp = Floor(node->right.get(), key);
        return (temp != nullptr) ? temp : node;
    }

    // Returns the node with the least key that is >= key.
    Node *Ceil(Node *node, const K &key) const {
        if (!node)
            return nullptr;
        if (node->key == key)
            return node;
        if (node->key < key)
            return Ceil(node->right.get(), key);
        // Else, key is less than node->key. Try to find a better candidate in the left subtree.
        Node *temp = Ceil(node->left.get(), key);
        return (temp != nullptr) ? temp : node;
    }

    // Returns the node with the minimum key in the subtree.
    Node *FindMin(Node *node) const {
        if (!node)
            return nullptr;
        while (node->left) {
            node = node->left.get();
        }
        return node;
    }

    // Returns the node with the maximum key in the subtree.
    Node *FindMax(Node *node) const {
        if (!node)
            return nullptr;
        while (node->right) {
            node = node->right.get();
        }
        return node;
    }
};

// ---------------- Implementation of Public Methods ----------------















// Returns the total number of items (including duplicates) in the multiset.
template <typename K>
size_t Multiset<K>::Size() {
    return size;
}

// Returns true if the multiset is empty.
template <typename K>
bool Multiset<K>::Empty() {
    return size == 0;
}

// Inserts an occurrence of key into the multiset.
template <typename K>
void Multiset<K>::Insert(const K &key) {
    Insert(root, key); // Recursively insert key into the BST.
    size++;            // Increment the total item count.
}

// Removes one occurrence of key from the multiset.
// Throws an exception if key is not found.
template <typename K>
void Multiset<K>::Remove(const K &key) {
    bool found = false;
    Remove(root, key, found); // Recursively remove key.
    if (!found) {
        throw std::runtime_error("Invalid key");
    }
    size--; // Decrement the total count.
}

// Returns true if key is present in the multiset.
template <typename K>
bool Multiset<K>::Contains(const K &key) {
    return Find(root, key) != nullptr;
}

// Returns the number of occurrences of key.
// Throws an exception if key is not found.
template <typename K>
size_t Multiset<K>::Count(const K &key) {
    Node *node = Find(root, key);
    if (!node) {
        throw std::runtime_error("Invalid key");
    }
    return node->count;
}

// Returns the greatest key less than or equal to key (floor).
// Throws an exception if the multiset is empty or no floor exists.
template <typename K>
const K &Multiset<K>::Floor(const K &key) {
    if (Empty()) {
        throw std::runtime_error("Empty multiset");
    }
    Node *candidate = Floor(root.get(), key);
    if (!candidate || candidate->key > key) {
        throw std::runtime_error("Invalid key");
    }
    return candidate->key;
}

// Returns the least key greater than or equal to key (ceil).
// Throws an exception if the multiset is empty or no ceil exists.
template <typename K>
const K &Multiset<K>::Ceil(const K &key) {
    if (Empty()) {
        throw std::runtime_error("Empty multiset");
    }
    Node *candidate = Ceil(root.get(), key);
    if (!candidate || candidate->key < key) {
        throw std::runtime_error("Invalid key");
    }
    return candidate->key;
}

// Returns the maximum key in the multiset.
// Throws an exception if the multiset is empty.
template <typename K>
const K &Multiset<K>::Max() {
    if (Empty()) {
        throw std::runtime_error("Empty multiset");
    }
    Node *maxNode = FindMax(root.get());
    return maxNode->key;
}

// Returns the minimum key in the multiset.
// Throws an exception if the multiset is empty.
template <typename K>
const K &Multiset<K>::Min() {
    if (Empty()) {
        throw std::runtime_error("Empty multiset");
    }
    Node *minNode = FindMin(root.get());
    return minNode->key;
}

#endif // MULTISET_H_
