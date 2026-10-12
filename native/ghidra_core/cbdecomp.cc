// cbdecomp -- CodeBreaker's front end to the Ghidra decompiler core.
//
// The core does the hard, general work: exact instruction semantics (SLEIGH p-code), data-flow in
// SSA form, stack-variable and type recovery, control-flow structuring, C printing. This file
// gives it what it lacks and CodeBreaker has: the image through our own loader, the module's
// function list, and (as they are added) each function's proven argument count and calling
// convention and every import's signature -- facts read from the binaries, not guessed.
//
//   cbdecomp <pe> --at <hexRVA> [<hexRVA> ...]     decompile the functions at those addresses
//   cbdecomp <pe> --exports                        decompile every exported function
//
// CB_GHIDRA_SPECS names the directory holding the x86 language files (x86.sla, x86.ldefs, ...).
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "libdecomp.hh"
#include "sleigh_arch.hh"
#include "loadimage.hh"
#include "pe_loader.h"

using namespace ghidra;

namespace {

// The image, served to the decompiler from our PE loader's flat (RVA-indexed) buffer.
class CbLoadImage : public LoadImage {
  const win95::PEImage& img_;
 public:
  explicit CbLoadImage(const win95::PEImage& img) : LoadImage("codebreaker"), img_(img) {}
  void loadFill(uint1* ptr, int4 size, const Address& addr) override {
    uintb a = addr.getOffset();
    for (int4 i = 0; i < size; ++i) {
      uintb off = a + (uintb)i - img_.image_base;
      ptr[i] = (a + (uintb)i >= img_.image_base && off < img_.image.size()) ? img_.image[(size_t)off] : 0;
    }
  }
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

}  // namespace

int main(int argc, char** argv) {
  if (argc < 3) {
    std::fprintf(stderr, "usage: %s <pe> --at <hexRVA>... | --exports\n", argv[0]);
    return 2;
  }
  win95::PEImage img = win95::LoadPE(argv[1]);
  if (!img.ok) { std::fprintf(stderr, "FAIL: %s\n", img.error.c_str()); return 1; }
  const char* specs = std::getenv("CB_GHIDRA_SPECS");
  if (!specs) { std::fprintf(stderr, "FAIL: CB_GHIDRA_SPECS is not set\n"); return 1; }

  std::vector<std::pair<uint32_t, std::string>> todo;        // (rva, name)
  std::string mode = argv[2];
  if (mode == "--exports") {
    for (auto& e : img.exports) todo.push_back({e.second, e.first});
  } else if (mode == "--at") {
    for (int i = 3; i < argc; ++i) {
      uint32_t rva = (uint32_t)strtoul(argv[i], nullptr, 16);
      char nm[32]; std::snprintf(nm, sizeof nm, "sub_%06X", rva);
      todo.push_back({rva, nm});
    }
  }

  try {
    startDecompilerLibrary(std::vector<std::string>{});
    SleighArchitecture::specpaths.addDir2Path(specs);
    CbLoadImage* load = new CbLoadImage(img);
    std::ostringstream errs;
    CbArchitecture arch(argv[1], "x86:LE:32:default:windows", &errs, load);
    DocumentStorage store;
    arch.init(store);
    arch.print->setOutputStream(&std::cout);
    for (auto& t : todo) {
      Address addr(arch.getDefaultCodeSpace(), (uintb)img.image_base + t.first);
      Funcdata* fd = arch.symboltab->getGlobalScope()->addFunction(addr, t.second)->getFunction();
      try {
        Action* act = arch.allacts.getCurrent();
        act->reset(*fd);
        act->perform(*fd);
        arch.print->docFunction(fd);
        std::cout << std::endl;
      } catch (LowlevelError& e) {
        std::cout << "/* " << t.second << ": decompile failed: " << e.explain << " */" << std::endl;
      }
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
