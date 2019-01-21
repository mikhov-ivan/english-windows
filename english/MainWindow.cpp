#include "Style.hpp"
#include "Globals.hpp"
#include "MainWindow.hpp"

#include <regex>
#include <locale>
#include <codecvt>
#include <fstream>
#include <stdlib.h>
#include <time.h>

WNDPROC wndDefEditProc, wndDefListProc;

HWND MainWindow::hWndEditLabel;
TestInfo MainWindow::testInfo;
String MainWindow::answer;
pair< pair< int, int >, String > MainWindow::editInfo;

int MainWindow::currentDic;
vector< Dictionary > MainWindow::dictionaries;

HFONT MainWindow::hFont15, MainWindow::hFont20;

PCWSTR MainWindow::ClassName() const {
	return L"MainWindow";
}

void MainWindow::Init(HWND hWnd) {
	srand((unsigned int) time(0));
	S.Update(GetWidth(), GetHeight());

	actionCount = 0;
	DeserializeUserData();
	InitControls(hWnd);
	UpdateControls();
}

void MainWindow::InitControls(HWND hWnd) {
	hFont15 = CreateFont(15, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Courier New");
	hFont20 = CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Courier New");

	hWndAddWord = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, S.WORD_MARGIN_LEFT, S.WORD_MARGIN_TOP, S.WORD_WIDTH, S.WORD_HEIGHT, hWnd, (HMENU) ID_EDIT_ADD_WORD, GetModuleHandle(NULL), NULL);
	SendMessage(hWndAddWord, WM_SETFONT, (WPARAM) hFont20, TRUE);
	SendMessage(hWndAddWord, EM_SETLIMITTEXT, (WPARAM) MAX_WORD_LENGTH - 1, 0);
	wndDefEditProc = (WNDPROC) SetWindowLongPtr(hWndAddWord, GWLP_WNDPROC, (LONG_PTR) DefEditProc);

	INITCOMMONCONTROLSEX icexWords;
	icexWords.dwICC = ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&icexWords);

	hWndWords = CreateWindowExW(WS_EX_CLIENTEDGE | LVS_EX_FULLROWSELECT, WC_LISTVIEWW, L"", WS_VISIBLE | WS_CHILD | LVS_REPORT | LVS_EDITLABELS | LVS_SINGLESEL, S. WORDS_MARGIN_LEFT, S.WORDS_MARGIN_TOP, S.WORDS_WIDTH, S.WORDS_HEIGHT, hWnd, (HMENU) ID_LIST_WORDS, GetModuleHandle(NULL), NULL);
	ListView_SetExtendedListViewStyleEx(hWndWords, 0, LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);

	LVCOLUMN lvcWords;
	lvcWords.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvcWords.iSubItem = 0;
	lvcWords.pszText = L"Word in English";
	lvcWords.cx = 230;
	lvcWords.fmt = LVCFMT_LEFT;

	ListView_InsertColumn(hWndWords, 0, &lvcWords);

	lvcWords.pszText = L"Transcription";
	ListView_InsertColumn(hWndWords, 1, &lvcWords);

	lvcWords.pszText = L"Translate";
	ListView_InsertColumn(hWndWords, 2, &lvcWords);

	lviWords.mask = LVIF_TEXT | LVIF_PARAM | LVIF_STATE;
	lviWords.state = 0;
	lviWords.stateMask = 0;

	SendMessage(hWndWords, WM_SETFONT, (WPARAM) hFont15, TRUE);
	wndDefListProc = (WNDPROC) SetWindowLongPtr(hWndWords, GWLP_WNDPROC, (LONG_PTR) DefListProc);

	hWndBtnDeleteWord		= CreateWindowEx(NULL, L"Button", L"", BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, S.WDBTN_MARGIN_LEFT,	S.WDBTN_MARGIN_TOP,		S.WDBTN_WIDTH,	S.WDBTN_HEIGHT, hWnd, (HMENU) ID_BTN_DELETEWORD, GetModuleHandle(NULL), NULL);
	hWndBtnSelectAllWords	= CreateWindowEx(NULL, L"Button", L"", BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, S.WSABTN_MARGIN_LEFT,	S.WSABTN_MARGIN_TOP,	S.WSABTN_WIDTH,	S.WSABTN_HEIGHT, hWnd, (HMENU) ID_BTN_SELECTALLWORDS, GetModuleHandle(NULL), NULL);

	INITCOMMONCONTROLSEX icexDics;
	icexDics.dwICC = ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&icexDics);

	hWndDics = CreateWindowExW(WS_EX_CLIENTEDGE | LVS_EX_FULLROWSELECT, WC_LISTVIEWW, L"", WS_VISIBLE | WS_CHILD | LVS_REPORT | LVS_EDITLABELS | LVS_SINGLESEL, S.DICS_MARGIN_LEFT, S.DICS_MARGIN_TOP, S.DICS_WIDTH, S.DICS_HEIGHT, hWnd, (HMENU) ID_LIST_DICS, GetModuleHandle(NULL), NULL);
	ListView_SetExtendedListViewStyleEx(hWndDics, 0, LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);

	LVCOLUMN lvcDics;
	lvcDics.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvcDics.iSubItem = 0;
	lvcDics.pszText = L"Dictionaries";
	lvcDics.cx = 225;
	lvcDics.fmt = LVCFMT_LEFT;

	ListView_InsertColumn(hWndDics, 0, &lvcDics);

	lviDics.mask = LVIF_TEXT | LVIF_PARAM | LVIF_STATE;
	lviDics.state = 0;
	lviDics.stateMask = 0;

	SendMessage(hWndDics, WM_SETFONT, (WPARAM) hFont15, TRUE);
	SetWindowLongPtr(hWndDics, GWLP_WNDPROC, (LONG_PTR) DefListProc);

	hWndBtnNewDic =			CreateWindowEx(NULL, L"Button", L"", BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, S.DNBTN_MARGIN_LEFT,		S.DNBTN_MARGIN_TOP,		S.DNBTN_WIDTH,	S.DNBTN_HEIGHT, hWnd, (HMENU) ID_BTN_NEWDIC, GetModuleHandle(NULL), NULL);
	hWndBtnDeleteDic =		CreateWindowEx(NULL, L"Button", L"", BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, S.DDBTN_MARGIN_LEFT,		S.DDBTN_MARGIN_TOP,		S.DDBTN_WIDTH,	S.DDBTN_HEIGHT, hWnd, (HMENU) ID_BTN_DELETEDIC, GetModuleHandle(NULL), NULL);
	hWndBtnSelectAllDics =	CreateWindowEx(NULL, L"Button", L"", BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, S.DSABTN_MARGIN_LEFT,	S.DSABTN_MARGIN_TOP,	S.DSABTN_WIDTH,	S.DSABTN_HEIGHT, hWnd, (HMENU) ID_BTN_SELECTALLDICS, GetModuleHandle(NULL), NULL);

	DWORD testWndStyle = WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_READONLY;
	hWndTest = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", testWndStyle, S.TEST_MARGIN_LEFT, S.TEST_MARGIN_TOP, S.TEST_WIDTH, S.TEST_HEIGHT, hWnd, (HMENU) ID_EDIT_TEST, GetModuleHandle(NULL), NULL);
	SendMessage(hWndTest, WM_SETFONT, (WPARAM) hFont15, TRUE);
	SetWindowLongPtr(hWndTest, GWLP_WNDPROC, (LONG_PTR) DefEditProc);
	AppendText(hWndTest, L"This is a simple test system.\r\nSelect at least one dictionary and test your knowledge.\r\n\r\n", true);

	hWndAnswer = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, S.ANSWER_MARGIN_LEFT, S.ANSWER_MARGIN_TOP, S.ANSWER_WIDTH, S.ANSWER_HEIGHT, hWnd, (HMENU) ID_EDIT_ANSWER, GetModuleHandle(NULL), NULL);
	SendMessage(hWndAnswer, WM_SETFONT, (WPARAM) hFont20, TRUE);
	SendMessage(hWndAnswer, EM_SETLIMITTEXT, (WPARAM) MAX_PART_LENGTH - 1, 0);
	SetWindowLongPtr(hWndAnswer, GWLP_WNDPROC, (LONG_PTR) DefEditProc);

	hWndBtnStartTest = CreateWindowEx(NULL, L"Button", L"", BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,	S.TSBTN_MARGIN_LEFT, S.TSBTN_MARGIN_TOP, S.TSBTN_WIDTH, S.TSBTN_HEIGHT, hWnd, (HMENU) ID_BTN_STARTTEST, GetModuleHandle(NULL), NULL);
	hWndBtnStopTest = CreateWindowEx(NULL, L"Button", L"", BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,	S.TEBTN_MARGIN_LEFT, S.TEBTN_MARGIN_TOP, S.TEBTN_WIDTH, S.TEBTN_HEIGHT, hWnd, (HMENU) ID_BTN_STOPTEST, GetModuleHandle(NULL), NULL);
	hWndBtnClearTest = CreateWindowEx(NULL, L"Button", L"", BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,	S.TCBTN_MARGIN_LEFT, S.TCBTN_MARGIN_TOP, S.TCBTN_WIDTH, S.TCBTN_HEIGHT, hWnd, (HMENU) ID_BTN_CLEARTEST, GetModuleHandle(NULL), NULL);

	SetFocus(hWndAddWord);
}

