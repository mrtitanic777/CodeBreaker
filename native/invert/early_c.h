// early_c.h -- invert/early_c.py in C++ (the 1983-84 Microsoft C 2.03 / 3.00 model): the register
// value tracker (class R: recover, recover_cfg onto flow16.cc directly) and the text passes over its
// statements, decision for decision.  The Python's module settings arrive as an EcCfg; its LAST dict
// goes in and comes back (it carries over from recover to recover_cfg, as in the Python).
#pragma once
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "cs.h"

namespace cbinv {

// The Python's hooks (SWITCH, CALLINFO, CALLREGS), asked through the shim.  status: -1 the hook
// raised, 0 None (or a falsy answer), 1 an answer: SWITCH a list of ints, CALLINFO an int, CALLREGS
// register names.
struct EcHooks {
  std::function<int(long long, std::vector<long long> &)> sw;     // empty: SWITCH is None
  std::function<int(long long, const std::string &, const std::string &, long long &)> callinfo;
  std::function<int(long long, const std::string &, const std::string &, std::vector<std::string> &)> callregs;
};

struct EcCfg {
  bool mark = false, farptr = false, deadregs = false, unsigned_cast = false, fpcasts = false;
  bool regvars_eager = false, frame_after_bp = false, regparam_reread = true;
  bool masm_cret = false;     // os.environ.get("MASM_CRET")
  bool cmpswitch = false;     // flow16.CMPSWITCH
  bool nocap = false;         // os.environ.get("CFG_NOCAP")
  int retval = -1;            // RETVAL: -1 None, else bool(RETVAL)
  long long hash[7] = {0};    // hash() of "ax","bx","cx","dx","si","di","es" in the caller's process
  EcHooks hooks;
};

// LAST, the Python's module dict: only the keys it has.
struct EcLast {
  bool has_saved = false, has_regparam = false, has_fptypes = false, has_fpret = false, has_ul = false;
  std::vector<std::string> saved;
  std::vector<std::pair<std::string, long long>> regparam;
  std::vector<std::pair<std::string, std::string>> fptypes;
  std::string fpret;
  std::vector<std::string> unsigned_longs;   // a set: insertion order kept
};

struct EcResult {
  long long fs = 0;
  std::vector<std::string> params, stmts;
  std::string pt = "int";
  std::vector<long long> loc;
};

EcResult ec_recover(const std::vector<uint8_t> &code, long long base, const std::map<long long, std::string> &cxmap,
                    const EcCfg &cfg, EcLast &last);
EcResult ec_recover_cfg(const std::vector<uint8_t> &code, long long base,
                        const std::map<long long, std::string> &cxmap, const EcCfg &cfg, EcLast &last);

// the text passes (early_c_text.cc)
typedef std::vector<std::string> Lines;
Lines ec_tidy(Lines lines);
Lines ec_strip_tail_returns(Lines lines);
Lines ec_dead_regs(Lines lines);
Lines ec_fold_ladders(Lines lines, bool mark);
Lines ec_fold_goto_or(Lines lines);
Lines ec_fold_longs(const Lines &lines);
std::string ec_fold_long_expr(const std::string &e);
Lines ec_fold_shortcircuit(Lines lines);
Lines ec_dead_store_recover(const Lines &stmts, const std::vector<bool> &rc);
std::vector<std::pair<long long, long long>> ec_detect_copies(const Lines &stmts);
std::string ec_to_c(const std::string &name, long long fs, const std::vector<std::string> &params, const Lines &stmts,
                    const std::string &ptype, const std::vector<long long> &locals, const std::vector<long long> &longset);

}  // namespace cbinv
