/*
 * ******************************************************************
 * Created by   Marko Kungla on 2010
 * Updated by   Marko Kungla on Aug 6, 2016 - 6:01:33 PM
 *
 * @copyright   2010 - 2016 Marko Kungla - https://github.com/mkungla
 * @license     MIT
 *
 * Package name    howiphp
 * @category       HOWI3
 * @package        howiphp
 *
 * File         howiphp.cpp
 * @link        https://github.com/mkungla/howiphp
 * ********************************************************************
 * Comments:
 * *******************************************************************/
#include "howiphp.h"

#include <getopt.h>

#include "Check/Check.h"
#include "List/List.h"
#include "PHPbuilder/PHPbuilder.h"
#include "UseVersion/UseVersion.h"
using namespace howiphp;
using namespace std;

int main(int argc, char *argv[])
{

	int opt_index = 0;
	int options;

	/* Flags */
	int flag_version = 0;
	int flag_colors = 0;
	int flag_help = 0;
	int flag_with_ping = 0;
	int flag_with_download = 0;
	int flag_list_installed = 0;
	int flag_list_available = 0;
	int flag_list_releases = 0;

	static struct option libhowi_php_opts[] =
	{
	{ "version", no_argument, &flag_version, 1 },
	{ "no-colors", no_argument, &flag_colors, 1 },
	{ "help", no_argument, &flag_help, 1 },
	{ "ping", no_argument, &flag_with_ping, 1 },
	{ "download", no_argument, &flag_with_download, 1 },
	{ "installed", no_argument, &flag_list_installed, 1 },
	{ "available", no_argument, &flag_list_available, 1 },
	{ "releases", no_argument, &flag_list_releases, 1 },
	{ 0, 0, 0, 0 } };

	/*
	 * Record startup timestamp
	 * and initialize output
	 * */
	Config::set_startup_time();
	output.resetCounters();
	output.init(flag_colors);

	/**
	 * If User is unable to execute ~/.local/bin as primary
	 * then we can not continue since, while installing PHP
	 * versions and having also system wide PHP version installed
	 * then PATH variable will load /usr/bin first! However we
	 * want that user does not need to run this application as root/sudo
	 * and we also do not want that this application conflicts with
	 * user environment.
	 */
	if (!Check::envpath())
	{
		output.error(
				"Make adjustments as described above! Without correct path you can not continue!");
		output.close();
	}

	/* Set flags for case internal command or no command is provided */
	while (1)
	{
		opt_index = 0;
		options = getopt_long_only(argc, argv, "", libhowi_php_opts,
				&opt_index);
		/* Detect the end of the options. */
		if (options == -1)
			break;
	}

	/* Print any remaining command line arguments (not options). */
	if (optind < argc)
	{

		for (int i = optind; i < argc; i++)
		{
			if (strcmp(argv[i], "check") == 0)
			{
				Check check;
				check.isConfigured(flag_colors);

				check.system();
				check.php_net();
				check.compiler();
				check.optional();

				check.gitrepos(flag_with_ping);
				check.mirrors(flag_with_ping);
				check.fastest_mirrors(flag_with_download);

				check.phpbuild_buildconf();
				check.phpbuild_configure();
				check.rsync_releases();
				check.gittags();

				if (!check.dnf_install.empty())
				{
					output.info(
							"Seem that you mis some dependencies you can install the all using command bellow!",
							true);
					output.banner(check.dnf_install);
				}
				output.close();
				exit(0);
			}

			if (strcmp(argv[i], "configure") == 0)
			{
				config.init(flag_colors, true);
				output.close();
				exit(0);
			}

			if (strcmp(argv[i], "use") == 0)
			{
				config.init(flag_colors);
				UseVersion useversion;
				string version_to_switch =
						(!argv[i + 1]) ? "" : string(argv[i + 1]);
				useversion.load(version_to_switch);
				output.close();
				exit(0);
			}
			if (strcmp(argv[i], "install") == 0)
			{

				config.init(flag_colors);
				output.version();
				PHPbuilder phpbuilder;
				string version_to_install =
						(!argv[i + 1]) ? "" : string(argv[i + 1]);
				phpbuilder.install(version_to_install);
				output.close();
				exit(0);
			}
			if (strcmp(argv[i], "remove") == 0)
			{

				config.init(flag_colors);
				output.version();
				PHPbuilder phpbuilder;
				string version_to_remove =
						(!argv[i + 1]) ? "" : string(argv[i + 1]);
				phpbuilder.remove(version_to_remove);
				output.close();
				exit(0);
			}
			if (strcmp(argv[i], "list") == 0)
			{

				config.init(flag_colors);
				output.version();

				/// If we want to list versions available
				/// then update tags and releases
				if(!flag_list_installed)
				{
					Check check;
					check.rsync_releases();
					check.gittags();
				}

				List list;
				list.listAll(flag_list_installed, flag_list_available,
						flag_list_releases);

				output.close();
				exit(0);
			}
			if (strcmp(argv[i], "empty") == 0)
			{
				config.init(flag_colors);
				output.ok("Single empty command!", true);
				exit(0);
			}
		}
	}
	/* If we got so far then let's check either to
	 * show some of the info based on flag */
	if (flag_version)
	{
		config.init(flag_colors);
		output.version();
	}
	else
	{
		/* We are not going to check flag_help since
		 * any condition we got so far we will show help */
		config.init(flag_colors);
		output.help(flag_colors);
	}
	exit(0);
}
