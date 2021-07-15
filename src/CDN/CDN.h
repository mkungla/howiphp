/*
 * ******************************************************************
 * Created by   Marko Kungla on 2010
 * Updated by   Marko Kungla on Aug 6, 2016 - 6:04:57 PM
 *
 * @copyright   2010 - 2016 Marko Kungla - https://github.com/mkungla
 * @license     MIT
 *
 * Package name    howiphp
 * @category       HOWI3
 * @package        howiphp
 *
 * File         CDN.h
 * @link        https://github.com/mkungla/howiphp
 * ********************************************************************
 * Comments:
 * *******************************************************************/
#ifndef HOWIPHP_CDN_H_
#define HOWIPHP_CDN_H_

#ifndef BUF_SIZE
#define BUF_SIZE 500
#endif

#include "../howiphp.h"
#include <netdb.h>
#include <arpa/inet.h>

namespace howiphp
{
/**
 * Class to determine speed of CDN's
 */
class CDN
{
public:

	/// Hostname of CDN
	HostName host;

	/// Protocol used for connection ex: http / https
	Protocol proto;

	/// Path used for download sample file
	SampleFilePath path;

	/// Download speed x.x MB/s | KB/s etc
	DownloadSpeed dl_speed;

	/// dl_speed converted to kbs
	Kbs kbs;

	/// dl_speed converted to mbs
	Mbs mbs;

	/// Second download took
	Seconds sec;

	/// Miliseconds download took
	Milliseconds ms;

	/// Ping to host
	Ping ping;

	/// Construct CDN
	CDN(Protocol, HostName, SampleFilePath, bool = false);

	/// Resolve ip address by hostname
	IPaddress resolvehostip();

	/* Download something to /dev/null/ and mesure download time */
	void get_download_speed();

	/// Ping to host
	Ping get_ping();

	/// Compare using sample file download speed
	bool operator <(const CDN&);

	///
	virtual ~CDN();
};
}

#endif /* HOWIPHP_CDN_H_ */
