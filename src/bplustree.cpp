#include <iostream>

#include "bplustree.h"
// movement of ptr value
// last ptr value to link to the next node
// duplicated value..? should not happen right

// void BPlusTree::InsertNode(std::uint32_t key, std::shared_ptr<Record> record)
void BPlusTree::InsertNode(float key, std::shared_ptr<Block>& blockPtr, std::uint16_t offset) {
    std::cerr << "[BPlusTree::InsertNode] insert key: " << key << ", offset: " << offset << std::endl; 
    Pointer newPointer(blockPtr, offset);
    if (root == nullptr) {
        // std::cerr << "root is nullptr, insert immediately" << std::endl;
        // create new node
        root = std::make_shared<Node>(true, size); 
        root->keys.push_back(key);
        root->ptrs.push_back(newPointer);
    } else {
        // std::cerr << "root is not nullptr" << std::endl;
        std::shared_ptr<Node> traverseNode = root;
        std::shared_ptr<Node> parentNode;
        while (!traverseNode->isLeaf) {
            // std::cerr << "traverse node is not leaf" << std::endl;
            // traverse until it is leaf
            parentNode = traverseNode;
            
            for (int i = 0; i < traverseNode->keys.size(); i++) {
                if (key < traverseNode->keys[i]) {
                    traverseNode = std::static_pointer_cast<Node>(traverseNode->ptrs[i].ptr);
                    break;
                }

                if (i == traverseNode->keys.size()-1) {
                    traverseNode = std::static_pointer_cast<Node>(traverseNode->ptrs[i+1].ptr);
                    break;
                }
            }
        }

        // std::cerr << "traverse node is at leaf node" << std::endl;
        // get leaf node
        if (traverseNode->keys.size() < size) {
            // std::cerr << "current traverseNode still have empty space" << std::endl;
            // insert into this current node
            
            int insertPos = -1;
            for (int i = 0; i < traverseNode->keys.size(); i++) {
                if (key < traverseNode->keys[i]) {
                    insertPos = i;
                    break;
                }

                if (i == traverseNode->keys.size()-1) {
                    insertPos = traverseNode->keys.size();
                }
            }
            // std::cerr << "insert at pos: " << insertPos << std::endl;
            if (insertPos != -1) {
                // insert key in vector
                std::vector<float>::iterator keyInsertItr = traverseNode->keys.begin() + insertPos;
                traverseNode->keys.insert(keyInsertItr, key);
                
                // insert ptr in vector
                std::vector<Pointer>::iterator ptrInsertItr = traverseNode->ptrs.begin() + insertPos;
                traverseNode->ptrs.insert(ptrInsertItr, newPointer);
            }

        } else {
            // std::cerr << "current traverseNode does not have empty space" << std::endl;
            // split
            std::shared_ptr<Node> newLeaf = std::make_shared<Node>(true, size);
            // std::cerr << "traverse node key size: " << traverseNode->keys.size() << std::endl;
            std::vector<float> tempKeys(traverseNode->keys);
            std::vector<Pointer> tempPtrs(traverseNode->ptrs);

            // std::cerr << "copy tempKey size: " << tempKeys.size() << std::endl;

            // to optimise, can use binary scan instead of linear scan
            int insertPos = -1;
            for (int i = 0; i < tempKeys.size(); i++) {
                // std::cerr << "stuck in for loop? " << i  << std::endl;
                if (key < tempKeys[i]) {
                    insertPos = i;
                    break;
                }
                if (i == tempKeys.size()-1) {
                    // insert at the end of the vector
                    tempKeys.push_back(key);
                    // insert ptr at the second last pos of the vector
                    tempPtrs.insert(tempPtrs.begin() + tempPtrs.size(), newPointer);
                    break;
                }
            }
            // std::cerr << "insert pos: " << insertPos << std::endl; 

            if (insertPos != -1) {
                // insert at key in vector
                std::vector<float>::iterator insertItr = tempKeys.begin() + insertPos;
                tempKeys.insert(insertItr, key);
                
                // insert ptr in vector
                std::vector<Pointer>::iterator ptrInsertItr = tempPtrs.begin() + insertPos;
                tempPtrs.insert(ptrInsertItr, newPointer);
            }

            // split vector into 2 for 2 nodes

            // qn, is the splitting correct...?
            std::size_t splitSize = (tempKeys.size() + 1)/2;
            std::vector<float> firstHalfKey(tempKeys.cbegin(), tempKeys.begin() + splitSize);
            std::vector<float> secondHalfKey(tempKeys.cbegin() + splitSize, tempKeys.cend());

            // std::cerr << "first half key: " << firstHalfKey.size() << std::endl;
            // std::cerr << "second half key: " << secondHalfKey.size() << std::endl;

            traverseNode->keys = firstHalfKey;
            newLeaf->keys = secondHalfKey;

            std::vector<Pointer>::const_iterator splitIter(tempPtrs.cbegin());
            std::advance(splitIter, splitSize);

            std::vector<Pointer> firstHalfPtr(tempPtrs.cbegin(), splitIter);
            std::vector<Pointer> secondHalfPtr(splitIter, tempPtrs.cend());

            // link traverseNode to the newleaf
            Pointer nextNode(newLeaf);
            firstHalfPtr.push_back(nextNode);

            // std::cerr << "first half ptr: " << firstHalfPtr.size() << std::endl;
            // std::cerr << "second half ptr: " << secondHalfPtr.size() << std::endl;

            traverseNode->ptrs = firstHalfPtr;
            newLeaf->ptrs = secondHalfPtr;
            
            // change the parent of subsequent above level

            Pointer ptrTraverseNode(traverseNode);
            Pointer ptrNewLeaf(newLeaf);

            // if it is the root node
            if (traverseNode == root) {
                std::shared_ptr<Node> newRoot = std::make_shared<Node>(false, size);
                // std::cerr << "key for new root: " << newLeaf->keys[0] << std::endl;
                newRoot->keys.push_back(newLeaf->keys[0]);
                newRoot->ptrs.push_back(ptrTraverseNode);
                newRoot->ptrs.push_back(ptrNewLeaf);
                this->root = newRoot;
            } else {
                InsertInternal(newLeaf->keys[0], parentNode, newLeaf);
            }
        }
    }
    // std::cerr << "end insert function" << std::endl;
}

