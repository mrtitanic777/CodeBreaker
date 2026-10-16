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
//   X      <hexVA> <name> <pop> [<model>]     a function outside the image, at an address the
//                                             describer chose (16-bit imports are patched in as
//                                             far pointers to such addresses)
//   reg    <register> <hexValue>              a register's value everywhere, e.g. DS = the data
//                                             segment of a 16-bit module
//
// F may carry a fifth field, the calling-convention model by name (e.g. __stdcall16far), or
// the word `inline`: a compiler helper that is part of every caller's prologue or epilogue
// (PowerPC register save/restore runs), whose instructions are read as the caller's own.
//
// (tab separated). The analysis itself -- exact instruction semantics, data flow, stack and type
// recovery, control-flow structuring, C printing -- is the Ghidra decompiler core in core/,
// linked in (Apache-2.0; see core/LICENSE and core/NOTICE). What is added here is that nothing
// is guessed that the binary states: function starts and names, calling convention and argument
// count from each function's own return, and imports resolved to named functions.
//
//   decompiler <description> [--at <hexVA>...]      every function listed, or only those
//
// For processors other than 32-bit x86, each pointer-typed local or parameter is reported as
// `/*@P <name> <bytes> */` (16-bit code has near and far pointers; the C text does not say which).
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

struct Fact { uint64_t va; std::string name; int pop; std::string model; int result = -1; };   // result: bytes it returns in AX (DX:AX), when known
struct Span { uint64_t va; std::vector<uint8_t> data; };
// A call through a pointer (a driver's dispatch table, a callback): what it is passed, as the
// code pushed it -- the callee is not known, so the analysis cannot tell what it takes.
struct CallSite { uint64_t site; uint64_t func; int pop; std::string model; };

