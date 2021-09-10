#include "record.h"
#include <iostream>
using namespace std;

Record::Record(string tconst, float averageRating, int numVotes){
    setRecord(tconst, averageRating, numVotes);
}

void Record::setRecord(string tconst_, float averageRating_, int numVotes_){
    tconst = tconst_;
    averageRating = averageRating_;
    numVotes = numVotes_;
}

int Record::getRecordSize(Record record){
    return sizeof(record.tconst) + sizeof(record.averageRating) + sizeof(record.numVotes);
}