void BPlusTree::InsertInternal(float key, std::shared_ptr<Node> parent, std::shared_ptr<Node> child) {
    std::cerr << "[BPlusTree::InsertInternal] insert internal, key: " << key << std::endl;
    Pointer newPointer(child);

    // parent still have space
    if (parent->keys.size() < size) {
        int insertPos = -1;
        for (int i = 0; i < parent->keys.size(); i++) {
            if (key < parent->keys[i]) {
                insertPos = i;
                break;
            }

            if (i == parent->keys.size()-1) {
                insertPos = parent->keys.size();
            }
        }

        std::cerr << "[BPlusTree::InsertInternal] parent stil have space, insert pos: " << insertPos << std::endl;

        if (insertPos != -1) {
            // insert key in vector
            std::vector<float>::iterator keyInsertItr = parent->keys.begin() + insertPos;
            parent->keys.insert(keyInsertItr, key);
            
            // insert ptr in vector
            std::vector<Pointer>::iterator ptrInsertItr = parent->ptrs.begin() + insertPos + 1;
            parent->ptrs.insert(ptrInsertItr, newPointer);
        }
    } else {
        // parent does not have space, split the parent node into 2
        // std::cerr << "parent does not have space" << std::endl;

        std::shared_ptr<Node> newInternalNode = std::make_shared<Node>(false, size);

        std::vector<float> tempKeys(parent->keys);
        std::vector<Pointer> tempPtrs(parent->ptrs);

        // insert into temp key & ptr
        int insertPos = -1;
        for (int i = 0; i < tempKeys.size(); i++) {
            if (key < tempKeys[i]) {
                insertPos = i;
                break;
            }

            if (i == tempKeys.size()-1) {
                insertPos = tempKeys.size();
                break;
            }
        }
        std::cerr << "[BPlusTree::InsertInternal] parent does not have space, insert pos: " << insertPos << std::endl;
        if (insertPos != -1) {
            // insert key in vector
            std::vector<float>::iterator keyInsertItr = tempKeys.begin() + insertPos;
            tempKeys.insert(keyInsertItr, key);
            
            // insert ptr in vector
            std::vector<Pointer>::iterator ptrInsertItr = tempPtrs.begin() + insertPos + 1;
            tempPtrs.insert(ptrInsertItr, newPointer);
        }

        // split temp key & ptr
        std::size_t splitSize = (tempKeys.size() + 1)/2;
        // std::cerr << "split size :" << splitSize  << std::endl;
        std::vector<float> firstHalfKey(tempKeys.cbegin(), tempKeys.begin() + splitSize);
        // +1 becuase for the second half dont need the first key
        std::vector<float> secondHalfKey(tempKeys.cbegin() + splitSize + 1, tempKeys.cend());

        // std::cerr << "first half key: " << firstHalfKey.size() << std::endl;
        // std::cerr << "second half key: " << secondHalfKey.size() << std::endl;

        parent->keys = firstHalfKey;
        newInternalNode->keys = secondHalfKey;

        std::vector<Pointer>::const_iterator splitIter(tempPtrs.cbegin());
        std::advance(splitIter, splitSize);

        std::vector<Pointer> firstHalfPtr(tempPtrs.cbegin(), splitIter + 1);
        std::vector<Pointer> secondHalfPtr(splitIter + 1, tempPtrs.cend());

        parent->ptrs = firstHalfPtr;
        newInternalNode->ptrs = secondHalfPtr;

        // std::cerr << "split first half, key size: " << firstHalfKey.size() << " , ptr size: " << firstHalfPtr.size() << std::endl;
        // std::cerr << "keys: ";
        // for (float k : firstHalfKey) {
        //     std::cerr << k << " ";
        // }
        // std::cerr << std::endl;


        // std::cerr << "second half, key size: " << secondHalfKey.size() << " , ptr size: " << secondHalfPtr.size() << std::endl;
        // std::cerr << "keys: ";
        // for (float k : secondHalfKey) {
        //     std::cerr << k << " ";
        // }
        // std::cerr << std::endl;
        
        Pointer internalNode1Ptr(parent);
        Pointer internalNode2Ptr(newInternalNode);
        float newParentKey = tempKeys[splitSize];
        // std::cerr << "new parent key: " << newParentKey << std::endl;

        if (parent == root) {
            std::shared_ptr<Node> newRoot = std::make_shared<Node>(false, size);
            newRoot->keys.push_back(newParentKey);
            newRoot->ptrs.push_back(internalNode1Ptr);
            newRoot->ptrs.push_back(internalNode2Ptr);
            root = newRoot;
        } else {
            InsertInternal(newParentKey, FindParent(root, parent), newInternalNode);
        }
    }
}

