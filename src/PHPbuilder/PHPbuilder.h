/*
 * ******************************************************************
 * Created by   Marko Kungla on 2010
 * Updated by   Marko Kungla @ Okram Labs on Aug 6, 2016 - 6:18:20 PM
 * Contact      okramlabs@gmail.com - https://okramlabs.com
 * @copyright   2010 - 2016 Marko Kungla - https://github.com/mkungla
 * @license     MIT
 *
 * Package name    howiphp
 * @category       HOWI3
 * @package        howiphp
 *
 * File         PHPbuilder.h
 * @link        https://github.com/okramlabs/howiphp
 * ********************************************************************
 * Comments:
 * *******************************************************************/
#ifndef HOWIPHP_PHPBUILDER_H_
#define HOWIPHP_PHPBUILDER_H_

#include "../howiphp.h"
#include "../Check/Check.h"

namespace howiphp
{
class PHPbuilder
{
public:

	static path installation_path;

	static path build_path;

	static path local_downloaded_file;

	static string current_build_version;

	static path config_file_path;

	PHPbuilder();

	void buildconf();

	void configure();

	void remove(string);

	void install(string);

	void install_release(string);

	void install_tag(string);

	void cleanup();

	void check_php_ini();

	void prepare(string);

	void download_archive(string, string, string, string);

	void download_tag(string, string, string, string);

	void extract_archive();

	void run_buildconf();

	void run_configure();

	void make();

	void make_test();

	void make_install();

	virtual ~PHPbuilder();
};
}

#endif /* HOWIPHP_PHPBUILDER_H_ */
