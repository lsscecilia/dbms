#include "gtest/gtest.h"
#include "memory.h"

#include "../src/block.h"
#include "../src/bplustree.h"

BPlusTree initDuplicatedTree() {
	BPlusTree bplustree(3);
	
	Record r1("r1", 1.00, 1);
	Record r2("r2", 2.00, 2);
	Record r3("r3", 3.00, 2);
	Record r4("r4", 4.00, 3);
	Record r5("r5", 5.00, 3);
	Record r6("r6", 6.00, 3);
	Record r7("r7", 7.00, 4);
	Record r8("r8", 8.00, 5);
	Record r9("r9", 9.00, 6);
	Record r10("r10", 10.00, 7);

	Record r11("r11", 11.00, 1);
	Record r12("r12", 12.00, 2);
	Record r13("r13", 13.00, 1);
	Record r14("r14", 14.00, 2);
	Record r15("r15", 15.00, 1);
	Record r16("r16", 16.00, 2);
	Record r17("r17", 17.00, 1);
	Record r18("r18", 18.00, 2);
	Record r19("r19", 19.00, 1);

	// each block only 2 record
	std::shared_ptr<Block> b1 = std::make_shared<Block>(2);

	std::shared_ptr<Block> b2 = std::make_shared<Block>(2);
	std::shared_ptr<Block> b3 = std::make_shared<Block>(2);
	std::shared_ptr<Block> b4 = std::make_shared<Block>(2);
	std::shared_ptr<Block> b5 = std::make_shared<Block>(2);

	std::shared_ptr<Block> b6 = std::make_shared<Block>(2);
	std::shared_ptr<Block> b7 = std::make_shared<Block>(2);
	std::shared_ptr<Block> b8 = std::make_shared<Block>(2);
	std::shared_ptr<Block> b9 = std::make_shared<Block>(2);
	std::shared_ptr<Block> b10 = std::make_shared<Block>(2);

	b1->addRecord(r1);
	b1->addRecord(r2);
    
    b2->addRecord(r3);
	b2->addRecord(r4);

    b3->addRecord(r5);
    b3->addRecord(r6);

	b4->addRecord(r7);
	b4->addRecord(r8);

	b5->addRecord(r9);
	b5->addRecord(r10);

	b6->addRecord(r11);
	b6->addRecord(r12);

	b7->addRecord(r13);
	b7->addRecord(r14);

	b8->addRecord(r15);
	b8->addRecord(r16);

	b9->addRecord(r17);
	b9->addRecord(r18);
	
	bplustree.InsertKey(1, b1);
	bplustree.InsertKey(2, b1);

	bplustree.InsertKey(2, b2);
    bplustree.InsertKey(3, b2);

    bplustree.InsertKey(3, b3);
    bplustree.InsertKey(3, b3);
 
	bplustree.InsertKey(4, b4);
    bplustree.InsertKey(5, b4);

	bplustree.InsertKey(6, b5);
    bplustree.InsertKey(7, b5);

	return bplustree;
}

