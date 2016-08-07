/*
 * ******************************************************************
 * Created by   Marko Kungla on 2010
 * Updated by   Marko Kungla @ Okram Labs on Aug 6, 2016 - 6:19:53 PM
 * Contact      okramlabs@gmail.com - https://okramlabs.com
 * @copyright   2010 - 2016 Marko Kungla - https://github.com/mkungla
 * @license     MIT
 *
 * Package name    howiphp
 * @category       HOWI3
 * @package        howiphp
 *
 * File         ProgressBar.cpp
 * @link        https://github.com/okramlabs/howiphp
 * ********************************************************************
 * Comments:
 * *******************************************************************/
#include "ProgressBar.h"

namespace howiphp
{
int ProgressBar::totaltasks = 0;
int ProgressBar::barspace = 0;

ProgressBar::ProgressBar()
{
	barlenght = 0.0;
	percentage = 0.0;
	state = 0;
}
void ProgressBar::setUp(int settasks, int mid_block_strlen)
{

	barlenght = 0.0;
	percentage = 0.0;
	state = 0;
	totaltasks = settasks;
	barspace = mid_block_strlen;
}
void ProgressBar::task()
{
	state++;
	percentage = (100.00 / totaltasks) * state;
	barlenght = (barspace / 100.00) * percentage;
}
double ProgressBar::getPercentage()
{
	return percentage;
}
double ProgressBar::getBarLenght()
{
	return barlenght;
}
bool ProgressBar::isValid()
{
	if (!totaltasks || (int) percentage > 100)
		return false;
	else
		return true;
}
void ProgressBar::reset()
{
	state = 0;
	totaltasks = 0;
	percentage = 0;
	barlenght = 0.0;
}
ProgressBar::~ProgressBar()
{
	// TODO Auto-generated destructor stub
}
}
