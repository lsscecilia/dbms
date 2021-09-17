#ifndef BLOCK_H
#define BLOCK_H

#include "record.h"
#include <vector>

//Data structure defining a block holding many records
struct Block
{
    std::uint16_t maxRecord;

    //Records in this block
    std::vector <Record> records;

    //Add a record to this block
    void addRecord(Record record);

    //Get number of records in this block
    int getNumRecords();

    //Get size of this block's content
    int getBlockSize();

    Record getRecord(int offset);

    std::string toString(int offset);
    
    Block(std::uint16_t maxRecord) : maxRecord(maxRecord) {};
};

#endif