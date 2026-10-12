// cbdecomp -- CodeBreaker's front end to the Ghidra decompiler core.
//
// The core does the hard, general work: exact instruction semantics (SLEIGH p-code), data-flow in
// SSA form, stack-variable and type recovery, control-flow structuring, C printing. This file
// gives it what it lacks and CodeBreaker has: the image through our own loader, and FACTS read
// from the binaries rather than guessed --
//
//   * every function's start, name, and how many bytes of arguments it removes on return
//     (its own `ret N`): that fixes its calling convention and argument count;
//   * every import: its name, and the same for it (from the DLL that exports it, checked against
//     the Win32 headers).
//
//   cbdecomp <pe> --facts <file>             decompile every function listed in the facts file
//   cbdecomp <pe> --facts <file> --at <hexRVA>...   only those
//   cbdecomp <pe> --at <hexRVA>...           no facts: the core's own guesses
//
// Facts file, one record per line (tab separated):
//   F <hexRVA> <name> <pop>        a function; pop = bytes removed on return, -1 unknown
//   I <hexRVA> <name> <pop>        an import-table slot and the function it holds
//
// Each function is preceded by `/*@@ <name> <hexRVA> */` so a caller can split the output.
// CB_GHIDRA_SPECS names the directory holding the x86 language files (x86.sla, x86.ldefs, ...).
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
#include "win95/pe_loader.h"

using namespace ghidra;

namespace {

struct Fact { uint32_t rva; std::string name; int pop; };

// Imports live at made-up addresses outside the image: an import-table slot reads as the address
// of "its" function, so `call [slot]` resolves to a named function with a real prototype.
const uint32_t kExternBase = 0xE0000000u;

// The image, served to the decompiler from our PE loader's flat (RVA-indexed) buffer.
class CbLoadImage : public LoadImage {
  const win95::PEImage& img_;
  std::map<uint32_t, uint32_t> slot_value_;        // import slot VA -> made-up function address
 public:
  explicit CbLoadImage(const win95::PEImage& img) : LoadImage("codebreaker"), img_(img) {}
  void setSlot(uint32_t slot_va, uint32_t value) { slot_value_[slot_va] = value; }
  void loadFill(uint1* ptr, int4 size, const Address& addr) override {
    uintb a = addr.getOffset();
    for (int4 i = 0; i < size; ++i) {
      uint32_t va = (uint32_t)(a + (uintb)i);
      auto it = slot_value_.upper_bound(va);
      if (it != slot_value_.begin()) {
        --it;
        if (va - it->first < 4) { ptr[i] = (uint1)(it->second >> (8 * (va - it->first))); continue; }
      }
      uintb off = (uintb)va - img_.image_base;
      ptr[i] = (va >= img_.image_base && off < img_.image.size()) ? img_.image[(size_t)off] : 0;
    }
  }
  const std::map<uint32_t, uint32_t>& slots() const { return slot_value_; }
  string getArchType(void) const override { return "x86:LE:32:default:windows"; }
  void adjustVma(long) override {}
};

class CbArchitecture : public SleighArchitecture {
  CbLoadImage* load_;
  void buildLoader(DocumentStorage&) override {
    collectSpecFiles(*errorstream);
    loader = load_;
  }
  void resolveArchitecture(void) override {
    archid = getTarget();
    SleighArchitecture::resolveArchitecture();
  }
 public:
  CbArchitecture(const string& fname, const string& target, ostream* estream, CbLoadImage* load)
      : SleighArchitecture(fname, target, estream), load_(load) {}
};

// A function that returns with `ret N` (N > 0) is stdcall with N/4 arguments; with a plain `ret`
// the caller cleans up (cdecl) and the argument count is left to the analysis.
void applyProto(CbArchitecture& arch, Funcdata* fd, const std::string& name, int pop) {
  if (pop < 0) return;
  FuncProto& fp = fd->getFuncProto();
  if (pop == 0) {
    ProtoModel* callerPops = arch.getModel("__cdecl");
    if (callerPops) { fp.setModel(callerPops); fp.setModelLock(true); }
    return;
  }
  PrototypePieces pieces;
  pieces.model = arch.getModel("__stdcall");
  pieces.name = name;
  pieces.outtype = arch.types->getBase(4, TYPE_INT);
  pieces.firstVarArgSlot = -1;
  for (int i = 0; i < pop / 4; ++i) {
    pieces.intypes.push_back(arch.types->getBase(4, TYPE_INT));
    pieces.innames.push_back("a" + std::to_string(i + 1));
  }
  fp.setPieces(pieces);
  fp.setInputLock(true);
  fp.setModelLock(true);
  // The types are placeholders (every argument is four bytes; whether it is a pointer is for the
  // analysis to find), and so is the return type.
  fp.setOutputLock(false);
}

}  // namespace

