/*
 * ******************************************************************
 * Created by   Marko Kungla on 2010
 * Updated by   Marko Kungla @ Okram Labs on Aug 6, 2016 - 6:18:46 PM
 * Contact      okramlabs@gmail.com - https://okramlabs.com
 * @copyright   2010 - 2016 Marko Kungla - https://github.com/mkungla
 * @license     MIT
 *
 * Package name    howiphp
 * @category       HOWI3
 * @package        howiphp
 *
 * File         PHPbuilder.cpp
 * @link        https://github.com/okramlabs/howiphp
 * ********************************************************************
 * Comments:
 * *******************************************************************/
#include "PHPbuilder.h"

namespace howiphp
{
path PHPbuilder::installation_path;
path PHPbuilder::build_path;
path PHPbuilder::local_downloaded_file;
string PHPbuilder::current_build_version;
path PHPbuilder::config_file_path;

PHPbuilder::PHPbuilder()
{
	// TODO Auto-generated constructor stub

}
void PHPbuilder::buildconf()
{
	output.task();
	string default_val =
			(config.conf_storage["php"].isMember("buildconf")
					&& !config.conf_storage["php"]["buildconf"].empty()) ?
					config.conf_storage["php"]["buildconf"].asString() :
					"--force";

	output.question(
			"Please give ./buildconf flags [default = " + default_val + "]");
	output.banner("Press ENTER to keep default or previously set value");

	string input;
	cin.ignore();
	getline(cin, input);
	if (!input.empty())
		config.conf_storage["php"]["buildconf"] = input;
	else
		config.conf_storage["php"]["buildconf"] = default_val;

	output.ok("PHP buildconf is configured!");

}
void PHPbuilder::configure()
{
	output.task();
	if (!config.conf_storage["php"]["configure"].empty())
	{
		Json::Value confflags;
		int i = 1;
		output.banner("Current flags of ./configure \\");
		for (Json::ValueConstIterator itr =
				config.conf_storage["php"]["configure"].begin();
				itr != config.conf_storage["php"]["configure"].end(); itr++)
		{
			output.banner((*itr).asString() + " \\");
			i++;
		}
		output.banner(
				"Press Enter to delete current value, enter (y) to keep previously set value or type your new value and press Enter.");
		cin.ignore();
		for (Json::ValueConstIterator itr =
				config.conf_storage["php"]["configure"].begin();
				itr != config.conf_storage["php"]["configure"].end(); itr++)
		{

			string default_val = (*itr).asString();
			output.question("./configure flag [" + default_val + "]");
			string input;
			getline(cin, input);
			if (!input.empty())
			{
				if (input == "y")
					confflags.append(default_val);
				else
					confflags.append(input);
			}
			i++;
		}
		string input;
		do
		{

			output.question(
					"Add more php ./configure flags enter the new value or type (q) and press Enter to continue.");

			getline(cin, input);

			if (!input.empty() && input != "q")
				confflags.append(input);

		} while (input != "q");

		config.conf_storage["php"]["configure"] = confflags;
		output.ok("PHP ./configure flags are configured!");
	}
	else
	{
		output.warning(
				"PHPbuild: All default ./configure flags are gone! File a BUG or reset configuration defaults!");
	}
}
void PHPbuilder::remove(string version_to_remove)
{
	output.task();
	output.info("Removing php installation..");
	current_build_version = version_to_remove;
	/* If User does not provide versions */
	if (version_to_remove.empty())
	{
		output.error("Usage: howiphp remove <version>", true);
		output.info("Check: howiphp --help for more options!", true);
		output.close();
	}
	/* If user suplies the version nr which is not instaled  */
	else if (!config.conf_storage["php"]["versions"].isMember(
			version_to_remove))
	{
		output.warning(
				"PHP version: " + version_to_remove + " is not installed!",
				true);
		output.close();
	}
	/* Lets remove it then */
	else
	{
		/* Remove installation files */
		output.info("Removing installation...", true);
		installation_path = path(
				config.conf_storage["paths"]["lib_php_path"].asString()).append(
				"versions").append(version_to_remove);
		remove_all(installation_path);

		/* Update configuration file */
		output.ok("Cleaning up the configuration", true);
		config.conf_storage["php"]["versions"].removeMember(version_to_remove);

		/* DONE php installation of specified version is removed */
		output.ok("PHP version: " + version_to_remove + " is removed!", true);
		output.close();
	}
}
void PHPbuilder::install(string version_to_install)
{
	output.task();
	output.info("Checking php installation..");
	current_build_version = version_to_install;

	if (version_to_install.empty())
	{
		output.error("Usage: howiphp install <version>", true);
		output.info("Check: howiphp --help for more options!", true);
		output.close();
	}
	else if (config.conf_storage["php"]["versions"].isMember(
			version_to_install))
	{
		output.ok(
				"PHP version: " + version_to_install
						+ " is already installed at "
						+ config.conf_storage["php"]["versions"][version_to_install].asString()
						+ "!", true);
		output.info(
				"You can switch to it by using: howiphp switch "
						+ version_to_install, true);
		output.close();
	}
	else if (config.conf_storage["php"]["releases"].isMember(
			version_to_install))
	{
		output.info("Installing php release " + version_to_install, true);
		install_release(version_to_install);
	}
	else if (config.conf_storage["php"]["tags"].isMember(version_to_install)
			|| version_to_install == "master")
	{
		output.info("Installing php from tag " + version_to_install, true);
		install_tag(version_to_install);
	}
	else
	{
		output.error("No such version available: " + version_to_install, true);
		output.info("Check available versions: howiphp list -a", true);
		output.close();
	}
	output.ok("PHP version: " + version_to_install + " is installed!", true);
}
void PHPbuilder::install_release(string version_to_install)
{
	output.task();
	output.info(
			"Starting installation progress of release " + version_to_install);
	prepare(version_to_install);

	string hostname =
			config.conf_storage["source"]["fastest-mirrors"]["fm1"]["hostname"].asString();
	Json::Value mirror = config.conf_storage["source"]["mirrors"][hostname];
	string download_file =
			config.conf_storage["php"]["releases"][version_to_install].asString();
	local_downloaded_file = path(
			config.conf_storage["paths"]["lib_tmp_path"].asString()).append(
			"php-" + download_file);

	download_archive(hostname, download_file, mirror["proto"].asString(),
			mirror["country"].asString());
	extract_archive();

	path cwd = current_path();
	chdir(build_path.c_str());

	run_buildconf();

	run_configure();

	char type = '\0';
	do
	{
		output.info(
				"If ./configure finished with errors then type (n) and check your dependencies and configuration",
				true);
		output.question("Do you want to run (make) command now? [y/n]");
		cin >> type;
	} while (type != 'y' && type != 'n');

	if (type == 'y' || type == 'Y')
	{
		make();
	}
	if (type == 'n' || type == 'N')
	{
		output.warning("PHP build was canceled!");
		output.close();
	}

	output.info(
			"Running (make test) can take long time! you can skip this step!",
			true);

	type = '\0';
	do
	{
		output.question("Do you want to run (make test)? [y/n]");
		cin >> type;
	} while (type != 'y' && type != 'n');

	if (type == 'y' || type == 'Y')
	{
		make_test();
	}
	if (type == 'n' || type == 'N')
	{
		output.info("Skipping make test!");
	}

	type = '\0';
	do
	{
		output.question("Do you want to run (make install) now? [y/n]");
		cin >> type;
	} while (type != 'y' && type != 'n');

	if (type == 'y' || type == 'Y')
	{
		make_install();
	}
	if (type == 'n' || type == 'N')
	{
		output.warning("PHP build was canceled!");
		output.close();
	}
	chdir(cwd.c_str());

	check_php_ini();

	cleanup();

	time_t now = time(0);
	string rightnow = ctime(&now);

	config.conf_storage["php"]["versions"][current_build_version] = rightnow;
}
void PHPbuilder::install_tag(string version_to_install)
{
	output.task();
	output.info("Starting installation progress of TAG " + version_to_install);
	prepare(version_to_install);

	string download_file;
	string hostname =
			config.conf_storage["source"]["repos"]["Github"]["host"].asString();

	Json::Value Github = config.conf_storage["source"]["repos"]["Github"];

	if (version_to_install == "master")
		download_file = "master.tar.gz";
	else
		download_file = "php-" + version_to_install + ".tar.gz";

	local_downloaded_file = path(
			config.conf_storage["paths"]["lib_tmp_path"].asString()).append(
			download_file);

	download_tag(hostname, download_file, Github["proto"].asString(), "Github");
	extract_archive();

	path cwd = current_path();
	chdir(build_path.c_str());

	run_buildconf();

	run_configure();

	char type = '\0';
	do
	{
		output.info(
				"If ./configure finished with errors then type (n) and check your dependencies and configuration",
				true);
		output.question("Do you want to run (make) command now? [y/n]");
		cin >> type;
	} while (type != 'y' && type != 'n');

	if (type == 'y' || type == 'Y')
	{
		make();
	}
	if (type == 'n' || type == 'N')
	{
		output.warning("PHP build was canceled!");
		output.close();
	}

	output.info(
			"Running (make test) can take long time! you can skip this step!",
			true);

	type = '\0';
	do
	{
		output.question("Do you want to run (make test)? [y/n]");
		cin >> type;
	} while (type != 'y' && type != 'n');

	if (type == 'y' || type == 'Y')
	{
		make_test();
	}
	if (type == 'n' || type == 'N')
	{
		output.info("Skipping make test!");
	}

	type = '\0';
	do
	{
		output.question("Do you want to run (make install) now? [y/n]");
		cin >> type;
	} while (type != 'y' && type != 'n');

	if (type == 'y' || type == 'Y')
	{
		make_install();
	}
	if (type == 'n' || type == 'N')
	{
		output.warning("PHP build was canceled!");
		output.close();
	}
	chdir(cwd.c_str());

	check_php_ini();

	cleanup();

	time_t now = time(0);
	string rightnow = ctime(&now);

	config.conf_storage["php"]["versions"][current_build_version] = rightnow;
}
void PHPbuilder::cleanup()
{
	output.task();
	output.info("Cleaning up...");

	for (directory_iterator end_dir_it, it(build_path); it != end_dir_it; ++it)
	{
		remove_all(it->path());
	}
	if (exists(local_downloaded_file.string()))
		remove_all(local_downloaded_file);

	output.ok("Everything is cleaned up!", true);
}
void PHPbuilder::check_php_ini()
{
	output.task();
	output.info("Checking php.ini files at ~/HOWI3/etc");
	/* TODO Could also check existing php ini file for configuration options  */

	// php.ini files provided by current instalation
	path inst_phpini_production = path(build_path).append("php.ini-production");
	path inst_phpini_development = path(build_path).append(
			"php.ini-development");

	/* paths to the local php.ini files */
	string HOWI_etc = config.conf_storage["paths"]["lib_etc_path"].asString();
	path local_phpini_production = path(HOWI_etc).append("php.ini-production");
	path local_phpini_development = path(HOWI_etc).append(
			"php.ini-development");
	path local_phpini = path(HOWI_etc).append("php.ini");

	bool updated_phpini = false;
	if (!exists(local_phpini_production) && exists(inst_phpini_production))
	{
		copy(inst_phpini_production, local_phpini_production);
		output.ok("Copied php.ini-production to ~/HOWI3/etc/php.ini-production",
				true);
		updated_phpini = true;
	}

	if (!exists(local_phpini_development) && exists(inst_phpini_development))
	{
		copy(inst_phpini_development, local_phpini_development);
		output.ok(
				"Copied php.ini-development to ~/HOWI3/etc/php.ini-development",
				true);
		updated_phpini = true;
	}

	if (!exists(local_phpini) && exists(local_phpini_development))
	{
		copy(local_phpini_development, local_phpini);
		output.ok("Copied php.ini-development to ~/HOWI3/etc/php.ini", true);
		updated_phpini = true;
	}
	if (!updated_phpini)
	{
		output.info("You already have php.ini files at ~/HOWI3/etc", true);
	}
	output.ok("PHP ini files ok check ~/HOWI3/etc to modify", true);
}
void PHPbuilder::prepare(string version_to_install)
{
	output.task();
	output.info("Preparing build destinations...");

	build_path =
			path(config.conf_storage["paths"]["lib_php_path"].asString()).append(
					"build");
	for (directory_iterator end_dir_it, it(build_path); it != end_dir_it; ++it)
	{
		remove_all(it->path());
	}

	installation_path = path(
			config.conf_storage["paths"]["lib_php_path"].asString()).append(
			"versions").append(version_to_install);

	output.ok("Build destination cleaned!", true);

	if (!exists(installation_path))
		boost::filesystem::create_directories(installation_path.string());
	output.ok("Build destinations created!", true);

	if (exists(installation_path))
	{
		for (directory_iterator end_dir_it, it(installation_path);
				it != end_dir_it; ++it)
		{
			remove_all(it->path());
		}
	}
	output.ok("Prepared to install.", true);
}

void PHPbuilder::download_archive(string hostname, string download_file,
		string proto, string download_cntry)
{
	output.task();
	output.info(
			"Downloading archive " + download_file + " from " + download_cntry
					+ " ...");

	if (exists(local_downloaded_file))
	{
		output.info("Reusing previously downloaded file!", true);
	}
	else
	{
		/* Check fastest mirrors since there is no local file*/
		Check check;
		check.fastest_mirrors(1);

		char buffer[256];
		sprintf(buffer,
				"wget -O %s --tries=1 --timeout=2 %s://%s/get/php-%s/from/this/mirror -q --show-progress",
				local_downloaded_file.c_str(), proto.c_str(), hostname.c_str(),
				download_file.c_str());
		FILE *p = popen(buffer, "r");
		pclose(p);
	}
	output.ok("Download done!", true);

}

void PHPbuilder::download_tag(string hostname, string download_file,
		string proto, string download_location)
{
	output.task();
	output.info(
			"Downloading TAG " + download_file + " from " + download_location
					+ " ...");

	if (exists(local_downloaded_file))
	{
		output.info("Reusing previously downloaded file!", true);
	}
	else
	{
		char buffer[256];
		sprintf(buffer,
				"wget -O %s --tries=1 --timeout=2 %s://%s/php/php-src/archive/%s -q --show-progress",
				local_downloaded_file.c_str(), proto.c_str(), hostname.c_str(),
				download_file.c_str());
		FILE *p = popen(buffer, "r");
		pclose(p);
	}
	output.ok("Download done!", true);
}
void PHPbuilder::extract_archive()
{
	output.task();
	output.info("Extracting archive..");

	if (exists(local_downloaded_file) && exists(build_path))
	{
		string tar_cmd = "tar xf " + local_downloaded_file.string() + " -C "
				+ build_path.string() + " --strip-components=1";
		system(tar_cmd.c_str());
	}
	else
	{
		output.error("Could not extract the archive!", true);
	}
	output.ok("Extracting archive done!", true);
}
void PHPbuilder::run_buildconf()
{
	output.task();
	output.info("Running ./buildconf ...");

	string buildconf_cmd =
			(config.conf_storage["php"].isMember("buildconf")
					&& !config.conf_storage["php"]["buildconf"].empty()) ?
					"./buildconf "
							+ config.conf_storage["php"]["buildconf"].asString() :
					"./buildconf ";

	system(buildconf_cmd.c_str());

	output.ok("Buildconf done!");
}
void PHPbuilder::run_configure()
{
	output.task();
	output.info("Running ./configure with flags...");
	if (!config.conf_storage["php"]["configure"].empty())
	{
		Json::Value confflags;
		int i = 1;

		string configure_cmd = "./configure";

		/* ~/HOWI3/etc/php */
		config_file_path = path(
				config.conf_storage["paths"]["lib_etc_path"].asString());

		configure_cmd.append(" --prefix=\"").append(installation_path.string()).append(
				"\"");
		configure_cmd.append(" --exec-prefix=\"").append(
				installation_path.string()).append("\"");
		configure_cmd.append(" --with-config-file-path=\"").append(
				config_file_path.string()).append("\"");

		for (Json::ValueConstIterator itr =
				config.conf_storage["php"]["configure"].begin();
				itr != config.conf_storage["php"]["configure"].end(); itr++)
		{
			configure_cmd = configure_cmd + " " + (*itr).asString();

			i++;
		}

		output.banner(configure_cmd);
		system(configure_cmd.c_str());
	}
	else
	{
		output.error(
				"PHPbuild: All default ./configure flags are gone! File a BUG or reset configuration defaults!");
	}
	output.ok("Configure finished!", true);
}
void PHPbuilder::make()
{
	output.task();
	output.info("Running command (make)");
	system("make");
	output.info("Make finished and Build complete", true);
}
void PHPbuilder::make_test()
{
	output.task();
	output.info("Running command (make test)");
	system("make test");
	output.info("Make test finished!", true);
}
void PHPbuilder::make_install()
{
	output.task();
	output.info("Running command (make install)");
	path httpconf = config_file_path.append("httpd/conf");
	if (!exists(httpconf))
		boost::filesystem::create_directories(httpconf.string());

	path httpconffile = httpconf.append("httpd.conf");

	if (exists("/etc/httpd/conf/httpd.conf") && !exists(httpconffile))
		create_symlink("/etc/httpd/conf/httpd.conf", httpconffile);

	string make_install_cmd = "make install";
	system(make_install_cmd.c_str());

	output.info("Make install finished!", true);
}

PHPbuilder::~PHPbuilder()
{
	// TODO Auto-generated destructor stub
}
}
