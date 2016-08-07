/*
 * ******************************************************************
 * Created by   Marko Kungla on 2010
 * Updated by   Marko Kungla @ Okram Labs on Aug 6, 2016 - 6:05:44 PM
 * Contact      okramlabs@gmail.com - https://okramlabs.com
 * @copyright   2010 - 2016 Marko Kungla - https://github.com/mkungla
 * @license     MIT
 *
 * Package name    howiphp
 * @category       HOWI3
 * @package        howiphp
 *
 * File         CDN.cpp
 * @link        https://github.com/okramlabs/howiphp
 * ********************************************************************
 * Comments:
 * *******************************************************************/
#include "CDN.h"

namespace howiphp
{

CDN::CDN(Protocol p, HostName hn, SampleFilePath sfp, bool check_dl)
{
	proto = p;
	host = hn;
	path = sfp;
	ms = sec = mbs = kbs = ping = 0;
	if (check_dl)
		get_download_speed();
}

/**
 * Get ping of the host
 * TODO Issue #11 FIX that up
 * This function is so dirty!
 *
 * @return Ping ping
 */
Ping CDN::get_ping()
{
	char buffer[256];
	char read_ping[32];
	sprintf(buffer,
			"ping -c5 -ABp ff -s1016 %s | gawk 'BEGIN {FS=\"[=]|[ ]\"} NR==11 {print $8}' | cut -d'/' -f2",
			host.c_str());
	fflush(stdout);
	FILE *p = popen(buffer, "r");
	fgets(read_ping, sizeof(buffer), p);
	read_ping[strcspn(read_ping, "\n")] = 0;
	pclose(p);
	ping = atof(read_ping);
	return (ping);
}

/**
 * Compare CDN's based on their download speed
 *
 * Here we compare times taken to download sample file
 * @return bool
 */
bool CDN::operator <(const CDN& other)
{
	return (bool(ms < other.ms));
}

/**
 * Get IPv4 address by hostname
 */
IPaddress CDN::resolvehostip()
{
	/* register the task */
	output.task();

	/* Local vars */
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	int sfd, s;
	IPaddress ip4addr;

	/* Obtain address(es) matching host/port */
	memset(&hints, 0, sizeof(struct addrinfo));

	/* Allow IPv4 or IPv6 */
	hints.ai_family = AF_UNSPEC;

	/* Datagram socket */
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = 0;

	/* Any protocol */
	hints.ai_protocol = 0;

	/* getaddrinfo */
	s = getaddrinfo(host.c_str(), proto.c_str(), &hints, &result);

	if (s != 0)
	{
		output.error("Could not resolve " + host);
		return (IPaddress());
	}

	/* Loop the result */
	for (rp = result; rp != NULL; rp = rp->ai_next)
	{
		sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (sfd == -1)
		{
			output.error("Could not connect with (" + proto + ") protocol");
			return (IPaddress());
			continue;
		}
		if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
		{
			struct sockaddr_in *addr;
			addr = (struct sockaddr_in *) rp->ai_addr;
			ip4addr = IPaddress(inet_ntoa((struct in_addr) addr->sin_addr));
			output.ok(
					"Successfully resolved " + host + " and connected to IP: "
							+ ip4addr);
			break;
		}
		close(sfd);
	}
	freeaddrinfo(result);

	/* We got IP address, but can we connect to it?! */
	if (rp == NULL)
	{
		output.error(
				"Resolved IP: " + ip4addr + " for " + host
						+ ", but connection to this IP failed!");
		return (IPaddress());
	}
	return (ip4addr);
}

/**
 * Get CDN's download speed
 * TODO Issue #12 wget!?!
 * Clean up this dirty stuff and implement socket HTTP GET
 *
 * This function should measure actual time download takes not including
 * all the overhead!
 *
 * set:
 * DownloadSpeed dl_speed
 * Kbs kbs
 * Mbs mbs
 * Milliseconds ms (download elapsed)
 * Seconds sec (download elapsed)
 */
void CDN::get_download_speed()
{
	/* DIRTY CODE START */
	struct timeval dl_started;
	gettimeofday(&dl_started, NULL);
	long int download_start = ((unsigned long long) dl_started.tv_sec * 1000000)
			+ dl_started.tv_usec;
	char buffer[256];
	char cmd_output[32];
	// Issue #12
	sprintf(buffer,
			"wget -O /dev/null --tries=1 --timeout=2 --no-dns-cache --no-cache %s://%s/%s 2>&1 | grep -o --color=never \"[0-9.]\\+ [KM]*B/s\"",
			proto.c_str(), host.c_str(), path.c_str());
	fflush(stdout);
	FILE *p = popen(buffer, "r");

	fgets(cmd_output, sizeof(buffer), p);
	cmd_output[strcspn(cmd_output, "\n")] = 0;
	pclose(p);

	dl_speed = DownloadSpeed(cmd_output);
	struct timeval download_ended;
	gettimeofday(&download_ended, NULL);
	long int download_end = ((unsigned long long) download_ended.tv_sec
			* 1000000) + download_ended.tv_usec;

	size_t output_type_k = dl_speed.find("KB/s");
	size_t output_type_m = dl_speed.find("MB/s");

	if (output_type_k != string::npos)
	{
		string dl_bytes = dl_speed.substr(0, output_type_k - 1);
		double dl_mb = atof(dl_bytes.c_str()) / 1000;
		kbs = atof(dl_bytes.c_str());
		mbs = dl_mb;

	}
	else if (output_type_m != string::npos)
	{
		string dl_bytes = dl_speed.substr(0, output_type_m - 1);
		double dl_kb = atof(dl_bytes.c_str()) * 1000;
		kbs = dl_kb;
		mbs = atof(dl_bytes.c_str());
	}
	else
	{
		dl_speed = "unknown";
		output.warning("Could not determine download speed! " + dl_speed, true);
	}

	ms = download_end - download_start;
	sec = ((float) ms) / CLOCKS_PER_SEC;
	/* DIRTY CODE END */
}
CDN::~CDN()
{
}
}
