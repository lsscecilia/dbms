#include <memory>
#include <vector>
#include "block.h"
#include<iostream>

// custom pointer with address offset for easy access of Record in block
struct Pointer {
    std::shared_ptr<void> ptr;
    std::uint16_t offset;
    Pointer(std::shared_ptr<void> ptr, std::uint16_t offset) : ptr(ptr), offset(offset) {};
    Pointer(std::shared_ptr<void> ptr) : ptr(ptr) {
        offset = 0;
    };
};

// non leaf node points to Node
// leaf node points to block
struct Node {
    std::vector<Pointer> ptrs;
    std::vector<float> keys;
    bool isLeaf;
    std::uint32_t size;
    Node(bool isLeaf, int size) : isLeaf(isLeaf), size(size) {};
};

// N is number of keys in a tree node
class BPlusTree {
    std::shared_ptr<Node> root;
    std::uint32_t size;
    
    public:
    BPlusTree(std::uint32_t size) : size(size) {
        // isit by default already nullptr
        root = nullptr;
    };
    void InsertNode(float key, std::shared_ptr<Block>& blockPtr, std::uint16_t offset);
    void InsertInternal(float key, std::shared_ptr<Node> parent, std::shared_ptr<Node> child);
    int DeleteKey(std::uint32_t key);
    int RemoveInternal(float key, std::shared_ptr<Node> parent, std::shared_ptr<Node> child);
    void Find(float key);
    void FindRange(float begin, float end);
    void SplitInternal();
    void SplitLeaf();
    void PrintTree();
    void PrintNode(std::shared_ptr<Node> node);
    void PrintRecords(std::shared_ptr<Node> node);
    void PrintRecord(Pointer& ptr);
    std::shared_ptr<Node> GetRoot();
    std::shared_ptr<Node> FindParent(std::shared_ptr<Node> root, std::shared_ptr<Node> child);
};