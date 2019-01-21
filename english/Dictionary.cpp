#include "Dictionary.hpp"

#include <regex>

Dictionary::Dictionary(const String& _name) {
	name = _name;
}

Dictionary::~Dictionary() {
	
}

bool Dictionary::rename(wchar_t* input, int len) {
	wstring target(input);
	wregex regExpression(L"(.)+");
	if (!regex_match(target.cbegin(), target.cend(), regExpression)) {
		return false;
	}

	name = String(input, len);

	return true;
}

bool Dictionary::rename(String& input) {
	wstring target(input.t_str());
	wregex regExpression(L"(.)+");
	if (!regex_match(target.cbegin(), target.cend(), regExpression)) {
		return false;
	}

	name = input;

	return true;
}

void Dictionary::add(THREE record) {
	list.push_back(record);
}

void Dictionary::remove(int i) {
	list.erase(list.begin() + i);
	list.shrink_to_fit();
}

String Dictionary::get(int i, int j) {
	String result = list[i].first.first;

	if (j == 1) {
		result = list[i].first.second;
	}

	if (j == 2) {
		result = list[i].second;
	}

	return result;
}

bool Dictionary::set(int i, int j, String& str) {
	wstring target(str.t_str());
	wregex regExpressionEngRus(L"[^\\[^\\]]+");
	wregex regExpressionTranscription(L"\\[[^\\[^\\]]+\\]");
	wregex regExpressionBareTranscription(L"[^\\[^\\]]+");

	switch (j) {
		case 0: {
			if (!regex_match(target.cbegin(), target.cend(), regExpressionEngRus)) {
				return false;
			} else {
				list[i].first.first = str;
			}
			break;
		}
		case 1: {
			bool isEmpty = str.get_length() == 1;
			bool isFull = regex_match(target.cbegin(), target.cend(), regExpressionTranscription);
			bool isBare = regex_match(target.cbegin(), target.cend(), regExpressionBareTranscription);

			if (!isEmpty && !isFull && !isBare) {
				return false;
			}

			if (isEmpty) {
				list[i].first.second = str;
			} else if (isFull) {
				list[i].first.second = str;
			} else if (isBare) {
				if (str.get_length() > MAX_PART_LENGTH) {
					return false;
				}

				str = String(L"[") + str;
				str = str + String(L"]");
				list[i].first.second = str;
			}
			break;
		}
		case 2: {
			if (!regex_match(target.cbegin(), target.cend(), regExpressionEngRus)) {
				return false;
			} else {
				list[i].second = str;
			}
			break;
		}
	}

	return true;
}

String Dictionary::dic() {
	return name;
}

int Dictionary::count() {
	return list.size();
}