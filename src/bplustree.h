#include <memory>
#include <vector>

// N is number of keys in a tree node
template <unsigned N>
class BPlusTree {
    template <typename T> using vector = std::vector<T>;
	template <typename T> using shared_ptr = std::shared_ptr<T>;
    struct node {
        std::vector<std::shared_ptr<Node>> ptr(N+1);
        std::vector<std::uint32_t> keys(N);
        bool ifLeaf;
    }

    public:
    void insertNode();
    void deleteNode();
    void find(std::uint32_t key);
    void findRange(std::uint32_t begin, std::uint32_t end);
    void splitInternal();
    void splitLeaf()
}