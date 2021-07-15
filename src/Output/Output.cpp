/*
 * ******************************************************************
 * Created by   Marko Kungla on 2010
 * Updated by   Marko Kungla on Aug 6, 2016 - 6:17:46 PM
 *
 * @copyright   2010 - 2016 Marko Kungla - https://github.com/mkungla
 * @license     MIT
 *
 * Package name    howiphp
 * @category       HOWI3
 * @package        howiphp
 *
 * File         Output.cpp
 * @link        https://github.com/mkungla/howiphp
 * ********************************************************************
 * Comments:
 * *******************************************************************/
#include "Output.h"

namespace howiphp
{
Output output;

int Output::error_counter = 0;
int Output::task_counter = 0;
int Output::warn_counter = 0;

bool Output::is_initiated = false;

winsize Output::console_size;

unsigned int Output::left_block_col = 0;
unsigned int Output::right_block_col = 0;
unsigned int Output::mid_block_col = 0;
unsigned int Output::mid_block_strlen = 0;

map<string, string> Output::fg_colors;
map<string, string> Output::bg_colors;

extern Config config;

Output::Output()
{

}

void Output::resetCounters()
{
	task_counter = 0;
	error_counter = 0;
	warn_counter = 0;

}
void Output::task()
{
	this->task_counter++;
}
void Output::init(int colored)
{
	if (!is_initiated)
	{
		if (!colored)
			set_colors();

		/* Console */
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &console_size);
		left_block_col = 4;
		right_block_col = 7;
		mid_block_col = console_size.ws_col - left_block_col - right_block_col
				- 7;
		mid_block_strlen = mid_block_col - 4;

		/* Done */
		is_initiated = true;
	}
}

void Output::progressbar(int totaltasks)
{

	if (totaltasks != 0)
	{
		system("setterm -cursor off");
		ProgressBar progress_object;
		progress_object.setUp(totaltasks, mid_block_strlen);
	}
	else
	{
		progress_object.task();
	}

}
void Output::show_progress(OutputMsg outputcolor)
{

	if (progress_object.isValid()
			|| (int) progress_object.getPercentage() == 100)
	{

		if (outputcolor.empty())
			outputcolor = fg_colors["White"];

		string DisplayMessage = "";

		for (int i = 1; i < progress_object.getBarLenght(); i++)
		{
			DisplayMessage += "#";
		}

		cout << fg_colors["White"] << left << "[\033[1m" << setw(left_block_col)
				<< left;
		cout << outputcolor << " >> \033[0m";
		cout << "\033[0m]: " << setw(mid_block_col) << left << DisplayMessage;

		cout << setw(1) << left << "[" << outputcolor;
		cout << setw(right_block_col - 2) << left
				<< (int) progress_object.getPercentage() << " %";
		cout << setw(1) << left << fg_colors["White"] << "\033[0m]";
		if ((int) progress_object.getPercentage() == 100)
		{
			cout << endl;
			progress_object.reset();
			system("setterm -cursor on");
		}
		cout << flush;
	}
}
void Output::info(OutputMsg message, bool skip_counter)
{
	if (progress_object.isValid())
		cout << "\r";

	string DisplayMessage =
			(message.length() < this->mid_block_strlen) ?
					message : message.substr(0, this->mid_block_strlen) + "...";

	cout << this->fg_colors["White"] << left << "[\033[1m"
			<< setw(this->left_block_col) << left;

	if (skip_counter == true)
		cout << this->fg_colors["Cyan"] << " >> \033[0m";
	else
		cout << this->task_counter;

	cout << "\033[0m]: \033[2m" << setw(this->mid_block_col) << left
			<< DisplayMessage;

	cout << setw(1) << left << "\033[0m[" << this->fg_colors["Cyan"];
	;
	cout << left << setw(this->right_block_col) << " INFO";
	cout << setw(1) << left << this->fg_colors["White"] << "\033[0m]";
	cout << endl;
	show_progress(this->fg_colors["Cyan"]);

}

