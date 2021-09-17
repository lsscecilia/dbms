#include "gtest/gtest.h"
#include "memory.h"

#include "../src/block.h"
#include "../src/bplustree.h"

BPlusTree init() {
	BPlusTree bplustree(3);
	
	Record r1("r1", 1.00, 1);
	Record r2("r2", 2.00, 2);
	Record r3("r3", 3.00, 1);
	Record r4("r4", 4.00, 2);
	Record r5("r5", 5.00, 1);
	Record r6("r6", 6.00, 2);
	Record r7("r7", 7.00, 1);
	Record r8("r8", 8.00, 2);
	Record r9("r9", 9.00, 1);
	Record r10("r10", 10.00, 1);

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

    std::cerr << "b1 ptr: " << b1 << std::endl;
    std::cerr << "b2 ptr: " << b2 << std::endl;


	bplustree.InsertNode(1.00, b1, 0);
	bplustree.InsertNode(2.00, b1, 1);

	bplustree.InsertNode(3.00, b2, 0);
    bplustree.InsertNode(4.00, b2, 1);

    bplustree.InsertNode(5.00, b3, 0);
    bplustree.InsertNode(6.00, b3, 1);
 
	bplustree.InsertNode(7.00, b4, 0);
    bplustree.InsertNode(8.00, b4, 1);

	std::cerr << "INSERT 9" << std::endl;
	bplustree.InsertNode(9.00, b5, 0);
	std::cerr << "INSERT 10" << std::endl;
    bplustree.InsertNode(10.00, b5, 1);

	bplustree.InsertNode(11.00, b6, 0);
	bplustree.InsertNode(12.00, b6, 1);

	bplustree.InsertNode(13.00, b7, 0);
	bplustree.InsertNode(14.00, b7, 1);

	bplustree.InsertNode(15.00, b8, 0);
	std::cerr << "INSERT 16" << std::endl;
	bplustree.InsertNode(16.00, b8, 1);
	return bplustree;
}

TEST (BPlusTreeTest, insertion) {
	BPlusTree bplustree = init();

	std::shared_ptr<Node> root = bplustree.GetRoot();
	std::cerr << "is root node leaf?" << root->isLeaf << std::endl;
	std::cerr << "keys: ";
	for (float key : root->keys) {
		std::cerr << key << " ";
	}
	std::cerr << "\n pointers: ";
	for (Pointer ptr : root->ptrs) {
		std::cerr << ptr.ptr << " " << ptr.offset << " ";
        std::shared_ptr<Node> n1 = std::static_pointer_cast<Node>(ptr.ptr);
        std::cerr << "key in ptrs ";
        for (float key : n1->keys) {
            std::cerr << key << " ";
        }
        std::cerr << std::endl;
		// std::cerr << "n1: " << n1 << std::endl; 
		// std::cerr << "test find parent: " << bplustree.FindParent(bplustree.GetRoot(), n1) << std::endl;
		// std::cerr << "end" << std::endl;

	}

	std::cerr << "test print node: " << std::endl;
	bplustree.PrintNode(root);

	for (int i = 0; i < root->ptrs.size(); i++) {
		std::shared_ptr<Node> itr = std::static_pointer_cast<Node>(root->ptrs[i].ptr);
		bplustree.PrintNode(itr);
		std::cerr << "child: " << std::endl;
		for (int r = 0; r < itr->ptrs.size(); r++) {
			bplustree.PrintNode(std::static_pointer_cast<Node>(itr->ptrs[r].ptr));
		}
		std::cerr << "end child" << std::endl;
	}

	// try both odd and even for the size (might have problem with splitting)
	// test does the leave node link together or not
}

TEST (BPlusTreeTest, linkLeafLevel) {
	BPlusTree bplustree = init();
	std::shared_ptr<Node> root = bplustree.GetRoot();
	std::shared_ptr<Node> ll = std::static_pointer_cast<Node>(root->ptrs[0].ptr);
	while (ll != nullptr) {
		if (ll->isLeaf) {
			break;
		} else {
			ll = std::static_pointer_cast<Node>(ll->ptrs[0].ptr);
		}
	}

	while (ll != nullptr) {
		for (float key : ll->keys) {
			std::cout << key << ",";
		}
		std::cout << "next node" << std::endl;
		if (ll->keys.size() == ll->ptrs.size())
			break;
		ll = std::static_pointer_cast<Node>(ll->ptrs[ll->ptrs.size()-1].ptr);
	}
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

int main(int argc, char** argv){
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}