void MainWindow::UpdateControls(int type) {
	if (!dictionaries.size()) {
		CreateDefaultDictionary();
		currentDic = 0;
	}

	if (!type || type == 1) {
		ListView_DeleteAllItems(hWndWords);
		for (int i = 0; i < dictionaries[currentDic].count(); i++) {
			lviWords.iItem = i;
			lviWords.iSubItem = 0;
			lviWords.pszText = dictionaries[currentDic].get(i, 0).t_str();
			lviWords.cchTextMax = MAX_PART_LENGTH;
			lviWords.lParam = (LPARAM) dictionaries[currentDic].get(i, 0).t_str();
			ListView_InsertItem(hWndWords, &lviWords);

			ListView_SetItemText(hWndWords, i, 1, dictionaries[currentDic].get(i, 1).t_str());
			ListView_SetItemText(hWndWords, i, 2, dictionaries[currentDic].get(i, 2).t_str());
		}
	}

	if (!type || type == 2) {
		int n = ListView_GetItemCount(hWndDics);

		vector< BOOL > checked(n, false);
		for (int i = 0; i < n; i++) {
			checked[i] = ListView_GetCheckState(hWndDics, i);
		}

		ListView_DeleteAllItems(hWndDics);
		for (unsigned int i = 0; i < dictionaries.size(); i++) {
			lviDics.iItem = i;
			lviDics.iSubItem = 0;
			lviDics.pszText = dictionaries[i].dic().t_str();
			lviDics.cchTextMax = MAX_PART_LENGTH;
			lviDics.lParam = (LPARAM) &dictionaries[i];
			ListView_InsertItem(hWndDics, &lviDics);

			if (checked.size() > i && checked[i]) {
				UINT stateMask = LVIS_STATEIMAGEMASK;
				UINT state = INDEXTOSTATEIMAGEMASK(2);
				ListView_SetItemState(hWndDics, i, state, stateMask);
			}
		}
	}
}

int MainWindow::Announce(int id, int index_1 = 0, int index_2 = 0) {
	UINT type = MB_OK | MB_ICONINFORMATION | MB_TASKMODAL;

	int result = 0;
	switch (id) {
		case A_TEST_IS_RUNNING:
			result = MessageBoxW(NULL, L"    Test is running\n\n    Please, stop your test to perform this action", L"Runnung test", type);
			break;
		case A_TEST_IS_NOT_RUNNING:
			result = MessageBoxW(NULL, L"    There are no active tests.\n    Please, run the test to perform this action", L"No running tests", type);
			break;
		case A_TEST_NO_DICS:
			result = MessageBox(NULL, L"    There are no words for test\n    Please, select at least one non-empty dictionary", L"Empty test dictionary", type);
			break;
		case A_WORD_MALFORMED:
			result = MessageBox(NULL, L"    Please check your input values\n    Use following samples:  example [ɪgˈzɑːmpl] - пример\n                                              example - пример\n\n    Max length of each part is 40 symbols", L"Input format", type);
			break;
		case A_LABEL_MALFORMED:
			result = MessageBox(NULL, L"    Please check your input values\n    Make it similar to input example\n\n    Max length of input string is 40 symbols", L"Input format", type);
			break;
		case A_WORD_EXISTS:
			result = MessageBox(NULL, L"    Please check your input values\n    Make it similar to input example\n\n    Max length of input string is 40 symbols", L"Input format", type);
			break;
	}

	return result;
}

void MainWindow::AppendText(const HWND& hWnd, wchar_t* newText, bool toEnd) {
	int start, stop;
	if (!toEnd) {
		SendMessage(hWnd, EM_GETSEL, (WPARAM) &start, (LPARAM) &stop);
		SendMessage(hWnd, EM_SETSEL, (WPARAM) start, (LPARAM) stop);
		SendMessage(hWnd, EM_REPLACESEL, 0, (LPARAM) ((LPSTR) newText));
	} else {
		start = stop = GetWindowTextLength(hWnd);
		SendMessage(hWnd, EM_SETSEL, (WPARAM) start, (LPARAM) stop);
		SendMessage(hWnd, EM_REPLACESEL, 0, (LPARAM) ((LPSTR) newText));
	}
}

