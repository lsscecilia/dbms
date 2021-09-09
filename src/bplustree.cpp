#include "bplustree.h"

// movement of ptr value
// last ptr value to link to the next node
// duplicated value..? should not happen right
void BPlusTree::InsertNode(std::uint32_t key) {
    if (root == nullptr) {
        // create new node
        this->root = std::shared_ptr<Node>(new Node(true, this->size));
        this->root->keys.push_back(key);
    } else {
        std::shared_ptr<Node> traverseNode = this->root;
        std::shared_ptr<Node> parentNode;
        while (!traverseNode->isLeaf) {
            // traverse until it is leaf
            parentNode = traverseNode;
            
            for (int i = 0; i < traverseNode->keys.size(); i++) {
                if (key < traverseNode->keys[i]) {
                    traverseNode = traverseNode->ptr[i];
                    break;
                }

                if (i == traverseNode->keys.size()-1) {
                    traverseNode = traverseNode->ptr[i+1];
                }
            }
        }

        // get leaf node
        if (traverseNode->keys.size() < this->size) {
            // insert into this current node
            
            int insertPos = -1;
            for (int i = 0; i < traverseNode->keys.size(); i++) {
                if (key < traverseNode->keys[i]) {
                    insertPos = i;
                    break;
                }

                if (i == traverseNode->keys.size()-1) {
                    // insert at the end of the node
                    traverseNode->keys.push_back(key);
                    // todo:: coresponding ptr?
                }
            }
            if (insertPos != -1) {
                // insert at insertPos
                std::vector<std::uint32_t>::iterator insertItr = traverseNode->keys.begin() + insertPos;
                traverseNode->keys.insert(insertItr, key);
                // todo:: coresponding ptr?
            }

        } else {
            // split
            std::shared_ptr<Node> newLeaf = std::shared_ptr<Node>(new Node(true, this->size));
            std::vector<std::uint32_t> tempKeys(traverseNode->keys);

            // to optimise, can use binary scan instead of linear scan
            int insertPos = -1;
            for (int i = 0; tempKeys.size(); i++) {
                if (key < tempKeys[i]) {
                    insertPos = i;
                    break;
                }
                if (i == tempKeys.size()-1) {
                    // insert at the end of the node
                    tempKeys.push_back(key);
                    // todo: coresponding ptr?
                }
            }

            if (insertPos != -1) {
                // insert at insertPos
                std::vector<std::uint32_t>::iterator insertItr = tempKeys.begin() + insertPos;
                tempKeys.insert(insertItr, key);
                // todo: coresponding ptr?
            }

            // split vector into 2 for 2 nodes

            // qn, is the splitting correct...?
            int splitSize = (tempKeys.size() + 1)/2;
            std::vector<std::uint32_t> firstHalf(tempKeys.begin(), tempKeys.begin() + splitSize);
            std::vector<std::uint32_t> secondHalf(tempKeys.begin() + splitSize, tempKeys.end());

            traverseNode->keys = firstHalf;
            newLeaf->keys = secondHalf;
            
            // change the parent of subsequent above level

            // if it is the root node
            if (traverseNode == this->root) {
                std::shared_ptr<Node> newRoot = std::shared_ptr<Node>(new Node(false, this->size));
                newRoot->keys.push_back(newLeaf->keys[0]);
                newRoot->ptr.push_back(traverseNode);
                newRoot->ptr.push_back(newLeaf);
                this->root = newRoot;
            } else {
                InsertInternal(newLeaf->keys[0], parentNode, newLeaf);
            }
        }
    }
}