BPlusTree initDeletion() {
	BPlusTree bplustree(3);
	
	Record r1("r1", 1.00, 1);
	Record r2("r2", 2.00, 2);
	Record r3("r3", 3.00, 3);
	Record r4("r4", 4.00, 4);
	Record r5("r5", 5.00, 5);
	Record r6("r6", 6.00, 6);
	Record r7("r7", 7.00, 7);
	Record r8("r8", 8.00, 8);
	Record r9("r9", 9.00, 9);
	Record r10("r10", 10.00, 10);

	Record r11("r11", 11.00, 11);
	Record r12("r12", 12.00, 12);
	Record r13("r13", 13.00, 13);
	Record r14("r14", 14.00, 14);
	Record r15("r15", 15.00, 15);
	Record r16("r16", 16.00, 16);
	Record r17("r17", 17.00, 17);
	Record r18("r18", 18.00, 18);
	Record r19("r19", 19.00, 19);

	// each block only 2 record
	std::shared_ptr<Block> b1 = std::make_shared<Block>(2);
	std::shared_ptr<Block> b2 = std::make_shared<Block>(2);
	std::shared_ptr<Block> b3 = std::make_shared<Block>(2);
	std::shared_ptr<Block> b4 = std::make_shared<Block>(2);
	std::shared_ptr<Block> b5 = std::make_shared<Block>(2);

	std::shared_ptr<Block> b6 = std::make_shared<Block>(2);
	std::shared_ptr<Block> b7 = std::make_shared<Block>(2);
	std::shared_ptr<Block> b8 = std::make_shared<Block>(2);
	std::shared_ptr<Block> b9 = std::make_shared<Block>(2);
	std::shared_ptr<Block> b10 = std::make_shared<Block>(2);

	b1->addRecord(r1);
	b1->addRecord(r2);
    
    b2->addRecord(r3);
	b2->addRecord(r4);

    b3->addRecord(r5);
    b3->addRecord(r6);

	b4->addRecord(r7);
	b4->addRecord(r8);

	b5->addRecord(r9);
	b5->addRecord(r10);

	b6->addRecord(r11);
	b6->addRecord(r12);

	b7->addRecord(r13);
	b7->addRecord(r14);

	b8->addRecord(r15);
	b8->addRecord(r16);

	b9->addRecord(r17);
	b9->addRecord(r18);

	bplustree.InsertKey(1.00, b1);
	bplustree.InsertKey(2.00, b1);

	bplustree.InsertKey(3.00, b2);
    bplustree.InsertKey(4.00, b2);

    bplustree.InsertKey(5.00, b3);
    bplustree.InsertKey(6.00, b3);
 
	bplustree.InsertKey(7.00, b4);
    bplustree.InsertKey(8.00, b4);

	bplustree.InsertKey(9.00, b5);
    bplustree.InsertKey(10.00, b5);

	bplustree.InsertKey(11.00, b6);
	bplustree.InsertKey(12.00, b6);

	bplustree.InsertKey(13.00, b7);
	bplustree.InsertKey(14.00, b7);

	bplustree.InsertKey(16.00, b8);

	bplustree.InsertKey(17.00, b9);

	bplustree.InsertKey(15.00, b8);
	bplustree.InsertKey(18.00, b9);

	return bplustree;
}