LRESULT CALLBACK MainWindow::DefEditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (GetDlgCtrlID(hWnd)) {
		case ID_EDIT_ADD_WORD: {
			switch (uMsg) {
				case WM_CHAR: {
					if (wParam == 13) {
						if (testInfo.isRunning) { Announce(A_TEST_IS_RUNNING); return 0; }

						NMHDR notification;
						notification.idFrom = GetDlgCtrlID(hWnd);
						notification.hwndFrom = hWnd;

						if (!dictionaries.size()) {
							notification.code = N_NO_DICS;
							SendMessage(GetParent(hWnd), WM_NOTIFY, notification.idFrom, (LPARAM) &notification);
						}

						wchar_t buff[MAX_WORD_LENGTH];
						int len = GetWindowTextLength(hWnd) + 1;
						GetWindowText(hWnd, buff, len);

						pair< int, int > checkExistenceResult = CheckWordExistence();
						if (checkExistenceResult.first != -1) {
							int result = Announce(A_WORD_EXISTS);
							switch (result) {
								case IDOK:
									notification.code = N_WORD_EXISTS_OK;
									SendMessage(GetParent(hWnd), WM_NOTIFY, notification.idFrom, (LPARAM)&notification);
									break;
								case IDCANCEL:
									notification.code = N_WORD_EXISTS_CANCEL;
									SendMessage(GetParent(hWnd), WM_NOTIFY, notification.idFrom, (LPARAM)&notification);
									return 0;
								default:
									break;
							}
						}

						THREE record = MakeRecord(buff, len);
						bool success = record.first.first.get_length() == 0;
						if (success) {
							dictionaries[currentDic].add(record);
							SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM) L"");
						} else {
							int result = Announce(A_WORD_MALFORMED);
							switch (result) {
								case IDOK:
									notification.code = N_WORD_MALFORMED_OK;
									SendMessage(GetParent(hWnd), WM_NOTIFY, notification.idFrom, (LPARAM)&notification);
									break;
								default:
									break;
							}
							return 0;
						}

						notification.code = N_ADD_WORD_ENTER;
						SendMessage(GetParent(hWnd), WM_NOTIFY,	notification.idFrom, (LPARAM) &notification);
						return 0;
					}

					if (TranscriptionSymbol(hWnd, wParam)) return 0;					
					break;
				}
		
				case WM_KEYDOWN: {
					if (TranscriptionSymbol(hWnd, wParam, 1)) return 0;
					break;
				}
			}
			break;
		}
		case ID_EDIT_ANSWER: {
			switch (uMsg) {
				case WM_CHAR: {
					if (wParam == 13) {
						if (!testInfo.isRunning) { Announce(A_TEST_IS_NOT_RUNNING); return 0; }

						NMHDR notification;
						notification.idFrom = GetDlgCtrlID(hWnd);
						notification.hwndFrom = hWnd;

						wchar_t buff[MAX_PART_LENGTH];
						int len = GetWindowTextLength(hWnd) + 1;
						GetWindowText(hWnd, buff, len);

						answer = String(buff, len);
						SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM) L"");

						notification.code = N_ANSWER;
						SendMessage(GetParent(hWnd), WM_NOTIFY,	notification.idFrom, (LPARAM) &notification);
						return 0;
					}
				}
			}
			break;
		}
		case ID_EDIT_DIC_LABEL:
		case ID_EDIT_LABEL: {
			switch (uMsg) {
				case WM_CHAR: {
					if (wParam == 13) {
						NMHDR notification;
						notification.idFrom = GetDlgCtrlID(hWnd);
						notification.hwndFrom = hWnd;

						wchar_t buff[MAX_PART_LENGTH];
						int len = GetWindowTextLength(hWnd) + 1;
						GetWindowText(hWnd, buff, len);

						editInfo.second = String(buff, len);
						SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM) L"");

						notification.code = N_EDIT_LABEL;
						SendMessage(GetParent(GetParent(hWnd)), WM_NOTIFY, notification.idFrom, (LPARAM) &notification);

						SendMessage(hWndEditLabel, WM_KILLFOCUS, 0, 0);
						DestroyWindow(hWndEditLabel);
						return 0;
					}

					if (TranscriptionSymbol(hWnd, wParam)) return 0;
					break;
				}
				case WM_KEYDOWN: {
					if (TranscriptionSymbol(hWnd, wParam, 1)) return 0;
					break;
				}
				case WM_KILLFOCUS: {
					DestroyWindow(hWndEditLabel);
					break;
				}
			}
			break;
		}
	}
	return CallWindowProc(wndDefEditProc, hWnd, uMsg, wParam, lParam);
}

THREE MainWindow::MakeRecord(wchar_t* input, int len) {
	wstring target(input);
	wregex regExpression(L"[^\\[^\\]]+ \\[[^\\[^\\]]+\\] \\- [^\\[^\\]]+");
	wregex regExpressionBareTranscription(L"[^\\[^\\]]+ \\- [^\\[^\\]]+");

	bool isFull = regex_match(target.cbegin(), target.cend(), regExpression);
	bool isBare = regex_match(target.cbegin(), target.cend(), regExpressionBareTranscription);

	if (!isFull && !isBare) {
		return MAKE_THREE(String(), String(), String());
	}

	String input_str(input, len);

	if (isFull) {
		int leftBracket = input_str.indexof('[');
		int rightBracket = input_str.indexof(']');

		if (leftBracket - 1 >= MAX_PART_LENGTH || rightBracket - leftBracket > MAX_PART_LENGTH || len - rightBracket - 3 - 1 > MAX_PART_LENGTH) {
			return MAKE_THREE(String(), String(), String());
		}

		wchar_t word[MAX_PART_LENGTH];
		wchar_t transcription[MAX_PART_LENGTH + 2];
		wchar_t translate[MAX_PART_LENGTH];
		int wordLength = 0, transcriptionLength = 0, translateLength = 0;

		for (int i = 0; i < leftBracket - 1; i++) {
			word[i] = input_str.char_at(i);
			wordLength++;
		}
		word[wordLength] = '\0';
		wordLength++;

		for (int i = leftBracket; i < rightBracket; i++) {
			transcription[i - leftBracket] = input_str.char_at(i);
			transcriptionLength++;
		}
		transcription[transcriptionLength] = ']';
		transcription[transcriptionLength + 1] = '\0';
		transcriptionLength += 2;

		for (int i = rightBracket + 4; i < len; i++) {
			translate[i - rightBracket - 4] = input_str.char_at(i);
			translateLength++;
		}

		return MAKE_THREE(
			String(word, wordLength),
			String(transcription, transcriptionLength),
			String(translate, translateLength)
		);
	}

	if (isBare) {
		int dash = input_str.indexof(L" - ");

		if (dash + 1 > MAX_PART_LENGTH || len - dash - 3 > MAX_PART_LENGTH) {
			return MAKE_THREE(String(), String(), String());
		}

		wchar_t word[MAX_PART_LENGTH];
		wchar_t translate[MAX_PART_LENGTH];
		int wordLength = 0, translateLength = 0;

		for (int i = 0; i < dash; i++) {
			word[i] = input_str.char_at(i);
			wordLength++;
		}
		word[wordLength] = '\0';
		wordLength++;

		for (int i = dash + 3; i < len; i++) {
			translate[i - dash - 3] = input_str.char_at(i);
			translateLength++;
		}

		return MAKE_THREE(
			String(word, wordLength),
			String(L"", 1),
			String(translate, translateLength)
		);
	}

	return MAKE_THREE(String(), String(), String());
}

pair< int, int > MainWindow::CheckWordExistence() {
	return make_pair(-1, -1);
}