// can optimise by using key to search instead
std::shared_ptr<Node> BPlusTree::FindParent(std::shared_ptr<Node> root, std::shared_ptr<Node> child) {
    std::cerr << "[BPlusTree::FindParent] root: " << root << ", child: " << child << std::endl;

    if (root->isLeaf || std::static_pointer_cast<Node>(root->ptrs[0].ptr)->isLeaf)
        return nullptr;

    // get the max key?
    for (int i = 0; i < root->ptrs.size(); i++) {
        if (root->ptrs[i].ptr == child) {
            return root;
        } else {
            std::shared_ptr<Node> parent = FindParent(std::static_pointer_cast<Node>(root->ptrs[i].ptr), child);
            if (parent != nullptr)
                return parent;
        }
    }
    return nullptr;
}

std::shared_ptr<Node> BPlusTree::GetRoot() {
    return root;
}

void BPlusTree::SetRoot(std::shared_ptr<Node> newRoot){
    this->root = newRoot;
    return;
}

void BPlusTree::PrintNode(std::shared_ptr<Node> node) {
    std::cout << "[";
    for (int i = 0; i < node->keys.size(); i++) {
        std::cout << node->ptrs[i].ptr << "|" << node->keys[i]<< "|";
    }
    std::cout << node->ptrs[node->ptrs.size()-1].ptr << "]" << std::endl;
}

void BPlusTree::PrintRecords(std::shared_ptr<Node> node) {
    for (int i = 0; i < node->ptrs.size(); i++) {
        PrintRecord(node->ptrs[i]);
    }
}

void BPlusTree::PrintRecord(Pointer& ptr) {
    std::shared_ptr<Block> block = std::static_pointer_cast<Block>(ptr.ptr);
    std::cout << block->toString(ptr.offset);
}