void Output::ok(OutputMsg message, bool skip_counter)
{

	if (progress_object.isValid())
		cout << "\r";
	string DisplayMessage =
			(message.length() < this->mid_block_strlen) ?
					message : message.substr(0, this->mid_block_strlen) + "...";

	cout << this->fg_colors["White"] << left << "[\033[1m"
			<< setw(this->left_block_col) << left;

	if (skip_counter == true)
		cout << this->fg_colors["Green"] << " >> \033[0m";
	else
		cout << this->task_counter;

	cout << "\033[0m]: " << setw(this->mid_block_col) << left << DisplayMessage;

	cout << setw(1) << left << "[" << this->fg_colors["Green"];
	cout << setw(this->right_block_col) << left << "  OK   ";
	cout << setw(1) << left << this->fg_colors["White"] << "\033[0m]" << endl;

	show_progress(this->fg_colors["Green"]);

}

void Output::warning(OutputMsg message, bool skip_counter)
{

	if (progress_object.isValid())
		cout << "\r";
	string DisplayMessage =
			(message.length() < this->mid_block_strlen) ?
					message : message.substr(0, this->mid_block_strlen) + "...";

	cout << this->fg_colors["White"] << left << "[\033[1m"
			<< setw(this->left_block_col) << left;

	if (skip_counter == true)
		cout << this->fg_colors["Yellow"] << " >> \033[0m";
	else
		cout << this->task_counter;

	cout << "\033[0m]: " << setw(this->mid_block_col) << left << DisplayMessage;

	cout << setw(1) << left << "[" << this->fg_colors["Yellow"];
	cout << setw(this->right_block_col) << left << "WARNING";
	cout << setw(1) << left << this->fg_colors["White"] << "\033[0m]" << endl;

	this->warn_counter++;

	show_progress(this->fg_colors["Yellow"]);

}

void Output::error(OutputMsg message, bool skip_counter)
{

	if (progress_object.isValid())
		cout << "\r";
	string DisplayMessage =
			(message.length() < this->mid_block_strlen) ?
					message : message.substr(0, this->mid_block_strlen) + "...";

	cout << this->fg_colors["White"] << left << "[\033[1m"
			<< setw(this->left_block_col) << left;

	if (skip_counter == true)
		cout << this->fg_colors["Red"] << " >> \033[0m";
	else
		cout << this->task_counter;

	cout << "\033[0m]: " << setw(this->mid_block_col) << left << DisplayMessage;

	cout << setw(1) << left << "[" << this->fg_colors["Red"];
	cout << setw(this->right_block_col) << left << " ERROR ";
	cout << setw(1) << left << this->fg_colors["White"] << "]" << endl;

	this->error_counter++;

	show_progress(this->fg_colors["Red"]);

}

