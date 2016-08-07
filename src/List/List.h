/*
 * ******************************************************************
 * Created by   Marko Kungla on 2010
 * Updated by   Marko Kungla @ Okram Labs on Aug 6, 2016 - 6:16:29 PM
 * Contact      okramlabs@gmail.com - https://okramlabs.com
 * @copyright   2010 - 2016 Marko Kungla - https://github.com/mkungla
 * @license     MIT
 *
 * Package name    howiphp
 * @category       HOWI3
 * @package        howiphp
 *
 * File         List.h
 * @link        https://github.com/okramlabs/howiphp
 * ********************************************************************
 * Comments:
 * *******************************************************************/
#ifndef HOWIPHP_LIST_H_
#define HOWIPHP_LIST_H_

#include "../howiphp.h"

namespace howiphp
{
class List
{
public:
	List();
	void listAll(int, int, int);
	virtual ~List();
};
}

#endif /* HOWIPHP_LIST_H_ */