BPlusTree init() {
	BPlusTree bplustree(3);
	
	Record r1("r1", 1.00, 1);
	Record r2("r2", 2.00, 2);
	Record r3("r3", 3.00, 3);
	Record r4("r4", 4.00, 4);
	Record r5("r5", 5.00, 5);
	Record r6("r6", 6.00, 6);
	Record r7("r7", 7.00, 7);
	Record r8("r8", 8.00, 8);
	Record r9("r9", 9.00, 9);
	Record r10("r10", 10.00, 10);

	Record r11("r11", 11.00, 11);
	Record r12("r12", 12.00, 12);
	Record r13("r13", 13.00, 13);
	Record r14("r14", 14.00, 14);
	Record r15("r15", 15.00, 15);
	Record r16("r16", 16.00, 16);
	Record r17("r17", 17.00, 17);
	Record r18("r18", 18.00, 18);
	Record r19("r19", 19.00, 19);

	// each block only 2 record
	std::shared_ptr<Block> b1 = std::make_shared<Block>(2);
	// std::cerr << "block address: " << b1 << std::endl;
	std::shared_ptr<Block> b2 = std::make_shared<Block>(2);
	// std::cerr << "block address b2: " << b2 << std::endl;
	std::shared_ptr<Block> b3 = std::make_shared<Block>(2);
	std::shared_ptr<Block> b4 = std::make_shared<Block>(2);
	std::shared_ptr<Block> b5 = std::make_shared<Block>(2);

	std::shared_ptr<Block> b6 = std::make_shared<Block>(2);
	std::shared_ptr<Block> b7 = std::make_shared<Block>(2);
	std::shared_ptr<Block> b8 = std::make_shared<Block>(2);
	std::shared_ptr<Block> b9 = std::make_shared<Block>(2);
	std::shared_ptr<Block> b10 = std::make_shared<Block>(2);

	b1->addRecord(r1);
	b1->addRecord(r2);
    
    b2->addRecord(r3);
	b2->addRecord(r4);

    b3->addRecord(r5);
    b3->addRecord(r6);

	b4->addRecord(r7);
	b4->addRecord(r8);

	b5->addRecord(r9);
	b5->addRecord(r10);

	b6->addRecord(r11);
	b6->addRecord(r12);

	b7->addRecord(r13);
	b7->addRecord(r14);

	b8->addRecord(r15);
	b8->addRecord(r16);

	b9->addRecord(r17);
	b9->addRecord(r18);

	bplustree.InsertKey(1.00, b1);
	bplustree.InsertKey(2.00, b1);

	bplustree.InsertKey(3.00, b2);
    bplustree.InsertKey(4.00, b2);

    bplustree.InsertKey(5.00, b3);
    bplustree.InsertKey(6.00, b3);
 
	bplustree.InsertKey(7.00, b4);
    bplustree.InsertKey(8.00, b4);

	bplustree.InsertKey(9.00, b5);
    bplustree.InsertKey(10.00, b5);

	bplustree.InsertKey(11.00, b6);
	bplustree.InsertKey(12.00, b6);

	bplustree.InsertKey(13.00, b7);
	bplustree.InsertKey(14.00, b7);

	bplustree.InsertKey(15.00, b8);
	bplustree.InsertKey(16.00, b8);

	bplustree.InsertKey(17.00, b9);

	return bplustree;
}

void PrintTreeWith2Level(BPlusTree& bplustree) {
	std::cerr << "Root Node: " << std::endl;
	std::shared_ptr<Node> root = bplustree.GetRoot();
	bplustree.PrintNode(root);
	std::cerr << std::endl;

	for (int i = 0; i < root->ptrs.size(); i++) {
		std::shared_ptr<Node> itr = std::static_pointer_cast<Node>(root->ptrs[i]);
		std::cerr << "level 1 node: " << std::endl;
		bplustree.PrintNode(itr);
		std::cerr << "level 2 node: " << std::endl;
		for (int r = 0; r < itr->ptrs.size(); r++) {
			bplustree.PrintNode(std::static_pointer_cast<Node>(itr->ptrs[r]));
		}
		std::cerr << std::endl;
	}
	bplustree.PrintStats();
}

void PrintTreeWith1Level(BPlusTree& bplustree) {
	std::shared_ptr<Node> root = bplustree.GetRoot();
	std::cerr << "Root Node: " << std::endl;
	bplustree.PrintNode(root);

	for (int i = 0; i < root->ptrs.size(); i++) {
		std::shared_ptr<Node> itr = std::static_pointer_cast<Node>(root->ptrs[i]);
		std::cerr << "level 1 node: " << std::endl;
		bplustree.PrintNode(itr);
	}
	std::cerr << std::endl;
}

TEST (BPlusTreeTest, insertion) {
	BPlusTree bplustree = init();

	std::shared_ptr<Node> root = bplustree.GetRoot();

	std::cerr << "keys: ";
	for (float key : root->keys) {
		std::cerr << key << " ";
	}
	std::cerr << "\n";

	PrintTreeWith2Level(bplustree);
	// try both odd and even for the size (might have problem with splitting)
	// test does the leave node link together or not
}