int main(int argc, char** argv) {
  if (argc < 3) {
    std::fprintf(stderr, "usage: %s <pe> [--facts <file>] [--at <hexRVA>...]\n", argv[0]);
    return 2;
  }
  win95::PEImage img = win95::LoadPE(argv[1]);
  if (!img.ok) { std::fprintf(stderr, "FAIL: %s\n", img.error.c_str()); return 1; }
  const char* specs = std::getenv("CB_GHIDRA_SPECS");
  if (!specs) { std::fprintf(stderr, "FAIL: CB_GHIDRA_SPECS is not set\n"); return 1; }

  std::vector<Fact> funcs, imports;
  std::set<uint32_t> only;
  for (int i = 2; i < argc; ++i) {
    std::string a = argv[i];
    if (a == "--facts" && i + 1 < argc) {
      std::ifstream in(argv[++i]);
      std::string line;
      while (std::getline(in, line)) {
        std::istringstream ls(line);
        std::string kind, rva, name; int pop = -1;
        if (!(ls >> kind >> rva >> name)) continue;
        ls >> pop;
        Fact f{(uint32_t)strtoul(rva.c_str(), nullptr, 16), name, pop};
        (kind == "I" ? imports : funcs).push_back(f);
      }
    } else if (a == "--at") {
      for (++i; i < argc && argv[i][0] != '-'; ++i) only.insert((uint32_t)strtoul(argv[i], nullptr, 16));
      --i;
    }
  }
  if (funcs.empty())
    for (uint32_t rva : only) {
      char nm[32]; std::snprintf(nm, sizeof nm, "sub_%06X", rva);
      funcs.push_back({rva, nm, -1});
    }

  try {
    startDecompilerLibrary(std::vector<std::string>{});
    SleighArchitecture::specpaths.addDir2Path(specs);
    CbLoadImage* load = new CbLoadImage(img);
    for (size_t i = 0; i < imports.size(); ++i)
      load->setSlot(img.image_base + imports[i].rva, kExternBase + (uint32_t)i * 0x10);
    std::ostringstream errs;
    CbArchitecture arch(argv[1], "x86:LE:32:default:windows", &errs, load);
    DocumentStorage store;
    arch.init(store);
    arch.readonlypropagate = true;
    AddrSpace* code = arch.getDefaultCodeSpace();
    Scope* glb = arch.symboltab->getGlobalScope();
    // An import-table slot never changes while the module runs: reading one gives a constant,
    // so `call [slot]` (and `mov esi, [slot]` ... `call esi`) is a call to that import.
    for (const auto& kv : load->slots())
      arch.symboltab->setPropertyRange(Varnode::readonly, Range(code, kv.first, kv.first + 3));

    for (size_t i = 0; i < imports.size(); ++i) {
      Address ea(code, (uintb)kExternBase + (uintb)i * 0x10);
      Funcdata* fd = glb->addFunction(ea, imports[i].name)->getFunction();
      applyProto(arch, fd, imports[i].name, imports[i].pop);
    }
    std::vector<std::pair<Funcdata*, Fact>> todo;
    for (const auto& f : funcs) {
      Address addr(code, (uintb)img.image_base + f.rva);
      Funcdata* fd = glb->addFunction(addr, f.name)->getFunction();
      applyProto(arch, fd, f.name, f.pop);
      if (only.empty() || only.count(f.rva)) todo.push_back({fd, f});
    }

    arch.print->setOutputStream(&std::cout);
    for (auto& t : todo) {
      std::cout << "/*@@ " << t.second.name << " " << std::hex << t.second.rva << std::dec << " */" << std::endl;
      try {
        Action* act = arch.allacts.getCurrent();
        act->reset(*t.first);
        act->perform(*t.first);
        // The globals this function touches, with the width of each access: `/*@G <va> <bytes> */`.
        // The printed name of an unnamed global carries its address but not its size.
        {
          std::set<std::pair<uint32_t, int>> seen;
          AddrSpace* ram = arch.getDefaultDataSpace();
          for (VarnodeLocSet::const_iterator it = t.first->beginLoc(ram); it != t.first->endLoc(ram); ++it) {
            const Varnode* vn = *it;
            if (vn->isAnnotation()) continue;
            uint32_t va = (uint32_t)vn->getOffset();
            if (va < img.image_base || va - img.image_base >= img.image.size()) continue;
            if (seen.insert({va, (int)vn->getSize()}).second)
              std::cout << "/*@G " << std::hex << va << std::dec << " " << vn->getSize() << " */" << std::endl;
          }
        }
        arch.print->docFunction(t.first);
      } catch (LowlevelError& e) {
        std::cout << "/* decompile failed: " << e.explain << " */" << std::endl;
      }
      std::cout << std::endl;
      arch.clearAnalysis(t.first);                 // free this function's analysis; keep its symbol
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
