/*
 * ******************************************************************
 * Created by   Marko Kungla on 2010
 * Updated by   Marko Kungla @ Okram Labs on Aug 6, 2016 - 6:17:26 PM
 * Contact      okramlabs@gmail.com - https://okramlabs.com
 * @copyright   2010 - 2016 Marko Kungla - https://github.com/mkungla
 * @license     MIT
 *
 * Package name    howiphp
 * @category       HOWI3
 * @package        howiphp
 *
 * File         Output.h
 * @link        https://github.com/okramlabs/howiphp
 * ********************************************************************
 * Comments:
 * *******************************************************************/
#ifndef HOWIPHP_OUTPUT_H_
#define HOWIPHP_OUTPUT_H_

#include <iostream>
#include <iomanip>
#include <cstring>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <sys/ioctl.h>
#include <sys/resource.h>
#include "../Config/Config.h"
#include "../ProgressBar/ProgressBar.h"

using namespace std;

namespace howiphp
{
class Output
{

public:

	/// Message string to output
	typedef string OutputMsg;

	Output();

	// Counted errors
	static int error_counter;

	// Counted warning
	static int warn_counter;

	// Totl tasks
	static int task_counter;

	// Is console initiated
	static bool is_initiated;

	ProgressBar progress_object;
	/**
	 * Colors
	 */

	static map<string, string> fg_colors;
	static map<string, string> bg_colors;

	void set_colors()
	{
		/* Setup foreground colors */
		Output::fg_colors["Black"] = "\33[30m";
		Output::fg_colors["Blue"] = "\33[34m";
		Output::fg_colors["Red"] = "\33[31m";
		Output::fg_colors["Magenta"] = "\33[35m";
		Output::fg_colors["Green"] = "\33[32m";
		Output::fg_colors["Cyan"] = "\33[36m";
		Output::fg_colors["Yellow"] = "\33[2;93m";
		Output::fg_colors["White"] = "\33[37m";
		/* Setup background colors */
		Output::bg_colors["Black"] = "\33[40m";
		Output::bg_colors["Blue"] = "\33[44m";
		Output::bg_colors["Red"] = "\33[41m";
		Output::bg_colors["Magenta"] = "\33[45m";
		Output::bg_colors["Green"] = "\33[42m";
		Output::bg_colors["Cyan"] = "\33[46m";
		Output::bg_colors["Yellow"] = "\33[103m";
		Output::bg_colors["White"] = "\33[47m";
	}
	/*
	 * Single line output format
	 *
	 * console_size
	 * unsigned short ws_row;	rows, in characters
	 * unsigned short ws_col;	columns, in characters
	 * unsigned short ws_xpixel;	horizontal size, pixels
	 * unsigned short ws_ypixel;	vertical size, pixels
	 */
	static struct winsize console_size;
	// Size of the left block
	static unsigned int left_block_col;
	// Size of the right bock
	static unsigned int right_block_col;
	// Size of the mid bock
	static unsigned int mid_block_col;
	// String lengght in mid block before cut of
	static unsigned int mid_block_strlen;

	/* Setup console formats
	 *
	 * Will set various output options
	 * */
	void init(int);

	// Reset all counters to 0
	void resetCounters();

	// Increment task counter
	void task();

	// set run progress
	void progressbar(int = 0);
	// show progresspar
	void show_progress(OutputMsg);

	// Output info message
	void info(OutputMsg, bool = false);

	// Output OK message
	void ok(OutputMsg, bool = false);

	// Output warning message
	void warning(OutputMsg, bool = false);

	// Output error message
	void error(OutputMsg, bool = false);

	// Print banners
	void banner(OutputMsg);

	// Print question
	void question(OutputMsg);

	// Version banner
	void version();

	/* Display help menu*/
	void help(int);

	// Help menu rows
	void help_row(OutputMsg, OutputMsg, OutputMsg, OutputMsg);

	// Table row
	void table_row(int, OutputMsg, OutputMsg, OutputMsg = "");

	// List php versons
	void listVersions(OutputMsg, OutputMsg, bool, OutputMsg = "");

	void rm_nl(OutputMsg&);
	// clean exit
	void close();

	virtual ~Output();
};
}

#endif /* HOWIPHP_OUTPUT_H_ */
