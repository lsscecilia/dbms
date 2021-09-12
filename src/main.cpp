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
using std::vector;
using namespace std;

int main()
{

    int block_size;

    //Read Input File
    ifstream infile;
    infile.open("data.tsv");
    if (!infile)
    {
        cout << "Error in reading the file" << endl; //show error if can't read file
        exit(1);
    }

    //Initialize the block size based on input
    cout << "Enter Block Size" << endl;
    cin >> block_size;

    cout << "Building Storage with Block Size of " << block_size << " bytes...." <<  endl;

    //Declaring and Initializing the variables

    string row;
    Storage s;
    Block blk;
    Record r;
    int max_records_in_block = floor(block_size/r.getRecordSize());
    s.blocks.push_back(blk);
    int block_count = s.getNumBlocks()-1;

    //Skip Header
    //getline(infile, row);

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

        //Max number of records in block shouldn't exceed floor of BLOCK_SIZE / RECORD_SIZE
        if(s.blocks[block_count].getNumRecords() < max_records_in_block)
        {
            s.blocks[block_count].addRecord(r);//push a record r to block b
            //cout << "block " << block_count << " Num Records:" << s.blocks[block_count].getNumRecords() << endl;
            //cout<< "Current Block Size: " << s.blocks[block_count].getBlockSize() << endl;
        }
        else
        {
            //cout<< "Number Records " << s.blocks[block_count].getNumRecords() << endl;
            //cout<< "Ending Block Size: " << s.blocks[block_count].getBlockSize() << endl;
            block_count++;//increment block counter
            Block blk; //create a new block
            s.addBlock(blk);
            s.blocks[block_count].addRecord(r);//push a record r to new block b
            //cout << "new_block " << block_count << " Num Records:" << s.blocks[block_count].getNumRecords() << endl;
        };


    }

    infile.close();//close the file after finish reading
    cout <<"--------------------------" << endl;
    cout <<"Number of Records: " << s.getNumRecords() << endl; //output the final number of blocks for a given block size
    cout <<"Size of Record: " << r.getRecordSize() << " bytes" << endl;
    cout <<"Number of Blocks: " << s.getNumBlocks() << endl;
    cout <<"Size of Block: " << block_size << " bytes" << endl;
    cout <<"Max Records in Block: " << max_records_in_block << " Records" << endl;
    cout <<"Min Records in Block: " << s.blocks[s.getNumBlocks()-1].getNumRecords() << " Records" << endl;
    cout <<"Total Size of Storage: " << ((double)s.getStorageSize() / 1024000)<< " Mb" << endl;
    cout <<"--------------------------" << endl;
    cout <<"Building B+ Tree Index on Average Rating" << endl;


/*    class BPlusTree tree;

    int maxNumKeys =  floor((block_size-4)/8); //Parameter N
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

    tree.Initialize(maxNumKeys);

    tree.Open_Output_File();
    vector <Block *> block_ptrs;
    for (int i = 0; i < s.getNumBlocks();i++){
        struct Block *block_ptr;
        block_ptr = &s.blocks[i];
        block_ptrs.push_back(block_ptr);
        std::ostringstream address;
        address << (void const *)block_ptr;
        std:string name = address.str();

        for(int j = 0; j < block_ptr->getNumRecords(); j++){
            tree.Insert(float(block_ptr->records[j].numVotes), i);

        }

    }
    cout << "B+ Tree has been created" << endl;
    tree.Print_Tree();

    cout <<"--------------------------" << endl;
    cout << "Searching Records with Average Rating = 8" << endl;
    tree.Search(8,s);

    cout <<"--------------------------" << endl;
    cout << "Searching Records with Average Rating from 7 - 9" << endl;

    tree.Search(7.0,9.0,s);

    cout <<"--------------------------" << endl;
    cout << "Deleting Records with Average Rating = 7" << endl;
    tree.Search_Delete(7.0,s);
    tree.Update_Tree(s,20);


    tree.Close_Output_File();

    return 1;*/
}