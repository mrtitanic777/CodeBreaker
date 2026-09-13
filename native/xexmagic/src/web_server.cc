// XEXMagic - Interactive web UI backend. See web_server.h.

#include "web_server.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <atomic>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <mutex>
#include <set>
#include <sstream>
#include <thread>
#include <unordered_map>

#include "decompiler_c.h"
#include "import_names.h"
#include "ppc_interp.h"
#include "app_paths.h"
#include "progress.h"
#include "version.h"
#include "sdk_config.h"

#pragma comment(lib, "ws2_32.lib")

namespace {

// ---------------------------------------------------------------------------
// Small string / JSON helpers
// ---------------------------------------------------------------------------

std::string JsonEscape(const std::string& s) {
  std::string o;
  o.reserve(s.size() + 8);
  for (char c : s) {
    switch (c) {
      case '"': o += "\\\""; break;
      case '\\': o += "\\\\"; break;
      case '\n': o += "\\n"; break;
      case '\r': o += "\\r"; break;
      case '\t': o += "\\t"; break;
      default:
        if ((unsigned char)c < 0x20) {
          char b[8];
          snprintf(b, sizeof(b), "\\u%04x", (unsigned char)c);
          o += b;
        } else {
          o += c;
        }
    }
  }
  return o;
}

std::string HexAddr(uint32_t a) {
  char b[16];
  snprintf(b, sizeof(b), "%08X", a);
  return b;
}

// Extract a flat string field from a small JSON object body: "key":"value".
// Good enough for the tiny POST payloads the UI sends; no nesting.
bool JsonGetStr(const std::string& body, const std::string& key,
                std::string* out) {
  std::string needle = "\"" + key + "\"";
  size_t k = body.find(needle);
  if (k == std::string::npos) return false;
  size_t colon = body.find(':', k + needle.size());
  if (colon == std::string::npos) return false;
  size_t q = body.find('"', colon + 1);
  if (q == std::string::npos) return false;
  std::string v;
  for (size_t i = q + 1; i < body.size(); i++) {
    char c = body[i];
    if (c == '\\' && i + 1 < body.size()) {
      char n = body[++i];
      switch (n) {
        case 'n': v += '\n'; break;
        case 't': v += '\t'; break;
        case 'r': v += '\r'; break;
        default: v += n; break;
      }
    } else if (c == '"') {
      *out = v;
      return true;
    } else {
      v += c;
    }
  }
  return false;
}

bool IsIdentChar(char c) {
  return std::isalnum((unsigned char)c) || c == '_';
}

// ---------------------------------------------------------------------------
// Content hashing
//
// A file dropped on the UI has no trustworthy identity: two unrelated titles
// both arrive as default.xex. Hashing the bytes gives one, so each file gets
// its own directory and its own database instead of overwriting a namesake's.
// FNV-1a - a staleness and collision check, not a security boundary.
// ---------------------------------------------------------------------------

#ifdef _WIN32
const char kSep = '\\';
#else
const char kSep = '/';
#endif

uint64_t HashBytes(const void* data, size_t len) {
  uint64_t h = 1469598103934665603ull;
  const uint8_t* p = (const uint8_t*)data;
  for (size_t i = 0; i < len; i++) {
    h ^= p[i];
    h *= 1099511628211ull;
  }
  return h;
}

std::string HashHex(uint64_t h) {
  char b[17];
  snprintf(b, sizeof(b), "%016llX", (unsigned long long)h);
  return b;
}

// Content hash and size of a file on disk. False when it cannot be read.
bool HashFile(const std::string& path, uint64_t* hash, uint64_t* size) {
  FILE* fp = fopen(path.c_str(), "rb");
  if (!fp) return false;
  uint64_t h = 1469598103934665603ull;
  uint64_t n = 0;
  std::vector<uint8_t> buf(1 << 20);
  for (;;) {
    size_t got = fread(buf.data(), 1, buf.size(), fp);
    if (!got) break;
    n += got;
    for (size_t i = 0; i < got; i++) {
      h ^= buf[i];
      h *= 1099511628211ull;
    }
  }
  fclose(fp);
  *hash = h;
  *size = n;
  return true;
}

// Whole-word replace of `from` with `to` (identifier boundaries), used to
// apply a local-variable rename to already-emitted pseudocode.
std::string ReplaceWord(const std::string& text, const std::string& from,
                        const std::string& to) {
  if (from.empty()) return text;
  std::string out;
  out.reserve(text.size());
  size_t i = 0;
  while (i < text.size()) {
    size_t p = text.find(from, i);
    if (p == std::string::npos) {
      out.append(text, i, std::string::npos);
      break;
    }
    bool lok = (p == 0) || !IsIdentChar(text[p - 1]);
    bool rok = (p + from.size() >= text.size()) ||
               !IsIdentChar(text[p + from.size()]);
    out.append(text, i, p - i);
    if (lok && rok) {
      out += to;
    } else {
      out += from;
    }
    i = p + from.size();
  }
  return out;
}

// ---------------------------------------------------------------------------
// Naming heuristics
// ---------------------------------------------------------------------------

std::string Lower(std::string s) {
  for (auto& c : s) c = (char)std::tolower((unsigned char)c);
  return s;
}

// Probable local-variable name from the callee that produced it.
std::string SuggestFromCallee(const std::string& callee) {
  struct Rule { const char* sub; const char* name; };
  static const Rule kExact[] = {
      {"IoAllocateIrp", "irp"},
      {"IoGetCurrentIrpStackLocation", "sl"},
      {"IoGetNextIrpStackLocation", "sl"},
      {"KeGetCurrentThread", "thread"},
      {"KeGetCurrentProcessType", "procType"},
      {"ObReferenceObjectByHandle", "status"},
      {"ObpReferenceObjectByHandle", "status"},
  };
  for (auto& r : kExact)
    if (callee == r.sub) return r.name;

  // Substring rules, most specific first.
  struct Sub { const char* sub; const char* name; };
  static const Sub kSubs[] = {
      {"AllocatePool", "pool"}, {"Allocate", "mem"},   {"MapView", "base"},
      {"CreateFile", "handle"}, {"OpenFile", "handle"}, {"Create", "obj"},
      {"Open", "obj"},          {"Reference", "obj"},   {"Lookup", "obj"},
      {"Acquire", "lock"},      {"Wait", "status"},
  };
  for (auto& r : kSubs)
    if (callee.find(r.sub) != std::string::npos) return r.name;

  // Nt/Zw wrappers return NTSTATUS.
  if (callee.rfind("Nt", 0) == 0 || callee.rfind("Zw", 0) == 0) return "status";

  // Generic: drop a known 2-3 char subsystem prefix, lowercase the rest.
  static const char* kPrefixes[] = {"Ke", "Ki", "Io", "Iop", "Ob", "Ex",
                                     "Rtl", "Mm", "Ps", "Xam", "Xex"};
  std::string base = callee;
  for (const char* p : kPrefixes) {
    size_t n = std::strlen(p);
    if (base.size() > n && base.compare(0, n, p) == 0 &&
        std::isupper((unsigned char)base[n])) {
      base = base.substr(n);
      break;
    }
  }
  if (base.empty()) return "";
  return Lower(base.substr(0, 1)) + base.substr(1);
}

// Shared struct type produced by a known allocator/accessor, or "" if the
// callee doesn't imply a well-known pointer type. Drives automatic unification:
// every `v = IoAllocateIrp(...)` in any function binds to one "IRP" type.
std::string TypeFromCallee(const std::string& callee) {
  struct Rule { const char* sub; const char* type; };
  static const Rule kRules[] = {
      {"IoAllocateIrp", "IRP"},
      {"IoGetCurrentIrpStackLocation", "IO_STACK_LOCATION"},
      {"IoGetNextIrpStackLocation", "IO_STACK_LOCATION"},
      {"KeGetCurrentThread", "KTHREAD"},
  };
  for (auto& r : kRules)
    if (callee == r.sub) return r.type;
  return "";
}

// ---------------------------------------------------------------------------
// Model
// ---------------------------------------------------------------------------

class Model {
 public:
  // Largest /api/mem read, in bytes. Big enough to sweep a section without
  // hundreds of round trips.
  static constexpr uint32_t kMaxMemLen = 0x100000;

  explicit Model(WebProgram prog) : prog_(std::move(prog)) {
    for (size_t i = 0; i < prog_.funcs.size(); i++)
      index_[prog_.funcs[i].entry] = i;
    HashSourceFile();
    NameImportThunks();
    BuildXrefs();
    LoadDb();
    // A cache from a previous run of the same file makes the expensive passes
    // unnecessary; otherwise do the work once and keep it. The library scan
    // runs before the prototypes, because those are matched by name and the
    // scan is what supplies the names on a stripped image.
    xexmagic::progress::BeginPhase("loading the saved analysis");
    bool cached = LoadAnalysis();
    if (!cached) {
      AutoScanSdk();
      // After the scan, so a shim the SDK identified by its bytes keeps that
      // name, and before the prototypes, which are matched by name.
      NameImportShims();
    }
    ApplySdkPrototypes();
    if (!cached) {
      InferTypes();
      xexmagic::progress::BeginPhase("saving the analysis");
      SaveAnalysis();
    }
  }

  const WebProgram& prog() const { return prog_; }

  std::string NameOf(uint32_t addr) {
    auto it = prog_.symbols.find(addr);
    if (it != prog_.symbols.end()) return it->second;
    return "sub_" + HexAddr(addr);
  }

  const ILFunction* Find(uint32_t addr) const {
    auto it = index_.find(addr);
    if (it == index_.end()) return nullptr;
    return &prog_.funcs[it->second];
  }

  // Find, or build on demand for a code address detection missed. Newly built
  // functions join the program so they get xrefs, names and links like any
  // other.
  const ILFunction* FindOrBuild(uint32_t addr) {
    const ILFunction* f = Find(addr);
    if (f) return f;
    if (!prog_.build_at) return nullptr;
    ILFunction nf;
    if (!prog_.build_at(addr, &nf)) return nullptr;
    prog_.funcs.push_back(std::move(nf));
    index_[addr] = prog_.funcs.size() - 1;
    printf("Built on demand: %08X\n", addr);
    fflush(stdout);
    return &prog_.funcs.back();
  }

  // ---- JSON producers ----

  // The file this model was built from, without its directory.
  std::string FileName() const {
    std::string p = prog_.input_path;
    size_t s = p.find_last_of("/\\");
    return s == std::string::npos ? p : p.substr(s + 1);
  }

  std::string MetaJson() {
    std::ostringstream o;
    o << "{\"version\":\"" << xexmagic::kVersion << "\",\"file\":\""
      << JsonEscape(FileName()) << "\",\"path\":\""
      << JsonEscape(prog_.input_path) << "\",\"name\":\""
      << JsonEscape(prog_.module_name) << "\",\"base\":\""
      << HexAddr(prog_.base) << "\",\"count\":" << prog_.funcs.size()
      << ",\"entry\":\"" << HexAddr(prog_.entry_point) << "\",\"strings\":"
      << prog_.strings.size() << "}";
    return o.str();
  }

  // Data cross-references straight from the IL, split by direction. Text
  // search can only show one line per function and cannot tell a read from a
  // write; this can, which is what the search-only version got wrong.
  // Collected during the whole-program render (see InferTypes), because only
  // there is the lis/addi pair folded into a real address. Doing it from the
  // raw IL gives just the 16-bit displacement, which matches nothing.
  void CollectDataRefs(uint32_t func_entry) {
    for (const auto& r : GetDataRefs())
      (r.write ? data_writes_ : data_reads_)[r.addr].insert(func_entry);
  }

  // Reads and writes of one address, each with the functions involved.
  std::string DataXrefJson(uint32_t addr) {
    std::ostringstream o;
    o << "{\"addr\":\"" << HexAddr(addr) << "\",\"reads\":";
    auto emit = [&](std::map<uint32_t, std::set<uint32_t>>& m) {
      std::ostringstream s;
      s << "[";
      auto it = m.find(addr);
      if (it != m.end()) {
        bool first = true;
        for (uint32_t fa : it->second) {
          if (!first) s << ",";
          first = false;
          s << "{\"addr\":\"" << HexAddr(fa) << "\",\"name\":\""
            << JsonEscape(NameOf(fa)) << "\"}";
        }
      }
      s << "]";
      return s.str();
    };
    o << emit(data_reads_) << ",\"writes\":" << emit(data_writes_) << "}";
    return o.str();
  }

  // ---- raw image access -------------------------------------------------

  // Runs a function out of the image so a conclusion can be tested rather than
  // argued. Body is JSON: entry, args[], writes[{addr,hex}], stubs{addr:val},
  // dumps[{addr,len}].
  std::string RunJson(const std::string& body) {
    InterpRequest rq;
    auto hexval = [](const std::string& s) {
      return (uint32_t)strtoul(s.c_str(), nullptr, 16);
    };
    std::string v;
    if (JsonGetStr(body, "entry", &v)) rq.entry = hexval(v);
    // args: "r3,r4,..." as hex, comma separated
    if (JsonGetStr(body, "args", &v)) {
      size_t p = 0;
      while (p < v.size()) {
        size_t e = v.find(',', p);
        rq.args.push_back(hexval(v.substr(p, e == std::string::npos ? e : e - p)));
        if (e == std::string::npos) break;
        p = e + 1;
      }
    }
    // writes: "addr:hexbytes,addr:hexbytes" (';' also accepted).
    // Both separators are taken because stubs and dumps use ',' and only this
    // field used ';'. A ',' here used to fall inside the hex run, where the
    // pair loop silently skipped the non-hex characters and produced a
    // plausible-looking but wrong byte string - every entry after the first
    // was quietly dropped. Non-hex now fails the request instead.
    if (JsonGetStr(body, "writes", &v)) {
      size_t p = 0;
      while (p < v.size()) {
        size_t e = v.find_first_of(";,", p);
        std::string one = v.substr(p, e == std::string::npos ? e : e - p);
        size_t c = one.find(':');
        if (c != std::string::npos) {
          InterpMemWrite mw;
          mw.addr = hexval(one.substr(0, c));
          std::string h = one.substr(c + 1);
          if (h.size() % 2 != 0)
            return "{\"error\":\"writes: odd hex digit count in one entry\"}";
          for (size_t i = 0; i < h.size(); i += 2) {
            if (!isxdigit((unsigned char)h[i]) || !isxdigit((unsigned char)h[i + 1]))
              return "{\"error\":\"writes: non-hex character in data\"}";
            mw.data.push_back((uint8_t)strtoul(h.substr(i, 2).c_str(), nullptr, 16));
          }
          rq.writes.push_back(std::move(mw));
        }
        if (e == std::string::npos) break;
        p = e + 1;
      }
    }
    // stubs: "addr=val,addr=val"
    if (JsonGetStr(body, "stubs", &v)) {
      size_t p = 0;
      while (p < v.size()) {
        size_t e = v.find(',', p);
        std::string one = v.substr(p, e == std::string::npos ? e : e - p);
        size_t q = one.find('=');
        if (q != std::string::npos)
          rq.stubs[hexval(one.substr(0, q))] = hexval(one.substr(q + 1));
        if (e == std::string::npos) break;
        p = e + 1;
      }
    }
    // dumps: "addr:len,addr:len"
    if (JsonGetStr(body, "dumps", &v)) {
      size_t p = 0;
      while (p < v.size()) {
        size_t e = v.find(',', p);
        std::string one = v.substr(p, e == std::string::npos ? e : e - p);
        size_t c = one.find(':');
        if (c != std::string::npos) {
          InterpDump d;
          d.addr = hexval(one.substr(0, c));
          d.len = hexval(one.substr(c + 1));
          if (d.len > 0x4000) d.len = 0x4000;
          rq.dumps.push_back(d);
        }
        if (e == std::string::npos) break;
        p = e + 1;
      }
    }
    // Run the XDK memory routines natively. They are VMX (lvsl/vperm/stvx) and
    // the VMX ALU is not modelled, so stepping them moves no bytes -- the
    // fastfile driver opens with XMemSet over its whole context, and without
    // this every later read sees stale memory and the run "fails" for reasons
    // that have nothing to do with the code under test.
    for (const auto& sym : prog_.symbols) {
      const std::string& n = sym.second;
      if (n == "XMemSet" || n == "memset")
        rq.intrinsics[sym.first] = InterpRequest::kMemset;
      else if (n == "XMemCpy" || n == "memcpy" || n == "_blkmov")
        rq.intrinsics[sym.first] = InterpRequest::kMemcpy;
      else if (n == "XMemMove" || n == "memmove")
        rq.intrinsics[sym.first] = InterpRequest::kMemmove;
    }
    if (JsonGetStr(body, "steps", &v)) rq.max_steps = strtoull(v.c_str(), nullptr, 10);
    if (JsonGetStr(body, "trace", &v)) rq.trace_steps = (uint32_t)atoi(v.c_str());
    if (JsonGetStr(body, "watch_mem", &v)) {
      // "lo-hi" in hex, e.g. "838AD238-838AD24F"
      size_t dash = v.find('-');
      rq.watch_mem_lo =
          (uint32_t)strtoul(v.substr(0, dash).c_str(), nullptr, 16);
      if (dash != std::string::npos)
        rq.watch_mem_hi =
            (uint32_t)strtoul(v.substr(dash + 1).c_str(), nullptr, 16);
    }
    if (JsonGetStr(body, "trace_from", &v))
      rq.trace_from = strtoull(v.c_str(), nullptr, 10);
    if (JsonGetStr(body, "trace_pc", &v)) {
      // "lo-hi" in hex, e.g. "822FA300-822FA620"
      size_t dash = v.find('-');
      rq.trace_pc_lo = (uint32_t)strtoul(v.substr(0, dash).c_str(), nullptr, 16);
      if (dash != std::string::npos)
        rq.trace_pc_hi =
            (uint32_t)strtoul(v.substr(dash + 1).c_str(), nullptr, 16);
    }
    // "watch":"r18" or "watch":"18" - the register whose every change is logged
    // with the instruction that made it.
    if (JsonGetStr(body, "watch", &v)) {
      const char* p = v.c_str();
      if (*p == 'r' || *p == 'R') p++;
      rq.watch_gpr = atoi(p);
    }

    InterpResult r = PPCInterpRun(rq, &Model::ReadImageByte, this);

    std::ostringstream o;
    o << "{\"ok\":" << (r.ok ? "true" : "false") << ",\"error\":\""
      << JsonEscape(r.error) << "\",\"r3\":\"" << HexAddr(r.r3)
      << "\",\"steps\":" << r.steps << ",\"vmx_skipped\":" << r.vmx_skipped
      << ",\"last_pc\":\"" << HexAddr(r.last_pc) << "\",\"calls\":[";
    {
      // Every call is emitted, not one per distinct target: the arguments are
      // the point, and the same function called twice with different arguments
      // is exactly the case worth seeing. Capped so a long run cannot produce
      // an unbounded response.
      const size_t kMaxCalls = 400;
      bool first = true;
      size_t n = 0;
      for (const auto& c : r.calls) {
        if (n++ >= kMaxCalls) break;
        if (!first) o << ",";
        first = false;
        o << "{\"addr\":\"" << HexAddr(c.target) << "\",\"name\":\""
          << JsonEscape(NameOf(c.target)) << "\",\"lr\":\"" << HexAddr(c.lr)
          << "\",\"args\":[";
        for (int i = 0; i < 8; i++) {
          if (i) o << ",";
          o << "\"" << HexAddr(c.arg[i]) << "\"";
        }
        o << "]}";
      }
    }
    // Kernel imports trampolined during the run. `unimplemented` is the list
    // worth reading: it names exactly what is missing, in one pass.
    o << "],\"kernel_calls\":[";
    {
      bool first = true;
      for (const auto& k : r.kernel_calls) {
        if (!first) o << ",";
        first = false;
        o << "{\"thunk\":\"" << HexAddr(k.thunk) << "\",\"ordinal\":\""
          << HexAddr(k.ordinal) << "\",\"name\":\"" << JsonEscape(k.name)
          << "\",\"implemented\":" << (k.implemented ? "true" : "false")
          << ",\"r3\":\"" << HexAddr(k.r3) << "\",\"args\":[";
        for (int i = 0; i < 4; i++)
          o << (i ? "," : "") << "\"" << HexAddr(k.arg[i]) << "\"";
        o << "]}";
      }
    }
    o << "],\"unimplemented\":[";
    {
      std::map<uint32_t, std::string> miss;
      for (const auto& k : r.kernel_calls)
        if (!k.implemented) miss[k.ordinal] = k.name;
      bool first = true;
      for (const auto& m : miss) {
        if (!first) o << ",";
        first = false;
        o << "{\"ordinal\":\"" << HexAddr(m.first) << "\",\"name\":\""
          << JsonEscape(m.second) << "\"}";
      }
    }
    o << "],\"dumps\":[";
    bool dfirst = true;
    for (const auto& d : r.dumps) {
      if (!dfirst) o << ",";
      dfirst = false;
      std::ostringstream hx;
      for (uint8_t b : d.data) {
        char t[4];
        snprintf(t, sizeof(t), "%02X", b);
        hx << t;
      }
      o << "{\"addr\":\"" << HexAddr(d.addr) << "\",\"hex\":\"" << hx.str()
        << "\"}";
    }
    o << "],\"trace\":[";
    bool tfirst = true;
    for (const auto& t : r.trace) {
      if (!tfirst) o << ",";
      tfirst = false;
      o << "{\"pc\":\"" << HexAddr(t.pc) << "\",\"w\":\"" << HexAddr(t.word)
        << "\",\"r\":[";
      for (int i = 0; i < 12; i++) {
        if (i) o << ",";
        o << "\"" << HexAddr(t.gpr[i]) << "\"";
      }
      o << "]}";
    }
    o << "],\"mem_watch\":[";
    bool mfirst = true;
    for (const auto& h : r.mem_watch) {
      if (!mfirst) o << ",";
      mfirst = false;
      o << "{\"pc\":\"" << HexAddr(h.pc) << "\",\"addr\":\"" << HexAddr(h.addr)
        << "\",\"old\":" << (int)h.old_value << ",\"new\":" << (int)h.new_value
        << ",\"step\":" << h.step << "}";
    }
    o << "],\"watch\":[";
    bool wfirst = true;
    for (const auto& h : r.watch) {
      if (!wfirst) o << ",";
      wfirst = false;
      o << "{\"pc\":\"" << HexAddr(h.pc) << "\",\"old\":\""
        << HexAddr(h.old_value) << "\",\"new\":\"" << HexAddr(h.new_value)
        << "\",\"step\":" << h.step << "}";
    }
    o << "]}";
    return o.str();
  }

  static int ReadImageByte(void* ctx, uint32_t va) {
    return static_cast<Model*>(ctx)->ByteAt(va);
  }

  // Match every unnamed function against the SDK library signatures.
  //
  // Only exact, size-equal, mask-aware comparisons count: linking rewrites the
  // relocated fields and nothing else, so a real match is byte-identical
  // everywhere the archive did not mark as relocatable. Functions that already
  // carry a name are left alone -- a name someone chose beats a guess.
  //
  // Library code the compiler inlined into a larger function. Reported as
  // annotations, never renames: the host function CONTAINS this code, it is not
  // that function, so renaming it would be wrong.
  std::string SdkInlineJson() {
    auto& sdk = xexmagic::SdkConfig::Get();
    std::ostringstream o;
    o << "{\"funcs\":[";

    size_t funcs = 0, total = 0;
    bool first = true;
    std::vector<uint8_t> buf;
    std::map<std::string, uint32_t> occ_of;  // symbol -> times seen in this fn

    for (const auto& f : prog_.funcs) {
      if (f.end <= f.entry) continue;
      const uint32_t size = f.end - f.entry;
      if (size < 64) continue;  // too small to have inlined anything
      occ_of.clear();

      buf.resize(size);
      bool readable = true;
      for (uint32_t i = 0; i < size; i++) {
        int b = ByteAt(f.entry + i);
        if (b < 0) { readable = false; break; }
        buf[i] = (uint8_t)b;
      }
      if (!readable) continue;

      auto hits = sdk.FindInlined(buf.data(), size);
      if (hits.empty()) continue;

      // One symbol matching at many scattered offsets is a repeated code
      // pattern, not that routine inlined N times -- near-identical library
      // functions (XACT's lookup helpers, the CRT string loops) share their
      // shape. Keep the longest run per symbol and say how many places it hit,
      // so the count reflects evidence rather than repetition.
      {
        std::map<std::string, xexmagic::SdkConfig::InlineHit> best;
        std::map<std::string, uint32_t> occurrences;
        for (const auto& h : hits) {
          occurrences[h.name]++;
          auto it = best.find(h.name);
          if (it == best.end() || h.insns > it->second.insns) best[h.name] = h;
        }
        std::vector<xexmagic::SdkConfig::InlineHit> collapsed;
        for (auto& [name, h] : best) {
          occ_of[name] = occurrences[name];
          collapsed.push_back(h);
        }
        std::sort(collapsed.begin(), collapsed.end(),
                  [](const auto& a, const auto& b) { return a.insns > b.insns; });
        hits.swap(collapsed);
      }

      // A whole-function match is not "inlined" -- it is that function, and the
      // rename pass already covers it.
      if (hits.size() == 1 && hits[0].offset == 0 &&
          hits[0].insns * 4 >= size - 8) {
        continue;
      }

      funcs++;
      if (!first) o << ",";
      first = false;
      o << "{\"addr\":\"" << HexAddr(f.entry) << "\",\"name\":\""
        << JsonEscape(NameOf(f.entry)) << "\",\"size\":" << size
        << ",\"hits\":[";
      for (size_t i = 0; i < hits.size(); i++) {
        if (i) o << ",";
        o << "{\"at\":" << hits[i].offset << ",\"insns\":" << hits[i].insns
          << ",\"occurrences\":" << occ_of[hits[i].name]
          << ",\"name\":\"" << JsonEscape(hits[i].name) << "\",\"demangled\":\""
          << JsonEscape(hits[i].demangled) << "\",\"lib\":\""
          << JsonEscape(hits[i].lib) << "\"}";
        total++;
      }
      o << "]}";
    }

    o << "],\"funcs_with_inlined\":" << funcs << ",\"regions\":" << total << "}";
    return o.str();
  }

  // `apply` renames in place; otherwise the matches are returned for review.
  // Byte-exact match of one function against the indexed SDK code signatures,
  // with COFF relocation fields masked out. Returns null when nothing matches
  // or when two different names match the same bytes - bytes that identify two
  // functions identify neither, and naming it would be a coin flip.
  const xexmagic::SdkConfig::CodeSig* MatchSdkCode(const ILFunction& f,
                                                   std::vector<uint8_t>& buf,
                                                   bool* ambiguous) {
    auto& sdk = xexmagic::SdkConfig::Get();
    if (ambiguous) *ambiguous = false;
    if (f.end <= f.entry) return nullptr;
    const uint32_t size = f.end - f.entry;

    const auto* cands = sdk.SigsForSize(size);
    if (!cands || cands->empty()) return nullptr;

    buf.resize(size);
    for (uint32_t i = 0; i < size; i++) {
      int b = ByteAt(f.entry + i);
      if (b < 0) return nullptr;
      buf[i] = (uint8_t)b;
    }

    const xexmagic::SdkConfig::CodeSig* hit = nullptr;
    int hits = 0;
    for (const auto& sig : *cands) {
      if (xexmagic::SdkConfig::HashMasked(buf.data(), size, sig.mask) ==
          sig.hash) {
        if (!hit || sig.name != hit->name) hits++;
        if (!hit) hit = &sig;
      }
    }
    if (!hit) return nullptr;
    if (hits > 1) {
      if (ambiguous) *ambiguous = true;
      return nullptr;
    }
    return hit;
  }

  // The name of an import belongs on its thunk, so every call site in the
  // image renders `NetDll_WSARecvFrom(...)` rather than `sub_82951714(...)`.
  // Thunks are not function entries, so this never collides with a real
  // function and SaveDb already skips anything that is not one - the .xdb
  // stays free of it. Runs before everything else, including a cached load,
  // because it is what the rest of the render reads.
  void NameImportThunks() {
    for (const auto& imp : prog_.imports) {
      if (imp.name.empty()) continue;
      for (uint32_t a : {imp.thunk_address, imp.value_address}) {
        if (!a) continue;
        if (prog_.symbols.count(a)) continue;
        prog_.symbols[a] = imp.name;
      }
    }
  }

