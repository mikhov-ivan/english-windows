#ifndef TEST_INFO_HPP
#define TEST_INFO_HPP

#include <vector>

class TestInfo {
public:
	TestInfo();
	virtual ~TestInfo();
	
	bool isRunning;
	int usedCount, wordsCount;
	int dicNum, wordNum;
	int mistakeStep;
	int questionCount, rightCount;

	std::vector< int > dics;						// All numbers of dictionaries used in current test
	std::vector< std::vector< bool > > used;
};

#endif