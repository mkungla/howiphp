/*
 * ******************************************************************
 * Created by   Marko Kungla on 2010
 * Updated by   Marko Kungla on Aug 6, 2016 - 6:10:18 PM
 *
 * @copyright   2010 - 2016 Marko Kungla - https://github.com/mkungla
 * @license     MIT
 *
 * Package name    howiphp
 * @category       HOWI3
 * @package        howiphp
 *
 * File         CompareVersion.h
 * @link        https://github.com/mkungla/howiphp
 * ********************************************************************
 * Comments:
 * *******************************************************************/
#ifndef HOWIPHP_COMPAREVERSION_H_
#define HOWIPHP_COMPAREVERSION_H_

#include "../howiphp.h"

namespace howiphp
{
class CompareVersion
{

public:
	int maj;
	int min;
	int rev;
	int build;
	string full_version;

	CompareVersion(std::string);

	bool operator <(const CompareVersion&);

	bool operator <=(const CompareVersion&);

	bool operator ==(const CompareVersion&);

	friend ostream& operator <<(ostream& stream, const CompareVersion& ver)
	{
		stream << ver.maj;
		stream << '.';
		stream << ver.min;
		stream << '.';
		stream << ver.rev;
		stream << '.';
		stream << ver.build;
		return stream;
	}
	;
	void reset();
	virtual ~CompareVersion();
};
}

#endif /* HOWIPHP_COMPAREVERSION_H_ */