bool MainWindow::TranscriptionSymbol(HWND hWnd, WPARAM wParam, bool type) {
	int tmp = GetDlgCtrlID(hWnd);
	if (!(tmp == ID_EDIT_ADD_WORD || (tmp == ID_EDIT_LABEL && editInfo.first.second == 1))) {
		return false;
	}

	if (!type) {
		switch (wParam) {
			case 49: {
				AppendText(hWnd, L"æ");
				return true;
			}
			case 50: {
				AppendText(hWnd, L"ɑ");
				return true;
			}
			case 51: {
				AppendText(hWnd, L"ɔ");
				return true;
			}
			case 52: {
				AppendText(hWnd, L"ʊ");
				return true;
			}
			case 53: {
				AppendText(hWnd, L"ɜ");
				return true;
			}
			case 54: {
				AppendText(hWnd, L"ə");
				return true;
			}
			case 55: {
				AppendText(hWnd, L"ɪ");
				return true;
			}
			case 56: {
				AppendText(hWnd, L"ʌ");
				return true;
			}
		}
	} else {
		switch (wParam) {
			case VK_F1: {
				AppendText(hWnd, L"θ");
				return true;
			}
			case VK_F2: {
				AppendText(hWnd, L"ð");
				return true;
			}
			case VK_F3: {
				AppendText(hWnd, L"ʃ");
				return true;
			}
			case VK_F4: {
				AppendText(hWnd, L"ʒ");
				return true;
			}
			case VK_F5: {
				AppendText(hWnd, L"ʧ");
				return true;
			}
			case VK_F6: {
				AppendText(hWnd, L"ʤ");
				return true;
			}
			case VK_F7: {
				AppendText(hWnd, L"ŋ");
				return true;
			}
		}
	}

	return false;
}

