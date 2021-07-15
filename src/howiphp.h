/*
 * ******************************************************************
 * Created by   Marko Kungla on 2010
 * Updated by   Marko Kungla on Aug 6, 2016 - 6:03:18 PM
 *
 * @copyright   2010 - 2016 Marko Kungla - https://github.com/mkungla
 * @license     MIT
 *
 * Package name    howiphp
 * @category       HOWI3
 * @package        howiphp
 *
 * File         howiphp.h
 * @link        https://github.com/mkungla/howiphp
 * ********************************************************************
 * Comments:
 * *******************************************************************/

#ifndef HOWIPHP_H_
#define HOWIPHP_H_

/* Including globally needed output and config
 * Since output and config are used within every other class!
 */
#include "Config/Config.h"
#include "Output/Output.h"

/**
 * howiphp formerly known as libhowi-php prototype library
 * is for to manage multiple php versions
 * Build from source / tags / reaeases
 * and switch between these
 */
namespace howiphp
{
/// Hostname as string
typedef string HostName;

/// Protocoll ex: http | https as string
typedef string Protocol;

/// SampleFilePath in remote server ex: sample.txt
/// NB! without / prefix
typedef string SampleFilePath;

/// Double Kb per second
typedef double Kbs;

/// Double Mb per second
typedef double Mbs;

/// Double seconds
typedef double Seconds;

/// Long int Millliseconds
typedef long int Milliseconds;

/// Double ping
typedef double Ping;

/// IP address
typedef string IPaddress;

/// DownloadSpeed string
typedef string DownloadSpeed;

/// All ids are type of int
typedef int ID;

/// Name of the repo given by user
typedef string RepoName;

/// Repository URL in context of host URI
typedef string RepoRemoteUrl;

/// Full country name
typedef string Country;

/// ProviderName CDN network provider
typedef string ProviderName;

/// OS Package name
typedef string PackageName;

/// OS Package name for Packagemanager
typedef string PackageManagerName;

/// Any version as string
typedef string VersionName;

/// Something is required
typedef bool Required;

/// FlagInt is the option specified as command line argument
/// Passes some flag with int val
typedef int FlagInt;
/**
 * EXTERNS
 * 1. Config class
 * 2. Output class
 */
extern Config config;
extern Output output;

/**
 * Class to check CDN
 *
 * Primary purpose of this class is to determine CDN's
 * download speed.
 */
class CDN;

/**
 * Multitude of check to perform on user environment.
 * Checking is library usable in Users environment,
 * are all dependencies installed for building php fro source
 */
class Check;

/**
 * CompareVersions
 *
 * This class is used in checks to compare dependency versions
 * It can compare most of common version formats
 */
class CompareVersion;

/**
 * List of dependencies
 */
class Dependencies;

/**
 * Git worker is to work with php-src repositories
 *
 * To build php out of master or php-src TAGS
 * TODO Issue #9 NB! branch master was temporarily removed
 * and currently tag is downloaded as archive.
 */
class GitWorker;

/**
 * List php versions
 *
 * howiphp list <options>
 * * List all php versions
 * * List only available versions
 * * List only installed versions
 * * List filters
 */
class List;

/**
 * PHPbuilder is a class taking care of building PHP from source
 */
class PHPbuilder;

/**
 * ProgressBar class,
 *
 * To show progressbar if total tasks is known
 */
class ProgressBar;

/**
 * UseVersion
 *
 * Switch between installed php versions
 */
class UseVersion;
}

#endif /* HOWIPHP_H_ */
