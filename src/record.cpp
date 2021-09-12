#include "record.h"
#include <iostream>
using namespace std;

void Record::setRecord(string tconst_, float averageRating_, int numVotes_){
    tconst = tconst_;
    averageRating = averageRating_;
    numVotes = numVotes_;
}

int Record::getRecordSize(){
    return sizeof(tconst) + sizeof(averageRating) + sizeof(numVotes);
}