TEST (BPlusTreeTest, linkLeafLevel) {
	BPlusTree bplustree = init();

	std::shared_ptr<Node> root = bplustree.GetRoot();
	std::shared_ptr<Node> ll = std::static_pointer_cast<Node>(root->ptrs[0]);
	while (ll != nullptr) {
		if (ll->isLeaf) {
			break;
		} else {
			ll = std::static_pointer_cast<Node>(ll->ptrs[0]);
		}
	}

	std::cerr << "get first leaf node ..  " << std::endl;
	
	while (ll != nullptr) {
		for (int i = 0; i < ll->keys.size(); i++) {
			std::cout << ll->keys[i] << ",";
			std::shared_ptr<std::vector<std::shared_ptr<Block>>> llForEachKey =  std::static_pointer_cast<std::vector<std::shared_ptr<Block>>>(ll->ptrs[i]);
			for (int r = 0; r < llForEachKey->size(); r++) {
				std::shared_ptr<Block> blk = llForEachKey->at(r);
				std::vector<Record> records = blk->getRecord(ll->keys[i]);
				for (Record r : records) {
					std::cout << "tconst: " << r.tconst << ", numVotes" << r.numVotes << " | ";
				}
			}
		}
		std::cout << "next node" << std::endl;
		if (ll->keys.size() == ll->ptrs.size())
			break;
		ll = std::static_pointer_cast<Node>(ll->ptrs[ll->ptrs.size()-1]);
	}
}

TEST (BPlusTree, insertDuplicate) {
	BPlusTree bplustree(3);
	
	Record r1("r1", 1.00, 1);
	Record r2("r2", 2.00, 1);
	Record r3("r3", 3.00, 1);
	Record r4("r4", 4.00, 2);
	Record r5("r5", 5.00, 2);
	Record r6("r6", 6.00, 3);
	Record r7("r7", 7.00, 4);
	Record r8("r8", 8.00, 5);
	

	std::shared_ptr<Block> b1 = std::make_shared<Block>(2);
	std::shared_ptr<Block> b2 = std::make_shared<Block>(2);
	std::shared_ptr<Block> b3 = std::make_shared<Block>(2);
	std::shared_ptr<Block> b4 = std::make_shared<Block>(2);

	b1->addRecord(r1);
	b1->addRecord(r2);
    
    b2->addRecord(r3);
	b2->addRecord(r4);

	b3->addRecord(r5);
    b3->addRecord(r6);
	b4->addRecord(r7);
	b4->addRecord(r8);

	bplustree.InsertKey(1, b1);
	bplustree.InsertKey(1, b1);

	bplustree.InsertKey(1, b2);
    bplustree.InsertKey(2, b2);

	bplustree.InsertKey(2, b3);
    bplustree.InsertKey(3, b3);
 
	bplustree.InsertKey(4, b4);
    bplustree.InsertKey(5, b4);

	std::shared_ptr<Node> root = bplustree.GetRoot();
	std::cerr << "test print node: " << std::endl;
	bplustree.PrintNode(root);

	bplustree.FindRange(1, 1);

}

TEST (BPlusTree, findDuplicateKeys) {
	BPlusTree bplustree = initDuplicatedTree();
	std::shared_ptr<Node> root = bplustree.GetRoot();
	bplustree.PrintNode(root);

	for (int i = 0; i < root->ptrs.size(); i++) {
		std::shared_ptr<Node> itr = std::static_pointer_cast<Node>(root->ptrs[i]);
		bplustree.PrintNode(itr);
	}

	bplustree.FindRange(2, 2);
	bplustree.FindRange(3, 3);
}

TEST (BPlusTreeTest, findrange4to10) {
	std::cerr << "test find range - 4 to 10" << std::endl;
	BPlusTree bplustree = init();
	bplustree.FindRange(4, 10);
}

TEST (BPlusTreeTest, findrange4to9) {
	std::cerr << "test find range - 4 to 9" << std::endl;
	BPlusTree bplustree = init();
	bplustree.FindRange(4, 9);
}

TEST (BPlusTreeTest, findrange1to1) {
	std::cerr << "test find range - 1 to 1" << std::endl;
	BPlusTree bplustree = init();
	bplustree.FindRange(1, 1);
}

TEST (BPlusTreeTest, findrange1to16) {
	std::cerr << "test find range - 1 to 16" << std::endl;
	BPlusTree bplustree = init();
	bplustree.FindRange(1, 16);
}

