/*
 * ******************************************************************
 * Created by   Marko Kungla on 2010
 * Updated by   Marko Kungla on Aug 6, 2016 - 6:15:31 PM
 *
 * @copyright   2010 - 2016 Marko Kungla - https://github.com/mkungla
 * @license     MIT
 *
 * Package name    howiphp
 * @category       HOWI3
 * @package        howiphp
 *
 * File         GitWorker.h
 * @link        https://github.com/mkungla/howiphp
 * ********************************************************************
 * Comments:
 * *******************************************************************/
#ifndef HOWIPHP_GITWORKER_H_
#define HOWIPHP_GITWORKER_H_

#include "../howiphp.h"
#include <git2.h>

#ifndef PRIuZ
/* Define the printf format specifer to use for size_t output */
#if defined(_MSC_VER) || defined(__MINGW32__)
#	define PRIuZ "Iu"
#else
#	define PRIuZ "zu"
#endif
#endif

namespace howiphp
{
class GitWorker
{
public:
	GitWorker();
	typedef struct
	{
		git_transfer_progress fetch_progress;
		size_t completed_steps;
		size_t total_steps;
		const char *path;
	} progress_data;

	typedef struct
	{
		const git_oid *target_id;
		git_otype target_type;
		const char *tag_name;
		const git_signature *tagger;
		const char *message;

	} tag_data;

	typedef struct
	{
		git_remote *remote;
		git_fetch_options *fetch_opts;
		int ret;
		int finished;
	} dl_data;

	static void print_progress_out(const progress_data *);

	static int sideband_progress(const char *, int, void *);

	static int fetch_progress(const git_transfer_progress *, void *);

	static void checkout_progress(const char *, size_t, size_t, void *);

	static int transfer_progress_cb(const git_transfer_progress *, void *);

	static int update_cb(const char *, const git_oid *, const git_oid *,
			void *);

	static int progress_cb(const char *, int, void *);

	int clone();

	int fetch();

	void tags();

	virtual ~GitWorker();
};
}

#endif /* HOWIPHP_GITWORKER_H_ */
