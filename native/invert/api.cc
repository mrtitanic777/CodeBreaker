// api.cc -- cbinvert.dll's C interface (cbinvert.h): the text the shim sends parsed, the models
// called, their answer as JSON, a Python exception as its kind and message.
#include "cbinvert.h"

#include <cstdlib>
#include <cstring>
#include <sstream>

#include "flow16.h"

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

}  // namespace

extern "C" {

int cbinv_version(void) { return 1; }

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

}  // extern "C"