LRESULT CALLBACK MainWindow::DefListProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	long editID = 0;
	switch (GetDlgCtrlID(hWnd)) {
		case ID_LIST_DICS:
			editID = ID_EDIT_DIC_LABEL;
		case ID_LIST_WORDS: {
			if (!editID) editID = ID_EDIT_LABEL;
			switch (uMsg) {
				case WM_LBUTTONDOWN: {
					SendMessage(hWndEditLabel, WM_KILLFOCUS, 0, 0);
					DestroyWindow(hWndEditLabel);

					LVHITTESTINFO item;
					item.pt.x = (long) LOWORD(lParam);
					item.pt.y = (long) HIWORD(lParam);

					if (ListView_SubItemHitTest(hWnd, &item) != -1 && (item.pt.x > 16 || item.pt.x < 4)) {
						const int LIMIT = (item.iSubItem == 1) ? MAX_PART_LENGTH + 2 : MAX_PART_LENGTH;

						wchar_t buff[MAX_PART_LENGTH + 2];
						LV_ITEM itemInfo;
						itemInfo.iItem = item.iItem;
						itemInfo.iSubItem = item.iSubItem;
						itemInfo.mask = LVIF_TEXT;
						itemInfo.cchTextMax = LIMIT;
						itemInfo.pszText = buff;
						ListView_GetItem(hWnd, &itemInfo);
						String itemText(buff, LIMIT);
				
						if (ListView_GetNextItem(hWnd, -1, LVNI_SELECTED) == item.iItem) {
							RECT rect;
							ListView_GetSubItemRect(hWnd, item.iItem, item.iSubItem, LVIR_BOUNDS, &rect);
							if (!item.iSubItem) {
								rect.left += 22;
								if (editID != ID_EDIT_DIC_LABEL) {
									rect.right -= 42;
								}
							}

							int width = rect.right - rect.left;
							int height = rect.bottom - rect.top;
							if (!item.iSubItem && editID != ID_EDIT_DIC_LABEL) {
								width /= 3;
							}
	
							editInfo.first = make_pair(item.iItem, item.iSubItem);

							hWndEditLabel = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, rect.left, rect.top, width, height, hWnd, (HMENU) editID, GetModuleHandle(NULL), NULL);
							SendMessage(hWndEditLabel, EM_SETLIMITTEXT, (WPARAM) LIMIT - 1, 0);
							SendMessage(hWndEditLabel, WM_SETTEXT, 0, (LPARAM) itemText.t_str());
							SendMessage(hWndEditLabel, EM_SETSEL, (WPARAM) itemText.get_length(), (LPARAM) itemText.get_length());
							SendMessage(hWndEditLabel, WM_SETFONT, (WPARAM) hFont15, TRUE);
							SetWindowLongPtr(hWndEditLabel, GWLP_WNDPROC, (LONG_PTR) DefEditProc);
							SetFocus(hWndEditLabel);

							return 0;
						} else {
							return CallWindowProc(wndDefListProc, hWnd, uMsg, wParam, lParam);
						}
					}
				}
			}
			break;
		}
	}
	return CallWindowProc(wndDefListProc, hWnd, uMsg, wParam, lParam);
}

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_GETMINMAXINFO: {
			MINMAXINFO* minMaxInfo = (MINMAXINFO*) lParam;
			minMaxInfo->ptMinTrackSize.x = MIN_WINDOW_WIDTH;
			minMaxInfo->ptMinTrackSize.y = MIN_WINDOW_HEIGHT;
			break;
		}

		case WM_CLOSE: {
			this->HandleClose(m_hwnd);
			break;
		}

		case WM_DESTROY: {
			SetWindowLong(hWndAddWord, GWL_WNDPROC, (LONG) wndDefEditProc);
			PostQuitMessage(0);
			break;
		}

		case WM_SIZE: {
			this->HandleResize();
			break;
		}

		case WM_DRAWITEM: {
			this->DrawButtons(reinterpret_cast<LPDRAWITEMSTRUCT> (lParam));
			break;
		}

		case WM_PAINT: {
			this->HandlePaint(m_hwnd);
			break;
		}

		case WM_NOTIFY: {
			switch (LOWORD(wParam)) {
				case ID_LIST_DICS: {		
					LPNMLISTVIEW pnmv = reinterpret_cast<LPNMLISTVIEW> (lParam);
					if (pnmv->uChanged & LVIF_STATE) {
						switch (pnmv->uNewState & 0x3000) {
						case INDEXTOSTATEIMAGEMASK(2):
						case INDEXTOSTATEIMAGEMASK(1):
							if (!MAGIC_FLAG_FOR_DOUBLE_NOTIFICATION && testInfo.isRunning) { 
								Announce(A_TEST_IS_RUNNING);
								MAGIC_FLAG_FOR_DOUBLE_NOTIFICATION = 1;
								return 0; 
							}
							MAGIC_FLAG_FOR_DOUBLE_NOTIFICATION = 0;
							return 0;
						}
					}

					switch ((reinterpret_cast<LPNMHDR> (lParam))->code) {
						case NM_DBLCLK: {
							if (testInfo.isRunning) { Announce(A_TEST_IS_RUNNING); return 0; }

							UpdateLabels();

							int index = SendMessage(hWndDics, LVM_GETNEXTITEM, -1, LVNI_FOCUSED);

							if (index == -1) {
								return 0;
							}

							currentDic = index;
							UpdateControls();
							return 0;
						}

						case LVN_BEGINLABELEDIT: {
							return 0;
						}
					}
					break;
				}
				case ID_LIST_WORDS: {
					switch ((reinterpret_cast<LPNMHDR> (lParam))->code) {
						case LVN_BEGINLABELEDIT: {
							return 0;
						}
					}
				}
			}

			switch ((reinterpret_cast<LPNMHDR> (lParam))->code) {
				case N_ADD_WORD_ENTER: {
					if (((LPNMHDR) lParam)->idFrom == ID_EDIT_ADD_WORD) {
						int n = ListView_GetItemCount(hWndWords);
						lviWords.iItem = n;
						lviWords.iSubItem = 0;
						lviWords.pszText = dictionaries[currentDic].get(n, 0).t_str();
						lviWords.cchTextMax = MAX_WORD_LENGTH;
						lviWords.lParam = (LPARAM) dictionaries[currentDic].get(n, 0).t_str();
						ListView_InsertItem(hWndWords, &lviWords);

						ListView_SetItemText(hWndWords, n, 1, dictionaries[currentDic].get(n, 1).t_str());
						ListView_SetItemText(hWndWords, n, 2, dictionaries[currentDic].get(n, 2).t_str());

						UpdateLabels();
						NewAction();
					}
					break;
				}
				case N_NO_DICS: {
					CreateDefaultDictionary();
					break;
				}
				case N_ANSWER: {
					if ((reinterpret_cast<LPNMHDR> (lParam))->idFrom == ID_EDIT_ANSWER) {
						TestStep();
					}
					break;
				}
				case N_EDIT_LABEL: {
					if ((reinterpret_cast<LPNMHDR> (lParam))->idFrom == ID_EDIT_LABEL) {
						if (editInfo.second.get_length() == 1 && (editInfo.first.second == 0 || editInfo.first.second == 2)) {
							return 0;
						}

						NMHDR notification;
						notification.idFrom = GetDlgCtrlID(m_hwnd);
						notification.hwndFrom = m_hwnd;

						if (!dictionaries[currentDic].set(editInfo.first.first, editInfo.first.second, editInfo.second)) {
							Announce(A_LABEL_MALFORMED);
							return 0;
						}

						ListView_SetItemText(hWndWords, editInfo.first.first, editInfo.first.second, editInfo.second.t_str());
						NewAction();
					}

					if ((reinterpret_cast<LPNMHDR> (lParam))->idFrom == ID_EDIT_DIC_LABEL) {
						if (dictionaries[editInfo.first.first].rename(editInfo.second)) {
							UpdateControls(2);
							NewAction();
						}
					}
					break;
				}
				case N_WORD_MALFORMED_OK: {
					SetFocus(hWndAddWord);
					break;
				}
				case NM_CUSTOMDRAW: {
					if (LOWORD(wParam) != ID_LIST_DICS) return CDRF_NEWFONT;
					LPNMLVCUSTOMDRAW lplvcd = reinterpret_cast<LPNMLVCUSTOMDRAW> (lParam);
					switch (lplvcd->nmcd.dwDrawStage) {
						case CDDS_PREPAINT: {
							return CDRF_NOTIFYITEMDRAW;
							break;
						}
						case CDDS_ITEMPREPAINT: {
							if ((int) lplvcd->nmcd.dwItemSpec == currentDic) {
								lplvcd->clrText = RGB(150, 0, 0);
							} else {
								lplvcd->clrText = RGB(0, 0, 0);
							}
							return CDRF_DODEFAULT;
						}
						case CDDS_SUBITEM | CDDS_ITEMPREPAINT: {
							return CDRF_DODEFAULT;
						}
					}
				}
			}
		}

		case WM_COMMAND: {
			switch (LOWORD(wParam)) {
				case ID_BTN_DELETEWORD: {
					if (testInfo.isRunning) { Announce(A_TEST_IS_RUNNING); return 0; }

					int n = ListView_GetItemCount(hWndWords);
					if (!n) return 0;

					vector< int > positions;
					for (int i = 0; i < n; i++) {
						if (ListView_GetCheckState(hWndWords, i)) {
							positions.push_back(i);
						}
					}

					for (int i = positions.size() - 1; i >= 0; i--) {
						dictionaries[currentDic].remove(positions[i]);
						ListView_DeleteItem(hWndWords, positions[i]);
					}

					NewAction();
					break;
				}
				case ID_BTN_SELECTALLWORDS: {
					if (testInfo.isRunning) { Announce(A_TEST_IS_RUNNING); return 0; }

					int n = ListView_GetItemCount(hWndWords);
					if (!n) return 0;

					int checked = 0;
					for (int i = 0; i < n; i++) {
						if (ListView_GetCheckState(hWndWords, i)) {
							checked++;
						} else {
							break;
						}
					}

					bool flag = checked != n;
					for (int i = 0; i < n; i++) {
						ListView_SetCheckState(hWndWords, i, flag);
					}
					break;
				}
				case ID_BTN_NEWDIC: {
					if (testInfo.isRunning) { Announce(A_TEST_IS_RUNNING); return 0; }

					CreateDefaultDictionary();
					currentDic = dictionaries.size() - 1;
					UpdateControls();
					NewAction();
					break;
				}
				case ID_BTN_DELETEDIC: {
					if (testInfo.isRunning) { Announce(A_TEST_IS_RUNNING); return 0; }

					int n = ListView_GetItemCount(hWndDics);
					bool currentDeletion = false;
					if (!n) return 0;

					vector< int > positions;
					for (int i = 0; i < n; i++) {
						if (ListView_GetCheckState(hWndDics, i)) {
							if (i == currentDic) {
								currentDeletion = true;
							}
							positions.push_back(i);
						}
					}

					for (int i = positions.size() - 1; i >= 0; i--) {
						dictionaries.erase(dictionaries.begin() + positions[i]);
						dictionaries.shrink_to_fit();
						ListView_DeleteItem(hWndDics, positions[i]);
					}

					if (!dictionaries.size()) {
						CreateDefaultDictionary();
						currentDic = dictionaries.size() - 1;
						UpdateControls();
					} else if (currentDeletion) {
						currentDic = 0;
						UpdateControls();
					}

					NewAction();
					break;
				}
				case ID_BTN_SELECTALLDICS: {
					if (testInfo.isRunning) { Announce(A_TEST_IS_RUNNING); return 0; }

					int n = ListView_GetItemCount(hWndDics);
					if (!n) return 0;

					int checked = 0;
					for (int i = 0; i < n; i++) {
						if (ListView_GetCheckState(hWndDics, i)) {
							checked++;
						} else {
							break;
						}
					}

					bool flag = checked != n;
					for (int i = 0; i < n; i++) {
						ListView_SetCheckState(hWndDics, i, flag);
					}
					break;
				}
				case ID_BTN_STARTTEST: {
					StartTest();
					break;
				}
				case ID_BTN_STOPTEST: {
					if (testInfo.isRunning) AppendText(hWndTest, L"\r\n", true);
					StopTest();
					break;
				}
				case ID_BTN_CLEARTEST: {
					if (testInfo.isRunning) { Announce(A_TEST_IS_RUNNING); return 0; }

					SendMessage(hWndTest, WM_SETTEXT, 0, (LPARAM) L"");
					AppendText(hWndTest, L"This is a simple test system.\r\nSelect at least one dictionary and test your knowlage.\r\n\r\n", true);
					break;
				}
			}
			break;
		}
	}

	return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}

bool MainWindow::TestCheck() {
	if (testInfo.isRunning) {
		return true;
	}
	return false;
}

void MainWindow::StartTest() {
	if (testInfo.isRunning) { 
		return; 
	}

	for (unsigned int i = 0; i < dictionaries.size(); i++) {
		if (ListView_GetCheckState(hWndDics, i) && dictionaries[i].count() > 0) {
			testInfo.dics.push_back(i);
			testInfo.used.push_back(vector< bool >(dictionaries[i].count(), false));
			testInfo.wordsCount += dictionaries[i].count();
		}
	}

	if (!testInfo.dics.size()) {
		Announce(A_TEST_NO_DICS);
		return;
	}

	testInfo.isRunning = true;

	ListView_DeleteAllItems(hWndWords);
	AppendText(hWndTest, L"Your test is started...\r\n", true);

	PrintNewQuestion();
	SetFocus(hWndAnswer);
	HandleResize();
}

