#ifndef BLOCK_H
#define BLOCK_H

#include "record.h"
#include <vector>

//Data structure defining a block holding many records
struct Block
{
    //Records in this block
    std::vector <Record> records;

    //Add a record to this block
    void addRecord(Record record);

    //Get number of records in this block
    int getNumRecords();

    //Get size of this block's content
    int getBlockSize();
    
};

#endif