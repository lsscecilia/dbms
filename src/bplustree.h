#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include <memory>
#include <vector>
#include "block.h"
#include<iostream>

// non leaf node points to Node
// leaf node points to vector with all the blocks
struct Node {
    std::vector<std::shared_ptr<void>> ptrs;
    std::vector<float> keys;
    bool isLeaf;
    std::uint32_t size;
    Node(bool isLeaf, int size) : isLeaf(isLeaf), size(size) {};
};

// N is number of keys in a tree node
class BPlusTree {
    std::shared_ptr<Node> root;
    std::uint32_t size;
    int numNodes;
    
    public:
    // todo: take in linkedlist size
    BPlusTree(std::uint32_t size) : size(size) {
        // isit by default already nullptr
        root = nullptr;
        numNodes = 0;
    };

    ~BPlusTree() = default;

    void InsertNode(float key, std::shared_ptr<Block> blockPtr);
    void InsertInternal(float key, std::shared_ptr<Node> parent, std::shared_ptr<Node> child);
    int DeleteKey(float key);
    int RemoveInternal(float key, std::shared_ptr<Node> traverseNode, std::shared_ptr<Node> childToDelete);
    void Find(float key);
    std::vector<std::pair<float, std::shared_ptr<std::vector<std::shared_ptr<Block>>>>> FindRange(float begin, float end);
    void PrintNode(std::shared_ptr<Node> node);
    void PrintNodeWithoutPtr(std::shared_ptr<Node> node);
    void PrintRecordsInNode(std::shared_ptr<Node> node);
    void PrintRecordInLL(std::shared_ptr<void> ptr, float key);
    std::shared_ptr<Node> GetRoot();
    void SetRoot(std::shared_ptr<Node> newRoot);
    std::shared_ptr<Node> FindParent(std::shared_ptr<Node> root, std::shared_ptr<Node> child);
    int GetNumNodes() {
        return numNodes;
    }

    int GetHeight();

    void PrintStats();
};

#endif