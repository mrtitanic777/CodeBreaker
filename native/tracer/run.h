// run.h -- one run's state and record: what trace_function's closures (hook_code, hook_block,
// hook_mem, hook_write, hook_unmapped, hook_intr, _branch_hook) share through `rec` and their
// little lists, here as one struct the hooks get as their user data.
#pragma once
#include <cstdint>
#include <deque>
#include <mutex>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "cbtrace.h"
#include "decode.h"
#include "libs.h"

namespace cbt {

constexpr uint64_t PAGE = 0x1000;
constexpr int64_t MAX_MEM = 40000;          // emulator._MAX_MEM
constexpr int64_t MAX_PATH_ = 20000;        // emulator._MAX_PATH
constexpr int64_t MAX_MAPS = 512;           // emulator._MAX_MAPS
constexpr int64_t MAX_MEMOPS = 3000000;     // emulator._MAX_MEMOPS
constexpr int64_t BRANCH_CAP = 4096;        // emulator._BRANCH_CAP
constexpr uint64_t RETSENT_BASE = 0xB1B10000ull;
constexpr uint64_t OUT_HANDLE = 0x0000A100ull;

// The caches every run shares (emulator._INSN, _DECODED), guarded for runs on several threads.
extern std::mutex cache_lock;

// A code range as in the file, with what was decoded of it (one entry of trace_function's `statics`).
struct Static {
    uint64_t lo = 0, hi = 0;
    std::vector<uint8_t> image;
    std::unordered_map<uint64_t, InsnInfo> insns;            // address -> instruction
    std::unordered_map<uint64_t, Straight> blocks;           // address << 32 | size -> straight run
};
Static *static_by_id(int32_t id);
const Branchy &branchy(uc_engine *uc, int32_t sha_id, uint64_t address);   // _DECODED

struct Call {
    uint64_t pc = 0;
    bool has_target = false;
    uint64_t target = 0;
    bool into_func = false;
    int state = 0;                             // 0 plain, 1 resolved by the next instruction, 2 a stand-in
    uint64_t resolved = 0;
    bool external = false;
    int64_t api = 0;
    bool has_ret_sentinel = false;
    int64_t ret_sentinel = 0;
    bool has_args = false;
    uint64_t args[6] = {0};
    uint64_t ecx = 0, edx = 0;
    bool has_argc = false;
    int64_t argc = 0;
};

struct Compare {                              // the compare in force: (mnemonic, size, a, b, at_a, at_b)
    bool has = false;
    int mn = 0, size = 0;
    uint64_t a = 0, b = 0;
    bool has_at_a = false, has_at_b = false;
    uint64_t at_a = 0, at_b = 0;
};

struct Run {
    uc_engine *uc = nullptr;
    uint32_t flags = 0;
    int32_t sha_id = 0;
    int64_t insn_cap = 0, stub_ret = 0, stub_cleanup = 0;
    int64_t gov_n = 0, gov_ceil = 0;
    uint64_t func_lo = 0, func_hi = 0, sec_lo = 0, sec_hi = 0, pads_lo = 0, pads_hi = 0;
    uint64_t quiet_lo = 0, quiet_hi = 0, lean_low_end = 0, lean_high_begin = 0;
    std::vector<Static *> statics;
    std::unordered_set<uint64_t> init_pages, mapped;
    std::unordered_map<uint64_t, int64_t> sentinels;          // landing address -> place
    cbt_resolver resolver = nullptr;
    std::unordered_map<int64_t, std::vector<int64_t>> answers; // the resolver's answer per import, this run
    bool fast = false, lean = false, reads = false, quiet = false, stop_wild = false, stub = false;
    bool unique = false, branches = false, pads = false, saving = false;

    // memory put back after the run (reuse): pages saved by Python before it, and by the hooks
    std::unordered_set<uint64_t> saved_keys;
    std::vector<uint64_t> saved_base;
    std::vector<std::vector<uint8_t>> saved_data;
    std::vector<bool> saved_none;                            // the page was not there before

