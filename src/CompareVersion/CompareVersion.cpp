/*
 * ******************************************************************
 * Created by   Marko Kungla on 2010
 * Updated by   Marko Kungla on Aug 6, 2016 - 6:11:01 PM
 *
 * @copyright   2010 - 2016 Marko Kungla - https://github.com/mkungla
 * @license     MIT
 *
 * Package name    howiphp
 * @category       HOWI3
 * @package        howiphp
 *
 * File         CompareVersion.cpp
 * @link        https://github.com/mkungla/howiphp
 * ********************************************************************
 * Comments:
 * *******************************************************************/
#include "CompareVersion.h"

namespace howiphp
{

CompareVersion::CompareVersion(string version)
{
	reset();
	full_version = version;
	if (version.compare(0, 1, ".") == 0)
		version = "0" + version;
	if (version.compare(version.size() - 1, 1, ".") == 0)
		version.append("0");
	sscanf(version.c_str(), "%d.%d.%d.%d", &maj, &min, &rev, &build);
	if (maj <= 0)
		maj = 0;
	if (min <= 0)
		min = 0;
	if (rev <= 0)
		rev = 0;
	if (build <= 0)
		build = 0;

}
bool CompareVersion::operator <(const CompareVersion& other)
{
	if (maj < other.maj)
		return true;
	if (maj <= other.maj && min < other.min)
		return true;
	if (maj <= other.maj && min <= other.min && rev < other.rev)
		return true;
	if (maj <= other.maj && min <= other.min && rev <= other.rev
			&& build < other.build)
		return true;

	return false;
}

bool CompareVersion::operator <=(const CompareVersion& other)
{
	if (maj >= other.maj)
		return true;
	if (min >= other.min)
		return true;
	if (rev >= other.rev)
		return true;
	if (build >= other.build)
		return true;
	return false;
}
bool CompareVersion::operator ==(const CompareVersion& other)
{
	return maj == other.maj && min == other.min && rev == other.rev
			&& build == other.build;
}
void CompareVersion::reset()
{
	maj = 0;
	min = 0;
	rev = 0;
	build = 0;
}
CompareVersion::~CompareVersion()
{
	// TODO Auto-generated destructor stub
}
}
