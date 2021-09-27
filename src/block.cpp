#include "block.h"
#include "record.h"
#include <vector>

using namespace std;


void Block::addRecord(Record record){
    records.push_back(record);
}

void Block::DeleteRecord(float key) {
    int deletePos;
    for (deletePos = 0; deletePos < records.size(); deletePos++) {
        if (records[deletePos].averageRating == key) {
            break;
        }
    }

    records.erase(records.begin() + deletePos);
    std::cerr << "[Block::DeleteRecord] Delete record, key - " << key << std::endl;
} 

Record Block::getRecord(int offset) {
    return records[offset];
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







