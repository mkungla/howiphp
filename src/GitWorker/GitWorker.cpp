/*
 * ******************************************************************
 * Created by   Marko Kungla on 2010
 * Updated by   Marko Kungla @ Okram Labs on Aug 6, 2016 - 6:16:05 PM
 * Contact      okramlabs@gmail.com - https://okramlabs.com
 * @copyright   2010 - 2016 Marko Kungla - https://github.com/mkungla
 * @license     MIT
 *
 * Package name    howiphp
 * @category       HOWI3
 * @package        howiphp
 *
 * File         GitWorker.cpp
 * @link        https://github.com/okramlabs/howiphp
 * ********************************************************************
 * Comments:
 * *******************************************************************/
#include "GitWorker.h"

namespace howiphp
{
GitWorker::GitWorker()
{

}

void GitWorker::print_progress_out(const progress_data *pd)
{
	int network_percent =
			pd->fetch_progress.total_objects > 0 ?
					(100 * pd->fetch_progress.received_objects)
							/ pd->fetch_progress.total_objects :
					0;
	int index_percent =
			pd->fetch_progress.total_objects > 0 ?
					(100 * pd->fetch_progress.indexed_objects)
							/ pd->fetch_progress.total_objects :
					0;

	int checkout_percent =
			pd->total_steps > 0 ?
					(100 * pd->completed_steps) / pd->total_steps : 0;
	int kbytes = pd->fetch_progress.received_bytes / 1024;

	cout << output.fg_colors["White"] << left << "[\033[1m"
			<< setw(output.left_block_col) << left << output.fg_colors["Green"]
			<< " >> \033[0m" << "\033[0m]: \033[0m";

	if (pd->fetch_progress.total_objects
			&& pd->fetch_progress.received_objects
					== pd->fetch_progress.total_objects)
	{
		printf("Resolving deltas %d/%d\r", pd->fetch_progress.indexed_deltas,
				pd->fetch_progress.total_deltas);
	}
	else
	{
		printf(
				"Cloning %3d%% (%4d kb, %5d/%5d)  /  idx %3d%% (%5d/%5d)  /  chk %3d%% (%4" PRIuZ "/%4" PRIuZ ") %s\r",
				network_percent, kbytes, pd->fetch_progress.received_objects,
				pd->fetch_progress.total_objects, index_percent,
				pd->fetch_progress.indexed_objects,
				pd->fetch_progress.total_objects, checkout_percent,
				pd->completed_steps, pd->total_steps, pd->path);
	}
	fflush(stdout);
}

int GitWorker::sideband_progress(const char *str, int len, void *payload)
{
	(void) payload; // unused

	cout << "\r" << output.fg_colors["White"] << left << "[\033[1m"
			<< setw(output.left_block_col) << left << output.fg_colors["Green"]
			<< " >> \033[0m" << "\033[0m]: \033[0m";
	printf("Remote: %*s ", len, str);
	fflush(stdout);
	return 0;
}

int GitWorker::fetch_progress(const git_transfer_progress *stats, void *payload)
{
	progress_data *pd = (progress_data*) payload;
	pd->fetch_progress = *stats;
	GitWorker::print_progress_out(pd);
	return 0;
}
void GitWorker::checkout_progress(const char *path, size_t cur, size_t tot,
		void *payload)
{
	progress_data *pd = (progress_data*) payload;
	pd->completed_steps = cur;
	pd->total_steps = tot;
	pd->path = path;
	GitWorker::print_progress_out(pd);
}

int GitWorker::clone()
{
	output.task();
	output.info("Cloning php-src master from Github");
	git_libgit2_init();
	system("setterm -cursor off");
	int error;
	git_repository *repo;

	// Before running the actual command, create an instance of the local
	// repository and pass it to the function.

	error = git_repository_open(&repo, ".git");
	if (error < 0)
		repo = NULL;

	// Run the command. If something goes wrong, print the error message to stderr
	progress_data pd =
	{
	{ 0 } };
	git_repository *cloned_repo = NULL;
	git_clone_options clone_opts = GIT_CLONE_OPTIONS_INIT;
	git_checkout_options checkout_opts = GIT_CHECKOUT_OPTIONS_INIT;

	Json::Value repodata = config.conf_storage["source"]["repos"]["Github"];
	string repourl = repodata["proto"].asString() + "://"
			+ repodata["host"].asString() + repodata["path"].asString();
	const char *url = repourl.c_str();
	string repolocalpath =
			config.conf_storage["paths"]["lib_php_path"].asString()
					+ "/git-src";
	const char *path = repolocalpath.c_str();

	(void) repo; // unused

	// Set up options
	checkout_opts.checkout_strategy = GIT_CHECKOUT_SAFE;
	checkout_opts.progress_cb = checkout_progress;
	checkout_opts.progress_payload = &pd;
	clone_opts.checkout_opts = checkout_opts;
	clone_opts.fetch_opts.callbacks.sideband_progress = sideband_progress;
	clone_opts.fetch_opts.callbacks.transfer_progress = &fetch_progress;

	clone_opts.fetch_opts.callbacks.payload = &pd;

	// Do the clone
	error = git_clone(&cloned_repo, url, path, &clone_opts);
	printf("\n");
	if (error != 0)
	{
		const git_error *err = giterr_last();
		if (err)
			printf("ERROR %d: %s\n", err->klass, err->message);
		else
			printf("ERROR %d: no detailed info\n", error);
	}
	else if (cloned_repo)
		git_repository_free(cloned_repo);

	if (error < 0)
	{
		if (giterr_last() == NULL)
			fprintf(stderr, "Error without message");
		else
			fprintf(stderr, "Bad news:\n %s\n", giterr_last()->message);
	}

	if (repo)
		git_repository_free(repo);

	output.ok("Cloning repository done!", true);
	git_libgit2_shutdown();
	system("setterm -cursor on");
	return error;
}

/** Entry point for this command */
int GitWorker::fetch()
{
	output.task();
	output.info("Updating repository ...");
	git_libgit2_init();
	system("setterm -cursor off");

	git_repository *repo;

	// Before running the actual command, create an instance of the local
	// repository and pass it to the function.

	string repolocalpath =
			config.conf_storage["paths"]["lib_php_path"].asString()
					+ "/git-src/.git";
	const char *path = repolocalpath.c_str();
	// Figure out whether it's a named remote or a URL
	git_repository_open(&repo, path);

	git_remote *remote = NULL;
	const git_transfer_progress *stats;
	//dl_data data; //triggers unused warning if we dont need it for reference remove it
	git_fetch_options fetch_opts = GIT_FETCH_OPTIONS_INIT;

	// Figure out whether it's a named remote or a URL
	string start_fetch_msg = "Fetching origin for repo ";

	output.info(start_fetch_msg, true);
	if (git_remote_lookup(&remote, repo, "origin") < 0)
	{
		if (git_remote_create_anonymous(&remote, repo, "origin") < 0)
			return -1;
	}

	// Set up the callbacks (only update_tips for now)
	fetch_opts.callbacks.update_tips = &update_cb;
	fetch_opts.callbacks.sideband_progress = &progress_cb;
	fetch_opts.callbacks.transfer_progress = transfer_progress_cb;

	/**
	 * Perform the fetch with the configured refspecs from the
	 * config. Update the reflog for the updated references with
	 * "fetch".
	 */
	if (git_remote_fetch(remote, NULL, &fetch_opts, "fetch") < 0)
		return -1;

	/**
	 * If there are local objects (we got a thin pack), then tell
	 * the user how many objects we saved from having to cross the
	 * network.
	 */
	stats = git_remote_stats(remote);
	char local_objects[256];
	if (stats->local_objects > 0)
	{

		sprintf(local_objects,
				"Received %d/%d objects in %" PRIuZ " bytes (used %d local objects)",
				stats->indexed_objects, stats->total_objects,
				stats->received_bytes, stats->local_objects);
	}
	else
	{
		sprintf(local_objects, "Received %d/%d objects in %" PRIuZ "bytes",
				stats->indexed_objects, stats->total_objects,
				stats->received_bytes);
	}
	output.ok(string(local_objects), true);

	git_remote_free(remote);
	git_repository_free(repo);
	git_libgit2_shutdown();
	system("setterm -cursor on");
	output.ok("Updating repository done!", true);
	return 0;
}
int GitWorker::progress_cb(const char *str, int len, void *data)
{
	(void) data;
	cout << output.fg_colors["White"] << left << "[\033[1m"
			<< setw(output.left_block_col) << left << output.fg_colors["Green"]
			<< " >> \033[0m" << "\033[0m]: \033[0m";
	printf("4 remote: %.*s", len, str);
	fflush(stdout); /* We don't have the \n to force the flush */
	return 0;
}
/**
 * This gets called during the download and indexing. Here we show
 * processed and total objects in the pack and the amount of received
 * data. Most frontends will probably want to show a percentage and
 * the download rate.
 */
int GitWorker::transfer_progress_cb(const git_transfer_progress *stats,
		void *payload)
{
	cout << output.fg_colors["White"] << left << "[\033[1m"
			<< setw(output.left_block_col) << left << output.fg_colors["Green"]
			<< " >> \033[0m" << "\033[0m]: \033[0m";

	if (stats->received_objects == stats->total_objects)
	{
		printf("5 Resolving deltas %d/%d\r", stats->indexed_deltas,
				stats->total_deltas);
	}
	else if (stats->total_objects > 0)
	{
		printf("6 Received %d/%d objects (%d) in %" PRIuZ " bytes\r",
				stats->received_objects, stats->total_objects,
				stats->indexed_objects, stats->received_bytes);
	}
	return 0;
}
/**
 * This function gets called for each remote-tracking branch that gets
 * updated. The message we output depends on whether it's a new one or
 * an update.
 */
int GitWorker::update_cb(const char *refname, const git_oid *a,
		const git_oid *b, void *data)
{
	char a_str[GIT_OID_HEXSZ + 1], b_str[GIT_OID_HEXSZ + 1];
	(void) data;

	git_oid_fmt(b_str, b);
	b_str[GIT_OID_HEXSZ] = '\0';

	char update_cb_msg[128];
	if (git_oid_iszero(a))
	{
		sprintf(update_cb_msg, "[new]     %.20s %s\n", b_str, refname);
	}
	else
	{
		git_oid_fmt(a_str, a);
		a_str[GIT_OID_HEXSZ] = '\0';
		sprintf(update_cb_msg, "[updated] %.10s..%.10s %s\n", a_str, b_str,
				refname);
	}
	output.ok(string(update_cb_msg), true);
	return 0;
}
void GitWorker::tags()
{

	output.task();
	git_libgit2_init();
	system("setterm -cursor off");
	int error;
	git_repository *repo;

	// Before running the actual command, create an instance of the local
	// repository and pass it to the function.

	string repolocalpath =
			config.conf_storage["paths"]["lib_php_path"].asString()
					+ "/git-src/.git";
	const char *path = repolocalpath.c_str();
	// Figure out whether it's a named remote or a URL
	error = git_repository_open(&repo, path);
	git_strarray tags =
	{ 0 };
	error = git_tag_list_match(&tags, "php-*", repo);
	if (error)
	{
		output.error("Failed to fetch tags!");
		output.close();
	}
	size_t i;
	for (i = 0; i < tags.count; i++)
	{
		string tagversion = string(tags.strings[i]);
		string tagversionnr = tagversion;
		string versionnr = tagversion.erase(0, 4);
		config.conf_storage["php"]["tags"][versionnr] = tagversionnr;
	}
	output.table_row(i, "total", "php releases available from tags");
	git_strarray_free(&tags);
	git_repository_free(repo);
	git_libgit2_shutdown();
	system("setterm -cursor on");
	output.ok("Updating TAGS done!", true);
}
GitWorker::~GitWorker()
{

}
}
