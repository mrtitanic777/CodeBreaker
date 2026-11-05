// api.cc -- cbinvert.dll's C interface (cbinvert.h): the text the shim sends parsed, the models
// called, their answer as JSON, a Python exception as its kind and message.
#include "cbinvert.h"

#include <cstdlib>
#include <cstring>
#include <sstream>

#include "cs.h"
#include "flow16.h"
#include "msc6.h"
#include "msvc2.h"
#include "early_c.h"
#include "pyval.h"

using namespace cbinv;

namespace {

char *dup(const std::string &s) {
  char *p = (char *)std::malloc(s.size() + 1);
  std::memcpy(p, s.c_str(), s.size() + 1);
  return p;
}

std::vector<std::string> split(const std::string &s, char c) {
  std::vector<std::string> out;
  size_t a = 0;
  for (;;) {
    size_t b = s.find(c, a);
    out.push_back(s.substr(a, b == std::string::npos ? std::string::npos : b - a));
    if (b == std::string::npos) break;
    a = b + 1;
  }
  return out;
}

std::vector<long long> nums(const std::string &s) {
  std::vector<long long> v;
  if (s.empty()) return v;
  for (auto &x : split(s, ',')) v.push_back(std::stoll(x));
  return v;
}

template <class F>
int guarded(char **out, F f) {
  try {
    *out = dup(f());
    return CBINV_OK;
  } catch (const PyError &e) {
    *out = dup(e.msg);
    return e.kind;
  } catch (const std::exception &e) {
    *out = dup(e.what());
    return CBINV_OTHER;
  }
}

std::vector<uint8_t> unhex(const std::string &h) {
  std::vector<uint8_t> b;
  auto d = [](char c) { return c <= '9' ? c - '0' : (c | 0x20) - 'a' + 10; };
  for (size_t i = 0; i + 1 < h.size(); i += 2) b.push_back((uint8_t)(d(h[i]) * 16 + d(h[i + 1])));
  return b;
}

}  // namespace

