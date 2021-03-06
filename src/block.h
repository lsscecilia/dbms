#ifndef BLOCK_H_
#define BLOCK_H_

#include <vector>

#include "record.h"

// Data structure defining a block holding many records
struct Block {
    std::uint16_t maxRecord;

    // Records in this block
    std::vector <Record> records;

    // Add a record to this block
    void addRecord(Record record);

    void DeleteRecord(float key);

    // Get number of records in this block
    int getNumRecords();

    // Get size of this block's content
    int getBlockSize();

    std::vector<Record> getRecord(float key);

    void toString();

    bool haveSpace() {
        if (records.size() < maxRecord)
            return true;
        return false;
    }

    explicit Block(std::uint16_t maxRecord) : maxRecord(maxRecord) {}
    Block() {}
};

#endif    // BLOCK_H_
