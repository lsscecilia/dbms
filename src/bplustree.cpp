#include "bplustree.h"

// movement of ptr value
// last ptr value to link to the next node
// duplicated value..? should not happen right

// void BPlusTree::InsertNode(std::uint32_t key, std::shared_ptr<Record> record)
void BPlusTree::InsertNode(std::uint32_t key, std::shared_ptr<void> blockPtr, std::uint16_t offset) {
    Pointer newPointer(blockPtr, offset);
    if (root == nullptr) {
        // create new node
        root = std::make_shared<Node>(true, size); 
        root->keys.push_back(key);
        root->ptrs.push_back(newPointer);
    } else {
        std::shared_ptr<Node> traverseNode = root;
        std::shared_ptr<Node> parentNode;
        while (!traverseNode->isLeaf) {
            // traverse until it is leaf
            parentNode = traverseNode;
            
            for (int i = 0; i < traverseNode->keys.size(); i++) {
                if (key < traverseNode->keys[i]) {
                    traverseNode = std::static_pointer_cast<Node>(traverseNode->ptrs[i].ptr);
                    break;
                }

                if (i == traverseNode->keys.size()-1) {
                    traverseNode = std::static_pointer_cast<Node>(traverseNode->ptrs[i+1].ptr);
                }
            }
        }

        // get leaf node
        if (traverseNode->keys.size() < size) {
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
                // insert key in vector
                std::vector<std::uint32_t>::iterator keyInsertItr = traverseNode->keys.begin() + insertPos;
                traverseNode->keys.insert(keyInsertItr, key);
                
                // insert ptr in vector
                std::vector<Pointer>::iterator ptrInsertItr = traverseNode->ptrs.begin() + insertPos;
                traverseNode->ptrs.insert(ptrInsertItr, newPointer);
            }

        } else {
            // split
            std::shared_ptr<Node> newLeaf = std::make_shared<Node>(true, size);
            std::vector<std::uint32_t> tempKeys(traverseNode->keys);
            std::vector<Pointer> tempPtrs(traverseNode->ptrs);

            // to optimise, can use binary scan instead of linear scan
            int insertPos = -1;
            for (int i = 0; tempKeys.size(); i++) {
                if (key < tempKeys[i]) {
                    insertPos = i;
                    break;
                }
                if (i == tempKeys.size()-1) {
                    // insert at the end of the vector
                    tempKeys.push_back(key);
                    // insert ptr at the second last pos of the vector
                    tempPtrs.insert(tempPtrs.begin() + tempPtrs.size()-1, newPointer);
                }
            }

            if (insertPos != -1) {
                // insert at key in vector
                std::vector<std::uint32_t>::iterator insertItr = tempKeys.begin() + insertPos;
                tempKeys.insert(insertItr, key);
                
                // insert ptr in vector
                std::vector<Pointer>::iterator ptrInsertItr = tempPtrs.begin() + insertPos;
                tempPtrs.insert(ptrInsertItr, newPointer);
            }

            // split vector into 2 for 2 nodes

            // qn, is the splitting correct...?
            std::size_t splitSize = (tempKeys.size() + 1)/2;
            std::vector<std::uint32_t> firstHalfKey(tempKeys.cbegin(), tempKeys.begin() + splitSize);
            std::vector<std::uint32_t> secondHalfKey(tempKeys.cbegin() + splitSize, tempKeys.cend());

            traverseNode->keys = firstHalfKey;
            newLeaf->keys = secondHalfKey;

            std::vector<Pointer>::const_iterator splitIter(tempPtrs.cbegin());
            std::advance(splitIter, splitSize);

            std::vector<Pointer> firstHalfPtr(tempPtrs.cbegin(), splitIter);
            std::vector<Pointer> secondHalfPtr(splitIter, tempPtrs.cend());

            // link traverseNode to the newleaf
            Pointer nextNode(newLeaf);
            firstHalfPtr.push_back(nextNode);

            traverseNode->ptrs = firstHalfPtr;
            newLeaf->ptrs = secondHalfPtr;
            
            // change the parent of subsequent above level

            Pointer ptrTraverseNode(traverseNode);
            Pointer ptrNewLeaf(newLeaf);

            // if it is the root node
            if (traverseNode == root) {
                std::shared_ptr<Node> newRoot = std::make_shared<Node>(false, size);
                newRoot->keys.push_back(newLeaf->keys[0]);
                newRoot->ptrs.push_back(ptrTraverseNode);
                newRoot->ptrs.push_back(ptrNewLeaf);
                this->root = newRoot;
            } else {
                InsertInternal(newLeaf->keys[0], parentNode, newLeaf);
            }
        }
    }
}

