#ifndef BPLUSTREE_H_
#define BPLUSTREE_H_

#include <memory>
#include <vector>
#include <utility>

#include "block.h"

struct Node {
    std::vector<std::shared_ptr<void>> ptrs;     // non-leaf node points to node, leaf node points to vector with shared_ptr of the blocks
    std::vector<float> keys;
    bool isLeaf;
    std::uint32_t size;    // max number of keys in the Node
    Node(bool isLeaf, int size) : isLeaf(isLeaf), size(size) {}
};


class BPlusTree {
    std::shared_ptr<Node> root;
    std::uint32_t size;    // max number of keys in a tree node
    int numNodes;    // number of nodes in this tree

 public:
    explicit BPlusTree(std::uint32_t size) : size(size) {
        root = nullptr;
        numNodes = 0;
    }

    ~BPlusTree() = default;

    void InsertKey(float key, std::shared_ptr<Block> blockPtr);
    void InsertInternal(float key, std::shared_ptr<Node> parent, std::shared_ptr<Node> child);
    int DeleteKey(float key);
    int RemoveInternal(float key, std::shared_ptr<Node> traverseNode, std::shared_ptr<Node> childToDelete);
    void Find(float key);
    std::vector<std::pair<float, std::shared_ptr<std::vector<std::shared_ptr<Block>>>>> FindRange(float begin, float end);
    void PrintNode(std::shared_ptr<Node> node);
    void PrintNodeWithoutPtr(std::shared_ptr<Node> node);
    void PrintRecordsInNode(std::shared_ptr<Node> node);
    void PrintRecordOfKey(std::shared_ptr<void> ptr, float key);
    std::shared_ptr<Node> GetRoot();
    void SetRoot(std::shared_ptr<Node> newRoot);
    std::shared_ptr<Node> FindParent(std::shared_ptr<Node> root, std::shared_ptr<Node> child);
    int GetNumNodes() {
        return numNodes;
    }

    int GetHeight();

    void PrintStats();
};

#endif    // BPLUSTREE_H_
