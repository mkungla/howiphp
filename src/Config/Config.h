/*
 * ******************************************************************
 * Created by   Marko Kungla on 2010
 * Updated by   Marko Kungla on Aug 6, 2016 - 6:12:07 PM
 *
 * @copyright   2010 - 2016 Marko Kungla - https://github.com/mkungla
 * @license     MIT
 *
 * Package name    howiphp
 * @category       HOWI3
 * @package        howiphp
 *
 * File         Config.h
 * @link        https://github.com/mkungla/howiphp
 * ********************************************************************
 * Comments:
 * *******************************************************************/
#ifndef HOWIPHP_CONFIG_H_
#define HOWIPHP_CONFIG_H_
#if HAVE_CONFIG_H
# include <config.h>
#endif
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/time.h>

#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <vendor/jsoncpp/dist/json/json.h>
#include <src/Output/Output.h>

#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#undef BOOST_NO_CXX11_SCOPED_ENUMS

using namespace boost::filesystem;
using namespace std;

namespace howiphp
{
class Config
{
public:

	/* Holds stratup time */
	static long int startup_time;

	Config();

	/* Sets startup time */
	static void set_startup_time();

	/* Get workers elapsed time in milliseconds */
	static long get_elapsed_time();

	static Json::Value conf_storage;

	/* load conf */
	void init(int, bool run_config = false);

	/* Start interactive configuration*/
	void configuration();

	void post_configuration(bool = false);

	/* Initialize config defaults ERASES previous configuration
	 * however backup will be made
	 * */
	Json::Value defauts(struct passwd *, std::string, Json::Value, path);

	virtual ~Config();
};
}

#endif /* HOWIPHP_CONFIG_H_ */
