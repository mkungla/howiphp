/*
 * ******************************************************************
 * Created by   Marko Kungla on 2010
 * Updated by   Marko Kungla on Aug 6, 2016 - 6:09:39 PM
 *
 * @copyright   2010 - 2016 Marko Kungla - https://github.com/mkungla
 * @license     MIT
 *
 * Package name    howiphp
 * @category       HOWI3
 * @package        howiphp
 *
 * File         Check.cpp
 * @link        https://github.com/mkungla/howiphp
 * ********************************************************************
 * Comments:
 * *******************************************************************/
#include "Check.h"
namespace howiphp
{

bool Check::errors;
bool Check::warnings;
string Check::dnf_install;

Check::Check()
{
	resetStates();
}

/* Check is configuration done */
bool Check::isConfigured(FlagInt flag_colors)
{
	output.task();
	string filename = string(getenv("HOME")) + "/.howiphp/etc/howiphp.json";

	config.init(flag_colors);
	output.info("Checking is libhowi-php configured...");
	output.version();

	if (file_exist(filename))
	{
		output.ok("Libhowi-php is configured.", true);
		return true;
	}

	output.info("libhowi-php is not configured!");
	return false;
}
/* Check the user environment path variable */
bool Check::envpath()
{
	// Get User environment path var
	string envvars = getenv("PATH");
	size_t p = envvars.find(string(getenv("HOME")) + "/.howiphp/bin");

	/**
	 * If first part of PATH is not ~/.howiphp
	 *
	 * then notify user that installed php versions are not reachable. Also if ~/.howiphp  is set but not in
	 * the beginning of the PATH then existing global php version would be used from /usr/bin instead intended v:
	 *
	 * PATH=/usr/local/bin:$PATH;
	 * export PATH
	 */
	if (p == string::npos)
	{
		output.warning(
				"Environment path is not set: Add export PATH=$HOME/.howiphp/bin:$PATH; to your ~/.bashrc file");
		output.banner(
				"so that $HOME/.howiphp/bin would be searched for active php executables");
		output.banner("then 'source ~/.bashrc | log out/in'");
		return false;
	}

	return true;
}

/* Check system dependencies */
bool Check::system()
{
	/* reset warnings and erroros to false */
	resetStates();

	/* Register this function as task */
	output.task();

	output.info("Checking system dependencies...");

	/* Get system dependencies and check these */
	Dependencies dependencies;
	auto system_deps = dependencies.system();

	for (auto& dep : system_deps)
		dependency(dep["dependeny"].asString(), dep["minversion"].asString(),
				dep["dnf_pkg"].asString(), dep["required"].asBool(),
				dep["required_by"].asString());

	if (hasWarnings())
	{
		output.warning(
				"Some howiphp dependencies have to low version update these!",
				true);
	}
	if (hasErrors())
	{
		output.error(
				"Missing howiphp dependencies: Install missing dependencies before you continue!",
				true);
		return false;
	}
	output.info(
			"Checking howiphp dependencies is done and these are all ok",
			true);
	return true;

}

/* Dependencies recommended by php.net */
bool Check::php_net()
{
	/* reset warnings and erroros to false */
	resetStates();

	/* Register this function as task */
	output.task();

	output.info("Checking dependencies recommended by php.net...");

	/* Get php_net dependencies and check these */
	Dependencies dependencies;
	auto php_net_deps = dependencies.php_net();

	for (auto& dep : php_net_deps)
		this->dependency(dep["dependeny"].asString(),
				dep["minversion"].asString(), dep["dnf_pkg"].asString(),
				dep["required"].asBool(), dep["required_by"].asString());

	if (hasWarnings())
	{
		output.warning(
				"Some php.net recommended dependencies have to low version update these!",
				true);
	}
	if (hasErrors())
	{
		output.error(
				"Missing php.net recommended dependencies: Install missing dependencies before you continue!",
				true);
		return false;
	}
	output.info(
			"Checking php.net recommended dependencies is done and these are all ok",
			true);
	return true;

}

/* Basic dependencies to compile php */
bool Check::compiler()
{
	/* reset warnings and erroros to false */
	resetStates();

	/* Register this function as task */
	output.task();

	output.info("Checking compiler dependencies...");

	/* Get php_net dependencies and check these */
	Dependencies dependencies;
	auto compiler_deps = dependencies.compiler();

	for (auto& dep : compiler_deps)
		this->dependency(dep["dependeny"].asString(),
				dep["minversion"].asString(), dep["dnf_pkg"].asString(),
				dep["required"].asBool(), dep["required_by"].asString());

	if (hasWarnings())
	{
		output.warning(
				"Some compiler dependencies have to low version update these!",
				true);
	}
	if (hasErrors())
	{
		output.error(
				"Missing compiler dependencies: Install missing dependencies before you continue!",
				true);
		return false;
	}
	output.info("Checking compiler dependencies is done and these are all ok",
			true);
	return true;

}

/** Check Optional dependencies required by php extesions and build variants*/
bool Check::optional()
{
	/* reset warnings and erroros to false */
	resetStates();

	/* Register this function as task */
	output.task();

	output.info("Checking optional dependencies...");

	/* Get php_net dependencies and check these */
	Dependencies dependencies;
	auto optional_deps = dependencies.optional();

	for (auto& dep : optional_deps)
		this->dependency(dep["dependeny"].asString(),
				dep["minversion"].asString(), dep["dnf_pkg"].asString(),
				dep["required"].asBool(), dep["required_by"].asString());

	if (hasWarnings())
	{
		output.warning(
				"Some optional dependencies have to low version update these!",
				true);
	}
	/* We do not report errors since these are optional dependencies */
	output.info("Checking optional dependencies is done and these are all ok",
			true);
	return true;

}

bool Check::gitrepos(FlagInt flag_with_ping)
{
	/* reset warnings and erroros to false */
	resetStates();

	/* Register this function as task */
	output.task();
	output.info("Checking GIT Repositories...");
	if (!config.conf_storage["source"].isMember("repos"))
	{
		output.info("Git repositories not configured!");
		return false;
	}
	const Json::Value source_repos = config.conf_storage["source"]["repos"];

	if (config.conf_storage["source"]["repos"].size() > 0)
	{
		output.info("Found repositories, checking connectivity!", true);
		int i = 1;
		for (Json::ValueConstIterator itr =
				config.conf_storage["source"]["repos"].begin();
				itr != config.conf_storage["source"]["repos"].end(); itr++)
		{
			gitrepo(i, itr.name(), (*itr)["ip"].asString(),
					(*itr)["host"].asString(), (*itr)["path"].asString(),
					(*itr)["proto"].asString(), flag_with_ping);

			i++;
		}
	}
	else
	{
		setWarnings(true);
	}
	if (hasWarnings())
	{
		output.info("No repos or validating connection to some of them failed.",
				true);
		output.banner(
				"If you want to install php versions from git then You have to set up some repositories.");
		output.banner(
				"You can ignore missing repository messages if you plan to install only stable versions or from tarball");
	}
	if (hasErrors())
	{
		output.error("Validating git repo failed!", true);
		return false;
	}
	if (!hasWarnings() && !hasErrors())
		output.info("Checking GIT repositories is done and these are all ok",
				true);
	return true;
}
bool Check::gitrepo(ID id, RepoName name, IPaddress ip, HostName host,
		RepoRemoteUrl repopath, Protocol proto, FlagInt flag_with_ping)
{
	output.task();
	char repo_ping[20];
	char cid[5];
	sprintf(cid, "%d", id);

	if (!flag_with_ping)
	{
		output.task();
		output.ok(
				(string(cid)) + " repository (" + name + ") - " + proto + "://"
						+ host);
		return true;
	}

	CDN repo_cdn = CDN(proto, host, repopath, false);
	if (ip.empty())
	{
		config.conf_storage["source"]["repos"][name]["ip"] =
				repo_cdn.resolvehostip();
		string ip =
				config.conf_storage["source"]["repos"][name]["ip"].asString();
		output.info(
				"Updating " + (string(cid)) + " (" + name + ") " + proto + "://"
						+ host + repopath + " IP!");
	}

	repo_cdn.get_ping();
	if (repo_cdn.ping > 0)
	{
		sprintf(repo_ping, "%.2f ms", repo_cdn.ping);
		output.ok(
				(string(cid)) + " (" + name + ") " + proto + "://" + host
						+ repopath + " IP:" + ip + " ping: " + repo_ping);
	}
	else
	{
		output.warning(
				"Can not reach repository " + (string(cid)) + " (" + name + ") "
						+ proto + "://" + host + repopath + " IP:" + ip);
		warnings = true;
		return false;
	}
	return true;
}
bool Check::mirrors(FlagInt flag_with_ping)
{
	errors = false;
	warnings = false;
	output.task();
	output.info("Checking download mirrors...");

	const Json::Value source_mirrors = config.conf_storage["source"]["mirrors"];

	if (config.conf_storage["source"]["mirrors"].size() > 0)
	{
		output.info("Found mirrors, checking connectivity!", true);
		int i = 1;
		output.progressbar(config.conf_storage["source"]["mirrors"].size());
		for (Json::ValueConstIterator itr =
				config.conf_storage["source"]["mirrors"].begin();
				itr != config.conf_storage["source"]["mirrors"].end(); itr++)
		{

			mirror(i, itr.name(), (*itr)["country"].asString(),
					(*itr)["provider"].asString(), (*itr)["ip"].asString(),
					(*itr)["proto"].asString(), flag_with_ping);
			output.progressbar();
			i++;

		}
	}
	else
	{
		warnings = true;
	}
	if (warnings)
	{
		output.warning(
				"Validating connection to some of the mirrors failed. or there are no mirrors",
				true);
	}
	if (errors)
	{
		output.error("Validating mirrors failed!", true);
		return false;
	}
	if (!warnings && !errors)
		output.info("Checking mirrors is done!", true);
	return true;
}
bool Check::mirror(ID id, HostName host, Country country, ProviderName provider,
		IPaddress ip, Protocol proto, FlagInt flag_with_ping)
{

	usleep(10000);
	char mirror_ping[20];
	char cid[5];

	sprintf(cid, "%d", id);

	if (!flag_with_ping)
	{
		output.task();
		output.ok(
				(string(cid)) + " mirror (" + country + ") - " + proto + "://"
						+ host + " by " + provider);
		return true;
	}
	CDN mirror = CDN(proto, host, "");
	if (ip.empty())
	{

		ip = mirror.resolvehostip();
		if (ip.empty())
		{
			return false;
			output.error(
					"Not valid hostname " + (string(cid)) + " (" + country
							+ ") - " + proto + "://" + host);
		}
		config.conf_storage["source"]["mirrors"][host]["ip"] = ip;

	}
	output.task();
	mirror.get_ping();
	if (mirror.ping > 0)
	{
		config.conf_storage["source"]["mirrors"][host]["ping"] =
				boost::lexical_cast<std::string>(mirror.ping);
		sprintf(mirror_ping, "%.2f ms", mirror.ping);
		output.ok(
				(string(cid)) + " (" + country + ") - (" + mirror_ping + ") "
						+ proto + "://" + host + " by " + provider);

	}
	else
	{
		output.info(
				"Did not recieve response ping to " + (string(cid)) + " ("
						+ country + ") - " + proto + "://" + host + " IP:"
						+ ip);
	}

	return true;
}
bool Check::fastest_mirrors(FlagInt flag_with_download)
{
	errors = false;
	warnings = false;
	output.task();
	output.info("Checking fastest mirrors...");
	const Json::Value source_mirrors = config.conf_storage["source"]["mirrors"];

	/* Update mirror speeds */
	if (flag_with_download)
	{
		if (config.conf_storage["source"]["mirrors"].size() > 0)
		{
			output.info("Updating fastest mirrors...", true);
			Json::Value new_fastest_mirrors;
			new_fastest_mirrors[0]["ms"] = "100000000000";
			new_fastest_mirrors[1]["ms"] = "100000000000";
			new_fastest_mirrors[2]["ms"] = "100000000000";
			new_fastest_mirrors[3]["ms"] = "100000000000";
			new_fastest_mirrors[4]["ms"] = "100000000000";

			output.progressbar(config.conf_storage["source"]["mirrors"].size());

			for (Json::ValueConstIterator itr = source_mirrors.begin();
					itr != source_mirrors.end(); itr++)
			{
				output.task();
				output.progressbar();

				string host = itr.name();
				string country = (*itr)["country"].asString();
				string proto = (*itr)["proto"].asString();

				char speed_message[100];
				CDN mirror_cdn = CDN(proto, host, "ChangeLog-4.php", true);

                // If mirror is failing we remove that mirror
				if(mirror_cdn.dl_speed == "unknown")
				{
				    config.conf_storage["source"]["mirrors"].removeMember(host);
				    for (Json::Value::iterator fitr = config.conf_storage["source"]["fastest-mirrors"].begin();
                        fitr != config.conf_storage["source"]["fastest-mirrors"].end(); fitr++)
                    {
                       if((*fitr)["hostname"].asString() == host) {
                    	   output.warning("Ghost fastest mirror was removed!!! (" + fitr.key().asString() + ")");
                    	   config.conf_storage["source"]["fastest-mirrors"].removeMember(fitr.key().asString());
                    	   output.info("You can now re run that command or re fetch fastest mirrors!");
                    	   output.close();
                        }
                    }
				    continue;
				}

				config.conf_storage["source"]["mirrors"][host]["kbs"] =
						mirror_cdn.kbs;
				config.conf_storage["source"]["mirrors"][host]["mbs"] =
						mirror_cdn.mbs;
				config.conf_storage["source"]["mirrors"][host]["speed"] =
						mirror_cdn.dl_speed;
				string dl_milliseconds;
				stringstream dl_milliseconds_stream;
				dl_milliseconds_stream << mirror_cdn.ms;
				dl_milliseconds_stream >> dl_milliseconds;
				config.conf_storage["source"]["mirrors"][host]["ms"] =
						dl_milliseconds;
				config.conf_storage["source"]["mirrors"][host]["sec"] =
						mirror_cdn.sec;

				/* Add downloads counter if not already set*/
				if (!config.conf_storage["source"]["mirrors"][host].isMember(
						"downloads"))
					config.conf_storage["source"]["mirrors"][host]["downloads"] =
							0;

				mirror_cdn.get_ping();
				char mirror_ping[20];
				if (mirror_cdn.ping > 0)
				{
					sprintf(mirror_ping, "%.2f", mirror_cdn.ping);
					config.conf_storage["source"]["mirrors"][host]["ping"] =
							mirror_ping;

				}
				sprintf(speed_message,
						"(%s) %s download took %.2f seconds avg speed %s ping: %s ms",
						country.c_str(), itr.name().c_str(), mirror_cdn.sec,
						mirror_cdn.dl_speed.c_str(), mirror_ping);
				output.ok(speed_message);

				bool fastest = false;
				for (int i = 0; i < 5; i++)
				{
					if (!new_fastest_mirrors[i].isMember("ms")
							|| mirror_cdn.ms
									< atol(
											new_fastest_mirrors[i]["ms"].asCString()))
					{
						config.conf_storage["source"]["mirrors"].removeMember(
								new_fastest_mirrors[4]["host"].asString());
						if (i < 5)
						{
							for (int e = 4; e > i; e--)
							{
								new_fastest_mirrors[e]["host"] =
										new_fastest_mirrors[(e - 1)]["host"];
								new_fastest_mirrors[e]["ms"] =
										new_fastest_mirrors[(e - 1)]["ms"];
							}
						}
						new_fastest_mirrors[i]["host"] = host;
						new_fastest_mirrors[i]["ms"] = dl_milliseconds;
						fastest = true;
						break;
					}

				}
				if (!fastest
						&& config.conf_storage["source"]["mirrors"][host].isMember(
								"downloads")
						&& config.conf_storage["source"]["mirrors"][host]["downloads"]
								== 0)
				{
					/*
					 * Removing slow mirrors1
					 * This will not remove mirrors which pop out from fastest mirrors
					 */
					config.conf_storage["source"]["mirrors"].removeMember(host);
				}

			}
			for (Json::ValueConstIterator itr = new_fastest_mirrors.begin();
					itr != new_fastest_mirrors.end(); itr++)
			{

				string str_hostname = (*itr)["host"].asString();

                // IF host is empty don't apply
                if(str_hostname.empty())
                {
                	continue;
                }
				string key = boost::lexical_cast<string>(itr.key().asInt() + 1);

				config.conf_storage["source"]["fastest-mirrors"]["fm" + key]["ms"] =
						config.conf_storage["source"]["mirrors"][str_hostname]["ms"].asString();
				config.conf_storage["source"]["fastest-mirrors"]["fm" + key]["sec"] =
						config.conf_storage["source"]["mirrors"][str_hostname]["sec"].asString();
				config.conf_storage["source"]["fastest-mirrors"]["fm" + key]["hostname"] =
						str_hostname;
				config.conf_storage["source"]["fastest-mirrors"]["fm" + key]["speed"] =
						config.conf_storage["source"]["mirrors"][str_hostname]["speed"].asString();
				config.conf_storage["source"]["mirrors"][str_hostname]["downloads"] =
						config.conf_storage["source"]["mirrors"][str_hostname]["downloads"].asInt()
								+ 1;
			}
		}
		else
		{
			output.warning(
					"Can not choose fastest mirror if there is no source mirrors!",
					true);
		}

	}

	char cid[5];
	auto fastest_mirrors = config.conf_storage["source"]["fastest-mirrors"];
	if (fastest_mirrors.size() > 0)
	{
		output.info("Found list of fastest mirrors!", true);
		int i = 1;
		// Right way to iterate json val
		for (auto& itr : fastest_mirrors)
		{

			output.task();
			sprintf(cid, "%d", i);
			string hostname = itr["hostname"].asString();
			string mirror_speed =
					config.conf_storage["source"]["mirrors"][hostname]["speed"].asString();
			if (!mirror_speed.empty())
				mirror_speed = " speed: " + mirror_speed;
			string mirror_ping =
					config.conf_storage["source"]["mirrors"][hostname]["ping"].asString();
			if (!mirror_ping.empty())
				mirror_ping = " ping: " + mirror_ping;
			string mirror_country =
					config.conf_storage["source"]["mirrors"][hostname]["country"].asString();
			string mirror_provider =
					config.conf_storage["source"]["mirrors"][hostname]["provider"].asString();

			output.ok(
					(string(cid)) + " (" + mirror_country + ") - " + hostname
							+ " by " + mirror_provider + mirror_speed
							+ mirror_ping);

			i++;
		}
	}
	else
	{
		warnings = true;
	}
	if (warnings)
	{
		output.warning(
				"No fastest mirrors listed. run (howiphp check -d) do update fastest mirrors!",
				true);
	}
	if (errors)
	{
		output.error("Validating fastest mirrors failed!", true);
		return false;
	}
	if (!warnings && !errors)
		output.info("Checking fastest mirrors is done!", true);
	return true;

	/*
	 */
}
bool Check::dependency(PackageName dependeny, VersionName minversion,
		PackageManagerName dnf_pkg, Required required, PackageName required_by)
{
	/* Check  */
	FILE *in;
	char buff[128];

	char check_command[sizeof(dependeny)];
	sprintf(check_command, "rpm -q --qf \"%%{VERSION}\" %s", dependeny.c_str());

	if (!(in = popen(check_command, "r")))
	{
		output.error("Failed to fetch dependency info!", true);
	}
	else
	{
		while (fgets(buff, sizeof(buff), in) != NULL)
		{
			char msg[sizeof(buff) + 64];
			string pg_version = string(buff);
			bool version_compare_non = CompareVersion(pg_version)
					== CompareVersion("0");

			bool version_to_low = CompareVersion(pg_version)
					< CompareVersion(minversion);
			if (version_compare_non)
			{
				sprintf(msg,
						"Can not find %s: run 'sudo dnf install %s' before you continue!",
						dependeny.c_str(), dnf_pkg.c_str());
				/* Start capturing the dnf command user should run to
				 * install all the dependencies at once */
				if (dnf_install.empty())
					dnf_install = "sudo dnf install";

				dnf_install = dnf_install + " " + dnf_pkg;

				if (required)
				{
					errors = true;
					output.error(msg, true);
					output.banner(
							"This package is required by: " + required_by);
				}
				else
				{
					warnings = true;
					output.warning(msg, true);
					output.banner(
							"This package is recommended: " + required_by);
				}

			}
			else if (version_to_low)
			{
				warnings = true;
				sprintf(msg,
						"Your %s version (%s) is to low. Compiling php from source it's recomended to have version: x >= %s",
						dependeny.c_str(), buff, minversion.c_str());
				output.warning(msg, true);
				if (required)
				{
					output.banner(
							"This package is required by: " + required_by);
				}
				else
				{
					output.banner(
							"This package is recommended: " + required_by);
				}
			}
			else
			{
				sprintf(msg, "%-25s version is %s", dependeny.c_str(), buff);
				if (!required_by.empty())
					output.ok(
							string(msg) + " "
									+ (required ? "required by" : "recommended")
									+ ": " + required_by, true);
				else
					output.ok(msg, true);
			}
		}
	}
	pclose(in);
	if (errors || (warnings && required))
		return false;

	return true;
}

bool Check::phpbuild_buildconf()
{
	output.task();
	output.info("Checking PHP buildconf...");

	if (!config.conf_storage["php"].isMember("buildconf"))
	{
		output.warning("Buildconf not configured! Run configuration.");
		return false;
	}

	output.ok("PHP buildconf parameters are already configured!", true);
	return true;
}
bool Check::phpbuild_configure()
{
	output.task();
	output.info("Checking PHP ./configure flags...");

	if (!config.conf_storage["php"].isMember("configure")
			|| config.conf_storage["php"]["configure"].empty())
	{
		output.warning("configure not configured! Run configuration.");
		return false;
	}

	output.ok("PHP ./configure flags are already configured!", true);
	return true;
}
bool Check::rsync_releases()
{
	output.task();
	output.info("Fetching available php releases!");

	if (!config.conf_storage.isMember("paths"))
	{
		output.info("Skipping releases check, howiphp is not configured!");
		return false;
	}

	char buffer[256];
	char cmd_output[32];
	string releases_file = config.conf_storage["paths"]["lib_path"].asString()
			+ "/php/releases.txt";
	sprintf(buffer,
			"rsync --list-only --include='**.tar.gz' --exclude='*' americas.rsync.php.net::phpweb/distributions/ | awk '{ $1=$2=$3=$4=\"\"; print substr($0,5);}' > %s | exit;",
			releases_file.c_str());
	fflush(stdout);
	FILE *p = popen(buffer, "r");
	fgets(cmd_output, sizeof(buffer), p);
	pclose(p);

	path releases_file_path = releases_file;
	if (exists(releases_file_path))
	{
		boost::filesystem::ifstream releases;
		releases.open(releases_file_path);

		int i = 0;
		while (!releases.eof())
		{
			string line;
			getline(releases, line);

			string versionnr = line.erase(0, 4);
			const string ext(".tar.gz");
			if (versionnr != ext && versionnr.size() > ext.size()
					&& versionnr.substr(versionnr.size() - ext.size())
							== ".tar.gz")
			{
				versionnr = versionnr.substr(0, versionnr.size() - ext.size());
			}
			if (!versionnr.empty())
			{
				config.conf_storage["php"]["releases"][versionnr] = line;

			}
			i++;
		}
		releases.close();
		output.table_row(i, "total", "php releases available");
		output.ok("Checking available PHP releases done!", true);
		return true;
	}
	else
	{
		output.warning("Failed to sync release files!", true);
		return false;
	}

}
bool Check::gitsrc()
{
	output.task();
	output.info("Checking php-src Git repository!");

	if (!config.conf_storage.isMember("paths"))
	{
		output.info("Skipping releases check, howiphp is not configured!");
		return false;
	}

	string pathname = config.conf_storage["paths"]["lib_php_path"].asString()
			+ "/git-src/.git";
	if (!file_exist(pathname))
	{
		output.info(
				"Git repo is not checked out! Run configure and in step checkout git master to build php from git TAGS.",
				true);
		return false;
	}
	else
	{
		output.ok("Checking php-src Git repository done!", true);
		return true;
	}
}
bool Check::gittags()
{
	output.task();
	output.info("Checking git tags...");
	if (gitsrc())
	{
		GitWorker gitworker;
		gitworker.tags();
		output.ok("Checkking git tags done!", true);
		return true;
	}
	output.warning(
			"Can not check git tags since there is no source repository!");
	return false;
}

bool Check::hasErrors()
{
	return errors;
}

void Check::setErrors(bool errors)
{
	this->errors = errors;
}

bool Check::hasWarnings()
{
	return warnings;
}

void Check::setWarnings(bool warnings)
{
	this->warnings = warnings;
}
void Check::resetStates()
{
	setErrors(false);
	setWarnings(false);
}

inline bool Check::file_exist(const string& name)
{
	std::ifstream file(name);
	return (!file) ? false : true;
}

Check::~Check()
{
}
}