void BPlusTree::FindRange(float begin, float end) {
    if (end < begin) {
        std::cerr << "[BPLUSTREE::FindRange] range error" << std::endl;
        return;
    } else if (begin == end) {
        Find(begin);
        return;
    }
    std::cerr << "[BPLUSTREE::FindRange] " << begin << " to " << end << std::endl;
    // check through key node, if begin < key node, go left, else go right to continue to find till the last element
    std::shared_ptr<Node> itr = root;
    bool itrIsLeaf = false;
    while (!itrIsLeaf) {
        for (int i = 0; i < itr->keys.size(); i++) {
            if (begin < itr->keys[i]) {
                itr = std::static_pointer_cast<Node>(itr->ptrs[i].ptr);
                //std::cerr << "left, key: " << itr->keys[i] << std::endl;
                break;
            }
            if (i == itr->keys.size() - 1) {
                itr = std::static_pointer_cast<Node>(itr->ptrs[i+1].ptr);
                //std::cerr << "right" << std::endl;
                break;
            }
        }
        if (itr->isLeaf) {
            itrIsLeaf = true;
            // std::cerr << "is leaf";
        }
        // std::cerr << "stuck in first for loop" << std::endl;
    }

    // scan through ll of leaf level & display it?
    bool terminate = false;
    while (!terminate) {
        // search through the keys to find the end point
        for (int i = 0; i < itr->keys.size(); i++) {
            if (itr->keys[i] > end) {
                // std::cerr << itr->keys[i] << ">"  << end << std::endl;
                terminate = true;
                break;
            } else if (itr->keys[i] >= begin) {
                // print content for key
                // std::cerr << "check offset: " << itr->ptrs[i].offset << std::endl;
                PrintRecord(itr->ptrs[i]);
            }
            if (i == itr->keys.size()-1 && itr->ptrs.size() > itr->keys.size()) {
                //std::cerr << "next" << std::endl;
                itr = std::static_pointer_cast<Node>(itr->ptrs[itr->ptrs.size()-1].ptr);
                break;
            } else if (i == itr->keys.size()-1) {
                terminate = true;
            }
        }
        
        //std::cerr << "stuck in second for loop" << std::endl;
    }
}

void BPlusTree::Find(float key) {
    std::cerr << "[BPlusTree::Find] find key: " << key << std::endl;
        // empty tree, return
    if (root == nullptr) {
        std::cout << "B+ tree is empty." << std::endl;
        return;
    }
    else {
        std::shared_ptr<Node> traverseNode = root;
        std::shared_ptr<Node> parentNode;
        while (!traverseNode->isLeaf) {
            // traverse tree to reach leaf node
            parentNode = traverseNode;

            for (int i = 0; i <traverseNode->keys.size(); i++) {
                if (key < traverseNode->keys[i]) {
                    // set traverseNode to child node if key to delete is less than traverse key
                    // child node is left pointer of key
                    traverseNode = std::static_pointer_cast<Node>(traverseNode->ptrs[i].ptr);
                    break;
                }
                if (key == traverseNode->keys.size() - 1) {
                    // key is larger than all keys in node
                    // set traverseNode to last pointer
                    traverseNode = std::static_pointer_cast<Node>(traverseNode->ptrs[i+1].ptr);
                    break;
                }
            }
        }

        //leaf node for key is found
        bool found = false;

        for (int i = 0; i < traverseNode->keys.size(); i++) {
            if (key == traverseNode->keys[i]) {
                // key is found
                found = true;
                PrintRecord(traverseNode->ptrs[i]);
                break;
            }
        }

        if (!found){
            // key does not exist in tree
            std::cout << "Specified key: " << key << " to be deleted does not exist" << std::endl;
            return;
        }
    }
}