  // A function whose whole body is "call this import and return" is a shim,
  // and `sub_82xxxxxx` tells you nothing about it. Naming it after what it
  // wraps is the whole content of the function.
  //
  // Deliberately strict: one import target, no other calls, and every
  // instruction is frame setup, a register move, a stack spill or the call
  // itself. Anything that computes, touches memory off the stack, or branches
  // is doing work of its own and keeps its address name.
  void NameImportShims() {
    if (prog_.imports.empty()) return;
    std::map<uint32_t, const WebProgram::Import*> by_addr;
    for (const auto& imp : prog_.imports) {
      if (imp.thunk_address) by_addr[imp.thunk_address] = &imp;
      if (imp.value_address) by_addr[imp.value_address] = &imp;
    }

    xexmagic::progress::BeginPhase("naming import shims", prog_.funcs.size());
    size_t named = 0;
    for (const auto& f : prog_.funcs) {
      xexmagic::progress::Complete();
      if (prog_.symbols.count(f.entry)) continue;  // already has a real name

      const WebProgram::Import* target = nullptr;
      bool shim = true;
      for (const auto& b : f.blocks) {
        if (!shim) break;
        for (const auto& in : b.instrs) {
          switch (in.op) {
            case ILOp::Call:
            case ILOp::Branch:
            case ILOp::BranchCTR:
            case ILOp::CallCTR: {
              auto it = in.target ? by_addr.find(in.target) : by_addr.end();
              if (it == by_addr.end()) {
                // The register save/restore helpers are part of the frame,
                // not something the wrapper does.
                std::string callee = in.target ? NameOf(in.target) : "";
                if (callee.rfind("__save", 0) == 0 ||
                    callee.rfind("__rest", 0) == 0)
                  break;
                shim = false;
                break;
              }
              // Two different imports is a dispatcher, not a shim.
              if (target && target != it->second) { shim = false; break; }
              target = it->second;
              break;
            }
            // Frame, linkage and register shuffling: what a wrapper is made of.
            case ILOp::Return:
            case ILOp::BranchLR:
            case ILOp::Assign:
            case ILOp::MoveSPR:
            case ILOp::StackAlloc:
            case ILOp::Nop:
              break;
            // Tearing the frame back down: `addi r1, r1, N`. Only when both
            // sides are the stack pointer - any other arithmetic is work.
            case ILOp::Add:
              if (in.dst.ppc_reg != 1 || in.src1.ppc_reg != 1) shim = false;
              break;
            // Spills only. A load or store through anything but the stack
            // pointer means it is reading or writing real state.
            case ILOp::Load:
            case ILOp::FLoad:
            case ILOp::LoadMultiple:
              if (in.src1.ppc_reg != 1) shim = false;
              break;
            case ILOp::Store:
            case ILOp::FStore:
            case ILOp::StoreMultiple:
              if (in.src1.ppc_reg != 1) shim = false;
              break;
            default:
              shim = false;
              break;
          }
          if (!shim) break;
        }
      }
      if (!shim || !target) continue;

      std::string what = target->name;
      if (what.empty()) {
        char ord[32];
        snprintf(ord, sizeof(ord), "ordinal_%u", target->ordinal);
        what = ord;
      }
      const std::string name = "Call_" + what;
      prog_.symbols[f.entry] = name;
      sdk_names_[f.entry] = name;  // derived, so it stays out of the .xdb
      named++;
    }
    printf("Import shims: named %zu wrapper functions\n", named);
    fflush(stdout);
  }

  // First analysis only: name statically linked SDK code before anything else
  // runs, so the prototypes, propagation and every later render see the real
  // names. The results are derived, not authored - they go in the analysis
  // cache, never in the .xdb.
  void AutoScanSdk() {
    auto& sdk = xexmagic::SdkConfig::Get();
    if (!sdk.indexed() || !sdk.symbol_count()) return;
    // A .lib is part of the SDK. Matching it against the SDK's own signatures
    // would only rediscover the archive in itself, and its COFF symbols
    // already carry the real names.
    if (prog_.is_archive) {
      printf("SDK scan: skipped, the input is a library\n");
      fflush(stdout);
      return;
    }
    const size_t nfuncs = prog_.funcs.size();
    xexmagic::progress::BeginPhase("matching statically linked SDK code",
                                   nfuncs);
    // Matching is a read of the image against the index: independent per
    // function, so it runs wide and the names are applied in address order.
    std::vector<std::string> hits(nfuncs);
    std::atomic<size_t> next{0};
    std::atomic<size_t> ambiguous{0};
    unsigned nthreads = std::thread::hardware_concurrency();
    if (nthreads == 0) nthreads = 4;
    auto worker = [&]() {
      std::vector<uint8_t> buf;
      for (;;) {
        size_t i = next.fetch_add(1);
        if (i >= nfuncs) return;
        const auto& f = prog_.funcs[i];
        xexmagic::progress::SetCurrent(NameOf(f.entry));
        // A name already present is either the image's own or the analyst's.
        if (!prog_.symbols.count(f.entry)) {
          bool amb = false;
          const auto* hit = MatchSdkCode(f, buf, &amb);
          if (amb) ambiguous.fetch_add(1);
          // The readable form where DbgHelp produced one. `?Simplify@Compiler@
          // D3DXShader@@AAA_NPAVInstruction@2@PA_N@Z` is the same fact as
          // `bool D3DXShader::Compiler::Simplify(Instruction *, bool *)` and
          // nobody reads the first one.
          if (hit) hits[i] = hit->demangled.empty() ? hit->name : hit->demangled;
        }
        xexmagic::progress::Complete();
      }
    };
    std::vector<std::thread> pool;
    for (unsigned t = 1; t < nthreads; t++) pool.emplace_back(worker);
    worker();
    for (auto& t : pool) t.join();

    size_t named = 0;
    for (size_t i = 0; i < nfuncs; i++) {
      if (hits[i].empty()) continue;
      prog_.symbols[prog_.funcs[i].entry] = hits[i];
      sdk_names_[prog_.funcs[i].entry] = hits[i];
      named++;
    }
    printf("SDK scan: named %zu functions, %zu ambiguous, on %u threads\n",
           named, ambiguous.load(), nthreads);
    fflush(stdout);
  }

  std::string SdkScanJson(bool apply) {
    std::ostringstream o;
    o << "{\"matches\":[";

    size_t found = 0, renamed = 0, ambiguous = 0;
    bool first = true;
    std::vector<uint8_t> buf;

    for (const auto& f : prog_.funcs) {
      // Named already? Leave it.
      if (prog_.symbols.count(f.entry)) continue;

      bool amb = false;
      const auto* hit = MatchSdkCode(f, buf, &amb);
      if (amb) ambiguous++;
      if (!hit) continue;

      found++;
      if (apply) {
        prog_.symbols[f.entry] =
            hit->demangled.empty() ? hit->name : hit->demangled;
        renamed++;
      }

      if (!first) o << ",";
      first = false;
      o << "{\"addr\":\"" << HexAddr(f.entry)
        << "\",\"size\":" << (f.end - f.entry) << ",\"name\":\""
        << JsonEscape(hit->name) << "\",\"demangled\":\""
        << JsonEscape(hit->demangled) << "\",\"lib\":\""
        << JsonEscape(hit->lib) << "\"}";
    }

    if (apply && renamed) {
      code_cache_.clear();
      SaveDb();
    }

    o << "],\"found\":" << found << ",\"renamed\":" << renamed
      << ",\"ambiguous\":" << ambiguous << "}";
    return o.str();
  }

  // Section map, so the memory view can offer somewhere to start.
  std::string SectionsJson() {
    std::ostringstream o;
    o << "[";
    bool first = true;
    for (const auto& s : prog_.sections) {
      if (!first) o << ",";
      first = false;
      o << "{\"name\":\"" << JsonEscape(s.name) << "\",\"addr\":\""
        << HexAddr(s.address) << "\",\"size\":\"" << HexAddr(s.size)
        << "\",\"code\":" << (s.code ? "true" : "false") << "}";
    }
    o << "]";
    return o.str();
  }

  // Byte at a virtual address, or -1 when it falls outside every section.
  int ByteAt(uint32_t va) const {
    if (!prog_.image) return -1;
    for (const auto& s : prog_.sections) {
      if (va >= s.address && va < s.address + s.size) {
        size_t off = s.file_offset + (va - s.address);
        if (off < prog_.image->size()) return (*prog_.image)[off];
      }
    }
    return -1;
  }

  // True when `va` lands in an executable section and is instruction-aligned.
  bool InCode(uint32_t va) const {
    if (va & 3) return false;
    for (const auto& s : prog_.sections)
      if (s.code && va >= s.address && va < s.address + s.size) return true;
    return false;
  }

  bool U32At(uint32_t va, uint32_t* out) const {
    uint32_t v = 0;
    for (int i = 0; i < 4; i++) {           // big-endian, as the target reads
      int b = ByteAt(va + i);
      if (b < 0) return false;
      v = (v << 8) | (uint32_t)b;
    }
    *out = v;
    return true;
  }

  // Printable NUL-terminated string at `va`, or "" if there isn't one.
  std::string StrAt(uint32_t va, size_t cap = 96) const {
    std::string s;
    for (size_t i = 0; i < cap; i++) {
      int b = ByteAt((uint32_t)(va + i));
      if (b < 0) return "";
      if (b == 0) return s;
      if (b < 0x20 || b > 0x7E) return "";
      s += (char)b;
    }
    return "";
  }

  // Hex/ASCII dump plus per-word interpretation, so a raw address can be read
  // without dropping out of the tool.
  std::string MemJson(uint32_t addr, uint32_t len) {
    // len is HEX (the UI sends it that way). It used to collapse to 0x100 when
    // it exceeded the cap, so an over-large request quietly returned 256 bytes
    // and a bulk scan built on it reported a confident "no hits" having read
    // 0.4% of the range. Clamp to the cap instead, and report the value used
    // in "len" so a caller can tell it was truncated.
    if (len == 0) len = 0x100;
    if (len > kMaxMemLen) len = kMaxMemLen;
    std::ostringstream o;
    std::string sec;
    for (const auto& s : prog_.sections)
      if (addr >= s.address && addr < s.address + s.size) sec = s.name;
    // Addresses past a section's raw data (BSS-style) have no file bytes at
    // all; say so instead of rendering a screen of blanks.
    bool mapped = false;
    for (uint32_t i = 0; i < len && !mapped; i++)
      if (ByteAt(addr + i) >= 0) mapped = true;
    o << "{\"addr\":\"" << HexAddr(addr) << "\",\"len\":" << len
      << ",\"mapped\":" << (mapped ? "true" : "false") << ",\"section\":\""
      << JsonEscape(sec) << "\",\"rows\":[";
    bool first = true;
    for (uint32_t off = 0; off < len; off += 16) {
      if (!first) o << ",";
      first = false;
      std::ostringstream hex, asc;
      for (int i = 0; i < 16; i++) {
        int b = ByteAt(addr + off + i);
        if (b < 0) { hex << "   "; asc << ' '; continue; }
        char hb[4];
        snprintf(hb, sizeof(hb), "%02X ", b);
        hex << hb;
        asc << (char)((b >= 0x20 && b <= 0x7E) ? b : '.');
      }
      o << "{\"addr\":\"" << HexAddr(addr + off) << "\",\"hex\":\""
        << JsonEscape(hex.str()) << "\",\"ascii\":\"" << JsonEscape(asc.str())
        << "\"}";
    }
    // Word-level interpretation: symbol, string, or code pointer.
    o << "],\"words\":[";
    first = true;
    for (uint32_t off = 0; off + 4 <= len && off < 0x100; off += 4) {
      uint32_t v;
      if (!U32At(addr + off, &v)) break;
      std::string note;
      auto sit = prog_.symbols.find(v);
      if (sit != prog_.symbols.end()) note = sit->second;
      else if (Find(v)) note = NameOf(v);
      if (note.empty()) {
        std::string s = StrAt(v, 40);
        if (s.size() >= 3) note = "\"" + s + "\"";
      }
      if (note.empty()) {
        auto strit = prog_.strings.find(v);
        if (strit != prog_.strings.end()) note = "\"" + strit->second + "\"";
      }
      if (!first) o << ",";
      first = false;
      o << "{\"addr\":\"" << HexAddr(addr + off) << "\",\"u32\":\"" << HexAddr(v)
        << "\",\"note\":\"" << JsonEscape(note) << "\"}";
    }
    o << "]}";
    return o.str();
  }

  // Scans a range for tables of pointers: consecutive slots whose words point
  // at a string and/or a function. This is how dispatch tables and vtables are
  // found without hand-written scans.
  std::string TablesJson(uint32_t addr, uint32_t len, uint32_t stride) {
    if (stride != 4 && stride != 8 && stride != 12 && stride != 16) stride = 8;
    // Clamp rather than collapse, for the same reason as MemJson.
    if (len == 0) len = 0x2000;
    if (len > kMaxMemLen) len = kMaxMemLen;
    std::ostringstream o;
    o << "[";
    bool first = true;
    uint32_t run_start = 0, run_len = 0;
    auto flush = [&]() {
      if (run_len >= 3) {                      // 3+ consecutive slots = a table
        if (!first) o << ",";
        first = false;
        o << "{\"addr\":\"" << HexAddr(run_start) << "\",\"stride\":" << stride
          << ",\"count\":" << run_len << ",\"entries\":[";
        for (uint32_t i = 0; i < run_len && i < 64; i++) {
          uint32_t a = run_start + i * stride, w0 = 0, w1 = 0;
          U32At(a, &w0);
          U32At(a + 4, &w1);
          std::string nm = StrAt(w0, 48);
          std::string tgt;
          if (Find(w1)) tgt = NameOf(w1);
          if (i) o << ",";
          o << "{\"slot\":\"" << HexAddr(a) << "\",\"name\":\"" << JsonEscape(nm)
            << "\",\"target\":\"" << HexAddr(w1) << "\",\"tname\":\""
            << JsonEscape(tgt) << "\"}";
        }
        o << "]}";
      }
      run_len = 0;
    };
    for (uint32_t a = addr; a + stride <= addr + len; a += stride) {
      uint32_t w0 = 0, w1 = 0;
      bool ok = U32At(a, &w0) && U32At(a + 4, &w1);
      // A slot counts when word0 names a string and word1 points into a code
      // section. Requiring a DETECTED function would miss exactly the targets
      // worth finding — handlers reached only through the table itself.
      bool good = ok && !StrAt(w0, 48).empty() && InCode(w1);
      if (good) {
        if (!run_len) run_start = a;
        run_len++;
      } else {
        flush();
      }
    }
    flush();
    o << "]";
    return o.str();
  }

  // Callers (up) or callees (down) to a depth, so a chain can be walked
  // without hand-rolling it.
  std::string CallGraphJson(uint32_t addr, int depth, bool up) {
    if (depth < 1) depth = 1;
    if (depth > 6) depth = 6;
    std::ostringstream o;
    o << "[";
    std::set<uint32_t> seen;
    std::vector<std::pair<uint32_t, int>> q{{addr, 0}};
    bool first = true;
    auto& edges = up ? callers_ : callees_;
    while (!q.empty()) {
      auto [a, d] = q.front();
      q.erase(q.begin());
      if (!seen.insert(a).second || d > depth) continue;
      if (!first) o << ",";
      first = false;
      o << "{\"addr\":\"" << HexAddr(a) << "\",\"name\":\"" << JsonEscape(NameOf(a))
        << "\",\"depth\":" << d << "}";
      auto it = edges.find(a);
      if (it == edges.end()) continue;
      for (uint32_t n : it->second) q.push_back({n, d + 1});
    }
    o << "]";
    return o.str();
  }

  // Sets the decompiler globals the cached render was produced with, so a
  // re-render of one function matches the rest of the cache.
  void PrepareRender() {
    SetGlobalSymbolMap(prog_.symbols);
    SetFieldNames({});
    SetStructTags({});
    SetTypeOverrides({});
    SetStructDecls({});
    SetCalleeDecls({});
    SetStringMap(&prog_.strings);
    SetGlobalStructs(&gstructs_);
  }

  // Re-renders whatever a rename invalidated. Done lazily at search time so a
  // burst of renames costs one refresh, not one per rename.
  void RefreshCache() {
    if (!dirty_all_ && dirty_.empty()) return;
    PrepareRender();
    if (dirty_all_) {
      for (auto& f : prog_.funcs) code_cache_[f.entry] = DecompileToC(f);
    } else {
      for (uint32_t a : dirty_) {
        auto it = index_.find(a);
        if (it != index_.end())
          code_cache_[a] = DecompileToC(prog_.funcs[it->second]);
      }
    }
    dirty_.clear();
    dirty_all_ = false;
  }

  // A function's name appears in its own body and at every call site, so its
  // callers need re-rendering too.
  void MarkDirtyFunc(uint32_t addr) {
    dirty_.insert(addr);
    auto it = callers_.find(addr);
    if (it != callers_.end())
      for (uint32_t c : it->second) dirty_.insert(c);
  }

  // Case-insensitive substring search over every function's decompiled text.
  // This is what answers "who references 0x8306DB74" — constants, globals,
  // calls and strings are all searchable because they appear in the code.
  std::string SearchJson(const std::string& query, size_t limit) {
    RefreshCache();  // renames since the last search must be visible
    std::string q = query;
    for (auto& ch : q) ch = (char)std::tolower((unsigned char)ch);
    std::ostringstream o;
    o << "[";
    size_t n = 0;
    if (!q.empty()) {
      for (const auto& kv : code_cache_) {
        if (n >= limit) break;
        std::string lc = kv.second;
        for (auto& ch : lc) ch = (char)std::tolower((unsigned char)ch);
        size_t p = lc.find(q);
        if (p == std::string::npos) continue;
        // Count hits and grab the first matching line for context.
        size_t hits = 0, sp = 0;
        while ((sp = lc.find(q, sp)) != std::string::npos) { hits++; sp += q.size(); }
        size_t ls = kv.second.rfind('\n', p);
        ls = (ls == std::string::npos) ? 0 : ls + 1;
        size_t le = kv.second.find('\n', p);
        if (le == std::string::npos) le = kv.second.size();
        std::string line = kv.second.substr(ls, le - ls);
        while (!line.empty() && (line[0] == ' ' || line[0] == '\t'))
          line.erase(0, 1);
        if (line.size() > 160) line = line.substr(0, 160) + "...";
        if (n) o << ",";
        n++;
        o << "{\"addr\":\"" << HexAddr(kv.first) << "\",\"name\":\""
          << JsonEscape(NameOf(kv.first)) << "\",\"hits\":" << hits
          << ",\"line\":\"" << JsonEscape(line) << "\"}";
      }
    }
    o << "]";
    return o.str();
  }

  // All bound globals with every field discovered program-wide and the
  // functions that touch each one.
  std::string GlobalsJson() {
    std::ostringstream o;
    o << "[";
    bool first = true;
    for (const auto& g : gstructs_) {
      if (!first) o << ",";
      first = false;
      o << "{\"addr\":\"" << HexAddr(g.addr) << "\",\"name\":\""
        << JsonEscape(g.name) << "\",\"size\":\"" << HexAddr(g.size)
        << "\",\"fields\":[";
      auto ait = gfield_all_.find(g.addr);
      bool ffirst = true;
      if (ait != gfield_all_.end()) {
        for (const auto& fo : ait->second) {
          if (!ffirst) o << ",";
          ffirst = false;
          char ob[16];
          snprintf(ob, sizeof(ob), "%X", fo.first);
          std::string nm;
          auto nit = g.fields.find(fo.first);
          if (nit != g.fields.end()) nm = nit->second;
          else { char b[24]; snprintf(b, sizeof(b), "field_%X", fo.first); nm = b; }
          o << "{\"off\":\"" << ob << "\",\"name\":\"" << JsonEscape(nm)
            << "\",\"nref\":" << fo.second.size() << ",\"refs\":[";
          bool rfirst = true;
          size_t n = 0;
          for (uint32_t fa : fo.second) {
            if (n++ >= 8) break;
            if (!rfirst) o << ",";
            rfirst = false;
            o << "{\"addr\":\"" << HexAddr(fa) << "\",\"name\":\""
              << JsonEscape(NameOf(fa)) << "\"}";
          }
          o << "]}";
        }
      }
      o << "]}";
    }
    o << "]";
    return o.str();
  }

  // Substring search over the harvested strings. Returns each match with the
  // functions that reference it, so a hit leads straight to code.
  std::string StringsJson(const std::string& query, size_t limit) {
    std::string q = query;
    for (auto& c : q) c = (char)std::tolower((unsigned char)c);
    std::ostringstream o;
    o << "[";
    size_t n = 0;
    for (const auto& kv : prog_.strings) {
      if (n >= limit) break;
      if (!q.empty()) {
        std::string hay = kv.second;
        for (auto& c : hay) c = (char)std::tolower((unsigned char)c);
        if (hay.find(q) == std::string::npos) continue;
      }
      if (n) o << ",";
      n++;
      o << "{\"addr\":\"" << HexAddr(kv.first) << "\",\"text\":\""
        << JsonEscape(kv.second) << "\",\"refs\":[";
      auto rit = str_refs_.find(kv.first);
      if (rit != str_refs_.end()) {
        bool first = true;
        for (uint32_t fa : rit->second) {
          if (!first) o << ",";
          first = false;
          o << "{\"addr\":\"" << HexAddr(fa) << "\",\"name\":\""
            << JsonEscape(NameOf(fa)) << "\"}";
        }
      }
      o << "]}";
    }
    o << "]";
    return o.str();
  }

  std::string FunctionsJson() {
    std::ostringstream o;
    o << "[";
    bool first = true;
    for (auto& f : prog_.funcs) {
      if (!first) o << ",";
      first = false;
      size_t nref = 0;
      auto it = callers_.find(f.entry);
      if (it != callers_.end()) nref = it->second.size();
      o << "{\"addr\":\"" << HexAddr(f.entry) << "\",\"name\":\""
        << JsonEscape(NameOf(f.entry)) << "\",\"size\":" << (f.end - f.entry)
        << ",\"nref\":" << nref << "}";
    }
    o << "]";
    return o.str();
  }

  // Everything the renderer produces for one function. Shared by the single
  // function view and by the whole-image export, so an exported file is the
  // same text the UI shows, user edits included.
  struct RenderedFunc {
    const ILFunction* f = nullptr;
    std::string code;
    std::vector<std::string> order;
    std::unordered_map<std::string, std::string> suggest;
    std::vector<std::string> label_ids;
    std::vector<InferredStruct> structs;
  };

  bool RenderFunc(uint32_t addr, RenderedFunc* out) {
    const ILFunction* f = FindOrBuild(addr);
    if (!f) return false;
    out->f = f;

    SetGlobalSymbolMap(prog_.symbols);
    SetStringMap(&prog_.strings);
    SetGlobalStructs(&gstructs_);

    // Pass 1: discover the pointer bases in this function.
    SetFieldNames({});
    SetStructTags({});
    auto ovit = inferred_types_.find(addr);
    SetTypeOverrides(ovit != inferred_types_.end()
                         ? ovit->second
                         : std::map<std::string, std::string>{});
    DecompileToC(*f);
    std::vector<InferredStruct> bases = GetInferredStructs();

    // A local that holds a copy of a parameter inherits whatever type the SDK
    // declared for that parameter. This is what carries `CONST POINT *` from
    // the prototype onto the register the prologue parked it in, which is the
    // base the field accesses actually go through.
    if (getenv("XEXMAGIC_TRACE_BIND")) {
      auto pit = param_struct_key_.find(addr);
      printf("[bind] %08X param_struct_keys=%zu origins=%zu\n", addr,
             pit == param_struct_key_.end() ? 0 : pit->second.size(),
             GetBaseParamOrigins().size());
      if (pit != param_struct_key_.end())
        for (const auto& kv : pit->second)
          printf("[bind]   param %d -> %s\n", kv.first, kv.second.c_str());
      for (const auto& o : GetBaseParamOrigins())
        printf("[bind]   origin %s <- arg %d\n", o.base.c_str(), o.position);
      for (const auto& s : bases) printf("[bind]   base %s\n", s.base.c_str());
      fflush(stdout);
    }
    // The type maps below are whole-program state, and the export renders
    // several functions at once. The decompiler's own state is per-thread; this
    // is the part that is shared. Nothing expensive happens under the lock.
    std::unique_lock<std::mutex> type_lock(types_mu_);
    for (const auto& origin : GetBaseParamOrigins()) {
      auto pit = param_struct_key_.find(addr);
      if (pit == param_struct_key_.end()) break;
      auto kit = pit->second.find(origin.position);
      if (kit == pit->second.end()) continue;
      // Never override a binding the user made by hand.
      if (base_type_[addr].count(origin.base)) continue;
      base_type_[addr][origin.base] = kit->second;
    }

    // Anything still unnamed gets matched against the headers by its layout.
    // A candidate is only accepted when every offset the code touches lands on
    // a declared field, so this names the struct rather than guessing at it.
    for (const auto& s : bases) {
      if (base_type_[addr].count(s.base)) continue;
      std::vector<uint32_t> offsets;
      for (const auto& fld : s.fields) offsets.push_back(fld.offset);
      if (offsets.empty()) continue;
      auto cands = xexmagic::SdkConfig::Get().MatchLayout(0, offsets, 2);
      if (cands.empty() || !cands[0].def) continue;
      // Two equally good candidates means the layout does not identify the
      // type; leave it inferred rather than pick one at random.
      if (cands.size() > 1 && cands[1].score == cands[0].score) continue;
      const auto* def = cands[0].def;
      std::string key = "t:" + def->name;
      base_type_[addr][s.base] = key;
      auto& t = types_[key];
      t.display = def->name;
      for (const auto& fld : def->fields)
        if (t.fields.find(fld.offset) == t.fields.end())
          t.fields[fld.offset] = fld.name;
    }

    // Resolve each base to its unified type, then build the field-name and
    // struct-tag overrides for the final render.
    auto callees = CalleeMap(*f);
    std::map<std::string, std::string> base_key;     // base -> typeKey
    std::map<std::string, std::map<uint32_t, std::string>> field_over;
    std::map<std::string, std::string> tag_over;
    for (auto& s : bases) {
      std::string key = ResolveTypeKey(addr, s.base, callees);
      base_key[s.base] = key;
      tag_over[s.base] = DisplayOf(key, s.base);
      auto tit = types_.find(key);
      if (tit != types_.end())
        for (auto& fo : tit->second.fields)
          field_over[s.base][fo.first] = fo.second;
    }
    type_lock.unlock();

    // Declarations for the bases bound to an SDK type, so the block above the
    // function is the header's struct rather than a list of observed offsets.
    std::map<std::string, std::string> decls;
    for (const auto& kv : base_key) {
      if (kv.second.rfind("t:", 0) != 0) continue;
      const auto* sdk_struct =
          xexmagic::SdkConfig::Get().FindStruct(kv.second.substr(2));
      if (!sdk_struct || sdk_struct->fields.empty()) continue;
      std::ostringstream d;
      d << "struct " << sdk_struct->name << " {  // " << sdk_struct->header;
      if (sdk_struct->size) d << ", " << sdk_struct->size << " bytes";
      d << "\n";
      for (const auto& f : sdk_struct->fields) {
        d << "  " << f.type << " " << f.name;
        if (f.array_count > 1) d << "[" << f.array_count << "]";
        char oc[24];
        snprintf(oc, sizeof(oc), ";  // +0x%X", f.offset);
        d << oc << "\n";
      }
      d << "};\n";
      decls[kv.first] = d.str();
    }
    SetStructDecls(decls);

    // Declarations for the callees this function referenced, gathered by the
    // first pass. The SDK's own declaration is used verbatim where there is
    // one; otherwise the shape is taken from the call site, which is all the
    // code proves.
    {
      auto& sdk = xexmagic::SdkConfig::Get();
      std::map<std::string, std::string> callee_decls;
      for (const auto& kv : GetCalledFunctions()) {
        const auto* proto = sdk.FindPrototype(kv.first);
        std::ostringstream d;
        if (proto) {
          d << proto->ret << " " << proto->name << "(";
          for (size_t i = 0; i < proto->params.size(); i++) {
            if (i) d << ", ";
            d << proto->params[i].first;
            if (!proto->params[i].second.empty())
              d << " " << proto->params[i].second;
          }
          if (proto->params.empty()) d << "void";
          d << ");  // " << proto->header;
        } else {
          d << "uint32_t " << kv.first << "(";
          for (int i = 0; i < kv.second; i++) {
            if (i) d << ", ";
            d << "uint32_t a" << (i + 1);
          }
          if (kv.second == 0) d << "void";
          d << ");  // arity from the call site";
        }
        callee_decls[kv.first] = d.str();
      }
      SetCalleeDecls(callee_decls);
    }

    // Pass 2: final render with unified names + annotation ordinal.
    SetFieldNames(field_over);
    SetStructTags(tag_over);
    auto sgit = sigs_.find(addr);
    SetSignature(sgit != sigs_.end() ? &sgit->second : nullptr);
    DecompileCOptions opt;
    auto oit = prog_.ordinals.find(addr);
    if (oit != prog_.ordinals.end()) opt.ordinal = oit->second;
    std::string code = DecompileToC(*f, opt);
    std::vector<InferredStruct> structs = GetInferredStructs();

    // Collect locals + suggestions from the freshly emitted text.
    std::vector<std::string> order;
    std::set<std::string> seen;
    std::unordered_map<std::string, std::string> suggest;
    std::istringstream is(code);
    std::string line;
    while (std::getline(is, line)) {
      // "  vNN = Callee(" -> suggestion.
      size_t eq = line.find(" = ");
      if (eq != std::string::npos) {
        size_t s = line.find_first_not_of(" \t");
        if (s != std::string::npos && line[s] == 'v' &&
            s + 1 < line.size() && std::isdigit((unsigned char)line[s + 1])) {
          std::string vid = line.substr(s, eq - s);
          size_t rhs = eq + 3;
          size_t p = line.find('(', rhs);
          if (p != std::string::npos) {
            std::string callee = line.substr(rhs, p - rhs);
            // callee must be a bare identifier (skip casts / derefs).
            bool ident = !callee.empty();
            for (char c : callee) ident &= IsIdentChar(c);
            if (ident) {
              std::string g = SuggestFromCallee(callee);
              if (!g.empty()) suggest[vid] = g;
            }
          }
        }
      }
      // Gather every vNN token.
      for (size_t i = 0; i + 1 < line.size(); i++) {
        if (line[i] == 'v' && std::isdigit((unsigned char)line[i + 1]) &&
            (i == 0 || !IsIdentChar(line[i - 1]))) {
          size_t j = i + 1;
          while (j < line.size() && std::isdigit((unsigned char)line[j])) j++;
          std::string vid = line.substr(i, j - i);
          if (seen.insert(vid).second) order.push_back(vid);
          i = j;
        }
      }
    }

    // Apply user variable renames to the code.
    auto vit = var_names_.find(addr);
    if (vit != var_names_.end())
      for (auto& kv : vit->second)
        code = ReplaceWord(code, kv.first, kv.second);

    // Extract the label set (LABEL_XXXX) before applying label renames, then
    // rewrite both the definition and every goto to the display name.
    std::vector<std::string> label_ids;
    {
      std::set<std::string> ids;
      size_t p = 0;
      while ((p = code.find("LABEL_", p)) != std::string::npos) {
        size_t q = p + 6;
        while (q < code.size() && std::isxdigit((unsigned char)code[q])) q++;
        if (q > p + 6) ids.insert(code.substr(p, q - p));
        p = q;
      }
      label_ids.assign(ids.begin(), ids.end());
      auto lit = labels_.find(addr);
      if (lit != labels_.end())
        for (auto& kv : lit->second)
          code = ReplaceWord(code, kv.first, kv.second);
    }

    out->code = code;
    out->order = order;
    out->suggest = suggest;
    out->label_ids = label_ids;
    out->structs = structs;
    return true;
  }

