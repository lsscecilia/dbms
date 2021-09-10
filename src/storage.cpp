#include "storage.h"
#include <iostream>
#include <vector>

using namespace std;

void Storage::addBlock(Block block){
    blocks.push_back(block);
}

int Storage::getNumBlocks(){
    return blocks.size();
}

int Storage::getNumRecords(){
    int numRecords = 0;

    for (int i=0; i < getNumBlocks(); i++){
        numRecords += blocks[i].getNumRecords();
    }
    return numRecords;
}

int Storage::getStorageSize(){
    int storageSize = 0;

    for (int i =0; i < getNumBlocks(); i++){
        storageSize += blocks[i].getBlockSize();
    }
    return storageSize;
}






