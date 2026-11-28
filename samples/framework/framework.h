/* samples/framework/framework.h -- what every CodeBreaker SDK sample shares, the way the XDK's samples share the ATG framework:
 * the command line, starting and stopping the SDK, failing with the library's own words, timing, and printing addresses.
 *
 *     int main(int argc, char** argv) {
 *         fw_args args;
 *         fw_parse(argc, argv, "HelloBinary <file>", 1, &args);    every sample takes --data-root, --state, --time and --help
 *         fw_start(&args);                                         cb_init, and a check that the DLL is one these headers work with
 *         fw_check(cb_binary_open(args.values[0], &b), "open");    prints cb_last_error() and exits on failure
 *         ...
 *         fw_stop();
 *     }
 *
 * Plain C, so a sample in C and one in C++ both use it. Timings (--time) go to stderr, so what a sample prints on stdout is the same on
 * every run and every machine (the gate compares it with tests/sdk/golden).
 */
#ifndef CB_SAMPLES_FRAMEWORK_H
#define CB_SAMPLES_FRAMEWORK_H

#include <stddef.h>

#include "cb/binary.h"
#include "cb/cb.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct fw_args {
    const char* program;       /* the sample's name */
    int count;                 /* the positional arguments */
    const char* values[16];
    const char* data_root;     /* --data-root DIR: cb_init_options.data_root (default: the SDK finds it) */
    const char* state_dir;     /* --state DIR: cb_init_options.state_dir */
    int timing;                /* --time: how long each step took, on stderr */
    int flags[8];              /* the sample's own switches, by fw_parse's `switches` */
} fw_args;

/* Reads the command line. `usage` is the first line --help prints; `positional` the fewest positional arguments; `switches` NULL or a
 * NULL-ended list of the sample's own flags ("--list"), each set in flags[i] when given. A wrong command line prints the usage and exits 2. */
void fw_parse(int argc, char** argv, const char* usage, int positional, const char* const* switches, fw_args* args);

/* cb_init with the arguments' folders; exits 1 with the library's message when it fails. Warns when the DLL is not one the headers fit. */
void fw_start(const fw_args* args);
/* cb_shutdown. */
void fw_stop(void);

/* On a status other than CB_OK: "<program>: <what> failed: <status>: <cb_last_error()>" on stderr, cb_shutdown, exit 1. */
void fw_check(cb_status status, const char* what);

/* Seconds from a fixed point, for timing. */
double fw_seconds(void);
/* With --time: "<what>: <ms> ms" on stderr, measured from `since` (fw_seconds). */
void fw_timed(const char* what, double since);

/* An address as text, into `buffer` (32 bytes). */
const char* fw_address(cb_address address, char* buffer);
/* The file name of a path (after its last / or \). */
const char* fw_file_name(const char* path);

#ifdef __cplusplus
}
#endif

#endif
