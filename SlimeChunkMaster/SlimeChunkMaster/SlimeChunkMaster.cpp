#include "PatternSearch.h"
#include <vector>
#include <utility>
#include <iostream>
#include <string>
#include "Windows.h"
using namespace std;

int main() {
	while(true) {
		long long seed;

		cout << "Enter game seed : ";
		cin >> seed;

		int x1;
		int x2;
		int z1;
		int z2;

		cout << "\nEnter the 2 corners.\n";

		cout << "First point X coord : ";
		cin >> x1;
		cout << "First point Z coord : ";
		cin >> z1;

		cout << "Second point X coord : ";
		cin >> x2;
		cout << "Second point Z coord : ";
		cin >> z2;

		vector<pair<int, int>> positions = {make_pair(x1, z1), make_pair(x2, z2)};

		int width;
		int height;
		cout << "\nEnter the pattern dimensions.\n";

		cout << "Pattern height : ";
		cin >> height;
		cout << "Pattern width : ";
		cin >> width;

		vector<int> pattern;
		cout << "\nEnter the pattern. \nPut 1 for slime chunk and 0 for none. \nMake sure to press enter everytime you finish a row. \nAlso, follow the dimensions you inputted earlier.\n";
		
		for(int i = 0; i < height; i++) {
			string s;
			cin >> s;
			for(int j = 0; j < width; j++) {
				pattern.push_back((s.at(j) == '1' ? 1 : 0));
			}
		}	

		cout << "\nWould you like to stop the search when the pattern is found? (Answer with yes or no.)\n";
		bool searchFlag = false;
		string sf;
		cin >> sf;
		if (sf == "no") { 
			searchFlag = true; 
			cout << "\nSearch will keep going 'till the end.\n\n";			
		}
		else {
			cout << "\nSearch will stop after first found.\n\n";
		}

		cout << "\nStarting search...\n\n";
		PatternSearch* search = new PatternSearch(height, width, pattern);
		search->startSearch(PatternSearch::JAVA_EDITION, seed, positions, searchFlag, true);
	}
	system("pause");
    return 0;
}

//The following functions were used for debugging
bool isSlimeChunk(int xr, int zr) {
	int seed = (xr * 522133279) ^ zr;

	int N = 624;
	int M = 397;
	int* mt = new int[N];
	int mti;
	int mtiFast;
	int UPPER_MASK = 0x80000000;
	int LOWER_MASK = 0x7fffffff;
	int MATRIX_A = 0x9908b0df;
	int MAG_01[] = { 0, MATRIX_A };

	mt[0] = seed;
	for (mtiFast = 1; mtiFast <= M; mtiFast++) {
		mt[mtiFast] = 1812433253
			* ((((unsigned int)mt[mtiFast - 1]) >> 30) ^ mt[mtiFast - 1])
			+ mtiFast;
	}
	mti = N;

	if (mti == N) {
		mti = 0;
	}
	else if (mti > N) {
		mt[0] = 5489;
		for (mtiFast = 1; mtiFast <= M; mtiFast++) {
			mt[mtiFast] = 1812433253
				* (((unsigned int)(mt[mtiFast - 1]) >> 30) ^ mt[mtiFast - 1])
				+ mtiFast;
		}
		mti = 0;
	}

	if (mti >= N - M) {
		if (mti >= N - 1) {
			mt[N - 1] = MAG_01[mt[0] & 1]
				^ (((unsigned int)(mt[0] & LOWER_MASK | mt[N - 1] & UPPER_MASK)) >> 1)
				^ mt[M - 1];
		}
		else {
			mt[mti] = MAG_01[mt[mti + 1] & 1]
				^ (((unsigned int)(mt[mti + 1] & LOWER_MASK | mt[mti] & UPPER_MASK)) >> 1)
				^ mt[mti - (N - M)];
		}
	}
	else {
		mt[mti] = MAG_01[mt[mti + 1] & 1]
			^ (((unsigned int)(mt[mti + 1] & LOWER_MASK | mt[mti] & UPPER_MASK)) >> 1)
			^ mt[mti + M];

		if (mtiFast < N) {
			mt[mtiFast] = 1812433253
				* ((((unsigned int)mt[mtiFast - 1]) >> 30) ^ mt[mtiFast - 1])
				+ mtiFast;
			mtiFast++;
		}
	}

	int ret = mt[mti++];
	ret = ((ret ^ ((unsigned int)ret >> 11)) << 7) & 0x9d2c5680 ^ ret ^ ((unsigned int)ret >> 11);
	ret = (ret << 15) & 0xefc60000 ^ ret ^ (((unsigned int)((ret << 15) & 0xefc60000 ^ ret)) >> 18);

	return ((unsigned long long)ret) % 10 == 0;
}

void printMap() {
	for (int i = 0; i < 40; i++) {
	for (int j = 0; j < 40; j++) {
	cout << "\n";
	}
	}
	CHAR_INFO* screenBuffer = new CHAR_INFO[40 * 40];
	for (int i = 0; i < 40; i++) {
	for (int j = 0; j < 40; j++) {
	screenBuffer[i * 40 + j].Char.UnicodeChar = 0x2588;
	screenBuffer[i * 40 + j].Attributes = (isSlimeChunk(i, j) ? 0x000A : 0x000F);
	}
	}
	SMALL_RECT rect = { 0, 0, 40, 40 };
	WriteConsoleOutput(GetStdHandle(STD_OUTPUT_HANDLE), screenBuffer, { (short)40, (short)40 }, { 0,0 }, &rect);
	system("pause");
}
