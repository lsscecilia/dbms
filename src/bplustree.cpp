#include <iostream>
#include <queue>

#include "bplustree.h"
// movement of ptr value
// last ptr value to link to the next node
// duplicated value..? should not happen right

// void BPlusTree::InsertNode(std::uint32_t key, std::shared_ptr<Record> record)
void BPlusTree::InsertNode(float key, std::shared_ptr<Block> blockPtr) {
    std::cerr << "[BPlusTree::InsertNode] insert key: " << key << std::endl; 
    if (root == nullptr) {
        // std::cerr << "root is nullptr, insert immediately" << std::endl;
        // create new node
        root = std::make_shared<Node>(true, size); 
        root->keys.push_back(key);
        // std::cout << "ll size: " << llSize << std::endl;
        std::shared_ptr<LinkedList> ll = std::make_shared<LinkedList>(llSize, blockPtr);
        // std::cerr << "ll address: " << ll << " , key: " << key << std::endl;
        root->ptrs.push_back(ll);
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
                    traverseNode = std::static_pointer_cast<Node>(traverseNode->ptrs[i]);
                    break;
                }

                if (i == traverseNode->keys.size()-1) {
                    traverseNode = std::static_pointer_cast<Node>(traverseNode->ptrs[i+1]);
                    break;
                }
            }
        }

        // std::cerr << "traverse node is at leaf node" << std::endl;
        // get leaf node
        bool duplicate = false;
        if (traverseNode->keys.size() > size) {
            std::cerr << "error with program" << std::endl;
            exit(1);
        }

        if (traverseNode->keys.size() < size) {
            std::cerr << "current traverseNode still have empty space" << std::endl;
            // insert into this current node
            
            int insertPos = -1;
            for (int i = 0; i < traverseNode->keys.size(); i++) {
                // check if there is duplicate
                if (key == traverseNode->keys[i]) {
                    duplicate = true;
                    insertPos = i;
                    break;
                } else if (key < traverseNode->keys[i]) {
                    insertPos = i;
                    break;
                }
                if (i == traverseNode->keys.size()-1) {
                    insertPos = traverseNode->keys.size();
                    break;
                }
            }

            // deal with duplicate
            if (duplicate && insertPos != -1) {
                // std::cerr << "duplicate" << std::endl;
                // insert into linkedlist
                std::shared_ptr<LinkedList> ll = std::static_pointer_cast<LinkedList>(traverseNode->ptrs[insertPos]);
                // std::cerr << "insert ll size: " << ll->ptrs.size() << std::endl;
                
                // while (ll->next != nullptr) {
                //     ll = ll->next;
                //     std::cerr << "should nto happen " << ll << std::endl;
                //     std::cerr << ".. " << (ll->next == nullptr) << std::endl;
                // }
                // // bug here, ptr pointer to idk whr
                // std::cerr << "ll: " << ll << std::endl; 

                bool llToInsertFound = false;
                while (!llToInsertFound) {
                    if (ll->haveSpace()) {
                        // std::cerr << "ll still have space" << std::endl;
                        ll->ptrs.push_back(blockPtr);
                        std::cerr << "ll count ref: " << ll.use_count() << std::endl;
                        std::cerr << "block inside: " << ll->ptrs[ll->ptrs.size()-1].use_count() << std::endl;
                        llToInsertFound = true;
                    } else {
                        if (ll->last) {
                            // create new ll
                            std::shared_ptr<LinkedList> newll = std::make_shared<LinkedList>(llSize, blockPtr);
                            ll->next = std::move(newll);
                            std::cerr << "new ll count ref: " << ll->next.use_count() << std::endl;
                            std::cerr << "ll inside: " << ll->ptrs[0].use_count() << std::endl;
                            llToInsertFound = true;
                            ll->last = false;
                        } else {
                            ll = ll->next;
                            // std::cerr << "next: " << ll->next << std::endl;
                        }
                        
                    }
                }
                
            } else if (insertPos != -1) {
                // insert key in vector
                std::vector<float>::iterator keyInsertItr = traverseNode->keys.begin() + insertPos;
                traverseNode->keys.insert(keyInsertItr, key);
                
                // insert ptr in vector
                // std::cerr << "before insert size: " << traverseNode->ptrs.size() << std::endl;

                std::vector<std::shared_ptr<void>>::iterator ptrInsertItr = traverseNode->ptrs.begin() + insertPos;
            
                std::shared_ptr<LinkedList> ll = std::make_shared<LinkedList>(llSize, blockPtr);
                // std::cerr << "ll address: " << ll << " , key: " << key << std::endl;
                traverseNode->ptrs.insert(ptrInsertItr, ll);

                // if (insertPos >= traverseNode->ptrs.size()) {
                //     traverseNode->ptrs.push_back(ll);
                // } else {
                //     std::cerr << "ptr size before: " << traverseNode->ptrs.size() << ", is insert pos: " << insertPos << std::endl;
                //     std::vector<std::shared_ptr<void>> tempPtr(traverseNode->ptrs);
                //     traverseNode->ptrs.clear();
                //     for (int x = 0; x < tempPtr.size(); x++) {
                //         if (x == insertPos) {
                //             std::cerr << "insertpos" << std::endl;
                //             traverseNode->ptrs.push_back(ll);
                //         }
                //         std::shared_ptr<void> temp = std::move(tempPtr[x]);
                //         traverseNode->ptrs.push_back(temp);
                //     }
                //     std::cerr << "ptr size after: " << tempPtr.size() << std::endl;
                //     std::cerr << "ptr size after: " << traverseNode->ptrs.size() << std::endl;
                // }
            }
        } else {
            std::cerr << "current traverseNode does not have empty space" << std::endl;


            // split
            std::shared_ptr<Node> newLeaf = std::make_shared<Node>(true, size);
            std::cerr << "traverse node key size: " << traverseNode->keys.size() << std::endl;
            std::cerr << "traverse node ptr size: " << traverseNode->ptrs.size() << std::endl;
            
            std::vector<float> tempKeys(traverseNode->keys);
            std::vector<std::shared_ptr<void>> tempPtrs(traverseNode->ptrs);

            std::cerr << "copy tempKey size: " << tempKeys.size() << std::endl;

            // to optimise, can use binary scan instead of linear scan

            bool duplicate = false;
            int insertPos = -1;
            for (int i = 0; i < tempKeys.size(); i++) {
                // std::cerr << "stuck in for loop? " << i  << std::endl;
                if (key == tempKeys[i]) {
                    insertPos = i;
                    duplicate = true;
                    break;
                } else if (key < tempKeys[i]) {
                    insertPos = i;
                    break;
                }
                if (i == tempKeys.size()-1) {
                    // change this too

                    // insert at the end of the vector
                    tempKeys.push_back(key);
                    // insert ptr at the second last pos of the vector
                    // Pointer llPointer(blockPtr);
                    

                    std::shared_ptr<LinkedList> ll = std::make_shared<LinkedList>(llSize, blockPtr);
                    std::cerr << "ll address: " << ll << " , key: " << key << std::endl;
                    tempPtrs.insert(tempPtrs.begin() + tempPtrs.size(), std::move(ll));
                    break;
                }
            }
            std::cerr << "insert pos: " << insertPos << std::endl; 


            if (duplicate && insertPos != -1) {
                // insert into linkedlist
                std::shared_ptr<LinkedList> ll = std::static_pointer_cast<LinkedList>(traverseNode->ptrs[insertPos]);
                // std::cerr << "insert ll size: " << ll->ptrs.size() << std::endl;
                bool llToInsertFound = false;
                while (!llToInsertFound) {
                    if (ll->haveSpace()) {
                        // std::cerr << "ll still have space" << std::endl;
                        ll->ptrs.push_back(blockPtr);
                        llToInsertFound = true;
                    } else {
                        if (ll->last) {
                            // create new ll
                            std::shared_ptr<LinkedList> newll = std::make_shared<LinkedList>(llSize, blockPtr);
                            ll->next = std::move(newll);
                            llToInsertFound = true;
                            ll->last = false;
                        } else {
                            ll = ll->next;
                            // std::cerr << "next: " << ll->next << std::endl;
                        }
                        
                    }
                }
                return;
            } else if (insertPos != -1) {
                // insert at key in vector
                std::vector<float>::iterator insertItr = tempKeys.begin() + insertPos;
                tempKeys.insert(insertItr, key);
                
                // insert ptr in vector
                std::vector<std::shared_ptr<void>>::iterator ptrInsertItr = tempPtrs.begin() + insertPos;
    
                std::shared_ptr<LinkedList> ll = std::make_shared<LinkedList>(llSize, blockPtr);
                // std::cerr << "ll address: " << ll << " , key: " << key << std::endl;
                tempPtrs.insert(ptrInsertItr, ll);
            }

            // split vector into 2 for 2 nodes

            std::cerr << "split" << std::endl;

            // qn, is the splitting correct...?
            std::size_t splitSize = (tempKeys.size() + 1)/2;
            std::vector<float> firstHalfKey(tempKeys.cbegin(), tempKeys.cbegin() + splitSize);
            std::vector<float> secondHalfKey(tempKeys.cbegin() + splitSize, tempKeys.cend());

            // std::cerr << "first half key: " << firstHalfKey.size() << std::endl;
            // std::cerr << "second half key: " << secondHalfKey.size() << std::endl;

            traverseNode->keys = firstHalfKey;
            newLeaf->keys = secondHalfKey;

            std::vector<std::shared_ptr<void>>::const_iterator splitIter(tempPtrs.cbegin());
            std::advance(splitIter, splitSize);

            std::vector<std::shared_ptr<void>> firstHalfPtr(tempPtrs.cbegin(), splitIter);
            std::vector<std::shared_ptr<void>> secondHalfPtr(splitIter, tempPtrs.cend());

            // link traverseNode to the newleaf
            std::shared_ptr<void> nextNode(newLeaf);
            firstHalfPtr.push_back(nextNode);

            // std::cerr << "first half ptr: " << firstHalfPtr.size() << std::endl;
            // std::cerr << "second half ptr: " << secondHalfPtr.size() << std::endl;

            traverseNode->ptrs = firstHalfPtr;
            newLeaf->ptrs = secondHalfPtr;
            
            // change the parent of subsequent above level

            std::shared_ptr<void> ptrTraverseNode(traverseNode);
            std::shared_ptr<void> ptrNewLeaf(newLeaf);

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
    // std::cerr << "[BPlusTree::InsertInternal] insert internal, key: " << key << std::endl;
    std::shared_ptr<void> newPointer(child);

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

        // std::cerr << "[BPlusTree::InsertInternal] parent stil have space, insert pos: " << insertPos << std::endl;

        if (insertPos != -1) {
            // insert key in vector
            std::vector<float>::iterator keyInsertItr = parent->keys.begin() + insertPos;
            parent->keys.insert(keyInsertItr, key);
            
            // insert ptr in vector
            std::vector<std::shared_ptr<void>>::iterator ptrInsertItr = parent->ptrs.begin() + insertPos + 1;
            parent->ptrs.insert(ptrInsertItr, newPointer);
        }
    } else {
        // parent does not have space, split the parent node into 2
        // std::cerr << "parent does not have space" << std::endl;

        std::shared_ptr<Node> newInternalNode = std::make_shared<Node>(false, size);

        std::vector<float> tempKeys(parent->keys);
        std::vector<std::shared_ptr<void>> tempPtrs(parent->ptrs);

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
        // std::cerr << "[BPlusTree::InsertInternal] parent does not have space, insert pos: " << insertPos << std::endl;
        if (insertPos != -1) {
            // insert key in vector
            std::vector<float>::iterator keyInsertItr = tempKeys.begin() + insertPos;
            tempKeys.insert(keyInsertItr, key);
            
            // insert ptr in vector
            std::vector<std::shared_ptr<void>>::iterator ptrInsertItr = tempPtrs.begin() + insertPos + 1;
            tempPtrs.insert(ptrInsertItr, newPointer);
        }

        // split temp key & ptr
        std::size_t splitSize = (tempKeys.size() + 1)/2;
        // std::cerr << "split size :" << splitSize  << std::endl;
        std::vector<float> firstHalfKey(tempKeys.cbegin(), tempKeys.cbegin() + splitSize);
        // +1 becuase for the second half dont need the first key
        std::vector<float> secondHalfKey(tempKeys.cbegin() + splitSize + 1, tempKeys.cend());

        // std::cerr << "first half key: " << firstHalfKey.size() << std::endl;
        // std::cerr << "second half key: " << secondHalfKey.size() << std::endl;

        parent->keys = firstHalfKey;
        newInternalNode->keys = secondHalfKey;

        std::vector<std::shared_ptr<void>>::const_iterator splitIter(tempPtrs.cbegin());
        std::advance(splitIter, splitSize);

        std::vector<std::shared_ptr<void>> firstHalfPtr(tempPtrs.cbegin(), splitIter + 1);
        std::vector<std::shared_ptr<void>> secondHalfPtr(splitIter + 1, tempPtrs.cend());

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
        
        std::shared_ptr<void> internalNode1Ptr(parent);
        std::shared_ptr<void> internalNode2Ptr(newInternalNode);
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
    // std::cerr << "[BPlusTree::FindParent] root: " << root << ", child: " << child << std::endl;

    if (root->isLeaf || std::static_pointer_cast<Node>(root->ptrs[0])->isLeaf)
        return nullptr;

    // get the max key?
    for (int i = 0; i < root->ptrs.size(); i++) {
        if (root->ptrs[i] == child) {
            return root;
        } else {
            std::shared_ptr<Node> parent = FindParent(std::static_pointer_cast<Node>(root->ptrs[i]), child);
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
        std::cout << node->ptrs[i] << "|" << node->keys[i]<< "|";
    }
    std::cout << node->ptrs[node->ptrs.size()-1] << "]" << std::endl;
}

// for leaf node
void BPlusTree::PrintRecords(std::shared_ptr<Node> node) {
    for (int i = 0; i < node->ptrs.size(); i++) {
        PrintRecordInLL(node->ptrs[i], node->keys[i]);
    }
}

void BPlusTree::PrintRecordInLL(std::shared_ptr<void> ptr, float key) {
    std::shared_ptr<LinkedList> ll = std::static_pointer_cast<LinkedList>(ptr);
    while (ll != nullptr) {
        for (std::shared_ptr<Block> block : ll->ptrs) {
            // print duplicate keys in same block
            std::vector<Record> records =  block->getRecord(key);
            // std::cerr << "ptr address : " << ptr.ptr << std::endl;
            // std::cerr << "num records: " << records.size() << std::endl;
            for (Record record : records) {
                std::cout << record.toString();
            }
        }
        if (ll->last)
            break;
        ll = ll->next;
    }
    
    // std::shared_ptr<Block> block = std::static_pointer_cast<Block>(ptr.ptr);
}

void BPlusTree::FindRange(float begin, float end) {
    int numIO = 0;
    if (end < begin) {
        std::cerr << "[BPLUSTREE::FindRange] range error" << std::endl;
        return;
    }
    // } else if (begin == end) {
    //     Find(begin);
    //     return;
    // }
    std::cerr << "[BPLUSTREE::FindRange] " << begin << " to " << end << std::endl;
    // check through key node, if begin < key node, go left, else go right to continue to find till the last element
    std::shared_ptr<Node> itr = root;
    bool itrIsLeaf = root->isLeaf;
    while (!itrIsLeaf) {
        for (int i = 0; i < itr->keys.size(); i++) {
            if (begin < itr->keys[i]) {
                itr = std::static_pointer_cast<Node>(itr->ptrs[i]);
                //std::cerr << "left, key: " << itr->keys[i] << std::endl;
                break;
            }
            if (i == itr->keys.size() - 1) {
                itr = std::static_pointer_cast<Node>(itr->ptrs[i+1]);
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
    //std::cerr << "go to leaf" << std::endl;
    // scan through ll of leaf level & display it?
    bool terminate = false;
    std::shared_ptr<void> addressBefore;
    while (!terminate) {
        // search through the keys to find the end point
        for (int i = 0; i < itr->keys.size(); i++) {
            if (itr->keys[i] > end) {
                //std::cerr << itr->keys[i] << ">"  << end << std::endl;
                terminate = true;
                break;
            } else if (itr->keys[i] >= begin) {
                //std::cerr << "+1" << std::endl;
                // print content for key
                // std::cerr << "check offset: " << itr->ptrs[i].offset << std::endl;
                // if (itr->ptrs[i].ptr == addressBefore.ptr)
                //     continue;
                // else
                //     addressBefore = itr->ptrs[i];
                PrintRecordInLL(itr->ptrs[i], itr->keys[i]);
                numIO++;
            }
            if (i == itr->keys.size()-1 && itr->ptrs.size() > itr->keys.size()) {
                //std::cerr << "next" << std::endl;
                itr = std::static_pointer_cast<Node>(itr->ptrs[itr->ptrs.size()-1]);
                break;
            } else if (i == itr->keys.size()-1) {
                terminate = true;
            }
        }
        //std::cerr << "stuck in second for loop" << std::endl;
    }
    std::cerr << "Number of IO: " << numIO << std::endl;
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
                    traverseNode = std::static_pointer_cast<Node>(traverseNode->ptrs[i]);
                    break;
                }
                if (i == traverseNode->keys.size() - 1) {
                    // key is larger than all keys in node
                    // set traverseNode to last pointer
                    traverseNode = std::static_pointer_cast<Node>(traverseNode->ptrs[i+1]);
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
                PrintRecordInLL(traverseNode->ptrs[i], traverseNode->keys[i]);
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

int BPlusTree::DeleteKey(float key) {
    // std::cerr << "[BPlusTree::DeleteKey] Key deleted: " << key << std::endl;

    int numNodesDeleted = 0;

    // empty tree, return
    if (root == nullptr) {
        std::cout << "B+ tree is empty." << std::endl;
        return numNodesDeleted;
    } else {
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
                    traverseNode = std::static_pointer_cast<Node>(traverseNode->ptrs[i]);
                    break;
                }
                if (i == traverseNode->keys.size() - 1) {
                    // key is larger than all keys in node
                    // set traverseNode to last pointer
                    traverseNode = std::static_pointer_cast<Node>(traverseNode->ptrs[i+1]);
                    leftSibling = i;
                    rightSibling = i + 2;
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
        std::shared_ptr<LinkedList> llToDelete = std::static_pointer_cast<LinkedList>(traverseNode->ptrs[deletePos]);
        for (std::shared_ptr<Block> keyBlock : llToDelete->ptrs) {
            keyBlock->DeleteRecord(key);
        }
        std::cerr << "records deleted: " << llToDelete->ptrs.size() << std::endl;
        //std::shared_ptr<Block> keyBlock = std::static_pointer_cast<Block>(traverseNode->ptrs[deletePos].ptr);
        
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

        // replace deleted key with new key
        // check if deleted key exists in internal node, replace if so

        // check if the leaf has at least ⌊(n+1)/2⌋ keys
        if (traverseNode->keys.size() >= (size + 1)/2) {
            // Minimum number of keys in leaf is satisfied
            // TODO: update parent key!!
            return numNodesDeleted;
        }
        // min. number of keys in leaf is not satisfied

        // borrow from left sibling and check if left sibling exists
        if (leftSibling >= 0){
            std::shared_ptr<Node> leftSiblingNode = std::static_pointer_cast<Node>(parentNode->ptrs[leftSibling]);

            // Check if left sibling has extra keys
            if (leftSiblingNode->keys.size() >= ((size + 1)/2) + 1) {
                // insert left sibling's last key as first key of current node
                int key = leftSiblingNode->keys[leftSiblingNode->keys.size() - 1];
                int initLeftSiblingSize = leftSiblingNode->keys.size();
            
                traverseNode->keys.insert(traverseNode->keys.begin(), key);

                // insert left sibling's largest key pointer as first pointer of current node
                traverseNode->ptrs.insert(traverseNode->ptrs.begin(), leftSiblingNode->ptrs[leftSiblingNode->keys.size() - 1]);

                // delete transferred key and pointer in left sibling
                leftSiblingNode->keys.erase(leftSiblingNode->keys.begin() + (initLeftSiblingSize - 1));

                leftSiblingNode->ptrs.erase(leftSiblingNode->ptrs.begin() + (initLeftSiblingSize - 1));

                // update parent's key with first key of current node
                parentNode->keys[leftSibling] = traverseNode->keys[0];
                std::cout << "Borrowed key from left sibling" << std::endl;
                return numNodesDeleted;
            }
        }

        //borrow from right sibling since left sibling is useless and check if right sibling exists
        if (rightSibling <= parentNode->keys.size()) {
            std::shared_ptr<Node> rightSiblingNode = std::static_pointer_cast<Node>(parentNode->ptrs[rightSibling]);

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
            std::shared_ptr<Node> leftSiblingNode = std::static_pointer_cast<Node>(parentNode->ptrs[leftSibling]);

            // merge left sibling with current node
            leftSiblingNode->ptrs.pop_back();
            leftSiblingNode->keys.insert(leftSiblingNode->keys.end(), traverseNode->keys.begin(), traverseNode->keys.end());
            leftSiblingNode->ptrs.insert(leftSiblingNode->ptrs.end(), traverseNode->ptrs.begin(), traverseNode->ptrs.end());

            numNodesDeleted++;
            numNodesDeleted += RemoveInternal(parentNode->keys[leftSibling], parentNode, traverseNode);
            std::cout << "Merged left sibling node" << std::endl;
            return numNodesDeleted;
        }

        else if (rightSibling <= parentNode->keys.size()) {
            // merge with right sibling
            std::shared_ptr<Node> rightSiblingNode = std::static_pointer_cast<Node>(parentNode->ptrs[rightSibling]);

            traverseNode->ptrs.pop_back();
            traverseNode->keys.insert(traverseNode->keys.end(), rightSiblingNode->keys.begin(), rightSiblingNode->keys.end());
            traverseNode->ptrs.insert(traverseNode->ptrs.end(), rightSiblingNode->ptrs.begin(), rightSiblingNode->ptrs.end());

            traverseNode->ptrs.back() = rightSiblingNode->ptrs.back();
            numNodesDeleted++;
            numNodesDeleted += RemoveInternal(parentNode->keys[rightSibling - 1], parentNode, rightSiblingNode);
            std::cout << "Merged right sibling node" << std::endl;
            return numNodesDeleted;
        }
    }
    return numNodesDeleted;
}

// Removes the invalid key = key in traverseNode
// Removes the invalid pointer = deletedChild in traverseNode by removing all pointers to childToDelete
int BPlusTree::RemoveInternal(float key, std::shared_ptr<Node> traverseNode, std::shared_ptr<Node> childToDelete) {
    std::shared_ptr<Node> root = GetRoot();
    int numNodesDeleted = 0;

    // Check if key to be deleted is from root
    if (traverseNode == root) {
        if (traverseNode->keys.size() == 1){
            // only one key exists and points to the child node
            // we set the new root as the merged node
            if (traverseNode->ptrs[1] == childToDelete) {
                // second pointer in root points to the invalid leaf node
                // set new root as the node pointed to by the left pointer (index 0)
                this->root = std::static_pointer_cast<Node>(traverseNode->ptrs[0]);
                std::cout << "Root Node Changed" << std::endl;
                numNodesDeleted++;
                return numNodesDeleted;
            }
            else if (traverseNode->ptrs[0] == childToDelete) {
                // first pointer in root points to the invalid node
                // set new root as the node pointed to by the right pointer
                this->root = std::static_pointer_cast<Node>(traverseNode->ptrs[1]);
                std::cout << "Root Node Changed" << std::endl;
                numNodesDeleted++;
                return numNodesDeleted;
            }
        }
    }

    // Deleting key from parent node

    // find the position of the key to be deleted in traverseNode (parent node of the deleted node)
    int deletePos;
    for (deletePos = 0; deletePos < traverseNode->keys.size(); deletePos++) {
        if (traverseNode->keys[deletePos] == key) {
            break;
        }
    }
    // delete the key at the position found
    traverseNode->keys.erase(traverseNode->keys.begin() + deletePos);

    // find the position of the pointer to be deleted in traverseNode
    for (deletePos = 0; deletePos < traverseNode->ptrs.size(); deletePos++) {
        if (traverseNode->ptrs[deletePos] == childToDelete) {
            break;
        }
    }

    // delete the pointer at the position found
    traverseNode->ptrs.erase(traverseNode->ptrs.begin() + deletePos);

    // check for underflow
    if (traverseNode->keys.size() >= ((size + 1) / 2 - 1)){
        // there is no underflow
        std::cout << key << " deleted from internal node" << std::endl;
        return numNodesDeleted;
    }

    // there is underflow in internal node
    std::cout << "Underflow detected in internal node" << std::endl;

    if (traverseNode == root) {
        // Okay for root node to have underflow
        std::cout << "Internal Node is root node" << std::endl;
        return numNodesDeleted;
    }

    // Need to get parent node to find left and right sibling nodes for underflow operations
    std::shared_ptr<Node> parentNode = FindParent(root, traverseNode);

    int pos, leftSibling, rightSibling;

    // find left and right siblings
    for (pos = 0; pos < parentNode->ptrs.size(); pos++) {
        if (parentNode->ptrs[pos] == traverseNode) {
            leftSibling = pos - 1;
            rightSibling = pos + 1;
            break;
        }
    }

    // check if left sibling exists
    if (leftSibling >= 0) {
        std::shared_ptr<Node> leftSiblingNode = std::static_pointer_cast<Node>(parentNode->ptrs[leftSibling]);

        // check if left sibling has an extra key to adjust with
        if (leftSiblingNode->keys.size() >= ((size + 1) / 2)) {
            // transfer key from parent into current node with underflow
            int leftKeyIndex = leftSiblingNode->keys.size() - 1;
            traverseNode->keys.insert(traverseNode->keys.begin(), parentNode->keys[leftSibling]);

            // update parent's key with left sibling's largest key
            parentNode->keys[leftSibling] = leftSiblingNode->keys[leftKeyIndex];

            // transfer last pointer from left sibling to head of current node
            int leftPtrIndex = leftSiblingNode->ptrs.size() - 1;
            traverseNode->ptrs.insert(traverseNode->ptrs.begin(), leftSiblingNode->ptrs[leftPtrIndex]);

            // delete the last key and pointer of left sibling node
            leftSiblingNode->keys.pop_back();
            leftSiblingNode->ptrs.pop_back();

            return numNodesDeleted;
        }
    }

    //transfer parent's key to current node and replace parent's key with right sibling's first key
    if (rightSibling <= parentNode->keys.size()) {
        std::shared_ptr<Node> rightSiblingNode = std::static_pointer_cast<Node>(parentNode->ptrs[rightSibling]);

        //check if right sibling has an extra key to borrow from
        if (rightSiblingNode->keys.size() >= ((size + 1) / 2)) {
            // insert parent's key into node with underflow
            traverseNode->keys.push_back(parentNode->keys[pos]);

            // replace parent's key with first key of right sibling
            parentNode->keys[pos] = rightSiblingNode->keys[0];

            // delete the first key of right sibling
            rightSiblingNode->keys.erase(rightSiblingNode->keys.begin());

            // transfer pointer from right sibling to current node
            traverseNode->ptrs.push_back(rightSiblingNode->ptrs[0]);

            // delete transferred pointer in right sibling node
            rightSiblingNode->ptrs.erase(rightSiblingNode->ptrs.begin());

            return numNodesDeleted;
        }
    }

    // left and right siblings do not have enough keys, so need to merge
    if (leftSibling >= 0) {
        
        // left sibling exists
        std::shared_ptr<Node> leftSiblingNode = std::static_pointer_cast<Node>(parentNode->ptrs[leftSibling]);

        // transfer key from parent into left sibling
        leftSiblingNode->keys.push_back(parentNode->keys[leftSibling]);

        leftSiblingNode->keys.insert(leftSiblingNode->keys.end(), traverseNode->keys.begin(), traverseNode->keys.end());
        leftSiblingNode->ptrs.insert(leftSiblingNode->ptrs.end(), traverseNode->ptrs.begin(), traverseNode->ptrs.end());

        numNodesDeleted += RemoveInternal(parentNode->keys[leftSibling], parentNode, traverseNode);
        std::cout << "Merged with left sibling in RemoveInternal()" << std::endl;
        numNodesDeleted++;
    } else if (rightSibling <= parentNode->keys.size()) {
        // right sibling exists
        std::shared_ptr<Node> rightSiblingNode = std::static_pointer_cast<Node>(parentNode->ptrs[rightSibling]);

        // transfer key from right sibling to current node
        traverseNode->keys.push_back(parentNode->keys[rightSibling - 1]);

        traverseNode->keys.insert(traverseNode->keys.end(), rightSiblingNode->keys.begin(), rightSiblingNode->keys.end());
        traverseNode->ptrs.insert(traverseNode->ptrs.end(), rightSiblingNode->ptrs.begin(), rightSiblingNode->ptrs.end());
        
        // // transfer all keys from right sibling to current node for merging
        // for (float keyValue: rightSiblingNode->keys) {
        //     traverseNode->keys.push_back(keyValue);
        // }

        // // transfer all pointers from right sibling to current node for merging
        // for (Pointer& ptrValue: rightSiblingNode->ptrs) {
        //     traverseNode->ptrs.push_back(ptrValue);
        // }

        numNodesDeleted += RemoveInternal(parentNode->keys[rightSibling - 1], parentNode, rightSiblingNode);
        std::cout << "Merged with right sibling in RemoveInternal()" << std::endl;
        numNodesDeleted++;
    }

    return numNodesDeleted;
}