void MainWindow::StopTest() {
	if (!testInfo.isRunning) {
		return;
	}

	testInfo.dics.clear();
	testInfo.dics.shrink_to_fit();

	testInfo.used.clear();
	testInfo.used.shrink_to_fit();

	testInfo.usedCount = testInfo.wordsCount = testInfo.mistakeStep = 0;
	testInfo.isRunning = false;

	UpdateControls(1);
	AppendText(hWndTest, L"Your result: ", true);
	AppendText(hWndTest, String(testInfo.rightCount).t_str(), true);
	AppendText(hWndTest, L" of ", true);
	AppendText(hWndTest, String(testInfo.questionCount).t_str(), true);
	AppendText(hWndTest, L"\r\n\r\n", true);
	HandleResize();

	testInfo.questionCount = testInfo.rightCount = 0;
}

void MainWindow::TestStep(int action) {
	if (testInfo.mistakeStep > 0) {
		TestMistakeStep();
		return;
	}

	AppendText(hWndTest, answer.t_str(), true);
	if (answer != dictionaries[testInfo.dics[testInfo.dicNum]].get(testInfo.wordNum, 0)) {
		AppendText(hWndTest, L" [", true);
		AppendText(hWndTest, dictionaries[testInfo.dics[testInfo.dicNum]].get(testInfo.wordNum, 0).t_str(), true);
		AppendText(hWndTest, L"]", true);
		AppendText(hWndTest, L"\r\n", true);
		TestMistake();
		return;
	} else {
		AppendText(hWndTest, L"\r\n", true);
		testInfo.rightCount++;
	}

	PrintNewQuestion();
}

void MainWindow::TestMistake() {
	AppendText(hWndTest, L"  ", true);
	AppendText(hWndTest, dictionaries[testInfo.dics[testInfo.dicNum]].get(testInfo.wordNum, 2).t_str(), true);
	AppendText(hWndTest, L" - ", true);

	testInfo.mistakeStep = MISTAKE_REPEAT_NUMBER;
}

void MainWindow::TestMistakeStep() {
	AppendText(hWndTest, answer.t_str(), true);
	if (answer != dictionaries[testInfo.dics[testInfo.dicNum]].get(testInfo.wordNum, 0)) {
		AppendText(hWndTest, L" [wrong]", true);
		AppendText(hWndTest, L"\r\n", true);
		testInfo.mistakeStep = MISTAKE_REPEAT_NUMBER;
	} else {
		testInfo.mistakeStep--;
		AppendText(hWndTest, L"\r\n", true);
		if (!testInfo.mistakeStep) {
			testInfo.mistakeStep--;
			PrintNewQuestion();
			return;
		}
	}

	AppendText(hWndTest, L"  ", true);
	AppendText(hWndTest, dictionaries[testInfo.dics[testInfo.dicNum]].get(testInfo.wordNum, 2).t_str(), true);
	AppendText(hWndTest, L" - ", true);
}

void MainWindow::PrintNewQuestion() {
	if (testInfo.usedCount == testInfo.wordsCount) {
		StopTest();
		return;
	}

	testInfo.questionCount++;

	testInfo.dicNum = rand() % testInfo.dics.size();
	testInfo.wordNum = rand() % dictionaries[testInfo.dics[testInfo.dicNum]].count();
	while (testInfo.used[testInfo.dicNum][testInfo.wordNum]) {
		testInfo.dicNum = rand() % testInfo.dics.size();																// random dictionary number in testInfo.dics
		testInfo.wordNum = rand() % dictionaries[testInfo.dics[testInfo.dicNum]].count();								// random word number in selected dictionary
	}

	testInfo.used[testInfo.dicNum][testInfo.wordNum] = true;
	testInfo.usedCount++;

	AppendText(hWndTest, L" ", true);
	AppendText(hWndTest, dictionaries[testInfo.dics[testInfo.dicNum]].get(testInfo.wordNum, 2).t_str(), true);
	AppendText(hWndTest, L" - ", true);
}

