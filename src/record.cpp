#include <sstream>

#include "record.h"

void Record::setRecord(std::string tconst_, float averageRating_, int numVotes_) {
    tconst = tconst_;
    averageRating = averageRating_;
    numVotes = numVotes_;
}

int Record::getRecordSize() {
    return sizeof(tconst) + sizeof(averageRating) + sizeof(numVotes);
}

std::string Record::toString() {
    return tconst + "," + std::to_string(averageRating) + "," + std::to_string(numVotes) + "\n";
}
