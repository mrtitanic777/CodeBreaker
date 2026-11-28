/* samples/framework/framework.c -- the samples' shared framework (framework.h). */
#include "framework.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static const fw_args* fw_current;

static void fw_usage(const fw_args* args, const char* usage, const char* const* switches) {
    int i;
    fprintf(stderr, "usage: %s [--data-root DIR] [--state DIR] [--time]", usage);
    for (i = 0; switches != NULL && switches[i] != NULL; ++i) fprintf(stderr, " [%s]", switches[i]);
    fprintf(stderr, "\n  (%s, on the CodeBreaker SDK %s)\n", args->program, CB_SDK_VERSION_STRING);
}

void fw_parse(int argc, char** argv, const char* usage, int positional, const char* const* switches, fw_args* args) {
    int i, k;
    memset(args, 0, sizeof *args);
    args->program = fw_file_name(argc > 0 ? argv[0] : "sample");
    for (i = 1; i < argc; ++i) {
        const char* a = argv[i];
        int known = 0;
        if (strcmp(a, "--help") == 0 || strcmp(a, "-h") == 0) {
            fw_usage(args, usage, switches);
            exit(0);
        }
        if ((strcmp(a, "--data-root") == 0 || strcmp(a, "--state") == 0) && i + 1 < argc) {
            if (a[2] == 'd') args->data_root = argv[++i];
            else args->state_dir = argv[++i];
            continue;
        }
        if (strcmp(a, "--time") == 0) {
            args->timing = 1;
            continue;
        }
        for (k = 0; switches != NULL && switches[k] != NULL && k < 8; ++k)
            if (strcmp(a, switches[k]) == 0) args->flags[k] = known = 1;
        if (known) continue;
        if (a[0] == '-' && a[1] == '-') {
            fprintf(stderr, "%s: %s is not an option\n", args->program, a);
            fw_usage(args, usage, switches);
            exit(2);
        }
        if (args->count == (int)(sizeof args->values / sizeof args->values[0])) {
            fprintf(stderr, "%s: too many arguments\n", args->program);
            exit(2);
        }
        args->values[args->count++] = a;
    }
    if (args->count < positional) {
        fw_usage(args, usage, switches);
        exit(2);
    }
    fw_current = args;
}

void fw_start(const fw_args* args) {
    cb_init_options options = CB_INIT_OPTIONS_INIT;
    const unsigned version = cb_version();
    double t = fw_seconds();
    options.data_root = args->data_root;
    options.state_dir = args->state_dir;
    if ((version >> 16) != CB_SDK_VERSION_MAJOR || ((version >> 8) & 0xFF) < CB_SDK_VERSION_MINOR)
        fprintf(stderr, "%s: warning: built for the SDK %s, running on %s\n", args->program, CB_SDK_VERSION_STRING, cb_version_string());
    fw_check(cb_init(&options), "cb_init");
    fw_timed("cb_init", t);
}

void fw_stop(void) { cb_shutdown(); }

void fw_check(cb_status status, const char* what) {
    if (status == CB_OK) return;
    fprintf(stderr, "%s: %s failed: %s: %s\n", fw_current ? fw_current->program : "sample", what, cb_status_name(status), cb_last_error());
    cb_shutdown();
    exit(1);
}

double fw_seconds(void) {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
}

void fw_timed(const char* what, double since) {
    if (fw_current != NULL && fw_current->timing) fprintf(stderr, "[time] %s: %.3f ms\n", what, (fw_seconds() - since) * 1000.0);
}

const char* fw_address(cb_address address, char* buffer) {
    if (cb_address_format(&address, buffer, 32, NULL) != CB_OK) buffer[0] = '\0';
    return buffer;
}

const char* fw_file_name(const char* path) {
    const char* name = path;
    const char* p;
    for (p = path; *p != '\0'; ++p)
        if (*p == '/' || *p == '\\') name = p + 1;
    return name;
}