int BPlusTree::DeleteKey(std::uint32_t key) {

    int numNodesDeleted = 0;

    // empty tree, return
    if (root == nullptr) {
        std::cout << "B+ tree is empty." << std::endl;
        return numNodesDeleted;
    }
    else {
        std::shared_ptr<Node> traverseNode = root; // Cursor node to keep track of current node
        std::shared_ptr<Node> parentNode;
        int leftSibling, rightSibling;
        while (!traverseNode->isLeaf) {
            // traverse tree to reach leaf node
            parentNode = traverseNode;

            for (int i = 0; i <traverseNode->keys.size(); i++) {

                leftSibling = i - 1;
                rightSibling = i + 1;

                if (key < traverseNode->keys[i]) {
                    // set traverseNode to child node if key to delete is less than traverse key
                    // child node is left pointer of key
                    traverseNode = std::static_pointer_cast<Node>(traverseNode->ptrs[i].ptr);
                    break;
                }
                if (key == traverseNode->keys.size() - 1) {
                    // key is larger than all keys in node
                    // set traverseNode to last pointer
                    traverseNode = std::static_pointer_cast<Node>(traverseNode->ptrs[i+1].ptr);
                    break;
                }
            }
        }

        // leaf node is found, now search if key exists

        int deletePos = -1; // Position of key and pointer in leaf node

        for (int i = 0; i < traverseNode->keys.size(); i++) {
            if (key == traverseNode->keys[i]) {
                // key is found
                deletePos = i;
                break;
            }
        }

        if (deletePos == -1){
            // key does not exist in tree
            std::cout << "Specified key: " << key << " to be deleted does not exist" << std::endl;
            return numNodesDeleted;
        }

        // delete the specified record
        // TODO: function to delete record given the pointer??

        // // shift keys and pointers forward in the vector
        // for (int i = deletePos; i < traverseNode->keys.size() - 1; i++) {
        //     traverseNode->keys[i] = traverseNode->keys[i + 1];
        //     traverseNode->ptrs[i] = traverseNode->ptrs[i + 1];
        // }

        // // shift last pointer forward
        // traverseNode->ptrs[traverseNode->ptrs.size() - 2] = traverseNode->ptrs[traverseNode->ptrs.size() - 1];

        // // delete last key and pointer in leaf node
        // traverseNode->keys.pop_back();
        // traverseNode->ptrs.pop_back();

        //delete key and pointer at i-th position in leaf node
        traverseNode->keys.erase(traverseNode->keys.begin() + deletePos);
        traverseNode->ptrs.erase(traverseNode->ptrs.begin() + deletePos);

        // check if traverseNode is root node
        if (traverseNode == root) {
            // check if root node is empty
            if (traverseNode->keys.size() == 0) {
                // set root as null pointer
                root = nullptr;
                numNodesDeleted++;
                std::cout << "B+ tree only has root node with 1 key" << std::endl;
                std::cout << "Root node deleted" << std::endl;
            }
            std::cout << "Successfully deleted" << key << "!" << std::endl;
            return numNodesDeleted;
        }

        // replace deleted key with check if deleted key exists in internal node, replace if so

        // check if the leaf has at least ⌊(n+1)/2⌋ keys
        if (traverseNode->keys.size() >= (size + 1)/2) {

            // Minimum number of keys in leaf is satisfied
            // TODO: update parent key!!
            return numNodesDeleted;
        }

        // min. number of keys in leaf is not satisfied

        // borrow from left sibling and check if left sibling exists
        if (leftSibling >= 0){
            std::shared_ptr<Node> leftSiblingNode = std::static_pointer_cast<Node>(parentNode->ptrs[leftSibling].ptr);

            // Check if left sibling has extra keys
            if (leftSiblingNode->keys.size() >= ((size + 1)/2) + 1) {
                // insert left sibling's last key as first key of current node
                int key = leftSiblingNode->keys[leftSiblingNode->keys.size() - 1];
                traverseNode->keys.insert(traverseNode->keys.begin(), key);

                // insert left sibling's largest key pointer as first pointer of current node
                traverseNode->ptrs.insert(traverseNode->ptrs.begin(), leftSiblingNode->ptrs[leftSiblingNode->keys.size() - 1]);

                // delete transferred key and pointer in left sibling
                leftSiblingNode->keys.erase(leftSiblingNode->keys.begin() + (leftSiblingNode->keys.size() - 1));
                leftSiblingNode->ptrs.erase(leftSiblingNode->ptrs.begin() + (leftSiblingNode->keys.size() - 1));

                // update parent's key with first key of current node
                parentNode->keys[leftSibling] = traverseNode->keys[0];
                std::cout << "Borrowed key from left sibling" << std::endl;
                return numNodesDeleted;
            }
        }

        //borrow from right sibling since left sibling is useless and check if right sibling exists
        if (rightSibling <= parentNode->keys.size()) {
            std::shared_ptr<Node> rightSiblingNode = std::static_pointer_cast<Node>(parentNode->ptrs[rightSibling].ptr);

            //check if right sibling has extra keys and
            if (rightSiblingNode->keys.size() >= ((size + 1)/2) + 1) {
                // insert right sibling's first key as last key of current node
                traverseNode->keys.push_back(rightSiblingNode->keys[0]);

                // insert right sibling's first pointer as second last pointer of current node
                traverseNode->ptrs.insert(traverseNode->ptrs.begin() + (traverseNode->ptrs.size() - 1), rightSiblingNode->ptrs[0]);

                // delete transferred key and pointer in the right sibling
                rightSiblingNode->keys.erase(rightSiblingNode->keys.begin());
                rightSiblingNode->ptrs.erase(rightSiblingNode->ptrs.begin());

                // update parent's keys with first key of sibling node
                parentNode->keys[rightSibling - 1] = rightSiblingNode->keys[0];
                std::cout << "Borrowed key from right sibling" << std::endl;
                return numNodesDeleted;
            }
        }

        // Left and Right sibling nodes do not have enough nodes to borrow from
        // Need to merge
        if (leftSibling >= 0) {
            // Left Sibling exists, merge with left sibling
            std::shared_ptr<Node> leftSiblingNode = std::static_pointer_cast<Node>(parentNode->ptrs[leftSibling].ptr);

            //merge left sibling with current node
            for (int i = 0; i < traverseNode->keys.size(); i++) {
                leftSiblingNode->keys.push_back(traverseNode->keys[i]);
                leftSiblingNode->ptrs.push_back(traverseNode->ptrs[i]);
            }

            leftSiblingNode->ptrs.back() = traverseNode->ptrs.back();
            numNodesDeleted++;
            numNodesDeleted += RemoveInternal(parentNode->keys[leftSibling], parentNode, traverseNode);
            std::cout << "Merged left sibling node" << std::endl;
            return numNodesDeleted;
        }

        else {
            // merge with right sibling
            std::shared_ptr<Node> rightSiblingNode = std::static_pointer_cast<Node>(parentNode->ptrs[rightSibling].ptr);

            //merge right sibling with current node
            for (int i = 0; i < traverseNode->keys.size(); i++) {
                traverseNode->keys.push_back(rightSiblingNode->keys[i]);
                traverseNode->ptrs.push_back(rightSiblingNode->ptrs[i]);
            }

            traverseNode->ptrs.back() = rightSiblingNode->ptrs.back();
            numNodesDeleted++;
            numNodesDeleted += RemoveInternal(parentNode->keys[rightSibling - 1], parentNode, traverseNode);
            std::cout << "Merged right sibling node" << std::endl;
            return numNodesDeleted;
        }
    }
}