void MainWindow::HandleResize() {
	S.Update(GetWidth(), GetHeight());

	if (GetWidth() < WIDE_WINDOW_WIDTH) {
		if (!testInfo.isRunning) {
			MoveWindow(hWndWords, S.WORDS_MARGIN_LEFT, S.WORDS_MARGIN_TOP, S.WORDS_WIDTH, S.WORDS_HEIGHT, TRUE);

			MoveWindow(hWndBtnDeleteWord, S.WDBTN_MARGIN_LEFT, S.WDBTN_MARGIN_TOP, S.WDBTN_WIDTH, S.WDBTN_HEIGHT, TRUE);
			MoveWindow(hWndBtnSelectAllWords, S.WSABTN_MARGIN_LEFT, S.WSABTN_MARGIN_TOP, S.WSABTN_WIDTH, S.WSABTN_HEIGHT, TRUE);

			MoveWindow(hWndDics, S.DICS_MARGIN_LEFT, S.DICS_MARGIN_TOP, S.DICS_WIDTH, S.DICS_HEIGHT, TRUE);

			MoveWindow(hWndBtnNewDic, S.DNBTN_MARGIN_LEFT, S.DNBTN_MARGIN_TOP, S.DNBTN_WIDTH, S.DNBTN_HEIGHT, TRUE);
			MoveWindow(hWndBtnDeleteDic, S.DDBTN_MARGIN_LEFT, S.DDBTN_MARGIN_TOP, S.DDBTN_WIDTH, S.DDBTN_HEIGHT, TRUE);
			MoveWindow(hWndBtnSelectAllDics, S.DSABTN_MARGIN_LEFT, S.DSABTN_MARGIN_TOP, S.DSABTN_WIDTH, S.DSABTN_HEIGHT, TRUE);

			MoveWindow(hWndTest, S.TEST_MARGIN_LEFT, S.TEST_MARGIN_TOP, S.TEST_WIDTH, S.TEST_HEIGHT, TRUE);

			MoveWindow(hWndAnswer, S.ANSWER_MARGIN_LEFT, S.ANSWER_MARGIN_TOP, S.ANSWER_WIDTH, S.ANSWER_HEIGHT, TRUE);

			MoveWindow(hWndBtnStartTest, S.TSBTN_MARGIN_LEFT, S.TSBTN_MARGIN_TOP, S.TSBTN_WIDTH, S.TSBTN_HEIGHT, TRUE);
			MoveWindow(hWndBtnStopTest, S.TEBTN_MARGIN_LEFT, S.TEBTN_MARGIN_TOP, S.TEBTN_WIDTH, S.TEBTN_HEIGHT, TRUE);
			MoveWindow(hWndBtnClearTest, S.TCBTN_MARGIN_LEFT, S.TCBTN_MARGIN_TOP, S.TCBTN_WIDTH, S.TCBTN_HEIGHT, TRUE);
		} else {
			MoveWindow(hWndWords, S.WORDS_MARGIN_LEFT, S.WORDS_MARGIN_TOP, S.WORDS_WIDTH, S.TEST_WORDS_HEIGHT, TRUE);

			MoveWindow(hWndBtnDeleteWord, S.WDBTN_MARGIN_LEFT, S.TEST_WDBTN_MARGIN_TOP, S.WDBTN_WIDTH, S.WDBTN_HEIGHT, TRUE);
			MoveWindow(hWndBtnSelectAllWords, S.WSABTN_MARGIN_LEFT, S.TEST_WSABTN_MARGIN_TOP, S.WSABTN_WIDTH, S.WSABTN_HEIGHT, TRUE);

			MoveWindow(hWndDics, S.DICS_MARGIN_LEFT, S.DICS_MARGIN_TOP, S.DICS_WIDTH, S.TEST_DICS_HEIGHT, TRUE);

			MoveWindow(hWndBtnNewDic, S.DNBTN_MARGIN_LEFT, S.TEST_DNBTN_MARGIN_TOP, S.DNBTN_WIDTH, S.DNBTN_HEIGHT, TRUE);
			MoveWindow(hWndBtnDeleteDic, S.DDBTN_MARGIN_LEFT, S.TEST_DDBTN_MARGIN_TOP, S.DDBTN_WIDTH, S.DDBTN_HEIGHT, TRUE);
			MoveWindow(hWndBtnSelectAllDics, S.DSABTN_MARGIN_LEFT, S.TEST_DSABTN_MARGIN_TOP, S.DSABTN_WIDTH, S.DSABTN_HEIGHT, TRUE);

			MoveWindow(hWndTest, S.TEST_MARGIN_LEFT, S.TEST_TEST_MARGIN_TOP, S.TEST_WIDTH, S.TEST_TEST_HEIGHT, TRUE);

			MoveWindow(hWndAnswer, S.ANSWER_MARGIN_LEFT, S.TEST_ANSWER_MARGIN_TOP, S.ANSWER_WIDTH, S.ANSWER_HEIGHT, TRUE);

			MoveWindow(hWndBtnStartTest, S.TSBTN_MARGIN_LEFT, S.TEST_TSBTN_MARGIN_TOP, S.TSBTN_WIDTH, S.TSBTN_HEIGHT, TRUE);
			MoveWindow(hWndBtnStopTest, S.TEBTN_MARGIN_LEFT, S.TEST_TEBTN_MARGIN_TOP, S.TEBTN_WIDTH, S.TEBTN_HEIGHT, TRUE);
			MoveWindow(hWndBtnClearTest, S.TCBTN_MARGIN_LEFT, S.TEST_TCBTN_MARGIN_TOP, S.TCBTN_WIDTH, S.TCBTN_HEIGHT, TRUE);
		}
	} else {
		MoveWindow(hWndWords, S.WORDS_MARGIN_LEFT, S.WORDS_MARGIN_TOP, S.WORDS_WIDTH, S.WIDE_WORDS_HEIGHT, TRUE);

		MoveWindow(hWndBtnDeleteWord, S.WDBTN_MARGIN_LEFT, S.WIDE_WDBTN_MARGIN_TOP, S.WDBTN_WIDTH, S.WDBTN_HEIGHT, TRUE);
		MoveWindow(hWndBtnSelectAllWords, S.WSABTN_MARGIN_LEFT, S.WIDE_WSABTN_MARGIN_TOP, S.WSABTN_WIDTH, S.WSABTN_HEIGHT, TRUE);

		MoveWindow(hWndDics, S.DICS_MARGIN_LEFT, S.DICS_MARGIN_TOP, S.DICS_WIDTH, S.WIDE_DICS_HEIGHT, TRUE);

		MoveWindow(hWndBtnNewDic, S.DNBTN_MARGIN_LEFT, S.WIDE_DNBTN_MARGIN_TOP, S.DNBTN_WIDTH, S.DNBTN_HEIGHT, TRUE);
		MoveWindow(hWndBtnDeleteDic, S.DDBTN_MARGIN_LEFT, S.WIDE_DDBTN_MARGIN_TOP, S.DDBTN_WIDTH, S.DDBTN_HEIGHT, TRUE);
		MoveWindow(hWndBtnSelectAllDics, S.DSABTN_MARGIN_LEFT, S.WIDE_DSABTN_MARGIN_TOP, S.DSABTN_WIDTH, S.DSABTN_HEIGHT, TRUE);

		MoveWindow(hWndTest, S.WIDE_TEST_MARGIN_LEFT, S.WIDE_TEST_MARGIN_TOP, S.WIDE_TEST_WIDTH, S.WIDE_TEST_HEIGHT, TRUE);

		MoveWindow(hWndAnswer, S.WIDE_ANSWER_MARGIN_LEFT, S.WIDE_ANSWER_MARGIN_TOP, S.WIDE_ANSWER_WIDTH, S.WIDE_ANSWER_HEIGHT, TRUE);

		MoveWindow(hWndBtnStartTest, S.WIDE_TSBTN_MARGIN_LEFT, S.WIDE_TSBTN_MARGIN_TOP, S.TSBTN_WIDTH, S.TSBTN_HEIGHT, TRUE);
		MoveWindow(hWndBtnStopTest, S.WIDE_TEBTN_MARGIN_LEFT, S.WIDE_TEBTN_MARGIN_TOP, S.TEBTN_WIDTH, S.TEBTN_HEIGHT, TRUE);
		MoveWindow(hWndBtnClearTest, S.WIDE_TCBTN_MARGIN_LEFT, S.WIDE_TCBTN_MARGIN_TOP, S.TCBTN_WIDTH, S.TCBTN_HEIGHT, TRUE);
	}

	RECT rect;
	GetWindowRect(m_hwnd, &rect);
	rect.top = 0;
	rect.left = 0;
	InvalidateRect(m_hwnd, &rect, TRUE);
}

void MainWindow::HandlePaint(HWND hWnd) {
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);

	RECT rectNum, rectLabel;
	GetClientRect(m_hwnd, &rectNum);
	GetClientRect(m_hwnd, &rectLabel);

	if (GetWidth() < WIDE_WINDOW_WIDTH) {
		if (!testInfo.isRunning) {
			rectLabel.top = S.WORDS_LABEL_MARGIN_TOP;
			rectLabel.left = S.WORDS_LABEL_MARGIN_LEFT;

			rectNum.top = S.WORDS_LABEL_MARGIN_TOP;
			rectNum.left = S.WORDS_LABEL_MARGIN_LEFT + S.WORDS_LABEL_SPACE;
		} else {
			rectLabel.top = S.TEST_WORDS_LABEL_MARGIN_TOP;
			rectLabel.left = S.WORDS_LABEL_MARGIN_LEFT;

			rectNum.top = S.TEST_WORDS_LABEL_MARGIN_TOP;
			rectNum.left = S.WORDS_LABEL_MARGIN_LEFT + S.WORDS_LABEL_SPACE;
		}
	} else {
		rectLabel.top = S.WIDE_WORDS_LABEL_MARGIN_TOP;
		rectLabel.left = S.WORDS_LABEL_MARGIN_LEFT;

		rectNum.top = S.WIDE_WORDS_LABEL_MARGIN_TOP;
		rectNum.left = S.WORDS_LABEL_MARGIN_LEFT + S.WORDS_LABEL_SPACE;
	}

	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, 0x00000000);
	HGDIOBJ defaultObj = SelectObject(hdc, hFont15);

	DrawText(hdc, L"Word count: ", -1, &rectLabel, NULL);
	DrawText(hdc, String(dictionaries[currentDic].count()).t_str(), -1, &rectNum, NULL);

	SelectObject(hdc, defaultObj);
	EndPaint(hWnd, &ps);
}

