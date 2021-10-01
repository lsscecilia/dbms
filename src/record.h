#ifndef RECORD_H
#define RECORD_H

#include <iostream>
/*
Data structure representing a movie record stored in blocks in the database, consisting of 3 columns:
tconst (String)
averageRating (float)
numVotes (int)
*/
struct Record {
public:
    std::string tconst;
    float averageRating;
    int numVotes;

    //Setter
    void setRecord(std::string tconst, float averageRating, int numVotes);

    //Get record size
    int getRecordSize();

    std::string toString();

    Record(std::string tconst, float averageRating, int numVotes) : tconst(tconst), averageRating(averageRating), numVotes(numVotes) {};
    Record() {
        tconst = "";
        averageRating = -1;
        int numVotes = -1;
    }
};

#endif