/*
 * ******************************************************************
 * Created by   Marko Kungla on 2010
 * Updated by   Marko Kungla on Aug 6, 2016 - 6:19:29 PM
 *
 * @copyright   2010 - 2016 Marko Kungla - https://github.com/mkungla
 * @license     MIT
 *
 * Package name    howiphp
 * @category       HOWI3
 * @package        howiphp
 *
 * File         ProgressBar.h
 * @link        https://github.com/mkungla/howiphp
 * ********************************************************************
 * Comments:
 * *******************************************************************/
#ifndef HOWIPHP_PROGRESSBAR_H_
#define HOWIPHP_PROGRESSBAR_H_

namespace howiphp
{
class ProgressBar
{
public:

	static int totaltasks;

	static int barspace;

	int state;

	double percentage;

	double barlenght;

	ProgressBar();

	void setUp(int, int);

	void task();

	double getPercentage();

	double getBarLenght();

	bool isValid();

	void reset();

	virtual ~ProgressBar();
};
}

#endif /* HOWIPHP_PROGRESSBAR_H_ */