void MainWindow::UpdateLabels() {
	int type = 0;
	if (GetWidth() <= WIDE_WINDOW_WIDTH) {
		if (testInfo.isRunning) {
			type = 2;
		}
	} else {
		type = 1;
	}

	RECT rect;
	switch (type) {
	case 0:
		rect.top = S.WORDS_LABEL_MARGIN_TOP;
		rect.left = S.WORDS_LABEL_MARGIN_LEFT;
		break;
	case 1:
		rect.top = S.WIDE_WORDS_LABEL_MARGIN_TOP;
		rect.left = S.WORDS_LABEL_MARGIN_LEFT;
		break;
	case 2:
		rect.top = S.TEST_WORDS_LABEL_MARGIN_TOP;
		rect.left = S.WORDS_LABEL_MARGIN_LEFT;
		break;
	}

	rect.bottom = rect.top + S.WDBTN_HEIGHT;
	rect.right = S.DICS_MARGIN_LEFT - 10;

	InvalidateRect(m_hwnd, &rect, TRUE);
}

void MainWindow::HandleClose(HWND hWnd) {
	SerializeUserData();
}

void MainWindow::DrawButtons(LPDRAWITEMSTRUCT lParam) {
	HBITMAP hBitmap;
	int ResourceId;

	if (lParam->CtlType != ODT_BUTTON) {
		return;
	}

	switch (lParam->CtlID) {
		case ID_BTN_DELETEWORD: {
			ResourceId = IDB_BITMAP32;
			if (lParam->itemState & ODS_SELECTED) ResourceId = IDB_BITMAP33;
			break;
		}
		case ID_BTN_SELECTALLWORDS: {
			ResourceId = IDB_BITMAP36;
			if (lParam->itemState & ODS_SELECTED) ResourceId = IDB_BITMAP37;
			break;
		}
		case ID_BTN_NEWDIC: {
			ResourceId = IDB_BITMAP34;
			if (lParam->itemState & ODS_SELECTED) ResourceId = IDB_BITMAP35;
			break;
		}
		case ID_BTN_DELETEDIC: {
			ResourceId = IDB_BITMAP32;
			if (lParam->itemState & ODS_SELECTED) ResourceId = IDB_BITMAP33;
			break;
		}
		case ID_BTN_SELECTALLDICS: {
			ResourceId = IDB_BITMAP36;
			if (lParam->itemState & ODS_SELECTED) ResourceId = IDB_BITMAP37;
			break;
		}
		case ID_BTN_STARTTEST: {
			ResourceId = IDB_BITMAP38;
			if (lParam->itemState & ODS_SELECTED) ResourceId = IDB_BITMAP39;
			break;
		}
		case ID_BTN_STOPTEST: {
			ResourceId = IDB_BITMAP40;
			if (lParam->itemState & ODS_SELECTED) ResourceId = IDB_BITMAP41;
			break;
		}
		case ID_BTN_CLEARTEST: {
			ResourceId = IDB_BITMAP42;
			if (lParam->itemState & ODS_SELECTED) ResourceId = IDB_BITMAP43;
			break;
		}
	}

	hBitmap = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(ResourceId));

	if (!hBitmap) {
		return;
	}

	if (lParam->itemAction & ODA_DRAWENTIRE || lParam->itemAction & ODA_SELECT) {
		DrawBitmap(lParam->hDC, (lParam->rcItem).left, (lParam->rcItem).top, hBitmap);
	}

	DeleteObject(hBitmap);
}

void MainWindow::DrawBitmap(HDC hDC, int x, int y, HBITMAP hBitmap) {
	HBITMAP hOldBitmap;
	HDC hMemDC;
	BITMAP bitmap;
	POINT ptSize, ptOrg;

	hMemDC = CreateCompatibleDC(hDC);
	hOldBitmap = (HBITMAP) SelectObject(hMemDC, hBitmap);

	if (hOldBitmap) {
		SetMapMode(hMemDC, GetMapMode(hDC));
		GetObject(hBitmap, sizeof(BITMAP), (LPSTR) &bitmap);

		ptSize.x = bitmap.bmWidth;
		ptSize.y = bitmap.bmHeight;
		DPtoLP(hDC, &ptSize, 1);

		ptOrg.x = 0;
		ptOrg.y = 0;
		DPtoLP(hMemDC, &ptOrg, 1);

		BitBlt(hDC, x, y, ptSize.x, ptSize.y, hMemDC, ptOrg.x, ptOrg.y, SRCCOPY);
		SelectObject(hMemDC, hOldBitmap);
	}

	DeleteDC(hMemDC);
}

void MainWindow::SerializeUserData() {
	const locale utf8 = locale(locale(), new codecvt_utf8< wchar_t >());
	wofstream output(L"user.dat");
	output.imbue(utf8);

	for (unsigned int i = 0; i < dictionaries.size(); i++) {
		output << dictionaries[i].count() << endl << dictionaries[i].dic().t_str() << endl;
		for (int j = 0; j < dictionaries[i].count(); j++) {
			output << dictionaries[i].get(j, 0).t_str();
			if (dictionaries[i].get(j, 1).get_length() > 1) output << " ";
			output << dictionaries[i].get(j, 1).t_str() << " - " << dictionaries[i].get(j, 2).t_str() << endl;
		}
	}
}

void MainWindow::DeserializeUserData() {
	wifstream input;
	wstring line;

	input.open("user.dat");
	if (!input.is_open()) return;
	input.imbue(locale(input.getloc(), new codecvt_utf8< wchar_t, 0x10ffff, consume_header >()));
	
	int n;
	int index = 0;
	String strLine;
	wchar_t word[MAX_WORD_LENGTH];
	while (input.good()) {
		n = -1;
		input >> n;
		if (n == -1) break;
		getline(input, line);
		getline(input, line);

		dictionaries.push_back(Dictionary(String(line)));

		for (int i = 0; i < n; i++) {
			getline(input, line);
			line += L'\0';
			strLine = String(line);
			for (int j = 0; j < MAX_WORD_LENGTH; j++) {
				word[j] = strLine.char_at(j);
			}

			THREE record = MakeRecord(word, line.length());
			dictionaries[index].add(record);
		}
		index++;
	}

	return;
}

void MainWindow::CreateDefaultDictionary() {
	wchar_t name[MAX_PART_LENGTH] = L"New Dictionary";
	int nameLength = 15;

	dictionaries.push_back(Dictionary(String(name, nameLength)));
	currentDic = 0;

	lviDics.iItem = ListView_GetItemCount(hWndDics);
	lviDics.iSubItem = 0;
	lviDics.pszText = dictionaries[currentDic].dic().t_str();
	lviDics.cchTextMax = MAX_PART_LENGTH;
	lviDics.lParam = (LPARAM) dictionaries[currentDic].dic().t_str();
	ListView_InsertItem(hWndDics, &lviDics);
	
	UpdateLabels();
}

void MainWindow::NewAction() {
	actionCount++;

	if (actionCount % 3 == 0) {
		SerializeUserData();
	}
}

int MainWindow::GetWidth() {
	RECT rect;
	if (GetWindowRect(m_hwnd, &rect)) {
		return rect.right - rect.left;
	}

	return 0;
}

int MainWindow::GetHeight() {
	RECT rect;
	if (GetWindowRect(m_hwnd, &rect)) {
		return rect.bottom - rect.top;
	}

	return 0;
}