  std::string FuncJson(uint32_t addr) {
    RenderedFunc r;
    if (!RenderFunc(addr, &r)) return "{\"error\":\"not found\"}";
    const ILFunction* f = r.f;
    const std::string& code = r.code;
    const auto& order = r.order;
    const auto& suggest = r.suggest;
    const auto& label_ids = r.label_ids;
    const auto& structs = r.structs;
    auto vit = var_names_.find(addr);

    std::ostringstream o;
    o << "{\"addr\":\"" << HexAddr(f->entry) << "\",\"name\":\""
      << JsonEscape(NameOf(f->entry)) << "\",\"size\":" << (f->end - f->entry)
      << ",\"code\":\"" << JsonEscape(code) << "\",\"callers\":"
      << XrefJson(callers_, f->entry) << ",\"callees\":"
      << XrefJson(callees_, f->entry) << ",\"vars\":[";
    bool first = true;
    for (auto& vid : order) {
      if (!first) o << ",";
      first = false;
      std::string nm = vid;
      if (vit != var_names_.end()) {
        auto r = vit->second.find(vid);
        if (r != vit->second.end()) nm = r->second;
      }
      std::string sg;
      auto sgit = suggest.find(vid);
      if (sgit != suggest.end()) sg = sgit->second;
      o << "{\"id\":\"" << vid << "\",\"name\":\"" << JsonEscape(nm)
        << "\",\"suggest\":\"" << JsonEscape(sg) << "\"}";
    }
    // Inferred struct pointer bases + their fields.
    o << "],\"structs\":[";
    bool sfirst = true;
    for (auto& s : structs) {
      if (!sfirst) o << ",";
      sfirst = false;
      std::string disp = s.base;  // display base with any var rename applied
      if (vit != var_names_.end()) {
        auto r = vit->second.find(s.base);
        if (r != vit->second.end()) disp = r->second;
      }
      o << "{\"base\":\"" << JsonEscape(s.base) << "\",\"disp\":\""
        << JsonEscape(disp) << "\",\"type\":\"" << JsonEscape(s.type_name)
        << "\",\"fields\":[";
      bool ffirst = true;
      for (auto& fld : s.fields) {
        if (!ffirst) o << ",";
        ffirst = false;
        o << "{\"off\":\"" << HexAddr(fld.offset) << "\",\"name\":\""
          << JsonEscape(fld.name) << "\",\"type\":\"" << JsonEscape(fld.ctype)
          << "\"}";
      }
      o << "]}";
    }
    // Bound globals whose fields this function touches.
    auto gused = GetGlobalFieldUses();
    o << "],\"globals\":[";
    bool gfirst = true;
    for (const auto& g : gstructs_) {
      auto uit = gused.find(g.addr);
      if (uit == gused.end() || uit->second.empty()) continue;
      if (!gfirst) o << ",";
      gfirst = false;
      o << "{\"addr\":\"" << HexAddr(g.addr) << "\",\"name\":\""
        << JsonEscape(g.name) << "\",\"size\":" << g.size << ",\"fields\":[";
      bool ffirst = true;
      for (const auto& fo : uit->second) {
        if (!ffirst) o << ",";
        ffirst = false;
        char ob[16];
        snprintf(ob, sizeof(ob), "%X", fo.first);
        o << "{\"off\":\"" << ob << "\",\"name\":\"" << JsonEscape(fo.second)
          << "\"}";
      }
      o << "]}";
    }
    // Strings referenced by this function.
    o << "],\"strings\":[";
    bool strfirst = true;
    auto fsit = func_strs_.find(addr);
    if (fsit != func_strs_.end()) {
      for (uint32_t sa : fsit->second) {
        auto sit = prog_.strings.find(sa);
        if (sit == prog_.strings.end()) continue;
        if (!strfirst) o << ",";
        strfirst = false;
        o << "{\"addr\":\"" << HexAddr(sa) << "\",\"text\":\""
          << JsonEscape(sit->second) << "\"}";
      }
    }
    // Labels: id (LABEL_XXXX), display name, and the address it encodes.
    o << "],\"labels\":[";
    bool lfirst = true;
    auto lit = labels_.find(addr);
    for (auto& id : label_ids) {
      if (!lfirst) o << ",";
      lfirst = false;
      std::string nm = id;
      if (lit != labels_.end()) {
        auto r = lit->second.find(id);
        if (r != lit->second.end()) nm = r->second;
      }
      o << "{\"id\":\"" << id << "\",\"name\":\"" << JsonEscape(nm)
        << "\",\"addr\":\"" << id.substr(6) << "\"}";
    }
    // Analyst note for this function, if any.
    auto cit = comments_.find(addr);
    o << "],\"comment\":\""
      << (cit != comments_.end() ? JsonEscape(cit->second) : "") << "\"}";
    return o.str();
  }

  std::string ExportFileName() const {
    std::string p = prog_.input_path;
    size_t s = p.find_last_of("/\\");
    if (s != std::string::npos) p = p.substr(s + 1);
    if (p.empty()) p = "export";
    return p + ".c";
  }

  // The whole image as one C file. Every function goes through the same
  // renderer the UI uses, so renames, types, signatures and notes from the
  // database are all in the exported text.
  std::string ExportPath() const { return prog_.input_path + ".c"; }

  std::string ExportAllC() {
    // Snapshot the entries first: RenderFunc can build a function on demand,
    // which pushes onto prog_.funcs and would invalidate an iterator over it.
    struct Ent {
      uint32_t entry;
      uint32_t size;
    };
    std::vector<Ent> ents;
    ents.reserve(prog_.funcs.size());
    for (const auto& fn : prog_.funcs)
      ents.push_back({fn.entry, fn.end - fn.entry});
    const size_t total = ents.size();

    // Render in parallel. The decompiler's render state is thread_local and
    // the shared type maps are locked inside RenderFunc, so the only ordering
    // that matters is reassembling the pieces by index afterwards.
    std::vector<std::string> parts(total);
    std::atomic<size_t> next{0};
    std::atomic<size_t> done{0};
    unsigned nthreads = std::thread::hardware_concurrency();
    if (nthreads == 0) nthreads = 4;
    if (nthreads > total) nthreads = (unsigned)total ? (unsigned)total : 1;
    printf("export: %zu functions on %u threads\n", total, nthreads);
    fflush(stdout);
    xexmagic::progress::BeginPhase("exporting the whole image to C", total);

    auto worker = [&]() {
      for (;;) {
        size_t i = next.fetch_add(1);
        if (i >= total) return;
        xexmagic::progress::SetCurrent(NameOf(ents[i].entry));
        size_t n = done.fetch_add(1) + 1;
        if (n % 250 == 0) {
          printf("export: %zu/%zu functions\n", n, total);
          fflush(stdout);
        }
        RenderedFunc r;
        if (!RenderFunc(ents[i].entry, &r)) {
          xexmagic::progress::Complete();
          continue;
        }
        std::ostringstream f;
        f << "// ============================================================\n"
          << "// " << HexAddr(ents[i].entry) << "  " << NameOf(ents[i].entry)
          << "  (" << ents[i].size << " bytes)\n";
        auto cit = comments_.find(ents[i].entry);
        if (cit != comments_.end() && !cit->second.empty()) {
          std::istringstream cs(cit->second);
          std::string cl;
          while (std::getline(cs, cl)) {
            if (!cl.empty() && cl.back() == '\r') cl.pop_back();
            f << "// " << cl << "\n";
          }
        }
        f << "// ============================================================\n"
          << r.code << "\n\n";
        parts[i] = f.str();
        xexmagic::progress::Complete();
      }
    };

    std::vector<std::thread> pool;
    for (unsigned t = 1; t < nthreads; t++) pool.emplace_back(worker);
    worker();
    for (auto& t : pool) t.join();

    std::ostringstream o;
    o << "// XEXMagic export of " << prog_.input_path << "\n"
      << "// " << total
      << " functions, with all names, types and notes from the database.\n\n"
      << "#include <stdint.h>\n\n";
    for (const auto& p : parts) o << p;

    // A full image takes minutes; write it beside the image too, so a browser
    // that gives up on the request does not throw the work away.
    std::string text = o.str();
    // Beside the image where that is writable (an SDK under Program Files is
    // not), otherwise in the working directory.
    std::string where = ExportPath();
    FILE* fp = fopen(where.c_str(), "wb");
    if (!fp) {
      where = ExportFileName();
      fp = fopen(where.c_str(), "wb");
    }
    if (fp) {
      fwrite(text.data(), 1, text.size(), fp);
      fclose(fp);
      printf("export: wrote %s (%zu bytes)\n", where.c_str(), text.size());
    } else {
      printf("export: could not write a copy to disk (%zu bytes served)\n",
             text.size());
    }
    fflush(stdout);
    xexmagic::progress::BeginPhase("");
    return text;
  }

  // Kernel and xam entry points matching `needle` by name, module or exact
  // ordinal. The baked tables are complete, so this searches everything the
  // console exports and marks the ones this image actually imports - the two
  // questions ("does this exist" and "does this title use it") are the same
  // search, and the answer to the second is an address to jump to.
  std::string ImportsJson(const std::string& needle, size_t limit) {
    std::string n = needle;
    for (char& c : n) c = (char)std::tolower((unsigned char)c);

    // What this image imports, keyed by (module stem, ordinal).
    auto stem_of = [](const std::string& s) {
      std::string t = s.substr(0, s.find('.'));
      for (char& c : t) c = (char)std::tolower((unsigned char)c);
      return t;
    };
    std::map<std::pair<std::string, uint32_t>, const WebProgram::Import*> used;
    for (const auto& imp : prog_.imports)
      used[{stem_of(imp.module), imp.ordinal}] = &imp;

    auto matches = [&](const std::string& name, const std::string& module,
                       uint32_t ordinal) {
      if (n.empty()) return true;
      std::string hay = name + " " + module;
      for (char& c : hay) c = (char)std::tolower((unsigned char)c);
      if (hay.find(n) != std::string::npos) return true;
      char ord[16];
      snprintf(ord, sizeof(ord), "%u", ordinal);
      return n == ord;
    };

    std::ostringstream o;
    o << "[";
    size_t shown = 0;
    auto emit = [&](const std::string& name, const std::string& module,
                    uint32_t ordinal, const WebProgram::Import* imp) {
      if (shown) o << ",";
      shown++;
      uint32_t at = 0;
      size_t ncallers = 0;
      if (imp) {
        at = imp->thunk_address ? imp->thunk_address : imp->value_address;
        ncallers = CallersOfImport(*imp).size();
      }
      o << "{\"name\":\"" << JsonEscape(name) << "\",\"module\":\""
        << JsonEscape(module) << "\",\"ordinal\":" << ordinal
        << ",\"imported\":" << (imp ? "true" : "false") << ",\"addr\":\""
        << HexAddr(at) << "\",\"callers\":" << ncallers << "}";
    };

    // Imported entries first: they are the ones with somewhere to go.
    std::set<std::pair<std::string, uint32_t>> emitted;
    for (const auto& imp : prog_.imports) {
      if (shown >= limit) break;
      std::string name = imp.name;
      if (name.empty()) {
        const char* known =
            LookupKernelOrdinal(stem_of(imp.module).c_str(), imp.ordinal);
        if (known) name = known;
      }
      if (!matches(name, imp.module, imp.ordinal)) continue;
      emitted.insert({stem_of(imp.module), imp.ordinal});
      emit(name, imp.module, imp.ordinal, &imp);
    }

    // Then everything else the tables know about.
    for (const auto& e : AllKernelExports()) {
      if (shown >= limit) break;
      if (emitted.count({e.module, e.ordinal})) continue;
      if (!matches(e.name, e.module, e.ordinal)) continue;
      auto it = used.find({e.module, e.ordinal});
      emit(e.name, e.module, e.ordinal,
           it == used.end() ? nullptr : it->second);
    }
    o << "]";
    return o.str();
  }

  // Every function that calls one import, by its thunk or value address.
  std::string ImportXrefJson(uint32_t addr) {
    const WebProgram::Import* found = nullptr;
    for (const auto& imp : prog_.imports) {
      if (imp.thunk_address == addr || imp.value_address == addr) {
        found = &imp;
        break;
      }
    }
    if (!found) return "{\"error\":\"not an import\"}";
    std::ostringstream o;
    o << "{\"name\":\"" << JsonEscape(found->name) << "\",\"module\":\""
      << JsonEscape(found->module) << "\",\"ordinal\":" << found->ordinal
      << ",\"callers\":[";
    bool first = true;
    for (uint32_t fa : CallersOfImport(*found)) {
      if (!first) o << ",";
      first = false;
      o << "{\"addr\":\"" << HexAddr(fa) << "\",\"name\":\""
        << JsonEscape(NameOf(fa)) << "\"}";
    }
    o << "]}";
    return o.str();
  }

  // ---- mutations ----

  bool RenameFunc(uint32_t addr, const std::string& name) {
    // Build on demand: an address can be a valid function the whole-program
    // pass never detected (dispatch-table handlers, cold stubs). Naming one
    // must work the same as navigating to it.
    if (!FindOrBuild(addr)) return false;
    prog_.symbols[addr] = name;
    MarkDirtyFunc(addr);
    SaveDb();
    return true;
  }

  bool RenameVar(uint32_t addr, const std::string& vid,
                 const std::string& name) {
    // Build on demand: an address can be a valid function the whole-program
    // pass never detected (dispatch-table handlers, cold stubs). Naming one
    // must work the same as navigating to it.
    if (!FindOrBuild(addr)) return false;
    if (name.empty() || name == vid)
      var_names_[addr].erase(vid);
    else
      var_names_[addr][vid] = name;
    dirty_.insert(addr);
    SaveDb();
    return true;
  }

  // Field rename applies to the base's unified TYPE, so every variable and
  // parameter bound to that type sees it, in every function.
  // Bind (or update, or with size 0 remove) a struct at a fixed address.
  // Re-runs the whole-program render so a newly bound struct's fields are
  // discovered immediately rather than only on restart.
  void RediscoverGlobals() {
    SetGlobalSymbolMap(prog_.symbols);
    SetFieldNames({});
    SetStructTags({});
    SetTypeOverrides({});
    SetStructDecls({});
    SetCalleeDecls({});
    SetStringMap(&prog_.strings);
    SetGlobalStructs(&gstructs_);
    gfield_all_.clear();
    for (auto& f : prog_.funcs) {
      code_cache_[f.entry] = DecompileToC(f);
      for (const auto& gu : GetGlobalFieldUses())
        for (const auto& fo : gu.second) gfield_all_[gu.first][fo.first].insert(f.entry);
    }
    // Every entry was just re-rendered, so nothing is stale any more.
    dirty_.clear();
    dirty_all_ = false;
  }

  bool BindGlobal(uint32_t addr, uint32_t size, const std::string& name) {
    for (size_t i = 0; i < gstructs_.size(); i++) {
      if (gstructs_[i].addr != addr) continue;
      if (!size) gstructs_.erase(gstructs_.begin() + i);
      else { gstructs_[i].size = size; if (!name.empty()) gstructs_[i].name = name; }
      SaveDb();
      RediscoverGlobals();
      return true;
    }
    if (!size) return false;
    GlobalStruct g;
    g.addr = addr;
    g.size = size;
    g.name = name.empty() ? ("g_" + HexAddr(addr)) : name;
    gstructs_.push_back(g);
    SaveDb();
    RediscoverGlobals();
    return true;
  }

  // Rename a field of a bound global — applies everywhere it is referenced.
  bool RenameGlobalField(uint32_t base, uint32_t off, const std::string& name) {
    for (auto& g : gstructs_) {
      if (g.addr != base) continue;
      if (name.empty()) g.fields.erase(off);
      else g.fields[off] = name;
      // Only functions that touch this global are affected — we know them.
      auto ait = gfield_all_.find(base);
      if (ait != gfield_all_.end()) {
        auto fit = ait->second.find(off);
        if (fit != ait->second.end())
          for (uint32_t fa : fit->second) dirty_.insert(fa);
      }
      SaveDb();
      return true;
    }
    return false;
  }

  // Free-text note on any address. Survives restarts, so a long analysis
  // doesn't have to be re-derived.
  // "ret|pos:type:name|pos:type:name|..."
  static FuncSignature ParseSigSpec(const std::string& spec) {
    FuncSignature s;
    size_t p = 0;
    bool first = true;
    while (p <= spec.size()) {
      size_t e = spec.find('|', p);
      std::string part = spec.substr(p, e == std::string::npos ? e : e - p);
      if (first) {
        s.ret = part;
        first = false;
      } else if (!part.empty()) {
        size_t c1 = part.find(':'), c2 = part.find(':', c1 + 1);
        if (c1 != std::string::npos) {
          int pos = atoi(part.substr(0, c1).c_str());
          std::string ty = part.substr(
              c1 + 1, c2 == std::string::npos ? std::string::npos : c2 - c1 - 1);
          std::string nm = c2 == std::string::npos ? "" : part.substr(c2 + 1);
          s.args[pos] = {ty, nm};
        }
      }
      if (e == std::string::npos) break;
      p = e + 1;
    }
    return s;
  }

  // An empty spec clears the override.
  bool SetSignature_(uint32_t addr, const std::string& spec) {
    if (!FindOrBuild(addr)) return false;
    if (spec.empty()) sigs_.erase(addr);
    else sigs_[addr] = ParseSigSpec(spec);
    // Once edited it is the user's, and must persist.
    sdk_sigs_.erase(addr);
    dirty_.insert(addr);
    SaveDb();
    return true;
  }

  // Every name the analysis produced, as a C header for the emulator source.
  std::string ExportHeader() {
    std::ostringstream o;
    o << "// Generated by XEXMagic from " << prog_.input_path << "\n"
      << "// " << prog_.funcs.size() << " functions analysed\n#pragma once\n\n";
    for (const auto& kv : prog_.symbols) {
      if (!index_.count(kv.first)) continue;
      if (kv.second.rfind("sub_", 0) == 0) continue;   // unnamed: no value
      o << "#define " << kv.second << "_ADDR 0x" << HexAddr(kv.first) << "\n";
    }
    o << "\n// bound globals\n";
    for (const auto& g : gstructs_) {
      o << "#define " << g.name << "_ADDR 0x" << HexAddr(g.addr) << "\n";
      for (const auto& f : g.fields)
        o << "#define " << g.name << "_" << f.second << " 0x"
          << HexAddr(g.addr + f.first) << "\n";
    }
    return o.str();
  }

  bool SetComment(uint32_t addr, const std::string& text) {
    if (text.empty()) comments_.erase(addr);
    else comments_[addr] = text;
    SaveDb();
    return true;
  }

  bool RenameLabel(uint32_t addr, const std::string& id,
                   const std::string& name) {
    // Build on demand: an address can be a valid function the whole-program
    // pass never detected (dispatch-table handlers, cold stubs). Naming one
    // must work the same as navigating to it.
    if (!FindOrBuild(addr)) return false;
    if (name.empty() || name == id)
      labels_[addr].erase(id);
    else
      labels_[addr][id] = name;
    dirty_.insert(addr);
    SaveDb();
    return true;
  }

  bool RenameField(uint32_t addr, const std::string& base, uint32_t off,
                   const std::string& name) {
    const ILFunction* f = Find(addr);
    if (!f) return false;
    std::string key = ResolveTypeKey(addr, base, CalleeMap(*f));
    auto& t = types_[key];
    if (t.display.empty()) t.display = DefaultDisplay(key, base);
    if (name.empty())
      t.fields.erase(off);
    else
      t.fields[off] = name;
    // A type can be shared by bases in any function, and we don't keep a
    // reverse map, so invalidate everything.
    dirty_all_ = true;
    SaveDb();
    return true;
  }

  // Bind a base to a named type: unifies with an existing type of that display
  // name, or creates a new one. Empty name reverts to the default binding.
  bool SetType(uint32_t addr, const std::string& base,
               const std::string& display) {
    // Build on demand: an address can be a valid function the whole-program
    // pass never detected (dispatch-table handlers, cold stubs). Naming one
    // must work the same as navigating to it.
    if (!FindOrBuild(addr)) return false;
    if (display.empty()) {
      auto fit = base_type_.find(addr);
      if (fit != base_type_.end()) fit->second.erase(base);
    } else {
      // Reuse an existing type key with this display name (unification).
      std::string key;
      for (auto& kv : types_)
        if (kv.second.display == display) { key = kv.first; break; }
      if (key.empty()) {
        key = "u:" + display;
        types_[key].display = display;
      }
      base_type_[addr][base] = key;
    }
    dirty_all_ = true;  // unification can retype bases in other functions
    SaveDb();
    return true;
  }

  static std::vector<std::string> SplitWords(const std::string& s) {
    std::vector<std::string> out;
    std::string cur;
    for (char c : s) {
      if (std::isalnum((unsigned char)c) || c == '_') {
        cur += c;
      } else {
        if (!cur.empty()) out.push_back(cur);
        cur.clear();
      }
    }
    if (!cur.empty()) out.push_back(cur);
    return out;
  }

  // Argument types and names for every function the SDK headers declare.
  //
  // No amount of looking at code recovers `D3DFORMAT DstFormat` - the only
  // place that exists is the declaration. Applying it here means the signature
  // is right, and it also seeds param_types_, so InferTypes' propagation pass
  // carries those types out of the callee and onto the variables every caller
  // passes in.
  void ApplySdkPrototypes() {
    auto& sdk = xexmagic::SdkConfig::Get();
    if (!sdk.indexed() || !sdk.prototype_count()) return;
    size_t applied = 0, seeded = 0, bound = 0;
    for (const auto& f : prog_.funcs) {
      // A signature the user asserted is the truth; never overwrite it.
      if (sigs_.count(f.entry)) continue;
      const auto* proto = sdk.FindPrototype(NameOf(f.entry));
      if (!proto) continue;

      FuncSignature sig;
      sig.ret = proto->ret;
      for (size_t i = 0; i < proto->params.size(); i++) {
        const auto& p = proto->params[i];
        // Positions are 1-based here, matching a1..aN as the decompiler
        // spells them.
        sig.args[(int)i + 1] = {p.first, p.second};
        // Only pointers are worth propagating - an int parameter tells a
        // caller's variable nothing it doesn't already know.
        if (p.first.find('*') != std::string::npos) {
          param_types_[f.entry][(int)i] = p.first;
          seeded++;
        }
      }
      sigs_[f.entry] = std::move(sig);
      sdk_sigs_.insert(f.entry);
      applied++;

      // A parameter declared as a pointer to a type the SDK also declares is
      // not an inferred layout at all - the header has the name and every
      // field name. Bind it, so the body reads pPoint->y instead of
      // V25_t::field_4.
      for (size_t i = 0; i < proto->params.size(); i++) {
        const std::string& ty = proto->params[i].first;
        if (ty.find('*') == std::string::npos) continue;
        // "CONST POINT *" -> "POINT"
        std::string tag;
        for (const auto& tok : SplitWords(ty)) {
          if (tok == "CONST" || tok == "const" || tok == "struct" ||
              tok == "*" || tok == "VOID" || tok == "void")
            continue;
          tag = tok;
        }
        if (tag.empty()) continue;
        const auto* sdk_struct = sdk.FindStruct(tag);
        if (!sdk_struct || sdk_struct->fields.empty()) continue;
        std::string base = "a" + std::to_string(i + 1);
        std::string key = "t:" + sdk_struct->name;
        base_type_[f.entry][base] = key;
        // Remembered by position too, so a local holding a copy of this
        // parameter can pick the same type up.
        param_struct_key_[f.entry][(int)i + 1] = key;
        auto& t = types_[key];
        t.display = sdk_struct->name;
        for (const auto& fld : sdk_struct->fields)
          if (t.fields.find(fld.offset) == t.fields.end())
            t.fields[fld.offset] = fld.name;
        bound++;
      }
    }
    if (applied) {
      printf("SDK prototypes: %zu functions typed from headers, %zu pointer "
             "parameters seeded for propagation, %zu parameters bound to "
             "declared structs\n", applied, seeded, bound);
      fflush(stdout);
    }
  }

