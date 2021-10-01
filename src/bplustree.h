#include <memory>
#include <vector>
#include "block.h"
#include<iostream>

// struct LinkedList {
//     int maxSize;
//     std::vector<std::shared_ptr<Block>> ptrs;
//     std::shared_ptr<LinkedList> next;
//     bool last;

//     LinkedList(int maxSize, std::shared_ptr<Block> ptr) : maxSize(maxSize){
//         ptrs.push_back(ptr);
//         //next = std::shared_ptr<LinkedList>(nullptr);
//         // std::cerr << "init next: " << next << std::endl;
//         last = true;
//     }

//     bool haveSpace() {
//         if (ptrs.size() < maxSize) {
//             return true;
//         }
//         return false;
//     }
// };

// non leaf node points to Node
// leaf node points to block
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
    std::uint32_t llSize;
    int numNodes;
    
    public:
    // todo: take in linkedlist size
    BPlusTree(std::uint32_t size, std::uint32_t llSize) : size(size), llSize(llSize) {
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
    void FindRange(float begin, float end);
    void SplitInternal();
    void SplitLeaf();
    void PrintTree();
    void PrintNode(std::shared_ptr<Node> node);
    void PrintRecords(std::shared_ptr<Node> node);
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