/*
 * ******************************************************************
 * Created by   Marko Kungla on 2010
 * Updated by   Marko Kungla on Aug 6, 2016 - 6:20:20 PM
 *
 * @copyright   2010 - 2016 Marko Kungla - https://github.com/mkungla
 * @license     MIT
 *
 * Package name    howiphp
 * @category       HOWI3
 * @package        howiphp
 *
 * File         UseVersion.h
 * @link        https://github.com/mkungla/howiphp
 * ********************************************************************
 * Comments:
 * *******************************************************************/
#ifndef SHOWIPHP_USEVERSION_H_
#define HOWIPHP_USEVERSION_H_

#include "../howiphp.h"

namespace howiphp
{
class UseVersion
{
public:

	UseVersion();

	static path installation_path;

	static path bin_path;

	static path sbin_path;

	bool update_extension_symlink(string, string = "");

	bool update_bin_symlink(string, string = "");

	bool update_sbin_symlink(string, string = "");

	void load(string);
};
}

#endif /* HOWIPHP_USEVERSION_H_ */
