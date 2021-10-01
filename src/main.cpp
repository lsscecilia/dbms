#include <iostream>
#include <fstream>
#include <cmath>
#include <sstream>

#include "record.h"
#include "block.h"
#include "storage.h"
#include "bplustree.h"

int main() {

    // get block size
    int blockSize;
    std::cout << "Enter Block Size" << std::endl;
    std::cin >> blockSize;
    std::cout << "Building Storage with Block Size of " << blockSize << " bytes" <<  std::endl;

    /*
    1. max record in a block
    2. max keys in a Node (for bplustree) - aka parameter n
    3. max keys in a Linkedlist (for storing ptrs to block)
    */

   int maxRecordInBlock = floor(blockSize/sizeof(Record));

    int sizeOfOtherDataInNode = sizeof(bool) + sizeof(uint32_t);
    int sizeOfKey = sizeof(float) + 4;
    int maxNumKeyInNode = floor((blockSize - sizeOfOtherDataInNode - 4) / sizeOfKey);

    int numPtrInLL = floor(blockSize - sizeof(int) - 4/4);

    std::cout << "Max records in a block: " << maxRecordInBlock << std::endl;
    std::cout << "Max key in a B+ tree node: " << maxNumKeyInNode << std::endl;
    std::cout << "Max ptr in linkedlist: " << numPtrInLL << std::endl;

    // init storage
    Storage storage;
    Block initBlock(maxRecordInBlock);
    std::shared_ptr<Block> initBlockPtr = std::make_shared<Block>(std::move(initBlock));
    storage.addBlock(initBlockPtr);
    int lastBlockIndex = storage.getNumBlocks() - 1;

    // init bplustree
    BPlusTree bplustree(maxNumKeyInNode, numPtrInLL);

    //Read Input File
    std::ifstream infile;
    infile.open("../data/data.tsv");

    // infile.open("../data/data.tsv");
    if (!infile) {
        std::cout << "Error in reading the file" << std::endl; //show error if can't read file
        exit(1);
    }

    // process data line by line
    std::string line;
    int count = 0;

    getline(infile, line);     // skip header

    while(getline(infile, line)) {
        std::vector<std::string> fields;
        std::istringstream iss(line);
        std::string field;
        
        // keep fields in vector
        while(getline(iss, field, '\t')) {
            fields.push_back(field);
        };

        // convert into Record
        Record newRecord;
        newRecord.tconst = fields[0];
        std::istringstream (fields[1]) >> newRecord.averageRating;
        std::istringstream (fields[2]) >> newRecord.numVotes;

        // insert into block in storage if there is space in the last block
        std::shared_ptr<Block> blockPtr;
        if (storage.blocks[lastBlockIndex]->haveSpace()) {
            storage.blocks[lastBlockIndex]->addRecord(newRecord);
            blockPtr = storage.blocks[lastBlockIndex];
        } else {
            Block newBlock(maxRecordInBlock);
            newBlock.addRecord(newRecord);
            blockPtr = std::make_shared<Block>(newBlock);
            storage.addBlock(blockPtr);
            lastBlockIndex++;
        }

        bplustree.InsertNode(newRecord.numVotes, blockPtr);
        // if (count == 100000)
        //     break;
        count++;
        // if (count%100000 == 0)
        std::cerr << count << std::endl;;
    }

    infile.close();     // close file
    std::cout <<"--------------------------" << std::endl;
    std::cout <<"Number of Records: " << storage.getNumRecords() << std::endl; //output the final number of blocks for a given block size
    std::cout <<"Size of Record: " << sizeof(Record) << " bytes" << std::endl;
    std::cout <<"Number of Blocks: " << storage.getNumBlocks() << std::endl;
    std::cout <<"Size of Block: " << blockSize << " bytes" << std::endl;
    std::cout <<"Max Records in Block: " << maxRecordInBlock << " Records" << std::endl;
    std::cout <<"Min Records in Block: " << storage.blocks[storage.getNumBlocks()-1]->getNumRecords() << " Records" << std::endl;
    std::cout <<"Total Size of Storage: " << ((double)storage.getStorageSize() / 1024000)<< " Mb" << std::endl;
    std::cout <<"--------------------------" << std::endl;


    std::cout << "find records with numVotes = 500" << std::endl;
    bplustree.FindRange(22, 22);

    bplustree.FindRange(18,18);

    bplustree.PrintStats();

    std::cout << "program end" << std::endl;
}