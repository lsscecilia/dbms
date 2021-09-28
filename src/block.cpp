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
    int deletePos;
    for (deletePos = 0; deletePos < records.size(); deletePos++) {
        if (records[deletePos].averageRating == key) {
            break;
        }
    }
    records.erase(records.begin() + deletePos);
    std::cerr << "[Block::DeleteRecord] Delete record, key - " << key << ", records left in block: " << records.size();
    if (records.size() > 0) {
        std::cerr << "(" << records[0].averageRating << std::endl;
    } else {
        std::cerr << std::endl;
    }
} 

Record Block::getRecord(float key) {
    for (Record record : records) {
        if (record.averageRating == key) {
            return record;
        }
    }
    // return empty record, but should not happen
    return Record();
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

std::string Block::toString(int offset) {
    return records[offset].toString();
}