  // Whole-program type propagation, run once at startup.
  //
  // Pass 1: decompile every function to learn (a) the pointer types its own
  //         body proves for each parameter, and (b) every "variable passed to
  //         callee at position N" flow.
  // Pass 2: push each callee's known parameter types back onto the variables
  //         its callers pass in, iterating so types travel more than one edge.
  //         Local evidence always wins, so this only fills genuine gaps.
  void InferTypes() {
    struct Flow { uint32_t func; std::string arg; uint32_t callee; int pos; };
    std::vector<Flow> flows;
    std::map<std::string, uint32_t> by_name;
    for (auto& f : prog_.funcs) by_name[NameOf(f.entry)] = f.entry;

    // Pass 1 renders every function. That is the bulk of a load, it is
    // independent per function, and the decompiler's render state is
    // thread_local - so it runs across every core and the facts each render
    // produced are merged afterwards, in address order, exactly as the serial
    // version merged them. Ordering the merge is what keeps a parallel run
    // byte-identical to a serial one.
    struct FuncFacts {
      std::string code;
      std::vector<DataRef> data_refs;
      std::vector<uint32_t> string_refs;
      std::map<uint32_t, std::map<uint32_t, std::string>> gfield_uses;
      std::vector<InferredStruct> structs;
      std::vector<ArgFlow> arg_flows;
    };
    const size_t nfuncs = prog_.funcs.size();
    std::vector<FuncFacts> facts(nfuncs);

    xexmagic::progress::BeginPhase("whole-program type inference", nfuncs);
    {
      std::atomic<size_t> next{0};
      unsigned nthreads = std::thread::hardware_concurrency();
      if (nthreads == 0) nthreads = 4;
      auto worker = [&]() {
        // Per-thread render state: every one of these setters writes a
        // thread_local, so each worker has to establish its own.
        SetGlobalSymbolMap(prog_.symbols);
        SetFieldNames({});
        SetStructTags({});
        SetTypeOverrides({});
        SetStructDecls({});
        SetCalleeDecls({});
        SetStringMap(&prog_.strings);
        SetGlobalStructs(&gstructs_);
        for (;;) {
          size_t i = next.fetch_add(1);
          if (i >= nfuncs) return;
          const auto& f = prog_.funcs[i];
          xexmagic::progress::SetCurrent(NameOf(f.entry));
          auto& out = facts[i];
          out.code = DecompileToC(f);
          out.data_refs = GetDataRefs();
          out.string_refs = GetStringRefs();
          out.gfield_uses = GetGlobalFieldUses();
          out.structs = GetInferredStructs();
          out.arg_flows = GetArgFlows();
          xexmagic::progress::Complete();
        }
      };
      std::vector<std::thread> pool;
      for (unsigned t = 1; t < nthreads; t++) pool.emplace_back(worker);
      worker();
      for (auto& t : pool) t.join();
      printf("Type inference: %zu functions on %u threads\n", nfuncs, nthreads);
      fflush(stdout);
    }

    xexmagic::progress::BeginPhase("collecting what the renders proved",
                                   nfuncs);
    for (size_t fi = 0; fi < nfuncs; fi++) {
      const auto& f = prog_.funcs[fi];
      auto& facts_f = facts[fi];
      xexmagic::progress::Complete();
      // Cached so code search doesn't have to re-decompile the whole program.
      code_cache_[f.entry] = std::move(facts_f.code);
      for (const auto& r : facts_f.data_refs)
        (r.write ? data_writes_ : data_reads_)[r.addr].insert(f.entry);
      // String cross-references, collected while the body is rendered.
      for (uint32_t sa : facts_f.string_refs) {
        str_refs_[sa].insert(f.entry);
        func_strs_[f.entry].insert(sa);
      }
      // Every field of a bound global touched anywhere, plus who touches it —
      // this is what reveals a bound struct's real extent.
      for (const auto& gu : facts_f.gfield_uses) {
        for (const auto& fo : gu.second) {
          gfield_all_[gu.first][fo.first].insert(f.entry);
        }
      }
      // (a) parameter types this body proves locally.
      for (auto& s : facts_f.structs) {
        if (s.base.size() < 2 || s.base[0] != 'a') continue;
        int pos = atoi(s.base.c_str() + 1) - 1;
        if (pos < 0) continue;
        // A declared type from the SDK headers is the real one - don't replace
        // it with a synthesized tag inferred from a single body.
        if (param_types_[f.entry].count(pos)) continue;
        // Name the type after the callee+position, not the local base tag, so
        // every caller passing into this slot shares one meaningful type.
        param_types_[f.entry][pos] =
            "struct " + NameOf(f.entry) + "_p" + std::to_string(pos + 1) + " *";
      }
      // (b) argument flows into callees.
      for (auto& fl : facts_f.arg_flows) {
        uint32_t target = fl.callee_target;
        if (!target) {
          auto it = by_name.find(fl.callee);
          if (it != by_name.end()) target = it->second;
        }
        if (!target || !index_.count(target)) continue;
        flows.push_back({f.entry, fl.arg, target, fl.position});
        // The callee's declaration names the type of this argument. That is
        // proof, not inference: a variable passed as `CONST TexelDesc *` is a
        // TexelDesc, wherever it came from and whatever the local code does
        // with it. Bind it so the field names come from the header.
        auto pk = param_struct_key_.find(target);
        if (pk == param_struct_key_.end()) continue;
        auto kit = pk->second.find(fl.position + 1);
        if (kit == pk->second.end()) continue;
        if (fl.offset) {
          // `v9 + 0xE0` passed as `TexelDesc *`: the object at that offset is
          // a TexelDesc, so name the field after the type rather than binding
          // the whole frame to it.
          std::string key = ResolveTypeKey(f.entry, fl.arg, {});
          auto& t = types_[key];
          if (t.display.empty()) t.display = DefaultDisplay(key, fl.arg);
          std::string tag = kit->second.substr(2);
          if (t.fields.find(fl.offset) == t.fields.end()) {
            std::string fname = tag;
            for (char& c : fname) c = (char)std::tolower((unsigned char)c);
            t.fields[fl.offset] = fname;
            dirty_all_ = true;
          }
          continue;
        }
        if (base_type_[f.entry].count(fl.arg)) continue;
        base_type_[f.entry][fl.arg] = kit->second;
      }
    }

    xexmagic::progress::BeginPhase("propagating types across call edges");
    // Pass 2: propagate callee parameter types to caller variables, iterating
    // so a type can travel several call edges. Converges quickly; capped.
    for (int round = 0; round < 4; round++) {
      size_t changed = 0;
      for (auto& fl : flows) {
        auto pit = param_types_.find(fl.callee);
        if (pit == param_types_.end()) continue;
        auto tit = pit->second.find(fl.pos);
        if (tit == pit->second.end() || tit->second.empty()) continue;
        auto& slot = inferred_types_[fl.func][fl.arg];
        if (!slot.empty()) continue;
        slot = tit->second;
        changed++;
        // A parameter that just gained a type becomes a source for its own
        // callers on the next round.
        if (fl.arg.size() >= 2 && fl.arg[0] == 'a') {
          int pos = atoi(fl.arg.c_str() + 1) - 1;
          if (pos >= 0 && !param_types_[fl.func].count(pos))
            param_types_[fl.func][pos] = tit->second;
        }
      }
      if (!changed) break;
    }
    printf("Type inference: %zu arg flows, %zu functions with inferred types\n",
           flows.size(), inferred_types_.size());
    fflush(stdout);
  }

 private:
  // Base variable -> the function it was produced by (from the IL Call whose
  // destination SSA id matches the "vN" base), for the producer type heuristic.
  std::map<std::string, std::string> CalleeMap(const ILFunction& f) {
    std::map<std::string, std::string> m;
    for (auto& b : f.blocks)
      for (auto& in : b.instrs)
        if (in.op == ILOp::Call && in.dst.id != kILNoValue &&
            !in.call_name.empty())
          m["v" + std::to_string(in.dst.id)] = in.call_name;
    return m;
  }

  // Type key a base binds to: an explicit user binding, else the producer
  // heuristic (shared "t:<TYPE>"), else a function-local key so same-named
  // locals in different functions never merge by accident.
  std::string ResolveTypeKey(uint32_t func, const std::string& base,
                             const std::map<std::string, std::string>& callees) {
    auto fit = base_type_.find(func);
    if (fit != base_type_.end()) {
      auto bit = fit->second.find(base);
      if (bit != fit->second.end()) return bit->second;
    }
    auto cit = callees.find(base);
    if (cit != callees.end()) {
      std::string t = TypeFromCallee(cit->second);
      if (!t.empty()) return "t:" + t;
    }
    return "l:" + HexAddr(func) + ":" + base;
  }

  // Default display tag for a type key not yet in types_.
  std::string DefaultDisplay(const std::string& key, const std::string& base) {
    if (key.rfind("t:", 0) == 0) return key.substr(2);
    if (key.rfind("u:", 0) == 0) return key.substr(2);
    std::string n = base;  // "l:" local -> V34_t style
    if (!n.empty() && n[0] >= 'a' && n[0] <= 'z')
      n[0] = (char)(n[0] - 'a' + 'A');
    return n + "_t";
  }

  std::string DisplayOf(const std::string& key, const std::string& base) {
    auto it = types_.find(key);
    if (it != types_.end() && !it->second.display.empty())
      return it->second.display;
    return DefaultDisplay(key, base);
  }

  std::string XrefJson(std::map<uint32_t, std::set<uint32_t>>& m,
                       uint32_t addr) {
    std::ostringstream o;
    o << "[";
    auto it = m.find(addr);
    if (it != m.end()) {
      bool first = true;
      for (uint32_t a : it->second) {
        if (!first) o << ",";
        first = false;
        o << "{\"addr\":\"" << HexAddr(a) << "\",\"name\":\""
          << JsonEscape(NameOf(a)) << "\"}";
      }
    }
    o << "]";
    return o.str();
  }

  void BuildXrefs() {
    // Import thunks are not decompiled, so they are not function entries and
    // the edge test below would throw these away - which is exactly the edge
    // worth having, since it is where the image calls the kernel.
    std::set<uint32_t> import_addrs;
    for (const auto& imp : prog_.imports) {
      if (imp.thunk_address) import_addrs.insert(imp.thunk_address);
      if (imp.value_address) import_addrs.insert(imp.value_address);
    }

    for (auto& f : prog_.funcs) {
      for (auto& b : f.blocks) {
        for (auto& in : b.instrs) {
          if (in.op != ILOp::Call && in.op != ILOp::Branch &&
              in.op != ILOp::BranchCond)
            continue;
          uint32_t tgt = in.target;
          if (!tgt) continue;
          if (import_addrs.count(tgt)) {
            import_callers_[tgt].insert(f.entry);
            continue;
          }
          // Only edges to known function entries (skip intra-func branches).
          if (!index_.count(tgt) || tgt == f.entry) continue;
          callees_[f.entry].insert(tgt);
          callers_[tgt].insert(f.entry);
        }
      }
    }
  }

  // Everything that reaches one import: direct calls to its thunk, and the
  // functions that load its pointer slot - an indirect call through the value
  // address is a call, it just does not look like one in the IL.
  std::set<uint32_t> CallersOfImport(const WebProgram::Import& imp) const {
    std::set<uint32_t> out;
    for (uint32_t a : {imp.thunk_address, imp.value_address}) {
      if (!a) continue;
      auto it = import_callers_.find(a);
      if (it != import_callers_.end()) out.insert(it->second.begin(),
                                                  it->second.end());
      auto rd = data_reads_.find(a);
      if (rd != data_reads_.end()) out.insert(rd->second.begin(),
                                              rd->second.end());
    }
    return out;
  }

  // Everything the tool writes for an image lives in one per-file directory
  // under %APPDATA%\XEXMagic\db, named by the content hash. Two different
  // default.xex files therefore keep separate databases, and an image on
  // read-only media (or under Program Files) is still fully editable.
  std::string DbDir() const {
    std::string dir = xexmagic::AppSubDir("db") + HashHex(file_hash_);
    xexmagic::EnsureDir(dir);
    return dir + kSep;
  }

  std::string DbPath() const { return DbDir() + "image.xdb"; }

  void LoadDb() {
    std::ifstream in(DbPath(), std::ios::binary);
    if (!in) return;
    std::stringstream ss;
    ss << in.rdbuf();
    std::string db = ss.str();
    // functions: { "addr":"name", ... }
    ParseSection(db, "\"functions\"", [&](uint32_t a, const std::string& n) {
      prog_.symbols[a] = n;
    });
    // vars: { "faddr": { "vN":"name", ... }, ... }
    size_t vp = db.find("\"vars\"");
    if (vp != std::string::npos) {
      size_t open = db.find('{', vp + 6);
      if (open != std::string::npos) {
        int depth = 0;
        size_t i = open;
        for (; i < db.size(); i++) {
          if (db[i] == '{') depth++;
          else if (db[i] == '}') { depth--; if (depth == 0) break; }
        }
        std::string vars = db.substr(open, i - open + 1);
        // For each "faddr": { ... } pair, parse the inner object.
        size_t pos = 1;
        while (true) {
          size_t kq = vars.find('"', pos);
          if (kq == std::string::npos) break;
          size_t kqe = vars.find('"', kq + 1);
          if (kqe == std::string::npos) break;
          std::string faddr = vars.substr(kq + 1, kqe - kq - 1);
          size_t io = vars.find('{', kqe);
          if (io == std::string::npos) break;
          size_t ie = vars.find('}', io);
          if (ie == std::string::npos) break;
          uint32_t fa = (uint32_t)strtoul(faddr.c_str(), nullptr, 16);
          std::string inner = vars.substr(io, ie - io + 1);
          ParseFlat(inner, [&](const std::string& k, const std::string& v) {
            var_names_[fa][k] = v;
          });
          pos = ie + 1;
        }
      }
    }
    // labels: { "faddr": { "LABEL_XXXX":"name", ... }, ... }
    size_t lp = db.find("\"labels\"");
    if (lp != std::string::npos) {
      size_t open = db.find('{', lp + 8);
      if (open != std::string::npos) {
        int depth = 0;
        size_t i = open;
        for (; i < db.size(); i++) {
          if (db[i] == '{') depth++;
          else if (db[i] == '}') { depth--; if (depth == 0) break; }
        }
        std::string lbls = db.substr(open, i - open + 1);
        size_t pos = 1;
        while (true) {
          size_t kq = lbls.find('"', pos);
          if (kq == std::string::npos) break;
          size_t kqe = lbls.find('"', kq + 1);
          if (kqe == std::string::npos) break;
          std::string faddr = lbls.substr(kq + 1, kqe - kq - 1);
          size_t io = lbls.find('{', kqe);
          if (io == std::string::npos) break;
          size_t ie = lbls.find('}', io);
          if (ie == std::string::npos) break;
          uint32_t fa = (uint32_t)strtoul(faddr.c_str(), nullptr, 16);
          std::string inner = lbls.substr(io, ie - io + 1);
          ParseFlat(inner, [&](const std::string& k, const std::string& v) {
            labels_[fa][k] = v;
          });
          pos = ie + 1;
        }
      }
    }
    // fields: { "faddr": { "base": { "off":"name", ... }, ... }, ... }
    auto match_brace = [](const std::string& s, size_t open) -> size_t {
      int d = 0;
      for (size_t i = open; i < s.size(); i++) {
        if (s[i] == '{') d++;
        else if (s[i] == '}') { d--; if (d == 0) return i; }
      }
      return std::string::npos;
    };
    size_t fp = db.find("\"fields\"");
    if (fp != std::string::npos) {
      size_t open = db.find('{', fp + 8);
      if (open != std::string::npos) {
        size_t end = match_brace(db, open);
        std::string fields = db.substr(open, end - open + 1);
        size_t pos = 1;
        while (true) {
          size_t kq = fields.find('"', pos);
          if (kq == std::string::npos) break;
          size_t kqe = fields.find('"', kq + 1);
          if (kqe == std::string::npos) break;
          std::string faddr = fields.substr(kq + 1, kqe - kq - 1);
          size_t bo = fields.find('{', kqe);
          if (bo == std::string::npos) break;
          size_t be = match_brace(fields, bo);
          if (be == std::string::npos) break;
          uint32_t fa = (uint32_t)strtoul(faddr.c_str(), nullptr, 16);
          std::string block = fields.substr(bo, be - bo + 1);
          // Each "base": { flat off->name } within this function.
          size_t bpos = 1;
          while (true) {
            size_t bq = block.find('"', bpos);
            if (bq == std::string::npos) break;
            size_t bqe = block.find('"', bq + 1);
            if (bqe == std::string::npos) break;
            std::string base = block.substr(bq + 1, bqe - bq - 1);
            size_t io = block.find('{', bqe);
            if (io == std::string::npos) break;
            size_t ie = block.find('}', io);
            if (ie == std::string::npos) break;
            std::string inner = block.substr(io, ie - io + 1);
            // Legacy per-function fields migrate onto the function-local type.
            std::string key = "l:" + HexAddr(fa) + ":" + base;
            if (types_[key].display.empty())
              types_[key].display = DefaultDisplay(key, base);
            ParseFlat(inner, [&](const std::string& k, const std::string& v) {
              types_[key].fields[(uint32_t)strtoul(k.c_str(), nullptr, 16)] = v;
            });
            bpos = ie + 1;
          }
          pos = be + 1;
        }
      }
    }

    // sigs: { "addr":"ret|pos:type:name|...", ... }
    ParseSection(db, "\"sigs\"", [&](uint32_t a, const std::string& spec) {
      if (!spec.empty()) sigs_[a] = ParseSigSpec(spec);
    });
    // comments: { "addr":"note", ... }
    ParseSection(db, "\"comments\"", [&](uint32_t a, const std::string& n) {
      comments_[a] = n;
    });
    // globals: { "addr": { "n":"name", "s":"size", "f": { "off":"name" } } }
    size_t gp = db.find("\"globals\"");
    if (gp != std::string::npos) {
      size_t open = db.find('{', gp + 9);
      if (open != std::string::npos) {
        size_t end = match_brace(db, open);
        std::string gs = db.substr(open, end - open + 1);
        size_t pos = 1;
        while (true) {
          size_t kq = gs.find('"', pos);
          if (kq == std::string::npos) break;
          size_t kqe = gs.find('"', kq + 1);
          if (kqe == std::string::npos) break;
          std::string as = gs.substr(kq + 1, kqe - kq - 1);
          size_t bo = gs.find('{', kqe);
          if (bo == std::string::npos) break;
          size_t be = match_brace(gs, bo);
          if (be == std::string::npos) break;
          std::string block = gs.substr(bo, be - bo + 1);
          GlobalStruct g;
          g.addr = (uint32_t)strtoul(as.c_str(), nullptr, 16);
          std::string nm, sz;
          JsonGetStr(block, "n", &nm);
          JsonGetStr(block, "s", &sz);
          g.name = nm;
          g.size = (uint32_t)strtoul(sz.c_str(), nullptr, 16);
          size_t fo = block.find("\"f\"");
          if (fo != std::string::npos) {
            size_t io = block.find('{', fo);
            size_t ie = io != std::string::npos ? match_brace(block, io)
                                                : std::string::npos;
            if (ie != std::string::npos) {
              std::string inner = block.substr(io, ie - io + 1);
              ParseFlat(inner, [&](const std::string& k, const std::string& v) {
                g.fields[(uint32_t)strtoul(k.c_str(), nullptr, 16)] = v;
              });
            }
          }
          if (g.size) gstructs_.push_back(g);
          pos = be + 1;
        }
      }
    }
    // types: { "key": { "d":"Display", "f": { "off":"name", ... } }, ... }
    size_t tp = db.find("\"types\"");
    if (tp != std::string::npos) {
      size_t open = db.find('{', tp + 7);
      if (open != std::string::npos) {
        size_t end = match_brace(db, open);
        std::string ts = db.substr(open, end - open + 1);
        size_t pos = 1;
        while (true) {
          size_t kq = ts.find('"', pos);
          if (kq == std::string::npos) break;
          size_t kqe = ts.find('"', kq + 1);
          if (kqe == std::string::npos) break;
          std::string key = ts.substr(kq + 1, kqe - kq - 1);
          size_t bo = ts.find('{', kqe);
          if (bo == std::string::npos) break;
          size_t be = match_brace(ts, bo);
          if (be == std::string::npos) break;
          std::string block = ts.substr(bo, be - bo + 1);
          std::string disp;
          JsonGetStr(block, "d", &disp);
          if (!disp.empty()) types_[key].display = disp;
          size_t fo = block.find("\"f\"");
          if (fo != std::string::npos) {
            size_t io = block.find('{', fo);
            size_t ie = io != std::string::npos ? match_brace(block, io)
                                                : std::string::npos;
            if (ie != std::string::npos) {
              std::string inner = block.substr(io, ie - io + 1);
              ParseFlat(inner, [&](const std::string& k, const std::string& v) {
                types_[key].fields[(uint32_t)strtoul(k.c_str(), nullptr, 16)] = v;
              });
            }
          }
          pos = be + 1;
        }
      }
    }

    // bindings: { "faddr": { "base":"typeKey", ... }, ... }
    size_t bp = db.find("\"bindings\"");
    if (bp != std::string::npos) {
      size_t open = db.find('{', bp + 10);
      if (open != std::string::npos) {
        size_t end = match_brace(db, open);
        std::string bs = db.substr(open, end - open + 1);
        size_t pos = 1;
        while (true) {
          size_t kq = bs.find('"', pos);
          if (kq == std::string::npos) break;
          size_t kqe = bs.find('"', kq + 1);
          if (kqe == std::string::npos) break;
          std::string faddr = bs.substr(kq + 1, kqe - kq - 1);
          size_t io = bs.find('{', kqe);
          if (io == std::string::npos) break;
          size_t ie = match_brace(bs, io);
          if (ie == std::string::npos) break;
          uint32_t fa = (uint32_t)strtoul(faddr.c_str(), nullptr, 16);
          std::string inner = bs.substr(io, ie - io + 1);
          ParseFlat(inner, [&](const std::string& k, const std::string& v) {
            base_type_[fa][k] = v;
          });
          pos = ie + 1;
        }
      }
    }
  }

  template <typename F>
  void ParseSection(const std::string& db, const std::string& key, F apply) {
    size_t p = db.find(key);
    if (p == std::string::npos) return;
    size_t open = db.find('{', p + key.size());
    if (open == std::string::npos) return;
    size_t close = db.find('}', open);
    if (close == std::string::npos) return;
    std::string obj = db.substr(open, close - open + 1);
    ParseFlat(obj, [&](const std::string& k, const std::string& v) {
      apply((uint32_t)strtoul(k.c_str(), nullptr, 16), v);
    });
  }

  // Parse a flat { "k":"v", ... } object.
  template <typename F>
  void ParseFlat(const std::string& obj, F apply) {
    size_t pos = 0;
    while (true) {
      size_t kq = obj.find('"', pos);
      if (kq == std::string::npos) break;
      size_t kqe = obj.find('"', kq + 1);
      if (kqe == std::string::npos) break;
      std::string k = obj.substr(kq + 1, kqe - kq - 1);
      size_t colon = obj.find(':', kqe);
      if (colon == std::string::npos) break;
      size_t vq = obj.find('"', colon);
      if (vq == std::string::npos) break;
      size_t vqe = obj.find('"', vq + 1);
      if (vqe == std::string::npos) break;
      std::string v = obj.substr(vq + 1, vqe - vq - 1);
      apply(k, v);
      pos = vqe + 1;
    }
  }

  // ---- analysis cache ----------------------------------------------------
  //
  // Whole-program inference is the expensive part of a load - minutes on a
  // real image - and it derives entirely from the file. Its results are
  // written beside the .xdb once and reloaded on the next run, so a fully
  // decompiled image comes back up in seconds. The .xdb itself stays small
  // and human-readable: it holds what the analyst authored, and it is
  // rewritten on every edit, which a cache this size must not be.
  //
  // Keyed by the source image's size and content hash, so editing or
  // replacing the file invalidates the cache instead of silently serving the
  // wrong analysis.

  std::string AnalysisPath() const { return DbPath() + ".analysis"; }

  // Hash and size of the file on disk, computed once. This is the identity a
  // dropped file was stored under, and what a cache is validated against.
  void HashSourceFile() {
    if (!HashFile(prog_.input_path, &file_hash_, &file_size_)) {
      file_hash_ = 0;
      file_size_ = 0;
    }
  }

  uint64_t SourceHash() const {
    // FNV-1a over the image bytes. Fast enough on a 40 MB image to run every
    // startup, and it is a staleness check, not a security boundary.
    uint64_t h = 1469598103934665603ull;
    if (prog_.image) {
      for (uint8_t b : *prog_.image) {
        h ^= b;
        h *= 1099511628211ull;
      }
    }
    return h;
  }

  struct Writer {
    std::string buf;
    void U32(uint32_t v) { buf.append((const char*)&v, 4); }
    void U64(uint64_t v) { buf.append((const char*)&v, 8); }
    void Str(const std::string& s) {
      U32((uint32_t)s.size());
      buf.append(s);
    }
  };

  struct Reader {
    const char* p = nullptr;
    const char* end = nullptr;
    bool ok = true;
    uint32_t U32() {
      if (p + 4 > end) { ok = false; return 0; }
      uint32_t v;
      memcpy(&v, p, 4);
      p += 4;
      return v;
    }
    uint64_t U64() {
      if (p + 8 > end) { ok = false; return 0; }
      uint64_t v;
      memcpy(&v, p, 8);
      p += 8;
      return v;
    }
    std::string Str() {
      uint32_t n = U32();
      if (!ok || p + n > end) { ok = false; return ""; }
      std::string s(p, n);
      p += n;
      return s;
    }
  };

