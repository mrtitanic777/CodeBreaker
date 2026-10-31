/* cbengine.h -- the C interface of CodeBreaker's engine and store (native/engine), loaded by
 * codebreaker/engine/native_engine.py through ctypes.
 *
 * Every call returns 0, or an error kind (1 LookupError, 2 other, 3 an exception a Python callback
 * raised -- cbe_error_py() gives its number in the shim's table --, 4 sqlite3's, 5 KeyError,
 * 6 ValueError, 7 OSError); cbe_error() gives the message, cbe_error_code() SQLite's or the OS's
 * code.  A buffer handed back (char** out, int64_t* n) is freed with cbe_free.  Lists and rows go
 * as row buffers: a run of fields, each a type byte (0 None, 1 int, 2 float, 3 text, 4 bytes) and
 * its value (8 bytes; or an 8-byte length and the bytes). */
#ifndef CBENGINE_H
#define CBENGINE_H
#include <stdint.h>

#ifdef CBE_BUILD
#define CBE_API __declspec(dllexport)
#else
#define CBE_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef int64_t (*cbe_step_cb)(const char* kind);
typedef int64_t (*cbe_version_cb)(const char* kind, void* sink);
typedef int64_t (*cbe_make_cb)(void* run, const char* kind, int32_t n, void** ctxs, int64_t hint, int64_t context,
                               int32_t batch);
typedef int64_t (*cbe_digest_cb)(const char* read, void* sink);
typedef void (*cbe_log_cb)(void* who, const char* text);

CBE_API int cbe_abi(void);                                 /* 1 */
CBE_API const char* cbe_error(void);
CBE_API int64_t cbe_error_py(void);
CBE_API int cbe_error_code(void);
CBE_API void cbe_free(void* p);
CBE_API void cbe_sink_put(void* sink, const char* p, int64_t n);
CBE_API void cbe_set_callbacks(cbe_step_cb step, cbe_version_cb version, cbe_make_cb make, cbe_digest_cb digest,
                               cbe_log_cb log);

/* the step registry */
CBE_API int cbe_step_register(const char* kind, const char* platform, const char* reads, int64_t nreads, int batch,
                              int always);
CBE_API int cbe_step_unregister(const char* kind);

/* the program database (facts.py) */
CBE_API int cbe_db_open(const char* path, void** out);
CBE_API void cbe_db_release(void* db);
CBE_API int cbe_db_close(void* db);
CBE_API int cbe_db_path(void* db, char** out, int64_t* n);
CBE_API int cbe_db_set_sha(void* db, const char* sha);
CBE_API int cbe_db_row(void* db, const char* kind, const char* key, char** out, int64_t* n);
CBE_API int cbe_db_rows(void* db, const char* kind, char** out, int64_t* n);
CBE_API int cbe_db_kinds(void* db, char** out, int64_t* n);
CBE_API int cbe_db_blob(void* db, const char* hash, char** out, int64_t* n);
CBE_API int cbe_db_about(void* db, const char* name, char** out, int64_t* n, int* found);
CBE_API int cbe_db_set_about(void* db, const char* name, const char* value);
CBE_API int cbe_db_write(void* db, const char* kind, const char* key, const char* tag, const char* data, int64_t ndata,
                         const char* step, const char* version, const char* inputs, int64_t ninputs, double took,
                         char* hash /* 64 */);
CBE_API int cbe_db_write_raw(void* db, const char* kind, const char* key, const char* hash, const char* tag,
                             const char* data, int64_t ndata, const char* step, const char* version,
                             const char* inputs_json, double took);
CBE_API int cbe_db_remake(void* db, const char* kind, const char* key, const char* version, const char* inputs,
                          int64_t ninputs);
CBE_API int cbe_db_forget(void* db, const char* kind, const char* key, int64_t* n);
CBE_API int cbe_db_drop_other_versions(void* db, const char* step, const char* version, int64_t* n);
CBE_API int cbe_db_collect(void* db);
CBE_API int cbe_db_cleaned(void* db, int op, const char* step, int* out);   /* op 0 in?, 1 add, 2 clear */
CBE_API int cbe_digest(const char* tag, const char* data, int64_t n, char* out /* 41 */);
CBE_API int cbe_sha(const char* path, char* out /* 65 */);
CBE_API int cbe_databases(const char* folder, char** out, int64_t* n);
CBE_API int cbe_hold(int64_t cap, const char* folder, char** out, int64_t* n);

/* a run (engine.py's Run) and the contexts its steps are given */
CBE_API int cbe_run_new(void* db, int log, void** out);
CBE_API void cbe_run_free(void* run);
CBE_API int cbe_run_set_log(void* run, int log);
CBE_API int cbe_run_current(void* run, const char* kind, const char* key, int* kept, char* hash /* 128 */);
CBE_API int cbe_run_many(void* run, const char* kind, const char* keys, int64_t nkeys, int64_t hint, int jobs,
                         int64_t context);
CBE_API int cbe_run_stale(void* run, const char* kind, const char* keys, int64_t nkeys, char** out, int64_t* n);
CBE_API int cbe_run_counts(void* run, char** out, int64_t* n);
CBE_API int cbe_ctx_need(void* ctx, const char* kind, const char* key);
CBE_API int cbe_ctx_need_many(void* ctx, const char* kind, const char* keys, int64_t nkeys, int64_t hint, int jobs,
                              int64_t context);
CBE_API int cbe_ctx_peek(void* ctx, const char* kind, const char* key, int* found);
CBE_API int cbe_ctx_knowledge(void* ctx, const char* reads, int64_t nreads);
CBE_API int cbe_ctx_result(void* ctx, const char* tag, const char* data, int64_t n);
CBE_API int cbe_ctx_unkept(void* ctx);
CBE_API int cbe_ctx_inputs(void* ctx, char** out, int64_t* n);
CBE_API int cbe_ctx_key(void* ctx, char** out, int64_t* n);

/* the function store (store/functions.py) and the knowledge base (knowledge/store.py) */
CBE_API int cbe_store_open(const char* path, const char* folder, int readonly, void** out);
CBE_API int cbe_store_call(void* store, const char* method, const char* args, int64_t nargs, char** out, int64_t* n);
CBE_API void cbe_store_free(void* store);
CBE_API int cbe_kb_open(const char* path, void** out);
CBE_API int cbe_kb_call(void* kb, const char* method, const char* args, int64_t nargs, char** out, int64_t* n);
CBE_API void cbe_kb_release(void* kb);
CBE_API int cbe_kb_close(void* kb);

/* the verdict vocabulary (verify/verdicts.py): verdict, klass, match, classify, and the tables */
CBE_API int cbe_verdicts_call(const char* method, const char* args, int64_t nargs, char** out, int64_t* n);

#ifdef __cplusplus
}
#endif
#endif
