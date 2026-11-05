// imports.cc -- the stand-in for an import entered (hook_code's EXTERNAL-LANDING STUB).
//
// Reaching an import's sentinel (by `call [iat]` or `jmp [iat]`), or a landing pad a pretend
// object's pointers aim at, returns to the caller as the import would: its arguments taken off
// (as many as its signature says -- Python is asked, the one call back during a run -- else as
// many as the call site pushed when its caller does not take them off), what it gives back
// through a pointer given, EAX the stand-in's value; and the call is recorded with its arguments.
#include <algorithm>

#include "run.h"

namespace cbt {

int stand_in(Run *r, uint64_t address, int64_t hit) {
    uc_engine *uc = r->uc;
    bool callback = hit < 0;                   // a landing pad: CALLBACK!k
    bool seen = false;
    for (int64_t d : r->deps)
        seen = seen || d == hit;
    if (!seen)
        r->deps.push_back(hit);

    bool argc_known = false;
    int64_t argc = 0;
    int64_t answer[2 + 2 * 64] = {0};
    if (callback) {
        argc_known = true;
    } else if (r->resolver) {
        // asked once per import per run: the answer is a lookup (the signature's count, the
        // declared out-parameters), the same each time it is asked
        auto known = r->answers.find(hit);
        if (known != r->answers.end()) {
            std::copy(known->second.begin(), known->second.end(), answer);
        } else {
            if (r->resolver(int32_t(hit), answer, 64)) {
                r->hook_error = 1;             // Python raised: stop, and it raises after the run
                r->stop();
                return 2;
            }
            int64_t n = answer[1] < 0 ? 0 : (answer[1] > 64 ? 64 : answer[1]);
            r->answers.emplace(hit, std::vector<int64_t>(answer, answer + 2 + 2 * n));
        }
        argc_known = answer[0] >= 0;
        argc = argc_known ? answer[0] : 0;
    }
    int64_t cleanup = argc_known ? 4 * argc : r->stub_cleanup;
    if (!argc_known || argc == 0) {
        // what the call site pushed, when its caller does not take it off (_callee_pops)
        uint32_t ret;
        if (read32(uc, reg(uc, UC_X86_REG_ESP), ret)) {
            int64_t pops = callee_pops(uc, ret);
            if (pops)
                cleanup = pops;
        }
    }

    uint64_t esp_now = reg(uc, UC_X86_REG_ESP);
    uint32_t ret_addr;
    if (!read32(uc, esp_now, ret_addr))
        return 1;                              // (Python's except: on as if nothing was entered)
    set_reg(uc, UC_X86_REG_ESP, (esp_now + 4 + uint64_t(cleanup)) & 0xFFFFFFFFull);
    // every import entered, in order, with the arguments its signature says it takes
    r->api_calls_set = true;
    if (r->napi_calls < 256) {
        std::vector<int64_t> row = {hit, argc_known ? argc : -1};
        if (argc_known) {
            for (int64_t i = 0; i < argc; i++) {
                uint32_t a;
                if (!read32(uc, esp_now + 4 + 4 * uint64_t(i), a))
                    return 1;
                row.push_back(a);
            }
        }
        r->api_calls.insert(r->api_calls.end(), row.begin(), row.end());
        r->napi_calls++;
    }
    // what the API gives back through a pointer: a handle by its place, or a number (0)
    if (argc_known && argc && !callback) {
        int64_t nouts = answer[1];
        for (int64_t k = 0; k < nouts && k < 64; k++) {
            int64_t i = answer[2 + 2 * k], given = answer[3 + 2 * k];
            if (i >= argc)
                continue;
            uint32_t p;
            if (!read32(uc, esp_now + 4 + 4 * uint64_t(i), p))
                return 1;
            if (p) {
                uint64_t v = given >= 0 ? uint64_t(given) : OUT_HANDLE + 4 * uint64_t(i);
                if (v <= 0xFFFFFFFFull) {
                    uint8_t b[4] = {uint8_t(v), uint8_t(v >> 8), uint8_t(v >> 16), uint8_t(v >> 24)};
                    if (UC.mem_write(uc, p, b, 4) == UC_ERR_OK && r->shadow)
                        sh_written(r->shadow, p, 4);
                }
            }
        }
    }
    int64_t nstub = r->stubbed < 0 ? 0 : r->stubbed;
    int64_t rv = r->unique ? int64_t(RETSENT_BASE) + nstub : r->stub_ret;
    set_reg(uc, UC_X86_REG_EAX, uint64_t(rv) & 0xFFFFFFFFull);
    set_reg(uc, UC_X86_REG_EIP, ret_addr);
    r->stubbed = nstub + 1;
    if (r->shadow)                             // (its result: stub_ret, an input of the run)
        sh_after_call(r->shadow, uc, !r->unique, uint64_t(rv));
    if (r->pending >= 0) {                     // annotate the call record, if via CALL
        Call &cr = r->calls[size_t(r->pending)];
        cr.state = 2;
        cr.resolved = address;
        cr.external = true;
        cr.api = hit;
        cr.has_ret_sentinel = r->unique;
        cr.ret_sentinel = rv;
        uint32_t a[6];
        bool ok = true;
        for (int i = 0; i < 6 && ok; i++)
            ok = read32(uc, esp_now + 4 + 4 * i, a[i]);
        if (ok) {
            cr.has_args = true;
            for (int i = 0; i < 6; i++)
                cr.args[i] = a[i];
            cr.ecx = reg(uc, UC_X86_REG_ECX);
            cr.edx = reg(uc, UC_X86_REG_EDX);
        }
        if (argc_known) {
            cr.has_argc = true;
            cr.argc = argc;
        }
        r->pending = -1;
    }
    return 0;
}

}  // namespace cbt
