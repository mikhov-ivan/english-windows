#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include "BaseWindow.hpp"
#include "Dictionary.hpp"
#include "TestInfo.hpp"
#include "Style.hpp"

class MainWindow : public BaseWindow< MainWindow > {
public:
	PCWSTR ClassName() const;
	void Init(HWND hWnd);																			// Deserialise -> InitConrols -> Update Controls -> etc.
	void InitControls(HWND hWnd);																	// Initialisation for all controls and their message handlers
	void UpdateControls(int type = 0);																// Refills list views (type: 0 - all; 1 - words; 2 - dictionaries)
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);									// Handles Main window events

	int GetHeight();																				// Returns Main window height
	int GetWidth();																					// Returns Main window width

	static int Announce(int id, int index_1, int index_2);											// Shows announcements (may include reference to dictionary and row int it)
	static void AppendText(const HWND& hWnd, wchar_t* newText, bool toEnd = false);					// Appends newText to text from hWnd control
	static LRESULT CALLBACK DefEditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);		// Custom events handler for edit controls (e.g. "Add Word" control)
	static LRESULT CALLBACK DefListProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);		// Custom events handler for list controls (e.g. "Words" control)

	static THREE MakeRecord(wchar_t* input, int len);												// Creates a record in format "word; transcription translate" (first element's zero length tells about error)
	static pair< int, int > CheckWordExistence();													// Checks if word is already exists at leat in one dictionary

	static bool TranscriptionSymbol(HWND hWnd, WPARAM wParam, bool type = 0);						// Handles transcription key press

	static HWND hWndEditLabel;
	static TestInfo testInfo;																		// Contains complete information about current test state
	static String answer;																			// Contains current answer
	static pair< pair< int, int >, String > editInfo;												// Contains information about label edition pair< < raw, col >, < text > >

	static int currentDic;																			// Index of current dictionary
	static vector< Dictionary > dictionaries;														// All existing dictionaries

	// Handlers for all existing fonts
	static HFONT hFont15, hFont20;

private:
	void NewAction();																				// Designed to do background tasks every 'n' actions (e.g. saving all user data)
	void HandleClose(HWND hWnd);																	// Handles Main window "Close" event
	void HandleResize();																			// Handles Main window "Resize" event
	void HandlePaint(HWND hWnd);
	void UpdateLabels();																			// Redraw Labels
	void DrawButtons(LPDRAWITEMSTRUCT lParam);														// Designed to draw all custom buttons
	void DrawBitmap(HDC hDC, int x, int y, HBITMAP hBitmap);										// Helper for drawing bitmaps

	void PrintNewQuestion();
	bool TestCheck();																				// Checks if test is running
	void StartTest();																				// Does all the stuff needed when test starts (e.g. displays the first word-question)
	void StopTest();																				// Does all the stuff needed when test stops (e.g. displays results)
	void TestStep(int action = 0);																	// Handles all user's actions (0 - just a try)
	void TestMistake();																				// Does the stuff on case of error
	void TestMistakeStep();

	void CreateDefaultDictionary();																	// Creates new dictionary with default name
	void SerializeUserData();																		// Serializes all user data
	void DeserializeUserData();																		// Deserializes saved user data

	// Identifiers for all controls
	enum { 
		ID_BTN_DELETEWORD = 10869, ID_BTN_SELECTALLWORDS,
		ID_BTN_NEWDIC, ID_BTN_DELETEDIC, ID_BTN_SELECTALLDICS,
		ID_BTN_STARTTEST, ID_BTN_STOPTEST, ID_BTN_CLEARTEST,
		ID_EDIT_ADD_WORD, ID_LIST_WORDS, ID_LIST_DICS, ID_EDIT_TEST, ID_EDIT_ANSWER, ID_EDIT_LABEL, ID_EDIT_DIC_LABEL
	};

	// Notification codes
	enum {
		N_ADD_WORD_ENTER = 11850, N_NO_DICS, N_ANSWER, N_EDIT_LABEL,
		N_WORD_MALFORMED_OK, N_WORD_EXISTS_OK, N_WORD_EXISTS_CANCEL
	};

	// Announcement codes
	enum {
		A_TEST_IS_RUNNING = 12850, A_TEST_IS_NOT_RUNNING, A_TEST_NO_DICS, A_WORD_MALFORMED, A_LABEL_MALFORMED, A_WORD_EXISTS
	};

	// Resize types
	enum {
		R_TEST_STARTED = 13850
	};
	
	// Handlers for all buttons
	HWND hWndBtnAE, hWndBtnA, hWndBtnO, hWndBtnU, hWndBtnER, hWndBtnE, hWndBtnI, hWndBtnV, hWndBtnTHO, hWndBtnTH, hWndBtnSH, hWndBtnDJ, hWndBtnCH, hWndBtnJ, hWndBtnN;
	HWND hWndBtnDeleteWord, hWndBtnSelectAllWords;
	HWND hWndBtnNewDic, hWndBtnDeleteDic, hWndBtnSelectAllDics;
	HWND hWndBtnStartTest, hWndBtnStopTest, hWndBtnClearTest;

	// Handlers for all edit controls
	HWND hWndAddWord, hWndTest, hWndAnswer;

	// Handlers for all list views
	HWND hWndWords, hWndDics;

	LV_ITEM lviWords, lviDics;

	long long actionCount;																			// Mentioned 'n' - number of actions till background processes will be executed
	Style S;
};

#endif