  void SaveAnalysis() {
    Writer w;
    w.buf.reserve(1 << 22);
    w.U32(0x4341'4D58);  // "XMAC"
    w.U32(xexmagic::kSchemaVersion);
    w.Str(xexmagic::BuildId());
    w.U64(file_size_);
    w.U64(file_hash_);
    w.U64(prog_.image ? prog_.image->size() : 0);
    w.U64(SourceHash());
    w.U32((uint32_t)prog_.funcs.size());

    auto u32_set_map = [&](const std::map<uint32_t, std::set<uint32_t>>& m) {
      w.U32((uint32_t)m.size());
      for (const auto& kv : m) {
        w.U32(kv.first);
        w.U32((uint32_t)kv.second.size());
        for (uint32_t v : kv.second) w.U32(v);
      }
    };
    auto u32_str_map = [&](const std::map<uint32_t, std::string>& m) {
      w.U32((uint32_t)m.size());
      for (const auto& kv : m) {
        w.U32(kv.first);
        w.Str(kv.second);
      }
    };
    auto str_str = [&](const std::map<std::string, std::string>& m) {
      w.U32((uint32_t)m.size());
      for (const auto& kv : m) {
        w.Str(kv.first);
        w.Str(kv.second);
      }
    };

    u32_str_map(sdk_names_);
    u32_str_map(code_cache_);
    u32_set_map(str_refs_);
    u32_set_map(func_strs_);
    u32_set_map(data_reads_);
    u32_set_map(data_writes_);

    w.U32((uint32_t)gfield_all_.size());
    for (const auto& kv : gfield_all_) {
      w.U32(kv.first);
      w.U32((uint32_t)kv.second.size());
      for (const auto& fo : kv.second) {
        w.U32(fo.first);
        w.U32((uint32_t)fo.second.size());
        for (uint32_t v : fo.second) w.U32(v);
      }
    }

    w.U32((uint32_t)param_types_.size());
    for (const auto& kv : param_types_) {
      w.U32(kv.first);
      w.U32((uint32_t)kv.second.size());
      for (const auto& p : kv.second) {
        w.U32((uint32_t)p.first);
        w.Str(p.second);
      }
    }

    for (const auto* m : {&inferred_types_, &base_type_}) {
      w.U32((uint32_t)m->size());
      for (const auto& kv : *m) {
        w.U32(kv.first);
        str_str(kv.second);
      }
    }

    w.U32((uint32_t)types_.size());
    for (const auto& kv : types_) {
      w.Str(kv.first);
      w.Str(kv.second.display);
      u32_str_map(kv.second.fields);
    }

    // A note about what this directory holds, since it is named by hash.
    if (FILE* meta = fopen((DbDir() + "origin.txt").c_str(), "wb"))  {
      fprintf(meta, "%s\n%s\n", prog_.input_path.c_str(),
              HashHex(file_hash_).c_str());
      fclose(meta);
    }

    std::string where = AnalysisPath();
    FILE* fp = fopen(where.c_str(), "wb");
    if (!fp) {
      printf("analysis cache: could not be written\n");
      fflush(stdout);
      return;
    }
    fwrite(w.buf.data(), 1, w.buf.size(), fp);
    fclose(fp);
    printf("analysis cache: wrote %s (%zu bytes)\n", where.c_str(),
           w.buf.size());
    fflush(stdout);
  }

  // Returns true when a matching cache was applied and inference can be
  // skipped. Anything the analyst authored (loaded from the .xdb just before
  // this) wins over the cached derivation.
  bool LoadAnalysis() {
    std::string where = AnalysisPath();
    FILE* fp = fopen(where.c_str(), "rb");
    if (!fp) return false;
    fseek(fp, 0, SEEK_END);
    long len = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    std::string data((size_t)len, '\0');
    size_t got = fread(&data[0], 1, (size_t)len, fp);
    fclose(fp);
    if (got != (size_t)len) return false;

    Reader r{data.data(), data.data() + data.size()};
    if (r.U32() != 0x4341'4D58) return false;
    if (r.U32() != xexmagic::kSchemaVersion) {
      printf("analysis cache: older format, re-analysing\n");
      fflush(stdout);
      return false;
    }
    std::string built = r.Str();
    if (!r.ok) return false;
    if (built != xexmagic::BuildId()) {
      printf("analysis cache: written by XEXMagic %s, this is %s - "
             "re-analysing\n", built.c_str(), xexmagic::BuildId().c_str());
      fflush(stdout);
      return false;
    }
    uint64_t fsize = r.U64();
    uint64_t fhash = r.U64();
    uint64_t size = r.U64();
    uint64_t hash = r.U64();
    uint32_t nfuncs = r.U32();
    if (!r.ok) return false;
    if (fsize != file_size_ || fhash != file_hash_) {
      printf("analysis cache: belongs to a different file (%s, this file is "
             "%s) - re-analysing\n", HashHex(fhash).c_str(),
             HashHex(file_hash_).c_str());
      fflush(stdout);
      return false;
    }
    if (size != (prog_.image ? prog_.image->size() : 0) ||
        nfuncs != prog_.funcs.size() || hash != SourceHash()) {
      printf("analysis cache: does not match this image, re-analysing\n");
      fflush(stdout);
      return false;
    }

    auto u32_set_map = [&](std::map<uint32_t, std::set<uint32_t>>& m) {
      uint32_t n = r.U32();
      for (uint32_t i = 0; i < n && r.ok; i++) {
        uint32_t k = r.U32();
        uint32_t c = r.U32();
        auto& dst = m[k];
        for (uint32_t j = 0; j < c && r.ok; j++) dst.insert(r.U32());
      }
    };
    auto u32_str_map = [&](std::map<uint32_t, std::string>& m) {
      uint32_t n = r.U32();
      for (uint32_t i = 0; i < n && r.ok; i++) {
        uint32_t k = r.U32();
        m[k] = r.Str();
      }
    };

    u32_str_map(sdk_names_);
    // Derived names go back on, but never over one the image carried or the
    // analyst authored - the .xdb was loaded before this.
    for (const auto& kv : sdk_names_)
      if (!prog_.symbols.count(kv.first)) prog_.symbols[kv.first] = kv.second;

    u32_str_map(code_cache_);
    u32_set_map(str_refs_);
    u32_set_map(func_strs_);
    u32_set_map(data_reads_);
    u32_set_map(data_writes_);

    uint32_t n = r.U32();
    for (uint32_t i = 0; i < n && r.ok; i++) {
      uint32_t g = r.U32();
      uint32_t nf = r.U32();
      for (uint32_t j = 0; j < nf && r.ok; j++) {
        uint32_t off = r.U32();
        uint32_t c = r.U32();
        auto& dst = gfield_all_[g][off];
        for (uint32_t k = 0; k < c && r.ok; k++) dst.insert(r.U32());
      }
    }

    n = r.U32();
    for (uint32_t i = 0; i < n && r.ok; i++) {
      uint32_t f = r.U32();
      uint32_t c = r.U32();
      for (uint32_t j = 0; j < c && r.ok; j++) {
        int pos = (int)r.U32();
        std::string t = r.Str();
        param_types_[f].emplace(pos, t);
      }
    }

    for (auto* m : {&inferred_types_, &base_type_}) {
      n = r.U32();
      for (uint32_t i = 0; i < n && r.ok; i++) {
        uint32_t f = r.U32();
        uint32_t c = r.U32();
        for (uint32_t j = 0; j < c && r.ok; j++) {
          std::string k = r.Str();
          std::string v = r.Str();
          (*m)[f].emplace(k, v);  // never over an authored binding
        }
      }
    }

    n = r.U32();
    for (uint32_t i = 0; i < n && r.ok; i++) {
      std::string key = r.Str();
      std::string disp = r.Str();
      uint32_t c = r.U32();
      auto& t = types_[key];
      if (t.display.empty()) t.display = disp;
      for (uint32_t j = 0; j < c && r.ok; j++) {
        uint32_t off = r.U32();
        std::string nm = r.Str();
        t.fields.emplace(off, nm);
      }
    }

    if (!r.ok) {
      printf("analysis cache: truncated, re-analysing\n");
      fflush(stdout);
      return false;
    }
    printf("analysis cache: loaded %s (%ld bytes)\n", where.c_str(), len);
    fflush(stdout);
    return true;
  }

  void SaveDb() {
    std::ofstream out(DbPath(), std::ios::binary | std::ios::trunc);
    if (!out) return;
    out << "{\n  \"functions\": {";
    bool first = true;
    for (auto& kv : prog_.symbols) {
      // Only persist names that differ from the default sub_ / that are funcs.
      if (!index_.count(kv.first)) continue;
      // Names the SDK scan derived are re-derived (or reloaded from the
      // analysis cache) on every run; persisting them here would make them
      // indistinguishable from the analyst's own and outlive a better scan.
      auto sit = sdk_names_.find(kv.first);
      if (sit != sdk_names_.end() && sit->second == kv.second) continue;
      if (!first) out << ",";
      first = false;
      out << "\n    \"" << HexAddr(kv.first) << "\": \""
          << JsonEscape(kv.second) << "\"";
    }
    out << "\n  },\n  \"vars\": {";
    first = true;
    for (auto& fv : var_names_) {
      if (fv.second.empty()) continue;
      if (!first) out << ",";
      first = false;
      out << "\n    \"" << HexAddr(fv.first) << "\": {";
      bool vfirst = true;
      for (auto& kv : fv.second) {
        if (!vfirst) out << ",";
        vfirst = false;
        out << " \"" << kv.first << "\": \"" << JsonEscape(kv.second) << "\"";
      }
      out << " }";
    }
    out << "\n  },\n  \"labels\": {";
    first = true;
    for (auto& fv : labels_) {
      if (fv.second.empty()) continue;
      if (!first) out << ",";
      first = false;
      out << "\n    \"" << HexAddr(fv.first) << "\": {";
      bool vfirst = true;
      for (auto& kv : fv.second) {
        if (!vfirst) out << ",";
        vfirst = false;
        out << " \"" << kv.first << "\": \"" << JsonEscape(kv.second) << "\"";
      }
      out << " }";
    }
    out << "\n  },\n  \"sigs\": {";
    first = true;
    for (const auto& kv : sigs_) {
      if (sdk_sigs_.count(kv.first)) continue;  // derived, not authored
      if (!first) out << ",";
      first = false;
      std::ostringstream spec;
      spec << kv.second.ret;
      for (const auto& a : kv.second.args)
        spec << "|" << a.first << ":" << a.second.first << ":" << a.second.second;
      out << "\n    \"" << HexAddr(kv.first) << "\": \""
          << JsonEscape(spec.str()) << "\"";
    }
    out << "\n  },\n  \"comments\": {";
    first = true;
    for (const auto& kv : comments_) {
      if (kv.second.empty()) continue;
      if (!first) out << ",";
      first = false;
      out << "\n    \"" << HexAddr(kv.first) << "\": \""
          << JsonEscape(kv.second) << "\"";
    }
    out << "\n  },\n  \"globals\": {";
    first = true;
    for (const auto& g : gstructs_) {
      if (!first) out << ",";
      first = false;
      out << "\n    \"" << HexAddr(g.addr) << "\": { \"n\": \""
          << JsonEscape(g.name) << "\", \"s\": \"" << HexAddr(g.size)
          << "\", \"f\": {";
      bool ffirst = true;
      for (const auto& fo : g.fields) {
        if (!ffirst) out << ",";
        ffirst = false;
        out << " \"" << HexAddr(fo.first) << "\": \"" << JsonEscape(fo.second)
            << "\"";
      }
      out << " } }";
    }
    out << "\n  },\n  \"types\": {";
    first = true;
    for (auto& kv : types_) {
      const SType& t = kv.second;
      if (t.fields.empty() && t.display.empty()) continue;
      if (!first) out << ",";
      first = false;
      out << "\n    \"" << JsonEscape(kv.first) << "\": { \"d\": \""
          << JsonEscape(t.display) << "\", \"f\": {";
      bool ffirst = true;
      for (auto& fo : t.fields) {
        if (!ffirst) out << ",";
        ffirst = false;
        out << " \"" << HexAddr(fo.first) << "\": \"" << JsonEscape(fo.second)
            << "\"";
      }
      out << " } }";
    }
    out << "\n  },\n  \"bindings\": {";
    first = true;
    for (auto& fv : base_type_) {
      if (fv.second.empty()) continue;
      if (!first) out << ",";
      first = false;
      out << "\n    \"" << HexAddr(fv.first) << "\": {";
      bool bfirst = true;
      for (auto& kv : fv.second) {
        if (!bfirst) out << ",";
        bfirst = false;
        out << " \"" << kv.first << "\": \"" << JsonEscape(kv.second) << "\"";
      }
      out << " }";
    }
    out << "\n  }\n}\n";
  }

  WebProgram prog_;
  // Identity of the file this model was built from.
  uint64_t file_hash_ = 0;
  uint64_t file_size_ = 0;
  std::map<uint32_t, size_t> index_;
  std::map<uint32_t, std::set<uint32_t>> callers_;
  std::map<uint32_t, std::set<uint32_t>> callees_;
  // Import thunk/value address -> functions that call or load it.
  std::map<uint32_t, std::set<uint32_t>> import_callers_;
  std::map<uint32_t, std::map<std::string, std::string>> var_names_;
  // func addr -> label id (LABEL_XXXX) -> display name.
  std::map<uint32_t, std::map<std::string, std::string>> labels_;

  // ---- unified struct types ----
  // A named struct type owns its fields; many pointer bases across functions
  // and parameters can bind to the same type, so a field rename propagates
  // everywhere and the tag is shared.
  struct SType {
    std::string display;                     // shown tag, e.g. "IRP"
    std::map<uint32_t, std::string> fields;  // offset -> field name
  };
  std::map<std::string, SType> types_;                 // typeKey -> type
  std::map<uint32_t, std::map<std::string, std::string>> base_type_;  // overrides
  // Guards the two maps above while the export renders functions in parallel.
  std::mutex types_mu_;

  // ---- interprocedural type inference (whole-program pre-pass) ----
  // func addr -> variable (aN/vN) -> declared type, derived from how the
  // variable is used by the routines this function calls.
  std::map<uint32_t, std::map<std::string, std::string>> inferred_types_;
  // func addr -> parameter position -> type, learned from each function's own
  // local evidence; the propagation source for its callers.
  std::map<uint32_t, std::map<int, std::string>> param_types_;

  // Structs bound to fixed addresses (user-declared).
  std::vector<GlobalStruct> gstructs_;
  // base -> offset -> functions touching it, across the whole program.
  std::map<uint32_t, std::map<uint32_t, std::set<uint32_t>>> gfield_all_;
  // Data address -> functions, split by direction (IL-derived, not text).
  std::map<uint32_t, std::set<uint32_t>> data_reads_, data_writes_;
  // Free-text notes per address, persisted; rendered above the function.
  std::map<uint32_t, std::string> comments_;
  // User-asserted prototypes: return type and per-argument type/name. Usage
  // can never recover documented names like HANDLE FileHandle, so they are
  // stated rather than inferred.
  std::map<uint32_t, FuncSignature> sigs_;
  // Which of those came from the SDK headers rather than from the user. They
  // are re-derived on every run, so persisting them would only leave a stale
  // copy behind if the SDK path changes.
  std::set<uint32_t> sdk_sigs_;
  // Names the byte-exact library scan derived, kept apart from the analyst's
  // so they stay out of the .xdb and can be replaced by a later scan.
  std::map<uint32_t, std::string> sdk_names_;
  // func -> argument position -> type key, for parameters the SDK declares as
  // pointers to a struct it also declares.
  std::map<uint32_t, std::map<int, std::string>> param_struct_key_;
  // Decompiled text per function, built during the whole-program pass and
  // reused for code search. Renames invalidate entries rather than rebuilding
  // the whole cache; RefreshCache() re-renders them at the next search.
  std::map<uint32_t, std::string> code_cache_;
  std::set<uint32_t> dirty_;
  bool dirty_all_ = false;

  // String cross-references, both directions.
  std::map<uint32_t, std::set<uint32_t>> str_refs_;   // string addr -> funcs
  std::map<uint32_t, std::set<uint32_t>> func_strs_;  // func addr -> strings
};

// ---------------------------------------------------------------------------
// Frontend (single self-contained page)
// ---------------------------------------------------------------------------

const char* kIndexHtml = R"HTML(<!doctype html>
<html><head><meta charset="utf-8"><title>XEXMagic</title>
<style>
:root{--bg:#1e1e1e;--panel:#252526;--edge:#333;--fg:#d4d4d4;--acc:#4ec9b0;--kw:#569cd6;--dim:#858585;--sel:#094771}
*{box-sizing:border-box}
body{margin:0;font:13px/1.5 Consolas,Menlo,monospace;background:var(--bg);color:var(--fg);height:100vh;display:flex;flex-direction:column}
#top{padding:6px 10px;background:var(--panel);border-bottom:1px solid var(--edge);display:flex;gap:10px;align-items:center}
#top b{color:var(--acc)}
#top button{background:var(--panel);color:var(--fg);border:1px solid var(--edge);padding:3px 10px;cursor:pointer;font:inherit}
#top button:hover{background:var(--sel)}
#entrybtn{margin-left:auto}
#pane{display:none;position:absolute;right:12px;top:44px;width:640px;max-height:74vh;background:var(--panel);border:1px solid var(--edge);z-index:60;box-shadow:0 4px 24px #000b;flex-direction:column}
#pane.open{display:flex}
#panehead{display:flex;justify-content:space-between;padding:6px 10px;border-bottom:1px solid var(--edge);color:var(--acc)}
#paneclose{cursor:pointer;color:var(--dim)}
#panebody{overflow:auto;padding:8px 10px;white-space:pre;font-size:12px}
#panebody .r{color:var(--dim)}
#panebody .h{color:var(--fg)}
#panebody .a{color:#ce9178}
#panebody .n{color:var(--acc);cursor:pointer}
#panebody .n:hover{text-decoration:underline}
#panebody input{background:var(--bg);color:var(--fg);border:1px solid var(--edge);width:100%;font:inherit;padding:4px}
.cmt{color:#6a9955;padding:4px 10px;border-bottom:1px solid var(--edge);white-space:pre-wrap}
#tabs{display:flex;border-bottom:1px solid var(--edge)}
.tab{flex:1;text-align:center;padding:5px;cursor:pointer;color:var(--dim)}
.tab.sel{color:var(--acc);background:var(--panel)}
.sr{padding:3px 8px;cursor:pointer;border-bottom:1px solid #2a2a2a}
.sr:hover{background:var(--panel)}
.sr .t{color:#ce9178;display:block;overflow:hidden;text-overflow:ellipsis;white-space:nowrap}
.sr .m{color:var(--dim);font-size:11px}
.str{color:#ce9178}
#bindg{color:var(--acc);cursor:pointer;font-size:10px}
#main{flex:1;display:flex;min-height:0}
#left{width:320px;border-right:1px solid var(--edge);display:flex;flex-direction:column}
#search{margin:6px;padding:5px;background:var(--bg);border:1px solid var(--edge);color:var(--fg)}
#sortbar{margin:0 6px 6px;color:var(--dim);font-size:11px}
#sortbar select{background:var(--bg);border:1px solid var(--edge);color:var(--fg);font:inherit;margin-left:4px}
#flist{flex:1;overflow:auto;overflow-x:auto}
.fn{padding:3px 8px;cursor:pointer;white-space:nowrap;min-width:max-content}
.fn:hover{background:var(--panel)}
.fn.sel{background:var(--sel)}
.fn .a{color:var(--dim);margin-right:6px}
.fn .r{color:var(--dim);margin-left:12px}
#center{flex:1;display:flex;flex-direction:column;min-width:0}
#chead{padding:6px 10px;background:var(--panel);border-bottom:1px solid var(--edge)}
#chead .nm{color:var(--acc);cursor:pointer}
#code{flex:1;overflow:auto;padding:10px;white-space:pre;tab-size:4}
#right{width:280px;border-left:1px solid var(--edge);overflow:auto;padding:8px}
#right h3{color:var(--dim);font-size:11px;text-transform:uppercase;margin:12px 0 4px}
.x{color:var(--kw);cursor:pointer;display:block;padding:1px 0;overflow:hidden;text-overflow:ellipsis;white-space:nowrap}
.x:hover{text-decoration:underline}
.vr{display:flex;justify-content:space-between;padding:1px 0}
.vr .s{color:var(--acc);cursor:pointer}
.st{margin:4px 0}
.st .h{color:var(--kw)}
.st .ty{color:var(--acc);cursor:pointer}
.st .ty:hover{text-decoration:underline}
.fld{display:flex;justify-content:space-between;padding:0 0 0 12px;color:var(--dim)}
.fld .n{color:var(--fg);cursor:pointer}
.fld .n:hover{color:var(--acc)}
.fld .o{color:var(--dim)}
.mo{cursor:pointer}
.mo:hover{color:var(--acc);text-decoration:underline}
.tok{cursor:pointer}
.tok:hover{outline:1px solid var(--acc)}
.ftok{color:#c586c0}
.fnlink{color:var(--acc);cursor:pointer}
.fnlink:hover{text-decoration:underline}
.lbl{color:#4fc1ff;cursor:pointer;text-decoration:underline}
.lbldef{color:#dcdcaa}
.flash{background:var(--sel);border-radius:2px}
.menu{position:absolute;background:var(--panel);border:1px solid var(--edge);z-index:100;min-width:150px;box-shadow:0 2px 10px #000a}
.mi{padding:6px 12px;cursor:pointer;white-space:nowrap}
.mi:hover{background:var(--sel)}
.fnname{color:var(--acc)}
.num{color:#b5cea8}
</style></head>
<body>
<div id="top"><b>XEXMagic</b> <span id="meta"></span>
  <button id="entrybtn" title="go to entry point">Entry</button>
  <button id="membtn" title="hex / data viewer">Memory</button>
  <button id="tablebtn" title="scan for pointer tables">Tables</button>
  <button id="cgbtn" title="call graph for the open function">CallGraph</button>
  <button id="notebtn" title="note on the open function">Note</button>
  <button id="sigbtn" title="set return type / argument types and names">Sig</button>
  <button id="expbtn" title="export names as a C header">Export</button>
  <button id="expcbtn" title="export the whole image decompiled to C, with your renames, types and notes">Export C</button>
  <button id="sdkbtn" title="Xbox 360 SDK: types and library symbols">SDK</button></div>
<div id="pane"><div id="panehead"><span id="panetitle"></span><span id="paneclose">close</span></div><div id="panebody"></div></div>
<div id="main">
  <div id="left">
    <div id="tabs"><span class="tab sel" data-m="fn">Functions</span><span class="tab" data-m="str">Strings</span><span class="tab" data-m="code">Code</span><span class="tab" data-m="imp">Imports</span></div>
    <input id="search" placeholder="filter functions...">
    <div id="sortbar">sort <select id="sortby">
      <option value="addr">address</option>
      <option value="name">alphabetic</option>
      <option value="size">size</option>
      <option value="callers">callers</option>
    </select></div>
    <div id="flist"></div>
  </div>
  <div id="center">
    <div id="chead"><span class="nm" id="cname" title="rename">-</span> <span id="caddr" class="a"></span></div>
    <div id="code"></div>
  </div>
  <div id="right">
    <h3>Callers</h3><div id="callers"></div>
    <h3>Callees</h3><div id="callees"></div>
    <h3>Locals</h3><div id="vars"></div>
    <h3>Structs</h3><div id="structs"></div>
    <h3>Globals <span id="bindg" title="bind a struct at a fixed address">[bind]</span></h3><div id="globals"></div>
  </div>
</div>
<script>
)HTML"
// Split here: MSVC caps a single string literal at 16380 chars, and adjacent
// literals are concatenated at compile time.
R"HTML(
let FUNS=[],CUR=null,FADDR={},META={},MODE='fn';  // FADDR: name -> address
const $=s=>document.querySelector(s);
// Bind both left-click and right-click (context menu) to a rename action.
function rc(el,fn){el.onclick=fn;el.oncontextmenu=e=>{e.preventDefault();fn();};}
async function jget(u){return (await fetch(u)).json();}
async function jpost(u,b){return (await fetch(u,{method:'POST',body:JSON.stringify(b)})).json();}

async function boot(){
  const m=await jget('/api/meta');
  const who=m.file?(m.name?`${m.file} (${m.name})`:m.file):(m.name||'image');
  $('#meta').textContent=`${m.version?'v'+m.version+' — ':''}${who} @ ${m.base} — ${m.count} functions`;
  $('#meta').title=m.path||'';
  document.title=m.file?`XEXMagic — ${m.file}`:'XEXMagic';
  FUNS=await jget('/api/functions');
  FADDR={};FUNS.forEach(f=>FADDR[f.name]=f.addr);
  renderList('');
  META=m;
  $('#meta').textContent+=` — ${m.strings||0} strings`;
  $('#search').addEventListener('input',e=>{
    if(MODE==='fn')renderList(e.target.value);
    else if(MODE==='str')searchStrings(e.target.value);
    else if(MODE==='imp')searchImports(e.target.value);
    else searchCode(e.target.value);
  });
  $('#sortby').onchange=()=>{
    const q=$('#search').value;
    if(MODE==='fn')renderList(q);
    else if(MODE==='imp')searchImports(q);
  };
  $('#entrybtn').onclick=()=>{if(META.entry)open(META.entry);};
  $('#bindg').onclick=bindGlobal;
  $('#membtn').onclick=()=>memView(prompt('Address (hex):', CUR||META.entry)||'');
  $('#tablebtn').onclick=scanTables;
  $('#cgbtn').onclick=callGraph;
  $('#notebtn').onclick=editNote;
  $('#sigbtn').onclick=editSig;
  $('#expbtn').onclick=async()=>{
    const t=await (await fetch('/api/export')).text();
    pane('Export — C header',esc(t));
  };
  $('#expcbtn').onclick=()=>{
    // Straight to the browser's downloader: the response carries
    // Content-Disposition, and tens of megabytes stream to disk instead of
    // being buffered in a blob first.
    const a=document.createElement('a');
    a.href='/api/export/c'; a.download='';
    document.body.appendChild(a); a.click(); a.remove();
  };
  async function sdkPane(){
    const c=await jget('/api/sdk');
    const stat=c.indexed
      ? c.structs+' types, '+c.symbols+' library symbols ('+c.headers+' headers, '+c.libs+' libs)'
      : (c.sdk_path?'not indexed yet':'no SDK configured');
    let h='<div style="padding:8px">'+
      '<div style="margin-bottom:6px">SDK folder <span style="opacity:.6">'+
      '(the one containing include\\ and lib\\)</span></div>'+
      '<input id="sdkpath" style="width:100%;box-sizing:border-box" '+
      'placeholder="C:\\Program Files (x86)\\Microsoft Xbox 360 SDK" value="'+
      esc(c.sdk_path||'')+'">'+
      '<div style="margin-top:6px">'+
      '<button id="sdksave">Save</button> '+
      '<button id="sdkindex">Build index</button> '+
      '<button id="sdkscan" title="match image code against SDK library bodies">Scan image</button> '+
      '<button id="sdkinl" title="find library code the compiler inlined into larger functions">Find inlined</button> '+
      '<span id="sdkstat" style="margin-left:8px;opacity:.75">'+esc(stat)+'</span>'+
      '</div><div id="sdkerr" style="color:#e66;margin-top:6px"></div>'+
      '<hr style="opacity:.2;margin:10px 0">'+
      '<div>Search <span style="opacity:.6">SDK types and library functions</span></div>'+
      '<input id="sdkq" style="width:100%;box-sizing:border-box" placeholder="XSESSION, XNetStartup, ...">'+
      '<div id="sdkres" style="margin-top:8px"></div></div>';
    pane('Xbox 360 SDK',h);
    const stEl=()=>document.getElementById('sdkstat');
    document.getElementById('sdksave').onclick=async()=>{
      const r=await (await fetch('/api/sdk',{method:'POST',
        headers:{'Content-Type':'application/json'},
        body:JSON.stringify({path:document.getElementById('sdkpath').value})})).json();
      document.getElementById('sdkerr').textContent=r.error||'';
      if(!r.error) stEl().textContent=r.sdk_path?'saved — now build the index':'cleared';
    };
    document.getElementById('sdkindex').onclick=async()=>{
      stEl().textContent='indexing...';
      const r=await (await fetch('/api/sdk/index',{method:'POST'})).json();
      stEl().textContent=r.structs+' types, '+r.symbols+' library symbols ('+
        r.headers+' headers, '+r.libs+' libs)';
    };
    document.getElementById('sdkscan').onclick=async()=>{
      stEl().textContent='scanning image...';
      const r=await (await fetch('/api/sdk/scan',{method:'POST'})).json();
      stEl().textContent=r.found+' matched'+
        (r.ambiguous?', '+r.ambiguous+' ambiguous (skipped)':'');
      const box=document.getElementById('sdkres');
      if(!r.matches.length){box.innerHTML='<div style="opacity:.6">no library code found in this image</div>';return;}
      let o='<div style="margin:4px 0"><button id="sdkapply">Apply '+
            r.matches.length+' names</button></div>';
      for(const m of r.matches.slice(0,300)){
        o+='<div><a href="#" data-a="'+m.addr+'" class="sdkgo">'+m.addr+'</a> '+
           esc(m.demangled||m.name)+' <span style="opacity:.6">'+esc(m.lib)+
           ' · '+m.size+'b</span></div>';
      }
      box.innerHTML=o;
      document.getElementById('sdkapply').onclick=async()=>{
        const a=await (await fetch('/api/sdk/scan?apply=1',{method:'POST'})).json();
        stEl().textContent='renamed '+a.renamed+' functions';
        renderList($('#search').value||'');
      };
      box.querySelectorAll('.sdkgo').forEach(el=>el.onclick=(ev)=>{
        ev.preventDefault(); open(el.dataset.a);
      });
    };
    document.getElementById('sdkinl').onclick=async()=>{
      stEl().textContent='searching for inlined library code...';
      const r=await (await fetch('/api/sdk/inlined',{method:'POST'})).json();
      stEl().textContent=r.funcs_with_inlined+' functions contain '+
        r.regions+' inlined regions';
      const box=document.getElementById('sdkres');
      if(!r.funcs.length){box.innerHTML='<div style="opacity:.6">none found</div>';return;}
      // Longest runs first: a 24-instruction match is far stronger evidence
      // than the 8-instruction minimum.
      const rows=[];
      for(const f of r.funcs)
        for(const h of f.hits) rows.push({f:f,h:h});
      rows.sort((a,b)=>b.h.insns-a.h.insns);
      let o='<div style="opacity:.7;margin:4px 0">INLINED LIBRARY CODE '+
            '<span style="opacity:.7">(annotation only — the function contains '+
            'this code, it is not this function)</span></div>';
      for(const x of rows.slice(0,300)){
        o+='<div><a href="#" data-a="'+x.f.addr+'" class="sdkgo">'+x.f.addr+
           '</a> +0x'+x.h.at.toString(16)+' <b>'+x.h.insns+' insns</b> '+
           esc(x.h.demangled||x.h.name)+
           ' <span style="opacity:.6">'+esc(x.h.lib)+'</span></div>';
      }
      box.innerHTML=o;
      box.querySelectorAll('.sdkgo').forEach(el=>el.onclick=(ev)=>{
        ev.preventDefault(); open(el.dataset.a);
      });
    };
    document.getElementById('sdkq').oninput=async(e)=>{
      const q=e.target.value.trim();
      const box=document.getElementById('sdkres');
      if(q.length<2){box.innerHTML='';return;}
      const [types,syms,imps]=await Promise.all([
        jget('/api/sdk/structs?q='+encodeURIComponent(q)),
        jget('/api/sdk/symbols?q='+encodeURIComponent(q)),
        jget('/api/imports?q='+encodeURIComponent(q))]);
      let o='';
      if(imps.length){
        o+='<div style="opacity:.7;margin:4px 0">KERNEL / XAM'+
           ' <span style="opacity:.7">(imported entries link to the thunk)'+
           '</span></div>';
        for(const i of imps.slice(0,40)){
          const label=i.name||(i.module+' ordinal '+i.ordinal);
          o+='<div style="margin-bottom:2px">'+
             (i.imported
               ?'<span style="opacity:.6">'+i.addr+'</span> '
               :'<span style="opacity:.35">not imported</span> ')+
             esc(label)+' <span style="opacity:.6">'+esc(i.module)+
             ' #'+i.ordinal+'</span>'+
             (i.callers
               ?' <a href="#" class="impx" data-a="'+i.addr+'">'+i.callers+
                 ' call'+(i.callers==1?'':'s')+'</a>'
               :(i.imported?' <span style="opacity:.4">no calls found</span>':''))+
             '<div class="impl" id="impl'+i.addr+'"></div></div>';
        }
      }
      if(types.length){
        o+='<div style="opacity:.7;margin:4px 0">TYPES</div>';
        for(const t of types.slice(0,25)){
          o+='<div style="margin-bottom:6px"><b>'+esc(t.name)+'</b> '+
             '<span style="opacity:.6">'+esc(t.header)+
             (t.size?' — '+t.size+' bytes':' — size unknown')+'</span>';
          for(const f of t.fields){
            o+='<div style="opacity:.85;margin-left:12px">+0x'+
               f.offset.toString(16).toUpperCase().padStart(2,'0')+'  '+
               esc(f.type)+' '+esc(f.name)+(f.count>1?'['+f.count+']':'')+'</div>';
          }
          o+='</div>';
        }
      }
      if(syms.length){
        o+='<div style="opacity:.7;margin:8px 0 4px">LIBRARY FUNCTIONS</div>';
        for(const s of syms.slice(0,40)){
          o+='<div style="margin-bottom:2px">'+esc(s.demangled||s.name)+
             ' <span style="opacity:.6">'+esc(s.lib)+'</span></div>';
        }
      }
      box.innerHTML=o||'<div style="opacity:.6">nothing found</div>';
      box.querySelectorAll('.sdkgo').forEach(el=>el.onclick=(ev)=>{
        ev.preventDefault(); open(el.dataset.a);
      });
      // The thunk itself is not decompiled, so the useful destination is the
      // list of functions that call it.
      box.querySelectorAll('.impx').forEach(el=>el.onclick=async(ev)=>{
        ev.preventDefault();
        const host=document.getElementById('impl'+el.dataset.a);
        if(host.innerHTML){host.innerHTML='';return;}
        const x=await jget('/api/import/xrefs?addr='+el.dataset.a);
        let h='';
        for(const c of (x.callers||[]))
          h+='<div style="margin-left:14px"><a href="#" class="sdkgo" data-a="'+
             c.addr+'">'+c.addr+'</a> '+esc(c.name)+'</div>';
        host.innerHTML=h||'<div style="margin-left:14px;opacity:.5">none</div>';
        host.querySelectorAll('.sdkgo').forEach(a=>a.onclick=(e2)=>{
          e2.preventDefault(); open(a.dataset.a);
        });
      });
    };
  }
  $('#sdkbtn').onclick=sdkPane;
  $('#paneclose').onclick=()=>$('#pane').classList.remove('open');
  document.querySelectorAll('.tab').forEach(t=>t.onclick=()=>{
    MODE=t.dataset.m;
    document.querySelectorAll('.tab').forEach(x=>x.classList.toggle('sel',x===t));
    $('#search').placeholder=MODE==='fn'?'filter functions...':
      (MODE==='str'?'search strings...':
      (MODE==='imp'?'search imports (XamUserGetName, 526, xam)...':
       'search code (e.g. 0x8306DB74)...'));
    $('#search').value='';
    if(MODE==='fn')renderList('');
    else if(MODE==='str')searchStrings('');
    else if(MODE==='imp')searchImports('');
    else $('#flist').innerHTML='';
  });
}

// Imports: what the image calls in xam and the kernel, by name or ordinal.
// The thunk is not decompiled, so a row expands to the functions that call it
// - that list is the thing worth navigating to.
async function searchImports(q){
  const L=$('#flist');
  const imps=await jget('/api/imports?q='+encodeURIComponent(q));
  L.innerHTML='';
  if(!imps.length){
    L.innerHTML='<div class="fn" style="opacity:.6">nothing found</div>';
    return;
  }
  for(const i of sortRows(imps)){
    const row=document.createElement('div');
    row.className='fn';
    const label=i.name||(i.module+' ordinal '+i.ordinal);
    row.innerHTML='<span class="a">'+(i.imported?i.addr:'--------')+'</span>'+
      esc(label)+' <span class="r">'+
      (i.imported?(i.callers+' call'+(i.callers==1?'':'s')):'not imported')+
      '</span>';
    row.title=i.module+' ordinal '+i.ordinal;
    const list=document.createElement('div');
    list.style.display='none';
    if(i.imported){
      row.onclick=async()=>{
        if(list.style.display==='block'){list.style.display='none';return;}
        list.style.display='block';
        if(!list.innerHTML){
          const x=await jget('/api/import/xrefs?addr='+i.addr);
          let h='';
          for(const c of (x.callers||[]))
            h+='<div class="fn" style="padding-left:22px" data-a="'+c.addr+
               '"><span class="a">'+c.addr+'</span>'+esc(c.name)+'</div>';
          list.innerHTML=h||
            '<div class="fn" style="padding-left:22px;opacity:.5">no calls found</div>';
          list.querySelectorAll('[data-a]').forEach(el=>{
            el.onclick=(ev)=>{ev.stopPropagation();open(el.dataset.a);};
          });
        }
      };
    }
    L.appendChild(row);
    L.appendChild(list);
  }
}
)HTML"
// Second split: MSVC caps one string literal at 16380 chars.
R"HTML(
// ---- side pane: memory, tables, call graph, data xrefs, notes ----
function pane(title,html){
  $('#panetitle').textContent=title;
  $('#panebody').innerHTML=html;
  $('#pane').classList.add('open');
}
// Hex/ASCII dump with per-word interpretation; addresses and symbols click through.
async function memView(addr,len){
  addr=(addr||'').replace(/^0x/i,'').trim(); if(!addr)return;
  const d=await jget(`/api/mem?addr=${addr}&len=${(len||0x100).toString(16)}`);
  if(d.error){pane('Memory','not mapped');return;}
  if(!d.mapped){pane('Memory @ '+d.addr,
    `<span class="r">section ${esc(d.section||'(none)')} — no file data at this address\n`
   +`(uninitialised/BSS region: exists at runtime, not stored in the image)</span>`);return;}
  let h=`<span class="r">section ${esc(d.section||'?')}  </span>`
       +`<span class="n" data-mem="${(parseInt(addr,16)-0x100).toString(16)}">[-]</span> `
       +`<span class="n" data-mem="${(parseInt(addr,16)+d.len).toString(16)}">[+]</span>\n\n`;
  d.rows.forEach(r=>{h+=`<span class="r">${r.addr}</span>  <span class="h">${esc(r.hex)}</span> <span class="a">${esc(r.ascii)}</span>\n`;});
  if(d.words.length){
    h+='\n<span class="r">-- words --</span>\n';
    d.words.forEach(w=>{
      const note=w.note?`  ${esc(w.note)}`:'';
      h+=`<span class="r">${w.addr}</span>  <span class="n" data-mem="${w.u32}">${w.u32}</span>`
        +`  <span class="n" data-xref="${w.addr}">xref</span>${note}\n`;
    });
  }
  pane('Memory @ '+d.addr,h);
  bindPane();
}
async function dataXref(addr){
  const d=await jget('/api/dataxref?addr='+addr);
  const fmt=l=>l.length?l.map(x=>`  <span class="n" data-go="${x.addr}">${x.addr} ${esc(x.name)}</span>`).join('\n'):'  (none)';
  pane('Data xrefs @ '+d.addr,
    `<span class="r">READS</span>\n${fmt(d.reads)}\n\n<span class="r">WRITES</span>\n${fmt(d.writes)}`);
  bindPane();
}
async function scanTables(){
  const a=prompt('Scan start (hex):',CUR||META.entry); if(!a)return;
  const n=prompt('Length (hex):','2000'); if(!n)return;
  const st=prompt('Stride (4/8/12/16):','8'); if(!st)return;
  const rows=await jget(`/api/tables?addr=${a.replace(/^0x/i,'')}&len=${n.replace(/^0x/i,'')}&stride=${st}`);
  if(!rows.length){pane('Tables','no pointer tables found');return;}
  let h='';
  rows.forEach(t=>{
    h+=`<span class="r">table ${t.addr}  stride ${t.stride}  ${t.count} entries</span>\n`;
    t.entries.forEach(e=>{h+=`  <span class="r">${e.slot}</span> <span class="a">${esc(e.name)}</span>`
      +`  -> <span class="n" data-go="${e.target}">${e.target} ${esc(e.tname)}</span>\n`;});
    h+='\n';
  });
  pane('Pointer tables',h);
  bindPane();
}
async function callGraph(){
  if(!CUR)return;
  const dir=confirm('OK = callers (up), Cancel = callees (down)')?'up':'down';
  const rows=await jget(`/api/callgraph?addr=${CUR}&depth=4&dir=${dir}`);
  let h='';
  rows.forEach(r=>{h+='  '.repeat(r.depth)+`<span class="n" data-go="${r.addr}">${r.addr} ${esc(r.name)}</span>\n`;});
  pane(`Call graph (${dir}) from ${CUR}`,h||'(none)');
  bindPane();
}
)HTML"
// Third split: MSVC caps one string literal at 16380 chars.
R"HTML(
// Assert a prototype. Usage cannot recover documented names/types, so state
// them:  "NTSTATUS|1:HANDLE:FileHandle|2:HANDLE:Event|..."
async function editSig(){
  if(!CUR)return;
  const s=prompt('Signature  ret|pos:type:name|...  (blank clears):','');
  if(s===null)return;
  await jpost('/api/rename',{kind:'sig',addr:CUR,name:s});
  open(CUR);
}
async function editNote(){
  if(!CUR)return;
  const f=await jget('/api/func?addr='+CUR);
  const n=prompt('Note for this function:',f.comment||'');
  if(n===null)return;
  await jpost('/api/rename',{kind:'comment',addr:CUR,name:n});
  open(CUR);
}
function bindPane(){
  document.querySelectorAll('#panebody .n').forEach(e=>{
    if(e.dataset.mem!==undefined)e.onclick=()=>memView(e.dataset.mem);
    else if(e.dataset.xref!==undefined)e.onclick=()=>dataXref(e.dataset.xref);
    else if(e.dataset.go!==undefined)e.onclick=()=>open(e.dataset.go);
  });
}
// Code tab: substring search across every function's decompiled body.
let CTIMER=null,CSEQ=0;
function searchCode(q){
  clearTimeout(CTIMER);
  const seq=++CSEQ;
  if(!q){$('#flist').innerHTML='';return;}
  CTIMER=setTimeout(async()=>{
    const rows=await jget('/api/search?q='+encodeURIComponent(q));
    if(seq!==CSEQ)return;
    const L=$('#flist');L.innerHTML='';
    if(!rows.length){L.innerHTML='<div class="sr"><span class="m">no matches</span></div>';return;}
    rows.forEach(r=>{
      const d=document.createElement('div');d.className='sr';
      d.innerHTML=`<span class="m">${r.addr} ${esc(r.name)} (${r.hits})</span><span class="t">${esc(r.line)}</span>`;
      d.onclick=()=>open(r.addr);
      L.appendChild(d);
    });
  },250);
}
// Strings tab: substring search, each hit listing the functions that use it.
let STIMER=null,SSEQ=0;
function searchStrings(q){
  clearTimeout(STIMER);
  const seq=++SSEQ;
  STIMER=setTimeout(async()=>{
    const rows=await jget('/api/strings?q='+encodeURIComponent(q));
    if(seq!==SSEQ)return;  // a newer search already went out — drop this one
    const L=$('#flist');L.innerHTML='';
    if(!rows.length){L.innerHTML='<div class="sr"><span class="m">no matches</span></div>';return;}
    rows.forEach(s=>{
      const d=document.createElement('div');d.className='sr';
      const refs=s.refs.length?s.refs.map(r=>esc(r.name)).join(', '):'(no refs)';
      d.innerHTML=`<span class="t">"${esc(s.text)}"</span><span class="m">${s.addr} &#8592; ${refs}</span>`;
      if(s.refs.length)d.onclick=()=>open(s.refs[0].addr);
      L.appendChild(d);
    });
  },180);
}
// Address order is the file's own order; the others answer "what is big" and
// "what is central", which is where an unfamiliar image is usually entered.
function sortRows(rows){
  const by=($('#sortby')||{}).value||'addr';
  if(by==='addr')return rows;
  const out=rows.slice();
  if(by==='name')
    out.sort((a,b)=>(a.name||'').localeCompare(b.name||''));
  else if(by==='size')
    out.sort((a,b)=>(b.size||0)-(a.size||0));
  else if(by==='callers')
    out.sort((a,b)=>(b.nref||b.callers||0)-(a.nref||a.callers||0));
  return out;
}
function renderList(filter){
  const f=filter.toLowerCase();
  const L=$('#flist');L.innerHTML='';
  for(const fn of sortRows(FUNS)){
    if(f && !fn.name.toLowerCase().includes(f) && !fn.addr.toLowerCase().includes(f))continue;
    const d=document.createElement('div');
    d.className='fn'+(CUR===fn.addr?' sel':'');
    d.dataset.addr=fn.addr;
    d.innerHTML=`<span class="a">${fn.addr}</span>${esc(fn.name)}<span class="r">${fn.nref?('&#8592;'+fn.nref):''}</span>`;
    d.onclick=()=>open(fn.addr);
    L.appendChild(d);
  }
}
function esc(s){return s.replace(/[&<>]/g,c=>({'&':'&amp;','<':'&lt;','>':'&gt;'}[c]));}
document.addEventListener('wheel',function(e){
  if(!e.shiftKey)return;
  const L=$('#flist');
  if(!L||!L.contains(e.target))return;
  const d=e.deltaX!==0?e.deltaX:(e.deltaY!==0?e.deltaY:e.deltaZ);
  if(!d)return;
  L.scrollLeft+=d;
  e.preventDefault();
},{passive:false});

async function open(addr){
  CUR=addr;
  const f=await jget('/api/func?addr='+addr);
  if(f.error){alert('No code at '+addr);return;}
  // A function built on demand isn't in the list yet — pull it in.
  if(!FUNS.some(x=>x.addr===addr)){
    FUNS=await jget('/api/functions');
    FADDR={};FUNS.forEach(x=>FADDR[x.name]=x.addr);
    renderList($('#search').value);
  }
  $('#cname').textContent=f.name;rc($('#cname'),()=>renameFunc(addr,f.name));
  $('#caddr').textContent=f.addr;
  const note=f.comment?`<div class="cmt">// ${esc(f.comment)}</div>`:'';
  $('#code').innerHTML=note+highlight(f.code,f.vars,f.structs,f.labels);
  bindTokens(addr,f.vars,f.labels);
  $('#callers').innerHTML='';f.callers.forEach(x=>$('#callers').appendChild(xref(x)));
  $('#callees').innerHTML='';f.callees.forEach(x=>$('#callees').appendChild(xref(x)));
  const V=$('#vars');V.innerHTML='';
  f.vars.forEach(v=>{
    const row=document.createElement('div');row.className='vr';
    const s=v.suggest&&v.suggest!==v.name?`<span class="s" title="apply">${esc(v.suggest)}</span>`:'';
    row.innerHTML=`<span class="tok" data-v="${v.id}">${esc(v.name)}</span>${s}`;
    rc(row.querySelector('.tok'),()=>renameVar(addr,v.id,v.name));
    if(s)row.querySelector('.s').onclick=()=>doVar(addr,v.id,v.suggest);
    V.appendChild(row);
  });
  const S=$('#structs');S.innerHTML='';
  (f.structs||[]).forEach(st=>{
    const box=document.createElement('div');box.className='st';
    box.innerHTML=`<div class="h">${esc(st.disp)} : <span class="ty" title="set type / unify">${esc(st.type)}</span></div>`;
    rc(box.querySelector('.ty'),()=>setType(addr,st.base,st.type));
    st.fields.forEach(fl=>{
      const r=document.createElement('div');r.className='fld';
      r.innerHTML=`<span class="n" title="${esc(fl.type)}">${esc(fl.name)}</span><span class="o">+0x${fl.off.replace(/^0+/,'')||'0'}</span>`;
      rc(r.querySelector('.n'),()=>renameField(addr,st.base,fl.off,fl.name));
      box.appendChild(r);
    });
    S.appendChild(box);
  });
  const G=$('#globals');G.innerHTML='';
  (f.globals||[]).forEach(g=>{
    const box=document.createElement('div');box.className='st';
    box.innerHTML=`<div class="h">${esc(g.name)} <span class="o mo" data-mem="${g.addr}">@${g.addr}</span></div>`;
    box.querySelector('.mo').onclick=()=>memView(g.addr);
    g.fields.forEach(fl=>{
      const r=document.createElement('div');r.className='fld';
      const fa=(parseInt(g.addr,16)+parseInt(fl.off,16)).toString(16).toUpperCase();
      r.innerHTML=`<span class="n">${esc(fl.name)}</span><span class="o mo" title="data xrefs">+0x${fl.off}</span>`;
      r.querySelector('.o').onclick=()=>dataXref(fa);
      rc(r.querySelector('.n'),()=>renameGField(addr,g.addr,fl.off,fl.name));
      box.appendChild(r);
    });
    G.appendChild(box);
  });
  document.querySelectorAll('.fn').forEach(e=>e.classList.toggle('sel',e.dataset.addr===addr));
}
async function renameGField(fnAddr,base,off,cur){
  const n=prompt('Rename global field (+0x'+off+'):',cur);
  if(n===null)return;
  await jpost('/api/rename',{kind:'gfield',addr:base,off,name:n});
  open(fnAddr);
}
async function bindGlobal(){
  const a=prompt('Bind struct at address (hex):','');
  if(!a)return;
  const s=prompt('Size in bytes (hex):','40');
  if(!s)return;
  const n=prompt('Struct name:','g_struct');
  if(n===null)return;
  await jpost('/api/rename',{kind:'gstruct',addr:a.replace(/^0x/i,''),off:s.replace(/^0x/i,''),name:n});
  if(CUR)open(CUR);
}
function xref(x){const a=document.createElement('span');a.className='x';a.textContent=x.name;a.title=x.addr;a.onclick=()=>open(x.addr);a.oncontextmenu=e=>{e.preventDefault();renameFunc(x.addr,x.name);};return a;}

function escAttr(s){return s.replace(/[&<>"]/g,c=>({'&':'&amp;','<':'&lt;','>':'&gt;','"':'&quot;'}[c]));}
// Left-to-right tokenizer: escapes literal text, and emits clickable spans for
// variable names and for `base->field` accesses (field spans carry base/off so
// a click renames the field on its unified type).
function highlight(code,vars,structs,labels){
  const varNames=new Set(vars.map(v=>v.name));
  const labelId={};
  (labels||[]).forEach(l=>labelId[l.name]=l.id);
  const fieldLk={};  // "disp fieldname" -> {base, off}
  (structs||[]).forEach(s=>s.fields.forEach(fl=>{
    fieldLk[s.disp+' '+fl.name]={base:s.base,off:fl.off};
  }));
  const isIdStart=c=>c&&/[A-Za-z_]/.test(c);
  const isId=c=>c&&/[A-Za-z0-9_]/.test(c);
  const varSpan=w=>varNames.has(w)
    ?`<span class="tok" data-name="${escAttr(w)}">${esc(w)}</span>`:esc(w);
  let out='',i=0,n=code.length;
  while(i<n){
    const c=code[i];
    if(isIdStart(c)){
      let j=i+1;while(j<n&&isId(code[j]))j++;
      const word=code.slice(i,j);
      // LABEL_XXXX: definition gets an anchor id; a reference (goto target) is
      // a clickable jump.
      if(labelId[word]!==undefined){
        const id=labelId[word];
        if(code[j]===':') out+=`<span class="lbldef" id="lbl-${id}" data-id="${id}" data-name="${escAttr(word)}">${esc(word)}</span>`;
        else out+=`<span class="lbl" data-target="${id}">${esc(word)}</span>`;
        i=j;continue;
      }
      if(code[j]==='-'&&code[j+1]==='>'&&isIdStart(code[j+2])){
        let k=j+2;while(k<n&&isId(code[k]))k++;
        const field=code.slice(j+2,k);
        out+=varSpan(word)+'-&gt;';
        const lk=fieldLk[word+' '+field];
        out+=lk
          ?`<span class="tok ftok" data-base="${escAttr(lk.base)}" data-off="${lk.off}" data-fname="${escAttr(field)}">${esc(field)}</span>`
          :esc(field);
        i=k;continue;
      }
      // A call target -> clickable jump. Known functions resolve by name;
      // sub_XXXXXXXX carries its own address, so targets that whole-program
      // detection missed are still navigable (built on demand server-side).
      if(code[j]==='('){
        let ta=FADDR[word];
        if(ta===undefined){
          const m=/^sub_([0-9A-Fa-f]{6,8})$/.exec(word);
          if(m)ta=m[1].toUpperCase().padStart(8,'0');
        }
        if(ta!==undefined){
          out+=`<span class="fnlink" data-addr="${ta}">${esc(word)}</span>`;
          i=j;continue;
        }
      }
      out+=varSpan(word);i=j;continue;
    }
    // String literal: consume it whole so its contents are never tokenized.
    if(c==='"'){
      let j=i+1;
      while(j<n&&code[j]!=='"'){ if(code[j]==='\\')j++; j++; }
      out+=`<span class="str">${esc(code.slice(i,Math.min(j+1,n)))}</span>`;
      i=j+1;continue;
    }
    if(/[0-9]/.test(c)){
      let j=i+1;while(j<n&&/[0-9A-Fa-fxX]/.test(code[j]))j++;
      out+=`<span class="num">${esc(code.slice(i,j))}</span>`;i=j;continue;
    }
    out+=esc(c);i++;
  }
  return out;
}
function bindTokens(addr,vars){
  const byName={};vars.forEach(v=>byName[v.name]=v.id);
  document.querySelectorAll('#code .tok').forEach(t=>{
    if(t.dataset.fname!==undefined){
      rc(t,()=>renameField(addr,t.dataset.base,t.dataset.off,t.dataset.fname));
    }else if(t.dataset.name!==undefined){
      const id=byName[t.dataset.name];
      if(id)rc(t,()=>renameVar(addr,id,t.dataset.name));
    }
  });
  // Call target -> navigate to that function.
  document.querySelectorAll('#code .fnlink').forEach(t=>{
    t.onclick=()=>open(t.dataset.addr);
    t.oncontextmenu=e=>{e.preventDefault();
      renameFunc(t.dataset.addr,t.textContent);};
  });
  // goto <label> -> jump to its definition.
  document.querySelectorAll('#code .lbl').forEach(t=>{
    t.onclick=()=>jumpTo(t.dataset.target);
  });
  // label definition -> context menu.
  document.querySelectorAll('#code .lbldef').forEach(t=>{
    t.onclick=e=>{
      e.stopPropagation();
      const id=t.dataset.id, ah='0x'+id.slice(6);
      showMenu(e.pageX,e.pageY,[
        {label:'Rename…',fn:()=>renameLabel(addr,id,t.dataset.name)},
        {label:'Highlight gotos',fn:()=>flashGotos(id)},
        {label:'Copy address ('+ah+')',fn:()=>copyText(ah)},
      ]);
    };
  });
}
function jumpTo(id){
  const el=document.getElementById('lbl-'+id);
  if(el){el.scrollIntoView({block:'center'});
    el.classList.add('flash');setTimeout(()=>el.classList.remove('flash'),900);}
}
function flashGotos(id){
  document.querySelectorAll('#code .lbl').forEach(t=>{
    if(t.dataset.target===id){t.classList.add('flash');
      setTimeout(()=>t.classList.remove('flash'),1400);}
  });
}
function copyText(s){if(navigator.clipboard)navigator.clipboard.writeText(s);}
function closeMenu(){const e=document.getElementById('ctxmenu');if(e)e.remove();}
function showMenu(x,y,items){
  closeMenu();
  const m=document.createElement('div');m.id='ctxmenu';m.className='menu';
  items.forEach(it=>{const b=document.createElement('div');b.className='mi';
    b.textContent=it.label;b.onclick=()=>{closeMenu();it.fn();};m.appendChild(b);});
  m.style.left=x+'px';m.style.top=y+'px';document.body.appendChild(m);
}
document.addEventListener('mousedown',e=>{
  if(!e.target.closest('#ctxmenu')&&!e.target.classList.contains('lbldef'))closeMenu();
});
async function renameLabel(addr,id,cur){
  const n=prompt('Rename label:',cur);
  if(n===null||n===''||n===cur)return;
  await jpost('/api/rename',{kind:'label',addr,id,name:n});
  open(addr);
}
async function renameFunc(addr,cur){
  const n=prompt('Rename function:',cur);
  if(n===null||n===''||n===cur)return;
  await jpost('/api/rename',{kind:'func',addr,name:n});
  const i=FUNS.findIndex(f=>f.addr===addr);if(i>=0)FUNS[i].name=n;
  FADDR={};FUNS.forEach(f=>FADDR[f.name]=f.addr);
  renderList($('#search').value);open(addr);
}
async function renameVar(addr,id,cur){
  const n=prompt('Rename local:',cur);
  if(n===null)return;
  await doVar(addr,id,n);
}
async function doVar(addr,id,n){
  await jpost('/api/rename',{kind:'var',addr,id,name:n});
  open(addr);
}
async function renameField(addr,base,off,cur){
  const n=prompt('Rename field (+0x'+(off.replace(/^0+/,'')||'0')+'):',cur);
  if(n===null)return;
  await jpost('/api/rename',{kind:'field',addr,base,off,name:n});
  open(addr);
}
async function setType(addr,base,cur){
  const n=prompt('Set struct type name (matching an existing type unifies with it):',cur);
  if(n===null)return;
  await jpost('/api/rename',{kind:'settype',addr,base,type:n});
  open(addr);
}
boot();
</script>
</body></html>)HTML";

// ---------------------------------------------------------------------------
// API documentation
//
// The spec is the source of truth: /api/openapi.json serves it verbatim (it
// imports into Postman/Insomnia as-is) and /api/ renders it. Adjacent literals
// concatenate, which is also how the 16380 char literal cap is dodged.
// ---------------------------------------------------------------------------

const char* kOpenApiJson =
R"JSON({
"openapi":"3.0.3",
"info":{
  "title":"XEXMagic API",
  "version":"1.0.0",
  "description":"Xbox 360 XEX reverse engineering. Addresses are HEX WITHOUT a 0x prefix unless noted. State persists to <image>.xdb."
},
"servers":[{"url":"/"}],
"tags":[
  {"name":"image","description":"Program-wide queries"},
  {"name":"memory","description":"Raw memory and data layout"},
  {"name":"functions","description":"Functions, decompilation, xrefs"},
  {"name":"sdk","description":"Xbox 360 SDK type and symbol matching"},
  {"name":"interp","description":"PPC interpreter"},
  {"name":"edit","description":"Renames and annotations"}
],
"paths":{
"/api/meta":{"get":{"tags":["image"],"summary":"Image metadata","description":"Entry point, title id, section count, function count.","responses":{"200":{"description":"OK"}}}},
"/api/sections":{"get":{"tags":["image"],"summary":"PE sections","responses":{"200":{"description":"OK"}}}},
"/api/globals":{"get":{"tags":["image"],"summary":"Detected globals","responses":{"200":{"description":"OK"}}}},
"/api/export":{"get":{"tags":["image"],"summary":"Export C header","description":"Every named struct and function signature as a .h. Returns text/plain, not JSON.","responses":{"200":{"description":"C header text"}}}},
"/api/export/c":{"get":{"tags":["image"],"summary":"Export the whole image decompiled to C","description":"Every function through the same renderer the UI uses, so all renames, types, signatures and notes are included. Returns text/plain as an attachment; can take minutes and run to tens of MB on a full XEX.","responses":{"200":{"description":"C source text"}}}},
"/api/strings":{"get":{"tags":["image"],"summary":"Search strings","parameters":[{"name":"q","in":"query","schema":{"type":"string"},"description":"Substring. Percent-encoded; + means space. Capped at 300 hits."}],"responses":{"200":{"description":"OK"}}}},
"/api/search":{"get":{"tags":["image"],"summary":"Global search","description":"Functions, labels and symbols by name or address. Capped at 300 hits.","parameters":[{"name":"q","in":"query","schema":{"type":"string"}}],"responses":{"200":{"description":"OK"}}}},
"/api/mem":{"get":{"tags":["memory"],"summary":"Read memory","description":"Blank rows mean the served image has a bad mapping - restart that instance rather than trusting its output. NOTE len is HEX: len=65536 means 0x65536, not 64 KB. Over the 0x100000 cap the request is clamped, and the response echoes the byte count actually read in \"len\" - check it before trusting a scan.","parameters":[
  {"name":"addr","in":"query","required":true,"schema":{"type":"string"},"example":"82000000","description":"Guest address, HEX"},
  {"name":"len","in":"query","required":true,"schema":{"type":"string"},"example":"100","description":"Byte count, HEX. Max 0x100000; larger is clamped, and \"len\" in the response says what was read. The words[] array stays capped at 0x100 regardless."}],"responses":{"200":{"description":"OK"}}}},
"/api/tables":{"get":{"tags":["memory"],"summary":"Interpret memory as a table","parameters":[
  {"name":"addr","in":"query","required":true,"schema":{"type":"string"},"description":"HEX"},
  {"name":"len","in":"query","required":true,"schema":{"type":"string"},"description":"HEX. Max 0x100000; larger is clamped, not reduced to the default."},
  {"name":"stride","in":"query","required":true,"schema":{"type":"integer"},"description":"Row size in bytes, DECIMAL"}],"responses":{"200":{"description":"OK"}}}},
"/api/dataxref":{"get":{"tags":["memory"],"summary":"Data cross-references","description":"Everything that reads or writes an address.","parameters":[{"name":"addr","in":"query","required":true,"schema":{"type":"string"},"description":"HEX"}],"responses":{"200":{"description":"OK"}}}},
"/api/functions":{"get":{"tags":["functions"],"summary":"List all functions","responses":{"200":{"description":"OK"}}}},
"/api/func":{"get":{"tags":["functions"],"summary":"Decompile one function","description":"Disassembly, decompiled C, locals and xrefs.","parameters":[{"name":"addr","in":"query","required":true,"schema":{"type":"string"},"example":"822F8F48","description":"HEX"}],"responses":{"200":{"description":"OK"}}}},
"/api/callgraph":{"get":{"tags":["functions"],"summary":"Call graph around a function","parameters":[
  {"name":"addr","in":"query","required":true,"schema":{"type":"string"},"description":"HEX"},
  {"name":"depth","in":"query","schema":{"type":"integer","default":3},"description":"DECIMAL. 0 means default 3."},
  {"name":"dir","in":"query","schema":{"type":"string","enum":["up","down"],"default":"up"},"description":"up = callers, down = callees. Anything other than 'down' means up."}],"responses":{"200":{"description":"OK"}}}},
)JSON"
R"JSON(
"/api/sdk":{
  "get":{"tags":["sdk"],"summary":"Get SDK config","description":"Configured path and index counts. Path is saved to xexmagic.json beside the exe and survives restarts.","responses":{"200":{"description":"OK"}}},
  "post":{"tags":["sdk"],"summary":"Set SDK path","description":"Stores and validates. An empty path clears it. Returns {\"error\":\"...\"} on failure, still with HTTP 200.","requestBody":{"required":true,"content":{"application/json":{"schema":{"type":"object","properties":{"path":{"type":"string"}}},"example":{"path":"C:\\\\Program Files (x86)\\\\Microsoft Xbox 360 SDK"}}}},"responses":{"200":{"description":"OK"}}}},
"/api/sdk/index":{"post":{"tags":["sdk"],"summary":"Build the SDK index","description":"Parses headers and libs. Takes roughly 20s on a full SDK. No body.","responses":{"200":{"description":"Config with updated counts"}}}},
"/api/imports":{"get":{"tags":["image"],"summary":"Search kernel/xam entry points","description":"Searches the complete baked xam and xboxkrnl ordinal tables by name, module or exact ordinal - not only what this image imports. Entries the image does import come first and carry `imported:true` and the thunk address in `addr`. Capped at 300 hits; an empty q lists from the top.","parameters":[{"name":"q","in":"query","schema":{"type":"string"},"example":"XamUser"}],"responses":{"200":{"description":"OK"}}}},
"/api/import/xrefs":{"get":{"tags":["image"],"summary":"Functions that call one import","description":"Direct calls to the thunk plus functions that load the value slot, which is how an indirect call reaches it. addr is either address, HEX.","parameters":[{"name":"addr","in":"query","required":true,"schema":{"type":"string"},"example":"8238EC44"}],"responses":{"200":{"description":"OK"}}}},
"/api/sdk/structs":{"get":{"tags":["sdk"],"summary":"Search SDK structs","parameters":[{"name":"q","in":"query","schema":{"type":"string"},"description":"Capped at 200 hits."}],"responses":{"200":{"description":"OK"}}}},
"/api/sdk/symbols":{"get":{"tags":["sdk"],"summary":"Search SDK symbols","description":"Functions the SDK libs define. Capped at 300 hits.","parameters":[{"name":"q","in":"query","schema":{"type":"string"}}],"responses":{"200":{"description":"OK"}}}},
"/api/sdk/match":{"get":{"tags":["sdk"],"summary":"Match a struct layout","description":"Candidate SDK types for an inferred layout. Every observed offset must land on a declared field or the candidate is dropped. Top 25.","parameters":[
  {"name":"size","in":"query","required":true,"schema":{"type":"integer"},"example":64,"description":"Struct size. Decimal, or 0x-prefixed hex."},
  {"name":"offsets","in":"query","required":true,"schema":{"type":"string"},"example":"0,8,48","description":"Comma separated observed offsets."}],"responses":{"200":{"description":"Scored candidates"}}}},
"/api/sdk/scan":{"post":{"tags":["sdk"],"summary":"Scan image for SDK code","description":"Byte-exact compare with COFF relocations masked. Never overwrites an existing name. Regions under 32 bytes, more than half relocated, or ambiguous are skipped.","parameters":[{"name":"apply","in":"query","schema":{"type":"string","enum":["1"]},"description":"apply=1 performs the renames. Omit to review matches first."}],"responses":{"200":{"description":"Matches"}}}},
"/api/sdk/inlined":{"post":{"tags":["sdk"],"summary":"Find inlined SDK code","description":"8-instruction windows; windows shared by more than one routine are dropped at index time. ANNOTATES only, never renames.","responses":{"200":{"description":"Inlined regions with occurrence counts"}}}},
"/api/run":{"post":{"tags":["interp"],"summary":"Run a guest function","description":"Executes real guest code so a conclusion can be tested instead of argued. LIMITS: 32-bit GPRs, XER carry only (no OE/OV/SO), CR has no SO bit, no FP, VMX skipped and counted.\n\nGOTCHAS: send steps as a STRING - an unquoted number followed by another key silently caps the run at 1 step. args must be a COMMA STRING, not a JSON array; an array is ignored and the callee runs with zeroed registers. Numbers in args parse as HEX.","requestBody":{"required":true,"content":{"application/json":{"schema":{"type":"object","properties":{
  "entry":{"type":"string","description":"Function address, hex"},
  "args":{"type":"string","description":"Comma separated r3..r10, parsed as hex"},
  "steps":{"type":"string","description":"Instruction budget. MUST be a string."},
  "writes":{"type":"string","description":"Memory to place before the run: \"addr:hexbytes\", several separated by ',' or ';'. Addresses and data are HEX. Unmapped/BSS addresses are writable and read back as 0 otherwise. Non-hex data is now an error rather than being silently skipped."},
  "stubs":{"type":"string","description":"addr=val functions to stub out"},
  "dumps":{"type":"string","description":"Memory ranges to dump after"},
  "trace":{"type":"string"},"trace_from":{"type":"string"},
  "trace_pc":{"type":"string","description":"lo-hi window"},
  "watch":{"type":"string","description":"e.g. r18"},
  "watch_mem":{"type":"string","description":"lo-hi"}}},
  "example":{"entry":"82558C90","args":"0,30000000","steps":"200000"}}}},"responses":{"200":{"description":"ok, error, r3, steps, last_pc, calls[], dumps, trace, watch, mem_watch"}}}},
)JSON"
R"JSON(
"/api/rename":{"post":{"tags":["edit"],"summary":"Rename or annotate one item","description":"kind selects what is changed. func/label/comment use addr+name; var uses addr+id+name; field uses addr+base+off+name; settype uses addr+base+type; sig uses addr+name; gstruct and gfield use addr+off+name.","requestBody":{"required":true,"content":{"application/json":{"schema":{"type":"object","properties":{
  "kind":{"type":"string","enum":["func","var","label","comment","field","gfield","gstruct","settype","sig"]},
  "addr":{"type":"string","description":"HEX"},
  "name":{"type":"string"},"id":{"type":"string"},
  "base":{"type":"string","description":"Owning struct"},
  "off":{"type":"string","description":"Field offset, HEX"},
  "type":{"type":"string"}}},
  "example":{"kind":"func","addr":"822F8F48","name":"ParsePlaylist"}}}},"responses":{"200":{"description":"{ok:true|false}"}}}},
"/api/batch":{"post":{"tags":["edit"],"summary":"Bulk rename","description":"One round trip for bulk work such as harvesting names out of a logger. Accepts the same kinds and fields as /api/rename.","requestBody":{"required":true,"content":{"application/json":{"schema":{"type":"array","items":{"type":"object","properties":{
  "kind":{"type":"string","enum":["func","var","label","comment","field","gfield","gstruct","settype","sig"]},
  "addr":{"type":"string"},"name":{"type":"string"},"id":{"type":"string"},
  "base":{"type":"string"},"off":{"type":"string"},"type":{"type":"string"}}}},
  "example":[{"kind":"func","addr":"82558C90","name":"atoi"},{"kind":"comment","addr":"82558CA4","name":"skips whitespace"}]}}},"responses":{"200":{"description":"{ok:<applied>,total:<seen>}"}}}}
}})JSON";

const char* kApiDocsHtml =
R"HTML(<!doctype html>
<html><head><meta charset="utf-8"><title>XEXMagic API</title>
<style>
:root{--bg:#1e1e1e;--panel:#252526;--edge:#333;--fg:#d4d4d4;--acc:#4ec9b0;--kw:#569cd6;--dim:#858585;--sel:#094771;--str:#ce9178;--cmt:#6a9955}
*{box-sizing:border-box}
body{margin:0;font:13px/1.6 Consolas,Menlo,monospace;background:var(--bg);color:var(--fg)}
header{padding:10px 16px;background:var(--panel);border-bottom:1px solid var(--edge);display:flex;gap:12px;align-items:baseline;position:sticky;top:0;z-index:10}
header b{color:var(--acc);font-size:15px}
header .v{color:var(--dim)}
header a{color:var(--kw);margin-left:auto;text-decoration:none}
header a:hover{text-decoration:underline}
#desc{padding:10px 16px;color:var(--cmt);border-bottom:1px solid var(--edge);white-space:pre-wrap}
#tags{max-width:1100px;margin:0 auto;padding:16px}
h2{color:var(--acc);font-size:14px;margin:22px 0 4px;border-bottom:1px solid var(--edge);padding-bottom:4px}
h2 span{color:var(--dim);font-weight:normal;margin-left:8px}
.op{border:1px solid var(--edge);margin:6px 0;background:var(--panel)}
.oph{display:flex;gap:10px;align-items:center;padding:7px 10px;cursor:pointer}
.oph:hover{background:var(--sel)}
.m{font-weight:bold;padding:2px 9px;border-radius:2px;color:#000;font-size:11px;min-width:46px;text-align:center}
.m.get{background:var(--acc)}
.m.post{background:#dcdcaa}
.p{color:var(--fg)}
.s{color:var(--dim);margin-left:auto;text-align:right}
.body{display:none;padding:10px 12px;border-top:1px solid var(--edge)}
.op.open .body{display:block}
.d{color:var(--cmt);white-space:pre-wrap;margin-bottom:10px}
table{border-collapse:collapse;width:100%;margin-bottom:10px}
th{text-align:left;color:var(--dim);font-weight:normal;border-bottom:1px solid var(--edge);padding:4px 6px}
td{padding:4px 6px;vertical-align:top;border-bottom:1px solid #2a2a2a}
td.n{color:var(--kw);white-space:nowrap}
td.t{color:var(--dim);white-space:nowrap}
.req{color:#f48771}
input,textarea{background:var(--bg);color:var(--fg);border:1px solid var(--edge);font:inherit;padding:4px;width:100%}
textarea{min-height:120px;white-space:pre;resize:vertical}
button{background:var(--panel);color:var(--fg);border:1px solid var(--edge);padding:5px 14px;cursor:pointer;font:inherit}
button:hover{background:var(--sel)}
button.go{border-color:var(--acc);color:var(--acc)}
.rw{margin-top:10px}
.st{color:var(--dim);margin-bottom:4px}
.st b.ok{color:var(--acc)}
.st b.err{color:#f48771}
pre{background:var(--bg);border:1px solid var(--edge);padding:8px;overflow:auto;max-height:420px;margin:0;white-space:pre-wrap;word-break:break-all}
</style></head><body>
<header><b>XEXMagic API</b><span class="v" id="ver"></span>
<a href="/api/openapi.json">openapi.json</a><a href="/">&larr; back to UI</a></header>
<div id="desc"></div>
<div id="tags">loading spec...</div>
<script>
const $=(t,c,x)=>{const e=document.createElement(t);if(c)e.className=c;if(x!=null)e.textContent=x;return e};

function paramRow(p){
  const tr=$('tr');
  const n=$('td','n');n.textContent=p.name;
  if(p.required)n.appendChild($('span','req',' *'));
  tr.appendChild(n);
  const sc=p.schema||{};
  tr.appendChild($('td','t',(sc.enum?sc.enum.join(' | '):sc.type||'string')));
  const d=$('td');d.textContent=p.description||'';
  if(sc.default!==undefined)d.appendChild($('div','t','default: '+sc.default));
  tr.appendChild(d);
  const v=$('td');
  const i=$('input');i.dataset.pname=p.name;i.placeholder=p.example!=null?String(p.example):'';
  if(p.example!=null)i.value=String(p.example);
  v.appendChild(i);tr.appendChild(v);
  return tr;
}

function render(spec){
  document.getElementById('ver').textContent='v'+spec.info.version;
  document.getElementById('desc').textContent=spec.info.description||'';
  const host=document.getElementById('tags');host.textContent='';
  const byTag={};
  for(const [path,ops] of Object.entries(spec.paths))
    for(const [method,op] of Object.entries(ops)){
      const t=(op.tags&&op.tags[0])||'other';
      (byTag[t]=byTag[t]||[]).push({path,method,op});
    }
  const order=(spec.tags||[]).map(t=>t.name);
  for(const k of Object.keys(byTag)) if(!order.includes(k)) order.push(k);
  for(const tag of order){
    if(!byTag[tag])continue;
    const meta=(spec.tags||[]).find(t=>t.name===tag);
    const h=$('h2',null,tag);
    if(meta&&meta.description)h.appendChild($('span',null,meta.description));
    host.appendChild(h);
    for(const e of byTag[tag]) host.appendChild(opCard(e));
  }
}
</script>
)HTML"
R"HTML(<script>
function opCard({path,method,op}){
  const card=$('div','op');
  const head=$('div','oph');
  head.appendChild($('span','m '+method,method.toUpperCase()));
  head.appendChild($('span','p',path));
  head.appendChild($('span','s',op.summary||''));
  head.onclick=()=>card.classList.toggle('open');
  card.appendChild(head);

  const body=$('div','body');
  if(op.description)body.appendChild($('div','d',op.description));

  const params=op.parameters||[];
  if(params.length){
    const tb=$('table');
    const hr=$('tr');
    for(const c of ['name','type','description','value'])hr.appendChild($('th',null,c));
    tb.appendChild(hr);
    for(const p of params)tb.appendChild(paramRow(p));
    body.appendChild(tb);
  }

  let ta=null;
  const rb=op.requestBody;
  if(rb){
    body.appendChild($('div','t','request body'+(rb.required?' (required)':'')));
    ta=$('textarea');
    const j=rb.content&&rb.content['application/json'];
    if(j&&j.example!==undefined)ta.value=JSON.stringify(j.example,null,2);
    else if(j&&j.schema)ta.value='{}';
    body.appendChild(ta);
  }

  const go=$('button','go','Execute');
  body.appendChild($('div','rw')).appendChild(go);
  const st=$('div','st');const out=$('pre');
  out.style.display='none';st.style.display='none';
  body.appendChild(st);body.appendChild(out);

  go.onclick=async()=>{
    let url=path;const qs=[];
    for(const i of body.querySelectorAll('input[data-pname]'))
      if(i.value!=='')qs.push(encodeURIComponent(i.dataset.pname)+'='+encodeURIComponent(i.value));
    if(qs.length)url+='?'+qs.join('&');
    const opt={method:method.toUpperCase()};
    if(ta&&ta.value.trim()){opt.headers={'Content-Type':'application/json'};opt.body=ta.value;}
    st.style.display='';out.style.display='';
    st.textContent='requesting...';out.textContent='';
    const t0=performance.now();
    try{
      const r=await fetch(url,opt);
      const txt=await r.text();
      const ms=Math.round(performance.now()-t0);
      st.textContent='';
      st.appendChild($('b',r.ok?'ok':'err',r.status+' '+r.statusText));
      st.appendChild(document.createTextNode('  '+ms+' ms  '+txt.length.toLocaleString()+' bytes'));
      // Pretty-print when it parses; /api/export is plain text and will not.
      try{out.textContent=JSON.stringify(JSON.parse(txt),null,2);}
      catch(e){out.textContent=txt;}
    }catch(e){
      st.textContent='';
      st.appendChild($('b','err','request failed'));
      out.textContent=String(e);
    }
  };
  card.appendChild(body);
  return card;
}

fetch('/api/openapi.json').then(r=>r.json()).then(render).catch(e=>{
  document.getElementById('tags').textContent='could not load /api/openapi.json: '+e;
});
</script>
</body></html>)HTML";

// ---------------------------------------------------------------------------
// HTTP server
// ---------------------------------------------------------------------------

std::string Recv(SOCKET s, std::string* body_out) {
  std::string data;
  char buf[4096];
  size_t header_end = std::string::npos;
  // Read until end of headers.
  while (header_end == std::string::npos) {
    int n = recv(s, buf, sizeof(buf), 0);
    if (n <= 0) return data;
    data.append(buf, n);
    header_end = data.find("\r\n\r\n");
    if (data.size() > (1u << 20)) break;
  }
  // Honor Content-Length for the body.
  size_t clp = data.find("Content-Length:");
  if (clp == std::string::npos) clp = data.find("content-length:");
  if (clp != std::string::npos && header_end != std::string::npos) {
    size_t vs = clp + 15;
    size_t ve = data.find("\r\n", vs);
    long clen = strtol(data.substr(vs, ve - vs).c_str(), nullptr, 10);
    size_t have = data.size() - (header_end + 4);
    while ((long)have < clen) {
      int n = recv(s, buf, sizeof(buf), 0);
      if (n <= 0) break;
      data.append(buf, n);
      have = data.size() - (header_end + 4);
    }
    if (body_out) *body_out = data.substr(header_end + 4, clen);
  }
  return data;
}

void Send(SOCKET s, const std::string& status, const std::string& ctype,
          const std::string& body) {
  std::ostringstream o;
  o << "HTTP/1.1 " << status << "\r\n"
    << "Content-Type: " << ctype << "\r\n"
    << "Content-Length: " << body.size() << "\r\n"
    << "Cache-Control: no-store\r\n"
    << "Connection: close\r\n\r\n"
    << body;
  std::string r = o.str();
  send(s, r.data(), (int)r.size(), 0);
}

// A whole-image export runs to tens of megabytes, which one send() will not
// take, so this one loops until the body is out.
void SendDownload(SOCKET s, const std::string& ctype,
                  const std::string& filename, const std::string& body) {
  std::ostringstream o;
  o << "HTTP/1.1 200 OK\r\n"
    << "Content-Type: " << ctype << "\r\n"
    << "Content-Length: " << body.size() << "\r\n"
    << "Content-Disposition: attachment; filename=\"" << filename << "\"\r\n"
    << "Cache-Control: no-store\r\n"
    << "Connection: close\r\n\r\n";
  std::string head = o.str();
  const std::string* parts[2] = {&head, &body};
  for (const std::string* p : parts) {
    size_t off = 0;
    while (off < p->size()) {
      size_t chunk = p->size() - off;
      if (chunk > (size_t)64 * 1024) chunk = 64 * 1024;
      int n = send(s, p->data() + off, (int)chunk, 0);
      if (n <= 0) return;
      off += (size_t)n;
    }
  }
}

std::string QueryParam(const std::string& path, const std::string& key) {
  size_t q = path.find('?');
  if (q == std::string::npos) return "";
  std::string qs = path.substr(q + 1);
  std::string needle = key + "=";
  size_t p = qs.find(needle);
  if (p == std::string::npos) return "";
  p += needle.size();
  size_t e = qs.find('&', p);
  return qs.substr(p, e == std::string::npos ? std::string::npos : e - p);
}

}  // namespace

// ---------------------------------------------------------------------------
// Server plumbing
//
// The listener comes up before the image has been analysed, so a browser
// pointed at the port during a long load gets a progress page instead of a
// refused connection. Requests are handled on their own thread and everything
// that touches the model takes one lock; the progress endpoints do not, so
// they still answer while a long task holds it.
// ---------------------------------------------------------------------------

namespace {

std::string UrlDecode(const std::string& s) {
  std::string o;
  for (size_t i = 0; i < s.size(); i++) {
    if (s[i] == '+') {
      o += ' ';
    } else if (s[i] == '%' && i + 2 < s.size()) {
      o += (char)strtoul(s.substr(i + 1, 2).c_str(), nullptr, 16);
      i += 2;
    } else {
      o += s[i];
    }
  }
  return o;
}

std::string HtmlEscape(const std::string& s) {
  std::string o;
  for (char c : s) {
    if (c == '&') o += "&amp;";
    else if (c == '<') o += "&lt;";
    else if (c == '>') o += "&gt;";
    else o += c;
  }
  return o;
}

std::timed_mutex g_model_mu;
Model* g_model = nullptr;
SOCKET g_srv = INVALID_SOCKET;
int g_port = 0;
// --serve with no file: the UI offers a drop target and nothing is loaded
// until the user provides one.
std::atomic<bool> g_awaiting_file{false};
// First run: no indexed SDK libraries yet. Setup comes before anything else,
// because the index is what puts real names on library code and a scan that
// arrives after an image was analysed cannot reach the propagation that
// spreads those names to callers.
std::atomic<bool> g_setup_mode{false};

// The setup page. Takes the SDK folder, indexes it, and gets out of the way.
std::string SetupHtml() {
  auto& sdk = xexmagic::SdkConfig::Get();
  std::string path = sdk.sdk_path();
  if (path.empty()) path = "C:\\Program Files (x86)\\Microsoft Xbox 360 SDK";
  std::ostringstream o;
  o << "<!doctype html><meta charset=\"utf-8\"><title>XEXMagic setup</title>"
       "<style>body{background:#1e1e1e;color:#d4d4d4;font:13px Consolas,"
       "monospace;margin:0;min-height:100vh;display:flex;align-items:center;"
       "justify-content:center}#b{max-width:640px;padding:40px}"
       "h1{font-size:16px;color:#4ec9b0;margin:0 0 6px}"
       ".n{color:#858585;margin:10px 0}"
       "input{background:#1e1e1e;border:1px solid #3e3e42;color:#d4d4d4;"
       "font:inherit;padding:6px;width:100%;box-sizing:border-box;margin:8px 0}"
       "button{background:#2d2d30;border:1px solid #3e3e42;color:#d4d4d4;"
       "font:inherit;padding:6px 14px;cursor:pointer}"
       "button:disabled{opacity:.5;cursor:default}"
       ".err{color:#e66;margin-top:8px}.ok{color:#4ec9b0}"
       ".bar{height:14px;background:#2d2d30;border:1px solid #3e3e42;"
       "margin-top:14px}.fill{height:100%;background:#569cd6;width:25%;"
       "animation:sweep 1.6s ease-in-out infinite alternate}"
       "@keyframes sweep{0%{margin-left:0}100%{margin-left:75%}}</style>"
       "<div id=\"b\"><h1>XEXMagic setup</h1>"
       "<div class=\"n\">Point XEXMagic at an installed Xbox 360 SDK. It is "
       "indexed once and kept, so this only happens on a first run.</div>"
       "<div class=\"n\">The folder holding <b>include\\</b> and <b>lib\\</b>:"
       "</div><input id=\"p\" value=\"" << HtmlEscape(path) << "\">"
       "<button id=\"go\">Index the SDK</button> "
       "<button id=\"skip\">Skip</button>"
       "<div class=\"n\" id=\"s\"></div><div class=\"err\" id=\"e\"></div>"
       "<div id=\"bar\" style=\"display:none\" class=\"bar\">"
       "<div class=\"fill\"></div></div>"
       "<div class=\"n\">Skipping is fine - the tool works without it, but "
       "library code keeps its <b>sub_82xxxxxx</b> names.</div></div>"
       "<script>"
       "const $=i=>document.getElementById(i);"
       "async function poll(){const p=await (await fetch('/api/progress'))"
       ".json();if(p.phase)$('s').textContent=p.phase+' - '+p.phase_seconds+'s';"
       "}"
       "$('go').onclick=async()=>{"
       "$('e').textContent='';$('go').disabled=true;$('skip').disabled=true;"
       "const r=await (await fetch('/api/sdk',{method:'POST',"
       "headers:{'Content-Type':'application/json'},"
       "body:JSON.stringify({path:$('p').value})})).json();"
       "if(r.error){$('e').textContent=r.error;$('go').disabled=false;"
       "$('skip').disabled=false;return;}"
       "$('bar').style.display='block';$('s').textContent='indexing...';"
       "const t=setInterval(poll,1000);"
       "const x=await (await fetch('/api/sdk/index',{method:'POST'})).json();"
       "clearInterval(t);$('bar').style.display='none';"
       "$('s').innerHTML='<span class=\"ok\">indexed '+x.structs+' types, '+"
       "x.symbols+' library symbols from '+x.libs+' libs</span>';"
       "await fetch('/api/setup/done',{method:'POST'});"
       "setTimeout(()=>location.href='/',700);};"
       "$('skip').onclick=async()=>{await fetch('/api/setup/done',"
       "{method:'POST'});location.href='/';};"
       "</script>";
  return o.str();
}

// Relaunches this executable against `path` and exits. The whole load path -
// XEX decryption, section walking, COFF layout - lives in main() and depends
// on locals built there; re-entering through main is what makes an arbitrary
// dropped file work without a second copy of that logic.
void RestartWithFile(const std::string& path) {
#ifdef _WIN32
  char exe[MAX_PATH];
  DWORD n = GetModuleFileNameA(nullptr, exe, MAX_PATH);
  if (!n || n >= MAX_PATH) return;
  std::ostringstream cmd;
  cmd << "\"" << exe << "\" --serve --port " << g_port << " \"" << path << "\"";
  std::string line = cmd.str();
  // Drop the listener first so the child can bind the same port.
  if (g_srv != INVALID_SOCKET) {
    closesocket(g_srv);
    g_srv = INVALID_SOCKET;
  }
  STARTUPINFOA si{};
  si.cb = sizeof(si);
  PROCESS_INFORMATION pi{};
  if (CreateProcessA(nullptr, &line[0], nullptr, nullptr, FALSE, 0, nullptr,
                     nullptr, &si, &pi)) {
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    printf("opening %s\n", path.c_str());
    fflush(stdout);
    exit(0);
  }
  fprintf(stderr, "Failed to relaunch for %s\n", path.c_str());
#endif
}

// The page shown when --serve was given no file.
std::string DropHtml() {
  return
      "<!doctype html><meta charset=\"utf-8\"><title>XEXMagic</title>"
      "<style>body{background:#1e1e1e;color:#d4d4d4;font:13px Consolas,"
      "monospace;margin:0;height:100vh;display:flex;align-items:center;"
      "justify-content:center}#d{border:2px dashed #3e3e42;border-radius:6px;"
      "padding:60px 80px;text-align:center;transition:.15s}"
      "#d.over{border-color:#4ec9b0;background:#252526}"
      "h1{font-size:16px;color:#4ec9b0;margin:0 0 14px}"
      ".n{color:#858585;margin-top:10px}"
      "input{background:#1e1e1e;border:1px solid #3e3e42;color:#d4d4d4;"
      "font:inherit;padding:6px;width:420px;max-width:100%;margin-top:18px}"
      "</style>"
      "<div id=\"d\"><h1>Drop a XEX, EXE or LIB here</h1>"
      "<div class=\"n\">or type a path and press Enter</div>"
      "<input id=\"p\" placeholder=\"C:\\\\path\\\\to\\\\default.xex\" "
      "autofocus>"
      "<div class=\"n\" id=\"s\"></div></div>"
      "<script>"
      "const d=document.getElementById('d'),s=document.getElementById('s');"
      "function waitForLoad(){s.textContent='Loading - this page will follow "
      "the progress.';setTimeout(()=>location.reload(),3000);}"
      "async function openPath(p){s.textContent='Opening '+p+' ...';"
      "await fetch('/api/open',{method:'POST',body:JSON.stringify({path:p})});"
      "waitForLoad();}"
      "async function upload(f){s.textContent='Uploading '+f.name+' ('+"
      "Math.round(f.size/1048576)+' MB) ...';"
      "await fetch('/api/open?name='+encodeURIComponent(f.name),"
      "{method:'POST',body:f});waitForLoad();}"
      "d.addEventListener('dragover',e=>{e.preventDefault();"
      "d.classList.add('over');});"
      "d.addEventListener('dragleave',()=>d.classList.remove('over'));"
      "document.addEventListener('dragover',e=>e.preventDefault());"
      "document.addEventListener('drop',e=>e.preventDefault());"
      "d.addEventListener('drop',e=>{e.preventDefault();"
      "d.classList.remove('over');const f=e.dataTransfer.files[0];"
      "if(f)upload(f);});"
      "document.getElementById('p').addEventListener('keydown',e=>{"
      "if(e.key==='Enter'&&e.target.value.trim())openPath(e.target.value.trim())"
      ";});"
      "</script>";
}

std::string ProgressJson() {
  auto s = xexmagic::progress::Get();
  std::ostringstream o;
  o << "{\"ready\":" << (s.ready ? "true" : "false") << ",\"phase\":\""
    << JsonEscape(s.phase) << "\",\"done\":" << s.done
    << ",\"total\":" << s.total << ",\"percent\":";
  if (s.total)
    o << (int)((s.done * 100) / s.total);
  else
    o << -1;
  o << ",\"phase_seconds\":" << s.phase_seconds << ",\"total_seconds\":"
    << s.total_seconds << ",\"current\":[";
  for (size_t i = 0; i < s.current.size(); i++) {
    if (i) o << ",";
    o << "\"" << JsonEscape(s.current[i]) << "\"";
  }
  o << "]}";
  return o.str();
}

// The page shown while the model is being built. Refreshes itself every 5
// seconds; the same markup renders whatever phase is running.
std::string ProgressHtml() {
  auto s = xexmagic::progress::Get();
  std::ostringstream o;
  o << "<!doctype html><meta charset=\"utf-8\">"
       "<meta http-equiv=\"refresh\" content=\"5\">"
       "<title>XEXMagic - working</title>"
       "<style>body{background:#1e1e1e;color:#d4d4d4;font:13px Consolas,"
       "monospace;margin:0;padding:40px}h1{font-size:15px;color:#569cd6;"
       "margin:0 0 18px}.bar{height:14px;background:#2d2d30;border:1px solid "
       "#3e3e42;width:520px;max-width:100%}.fill{height:100%;background:#569cd6"
       "}.n{color:#808080;margin:8px 0 20px}h2{font-size:12px;color:#808080;"
       "text-transform:uppercase;margin:0 0 6px}li{list-style:none;color:#ce9178"
       ";padding:1px 0}ul{margin:0;padding:0}"
       "@keyframes sweep{0%{margin-left:0}100%{margin-left:75%}}"
       ".fill.ind{width:25%;animation:sweep 1.6s ease-in-out infinite alternate}"
       "</style>";
  o << "<h1>XEXMagic is working on " << HtmlEscape(s.phase.empty()
                                                       ? std::string("startup")
                                                       : s.phase)
    << "</h1>";
  int pct = s.total ? (int)((s.done * 100) / s.total) : -1;
  if (pct >= 0) {
    o << "<div class=\"bar\"><div class=\"fill\" style=\"width:" << pct
      << "%\"></div></div><div class=\"n\">" << pct << "% - " << s.done
      << " of " << s.total << ", " << s.phase_seconds << "s in this phase, "
      << s.total_seconds << "s total</div>";
  } else {
    // No item count for this phase - show that it is running rather than a
    // zero, which reads as stuck.
    o << "<div class=\"bar\"><div class=\"fill ind\"></div></div>"
         "<div class=\"n\">working, "
      << s.phase_seconds << "s in this phase, " << s.total_seconds
      << "s total</div>";
  }
  if (!s.current.empty()) {
    o << "<h2>Decompiling now</h2><ul>";
    for (const auto& cur : s.current) o << "<li>" << HtmlEscape(cur) << "</li>";
    o << "</ul>";
  }
  o << "<div class=\"n\" style=\"margin-top:24px\">This page refreshes every 5"
       " seconds.</div>";
  return o.str();
}

void HandleConn(Model& model, SOCKET c, const std::string& method,
                const std::string& path, const std::string& bare,
                const std::string& body) {
  {
    if (method == "GET" && bare == "/") {
      Send(c, "200 OK", "text/html; charset=utf-8", kIndexHtml);
    } else if (method == "GET" &&
               (bare == "/api" || bare == "/api/" || bare == "/api/docs")) {
      Send(c, "200 OK", "text/html; charset=utf-8", kApiDocsHtml);
    } else if (method == "GET" && bare == "/api/openapi.json") {
      Send(c, "200 OK", "application/json", kOpenApiJson);
    } else if (method == "GET" && bare == "/api/meta") {
      Send(c, "200 OK", "application/json", model.MetaJson());
    } else if (method == "GET" && bare == "/api/mem") {
      uint32_t a = (uint32_t)strtoul(QueryParam(path, "addr").c_str(), nullptr, 16);
      uint32_t n = (uint32_t)strtoul(QueryParam(path, "len").c_str(), nullptr, 16);
      Send(c, "200 OK", "application/json", model.MemJson(a, n));
    } else if (method == "GET" && bare == "/api/tables") {
      uint32_t a = (uint32_t)strtoul(QueryParam(path, "addr").c_str(), nullptr, 16);
      uint32_t n = (uint32_t)strtoul(QueryParam(path, "len").c_str(), nullptr, 16);
      uint32_t st = (uint32_t)strtoul(QueryParam(path, "stride").c_str(), nullptr, 10);
      Send(c, "200 OK", "application/json", model.TablesJson(a, n, st));
    } else if (method == "GET" && bare == "/api/callgraph") {
      uint32_t a = (uint32_t)strtoul(QueryParam(path, "addr").c_str(), nullptr, 16);
      int d = (int)strtoul(QueryParam(path, "depth").c_str(), nullptr, 10);
      bool up = QueryParam(path, "dir") != "down";
      Send(c, "200 OK", "application/json", model.CallGraphJson(a, d ? d : 3, up));
    } else if (method == "GET" && bare == "/api/dataxref") {
      uint32_t a = (uint32_t)strtoul(QueryParam(path, "addr").c_str(), nullptr, 16);
      Send(c, "200 OK", "application/json", model.DataXrefJson(a));
    } else if (method == "GET" && bare == "/api/export") {
      Send(c, "200 OK", "text/plain; charset=utf-8", model.ExportHeader());
    } else if (method == "GET" && bare == "/api/sections") {
      Send(c, "200 OK", "application/json", model.SectionsJson());
    } else if (method == "GET" && bare == "/api/globals") {
      Send(c, "200 OK", "application/json", model.GlobalsJson());
    } else if (method == "GET" &&
               (bare == "/api/strings" || bare == "/api/search")) {
      std::string q = QueryParam(path, "q");
      // Query values arrive percent-encoded.
      std::string dec;
      for (size_t i = 0; i < q.size(); i++) {
        if (q[i] == '+') dec += ' ';
        else if (q[i] == '%' && i + 2 < q.size()) {
          dec += (char)strtoul(q.substr(i + 1, 2).c_str(), nullptr, 16);
          i += 2;
        } else dec += q[i];
      }
      Send(c, "200 OK", "application/json",
           bare == "/api/search" ? model.SearchJson(dec, 300)
                                 : model.StringsJson(dec, 300));
    } else if (method == "GET" && bare == "/api/functions") {
      Send(c, "200 OK", "application/json", model.FunctionsJson());
    } else if (method == "GET" && bare == "/api/func") {
      uint32_t a = (uint32_t)strtoul(QueryParam(path, "addr").c_str(), nullptr, 16);
      Send(c, "200 OK", "application/json", model.FuncJson(a));
    } else if (method == "GET" && bare == "/api/export/c") {
      std::string out = model.ExportAllC();
      SendDownload(c, "text/plain; charset=utf-8", model.ExportFileName(), out);
    } else if (method == "GET" && bare == "/api/sdk") {
      Send(c, "200 OK", "application/json",
           xexmagic::SdkConfig::Get().ConfigJson());
    } else if (method == "POST" && bare == "/api/sdk") {
      // {"path":"D:\\XDK"} - stores and validates. Empty path clears it.
      std::string p;
      JsonGetStr(body, "path", &p);
      std::string err;
      auto& sdk = xexmagic::SdkConfig::Get();
      if (sdk.SetSdkPath(p, &err)) {
        Send(c, "200 OK", "application/json", sdk.ConfigJson());
      } else {
        Send(c, "200 OK", "application/json",
             std::string("{\"error\":\"") + JsonEscape(err) + "\"}");
      }
    } else if (method == "POST" && bare == "/api/sdk/index") {
      auto& sdk = xexmagic::SdkConfig::Get();
      xexmagic::progress::BeginPhase("indexing the SDK");
      sdk.BuildIndex();
      xexmagic::progress::BeginPhase("");
      Send(c, "200 OK", "application/json", sdk.ConfigJson());
    } else if (method == "GET" && bare == "/api/sdk/structs") {
      auto& sdk = xexmagic::SdkConfig::Get();
      const std::string q = QueryParam(path, "q");
      std::ostringstream o;
      o << "[";
      bool first = true;
      for (const auto* s : sdk.SearchStructs(q, 200)) {
        if (!first) o << ",";
        first = false;
        o << sdk.StructJson(*s);
      }
      o << "]";
      Send(c, "200 OK", "application/json", o.str());
    } else if (method == "POST" && bare == "/api/sdk/inlined") {
      Send(c, "200 OK", "application/json", model.SdkInlineJson());
    } else if (method == "POST" && bare == "/api/sdk/scan") {
      // ?apply=1 renames; without it the matches come back for review.
      Send(c, "200 OK", "application/json",
           model.SdkScanJson(QueryParam(path, "apply") == "1"));
    } else if (method == "GET" && bare == "/api/import/xrefs") {
      uint32_t a = (uint32_t)strtoul(QueryParam(path, "addr").c_str(), nullptr, 16);
      Send(c, "200 OK", "application/json", model.ImportXrefJson(a));
    } else if (method == "GET" && bare == "/api/imports") {
      Send(c, "200 OK", "application/json",
           model.ImportsJson(UrlDecode(QueryParam(path, "q")), 300));
    } else if (method == "GET" && bare == "/api/sdk/symbols") {
      // Functions the SDK libs define. Used to put a real name on code that
      // came from, or calls into, a shipped library.
      auto& sdk = xexmagic::SdkConfig::Get();
      const std::string q = QueryParam(path, "q");
      std::ostringstream o;
      o << "[";
      bool first = true;
      for (const auto* s : sdk.SearchSymbols(q, 300)) {
        if (!first) o << ",";
        first = false;
        o << "{\"name\":\"" << JsonEscape(s->name) << "\",\"demangled\":\""
          << JsonEscape(s->demangled) << "\",\"lib\":\"" << JsonEscape(s->lib)
          << "\",\"obj\":\"" << JsonEscape(s->obj)
          << "\",\"code_size\":" << s->code_size << "}";
      }
      o << "]";
      Send(c, "200 OK", "application/json", o.str());
    } else if (method == "GET" && bare == "/api/sdk/match") {
      // size=<dec>&offsets=0,4,12 - candidate SDK types for an inferred layout.
      auto& sdk = xexmagic::SdkConfig::Get();
      uint32_t size =
          (uint32_t)strtoul(QueryParam(path, "size").c_str(), nullptr, 0);
      std::vector<uint32_t> offsets;
      const std::string list = QueryParam(path, "offsets");
      size_t pos = 0;
      while (pos < list.size()) {
        size_t comma = list.find(',', pos);
        std::string one =
            list.substr(pos, comma == std::string::npos ? comma : comma - pos);
        if (!one.empty()) {
          offsets.push_back((uint32_t)strtoul(one.c_str(), nullptr, 0));
        }
        if (comma == std::string::npos) break;
        pos = comma + 1;
      }
      std::ostringstream o;
      o << "[";
      bool first = true;
      for (const auto& cand : sdk.MatchLayout(size, offsets, 25)) {
        if (!first) o << ",";
        first = false;
        o << "{\"score\":" << cand.score << ",\"why\":\"" << JsonEscape(cand.why)
          << "\",\"def\":" << sdk.StructJson(*cand.def) << "}";
      }
      o << "]";
      Send(c, "200 OK", "application/json", o.str());
    } else if (method == "POST" && bare == "/api/run") {
      Send(c, "200 OK", "application/json", model.RunJson(body));
    } else if (method == "POST" && bare == "/api/batch") {
      // [{"kind":"func","addr":"...","name":"..."}, ...] — one round trip for
      // bulk work like harvesting names out of a logger.
      int ok = 0, total = 0;
      size_t p = 0;
      while ((p = body.find('{', p)) != std::string::npos) {
        size_t e = body.find('}', p);
        if (e == std::string::npos) break;
        std::string one = body.substr(p, e - p + 1);
        p = e + 1;
        total++;
        std::string kind, addrs, name, id, base, offs, type;
        JsonGetStr(one, "kind", &kind);
        JsonGetStr(one, "addr", &addrs);
        JsonGetStr(one, "name", &name);
        JsonGetStr(one, "id", &id);
        JsonGetStr(one, "base", &base);
        JsonGetStr(one, "off", &offs);
        JsonGetStr(one, "type", &type);
        uint32_t a = (uint32_t)strtoul(addrs.c_str(), nullptr, 16);
        uint32_t off = (uint32_t)strtoul(offs.c_str(), nullptr, 16);
        bool r = false;
        if (kind == "func") r = model.RenameFunc(a, name);
        else if (kind == "var") r = model.RenameVar(a, id, name);
        else if (kind == "label") r = model.RenameLabel(a, id, name);
        else if (kind == "comment") r = model.SetComment(a, name);
        else if (kind == "field") r = model.RenameField(a, base, off, name);
        else if (kind == "gfield") r = model.RenameGlobalField(a, off, name);
        else if (kind == "settype") r = model.SetType(a, base, type);
        else if (kind == "sig") r = model.SetSignature_(a, name);
        else if (kind == "gstruct") r = model.BindGlobal(a, off, name);
        if (r) ok++;
      }
      std::ostringstream o;
      o << "{\"ok\":" << ok << ",\"total\":" << total << "}";
      Send(c, "200 OK", "application/json", o.str());
    } else if (method == "POST" && bare == "/api/rename") {
      std::string kind, addrs, name, id, base, offs, type;
      JsonGetStr(body, "kind", &kind);
      JsonGetStr(body, "addr", &addrs);
      JsonGetStr(body, "name", &name);
      JsonGetStr(body, "id", &id);
      JsonGetStr(body, "base", &base);
      JsonGetStr(body, "off", &offs);
      JsonGetStr(body, "type", &type);
      uint32_t a = (uint32_t)strtoul(addrs.c_str(), nullptr, 16);
      bool ok = false;
      if (kind == "func") ok = model.RenameFunc(a, name);
      else if (kind == "var") ok = model.RenameVar(a, id, name);
      else if (kind == "field")
        ok = model.RenameField(a, base,
                               (uint32_t)strtoul(offs.c_str(), nullptr, 16),
                               name);
      else if (kind == "settype") ok = model.SetType(a, base, type);
      else if (kind == "label") ok = model.RenameLabel(a, id, name);
      else if (kind == "comment") ok = model.SetComment(a, name);
      else if (kind == "sig") ok = model.SetSignature_(a, name);
      else if (kind == "gstruct")
        ok = model.BindGlobal(a, (uint32_t)strtoul(offs.c_str(), nullptr, 16),
                              name);
      else if (kind == "gfield")
        ok = model.RenameGlobalField(
            a, (uint32_t)strtoul(offs.c_str(), nullptr, 16), name);
      Send(c, "200 OK", "application/json",
           ok ? "{\"ok\":true}" : "{\"ok\":false}");
    } else {
      Send(c, "404 Not Found", "text/plain", "not found");
    }
  }
}

// One connection: parse the request line, answer progress without touching the
// model, and serialise everything else behind the model lock.
void ServeConn(SOCKET c) {
  std::string body;
  std::string req = Recv(c, &body);
  size_t sp1 = req.find(' ');
  size_t sp2 = req.find(' ', sp1 + 1);
  std::string method = sp1 != std::string::npos ? req.substr(0, sp1) : "";
  std::string path = (sp1 != std::string::npos && sp2 != std::string::npos)
                         ? req.substr(sp1 + 1, sp2 - sp1 - 1)
                         : "/";
  std::string bare = path.substr(0, path.find('?'));

  if (method == "GET" && bare == "/api/progress") {
    Send(c, "200 OK", "application/json", ProgressJson());
    closesocket(c);
    return;
  }

  // Setup comes first. Until it is done nothing else is served, so a file
  // cannot be opened before the SDK index that names its library code exists.
  if (g_setup_mode.load()) {
    if (method == "POST" && bare == "/api/setup/done") {
      g_setup_mode = false;
      Send(c, "200 OK", "application/json", "{\"ok\":true}");
      closesocket(c);
      return;
    }
    // Handled here rather than by falling through: there is no model during
    // setup, and the dispatch below turns every /api request into a 503 when
    // the model is missing - which silently swallowed these and let setup
    // "finish" without indexing anything.
    auto& sdk = xexmagic::SdkConfig::Get();
    if (method == "GET" && bare == "/api/sdk") {
      Send(c, "200 OK", "application/json", sdk.ConfigJson());
      closesocket(c);
      return;
    }
    if (method == "POST" && bare == "/api/sdk") {
      std::string p;
      JsonGetStr(body, "path", &p);
      std::string err;
      if (sdk.SetSdkPath(p, &err)) {
        Send(c, "200 OK", "application/json", sdk.ConfigJson());
      } else {
        Send(c, "200 OK", "application/json",
             std::string("{\"error\":\"") + JsonEscape(err) + "\"}");
      }
      closesocket(c);
      return;
    }
    if (method == "POST" && bare == "/api/sdk/index") {
      xexmagic::progress::BeginPhase("indexing the SDK");
      sdk.BuildIndex();
      xexmagic::progress::BeginPhase("");
      printf("SDK: %zu structs from %zu headers, %zu prototypes, %zu symbols "
             "from %zu libs\n", sdk.struct_count(), sdk.header_count(),
             sdk.prototype_count(), sdk.symbol_count(), sdk.lib_count());
      fflush(stdout);
      Send(c, "200 OK", "application/json", sdk.ConfigJson());
      closesocket(c);
      return;
    }

    if (method == "GET" && bare.rfind("/api", 0) != 0) {
      Send(c, "200 OK", "text/html; charset=utf-8", SetupHtml());
    } else {
      Send(c, "503 Service Unavailable", "application/json",
           "{\"setup\":true}");
    }
    closesocket(c);
    return;
  }

  // No file was given on the command line: offer the drop target, and take
  // either a local path or an uploaded file.
  if (g_awaiting_file.load()) {
    if (method == "POST" && bare == "/api/open") {
      std::string open_path;
      std::string name = UrlDecode(QueryParam(path, "name"));
      if (!name.empty()) {
        // An upload. The browser gives bytes and a filename, and a filename
        // is not an identity - every title ships a default.xex. Store under
        // the content hash, one directory each, so two different files with
        // the same name cannot overwrite each other's database.
        std::string hash = HashHex(HashBytes(body.data(), body.size()));
        std::string ext;
        size_t dot = name.find_last_of('.');
        if (dot != std::string::npos && name.size() - dot <= 8)
          ext = name.substr(dot);
        std::string dir = xexmagic::AppSubDir("images") + hash;
        xexmagic::EnsureDir(dir);
        dir += kSep;
        open_path = dir + hash + ext;

        FILE* fp = fopen(open_path.c_str(), "wb");
        if (!fp) {
          Send(c, "500 Internal Server Error", "application/json",
               "{\"error\":\"could not write the uploaded file\"}");
          closesocket(c);
          return;
        }
        fwrite(body.data(), 1, body.size(), fp);
        fclose(fp);
        // The name it arrived under is worth keeping - the hash identifies
        // the file but says nothing about what it is.
        if (FILE* meta = fopen((dir + "origin.txt").c_str(), "wb")) {
          fprintf(meta, "%s\n", name.c_str());
          fclose(meta);
        }
        printf("received %s as %s (%zu bytes)\n", name.c_str(),
               open_path.c_str(), body.size());
        fflush(stdout);
      } else {
        JsonGetStr(body, "path", &open_path);
      }
      if (open_path.empty()) {
        Send(c, "400 Bad Request", "application/json",
             "{\"error\":\"no file\"}");
        closesocket(c);
        return;
      }
      Send(c, "200 OK", "application/json", "{\"ok\":true}");
      closesocket(c);
      g_awaiting_file = false;
      RestartWithFile(open_path);
      return;
    }
    if (method == "GET" && bare.rfind("/api", 0) != 0) {
      Send(c, "200 OK", "text/html; charset=utf-8", DropHtml());
    } else {
      Send(c, "503 Service Unavailable", "application/json",
           "{\"awaiting_file\":true}");
    }
    closesocket(c);
    return;
  }

  // No model yet, or a long task is holding it: those get the progress page
  // rather than a stall. Ordinary contention is not that - the UI fires
  // several requests at once and try_lock alone rejected whichever lost the
  // race, which showed up as sections silently missing from the results. Wait
  // briefly first, and only fall through when something really is camped on
  // the model.
  std::unique_lock<std::timed_mutex> lk(g_model_mu, std::defer_lock);
  bool got = lk.try_lock_for(std::chrono::seconds(20));
  if (!g_model || !got) {
    if (method == "GET" && (bare == "/" || bare.rfind("/api", 0) != 0)) {
      Send(c, "200 OK", "text/html; charset=utf-8", ProgressHtml());
    } else {
      Send(c, "503 Service Unavailable", "application/json",
           std::string("{\"busy\":true,\"progress\":") + ProgressJson() + "}");
    }
    closesocket(c);
    return;
  }

  HandleConn(*g_model, c, method, path, bare, body);
  closesocket(c);
}

void AcceptLoop() {
  for (;;) {
    SOCKET c = accept(g_srv, nullptr, nullptr);
    if (c == INVALID_SOCKET) continue;
    std::thread(ServeConn, c).detach();
  }
}

}  // namespace

static bool g_no_browser_launch = false;
int StartWebServer(int port) {
  // Already listening: --serve brings the port up before analysis and the
  // paths that finish with RunWebServer come back through here.
  if (g_srv != INVALID_SOCKET) return 0;

  WSADATA wsa;
  if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
    fprintf(stderr, "WSAStartup failed\n");
    return 1;
  }

  g_srv = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (g_srv == INVALID_SOCKET) {
    fprintf(stderr, "socket() failed\n");
    return 1;
  }
  BOOL yes = TRUE;
  setsockopt(g_srv, SOL_SOCKET, SO_REUSEADDR, (const char*)&yes, sizeof(yes));

  sockaddr_in addr{};
  addr.sin_family = AF_INET;
  addr.sin_port = htons((u_short)port);
  inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
  if (bind(g_srv, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
    fprintf(stderr, "bind() to port %d failed (in use?)\n", port);
    return 1;
  }
  listen(g_srv, 16);
  g_port = port;
  std::thread(AcceptLoop).detach();
  char url[64];
  snprintf(url, sizeof(url), "http://127.0.0.1:%d/", port);
  printf("XEXMagic UI: %s  (analysing; the page will show progress)\n", url);
  fflush(stdout);
#ifdef _WIN32
  // Open it in whatever the user's default browser is. The empty quoted
  // argument is the window title `start` otherwise takes the URL for.
  //
  // Suppressed by --no-browser-launch. Scripted use restarts this process after
  // every rebuild, and a browser window per restart piles up fast.
  if (!g_no_browser_launch) {
    std::string cmd = std::string("start \"\" \"") + url + "\"";
    system(cmd.c_str());
  }
#endif
  return 0;
}

void SetNoBrowserLaunch(bool v) { g_no_browser_launch = v; }

void SetWebProgram(WebProgram program) {
  // Index the SDK before the model is built: prototypes are applied while the
  // whole-program pass runs, and a signature that arrives afterwards would not
  // reach the propagation that spreads it to callers.
  {
    auto& sdk = xexmagic::SdkConfig::Get();
    if (!sdk.sdk_path().empty() && !sdk.indexed()) {
      xexmagic::progress::BeginPhase("indexing the SDK headers");
      printf("Indexing SDK at %s ...\n", sdk.sdk_path().c_str());
      fflush(stdout);
      // Serving a library: its code signatures would only be matched against
      // themselves, so index the headers and skip the SDK's own .lib files.
      sdk.BuildIndex(!program.is_archive);
      printf("SDK: %zu structs from %zu headers, %zu prototypes, %zu symbols "
             "from %zu libs\n", sdk.struct_count(), sdk.header_count(),
             sdk.prototype_count(), sdk.symbol_count(), sdk.lib_count());
      fflush(stdout);
    }
  }

  xexmagic::progress::BeginPhase("whole-program type inference");
  auto* model = new Model(std::move(program));
  {
    std::lock_guard<std::timed_mutex> lk(g_model_mu);
    g_model = model;
  }
  xexmagic::progress::BeginPhase("");
  xexmagic::progress::SetReady(true);
  printf("XEXMagic UI: http://127.0.0.1:%d/  (%zu functions)\n", g_port,
         model->prog().funcs.size());
  fflush(stdout);
}

void SetAwaitingFile(bool awaiting) { g_awaiting_file = awaiting; }

bool SetupRequired() {
  // The library index is the artifact setup produces. Its absence is what
  // "never been set up" means - an SDK path with no index is not enough.
  FILE* fp = fopen(xexmagic::SdkConfig::LibraryCachePath().c_str(), "rb");
  if (!fp) return true;
  fclose(fp);
  return false;
}

void RunSetup() {
  g_setup_mode = true;
  printf("Setup needed: open http://127.0.0.1:%d/ to point XEXMagic at the "
         "Xbox 360 SDK\n", g_port);
  fflush(stdout);
  while (g_setup_mode.load())
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
  printf("Setup done.\n");
  fflush(stdout);
}

void WaitWebServer() {
  for (;;) std::this_thread::sleep_for(std::chrono::hours(1));
}

int RunWebServer(WebProgram program, int port) {
  if (StartWebServer(port) != 0) return 1;
  SetWebProgram(std::move(program));
  WaitWebServer();
  return 0;
}
