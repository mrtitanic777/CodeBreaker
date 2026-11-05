/* cbdos.h -- the C interface of cbdos.dll: CodeBreaker's in-process DOS executive and DPMI host
 * (exact/emu/dos.py run() and exact/emu/dpmi.py, in C++).  Used through ctypes by
 * codebreaker/exact/emu/native_dosexec.py; nothing here depends on Python.
 *
 *   cbd_init(unicorn.dll path)          once: the Unicorn Python loaded
 *   fs = cbd_fs_new(); cbd_fs_hide / cbd_fs_put / cbd_fs_gone    a MemFS's state, handed over
 *   r = cbd_run(&args)                  one run (its EXEC children inside it)
 *   cbd_fs_changed / cbd_fs_entry / cbd_fs_ngone / cbd_fs_gone_at   what the run changed in the MemFS
 *   cbd_free(r); cbd_fs_free(fs)
 */
#pragma once
#include <stdint.h>
#include <wchar.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CBD_API __declspec(dllexport)

/* why a run raised (dos.run's exception, by the Python class it would have been) */
enum {
    CBD_OK = 0,
    CBD_RUNTIME = 1,      /* RuntimeError(message) */
    CBD_UC = 2,           /* unicorn.UcError(err_no) */
    CBD_OS = 3,           /* OSError(err_no, message, filename)   (err_no < 0: winerror -err_no) */
    CBD_ASSERT = 4,       /* AssertionError(message) */
    CBD_VALUE = 5,        /* ValueError(message) */
    CBD_STRUCT = 6,       /* struct.error(message) */
    CBD_UNSUPPORTED = 7,  /* io.UnsupportedOperation(message) */
    CBD_OTHER = 8         /* RuntimeError(message): an internal failure */
};

typedef struct cbd_args {
    const wchar_t *exe;            /* the program, as dos.run was given it */
    const wchar_t *workdir;        /* the working folder (drive C:) */
    const uint8_t *cmdline;        /* the command tail, latin-1 */
    int64_t ncmdline;
    const uint8_t *stdin_data;
    int64_t nstdin;
    const uint8_t *env;            /* the environment strings (dos.run's env) */
    int64_t nenv;
    int64_t max_blocks;
    int32_t trace_calls;
    int32_t psp;                   /* 0: the default */
    int32_t top;                   /* 0: the default */
    int32_t fast;
    int32_t dpmi;                  /* a DPMI host */
    int32_t debug;                 /* DOSRUN_DEBUG */
    int32_t drive;                 /* DOSRUN_DRIVE (INT 21h 19h) */
    double fast_limit;             /* dos.FAST_LIMIT */
    double dpmi_limit;             /* DOSRUN_DPMI_LIMIT */
    void *fs;                      /* a cbd_fs, or NULL: the disk itself */
} cbd_args;

typedef struct cbd_result {
    int32_t status;                /* CBD_OK or why it raised */
    int32_t err_no;
    const char *message;           /* UTF-8 */
    const wchar_t *filename;       /* (CBD_OS) */
    int32_t has_exit;
    int32_t exit;
    const uint8_t *console;
    int64_t nconsole;
    const uint64_t *blocks;        /* the executed blocks' addresses (a run that is not fast) */
    int64_t nblocks;
    int64_t steps;
    int32_t load_seg;
    const uint64_t *calls;         /* (address, count) pairs (trace_calls) */
    int64_t ncalls;
} cbd_result;

CBD_API int cbd_version(void);
CBD_API int cbd_init(const wchar_t *unicorn_dll, char *err, int errlen);

CBD_API void *cbd_fs_new(void);
CBD_API void cbd_fs_free(void *fs);
CBD_API void cbd_fs_hide(void *fs, const wchar_t *prefix);
CBD_API void cbd_fs_put(void *fs, const wchar_t *key, const uint8_t *data, int64_t n);
CBD_API void cbd_fs_gone(void *fs, const wchar_t *key);
CBD_API int64_t cbd_fs_changed(void *fs);
CBD_API int cbd_fs_entry(void *fs, int64_t i, const wchar_t **key, const uint8_t **data, int64_t *n);
CBD_API int64_t cbd_fs_ngone(void *fs);
CBD_API const wchar_t *cbd_fs_gone_at(void *fs, int64_t i);

CBD_API cbd_result *cbd_run(const cbd_args *args);
CBD_API void cbd_free(cbd_result *r);

#ifdef __cplusplus
}
#endif