void Output::help_row(OutputMsg short_arg, OutputMsg long_arg,
		OutputMsg arguments, OutputMsg description)
{
	cout << this->fg_colors["Yellow"];
	cout << setw(8) << right << short_arg << "  \033[0m " << setw(15) << left
			<< long_arg << setw(15) << left << arguments << left
			<< description + "\n";

}
void Output::table_row(int id, OutputMsg key, OutputMsg val, OutputMsg appendix)
{
	cout << this->fg_colors["Yellow"];
	cout << setw(10) << right << id << "  \033[0m " << setw(30) << left << key
			<< setw(50) << left << val;
	if (!appendix.empty())
		cout << left << appendix;

	cout << endl;
}
void Output::listVersions(OutputMsg versionnr, OutputMsg type, bool installed,
		OutputMsg date)
{
	if (installed)
		cout << this->fg_colors["Green"];
	else
		cout << this->fg_colors["Yellow"];

	cout << "     " << setw(20) << left << versionnr << "  \033[0m " << setw(20)
			<< left << type;

	if (installed)
	{
		cout << this->fg_colors["Green"] << "INSTALLED";
	}
	else
	{
		cout << this->fg_colors["Yellow"] << "NOT INSTALLED";
	}
	cout << "\033[0m";
	if (!date.empty())
	{
		rm_nl(date);
		cout << " - " << date;
	}

	cout << endl;
}
void Output::banner(OutputMsg message)
{

	cout << left << setw(this->left_block_col + 1)
			<< " \033[2m  " + message + "\033[0m" << endl;

}
void Output::question(OutputMsg message)
{
	cout << left << setw(this->left_block_col + 1) << " \033[0m  " + message
			<< endl << "[\033[1myou\033[0m]$ ";
}
void Output::version()
{

	char banner_msg_1[72];
	sprintf(banner_msg_1, "%s v \33[2;93m%s\033[0m\033[2m (%s %s)",
			PACKAGE_NAME, PACKAGE_VERSION, __DATE__, __TIME__);

	char banner_msg_2[72];
	sprintf(banner_msg_2, "Copyright (c) %s %s - License (%s)", HOWIPHP_COPY,
			HOWIPHP_AUTHOR, HOWIPHP_LICENSE);

	char banner_msg_3[72];
	sprintf(banner_msg_3, "%s - %s\n", HOWIPHP_VENDOR, HOWIPHP_VENDOR_SITE);
	output.banner(banner_msg_1);
	output.banner(banner_msg_2);
	output.banner(banner_msg_3);
}
void Output::help(int colored)
{
	output.init(colored);
	version();
	output.banner("\n Run checks on libhowi-php installation:");
	output.help_row(" ", "check", "", "Check system dependencies.");
	output.help_row("-p", "--ping", "",
			"Check mirrors and repositories by pinging them..");
	output.help_row("-d", "--download", "",
			"Determine fastest mirrors. (-p flag is not needed)");

	output.banner("\n Run initial or post configuration:");
	output.help_row(" ", "configure", "", "Run interactive configuration.");

	output.banner("\n Install new php version:");
	output.help_row(" ", "install", "master", "Build PHP from git master");
	output.help_row(" ", "install", "<version>",
			"Build specified php version.");

	output.banner("\n Remove php versions installed with libhowi-php:");
	output.help_row(" ", "remove", "<version>", "Remove specified php version");

	output.banner("\n Switching between installed php versions:");
	output.help_row(" ", "use", "<version>",
			"Switch to php version specified.");

	output.banner("\n Listing php versions:");
	output.help_row(" ", "list", "",
			"List all installed and available php releases and tags!");
	output.help_row("-i", "--installed", "",
			"List only installed php versions");
	output.help_row("-a", "--available", "",
			"List only available not installed php versions");
	output.help_row("-r", "--releases", "",
			"List only available not installed php versions");

	output.banner("\n Optional arguments:");
	output.help_row("-h", "--help", "", "This help");
	output.help_row("-n", "--no-colors", "", "Show non colored output");
	output.help_row("-v", "--version", "", "Version information \n");

}
void Output::close()
{
	string bg_color = this->bg_colors["Black"];
	string fg_color = this->fg_colors["Green"];

	char status_msg[this->mid_block_strlen];
	char time_taken[50];

	if (warn_counter)
	{
		bg_color = this->bg_colors["Yellow"];
		fg_color = this->fg_colors["Black"];
		sprintf(status_msg, "WARNINGS: Tasks (%d), Warnings (%d)", task_counter,
				warn_counter);
	}
	if (error_counter)
	{
		bg_color = this->bg_colors["Red"];
		fg_color = this->fg_colors["White"];
		sprintf(status_msg, "WARNINGS: Tasks (%d), Warnings (%d), Errors (%d)",
				task_counter, warn_counter, error_counter);
	}
	if (!warn_counter && !error_counter)
	{
		sprintf(status_msg, "OK: Tasks (%d)", this->task_counter);
	}

	/* Memory usage */
	char peak_mem_usage[20];
	struct rusage rusage;
	getrusage( RUSAGE_SELF, &rusage);
	sprintf(peak_mem_usage, "%.2fMb",
			(double) (size_t) (rusage.ru_maxrss) / 1024L);
	/* Time taken */
	long elapsed = config.get_elapsed_time();

	sprintf(time_taken, " Elapsed time: %.2f seconds, memory peak: ",
			((float) elapsed) / CLOCKS_PER_SEC);

	string DisplayMessage = bg_color + fg_color + " \033[1m" + status_msg
			+ " \033[0m";

	cout << endl << endl << time_taken;
	cout << peak_mem_usage << endl;
	cout << endl << DisplayMessage << endl << endl;

	if (config.conf_storage.isMember("paths")
			&& config.conf_storage["paths"].isMember("conf_file")
			&& boost::filesystem::exists(
					config.conf_storage["paths"]["conf_file"].asString()))
	{
		time_t now = time(0);
		string rightnow = ctime(&now);

		Config::conf_storage["last-used"] = rightnow;
		/* Update and save any changed configuration */
		boost::filesystem::ofstream file_id;
		file_id.open(Config::conf_storage["paths"]["conf_file"].asString());
		file_id << Config::conf_storage;
		file_id.close();
	}
	exit(0);
}
void Output::rm_nl(OutputMsg &s)
{
	for (int p = s.find("\n"); p != (int) string::npos; p = s.find("\n"))
		s.erase(p, 1);
}
Output::~Output()
{
	fflush(stdout);
}
}
