// decompiler -- CodeBreaker's decompiler: machine code in, C out, for any processor it has a
// language description for and any file format someone can describe to it.
//
// It does not read executables. Whoever calls it has already taken the file apart and says, in a
// plain description, where the bytes go in memory and what is known for certain about the code:
//
//   arch   <language id>                      e.g. x86:LE:32:default:windows
//                                                  PowerPC:BE:64:A2ALT-32addr:default
//   bytes  <hexVA> <file> <hexOffset> <hexSize>    these bytes of that file are at this address
//   F      <hexVA> <name> <pop>               a function; pop = bytes of arguments it removes on
//                                             return (x86 `ret N`), -1 when unknown/not applicable
//   I      <hexVA> <name> <pop>               an import slot (a pointer the loader fills in) and
//                                             the function it will hold
//
// (tab separated). The analysis itself -- exact instruction semantics, data flow, stack and type
// recovery, control-flow structuring, C printing -- is the Ghidra decompiler core in core/,
// linked in (Apache-2.0; see core/LICENSE and core/NOTICE). What is added here is that nothing
// is guessed that the binary states: function starts and names, calling convention and argument
// count from each function's own return, and imports resolved to named functions.
//
//   decompiler <description> [--at <hexVA>...]      every function listed, or only those
//
// Each function is preceded by `/*@@ <name> <hexVA> */`; each global it touches is reported as
// `/*@G <hexVA> <bytes> */` (the printed name of an unnamed global does not carry its size).
// CB_LANGUAGES names the folder of language files (*.sla, *.ldefs, *.pspec, *.cspec).
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "libdecomp.hh"
#include "sleigh_arch.hh"
#include "loadimage.hh"

using namespace ghidra;

