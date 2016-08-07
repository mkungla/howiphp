/*
 * ******************************************************************
 * Created by   Marko Kungla on 2010
 * Updated by   Marko Kungla @ Okram Labs on Aug 6, 2016 - 6:13:52 PM
 * Contact      okramlabs@gmail.com - https://okramlabs.com
 * @copyright   2010 - 2016 Marko Kungla - https://github.com/mkungla
 * @license     MIT
 *
 * Package name    howiphp
 * @category       HOWI3
 * @package        howiphp
 *
 * File         Dependencies.h
 * @link        https://github.com/okramlabs/howiphp
 * ********************************************************************
 * Comments:
 * *******************************************************************/
#ifndef HOWIPHP_DEPENDENCIES_H_
#define HOWIPHP_DEPENDENCIES_H_

#include "../howiphp.h"

namespace howiphp
{
class Dependencies
{
public:
	Dependencies();
	/**
	 * Dependencies required by libhowi-php
	 * */
	Json::Value system();

	/**
	 * Dependencies recommended by php.net
	 */
	Json::Value php_net();

	/**
	 * Basic dependencies to compile php
	 */
	Json::Value compiler();

	/**
	 * Optional dependencies required by
	 * php extesions and build variants
	 */
	Json::Value optional();

	virtual ~Dependencies();
};
}

#endif /* HOWIPHP_DEPENDENCIES_H_ */
