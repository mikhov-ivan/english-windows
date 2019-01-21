#include <vector>
#include "Globals.hpp"

using namespace std;

class String {
public:
	String();
	String(int n);
	String(wchar_t* str, int len);
	String(wstring str);

	wchar_t* t_str();
	int get_length();
	void set_length(int _length);
	wchar_t char_at(int i);

	String& String::operator=(const String& str);
	String& String::operator+(const String& str);
	bool String::operator==(const String& str);
	bool String::operator!=(const String& str);

	int indexof(wchar_t ch, int start = 0, int stop = -1);
	int indexof(const wchar_t* ch, int start = 0, int stop = -1);

private:
	wchar_t a[MAX_WORD_LENGTH];
	int length;
};