// err
TEST (BPlusTreeTest, findrange10to4) {
	std::cerr << "test find range - 10 to 4" << std::endl;
	BPlusTree bplustree = init();
	bplustree.FindRange(10, 4);
}

TEST (BPlusTree, deletionCase1) {
	BPlusTree bplustree = initDeletion();
	std::cerr << "finish insertion" << std::endl;
	bplustree.DeleteKey(16);

	PrintTreeWith2Level(bplustree);
}

TEST (BPlusTree, deletionCase2) {
	BPlusTree bplustree = initDeletion();
	std::cerr << "finish insertion" << std::endl;
	bplustree.DeleteKey(16);
	bplustree.DeleteKey(18);

	PrintTreeWith2Level(bplustree);
}

TEST (BPlusTree, deletionCase3) {
	BPlusTree bplustree = initDeletion();
	std::cerr << "finish insertion" << std::endl;
	bplustree.DeleteKey(16);
	bplustree.DeleteKey(18);
	bplustree.DeleteKey(17);

	PrintTreeWith2Level(bplustree);
}

TEST (BPlusTree, deletionCase3_2) {
	BPlusTree bplustree = initDeletion();
	std::cerr << "finish insertion" << std::endl;
	bplustree.DeleteKey(16);
	bplustree.DeleteKey(18);
	bplustree.DeleteKey(15);

	PrintTreeWith2Level(bplustree);
}


TEST (BPlusTree, deletionCase3_3) {
	BPlusTree bplustree = initDeletion();
	std::cerr << "finish insertion" << std::endl;
	bplustree.DeleteKey(16);
	bplustree.DeleteKey(18);
	bplustree.DeleteKey(17);
	bplustree.DeleteKey(10);

	PrintTreeWith2Level(bplustree);
}

TEST (BPlusTree, deletionCase3_4) {
	BPlusTree bplustree = initDeletion();
	std::cerr << "finish insertion" << std::endl;
	bplustree.DeleteKey(16);
	bplustree.DeleteKey(18);
	bplustree.DeleteKey(17);
	bplustree.DeleteKey(10);
	bplustree.DeleteKey(4);

	PrintTreeWith2Level(bplustree);
}

TEST (BPlusTree, deletionCaseRemoveKeyFoundInKeysAbove) {
	BPlusTree bplustree = initDeletion();
	std::cerr << "finish insertion" << std::endl;
	bplustree.DeleteKey(13);

	PrintTreeWith2Level(bplustree);
}

TEST (BPlusTree, deletionCase3_5) {
	BPlusTree bplustree = initDeletion();
	std::cerr << "finish insertion" << std::endl;
	bplustree.DeleteKey(16);
	bplustree.DeleteKey(18);
	bplustree.DeleteKey(17);
	bplustree.DeleteKey(10);
	bplustree.DeleteKey(4);
	bplustree.DeleteKey(13);

	PrintTreeWith2Level(bplustree);
}

TEST (BPlusTree, deletionCase3_6) {
	BPlusTree bplustree = initDeletion();
	std::cerr << "finish insertion" << std::endl;
	bplustree.DeleteKey(16);
	bplustree.DeleteKey(18);
	bplustree.DeleteKey(17);
	bplustree.DeleteKey(10);
	bplustree.DeleteKey(4);
	bplustree.DeleteKey(13);
	bplustree.DeleteKey(12);

	PrintTreeWith2Level(bplustree);
}

