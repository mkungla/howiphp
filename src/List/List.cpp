/*
 * ******************************************************************
 * Created by   Marko Kungla on 2010
 * Updated by   Marko Kungla on Aug 6, 2016 - 6:17:00 PM
 *
 * @copyright   2010 - 2016 Marko Kungla - https://github.com/mkungla
 * @license     MIT
 *
 * Package name    howiphp
 * @category       HOWI3
 * @package        howiphp
 *
 * File         List.cpp
 * @link        https://github.com/mkungla/howiphp
 * ********************************************************************
 * Comments:
 * *******************************************************************/
#include "List.h"

namespace howiphp
{
List::List()
{
	// TODO Auto-generated constructor stub

}
void List::listAll(int flag_list_installed, int flag_list_available,
		int flag_list_releases)
{
	Json::Value phpinstalables;

	for (Json::ValueConstIterator itr =
			config.conf_storage["php"]["releases"].begin();
			itr != config.conf_storage["php"]["releases"].end(); itr++)
	{
		phpinstalables[itr.name()]["type"] = "release";

	}
	if (!flag_list_releases)
	{
		for (Json::ValueConstIterator itr =
				config.conf_storage["php"]["tags"].begin();
				itr != config.conf_storage["php"]["tags"].end(); itr++)
		{
			if (!phpinstalables.isMember(itr.name()))
				phpinstalables[itr.name()]["type"] = "tag";
		}
	}

	int installed_total = 0;
	int available_total = 0;
	for (Json::ValueConstIterator itr = phpinstalables.begin();
			itr != phpinstalables.end(); itr++)
	{
		bool installed = config.conf_storage["php"]["versions"].isMember(
				itr.name());
		string date;
		if (installed)
		{
			installed_total++;
			date =
					config.conf_storage["php"]["versions"][itr.name()].asString();
		}
		else
			available_total++;

		if (!flag_list_installed && !flag_list_available)
		{
			output.listVersions(itr.name(), (*itr)["type"].asString(),
					installed);
		}
		else if (flag_list_installed && installed)
		{

			output.listVersions(itr.name(), (*itr)["type"].asString(),
					installed, date);
		}
		else if (flag_list_available && !installed)
		{

			output.listVersions(itr.name(), (*itr)["type"].asString(),
					installed);
		}
	}

	char totals[80];
	sprintf(totals, "Total installed %d and total available %d",
			installed_total, available_total);
	output.banner("");
	output.info(string(totals), true);
}
List::~List()
{
	// TODO Auto-generated destructor stub
}
}
