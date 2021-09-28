#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <cmath>
#include "record.h"
#include "block.h"
#include "storage.h"
#include "bplustree.h"

using std::vector;
using namespace std;

int main()
{

    int blockSize;

    //Read Input File
    ifstream infile;
    infile.open("../data/data.tsv");
    if (!infile)
    {
        cout << "Error in reading the file" << endl; //show error if can't read file
        exit(1);
    }

    //Initialize the block size based on input
    cout << "Enter Block Size" << endl;
    cin >> blockSize;

    cout << "Building Storage with Block Size of " << blockSize << " bytes...." <<  endl;

    //Declaring and Initializing the variables

    string row;
    Storage s;
    
    Record r;
    int max_records_in_block = floor(blockSize/r.getRecordSize());

	Block blk(max_records_in_block);
    s.blocks.push_back(blk);
    int blockCount = s.getNumBlocks()-1;
    cout << "init block count: " << blockCount << endl;

    // get parameter N for bplustree
    int sizeOfOtherDataInNode = sizeof(bool) + sizeof(uint32_t);
    int sizeOfKey = sizeof(float) + sizeof(Pointer);

    cout << "size of other shit in node: " << sizeOfOtherDataInNode << endl;
    cout << "size of key: " << sizeOfKey << endl; 

    int maxNumKeyInNode = floor((blockSize - sizeOfOtherDataInNode - sizeof(Pointer)) / sizeOfKey);
    cout << "max num key in node: " << maxNumKeyInNode << endl;

    BPlusTree bplustree(maxNumKeyInNode);

    //Skip Header
    //getline(infile, row);
    std::shared_ptr<Block> blockPtr;
    //For all lines in the file
    while(getline(infile, row)) {
        vector <string> tokens;
        istringstream iss(row);
        string token;

        //Push valid token to vector tokens
        while(getline(iss, token, '\t')){
            tokens.push_back(token);
        };

        //Taking values from each row and modifying the first column by removing 'tt' from the data.
        istringstream (tokens[0].erase(0,2)) >> r.tconst;
        istringstream (tokens[1]) >> r.averageRating;
        istringstream (tokens[2]) >> r.numVotes;


        //If there is space in current block, add record to this block else create a new block and and record to the new block

        //Max number of records in block shouldn't exceed floor of BLOCKSize / RECORD_SIZE
        if(s.blocks[blockCount].getNumRecords() < max_records_in_block)
        {
            s.blocks[blockCount].addRecord(r);//push a record r to block b
            //cout << "block " << blockCount << " Num Records:" << s.blocks[blockCount].getNumRecords() << endl;
            //cout<< "Current Block Size: " << s.blocks[blockCount].getBlockSize() << endl;
        }
        else
        {
            //cout<< "Number Records " << s.blocks[blockCount].getNumRecords() << endl;
            //cout<< "Ending Block Size: " << s.blocks[blockCount].getBlockSize() << endl;
            blockCount++;//increment block counter
            blockPtr = std::make_shared<Block>();
            s.addBlock(*blockPtr.get());
            s.blocks[blockCount].addRecord(r);//push a record r to new block b
            //cout << "new_block " << blockCount << " Num Records:" << s.blocks[blockCount].getNumRecords() << endl;
        };

        // insert into bplus tree
        // bplustree.InsertNode(r.numVotes, blockPtr);
    }

    infile.close();//close the file after finish reading
    cout <<"--------------------------" << endl;
    cout <<"Number of Records: " << s.getNumRecords() << endl; //output the final number of blocks for a given block size
    cout <<"Size of Record: " << r.getRecordSize() << " bytes" << endl;
    cout <<"Number of Blocks: " << s.getNumBlocks() << endl;
    cout <<"Size of Block: " << blockSize << " bytes" << endl;
    cout <<"Max Records in Block: " << max_records_in_block << " Records" << endl;
    cout <<"Min Records in Block: " << s.blocks[s.getNumBlocks()-1].getNumRecords() << " Records" << endl;
    cout <<"Total Size of Storage: " << ((double)s.getStorageSize() / 1024000)<< " Mb" << endl;
    cout <<"--------------------------" << endl;
    cout <<"Building B+ Tree Index on Average Rating" << endl;


    Pointer ptr;
    float random = 10;
    cout << "size of pointer: " << sizeof(ptr) << endl;
    cout << "size of float: " << sizeof(random) << endl; 
    cout << "bool: " << sizeof(bool) << endl;
    cout << "uint32_t: " << sizeof(uint32_t) << endl;

    int maxNumKeys =  floor((blockSize-4)/8); //Parameter N
    int min_internal = floor(maxNumKeys/2);
    int min_leaf = floor((maxNumKeys+1)/2);

    int num_levels = 2;

    for(int i = 1; i < 999; i++ ){
        int max_records_indexed = pow(maxNumKeys,i)*maxNumKeys;
        if(max_records_indexed >= s.getNumRecords()){
            break;

        }else{
            num_levels+=1;
        }
    }



    
    // bplustree.Initialize(maxNumKeys);

    // bplustree.Open_Output_File();
    // vector <Block *> block_ptrs;
    // for (int i = 0; i < s.getNumBlocks();i++){
    //     struct Block *block_ptr;
    //     block_ptr = &s.blocks[i];
    //     block_ptrs.push_back(block_ptr);
    //     std::ostringstream address;
    //     address << (void const *)block_ptr;
    //     std:string name = address.str();

    //     for(int j = 0; j < block_ptr->getNumRecords(); j++){
    //         bplustree.Insert(float(block_ptr->records[j].numVotes), i);

    //     }

    // }
    // cout << "B+ Tree has been created" << endl;
    // bplustree.Print_Tree();

    // cout <<"--------------------------" << endl;
    // cout << "Searching Records with Average Rating = 8" << endl;
    // bplustree.Search(8,s);

    // cout <<"--------------------------" << endl;
    // cout << "Searching Records with Average Rating from 7 - 9" << endl;

    // bplustree.Search(7.0,9.0,s);

    // cout <<"--------------------------" << endl;
    // cout << "Deleting Records with Average Rating = 7" << endl;
    // bplustree.Search_Delete(7.0,s);
    // bplustree.Update_Tree(s,20);


    // bplustree.Close_Output_File();

    // return 1;
}