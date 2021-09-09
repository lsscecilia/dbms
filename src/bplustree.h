#include <memory>
#include <vector>

// N is number of keys in a tree node
class BPlusTree {
    struct Node {
        std::vector<std::shared_ptr<Node>> ptr;
        std::vector<std::uint32_t> keys;
        bool isLeaf;
        std::uint32_t size;
        Node(bool isLeaf, int size) : isLeaf(isLeaf), size(size) {};
    };

    std::shared_ptr<Node> root;
    std::uint32_t size;

    public:
    void InsertNode(std::uint32_t key);
    void InsertInternal(std::uint32_t key, std::shared_ptr<Node> parent, std::shared_ptr<Node> child);
    void DeleteNode();
    void Find(std::uint32_t key);
    void FindRange(std::uint32_t begin, std::uint32_t end);
    void SplitInternal();
    void SplitLeaf();
};