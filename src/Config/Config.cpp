/*
 * ******************************************************************
 * Created by   Marko Kungla on 2010
 * Updated by   Marko Kungla @ Okram Labs on Aug 6, 2016 - 6:12:33 PM
 * Contact      okramlabs@gmail.com - https://okramlabs.com
 * @copyright   2010 - 2016 Marko Kungla - https://github.com/mkungla
 * @license     MIT
 *
 * Package name    howiphp
 * @category       HOWI3
 * @package        howiphp
 *
 * File         Config.cpp
 * @link        https://github.com/okramlabs/howiphp
 * ********************************************************************
 * Comments:
 * *******************************************************************/
#include "Config.h"
#include "../Check/Check.h"
#include "../PHPbuilder/PHPbuilder.h"
#include "../GitWorker/GitWorker.h"

namespace howiphp
{
Config config;
extern Output output;
long int Config::startup_time;
Json::Value Config::conf_storage;
Config::Config()
{
	// TODO Auto-generated constructor stub

}
void Config::set_startup_time()
{
	struct timeval startup_timeval;
	gettimeofday(&startup_timeval, NULL);
	startup_time = ((unsigned long long) startup_timeval.tv_sec * 1000000)
			+ startup_timeval.tv_usec;
}
long Config::get_elapsed_time()
{
	struct timeval startup_timeval;
	gettimeofday(&startup_timeval, NULL);
	long end_time = ((unsigned long long) startup_timeval.tv_sec * 1000000)
			+ startup_timeval.tv_usec;
	return end_time - startup_time;
}
void Config::init(int colored, bool run_config)
{

	/* Get path */
	struct passwd *pw = getpwuid(getuid());
	char homedir[100];
	strcpy(homedir, pw->pw_dir);
	strcat(homedir, "/HOWI3/etc/libhowi/php.json");

	/* Load Configuration */
	std::ifstream config_doc(homedir, std::ifstream::binary);
	Json::Value root;
	Json::CharReaderBuilder rbuilder;
	rbuilder["collectComments"] = false;
	std::string errs;
	bool parsingSuccessful = Json::parseFromStream(rbuilder, config_doc, &root,
			&errs);
	config_doc.close();

	if (!parsingSuccessful)
	{
		output.version();
		output.info("libhowi-php is not configured!", true);
		output.banner(
				"You can follow the interactive configuration to setup everything.");
		char type = '\0';
		do
		{

			output.question("Would you like to perform configuration? [y/n]");
			cin >> type;
		} while (type != 'y' && type != 'n');

		if (type == 'y' || type == 'Y')
			configuration();
		else
			output.info("Bye!", true);

		output.close();
		exit(0);
	}
	else
	{
		conf_storage = root;
		if (run_config)
		{
			output.version();
			post_configuration();
			output.close();
			exit(0);
		}

	}
}
void Config::configuration()
{
	output.info("Starting configuration...", true);
	output.resetCounters();

	/* USER */
	struct passwd *pw = getpwuid(getuid());

	/* Time */
	time_t now = time(0);
	string setup_date = ctime(&now);

	/* paths */
	Json::Value libpaths;
	path lib_path = path(pw->pw_dir).append("HOWI3");
	path bin_path = path(pw->pw_dir).append("HOWI3/bin");

	path lib_bin_path = path(lib_path.string()).append("bin");
	path lib_etc_path = path(lib_path.string()).append("etc");
	path lib_sbin_path = path(lib_path.string()).append("sbin");
	path lib_php_path = path(lib_path.string()).append("php");
	path lib_tmp_path = path(lib_path.string()).append("tmp");
	path conf_file = path(lib_path.string()).append("etc/libhowi/php.json");

	libpaths["lib_path"] = lib_path.string();
	libpaths["bin_path"] = bin_path.string();
	libpaths["lib_bin_path"] = lib_bin_path.string();
	libpaths["lib_sbin_path"] = lib_sbin_path.string();
	libpaths["lib_etc_path"] = lib_etc_path.string();
	libpaths["lib_php_path"] = lib_php_path.string();
	libpaths["lib_tmp_path"] = lib_tmp_path.string();
	libpaths["conf_file"] = conf_file.string();

	libpaths["php_extensions"] = libpaths["lib_path"].asString()
			+ "/usr/local/lib/php/extensions";
	/*
	 * INIT
	 */

	/* Create /check main working dir */
	output.task();
	if (!exists(lib_path.normalize()))
	{
		bool create_ws_dir = boost::filesystem::create_directories(
				lib_path.string());
		if (create_ws_dir)
		{
			boost::filesystem::create_directories(lib_bin_path.string());

			boost::filesystem::create_directories(lib_etc_path.string());
			lib_etc_path.append("libhowi");
			libpaths["lib_etc_libhowi"] = lib_etc_path.string();
			boost::filesystem::create_directories(lib_etc_path.string());
			lib_etc_path.append("php");
			boost::filesystem::create_directories(lib_etc_path.string());
			boost::filesystem::create_directories(lib_sbin_path.string());

			boost::filesystem::create_directories(lib_php_path.string());
			boost::filesystem::create_directories(
					lib_php_path.string() + "/git-src");
			boost::filesystem::create_directories(
					lib_php_path.string() + "/build");
			boost::filesystem::create_directories(
					lib_php_path.string() + "/versions");

			boost::filesystem::create_directories(lib_tmp_path.string());

			boost::filesystem::create_directories(
					libpaths["php_extensions"].asString());
			output.ok("Created workspace directories (~/HOWI3)");
		}
		else
		{
			output.error("Failed to create workspace directories!");
			output.banner(
					"For some reason you are not allowed to create a directory (~/HOWI3/php).");
		}
	}
	else
	{
		/* Thats all we care about (All other scripts should of cource make own chacks! ) */
		output.ok("libhowi-php installation paths are already there!");

	}

	/* Creating configuration files */
	output.task();
	if (!conf_storage.isMember("setup"))
	{
		conf_storage = defauts(pw, setup_date, libpaths, conf_file);
	}
	post_configuration(true);
}
void Config::post_configuration(bool skip_pre_conf)
{
	output.info("Starting configuration...", true);

	Check check;
	if (!skip_pre_conf)
	{
		char type = '\0';
		do
		{
			output.question(
					"Would you like to erase your current configuration and reset defaults? [y/n]");
			cin >> type;
		} while (type != 'y' && type != 'n');

		if (type == 'y' || type == 'Y')
		{
			configuration();
			exit(0);
		}
		output.resetCounters();
	}

	/* USER */
	//struct passwd *pw = getpwuid(getuid());
	/* Time */
	time_t now = time(0);
	string setup_date = ctime(&now);
	PHPbuilder phpbuilder;
	conf_storage["setup"]["updated"] = setup_date;

	/* POST CONF */
	/* Check git rpos */
	check.gitrepos(1);

	/* Check mirrors */
	check.mirrors();

	/* Check fastest mirrors */
	check.fastest_mirrors(1);

	/* PHP Buildconf */
	if (!check.phpbuild_buildconf())
		phpbuilder.buildconf();
	else
	{
		char type = '\0';
		do
		{
			output.question("Configure php buildconf? [y/n]");
			cin >> type;
		} while (type != 'y' && type != 'n');

		if (type == 'y' || type == 'Y')
		{
			phpbuilder.buildconf();
		}
	}

	/* PHP build ./configure */
	if (!check.phpbuild_configure())
		phpbuilder.configure();
	else
	{

		char type = '\0';
		do
		{
			output.question("Configure php ./configure flags? [y/n]");
			cin >> type;
		} while (type != 'y' && type != 'n');

		if (type == 'y' || type == 'Y')
		{
			phpbuilder.configure();
		}
	}

	/* Rsync available releases */
	check.rsync_releases();

	/* Git availabe tags */
	if (!check.gitsrc())
	{
		char type = '\0';
		do
		{
			output.question(
					"Check out php master to get possibility to build php from git TAGS? [y/n]");
			cin >> type;
		} while (type != 'y' && type != 'n');

		if (type == 'y' || type == 'Y')
		{
			GitWorker gitworker;
			gitworker.clone();
		}
	}
	else
	{
		GitWorker gitworker;
		gitworker.fetch();
	}

	check.gittags();

	char buff[PATH_MAX];
	string selfpath;
	ssize_t len = ::readlink("/proc/self/exe", buff, sizeof(buff) - 1);
	if (len != -1)
	{
		buff[len] = '\0';
		selfpath = string(buff);
	}
	path newbin =
			path(config.conf_storage["paths"]["bin_path"].asString()).append(
					"howiphp");
	/**
	 * selfpath == "/usr/bin/howiphp"
	 * would check if libhowi-php was installed trough os package manager
	 * so we dont need to make dummy copy of the executable
	 */
	if ((selfpath != newbin.string() && selfpath != "/usr/bin/howiphp")
			|| selfpath != "/usr/bin/howiphp")
	{
		char type = '\0';
		do
		{
			output.question(
					"Add executable to ~/HOWI3/bin so that you don't need current executable anymore [y/n]");
			cin >> type;
		} while (type != 'y' && type != 'n');

		if (type == 'y' || type == 'Y')
		{
			if (!exists(config.conf_storage["paths"]["bin_path"].asString()))
				boost::filesystem::create_directories(
						config.conf_storage["paths"]["bin_path"].asString());

			if (exists(newbin.string()))
				boost::filesystem::remove(newbin);

			boost::filesystem::copy_file(path(selfpath), newbin);
			output.ok("You can now execute libhowi-php as (howiphp) ");
		}
	}
	else
	{
		output.ok("Already executing howiphp executable!");
	}

	output.ok("Configuration done!", true);
	output.close();
	exit(1);
}

Json::Value Config::defauts(struct passwd *pw, std::string setup_date,
		Json::Value libpaths, path conf_file)
{
	Json::Value root;

	struct group * grp = getgrgid(pw->pw_gid);
	root["last-used"],

	/* Path variables */
	root["paths"]["lib_path"] = libpaths["lib_path"];
	root["paths"]["bin_path"] = libpaths["bin_path"];
	root["paths"]["lib_bin_path"] = libpaths["lib_bin_path"];
	root["paths"]["lib_sbin_path"] = libpaths["lib_sbin_path"];
	root["paths"]["lib_etc_path"] = libpaths["lib_etc_path"];
	root["paths"]["lib_php_path"] = libpaths["lib_php_path"];
	root["paths"]["lib_tmp_path"] = libpaths["lib_tmp_path"];
	root["paths"]["lib_etc_libhowi"] = libpaths["lib_etc_libhowi"];
	root["paths"]["conf_file"] = libpaths["conf_file"];
	root["paths"]["php_extensions"] = libpaths["php_extensions"];

	/* Will hold php versions installed */
	root["php"]["versions"];

	root["php"]["releases"];

	root["php"]["rsync"] = false;

	root["php"]["buildconf"] = "--force";
	root["php"]["configure"];
	//root["php"]["configure"].append("--with-config-file-path=$HOME/.libhowi/etc/php");
	root["php"]["configure"].append("--enable-fpm");
	string username_flag = "--with-fpm-user=" + string(pw->pw_name);
	root["php"]["configure"].append(username_flag);
	string groupname_flag = "--with-fpm-group=" + string(grp->gr_name);
	root["php"]["configure"].append(groupname_flag);
	root["php"]["configure"].append("--with-fpm-systemd");
	root["php"]["configure"].append("--with-libdir=lib64");
	root["php"]["configure"].append("--enable-fastcgi");
	root["php"]["configure"].append("--enable-cgi");
	root["php"]["configure"].append("--enable-force-cgi-redirect");
	root["php"]["configure"].append("--enable-spl");
	root["php"]["configure"].append("--enable-session");
	root["php"]["configure"].append("--enable-ctype");
	root["php"]["configure"].append("--enable-bcmath");
	root["php"]["configure"].append("--enable-mbstring=all");
	root["php"]["configure"].append("--enable-zip");
	root["php"]["configure"].append("--enable-pcntl");
	root["php"]["configure"].append("--enable-ftp");
	root["php"]["configure"].append("--enable-exif");
	root["php"]["configure"].append("--enable-calendar");
	root["php"]["configure"].append("--enable-sysvmsg");
	root["php"]["configure"].append("--enable-sysvsem");
	root["php"]["configure"].append("--enable-sysvshm");
	root["php"]["configure"].append("--enable-wddx");
	root["php"]["configure"].append("--enable-gd-native-ttf");
	root["php"]["configure"].append("--enable-gd-jis-conv");
	root["php"]["configure"].append("--enable-fd-setsize=2048");
	root["php"]["configure"].append("--enable-soap");
	root["php"]["configure"].append("--enable-sockets");
	root["php"]["configure"].append("--disable-option-checking");
	root["php"]["configure"].append("--disable-huge-code-pages");
	root["php"]["configure"].append("--with-curl=/usr");
	root["php"]["configure"].append("--with-mcrypt=/usr");
	root["php"]["configure"].append("--with-kerberos");
	root["php"]["configure"].append("--with-iconv");
	root["php"]["configure"].append("--with-gmp=/usr");
	root["php"]["configure"].append("--with-pspell");
	root["php"]["configure"].append("--with-gd");
	root["php"]["configure"].append("--with-expat-dir=/usr");
	root["php"]["configure"].append("--with-imap=/usr");
	root["php"]["configure"].append("--with-imap-ssl=/usr");
	root["php"]["configure"].append("--with-ldap=/usr");
	root["php"]["configure"].append("--with-icu-dir=/usr");
	root["php"]["configure"].append("--with-tidy=/usr");
	root["php"]["configure"].append("--with-pdo_sqlite=/usr");
	root["php"]["configure"].append("--with-sqlite3=/usr");
	root["php"]["configure"].append("--with-libxml-dir=/usr");
	root["php"]["configure"].append("--with-xsl=/usr");
	root["php"]["configure"].append("--with-dom=/usr");
	root["php"]["configure"].append("--with-xmlrpc");
	root["php"]["configure"].append("--with-jpeg-dir=/usr");
	root["php"]["configure"].append("--with-png-dir=/usr");
	root["php"]["configure"].append("--with-zlib-dir=/usr");
	root["php"]["configure"].append("--with-zlib");
	root["php"]["configure"].append("--with-xpm-dir=/usr");
	root["php"]["configure"].append("--with-freetype-dir=/usr");
	root["php"]["configure"].append("--with-openssl");
	root["php"]["configure"].append("--with-pdo-mysql=mysqlnd");
	root["php"]["configure"].append("--with-gettext");
	root["php"]["configure"].append("--with-mysql=mysqlnd");
	root["php"]["configure"].append("--with-mysqli=mysqlnd");
	root["php"]["configure"].append("--with-bz2=/usr");
	root["php"]["configure"].append("--enable-intl");
	root["php"]["configure"].append("--with-regex=php");
	root["php"]["configure"].append("--with-readline=/usr");

	// root["php"]["configure"].append("--without-pear");
	// root["php"]["configure"].append("--with-apxs2=/usr/bin/apxs");
	// root["php"]["configure"].append("\"CFLAGS=-O2 -m64 -fPIC\"");
	// root["php"]["configure"].append("\"CXXFLAGS=-m64\"");
	// root["php"]["configure"].append("\"LDFLAGS=-m64\"");
	// root["php"]["configure"].append("\"EXTRACFLAGS=-fPIC\"");
	/* Setup information */
	root["setup"]["status"] = "incomplete";
	root["setup"]["date"] = setup_date;
	root["setup"]["updated"] = setup_date;
	root["setup"]["file"] = "php.json";
	root["setup"]["pw_name"] = pw->pw_name;
	root["setup"]["pw_uid"] = pw->pw_uid;
	root["setup"]["pw_gid"] = pw->pw_gid;
	root["setup"]["group_name"] = grp->gr_name;
	root["setup"]["pw_gecos"] = pw->pw_gecos;
	root["setup"]["pw_dir"] = pw->pw_dir;
	root["setup"]["pw_shell"] = pw->pw_shell;

	/* fastest-mirrors */
	root["source"]["fastest-mirrors"]["fm1"];
	root["source"]["fastest-mirrors"]["fm2"];
	root["source"]["fastest-mirrors"]["fm3"];
	root["source"]["fastest-mirrors"]["fm4"];
	root["source"]["fastest-mirrors"]["fm5"];

	/* All source mirrors */
	root["source"]["mirrors"]["am1.php.net"]["country"] = "Armenia";
	root["source"]["mirrors"]["am1.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["am1.php.net"]["provider"] =
			"ARMINCO Global Telecommunications";
	root["source"]["mirrors"]["ar2.php.net"]["country"] = "Argentina";
	root["source"]["mirrors"]["ar2.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["ar2.php.net"]["provider"] =
			"XMundo Hosting Solutions";
	root["source"]["mirrors"]["at1.php.net"]["country"] = "Austria";
	root["source"]["mirrors"]["at1.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["at1.php.net"]["provider"] =
			"Goodie Domain Service";
	root["source"]["mirrors"]["at2.php.net"]["country"] = "Austria";
	root["source"]["mirrors"]["at2.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["at2.php.net"]["provider"] =
			"Yalwa Local Directory Services Austria";
	root["source"]["mirrors"]["au1.php.net"]["country"] = "Australia";
	root["source"]["mirrors"]["au1.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["au1.php.net"]["provider"] = "UberGlobal";
	root["source"]["mirrors"]["au2.php.net"]["country"] = "Australia";
	root["source"]["mirrors"]["au2.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["au2.php.net"]["provider"] =
			"Servers Australia Pty. Ltd.";
	root["source"]["mirrors"]["be2.php.net"]["country"] = "Belgium";
	root["source"]["mirrors"]["be2.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["be2.php.net"]["provider"] = "Cu.be Solutions";
	root["source"]["mirrors"]["bg2.php.net"]["country"] = "Bulgaria";
	root["source"]["mirrors"]["bg2.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["bg2.php.net"]["provider"] = "Data.BG";
	root["source"]["mirrors"]["br2.php.net"]["country"] = "Brazil";
	root["source"]["mirrors"]["br2.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["br2.php.net"]["provider"] = "Umbler";
	root["source"]["mirrors"]["ca1.php.net"]["country"] = "Canada";
	root["source"]["mirrors"]["ca1.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["ca1.php.net"]["provider"] = "easyDNS";
	root["source"]["mirrors"]["ca2.php.net"]["country"] = "Canada";
	root["source"]["mirrors"]["ca2.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["ca2.php.net"]["provider"] = "Parasane, LLC";
	root["source"]["mirrors"]["ca3.php.net"]["country"] = "Canada";
	root["source"]["mirrors"]["ca3.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["ca3.php.net"]["provider"] = "egateDOMAINS";
	root["source"]["mirrors"]["ch1.php.net"]["country"] = "Switzerland";
	root["source"]["mirrors"]["ch1.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["ch1.php.net"]["provider"] = "ComunidadHosting";
	root["source"]["mirrors"]["cl1.php.net"]["country"] = "Chile";
	root["source"]["mirrors"]["cl1.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["cl1.php.net"]["provider"] = "Caos Consultores";
	root["source"]["mirrors"]["cz1.php.net"]["country"] = "Czech Republic";
	root["source"]["mirrors"]["cz1.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["cz1.php.net"]["provider"] =
			"Czech Technical University in Prague";
	root["source"]["mirrors"]["cz2.php.net"]["country"] = "Czech Republic";
	root["source"]["mirrors"]["cz2.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["cz2.php.net"]["provider"] = "Softaculous Ltd.";
	root["source"]["mirrors"]["de1.php.net"]["country"] = "Germany";
	root["source"]["mirrors"]["de1.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["de1.php.net"]["provider"] = "@GLOBE GmbH";
	root["source"]["mirrors"]["de2.php.net"]["country"] = "Germany";
	root["source"]["mirrors"]["de2.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["de2.php.net"]["provider"] =
			"Locanto Kleinanzeigen";
	root["source"]["mirrors"]["dk2.php.net"]["country"] = "Denmark";
	root["source"]["mirrors"]["dk2.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["dk2.php.net"]["provider"] = "Kobalt";
	root["source"]["mirrors"]["docs.php.net"]["country"] = "Unknown";
	root["source"]["mirrors"]["docs.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["docs.php.net"]["provider"] = "EUKhost";
	root["source"]["mirrors"]["ee1.php.net"]["country"] = "Estonia";
	root["source"]["mirrors"]["ee1.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["ee1.php.net"]["provider"] = "Zone Media LLC";
	root["source"]["mirrors"]["es1.php.net"]["country"] = "Spain";
	root["source"]["mirrors"]["es1.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["es1.php.net"]["provider"] =
			"GRN Serveis Telematics";
	root["source"]["mirrors"]["fi2.php.net"]["country"] = "Finland FIN";
	root["source"]["mirrors"]["fi2.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["fi2.php.net"]["provider"] =
			"Planeetta Internet OY";
	root["source"]["mirrors"]["fr2.php.net"]["country"] = "France FRA";
	root["source"]["mirrors"]["fr2.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["fr2.php.net"]["provider"] = "Crihan";
	root["source"]["mirrors"]["hk1.php.net"]["country"] = "Hong Kong";
	root["source"]["mirrors"]["hk1.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["hk1.php.net"]["provider"] =
			"Nethub Online Limited";
	root["source"]["mirrors"]["hk2.php.net"]["country"] = "Hong Kong";
	root["source"]["mirrors"]["hk2.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["hk2.php.net"]["provider"] =
			"Website Solution Web Hosting";
	root["source"]["mirrors"]["ie1.php.net"]["country"] = "Ireland";
	root["source"]["mirrors"]["ie1.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["ie1.php.net"]["provider"] =
			"Yalwa - Local Directory Services Ireland";
	root["source"]["mirrors"]["ie2.php.net"]["country"] = "Ireland";
	root["source"]["mirrors"]["ie2.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["ie2.php.net"]["provider"] = "Hosting Ireland";
	root["source"]["mirrors"]["il1.php.net"]["country"] = "Israel";
	root["source"]["mirrors"]["il1.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["il1.php.net"]["provider"] = "SPD HOSTING LTD";
	root["source"]["mirrors"]["in1.php.net"]["country"] = "India";
	root["source"]["mirrors"]["in1.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["in1.php.net"]["provider"] =
			"Directi Web Hosting";
	root["source"]["mirrors"]["in2.php.net"]["country"] = "India";
	root["source"]["mirrors"]["in2.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["in2.php.net"]["provider"] =
			"Directi Web Hosting";
	root["source"]["mirrors"]["in3.php.net"]["country"] = "India";
	root["source"]["mirrors"]["in3.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["in3.php.net"]["provider"] =
			"IndiaLinks Web Hosting Pvt Ltd";
	root["source"]["mirrors"]["ir1.php.net"]["country"] = "Iran";
	root["source"]["mirrors"]["ir1.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["ir1.php.net"]["provider"] = "Golha.IR";
	root["source"]["mirrors"]["ir2.php.net"]["country"] = "Iran";
	root["source"]["mirrors"]["ir2.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["ir2.php.net"]["provider"] = "ParsPack.com";
	root["source"]["mirrors"]["is1.php.net"]["country"] = "Iceland";
	root["source"]["mirrors"]["is1.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["is1.php.net"]["provider"] =
			"Hysingarfelagio ehf";
	root["source"]["mirrors"]["is2.php.net"]["country"] = "Iceland";
	root["source"]["mirrors"]["is2.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["is2.php.net"]["provider"] = "Dotgeek";
	root["source"]["mirrors"]["it2.php.net"]["country"] = "Italy";
	root["source"]["mirrors"]["it2.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["it2.php.net"]["provider"] = "nidohosting";
	root["source"]["mirrors"]["jm2.php.net"]["country"] = "Jamaica";
	root["source"]["mirrors"]["jm2.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["jm2.php.net"]["provider"] =
			"Teamopolis Sports Websites Inc.";
	root["source"]["mirrors"]["jp2.php.net"]["country"] = "Japan";
	root["source"]["mirrors"]["jp2.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["jp2.php.net"]["provider"] = "snotch";
	root["source"]["mirrors"]["kr1.php.net"]["country"] = "Republic of Korea";
	root["source"]["mirrors"]["kr1.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["kr1.php.net"]["provider"] = "ask.sarang.net";
	root["source"]["mirrors"]["li1.php.net"]["country"] = "Liechtenstein";
	root["source"]["mirrors"]["li1.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["li1.php.net"]["provider"] =
			"Telecom Liechtenstein AG";
	root["source"]["mirrors"]["lt1.php.net"]["country"] = "Lithuania";
	root["source"]["mirrors"]["lt1.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["lt1.php.net"]["provider"] =
			"UAB Interneto vizija";
	root["source"]["mirrors"]["lv1.php.net"]["country"] = "Latvia";
	root["source"]["mirrors"]["lv1.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["lv1.php.net"]["provider"] = "Kaspars Foigts";
	root["source"]["mirrors"]["md1.php.net"]["country"] = "Republic of Moldova";
	root["source"]["mirrors"]["md1.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["md1.php.net"]["provider"] = "dev.md";
	root["source"]["mirrors"]["mx1.php.net"]["country"] = "Mexico";
	root["source"]["mirrors"]["mx1.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["mx1.php.net"]["provider"] = "uServers Mexico";
	root["source"]["mirrors"]["mx2.php.net"]["country"] = "Mexico";
	root["source"]["mirrors"]["mx2.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["mx2.php.net"]["provider"] =
			"Universidad Autonoma Metropolitana Azcapotzalco";
	root["source"]["mirrors"]["my1.php.net"]["country"] = "Malaysia";
	root["source"]["mirrors"]["my1.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["my1.php.net"]["provider"] = "MaxDedicated";
	root["source"]["mirrors"]["nl1.php.net"]["country"] = "Netherlands";
	root["source"]["mirrors"]["nl1.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["nl1.php.net"]["provider"] = "Stream Service";
	root["source"]["mirrors"]["nl3.php.net"]["country"] = "Netherlands";
	root["source"]["mirrors"]["nl3.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["nl3.php.net"]["provider"] =
			"Computel Standby BV";
	root["source"]["mirrors"]["no1.php.net"]["country"] = "Norway";
	root["source"]["mirrors"]["no1.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["no1.php.net"]["provider"] = "P&#314;l Sollie";
	root["source"]["mirrors"]["no2.php.net"]["country"] = "Norway";
	root["source"]["mirrors"]["no2.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["no2.php.net"]["provider"] = "Verdens Gang AS";
	root["source"]["mirrors"]["nz2.php.net"]["country"] = "New Zealand";
	root["source"]["mirrors"]["nz2.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["nz2.php.net"]["provider"] = "Catalyst IT Ltd";
	root["source"]["mirrors"]["pa1.php.net"]["country"] = "Panama";
	root["source"]["mirrors"]["pa1.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["pa1.php.net"]["provider"] = "Unidominios";
	root["source"]["mirrors"]["php.net"]["country"] = "United States";
	root["source"]["mirrors"]["php.net"]["proto"] = "https";
	root["source"]["mirrors"]["php.net"]["provider"] = "myracloud";
	root["source"]["mirrors"]["pl1.php.net"]["country"] = "Poland";
	root["source"]["mirrors"]["pl1.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["pl1.php.net"]["provider"] = "WEBdev";
	root["source"]["mirrors"]["ro1.php.net"]["country"] = "Romania";
	root["source"]["mirrors"]["ro1.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["ro1.php.net"]["provider"] = "SpiderVPS";
	root["source"]["mirrors"]["se1.php.net"]["country"] = "Sweden";
	root["source"]["mirrors"]["se1.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["se1.php.net"]["provider"] = "Portlane AB";
	root["source"]["mirrors"]["se2.php.net"]["country"] = "Sweden";
	root["source"]["mirrors"]["se2.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["se2.php.net"]["provider"] = "SpaceDump IT AB";
	root["source"]["mirrors"]["sg2.php.net"]["country"] = "Singapore";
	root["source"]["mirrors"]["sg2.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["sg2.php.net"]["provider"] =
			"Xssist Group (Singapore) Pte Ltd";
	root["source"]["mirrors"]["sg3.php.net"]["country"] = "Singapore";
	root["source"]["mirrors"]["sg3.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["sg3.php.net"]["provider"] = "NamesAhead.com";
	root["source"]["mirrors"]["tr1.php.net"]["country"] = "Turkey";
	root["source"]["mirrors"]["tr1.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["tr1.php.net"]["provider"] =
			"Istanbul Teknik Universitesi Bilgi Islem Daire Baskanligi";
	root["source"]["mirrors"]["tr2.php.net"]["country"] = "Turkey";
	root["source"]["mirrors"]["tr2.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["tr2.php.net"]["provider"] = "DGN Teknoloji";
	root["source"]["mirrors"]["tw1.php.net"]["country"] = "Taiwan";
	root["source"]["mirrors"]["tw1.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["tw1.php.net"]["provider"] = "twemail.com";
	root["source"]["mirrors"]["tw2.php.net"]["country"] = "Taiwan";
	root["source"]["mirrors"]["tw2.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["tw2.php.net"]["provider"] = "www.mirror.tw";
	root["source"]["mirrors"]["tz1.php.net"]["country"] =
			"United Republic of Tanzania";
	root["source"]["mirrors"]["tz1.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["tz1.php.net"]["provider"] = "Aptus Solutions";
	root["source"]["mirrors"]["ua2.php.net"]["country"] = "Ukraine";
	root["source"]["mirrors"]["ua2.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["ua2.php.net"]["provider"] = "Max Khaikin";
	root["source"]["mirrors"]["uk1.php.net"]["country"] = "United Kingdom";
	root["source"]["mirrors"]["uk1.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["uk1.php.net"]["provider"] = "Camel Network";
	root["source"]["mirrors"]["uk3.php.net"]["country"] = "United Kingdom";
	root["source"]["mirrors"]["uk3.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["uk3.php.net"]["provider"] = "CatN PHP Hosting";
	root["source"]["mirrors"]["us1.php.net"]["country"] = "United States";
	root["source"]["mirrors"]["us1.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["us1.php.net"]["provider"] = "NEXCESS.NET";
	root["source"]["mirrors"]["us2.php.net"]["country"] = "United States";
	root["source"]["mirrors"]["us2.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["us2.php.net"]["provider"] = "Hurricane Electric";
	root["source"]["mirrors"]["us3.php.net"]["country"] = "United States";
	root["source"]["mirrors"]["us3.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["us3.php.net"]["provider"] = "C7 Data Centers";
	root["source"]["mirrors"]["za1.php.net"]["country"] = "South Africa";
	root["source"]["mirrors"]["za1.php.net"]["proto"] = "http";
	root["source"]["mirrors"]["za1.php.net"]["provider"] = "AfriCC";

	/* Will hold php souce repos, and other download urls */
	root["source"]["repos"]["Github"]["host"] = "github.com";
	root["source"]["repos"]["Github"]["path"] = "/php/php-src.git";
	root["source"]["repos"]["Github"]["ip"] = "";
	root["source"]["repos"]["Github"]["proto"] = "https";

	boost::filesystem::ofstream file_id;
	file_id.open(conf_file.string());
	file_id << root;
	file_id.close();
	output.ok(
			"Created default configuration file. (~/HOWI3/etc/libhowi/php.json)");

	return root;
}
Config::~Config()
{
	// TODO Auto-generated destructor stub
}
}
