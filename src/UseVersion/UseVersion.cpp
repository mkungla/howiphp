/*
 * ******************************************************************
 * Created by   Marko Kungla on 2010
 * Updated by   Marko Kungla @ Okram Labs on Aug 6, 2016 - 6:20:40 PM
 * Contact      okramlabs@gmail.com - https://okramlabs.com
 * @copyright   2010 - 2016 Marko Kungla - https://github.com/mkungla
 * @license     MIT
 *
 * Package name    howiphp
 * @category       HOWI3
 * @package        howiphp
 *
 * File         UseVersion.cpp
 * @link        https://github.com/okramlabs/howiphp
 * ********************************************************************
 * Comments:
 * *******************************************************************/
#include "UseVersion.h"
namespace howiphp
{
path UseVersion::installation_path;
path UseVersion::bin_path;
path UseVersion::sbin_path;

UseVersion::UseVersion()
{
	// TODO Auto-generated constructor stub

}
void UseVersion::load(string version_to_switch)
{
	if (version_to_switch.empty())
	{
		output.error("Usage: howiphp use <version> ; ex hphp use php 5.4.0");
		output.close();
	}

	else if (config.conf_storage["php"]["versions"].isMember(version_to_switch))
	{
		output.task();
		output.info("Switching to php version: " + version_to_switch);

		installation_path = path(
				config.conf_storage["paths"]["lib_php_path"].asString()).append(
				"versions").append(version_to_switch);
		bin_path = path(config.conf_storage["paths"]["bin_path"].asString());
		sbin_path = path(
				config.conf_storage["paths"]["lib_sbin_path"].asString());

		if (exists(installation_path) && update_bin_symlink("php"))
		{
			update_bin_symlink("php-cgi");
			update_bin_symlink("php-config");
			update_bin_symlink("phar.phar");
			update_bin_symlink("phar.phar", "phar");
			update_bin_symlink("phpdbg");
			update_bin_symlink("phpize");
			update_sbin_symlink("php-fpm");

			/* Update symlinks to Xdebug */
			update_extension_symlink("xdebug.so");

			output.ok("Switched to php versions: " + version_to_switch, true);
		}
		else
			output.error(
					"Seems that your php installation is corrupted or you found a bug!");

		output.close();
	}
	else
	{
		output.task();
		output.warning(
				"Switching to php version: " + version_to_switch
						+ " failed version not installed!");
		output.close();
	}
}
bool UseVersion::update_extension_symlink(string linkname, string newname)
{
	/**
	 * Get / set installation path of php version which user attends to switch.
	 */
	path sourcefile = installation_path;
	sourcefile.append("usr/local/lib/php/extensions/" + linkname);
	/**
	 * Get / set destination of symbolic link
	 */
	path destlink = config.conf_storage["paths"]["php_extensions"].asString();
	if (!exists(destlink))
		return false;

	newname.empty() ? destlink.append(linkname) : destlink.append(newname);

	/**
	 * We remove old link anyway since
	 * otherwise we can end up with links from different sources
	 */
	if (symbolic_link_exists(destlink))
		remove(destlink);

	/**
	 * Now link to previously used php version is removed
	 * we check can new one be made, so does PHP version user attends to
	 * use have such binary.
	 */
	if (!exists(sourcefile))
		return false;

	/**
	 * So source should be ok and old link removed so lets make new one
	 */
	create_symlink(sourcefile, destlink);
	return true;
}
bool UseVersion::update_bin_symlink(string linkname, string newname)
{
	/**
	 * Get / set installation path of php version which user attends to switch.
	 */
	path sourcefile = installation_path;
	sourcefile.append("bin/" + linkname);
	/**
	 * Get / set destination of symbolic link
	 */
	path destlink = bin_path;
	if (!exists(destlink))
		return false;

	newname.empty() ? destlink.append(linkname) : destlink.append(newname);

	/**
	 * We remove old link anyway since
	 * otherwise we can end up with links from different sources
	 */
	if (symbolic_link_exists(destlink))
		remove(destlink);

	/**
	 * Now link to previously used php version is removed
	 * we check can new one be made, so does PHP version user attends to
	 * use have such binary.
	 */
	if (!exists(sourcefile))
		return false;

	/**
	 * So source should be ok and old link removed so lets make new one
	 */
	create_symlink(sourcefile, destlink);
	return true;
}
bool UseVersion::update_sbin_symlink(string linkname, string newname)
{
	/**
	 * Get / set installation path of php version which user attends to switch.
	 */
	path sourcefile = installation_path;
	sourcefile.append("sbin/" + linkname);
	if (!exists(sourcefile))
		return false;
	/**
	 * Get / set destination of symbolic link
	 */
	path destlink = sbin_path;
	if (!exists(destlink))
		return false;

	newname.empty() ? destlink.append(linkname) : destlink.append(newname);

	/**
	 * We remove old link anyway since
	 * otherwise we can end up with links from different sources
	 */
	if (symbolic_link_exists(destlink))
		remove(destlink);

	/**
	 * Now link to previously used php version is removed
	 * we check can new one be made, so does PHP version user attends to
	 * use have such binary.
	 */
	if (!exists(sourcefile))
		return false;

	/**
	 * So source should be ok and old link removed so lets make new one
	 */
	create_symlink(sourcefile, destlink);
	return true;
}
}
