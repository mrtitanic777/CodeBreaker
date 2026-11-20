// api.cc -- cbdos.h: the file system handed over, a run, its result.
#include "exact/emu/dosexec/cbdos.h"
#include "exact/emu/dosexec/dosexec.h"

#include <cstdio>
#include <cstring>
#include <cwctype>

using namespace cbd;

namespace {
struct Result : cbd_result {
    std::string msg;
    std::wstring fname;
    Bytes con;
    std::vector<uint64_t> blk, cl, wt;
    Bytes wd, so;
};
}  // namespace

extern "C" {

CBD_API int cbd_version(void) { return 1; }

CBD_API int cbd_init(const wchar_t *unicorn_dll, char *err, int errlen) {
    return load_unicorn(unicorn_dll, err, size_t(errlen)) ? 0 : 1;
}

CBD_API void *cbd_fs_new(void) { return new MemFS(); }
CBD_API void cbd_fs_free(void *fs) { delete static_cast<MemFS *>(fs); }

CBD_API void cbd_fs_hide(void *fs, const wchar_t *prefix) { static_cast<MemFS *>(fs)->hide.push_back(prefix); }

CBD_API void cbd_fs_put(void *fs, const wchar_t *key, const uint8_t *data, int64_t n) {
    static_cast<MemFS *>(fs)->files[key] = std::make_shared<Bytes>(data, data + n);
}

CBD_API void cbd_fs_gone(void *fs, const wchar_t *key) { static_cast<MemFS *>(fs)->gone.insert(key); }

CBD_API int64_t cbd_fs_changed(void *fs) {
    MemFS &m = *static_cast<MemFS *>(fs);
    m.out_keys.assign(m.touched.begin(), m.touched.end());
    m.out_data.clear();
    for (const auto &k : m.out_keys) {
        auto it = m.files.find(k);
        m.out_data.push_back(it != m.files.end() ? it->second : nullptr);
    }
    m.out_gone.assign(m.gone.begin(), m.gone.end());
    return int64_t(m.out_keys.size());
}

CBD_API int cbd_fs_entry(void *fs, int64_t i, const wchar_t **key, const uint8_t **data, int64_t *n) {
    MemFS &m = *static_cast<MemFS *>(fs);
    *key = m.out_keys[size_t(i)].c_str();
    const BufP &b = m.out_data[size_t(i)];
    if (!b) {                                         // removed from the files
        *data = nullptr;
        *n = 0;
        return 0;
    }
    *data = b->data();
    *n = int64_t(b->size());
    return 1;
}

CBD_API int64_t cbd_fs_ngone(void *fs) { return int64_t(static_cast<MemFS *>(fs)->out_gone.size()); }
CBD_API const wchar_t *cbd_fs_gone_at(void *fs, int64_t i) { return static_cast<MemFS *>(fs)->out_gone[size_t(i)].c_str(); }

CBD_API cbd_result *cbd_run(const cbd_args *a) {
    Result *r = new Result();
    std::memset(static_cast<cbd_result *>(r), 0, sizeof(cbd_result));
    try {
        Run run;
        run.exe = a->exe;
        run.workdir = a->workdir;
        run.cmdline.assign(a->cmdline, a->cmdline + a->ncmdline);
        run.stdin_data.assign(a->stdin_data, a->stdin_data + a->nstdin);
        run.env.assign(a->env, a->env + a->nenv);
        run.max_blocks = a->max_blocks;
        run.trace_calls = a->trace_calls != 0;
        if (a->psp) run.PSP = a->psp;
        if (a->top) run.TOP = a->top;
        run.fast = a->fast != 0;
        run.want_dpmi = a->dpmi != 0;
        run.fs = static_cast<MemFS *>(a->fs);
        run.S.fast_limit = a->fast_limit;
        run.S.dpmi_limit = a->dpmi_limit;
        run.S.drive = a->drive;
        run.S.debug = a->debug != 0;
        run.watch = a->watch;
        run.watch_near = a->watch_near != 0;
        if (a->watch_max > 0) run.watch_max = a->watch_max;
        if (a->watch_ds_bytes > 0) run.watch_ds_bytes = a->watch_ds_bytes;
        run.watch_dumps = a->watch_dumps;
        if (a->drives) {   // "C=folder\nD=folder"
            std::wstring all = a->drives, line;
            for (size_t k = 0; k <= all.size(); ++k) {
                if (k == all.size() || all[k] == L'\n') {
                    if (line.size() > 2 && line[1] == L'=') run.drives[wchar_t(towupper(line[0]))] = line.substr(2);
                    line.clear();
                } else {
                    line += all[k];
                }
            }
        }
        try {
            run.go();
            r->status = CBD_OK;
        } catch (PyErr &e) {
            r->status = e.kind;
            r->err_no = e.err_no;
            r->msg = e.message;
            r->fname = e.filename;
        }
        r->has_exit = run.exit.has_value();
        r->exit = run.exit ? int32_t(*run.exit) : 0;
        r->con = run.console;
        r->blk.assign(run.blocks.begin(), run.blocks.end());
        for (const auto &kv : run.calls) {
            r->cl.push_back(kv.first);
            r->cl.push_back(uint64_t(kv.second));
        }
        r->steps = run.n;
        r->wt = run.watched;
        r->wd = run.watch_bytes;
        r->so = run.standard_out;
        r->load_seg = run.load;
    } catch (std::exception &e) {
        r->status = CBD_OTHER;
        r->msg = e.what();
    }
    r->message = r->msg.c_str();
    r->filename = r->fname.c_str();
    r->console = r->con.data();
    r->nconsole = int64_t(r->con.size());
    r->blocks = r->blk.data();
    r->nblocks = int64_t(r->blk.size());
    r->calls = r->cl.data();
    r->ncalls = int64_t(r->cl.size() / 2);
    r->watched = r->wt.data();
    r->nwatched = int64_t(r->wt.size() / CBD_WATCH_STRIDE);
    r->dumps = r->wd.data();
    r->ndumps = int64_t(r->wd.size());
    r->standard_out = r->so.data();
    r->nstandard_out = int64_t(r->so.size());
    return r;
}

CBD_API void cbd_free(cbd_result *r) { delete static_cast<Result *>(r); }

}  // extern "C"
