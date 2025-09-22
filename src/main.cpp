#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <map>
#include "Avl_tree.h"

using namespace std;
using namespace std::chrono;

void run_demo() {
	// initialization
	Avl_tree <int, string> tr{ {50, "word-50"}, {4, "word-4"}, {27, "word-27"}, {9, "word-9"}, {10, "word-10"}, {20,"word-20"}, {14, "word-14"}, {15,"word-15"}, {30, "word-30"} , {35,"word-35"} , {40, "word-40"} };
	// print tree
	cout << "Tree:" << endl;
	tr.print();
	tr.insert(17);
	cout << endl;
	cout << "Tree:" << endl;
	tr.print();
	cout << "tree size is: " << tr.size() << endl;
	if (tr.empty()) cout << "tree is empty" << endl;
	else cout << "tree is not empty" << endl;

	cout << "Printing our tree with iterators:" << endl;
	for (auto i = tr.begin(); i != tr.end(); ++i) {
		cout << i->key << " ";
	}
	cout << endl << "Reversive print with iterators:" << endl;
	auto i = tr.end();
	--i;
	for (i; i != tr.begin(); --i) {
		cout << i->key << " ";
	}
	cout << (tr.begin())->key << endl;


	cout << endl << "Erase 50:" << endl;
	tr.erase(50);
	tr.print();
	cout << "Clear Tree:" << endl;
	tr.clear();
	cout << "tree size is: " << tr.size() << endl;
	if (tr.empty()) {
		cout << "tree is empty" << endl;
	}
	else {
		cout << "tree is not empty" << endl;
	}
	tr.insert(50);
	tr[40] = "word-40";
	tr.print();
	tr.clear();
}

void run_bench(int maxN = 500'000, int numRepeats = 100, int batchSize = 50)
{
    ofstream out("results.csv");
    out << "N,batch,"
        "avl_insert_us,avl_find_us,avl_erase_us,"
        "map_insert_us,map_find_us,map_erase_us\n";

    Avl_tree<int, int> avl;
    std::map<int, int> mp;

    for (int blockSize = 100; blockSize <= maxN; blockSize *= 10) {
        avl.clear();
        mp.clear();
        int currentN = 0;

        for (int repeatIdx = 0; repeatIdx < numRepeats; ++repeatIdx) {
            const int prefillEnd = blockSize * (repeatIdx + 1) + batchSize * repeatIdx;
            const int measureEnd = (blockSize + batchSize) * (repeatIdx + 1);
            if (measureEnd > maxN) break;

            // --- prefill grow by blockSize ---
            for (int key = blockSize * repeatIdx + batchSize * repeatIdx; key < prefillEnd; ++key) {
                avl.insert(key, key);
                mp.emplace(key, key);
            }

            // --- create [prefillEnd, measureEnd) for find/erase ---
            for (int key = prefillEnd; key < measureEnd; ++key) {
                avl.insert(key, key);
                mp.emplace(key, key);
            }

            // ---- measure INSERT on fresh keys (shifted) ----
            auto t0 = high_resolution_clock::now();
            for (int key = prefillEnd; key < measureEnd; ++key)
                avl.insert(key + 1'000'000, key);
            auto t1 = high_resolution_clock::now();

            auto t2 = high_resolution_clock::now();
            for (int key = prefillEnd; key < measureEnd; ++key)
                mp.emplace(key + 1'000'000, key);
            auto t3 = high_resolution_clock::now();

            long long avl_ins = duration_cast<microseconds>(t1 - t0).count();
            long long map_ins = duration_cast<microseconds>(t3 - t2).count();

            // ---- measure FIND on existing block ----
            auto t4 = high_resolution_clock::now();
            for (int key = prefillEnd; key < measureEnd; ++key)
                (void)avl.find(key);
            auto t5 = high_resolution_clock::now();

            auto t6 = high_resolution_clock::now();
            for (int key = prefillEnd; key < measureEnd; ++key)
                (void)mp.find(key);
            auto t7 = high_resolution_clock::now();

            long long avl_fnd = duration_cast<microseconds>(t5 - t4).count();
            long long map_fnd = duration_cast<microseconds>(t7 - t6).count();

            // ---- measure ERASE on existing block ----
            auto t8 = high_resolution_clock::now();
            for (int key = prefillEnd; key < measureEnd; ++key)
                avl.erase(key);
            auto t9 = high_resolution_clock::now();

            auto t10 = high_resolution_clock::now();
            for (int key = prefillEnd; key < measureEnd; ++key)
                mp.erase(key);
            auto t11 = high_resolution_clock::now();

            long long avl_ers = duration_cast<microseconds>(t9 - t8).count();
            long long map_ers = duration_cast<microseconds>(t11 - t10).count();

            // restore erased keys
            for (int key = prefillEnd; key < measureEnd; ++key) {
                avl.insert(key, key);
                mp.emplace(key, key);
            }

            currentN += blockSize;
            out << currentN << ',' << batchSize << ','
                << avl_ins << ',' << avl_fnd << ',' << avl_ers << ','
                << map_ins << ',' << map_fnd << ',' << map_ers << '\n';
        }
    }
}

int main() {
	run_bench();
	return 0;
}