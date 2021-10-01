#include "block.h"
#include "record.h"
#include <vector>

using namespace std;


void Block::addRecord(Record record){
    records.push_back(record);
}

void Block::DeleteRecord(float key) {
    if (records.size() == 0) {
        std::cerr << "[Block::DeleteRecord] error, no record avaliable" << std::endl;
        return;
    }
    std::vector<int> deletePos;
    for (int i = records.size()-1; i >= 0; i--) {
        if (records[i].numVotes == key) {
            records.erase(records.begin() + i);
        }
    }
} 

std::vector<Record> Block::getRecord(float key) {
    std::vector<Record> recordFound;
    for (Record record : records) {
        if (record.numVotes == key) {
            recordFound.push_back(record);
            // return record;
        }
    }
    // return empty record, but should not happen
    return recordFound;
}

int Block::getNumRecords(){
    return records.size();
}

int Block::getBlockSize(){
    int blockSize = 0;
    for (int i=0; i < getNumRecords(); i++){
        blockSize += records[i].getRecordSize();
    }
    return blockSize;
}

void Block::toString() {
    for (Record r : records) {
        std::cout << r.tconst << " ,";
    }
    std::cout << std::endl;
}







