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
            // deletePos.push_back(i);
            std::cerr << "to delete: " << records[i].tconst << ", pos: " << i << std::endl;
            records.erase(records.begin() + i);
        }
    }
    std::cerr << "num records to delete from this block: " << deletePos.size() << std::endl;
    // std::cerr << deletePos[0] << std::endl;

    // // delete from behind, so index will not change for front
    // for (int i = deletePos.size()-1; i >= 0; i--) {
    //     std::cerr << "[Block::DeleteRecord] Delete record, id " << records[i].tconst << ", records left in block: " << records.size()-1;
    //     records.erase(records.begin() + i);
        
    // }
    
    // comment out for actual code
    if (records.size() > 0) {
        std::cerr << "(" << records[0].tconst << ")" << std::endl;
    } else {
        std::cerr << std::endl;
    }
} 

std::vector<Record> Block::getRecord(float key) {
    std::vector<Record> recordFound;
    for (Record record : records) {
        // std::cerr << "get record, num votes = " << record.numVotes << std::endl;
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

std::string Block::toString(int offset) {
    return records[offset].toString();
}