namespace {

struct Fact { uint64_t va; std::string name; int pop; };
struct Span { uint64_t va; std::vector<uint8_t> data; };

// Imports live at made-up addresses outside the image: an import slot reads as the address of
// "its" function, so a call through the slot resolves to a named function with a real prototype.
const uint64_t kExternBase = 0xE0000000u;

class Image : public LoadImage {
  std::vector<Span> spans_;
  std::map<uint64_t, uint64_t> slot_value_;        // import slot VA -> made-up function address
  std::string arch_;
  bool big_ = false;
  int slot_size_ = 4;
 public:
  explicit Image(const std::string& arch) : LoadImage("codebreaker"), arch_(arch) {
    big_ = arch.find(":BE:") != std::string::npos;
  }
  void add(uint64_t va, std::vector<uint8_t> data) { spans_.push_back({va, std::move(data)}); }
  void setSlot(uint64_t slot_va, uint64_t value) { slot_value_[slot_va] = value; }
  const std::map<uint64_t, uint64_t>& slots() const { return slot_value_; }
  int slotSize() const { return slot_size_; }
  bool holds(uint64_t va) const {
    for (const auto& s : spans_)
      if (va >= s.va && va - s.va < s.data.size()) return true;
    return false;
  }
  void loadFill(uint1* ptr, int4 size, const Address& addr) override {
    uint64_t a = addr.getOffset();
    for (int4 i = 0; i < size; ++i) {
      uint64_t va = a + (uint64_t)i;
      auto it = slot_value_.upper_bound(va);
      if (it != slot_value_.begin()) {
        --it;
        uint64_t k = va - it->first;
        if (k < (uint64_t)slot_size_) {
          int shift = 8 * (int)(big_ ? (slot_size_ - 1 - k) : k);
          ptr[i] = (uint1)(it->second >> shift);
          continue;
        }
      }
      ptr[i] = 0;
      for (const auto& s : spans_)
        if (va >= s.va && va - s.va < s.data.size()) { ptr[i] = s.data[(size_t)(va - s.va)]; break; }
    }
  }
  string getArchType(void) const override { return arch_; }
  void adjustVma(long) override {}
};

class Machine : public SleighArchitecture {
  Image* image_;
  void buildLoader(DocumentStorage&) override {
    collectSpecFiles(*errorstream);
    loader = image_;
  }
  void resolveArchitecture(void) override {
    archid = getTarget();
    SleighArchitecture::resolveArchitecture();
  }
 public:
  Machine(const string& fname, const string& target, ostream* estream, Image* image)
      : SleighArchitecture(fname, target, estream), image_(image) {}
};

// x86, 32-bit: a function that returns with `ret N` (N > 0) removes its own N/4 arguments
// (stdcall); with a plain `ret` the caller cleans up (cdecl) and the argument count is left to
// the analysis.
void applyX86Proto(Machine& m, Funcdata* fd, const std::string& name, int pop) {
  if (pop < 0) return;
  FuncProto& fp = fd->getFuncProto();
  PrototypePieces pieces;
  pieces.model = m.getModel(pop == 0 ? "__cdecl" : "__stdcall");
  pieces.name = name;
  // Every x86 function leaves a value in EAX and callers are free to use it; declaring the
  // result keeps it. Left to itself the analysis calls a function `void` whenever it does not
  // see the value used, and the function then returns nothing.
  pieces.outtype = m.types->getBase(4, TYPE_INT);
  pieces.firstVarArgSlot = -1;
  for (int i = 0; i < pop / 4; ++i) {
    pieces.intypes.push_back(m.types->getBase(4, TYPE_INT));
    pieces.innames.push_back("a" + std::to_string(i + 1));
  }
  fp.setPieces(pieces);
  // The stdcall model says only "the callee removes some bytes"; left at that, the analysis has
  // to solve for how many, and when it solves wrong every stack reference after the call is off
  // by four. We know the number: the return address plus the function's own `ret N`.
  fp.setExtraPop(4 + pop);
  // With `ret N` the argument count is known; with a plain `ret` it is left to the analysis.
  // The argument TYPES are placeholders either way (four bytes each).
  fp.setInputLock(pop > 0);
  fp.setModelLock(true);
  fp.setOutputLock(true);
}

bool readFile(const std::string& path, uint64_t off, uint64_t size, std::vector<uint8_t>& out) {
  std::ifstream in(path, std::ios::binary);
  if (!in) return false;
  in.seekg((std::streamoff)off);
  out.resize((size_t)size);
  in.read((char*)out.data(), (std::streamsize)size);
  out.resize((size_t)in.gcount());
  return true;
}

std::vector<std::string> split(const std::string& line) {
  std::vector<std::string> out;
  std::string cur;
  for (char c : line) {
    if (c == '\t') { out.push_back(cur); cur.clear(); }
    else if (c != '\r') cur += c;
  }
  out.push_back(cur);
  return out;
}

}  // namespace

