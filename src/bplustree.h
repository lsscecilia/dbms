#include <memory>
#include <vector>

// N is number of keys in a tree node
class BPlusTree {
    // custom pointer with address offset for easy access of Record in block
    struct Pointer {
        std::shared_ptr<void> ptr;
        std::uint16_t offset;
        Pointer(std::shared_ptr<void> ptr, std::uint16_t offset) : ptr(ptr), offset(offset) {};
        Pointer(std::shared_ptr<void> ptr) : ptr(ptr) {};
    };

    // non leaf node points to Node
    // leaf node points to block
    struct Node {
        std::vector<Pointer> ptrs;
        std::vector<std::uint32_t> keys;
        bool isLeaf;
        std::uint32_t size;
        Node(bool isLeaf, int size) : isLeaf(isLeaf), size(size) {};
    };

    std::shared_ptr<Node> root;
    std::uint32_t size;
    
    public:
    BPlusTree(std::uint32_t size) : size(size) {};
    void InsertNode(std::uint32_t key, std::shared_ptr<void> blockPtr, std::uint16_t offset);
    void InsertInternal(std::uint32_t key, std::shared_ptr<Node> parent, std::shared_ptr<Node> child);
    void DeleteNode();
    void Find(std::uint32_t key);
    void FindRange(std::uint32_t begin, std::uint32_t end);
    void SplitInternal();
    void SplitLeaf();
    void PrintTree();
};