    // the record
    int64_t insn_count = 0;
    bool truncated = false, governed = false, dirty = false, ran_into_zeros = false;
    int64_t memops = -1, writes_seen = -1, stuck = -1, bogus = -1, stubbed = -1;
    int64_t lean_seen = 0;
    std::vector<uint64_t> path;
    std::vector<uint64_t> mn_addr;
    std::vector<const std::string *> mn_text;
    std::vector<bool> mn_dead;
    std::unordered_map<uint64_t, size_t> mn_index;
    std::deque<InsnInfo> owned;                              // texts decoded from memory, not the file
    std::vector<Call> calls;
    std::vector<uint64_t> reads_rec, writes_rec;             // CBT_MEM_WORDS / CBT_LEAN_WORDS each
    std::vector<uint64_t> pages0_base;
    std::vector<uint8_t> pages0_data;
    std::unordered_set<uint64_t> pages0_keys;
    std::vector<uint64_t> lazy;
    std::vector<int64_t> deps;
    std::vector<int64_t> api_calls;
    int64_t napi_calls = 0;
    bool api_calls_set = false;
    std::vector<uint64_t> vxd;
    std::vector<uint64_t> branch_log;

    // the hooks' own state
    std::vector<uc_hook> hooks;
    int64_t pending = -1;                                    // a call whose destination is the next instruction
    int zeros = 0;
    bool skip_set = false;
    uint64_t skip = 0;
    bool switched = false;
    const Straight *last = nullptr;                          // the straight run being executed
    std::vector<uint64_t> last_new;                          // ... and the instructions first seen in it
    std::unordered_set<uint64_t> seen_blocks;
    // the branch log's
    bool pend = false;
    uint64_t pend_addr = 0, pend_target = 0, pend_next = 0;
    Compare pend_cmp, cmp;
    std::unordered_map<int, std::pair<uint64_t, int>> loaded; // 32-bit register -> (address, size)

    int64_t hook_error = 0, uc_err = 0;
    cbt_result result;
    std::vector<uint64_t> out_calls, out_mn;
    std::string out_text;

    bool init_page(uint64_t page) const { return init_pages.count(page) != 0; }
    void stop() { UC.emu_stop(uc); }
    bool has_mnemonic(uint64_t a) const { return mn_index.count(a) != 0; }
    void add_mnemonic(uint64_t a, const std::string *text) {
        mn_index[a] = mn_addr.size();
        mn_addr.push_back(a);
        mn_text.push_back(text);
        mn_dead.push_back(false);
    }
    void drop_mnemonic(uint64_t a) {
        auto it = mn_index.find(a);
        if (it != mn_index.end()) {
            mn_dead[it->second] = true;
            mn_index.erase(it);
        }
    }
    void save_page(uint64_t base);                           // what the page holds now, once
};

// the hooks (recorder.cc, memory.cc, imports.cc, branches.cc)
void code_step(Run *r, uint64_t address, uint32_t size);
void hook_code(uc_engine *uc, uint64_t address, uint32_t size, void *user);
void hook_block(uc_engine *uc, uint64_t address, uint32_t size, void *user);
void settle(Run *r);
void hook_mem(uc_engine *uc, uc_mem_type type, uint64_t address, int size, int64_t value, void *user);
void hook_write(uc_engine *uc, uc_mem_type type, uint64_t address, int size, int64_t value, void *user);
bool hook_unmapped(uc_engine *uc, uc_mem_type type, uint64_t address, int size, int64_t value, void *user);
void hook_intr(uc_engine *uc, uint32_t intno, void *user);
int stand_in(Run *r, uint64_t address, int64_t hit);        // 0 answered, 1 go on, 2 stop (Python raised)
void hook_branch(uc_engine *uc, uint64_t address, uint32_t size, void *user);
int restore(Run *r);

}  // namespace cbt
