#ifndef STORAGE_H
#define STORAGE_H

#include "block.h"
#include <vector>

/*
Singleton structure representing the disk storage of a database system.
holds multiple blocks in memory, where each blocks holds multiple records.
*/

struct Storage
{
    //Blocks in this record
    std::vector <Block> blocks;

    //Add a block
    void addBlock(Block block);

    //Get number of blocks
    int getNumBlocks();

    //get number of records
    int getNumRecords();

    //Get storage size
    int getStorageSize();
    
};



#endif