int main(int argc, char** argv) {
  if (argc < 2) {
    std::fprintf(stderr, "usage: %s <description> [--at <hexVA>...]\n", argv[0]);
    return 2;
  }
  const char* specs = std::getenv("CB_LANGUAGES");
  if (!specs) { std::fprintf(stderr, "FAIL: CB_LANGUAGES is not set\n"); return 1; }

  std::string arch;
  std::vector<Fact> funcs, imports;
  std::vector<Span> spans;
  {
    std::ifstream in(argv[1]);
    if (!in) { std::fprintf(stderr, "FAIL: cannot read %s\n", argv[1]); return 1; }
    std::string line;
    while (std::getline(in, line)) {
      std::vector<std::string> f = split(line);
      if (f.size() >= 2 && f[0] == "arch") arch = f[1];
      else if (f.size() >= 5 && f[0] == "bytes") {
        Span s;
        s.va = strtoull(f[1].c_str(), nullptr, 16);
        if (!readFile(f[2], strtoull(f[3].c_str(), nullptr, 16), strtoull(f[4].c_str(), nullptr, 16), s.data)) {
          std::fprintf(stderr, "FAIL: cannot read %s\n", f[2].c_str());
          return 1;
        }
        spans.push_back(std::move(s));
      } else if (f.size() >= 3 && (f[0] == "F" || f[0] == "I")) {
        Fact x{strtoull(f[1].c_str(), nullptr, 16), f[2], f.size() > 3 ? atoi(f[3].c_str()) : -1};
        (f[0] == "I" ? imports : funcs).push_back(x);
      }
    }
  }
  if (arch.empty() || spans.empty()) { std::fprintf(stderr, "FAIL: the description names no arch or no bytes\n"); return 1; }
  std::set<uint64_t> only;
  for (int i = 2; i < argc; ++i)
    if (std::string(argv[i]) == "--at")
      for (++i; i < argc && argv[i][0] != '-'; ++i) only.insert(strtoull(argv[i], nullptr, 16));
  const bool x86_32 = arch.rfind("x86:LE:32", 0) == 0;

  try {
    startDecompilerLibrary(std::vector<std::string>{});
    SleighArchitecture::specpaths.addDir2Path(specs);
    Image* image = new Image(arch);
    for (auto& s : spans) image->add(s.va, std::move(s.data));
    for (size_t i = 0; i < imports.size(); ++i)
      image->setSlot(imports[i].va, kExternBase + (uint64_t)i * 0x10);
    std::ostringstream errs;
    Machine m(argv[1], arch, &errs, image);
    DocumentStorage store;
    m.init(store);
    m.readonlypropagate = true;
    AddrSpace* code = m.getDefaultCodeSpace();
    Scope* glb = m.symboltab->getGlobalScope();
    // An import slot never changes while the module runs: reading one gives a constant, so a
    // call through it -- directly or after the slot was copied to a register -- is a call to
    // that import.
    for (const auto& kv : image->slots())
      m.symboltab->setPropertyRange(Varnode::readonly, Range(code, kv.first, kv.first + image->slotSize() - 1));

    for (size_t i = 0; i < imports.size(); ++i) {
      Address ea(code, (uintb)kExternBase + (uintb)i * 0x10);
      Funcdata* fd = glb->addFunction(ea, imports[i].name)->getFunction();
      if (x86_32) applyX86Proto(m, fd, imports[i].name, imports[i].pop);
    }
    std::vector<std::pair<Funcdata*, Fact>> todo;
    for (const auto& f : funcs) {
      Address addr(code, (uintb)f.va);
      Funcdata* fd = glb->addFunction(addr, f.name)->getFunction();
      if (x86_32) applyX86Proto(m, fd, f.name, f.pop);
      if (only.empty() || only.count(f.va)) todo.push_back({fd, f});
    }

    m.print->setOutputStream(&std::cout);
    for (auto& t : todo) {
      std::cout << "/*@@ " << t.second.name << " " << std::hex << t.second.va << std::dec << " */" << std::endl;
      try {
        Action* act = m.allacts.getCurrent();
        act->reset(*t.first);
        act->perform(*t.first);
        std::set<std::pair<uint64_t, int>> seen;
        AddrSpace* ram = m.getDefaultDataSpace();
        for (VarnodeLocSet::const_iterator it = t.first->beginLoc(ram); it != t.first->endLoc(ram); ++it) {
          const Varnode* vn = *it;
          if (vn->isAnnotation()) continue;
          uint64_t va = vn->getOffset();
          if (!image->holds(va)) continue;
          if (seen.insert({va, (int)vn->getSize()}).second)
            std::cout << "/*@G " << std::hex << va << std::dec << " " << vn->getSize() << " */" << std::endl;
        }
        m.print->docFunction(t.first);
      } catch (LowlevelError& e) {
        std::cout << "/* decompile failed: " << e.explain << " */" << std::endl;
      }
      std::cout << std::endl;
      m.clearAnalysis(t.first);                    // free this function's analysis; keep its symbol
    }
    if (!errs.str().empty()) std::fprintf(stderr, "%s\n", errs.str().c_str());
  } catch (LowlevelError& e) {
    std::fprintf(stderr, "FAIL: %s\n", e.explain.c_str());
    return 1;
  } catch (DecoderError& e) {
    std::fprintf(stderr, "FAIL: %s\n", e.explain.c_str());
    return 1;
  }
  return 0;
}