// Imports live at made-up addresses outside the image: an import slot reads as the address of
// "its" function, so a call through the slot resolves to a named function with a real prototype.
const uint64_t kExternBase = 0xE0000000u;
// The software interrupts' made-up functions: vector n at F000:n*16 (languages/x86-16.cspec).
// What one hands back besides AX is named int_<register> (core/cpp/database.cc, buildDefaultName).
const uint64_t kInterruptBase = 0xF0000000u;

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
//
// `self`: the function takes its first argument in ECX (a C++ member function's object) and
// removes the rest itself -- said by the describer, which saw ECX read before it is written.
void applyX86Proto(Machine& m, Funcdata* fd, const std::string& name, int pop, bool self = false,
                   bool second = false) {
  if (pop < 0) return;
  FuncProto& fp = fd->getFuncProto();
  PrototypePieces pieces;
  // (`second`: and its second in EDX -- the two-register convention)
  pieces.model = m.getModel(second ? "__fastcall" : self ? "__thiscall" : pop == 0 ? "__cdecl" : "__stdcall");
  pieces.name = name;
  // Every x86 function leaves a value in EAX and callers are free to use it; declaring the
  // result keeps it. Left to itself the analysis calls a function `void` whenever it does not
  // see the value used, and the function then returns nothing.
  pieces.outtype = m.types->getBase(4, TYPE_INT);
  pieces.firstVarArgSlot = -1;
  if (self) {
    pieces.intypes.push_back(m.types->getBase(4, TYPE_INT));
    pieces.innames.push_back("self");
  }
  if (second) {
    pieces.intypes.push_back(m.types->getBase(4, TYPE_INT));
    pieces.innames.push_back("second");
  }
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
  fp.setInputLock(pop > 0 || self);
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
  std::vector<Fact> funcs, imports, externs;
  std::vector<std::pair<std::string, uint64_t>> regs;
  std::vector<Span> spans;
  std::vector<CallSite> calls;
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
      } else if (f.size() >= 3 && (f[0] == "F" || f[0] == "I" || f[0] == "X")) {
        Fact x{strtoull(f[1].c_str(), nullptr, 16), f[2], f.size() > 3 ? atoi(f[3].c_str()) : -1,
               f.size() > 4 ? f[4] : std::string()};
        if (f.size() > 5 && !f[5].empty()) x.result = atoi(f[5].c_str());
        (f[0] == "I" ? imports : f[0] == "X" ? externs : funcs).push_back(x);
      } else if (f.size() >= 5 && f[0] == "O") {
        calls.push_back({strtoull(f[1].c_str(), nullptr, 16), strtoull(f[2].c_str(), nullptr, 16),
                         atoi(f[3].c_str()), f[4]});
      } else if (f.size() >= 3 && f[0] == "reg") {
        regs.push_back({f[1], strtoull(f[2].c_str(), nullptr, 16)});
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
    // Nothing stops an escaped local's aliasing: a buffer whose address is passed on is as long as
    // the callee writes, not as the array the analysis guessed (KRNL386 fills a struct and reads
    // a field past what it sized as the array -- read as a variable never set)
    m.alias_block_level = 0;
    m.max_instructions = 20000;                    // a wrong function start must fail quickly, not walk memory
    if (const char* mx = std::getenv("CB_MAX_INSTRUCTIONS"))  // (a large function is tried again with more)
      m.max_instructions = (uint4)std::atoi(mx);
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
    for (const auto& r : regs) {                     // a register known to hold one value throughout
      const VarnodeData& vd = m.translate->getRegister(r.first);
      TrackedSet& ts = m.context->getTrackedDefault();
      ts.emplace_back();
      ts.back().loc = vd;
      ts.back().val = (uintb)r.second;
    }
    auto applyModel = [&](Funcdata* fd, const Fact& f) {
      if (f.model == "inline") { fd->getFuncProto().setInline(true); return; }
      if (x86_32) { applyX86Proto(m, fd, f.name, f.pop, f.model == "__thiscall" || f.model == "__fastcall",
                                  f.model == "__fastcall"); return; }
      if (!f.model.empty() && m.hasModel(f.model)) {
        FuncProto& fp = fd->getFuncProto();
        const int ret_addr = f.model.find("far") != std::string::npos ? 4 : 2;
        if (f.pop > 0 && f.model.find("stdcall") != std::string::npos) {
          // 16-bit, the function removes its own arguments: `ret N` / `retf N` states how many
          // bytes there are, so the arguments are N/2 words. (Their types are placeholders; a
          // far pointer is two of them.) Without this the analysis decides per function how
          // many it sees used, and a function that ignores an argument is rebuilt removing
          // fewer bytes than its callers pushed.
          PrototypePieces pieces;
          pieces.model = m.getModel(f.model);
          pieces.name = f.name;
          pieces.outtype = m.types->getBase(2, TYPE_INT);
          pieces.firstVarArgSlot = -1;
          for (int i = 0; i < f.pop / 2; ++i) {
            pieces.intypes.push_back(m.types->getBase(2, TYPE_INT));
            pieces.innames.push_back("a" + std::to_string(i + 1));
          }
          if (f.result == 2 || f.result == 4)
            pieces.outtype = m.types->getBase(f.result, TYPE_INT);
          fp.setPieces(pieces);
          fp.setInputLock(true);
          // the result: what its instructions leave in AX (DX:AX) when it returns, where the
          // description says (an export's caller is Windows: nothing in the module shows it read);
          // otherwise two bytes or four, left to the analysis
          fp.setOutputLock(f.result == 2 || f.result == 4);
        } else {
          fp.setModel(m.getModel(f.model));
          if (f.result == 2 || f.result == 4) {
            // the arguments left to the analysis, the result as the description says
            PrototypePieces pieces;
            pieces.model = m.getModel(f.model);
            pieces.name = f.name;
            pieces.outtype = m.types->getBase(f.result, TYPE_INT);
            pieces.firstVarArgSlot = -1;
            fp.setPieces(pieces);
            fp.setInputLock(false);
            fp.setOutputLock(true);
          }
        }
        fp.setModelLock(true);
        // the bytes a function removes: its own `ret N` plus the return address
        if (f.pop >= 0) fp.setExtraPop(f.pop + ret_addr);
      }
    };
    for (const auto& x : externs) {
      Address ea(code, (uintb)x.va);
      applyModel(glb->addFunction(ea, x.name)->getFunction(), x);
    }
    // 16-bit: the software interrupts. The compiler spec turns `INT n` into a call to F000:n*16
    // (its swi fixup); each of those is a function int<n> taking its arguments and giving its
    // result in registers (__int16), so what the code puts in them before the interrupt is seen.
    if (!x86_32 && m.hasModel("__int16")) {
      for (const char* nm : {"__int16", "__int16keepds"})  // its registers are its arguments, by position
        if (m.hasModel(nm)) m.getModel(nm)->keepArgumentHoles();
      for (int v = 0; v < 256; ++v) {
        char nm[16];
        std::snprintf(nm, sizeof(nm), "int%02x", v);
        Fact x{kInterruptBase + (uint64_t)v * 16, nm, -1,
               (v == 0x21 || v == 0x2f || !m.hasModel("__int16keepds")) ? "__int16" : "__int16keepds"};
        Address ea(code, (uintb)x.va);
        applyModel(glb->addFunction(ea, x.name)->getFunction(), x);
      }
    }
    std::vector<std::pair<Funcdata*, Fact>> todo;
    for (const auto& f : funcs) {
      Address addr(code, (uintb)f.va);
      Funcdata* fd = glb->addFunction(addr, f.name)->getFunction();
      applyModel(fd, f);
      if (f.model != "inline" && (only.empty() || only.count(f.va))) todo.push_back({fd, f});
    }

    m.print->setOutputStream(&std::cout);
    for (auto& t : todo) {
      std::cout << "/*@@ " << t.second.name << " " << std::hex << t.second.va << std::dec << " */" << std::endl;
      try {
        // its calls through pointers: each passed the words the code pushed before it, removed
        // by the callee (Windows' drivers and callbacks are Pascal)
        for (const auto& c : calls) {
          if (c.func != t.second.va || !m.hasModel(c.model)) continue;
          FuncProto* p = new FuncProto();
          p->setInternal(m.getModel(c.model), m.types->getTypeVoid());
          PrototypePieces pieces;
          pieces.model = m.getModel(c.model);
          pieces.name = "";
          pieces.outtype = m.types->getBase(2, TYPE_UNKNOWN);
          pieces.firstVarArgSlot = -1;
          for (int i = 0; i < c.pop / 2; ++i) {
            pieces.intypes.push_back(m.types->getBase(2, TYPE_INT));
            pieces.innames.push_back("p" + std::to_string(i + 1));
          }
          p->setPieces(pieces);
          p->setInputLock(true);
          p->setOutputLock(false);
          p->setExtraPop(c.pop + (c.model.find("far") != std::string::npos ? 4 : 2));
          t.first->getOverride().insertProtoOverride(Address(code, (uintb)c.site), p);
        }
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
        // 16-bit code has two sizes of pointer and C text does not show which a variable is:
        // report every pointer-typed local and parameter with its size, `/*@P <name> <bytes> */`.
        if (!x86_32) {
          ScopeLocal* loc = t.first->getScopeLocal();
          std::set<std::string> said;
          for (MapIterator it = loc->begin(); it != loc->end(); ++it) {
            const Symbol* sym = (*it)->getSymbol();
            if (sym == nullptr || sym->getType() == nullptr) continue;
            if (sym->getType()->getMetatype() != TYPE_PTR) continue;
            if (said.insert(sym->getName()).second)
              std::cout << "/*@P " << sym->getName() << " " << sym->getType()->getSize() << " */" << std::endl;
          }
          // ...and the function's own result, under the name `return`
          Datatype* rt = t.first->getFuncProto().getOutputType();
          if (rt != nullptr && rt->getMetatype() == TYPE_PTR)
            std::cout << "/*@P return " << rt->getSize() << " */" << std::endl;
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