// Removes the invalid key = key in the node pointed to by pointer = traverseNode
int BPlusTree::RemoveInternal(float key, std::shared_ptr<Node> traverseNode, std::shared_ptr<Node> child) {
    std::shared_ptr<Node> root = GetRoot();
    int numNodesDeleted = 0;

    // Check if key to be deleted is from root
    if (traverseNode == root) {
        // TODO: Check if this part makes sense, as senior code sets new root as ptrs[0] or ptrs[1]
        // TODO: when it should be ptrs[0], since we always merge with left regardless and right node is redundant
        // TODO: if so, remove condition check
        if (traverseNode->keys.size() == 1){
            // only one key exists and points to the child node
            // we set the new root as the merged node
            // since we merge all nodes to the left node (regardless if we merged with left sibling or with right sibling)
            if (traverseNode->ptrs[1].ptr == child) {
                // second pointer in root points to the child node
                // means that a leaf node merged with its left sibling
                this->root = traverseNode->ptrs[0].ptr;
                std::cout << "Root Node Changed" << std::endl;
                numNodesDeleted++;
                return numNodesDeleted;
            }
            else if (traverseNode->ptrs[0].ptr == child) {
                // first pointer in root points to the child node
                // means that a leaf node was merged with its right sibling
                this->root = traverseNode->ptrs[0].ptr;
                std::cout << "Root Node Changed" << std::endl;
                numNodesDeleted++;
                return numNodesDeleted;
            }
        }
    }
}

