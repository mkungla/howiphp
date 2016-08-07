/*
 * ******************************************************************
 * Created by   Marko Kungla on 2010
 * Updated by   Marko Kungla @ Okram Labs on Aug 6, 2016 - 6:14:55 PM
 * Contact      okramlabs@gmail.com - https://okramlabs.com
 * @copyright   2010 - 2016 Marko Kungla - https://github.com/mkungla
 * @license     MIT
 *
 * Package name    howiphp
 * @category       HOWI3
 * @package        howiphp
 *
 * File         Dependencies.cpp
 * @link        https://github.com/okramlabs/howiphp
 * ********************************************************************
 * Comments:
 * *******************************************************************/
#include "Dependencies.h"

namespace howiphp
{
Dependencies::Dependencies()
{
	// TODO Auto-generated constructor stub

}
Json::Value Dependencies::system()
{
	Json::Value system_deps;

	/* DNF */
	system_deps[0]["dependeny"] = "dnf";
	system_deps[0]["minversion"] = "1";
	system_deps[0]["dnf_pkg"] = "dnf";
	system_deps[0]["required"] = false;
	system_deps[0]["required_by"] = "system package manager";

	/* DNF */
	system_deps[1]["dependeny"] = "rpm";
	system_deps[1]["minversion"] = "4";
	system_deps[1]["dnf_pkg"] = "rpm";
	system_deps[1]["required"] = false;
	system_deps[1]["required_by"] = "RPM manager";

	/* boost-filesystem */
	system_deps[2]["dependeny"] = "boost-filesystem";
	system_deps[2]["minversion"] = "1.58";
	system_deps[2]["dnf_pkg"] = "boost-filesystem";
	system_deps[2]["required"] = true;
	system_deps[2]["required_by"] = "libhowi-php";

	/* libgit2 */
	system_deps[3]["dependeny"] = "libgit2";
	system_deps[3]["minversion"] = "0.23";
	system_deps[3]["dnf_pkg"] = "libgit2";
	system_deps[3]["required"] = true;
	system_deps[3]["required_by"] = "libhowi-php";

	/* http-parser */
	system_deps[4]["dependeny"] = "http-parser";
	system_deps[4]["minversion"] = "2";
	system_deps[4]["dnf_pkg"] = "http-parser";
	system_deps[4]["required"] = true;
	system_deps[4]["required_by"] = "libhowi-php";

	/* git */
	system_deps[5]["dependeny"] = "git";
	system_deps[5]["minversion"] = "2.5";
	system_deps[5]["dnf_pkg"] = "git";
	system_deps[5]["required"] = true;
	system_deps[5]["required_by"] = "libhowi-php";

	system_deps[6]["dependeny"] = "rsync";
	system_deps[6]["minversion"] = "0";
	system_deps[6]["dnf_pkg"] = "rsync";
	system_deps[6]["required"] = true;
	system_deps[6]["required_by"] = "libhowi-php";

	/* system_deps[0]["dependeny"] 	= "";
	 system_deps[0]["minversion"] 	= "";
	 system_deps[0]["dnf_pkg"] 		= "";
	 system_deps[0]["required"] 		= true;
	 system_deps[0]["required_by"] 	= ""; */
	return system_deps;
}

Json::Value Dependencies::php_net()
{
	Json::Value php_net_deps;

	/* Autoconf */
	php_net_deps[0]["dependeny"] = "autoconf";
	php_net_deps[0]["minversion"] = "2.13";
	php_net_deps[0]["dnf_pkg"] = "autoconf";
	php_net_deps[0]["required"] = false;
	php_net_deps[0]["required_by"] = "php.net";

	/* automake */
	php_net_deps[1]["dependeny"] = "automake";
	php_net_deps[1]["minversion"] = "1.4";
	php_net_deps[1]["dnf_pkg"] = "automake";
	php_net_deps[1]["required"] = false;
	php_net_deps[1]["required_by"] = "php.net";

	/* m4 */
	php_net_deps[2]["dependeny"] = "m4";
	php_net_deps[2]["minversion"] = "1.4";
	php_net_deps[2]["dnf_pkg"] = "m4";
	php_net_deps[2]["required"] = false;
	php_net_deps[2]["required_by"] = "php.net";

	/* perl-Thread-Queue */
	php_net_deps[3]["dependeny"] = "perl-Thread-Queue";
	php_net_deps[3]["minversion"] = "3";
	php_net_deps[3]["dnf_pkg"] = "perl-Thread-Queue";
	php_net_deps[3]["required"] = false;
	php_net_deps[3]["required_by"] = "php.net";

	/* libtool */
	php_net_deps[4]["dependeny"] = "libtool";
	php_net_deps[4]["minversion"] = "2";
	php_net_deps[4]["dnf_pkg"] = "libtool";
	php_net_deps[4]["required"] = false;
	php_net_deps[4]["required_by"] = "php.net";

	/* re2c */
	php_net_deps[5]["dependeny"] = "re2c";
	php_net_deps[5]["minversion"] = "0.13.4";
	php_net_deps[5]["dnf_pkg"] = "re2c";
	php_net_deps[5]["required"] = false;
	php_net_deps[5]["required_by"] = "php.net";

	/* flex */
	php_net_deps[6]["dependeny"] = "flex";
	php_net_deps[6]["minversion"] = "2.5.4";
	php_net_deps[6]["dnf_pkg"] = "flex";
	php_net_deps[6]["required"] = false;
	php_net_deps[6]["required_by"] = "php.net";

	/* bison */
	php_net_deps[7]["dependeny"] = "bison";
	php_net_deps[7]["minversion"] = "2.4.0";
	php_net_deps[7]["dnf_pkg"] = "bison";
	php_net_deps[7]["required"] = false;
	php_net_deps[7]["required_by"] = "php.net";

	/* php_net_deps[0]["dependeny"] 	= "";
	 php_net_deps[0]["minversion"] 	= "";
	 php_net_deps[0]["dnf_pkg"] 		= "";
	 php_net_deps[0]["required"] 	= false;
	 php_net_deps[0]["required_by"] 	= "php.net"; */

	return php_net_deps;
}
Json::Value Dependencies::compiler()
{
	Json::Value compiler_deps;

	compiler_deps[0]["dependeny"] = "binutils";
	compiler_deps[0]["minversion"] = "2.25";
	compiler_deps[0]["dnf_pkg"] = "binutils";
	compiler_deps[0]["required"] = true;
	compiler_deps[0]["required_by"] = "compiler";

	compiler_deps[1]["dependeny"] = "cpp";
	compiler_deps[1]["minversion"] = "5.3";
	compiler_deps[1]["dnf_pkg"] = "cpp";
	compiler_deps[1]["required"] = true;
	compiler_deps[1]["required_by"] = "compiler";

	compiler_deps[2]["dependeny"] = "gcc";
	compiler_deps[2]["minversion"] = "5.3";
	compiler_deps[2]["dnf_pkg"] = "gcc";
	compiler_deps[2]["required"] = true;
	compiler_deps[2]["required_by"] = "compiler";

	compiler_deps[3]["dependeny"] = "gcc-c++";
	compiler_deps[3]["minversion"] = "5.3";
	compiler_deps[3]["dnf_pkg"] = "gcc-c++";
	compiler_deps[3]["required"] = true;
	compiler_deps[3]["required_by"] = "compiler";

	compiler_deps[4]["dependeny"] = "libstdc++-devel";
	compiler_deps[4]["minversion"] = "5.3";
	compiler_deps[4]["dnf_pkg"] = "libstdc++-devel";
	compiler_deps[4]["required"] = true;
	compiler_deps[4]["required_by"] = "compiler";

	compiler_deps[5]["dependeny"] = "glibc-devel";
	compiler_deps[5]["minversion"] = "2.22";
	compiler_deps[5]["dnf_pkg"] = "glibc-devel";
	compiler_deps[5]["required"] = true;
	compiler_deps[5]["required_by"] = "compiler";

	compiler_deps[6]["dependeny"] = "glibc-headers";
	compiler_deps[6]["minversion"] = "2.22";
	compiler_deps[6]["dnf_pkg"] = "glibc-headers";
	compiler_deps[6]["required"] = true;
	compiler_deps[6]["required_by"] = "compiler";

	compiler_deps[7]["dependeny"] = "glibc-common";
	compiler_deps[7]["minversion"] = "2.22";
	compiler_deps[7]["dnf_pkg"] = "glibc-common";
	compiler_deps[7]["required"] = true;
	compiler_deps[7]["required_by"] = "compiler";

	compiler_deps[8]["dependeny"] = "isl";
	compiler_deps[8]["minversion"] = "0.14";
	compiler_deps[8]["dnf_pkg"] = "isl";
	compiler_deps[8]["required"] = true;
	compiler_deps[8]["required_by"] = "compiler";

	compiler_deps[9]["dependeny"] = "kernel-headers";
	compiler_deps[9]["minversion"] = "0";
	compiler_deps[9]["dnf_pkg"] = "kernel-headers";
	compiler_deps[9]["required"] = true;
	compiler_deps[9]["required_by"] = "compiler";

	compiler_deps[10]["dependeny"] = "libmpc";
	compiler_deps[10]["minversion"] = "1";
	compiler_deps[10]["dnf_pkg"] = "libmpc";
	compiler_deps[10]["required"] = true;
	compiler_deps[10]["required_by"] = "compiler";

	compiler_deps[11]["dependeny"] = "libxml2-devel";
	compiler_deps[11]["minversion"] = "2.9.3";
	compiler_deps[11]["dnf_pkg"] = "libxml2-devel";
	compiler_deps[11]["required"] = true;
	compiler_deps[11]["required_by"] = "compiler";

	compiler_deps[12]["dependeny"] = "xz-devel";
	compiler_deps[12]["minversion"] = "5.2";
	compiler_deps[12]["dnf_pkg"] = "xz-devel";
	compiler_deps[12]["required"] = true;
	compiler_deps[12]["required_by"] = "compiler";

	compiler_deps[13]["dependeny"] = "zlib-devel";
	compiler_deps[13]["minversion"] = "1.2";
	compiler_deps[13]["dnf_pkg"] = "zlib-devel";
	compiler_deps[13]["required"] = true;
	compiler_deps[13]["required_by"] = "compiler";

	compiler_deps[14]["dependeny"] = "make";
	compiler_deps[14]["minversion"] = "4";
	compiler_deps[14]["dnf_pkg"] = "make";
	compiler_deps[14]["required"] = true;
	compiler_deps[14]["required_by"] = "compiler";

	/*
	 compiler_deps[]["dependeny"] 	= "";
	 compiler_deps[]["minversion"] 	= "";
	 compiler_deps[]["dnf_pkg"] 	= "";
	 compiler_deps[]["required"] 	= true;
	 compiler_deps[]["required_by"] = "compiler"; */
	return compiler_deps;
}
Json::Value Dependencies::optional()
{

	Json::Value optional;

	/* httpd-devel */
	optional[0]["dependeny"] = "httpd-devel";
	optional[0]["minversion"] = "2.4";
	optional[0]["dnf_pkg"] = "httpd-devel";
	optional[0]["required"] = false;
	optional[0]["required_by"] = "(--with-apsx)";

	optional[1]["dependeny"] = "apr-devel";
	optional[1]["minversion"] = "1.5";
	optional[1]["dnf_pkg"] = "apr-devel";
	optional[1]["required"] = false;
	optional[1]["required_by"] = "(--with-apsx)";

	optional[2]["dependeny"] = "apr-util-devel";
	optional[2]["minversion"] = "1.5";
	optional[2]["dnf_pkg"] = "apr-util-devel";
	optional[2]["required"] = false;
	optional[2]["required_by"] = "(--with-apsx)";

	optional[3]["dependeny"] = "cyrus-sasl-devel";
	optional[3]["minversion"] = "2.1";
	optional[3]["dnf_pkg"] = "cyrus-sasl-devel";
	optional[3]["required"] = false;
	optional[3]["required_by"] = "(--with-apsx)";

	optional[4]["dependeny"] = "expat-devel";
	optional[4]["minversion"] = "2.1";
	optional[4]["dnf_pkg"] = "expat-devel";
	optional[4]["required"] = false;
	optional[4]["required_by"] = "(--with-apsx)";

	optional[5]["dependeny"] = "libdb-devel";
	optional[5]["minversion"] = "5.3";
	optional[5]["dnf_pkg"] = "libdb-devel";
	optional[5]["required"] = false;
	optional[5]["required_by"] = "(--with-apsx)";

	optional[6]["dependeny"] = "openldap-devel";
	optional[6]["minversion"] = "2.4";
	optional[6]["dnf_pkg"] = "openldap-devel";
	optional[6]["required"] = false;
	optional[6]["required_by"] = "(--with-apsx)";
	/*/ httpd-devel */

	/* systemd-devel */
	optional[7]["dependeny"] = "systemd-devel";
	optional[7]["minversion"] = "222";
	optional[7]["dnf_pkg"] = "systemd-devel";
	optional[7]["required"] = false;
	optional[7]["required_by"] = "(--with-fpm-systemd)";

	optional[8]["dependeny"] = "openssl-devel";
	optional[8]["minversion"] = "1";
	optional[8]["dnf_pkg"] = "openssl-devel";
	optional[8]["required"] = false;
	optional[8]["required_by"] = "(--openssl-devel)";

	optional[9]["dependeny"] = "keyutils-libs-devel";
	optional[9]["minversion"] = "1.5";
	optional[9]["dnf_pkg"] = "keyutils-libs-devel";
	optional[9]["required"] = false;
	optional[9]["required_by"] = "(--openssl-devel)";

	optional[10]["dependeny"] = "krb5-devel";
	optional[10]["minversion"] = "1.14";
	optional[10]["dnf_pkg"] = "krb5-devel";
	optional[10]["required"] = false;
	optional[10]["required_by"] = "(--openssl-devel)";

	optional[11]["dependeny"] = "libcom_err-devel";
	optional[11]["minversion"] = "1.42";
	optional[11]["dnf_pkg"] = "libcom_err-devel";
	optional[11]["required"] = false;
	optional[11]["required_by"] = "(--openssl-devel)";

	optional[12]["dependeny"] = "libselinux-devel";
	optional[12]["minversion"] = "2.4";
	optional[12]["dnf_pkg"] = "libselinux-devel";
	optional[12]["required"] = false;
	optional[12]["required_by"] = "(--openssl-devel)";

	optional[13]["dependeny"] = "libsepol-devel";
	optional[13]["minversion"] = "2.4";
	optional[13]["dnf_pkg"] = "libsepol-devel";
	optional[13]["required"] = false;
	optional[13]["required_by"] = "(--openssl-devel)";

	optional[14]["dependeny"] = "libverto-devel";
	optional[14]["minversion"] = "0.2";
	optional[14]["dnf_pkg"] = "libverto-devel";
	optional[14]["required"] = false;
	optional[14]["required_by"] = "(--openssl-devel)";

	optional[15]["dependeny"] = "pcre-devel";
	optional[15]["minversion"] = "8";
	optional[15]["dnf_pkg"] = "pcre-devel";
	optional[15]["required"] = false;
	optional[15]["required_by"] = "(--openssl-devel)";

	/**
	 * PHP documentation says that sqlite3 does not need ext
	 * libs but sqlite-devel is needed
	 */
	optional[16]["dependeny"] = "sqlite-devel";
	optional[16]["minversion"] = "3.9";
	optional[16]["dnf_pkg"] = "sqlite-devel";
	optional[16]["required"] = false;
	optional[16]["required_by"] = "(--with-sqlite3) ";

	optional[17]["dependeny"] = "bzip2-devel";
	optional[17]["minversion"] = "1";
	optional[17]["dnf_pkg"] = "bzip2-devel";
	optional[17]["required"] = false;
	optional[17]["required_by"] = "(--with-bz2=/usr)";

	optional[18]["dependeny"] = "libcurl-devel";
	optional[18]["minversion"] = "7";
	optional[18]["dnf_pkg"] = "libcurl-devel";
	optional[18]["required"] = false;
	optional[18]["required_by"] = "(--with-curl=/usr)";

	optional[19]["dependeny"] = "libjpeg-turbo-devel";
	optional[19]["minversion"] = "1.4";
	optional[19]["dnf_pkg"] = "libjpeg-turbo-devel";
	optional[19]["required"] = false;
	optional[19]["required_by"] = "(--with-jpeg-dir=/usr)";

	optional[20]["dependeny"] = "libpng-devel";
	optional[20]["minversion"] = "1.6";
	optional[20]["dnf_pkg"] = "libpng-devel";
	optional[20]["required"] = false;
	optional[20]["required_by"] = "(--with-png-dir=/usr)";

	optional[21]["dependeny"] = "libXpm-devel";
	optional[21]["minversion"] = "3.5";
	optional[21]["dnf_pkg"] = "libXpm-devel";
	optional[21]["required"] = false;
	optional[21]["required_by"] = "(--with-xpm-dir=/usr)";

	optional[22]["dependeny"] = "libXau-devel";
	optional[22]["minversion"] = "1";
	optional[22]["dnf_pkg"] = "libXau-devel";
	optional[22]["required"] = false;
	optional[22]["required_by"] = "(--with-xpm-dir=/usr)";

	optional[23]["dependeny"] = "libX11-devel";
	optional[23]["minversion"] = "1.6";
	optional[23]["dnf_pkg"] = "libX11-devel";
	optional[23]["required"] = false;
	optional[23]["required_by"] = "(--with-xpm-dir=/usr)";

	optional[24]["dependeny"] = "libxcb-devel";
	optional[24]["minversion"] = "1.11";
	optional[24]["dnf_pkg"] = "libxcb-devel";
	optional[24]["required"] = false;
	optional[24]["required_by"] = "(--with-xpm-dir=/usr)";

	optional[25]["dependeny"] = "xorg-x11-proto-devel";
	optional[25]["minversion"] = "7";
	optional[25]["dnf_pkg"] = "xorg-x11-proto-devel";
	optional[25]["required"] = false;
	optional[25]["required_by"] = "(--with-xpm-dir=/usr)";

	optional[26]["dependeny"] = "freetype-devel";
	optional[26]["minversion"] = "2.6";
	optional[26]["dnf_pkg"] = "freetype-devel";
	optional[26]["required"] = false;
	optional[26]["required_by"] = "(--with-freetype-dir=/usr)";

	optional[27]["dependeny"] = "gmp-devel";
	optional[27]["minversion"] = "6";
	optional[27]["dnf_pkg"] = "gmp-devel";
	optional[27]["required"] = false;
	optional[27]["required_by"] = "(--with-gmp=/usr)";

	optional[28]["dependeny"] = "uw-imap-devel";
	optional[28]["minversion"] = "2007";
	optional[28]["dnf_pkg"] = "uw-imap-devel";
	optional[28]["required"] = false;
	optional[28]["required_by"] = "(--with-imap)";

	optional[29]["dependeny"] = "libc-client";
	optional[29]["minversion"] = "2007";
	optional[29]["dnf_pkg"] = "libc-client";
	optional[29]["required"] = false;
	optional[29]["required_by"] = "(--with-imap)";

	optional[30]["dependeny"] = "libicu-devel";
	optional[30]["minversion"] = "54";
	optional[30]["dnf_pkg"] = "libicu-devel";
	optional[30]["required"] = false;
	optional[30]["required_by"] = "(--enable-intl)";

	optional[31]["dependeny"] = "openldap-devel";
	optional[31]["minversion"] = "2.4";
	optional[31]["dnf_pkg"] = "openldap-devel";
	optional[31]["required"] = false;
	optional[31]["required_by"] = "(--with-ldap=/usr)";

	optional[32]["dependeny"] = "cyrus-sasl-devel";
	optional[32]["minversion"] = "2.1";
	optional[32]["dnf_pkg"] = "cyrus-sasl-devel";
	optional[32]["required"] = false;
	optional[32]["required_by"] = "(--with-ldap=/usr)";

	optional[33]["dependeny"] = "libmcrypt-devel";
	optional[33]["minversion"] = "2.5";
	optional[33]["dnf_pkg"] = "libmcrypt-devel";
	optional[33]["required"] = false;
	optional[33]["required_by"] = "(--with-mcrypt=/usr)";

	optional[34]["dependeny"] = "libmcrypt";
	optional[34]["minversion"] = "2.5";
	optional[34]["dnf_pkg"] = "libmcrypt";
	optional[34]["required"] = false;
	optional[34]["required_by"] = "(--with-mcrypt=/usr)";

	optional[35]["dependeny"] = "aspell-devel";
	optional[35]["minversion"] = "0.60";
	optional[35]["dnf_pkg"] = "aspell-devel";
	optional[35]["required"] = false;
	optional[35]["required_by"] = "(--with-pspell)";

	optional[36]["dependeny"] = "aspell";
	optional[36]["minversion"] = "0.60";
	optional[36]["dnf_pkg"] = "aspell";
	optional[36]["required"] = false;
	optional[36]["required_by"] = "(--with-pspell)";

	optional[37]["dependeny"] = "readline-devel";
	optional[37]["minversion"] = "6.3";
	optional[37]["dnf_pkg"] = "readline-devel";
	optional[37]["required"] = false;
	optional[37]["required_by"] = "(--with-readline=/usr)";

	optional[38]["dependeny"] = "ncurses-devel";
	optional[38]["minversion"] = "5.9";
	optional[38]["dnf_pkg"] = "ncurses-devel";
	optional[38]["required"] = false;
	optional[38]["required_by"] = "(--with-readline=/usr)";

	optional[39]["dependeny"] = "libtidy-devel";
	optional[39]["minversion"] = "0.99";
	optional[39]["dnf_pkg"] = "libtidy-devel";
	optional[39]["required"] = false;
	optional[39]["required_by"] = "(--with-tidy=/usr)";

	optional[40]["dependeny"] = "libtidy";
	optional[40]["minversion"] = "0.99";
	optional[40]["dnf_pkg"] = "libtidy";
	optional[40]["required"] = false;
	optional[40]["required_by"] = "(--with-tidy=/usr)";

	optional[41]["dependeny"] = "libldb-devel";
	optional[41]["minversion"] = "1.1";
	optional[41]["dnf_pkg"] = "libldb-devel";
	optional[41]["required"] = false;
	optional[41]["required_by"] = "";

	optional[42]["dependeny"] = "libtalloc-devel";
	optional[42]["minversion"] = "2.1";
	optional[42]["dnf_pkg"] = "libtalloc-devel";
	optional[42]["required"] = false;
	optional[42]["required_by"] = "";

	optional[43]["dependeny"] = "libtdb-devel";
	optional[43]["minversion"] = "1.3";
	optional[43]["dnf_pkg"] = "libtdb-devel";
	optional[43]["required"] = false;
	optional[43]["required_by"] = "";

	optional[44]["dependeny"] = "libtevent-devel";
	optional[44]["minversion"] = "0.9";
	optional[44]["dnf_pkg"] = "libtevent-devel";
	optional[44]["required"] = false;
	optional[44]["required_by"] = "";

	optional[45]["dependeny"] = "libgcrypt-devel";
	optional[45]["minversion"] = "1.6";
	optional[45]["dnf_pkg"] = "libgcrypt-devel";
	optional[45]["required"] = false;
	optional[45]["required_by"] = "";

	optional[46]["dependeny"] = "libgpg-error-devel";
	optional[46]["minversion"] = "1.21";
	optional[46]["dnf_pkg"] = "libgpg-error-devel";
	optional[46]["required"] = false;
	optional[46]["required_by"] = "";

	optional[47]["dependeny"] = "libxslt-devel";
	optional[47]["minversion"] = "1.1";
	optional[47]["dnf_pkg"] = "libxslt-devel";
	optional[47]["required"] = false;
	optional[47]["required_by"] = "";

	optional[48]["dependeny"] = "pkgconfig";
	optional[48]["minversion"] = "0";
	optional[48]["dnf_pkg"] = "pkgconfig";
	optional[48]["required"] = false;
	optional[48]["required_by"] = "";

	optional[49]["dependeny"] = "mariadb-devel";
	optional[49]["minversion"] = "0";
	optional[49]["dnf_pkg"] = "mariadb-devel";
	optional[49]["required"] = false;
	optional[49]["required_by"] = "";

	optional[50]["dependeny"] = "recode";
	optional[50]["minversion"] = "0";
	optional[50]["dnf_pkg"] = "recode";
	optional[50]["required"] = false;
	optional[50]["required_by"] = "";

	//
	/*
	 optional[]["dependeny"] 	= "";
	 optional[]["minversion"] 	= "";
	 optional[]["dnf_pkg"] 		= "";
	 optional[]["required"] 	= false;
	 optional[]["required_by"]	= ""; */
	return optional;
}

Dependencies::~Dependencies()
{
	// TODO Auto-generated destructor stub
}
}
