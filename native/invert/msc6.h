// msc6.h -- the Microsoft C 6.0a model (invert/msc6.py and its helpers) in C++.
#pragma once

#include <cstdint>
#include <map>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace cbinv {

typedef std::vector<std::pair<std::string, std::string>> Params;   // [(name, 'int'|'far'|'long')]

struct Msc6In {
  std::vector<uint8_t> code;
  std::map<long long, std::string> imports;
  std::map<long long, std::pair<std::string, std::string>> syms;   // off -> (name, type)
  std::string rtype = "int";
  std::optional<Params> params;
  std::map<long long, std::string> nearmap;
  long long func_base = 0;
};

// msc6.invert: JSON {"t": text, "m": meta} or {"t": null, "w": reason}
std::string msc6_invert(const Msc6In &in);
// msc6_calls.infer_segment_sigs(_, _, segb): JSON [[offset, [[name, type], ...]], ...]
std::string msc6_segment_sigs(const std::vector<uint8_t> &segb);
// segment_thunk.verify + emit_asm: JSON null, or {"asm": text, "kind": ..., "seg": n, "retf": n|null,
// "body": [[name, hex], ...], "pro": hex, "epi": hex, "pad": hex}; parse when `exact_only` is false
std::string segment_thunk(const std::vector<uint8_t> &fb, const std::string &name, bool exact_only);
// whole_function.candidates: JSON [[template, text], ...]
std::string whole_function_candidates(const std::vector<uint8_t> &fb);
// msc6_function._mask: the bytes with the relocated operands zeroed
std::vector<uint8_t> msc6_mask(const std::vector<uint8_t> &code);
// msc6_function.invert_cleanup: a JSON string (the text)
std::string msc6_cleanup(const std::vector<uint8_t> &b, const std::map<long long, std::string> &syms);
// msc6_walk.invert and msc6_templates.invert: {"t": text, "m": meta} or {"t": null, "w": why}
std::string msc6_walk(const std::vector<uint8_t> &b, const std::map<long long, std::pair<std::string, std::string>> &syms,
                      const std::string &callee);
std::string msc6_templates(const std::vector<uint8_t> &b);

}  // namespace cbinv
