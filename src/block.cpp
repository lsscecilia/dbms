#include "block.h"
#include "record.h"
#include <vector>

using namespace std;


void Block::addRecord(Record record){
    records.push_back(record);
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








