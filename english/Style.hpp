#ifndef STYLE_HPP
#define STYLE_HPP

typedef unsigned short word;

class Style {
public:
	word DISPLAY_WIDTH;
	word DISPLAY_HEIGHT;

	//############################### WORD'S EDIT BOX ####################################

	word WORD_MARGIN_TOP;
	word WORD_MARGIN_LEFT;
	word WORD_WIDTH;
	word WORD_HEIGHT;

	//################################### WORDS ##########################################

	word WORDS_MARGIN_TOP;
	word WORDS_MARGIN_LEFT;
	word WORDS_MARGIN_BOTTOM;
	word WORDS_WIDTH;
	word WORDS_HEIGHT;

	word WIDE_WORDS_MARGIN_BOTTOM;
	word WIDE_WORDS_HEIGHT;

	word TEST_WORDS_HEIGHT;

	//################################### WORDS BUTTONS ##################################

	word WDBTN_MARGIN_TOP;
	word WDBTN_MARGIN_LEFT;
	word WDBTN_WIDTH;
	word WDBTN_HEIGHT;

	word WSABTN_MARGIN_TOP;
	word WSABTN_MARGIN_LEFT;
	word WSABTN_WIDTH;
	word WSABTN_HEIGHT;

	word WIDE_WDBTN_MARGIN_TOP;
	word WIDE_WSABTN_MARGIN_TOP;

	word TEST_WDBTN_MARGIN_TOP;
	word TEST_WSABTN_MARGIN_TOP;

	//################################### DICS ###########################################

	word DICS_MARGIN_TOP;
	word DICS_MARGIN_LEFT;
	word DICS_MARGIN_BOTTOM;
	word DICS_WIDTH;
	word DICS_HEIGHT;

	word WIDE_DICS_MARGIN_BOTTOM;
	word WIDE_DICS_HEIGHT;

	word TEST_DICS_HEIGHT;

	//################################### DICS BUTTONS ###################################

	word DNBTN_MARGIN_TOP;
	word DNBTN_MARGIN_LEFT;
	word DNBTN_WIDTH;
	word DNBTN_HEIGHT;

	word DDBTN_MARGIN_TOP;
	word DDBTN_MARGIN_LEFT;
	word DDBTN_WIDTH;
	word DDBTN_HEIGHT;

	word DSABTN_MARGIN_TOP;
	word DSABTN_MARGIN_LEFT;
	word DSABTN_WIDTH;
	word DSABTN_HEIGHT;

	word WIDE_DNBTN_MARGIN_TOP;
	word WIDE_DDBTN_MARGIN_TOP;
	word WIDE_DSABTN_MARGIN_TOP;

	word TEST_DNBTN_MARGIN_TOP;
	word TEST_DDBTN_MARGIN_TOP;
	word TEST_DSABTN_MARGIN_TOP;

	//################################### TEST ###########################################

	word TEST_MARGIN_TOP;
	word TEST_MARGIN_LEFT;
	word TEST_WIDTH;
	word TEST_HEIGHT;

	word WIDE_TEST_MARGIN_TOP;
	word WIDE_TEST_MARGIN_LEFT;
	word WIDE_TEST_WIDTH;
	word WIDE_TEST_HEIGHT;

	word TEST_TEST_MARGIN_TOP;
	word TEST_TEST_MARGIN_BOTTOM;
	word TEST_TEST_HEIGHT;

	//################################### ANSWER #########################################

	word ANSWER_MARGIN_TOP;
	word ANSWER_MARGIN_LEFT;
	word ANSWER_WIDTH;
	word ANSWER_HEIGHT;

	word WIDE_ANSWER_MARGIN_TOP;
	word WIDE_ANSWER_MARGIN_LEFT;
	word WIDE_ANSWER_WIDTH;
	word WIDE_ANSWER_HEIGHT;

	word TEST_ANSWER_MARGIN_TOP;

	//################################### TEST BUTTONS ###################################

	word TSBTN_MARGIN_TOP;
	word TSBTN_MARGIN_LEFT;
	word TSBTN_WIDTH;
	word TSBTN_HEIGHT;

	word TEBTN_MARGIN_TOP;
	word TEBTN_MARGIN_LEFT;
	word TEBTN_WIDTH;
	word TEBTN_HEIGHT;

	word TCBTN_MARGIN_TOP;
	word TCBTN_MARGIN_LEFT;
	word TCBTN_WIDTH;
	word TCBTN_HEIGHT;

	word WIDE_TSBTN_MARGIN_TOP;
	word WIDE_TSBTN_MARGIN_LEFT;

	word WIDE_TEBTN_MARGIN_TOP;
	word WIDE_TEBTN_MARGIN_LEFT;

	word WIDE_TCBTN_MARGIN_TOP;
	word WIDE_TCBTN_MARGIN_LEFT;

	word TEST_TSBTN_MARGIN_TOP;
	word TEST_TEBTN_MARGIN_TOP;
	word TEST_TCBTN_MARGIN_TOP;

	//################################### LABELS #########################################

	word WORDS_LABEL_SPACE;
	word WORDS_LABEL_MARGIN_TOP;
	word WORDS_LABEL_MARGIN_LEFT;
	word WIDE_WORDS_LABEL_MARGIN_TOP;
	word TEST_WORDS_LABEL_MARGIN_TOP;

	Style();
	void Update(int w, int h);
};

#endif