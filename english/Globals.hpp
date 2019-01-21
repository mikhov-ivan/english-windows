#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#define MIN_WINDOW_WIDTH 1024
#define MIN_WINDOW_HEIGHT 500

#define DEF_WINDOW_WIDTH 1024
#define DEF_WINDOW_HEIGHT 600

#define WIDE_WINDOW_WIDTH 1324

#define MAX_PART_LENGTH 41				// ENG WORD + \0
#define MAX_WORD_LENGTH 127				// MAX_PART_LENGTH * 3 - 2 + 6 "#####\0 [#####\0] - #####\0"

#define MISTAKE_REPEAT_NUMBER 2

#define THREE pair< pair< String, String >, String >
#define MAKE_THREE(x, y, z) make_pair(make_pair(x, y), z)

static bool MAGIC_FLAG_FOR_DOUBLE_NOTIFICATION = 0;

#endif