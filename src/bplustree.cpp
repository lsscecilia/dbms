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
}