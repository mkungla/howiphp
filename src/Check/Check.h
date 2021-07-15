/*
 * ******************************************************************
 * Created by   Marko Kungla on 2010
 * Updated by   Marko Kungla on Aug 6, 2016 - 6:09:09 PM
 *
 * @copyright   2010 - 2016 Marko Kungla - https://github.com/mkungla
 * @license     MIT
 *
 * Package name    howiphp
 * @category       HOWI3
 * @package        howiphp
 *
 * File         Check.h
 * @link        https://github.com/mkungla/howiphp
 * ********************************************************************
 * Comments:
 * *******************************************************************/
#ifndef HOWIPHP_CHECK_H_
#define HOWIPHP_CHECK_H_

#include "../howiphp.h"
#include <boost/lexical_cast.hpp>
#include "../CDN/CDN.h"
#include "../CompareVersion/CompareVersion.h"
#include "../Dependencies/Dependencies.h"
#include "../GitWorker/GitWorker.h"

namespace howiphp
{
class Check
{
public:

	static bool errors;
	static bool warnings;
	static string dnf_install;
	Check();

	/* Check the user environment path variable */
	static bool envpath();

	/* Check is configuration done */
	bool isConfigured(FlagInt = 0);

	/* Check system dependencies */
	bool system();

	/* Check Dependencies recommended by php.net */
	bool php_net();

	/* Check Basic dependencies to compile php  */
	bool compiler();

	/** Check Optional dependencies required by php extesions and build variants*/
	bool optional();

	/* Check repos */
	bool gitrepos(FlagInt = 0);

	/* Check individual git repository */
	bool gitrepo(ID, RepoName, IPaddress, HostName, RepoRemoteUrl, Protocol,
			FlagInt = 0);

	/* Mirrors for tarballs */
	bool mirrors(FlagInt = 0);

	/* Mirror */
	bool mirror(ID, HostName, Country, ProviderName, IPaddress, Protocol,
			FlagInt = 0);

	/* Check fastest mirrors */
	bool fastest_mirrors(FlagInt = 0);

	/* Check dependencies */
	bool dependency(PackageName, VersionName, PackageManagerName, Required =
			false, PackageName = "");

	/* Check build configuration */
	bool phpbuild_buildconf();

	/* PHP build ./configure */
	bool phpbuild_configure();

	/* Get releases from the mirror */
	bool rsync_releases();

	/* Check git src */
	bool gitsrc();

	/* Check git tags */
	bool gittags();

	static bool hasErrors();

	void setErrors(bool errors);

	static bool hasWarnings();

	void setWarnings(bool warnings);

	void resetStates();

	inline bool file_exist(const string&);

	virtual ~Check();
};
}

#endif /* HOWIPHP_CHECK_H_ */
