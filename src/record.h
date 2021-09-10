#ifndef RECORD_H
#define RECORD_H

#include <iostream>
using namespace std;
/*
Data structure representing a movie record stored in blocks in the database, consisting of 3 columns:
tconst (String)
averageRating (float)
numVotes (int)
*/
struct Record
{
    string tconst;
    float averageRating;
    int numVotes;

    int getRecordSize(){
        return sizeof(tconst) + sizeof(averageRating) + sizeof(numVotes);
    }

};

#endif