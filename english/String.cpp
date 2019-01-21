#include <vector>
#include "String.hpp"
#include "Globals.hpp"

using namespace std;

String::String() {
	length = 0;
}

String::String(int n) {
	if (!n) {
		length = 1;

		a[0] = '0';
		a[1] = '\0';
	} else {
		int digit = n % 10;
		vector< wchar_t > digits;
		while (n) {
			digits.push_back('0' + digit);
			n = (int)(n / 10);
			digit = n % 10;
		}

		length = 0;
		for (int i = digits.size() - 1; i >= 0; i--) {
			a[length] = digits[i];
			length++;
		}
		a[length] = '\0';
	}
}

String::String(wchar_t* str, int len) {
	for (length = 0; length < len; length++) {
		a[length] = str[length];
		if (a[length] == '\0') {
			length++;
			break;
		}
	}
}

String::String(wstring str) {
	if (str.length()) {
		int pos = str.find('\n');
		if (pos != -1) {
			str[pos] = '\0';
		} else {
			str += L'\0';
		}

		length = str.length();
		for (int i = 0; i < length; i++) {
			a[i] = str[i];
		}
	}
}

wchar_t* String::t_str() {
	return a;
}

int String::get_length() {
	return length;
}

void String::set_length(int _length) {
	length = _length;
}

wchar_t String::char_at(int i) {
	return a[i];
}

String& String::operator=(const String& str) {
	memcpy(a, str.a, sizeof(str.a));
	length = str.length;
	return *this;
}

String& String::operator+(const String& str) {
	for (int i = length - 1; i < length + str.length - 1; i++) {
		a[i] = str.a[i - length + 1];
	}

	length += str.length - 1;
	return *this;
}

bool String::operator==(const String& str) {
	if (length != str.length) return false;
	for (int i = 0; i < length; i++) {
		if (a[i] != str.a[i]) {
			return false;
		}
	}

	return true;
}

bool String::operator!=(const String& str) {
	if (length != str.length) return true;
	for (int i = 0; i < length; i++) {
		if (a[i] != str.a[i]) {
			return true;
		}
	}

	return false;
}

int String::indexof(wchar_t ch, int start, int stop) {
	if (stop == -1) stop = length;
	for (int i = start; i < stop; i++) {
		if (a[i] == ch) {
			return i;
		}
	}

	return -1;
}

int String::indexof(const wchar_t* ch, int start, int stop) {
	int subLen = wcslen(ch);

	if (stop == -1) stop = length;
	for (int i = start; i < stop; i++) {
		if (a[i] == ch[0]) {
			for (int j = 0; j < subLen; j++) {
				if ((i + j) > length || a[i + j] != ch[j]) {
					break;
				}

				if (j == subLen - 1) {
					return i;
				}
			}
		}
	}

	return -1;
}