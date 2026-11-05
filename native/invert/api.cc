// api.cc -- cbinvert.dll's C interface (cbinvert.h): the text the shim sends parsed, the models
// called, their answer as JSON, a Python exception as its kind and message.
#include "cbinvert.h"

#include <cstdlib>
#include <cstring>
#include <sstream>

#include "cs.h"
#include "flow16.h"
#include "msc6.h"

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

int cbinv_version(void) { return 2; }

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

}  // extern "C"