TEST (BPlusTree, deleteTreeStep1) {
	BPlusTree bplustree = initDeletion();
	std::cerr << "finish insertion" << std::endl;
	bplustree.DeleteKey(16);
	bplustree.DeleteKey(18);
	bplustree.DeleteKey(17);
	bplustree.DeleteKey(10);
	bplustree.DeleteKey(4);
	bplustree.DeleteKey(13);
	bplustree.DeleteKey(12);
	bplustree.DeleteKey(9);
	bplustree.DeleteKey(15);

	PrintTreeWith2Level(bplustree);
	std::shared_ptr<Node> root = bplustree.GetRoot();

	std::cerr << "\ntest linking of leaf nodes" << std::endl;
	std::shared_ptr<Node> ll = std::static_pointer_cast<Node>(root);
	while (ll != nullptr) {
		if (ll->isLeaf) {
			break;
		} else {
			ll = std::static_pointer_cast<Node>(ll->ptrs[0]);
		}
	}

	while (ll != nullptr) {
		for (float key : ll->keys) {
			std::cout << key << ",";
		}
		std::cout << "next node" << std::endl;
		if (ll->keys.size() == ll->ptrs.size())
			break;
		ll = std::static_pointer_cast<Node>(ll->ptrs[ll->ptrs.size()-1]);
	}
}

TEST (BPlusTree, deleteTreeStep2) {
	BPlusTree bplustree = initDeletion();
	std::cerr << "finish insertion" << std::endl;
	bplustree.DeleteKey(16);
	bplustree.DeleteKey(18);
	bplustree.DeleteKey(17);
	bplustree.DeleteKey(10);
	bplustree.DeleteKey(4);
	bplustree.DeleteKey(13);
	bplustree.DeleteKey(12);
	bplustree.DeleteKey(9);
	bplustree.DeleteKey(15);
	bplustree.DeleteKey(7);
	bplustree.DeleteKey(14);

	PrintTreeWith1Level(bplustree);
}

TEST (BPlusTree, deleteTreeStep3) {
	BPlusTree bplustree = initDeletion();
	std::cerr << "finish insertion" << std::endl;
	bplustree.DeleteKey(16);
	bplustree.DeleteKey(18);
	bplustree.DeleteKey(17);
	bplustree.DeleteKey(10);
	bplustree.DeleteKey(4);
	bplustree.DeleteKey(13);
	bplustree.DeleteKey(12);
	bplustree.DeleteKey(9);
	bplustree.DeleteKey(15);
	bplustree.DeleteKey(7);
	bplustree.DeleteKey(14);
	bplustree.DeleteKey(1);
	bplustree.DeleteKey(8);

	PrintTreeWith1Level(bplustree);
}

TEST (BPlusTree, deleteTreeStep4) {
	BPlusTree bplustree = initDeletion();
	std::cerr << "finish insertion" << std::endl;
	bplustree.DeleteKey(16);
	bplustree.DeleteKey(18);
	bplustree.DeleteKey(17);
	bplustree.DeleteKey(10);
	bplustree.DeleteKey(4);
	bplustree.DeleteKey(13);
	bplustree.DeleteKey(12);
	bplustree.DeleteKey(9);
	bplustree.DeleteKey(15);
	bplustree.DeleteKey(7);
	bplustree.DeleteKey(14);
	bplustree.DeleteKey(1);
	bplustree.DeleteKey(8);
	bplustree.DeleteKey(3);

	PrintTreeWith1Level(bplustree);
}

TEST (BPlusTree, deleteTreeStep5) {
	BPlusTree bplustree = initDeletion();
	std::cerr << "finish insertion" << std::endl;
	bplustree.DeleteKey(16);
	bplustree.DeleteKey(18);
	bplustree.DeleteKey(17);
	bplustree.DeleteKey(10);
	bplustree.DeleteKey(4);
	bplustree.DeleteKey(13);
	bplustree.DeleteKey(12);
	bplustree.DeleteKey(9);
	bplustree.DeleteKey(15);
	bplustree.DeleteKey(7);
	bplustree.DeleteKey(14);
	bplustree.DeleteKey(1);
	bplustree.DeleteKey(8);
	bplustree.DeleteKey(3);
	bplustree.DeleteKey(5);

	std::shared_ptr<Node> root = bplustree.GetRoot();
	std::cerr << "Root Node: " << std::endl;
	bplustree.PrintNode(root);
}

