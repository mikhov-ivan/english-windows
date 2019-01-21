#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP

#include "Globals.hpp"
#include "String.hpp"

using namespace std;

class Dictionary {
public:
	Dictionary(const String& _name);
	~Dictionary();

	bool rename(String& input);								// Assignes a new name to the dictionary
	bool rename(wchar_t* input, int len);					// Assignes a new name to the dictionary
	void add(THREE record);									// Adds new record to the dictionary
	void remove(int i);										// Removes word i'th from the dictionary
	String get(int i, int j);								// Designed to return i'th word's part (0 - eng; 1 - transcription; 2 - rus)
	bool set(int i, int j, String& str);					// Designed to return i'th word's part (0 - eng; 1 - transcription; 2 - rus)
	String dic();											// Returns name of the dictionary
	int count();											// Returns size of the dictionary

private:
	String name;
	vector< THREE > list;
};

#endif