extern "C" {

int cbinv_init(const wchar_t *capstone_dll, char **out) {
  std::string err;
  if (!cs_load(capstone_dll, err)) {
    *out = dup(err);
    return CBINV_OTHER;
  }
  *out = dup("ok");
  return CBINV_OK;
}

int cbinv_msc6(const char *in, char **out) {
  return guarded(out, [&] {
    Msc6In m;
    std::istringstream is(in);
    std::string line;
    while (std::getline(is, line)) {
      auto f = split(line, '\t');
      const std::string &k = f[0];
      if (k == "C" && f.size() >= 2) m.code = unhex(f[1]);
      else if (k == "R" && f.size() >= 2) m.rtype = f[1];
      else if (k == "B" && f.size() >= 2) m.func_base = std::stoll(f[1]);
      else if (k == "P" && f.size() >= 2) { if (std::stoll(f[1]) >= 0) m.params = Params(); }
      else if (k == "p" && f.size() >= 3) m.params->push_back({f[1], f[2]});
      else if (k == "I" && f.size() >= 3) m.imports[std::stoll(f[1])] = f[2];
      else if (k == "Y" && f.size() >= 4) m.syms[std::stoll(f[1])] = {f[2], f[3]};
      else if (k == "N" && f.size() >= 3) m.nearmap[std::stoll(f[1])] = f[2];
    }
    return msc6_invert(m);
  });
}

int cbinv_msc6_sigs(const char *in, char **out) {
  return guarded(out, [&] { return msc6_segment_sigs(unhex(in)); });
}

int cbinv_thunk(const char *in, const char *name, int exact, char **out) {
  return guarded(out, [&] { return segment_thunk(unhex(in), name, exact != 0); });
}

int cbinv_wf(const char *in, char **out) {
  return guarded(out, [&] { return whole_function_candidates(unhex(in)); });
}

int cbinv_mask(const char *in, char **out) {
  return guarded(out, [&] {
    auto b = msc6_mask(unhex(in));
    static const char *D = "0123456789abcdef";
    std::string s;
    for (auto x : b) s += D[x >> 4], s += D[x & 15];
    return s;
  });
}

int cbinv_msc6reg(const char *in, int which, char **out) {
  return guarded(out, [&] {
    std::string code;
    std::map<long long, std::string> imports;
    std::istringstream is(in);
    std::string line;
    while (std::getline(is, line)) {
      auto f = split(line, '\t');
      if (f[0] == "C" && f.size() >= 2) {
        auto b = unhex(f[1]);
        code.assign(b.begin(), b.end());
      } else if (f[0] == "I" && f.size() >= 3) {
        imports[std::stoll(f[1])] = f[2];
      }
    }
    return which == 0 ? msc6_invert_reg(code, imports) : msc6_invert_loop(code, imports);
  });
}

int cbinv_msvc2(const char *in, int which, char **out) {
  return guarded(out, [&] { return msvc2_call(in, which); });
}

int cbinv_version(void) { return 4; }

void cbinv_free(char *p) { std::free(p); }

int cbinv_cfg(const char *in, int cmpswitch, char **out) {
  return guarded(out, [&] {
    std::vector<Ins> insns;
    std::map<long long, std::vector<long long>> sw;
    std::istringstream is(in);
    std::string line;
    while (std::getline(is, line)) {
      auto f = split(line, '\t');
      if (f.size() >= 3 && f[0] == "S") {
        auto v = nums(f[2]);
        sw[std::stoll(f[1])] = v;
      } else if (f.size() >= 3) {
        insns.push_back({std::stoll(f[0]), f[1], f[2]});
      }
    }
    return json_blocks(build_cfg(insns, sw, cmpswitch != 0));
  });
}

int cbinv_structure(const char *in, int entry, int nocap, char **out) {
  return guarded(out, [&] {
    std::vector<Block> blocks;
    std::istringstream is(in);
    std::string line;
    while (std::getline(is, line)) {
      auto f = split(line, '\t');
      if (f.size() < 7) continue;
      Block b;
      b.id = (int)blocks.size();
      b.term = f[0];
      b.lo = std::stoi(f[1]);
      b.hi = std::stoi(f[2]);
      for (auto v : nums(f[3])) b.succ.push_back((int)v);
      if (f[4] != "-") {
        b.has_cases = true;
        if (!f[4].empty())
          for (auto &c : split(f[4], ',')) {
            size_t k = c.find(':');
            b.cases.push_back({std::stoll(c.substr(0, k)), std::stoi(c.substr(k + 1))});
          }
      }
      if (f[5] != "-") b.def = std::stoi(f[5]);
      if (f[6] != "-") {
        b.has_table = true;
        for (auto v : nums(f[6])) b.table.push_back((int)v);
      }
      blocks.push_back(b);
    }
    return json_seq(structure(blocks, entry, nocap != 0));
  });
}

int cbinv_small(const char *in, int which, char **out) {
  return guarded(out, [&] {
    std::vector<uint8_t> code;
    std::map<long long, std::string> names;
    std::map<long long, std::pair<std::string, std::string>> syms;
    std::string callee = "G";
    std::istringstream is(in);
    std::string line;
    while (std::getline(is, line)) {
      auto f = split(line, '\t');
      if (f[0] == "C" && f.size() >= 2) code = unhex(f[1]);
      else if (f[0] == "Y" && f.size() >= 3) names[std::stoll(f[1])] = f[2], syms[std::stoll(f[1])] = {f[2], f.size() > 3 ? f[3] : ""};
      else if (f[0] == "K" && f.size() >= 2) callee = f[1];
    }
    if (which == 0) return msc6_cleanup(code, names);
    if (which == 1) return msc6_walk(code, syms, callee);
    return msc6_templates(code);
  });
}


int cbinv_early_c(const char *in, int which, cbinv_hook_t hook, char **out) {
  return guarded(out, [&]() -> std::string {
    auto strs = [](const V &v) {
      std::vector<std::string> o;
      for (auto &x : v.a) o.push_back(x.s);
      return o;
    };
    auto ints = [](const V &v) {
      std::vector<long long> o;
      for (auto &x : v.a) o.push_back(x.i);
      return o;
    };
    auto jl = [](const std::vector<std::string> &v) {
      std::string o = "[";
      for (size_t k = 0; k < v.size(); k++) o += (k ? "," : "") + jstr(v[k]);
      return o + "]";
    };
    if (which >= 2) {
      V a = from_json(in);
      if (which == 2)
        return jstr(ec_to_c(a[0].s, a[1].i, strs(a[2]), strs(a[3]), a[4].s, ints(a[5]), ints(a[6])));
      if (which == 3) return jl(ec_fold_ladders(strs(a[0]), a[1].truthy()));
      std::string o = "[";
      auto c = ec_detect_copies(strs(a[0]));
      for (size_t k = 0; k < c.size(); k++)
        o += (k ? "," : "") + std::string("[") + std::to_string(c[k].first) + "," + std::to_string(c[k].second) + "]";
      return o + "]";
    }
    std::vector<uint8_t> code;
    long long base = 0;
    std::map<long long, std::string> cxmap;
    EcCfg C;
    EcLast L;
    std::istringstream is(in);
    std::string line;
    auto items = [](const std::string &s) {
      std::vector<std::string> o;
      if (s.empty()) return o;
      return split(s, '\x1f');
    };
    while (std::getline(is, line)) {
      auto f = split(line, '\t');
      const std::string &k = f[0];
      std::string v = f.size() > 1 ? f[1] : "";
      if (k == "C") code = unhex(v);
      else if (k == "B") base = std::stoll(v);
      else if (k == "X" && f.size() >= 3) cxmap[std::stoll(f[1])] = f[2];
      else if (k == "F") {
        bool *fl[] = {&C.mark, &C.farptr, &C.deadregs, &C.unsigned_cast, &C.fpcasts, &C.regvars_eager,
                      &C.frame_after_bp, &C.regparam_reread, &C.masm_cret, &C.cmpswitch, &C.nocap};
        for (size_t q = 0; q < 11 && q < v.size(); q++) *fl[q] = v[q] == '1';
      } else if (k == "V") C.retval = std::stoi(v);
      else if (k == "H") {
        auto h = split(v, ',');
        for (size_t q = 0; q < 7 && q < h.size(); q++) C.hash[q] = std::stoll(h[q]);
      } else if (k == "K" && hook) {
        if (v.find('s') != std::string::npos)
          C.hooks.sw = [hook](long long a, std::vector<long long> &t) {
            std::vector<char> buf(1 << 16);
            int st = hook(0, a, "", "", buf.data(), (int)buf.size());
            if (st == 1) t = nums(std::string(buf.data()));
            return st;
          };
        if (v.find('i') != std::string::npos)
          C.hooks.callinfo = [hook](long long a, const std::string &kind, const std::string &tg, long long &n) {
            std::vector<char> buf(256);
            int st = hook(1, a, kind.c_str(), tg.c_str(), buf.data(), (int)buf.size());
            if (st == 1) n = std::stoll(std::string(buf.data()));
            return st;
          };
        if (v.find('r') != std::string::npos)
          C.hooks.callregs = [hook](long long a, const std::string &kind, const std::string &tg,
                                    std::vector<std::string> &rr) {
            std::vector<char> buf(4096);
            int st = hook(2, a, kind.c_str(), tg.c_str(), buf.data(), (int)buf.size());
            if (st == 1) {
              std::string s(buf.data());
              rr.clear();
              if (!s.empty()) rr = split(s, ',');
            }
            return st;
          };
      } else if (k == "Ls") {
        L.has_saved = true;
        L.saved = items(v);
      } else if (k == "Lr") {
        L.has_regparam = true;
        for (auto &x : items(v)) {
          size_t c = x.find('\x1e');
          L.regparam.push_back({x.substr(0, c), std::stoll(x.substr(c + 1))});
        }
      } else if (k == "Lt") {
        L.has_fptypes = true;
        for (auto &x : items(v)) {
          size_t c = x.find('\x1e');
          L.fptypes.push_back({x.substr(0, c), x.substr(c + 1)});
        }
      } else if (k == "Lf") {
        L.has_fpret = true;
        L.fpret = v;
      } else if (k == "Lu") {
        L.has_ul = true;
        L.unsigned_longs = items(v);
      }
    }
    // (the Python changed LAST in place before it raised: the exception comes back with LAST)
    EcResult r;
    std::string o;
    try {
      r = which == 0 ? ec_recover(code, base, cxmap, C, L) : ec_recover_cfg(code, base, cxmap, C, L);
      o = "{\"fs\":" + std::to_string(r.fs) + ",\"params\":" + jl(r.params) + ",\"stmts\":" + jl(r.stmts) +
          ",\"pt\":" + jstr(r.pt) + ",\"loc\":[";
      for (size_t q = 0; q < r.loc.size(); q++) o += (q ? "," : "") + std::to_string(r.loc[q]);
      o += "],";
    } catch (const PyError &e) {
      o = "{\"err\":" + std::to_string(e.kind) + ",\"msg\":" + jstr(e.msg) + ",";
    } catch (const std::exception &e) {
      o = "{\"err\":" + std::to_string(CBINV_OTHER) + ",\"msg\":" + jstr(e.what()) + ",";
    }
    o += "\"last\":{";
    std::vector<std::string> parts;
    if (L.has_saved) parts.push_back("\"saved\":" + jl(L.saved));
    if (L.has_regparam) {
      std::string x = "\"regparam\":[";
      for (size_t q = 0; q < L.regparam.size(); q++)
        x += (q ? "," : "") + std::string("[") + jstr(L.regparam[q].first) + "," + std::to_string(L.regparam[q].second) + "]";
      parts.push_back(x + "]");
    }
    if (L.has_fptypes) {
      std::string x = "\"fptypes\":[";
      for (size_t q = 0; q < L.fptypes.size(); q++)
        x += (q ? "," : "") + std::string("[") + jstr(L.fptypes[q].first) + "," + jstr(L.fptypes[q].second) + "]";
      parts.push_back(x + "]");
    }
    if (L.has_fpret) parts.push_back("\"fpret\":" + jstr(L.fpret));
    if (L.has_ul) parts.push_back("\"unsigned_longs\":" + jl(L.unsigned_longs));
    for (size_t q = 0; q < parts.size(); q++) o += (q ? "," : "") + parts[q];
    return o + "}}";
  });
}

}  // extern "C"