TEST (BPlusTree, deleteTreeStepLast) {
	BPlusTree bplustree = initDeletion();
	bplustree.PrintStats();

	std::cerr << "\ndeleting... " << std::endl;
	bplustree.DeleteKey(16);
	bplustree.DeleteKey(18);
	bplustree.DeleteKey(17);
	bplustree.DeleteKey(10);
	bplustree.DeleteKey(4);
	bplustree.DeleteKey(13);
	bplustree.DeleteKey(12);
	bplustree.DeleteKey(9);
	bplustree.DeleteKey(15);
	bplustree.DeleteKey(7);
	bplustree.DeleteKey(14);
	bplustree.DeleteKey(1);
	bplustree.DeleteKey(8);
	bplustree.DeleteKey(3);
	bplustree.DeleteKey(5);
	bplustree.DeleteKey(6);
	bplustree.DeleteKey(2);
	bplustree.DeleteKey(11);
	std::cerr << "deletion completed \n" << std::endl;

	bplustree.PrintStats();
}

TEST (BPlusTree, leafLevelLinkAfterDeletion) {
	BPlusTree bplustree = initDeletion();
	std::cerr << "finish insertion" << std::endl;
	bplustree.DeleteKey(16);
	bplustree.DeleteKey(18);
	bplustree.DeleteKey(17);
	bplustree.DeleteKey(10);
	bplustree.DeleteKey(4);
	bplustree.DeleteKey(13);
	bplustree.DeleteKey(12);
	bplustree.DeleteKey(9);
	bplustree.DeleteKey(15);
	bplustree.DeleteKey(7);
	bplustree.DeleteKey(14);
	bplustree.DeleteKey(1);
	bplustree.DeleteKey(8);
	bplustree.DeleteKey(3);
	bplustree.DeleteKey(5);

	std::shared_ptr<Node> root = bplustree.GetRoot();
	std::cerr << "test print node: " << std::endl;
	bplustree.PrintNode(root);

	std::cerr << "\ntest linking in leaf level: " << std::endl;
	std::shared_ptr<Node> ll = std::static_pointer_cast<Node>(root);
	while (ll != nullptr) {
		if (ll->isLeaf) {
			break;
		} else {
			ll = std::static_pointer_cast<Node>(ll->ptrs[0]);
		}
	}

	while (ll != nullptr) {
		for (int i = 0; i < ll->keys.size(); i++) {
			std::cout << ll->keys[i] << ",";
			std::shared_ptr<std::vector<std::shared_ptr<Block>>> llForEachKey =  std::static_pointer_cast<std::vector<std::shared_ptr<Block>>>(ll->ptrs[i]);
			for (int r = 0; r < llForEachKey->size(); r++) {
				std::shared_ptr<Block> blk = llForEachKey->at(r);
				std::vector<Record> records = blk->getRecord(ll->keys[i]);
				for (Record r : records) {
					std::cout << "tconst: " << r.tconst << ", numVotes" << r.numVotes << " | ";
				}
			}
		}
		std::cout << "next node" << std::endl;
		if (ll->keys.size() == ll->ptrs.size())
			break;
		ll = std::static_pointer_cast<Node>(ll->ptrs[ll->ptrs.size()-1]);
	}
}

TEST (BPlusTree, initDeletion) {
	BPlusTree bplustree = initDeletion();
	std::shared_ptr<Node> root = bplustree.GetRoot();
	std::cerr << "test print node: " << std::endl;
	PrintTreeWith2Level(bplustree);
}

TEST (BPlusTree, deletionKeyDoesNotExists) {
	BPlusTree bplustree = initDeletion();
	bplustree.DeleteKey(20);
}

TEST (BPlusTree, deleteKeysFromDuplicatedTree) {
	BPlusTree bplustree = initDuplicatedTree();
	PrintTreeWith1Level(bplustree);

	std::cerr << "Deletion ..." << std::endl;
	bplustree.DeleteKey(2);

	PrintTreeWith1Level(bplustree);
}

int main(int argc, char** argv){
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}