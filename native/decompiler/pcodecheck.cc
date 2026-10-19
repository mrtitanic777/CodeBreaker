// pcodecheck -- run single instructions through the SLEIGH specification's p-code, for checking
// the specification against an emulator of the real processor (tools/sleigh_verify.py).
//
//   pcodecheck <file.sla> <mode>        mode: x86-16-real, x86-32-flat, ppc-be-64
//
// Reads cases from stdin, one per line:
//   <id> CS=<hex> IP=<hex> EAX=<hex> ... CF=<0|1> ... M<hex address>=<hex bytes> ...
// (the instruction's bytes are given as memory at CS*16+IP; in x86-32-flat at IP itself, the
// 32-bit offset, as the decompiler's x86:LE:32 sees a Windows program: every segment based at 0;
// in ppc-be-64 at IP, with the PowerPC registers r0.. LR CTR cr0.. xer_so .. f0.. instead, and
// memory big-endian). Executes the one instruction's
// p-code with the registers and memory given -- memory not given reads as zero -- and writes:
//   <id> ok LIN=<hex> EAX=<hex> ... CF=<0|1> ... W<hex address>=<hex byte> ...
//   <id> skip <why>        (a user operation the emulator has no meaning for: in, out, int ...)
//   <id> err <message>
// LIN is the linear address of the next instruction; W are the bytes of memory written.
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "sleigh.hh"
#include "emulate.hh"
#include "memstate.hh"
#include "loadimage.hh"

using namespace ghidra;

namespace {

const std::vector<std::string> REGS_X86 = {"EAX", "ECX", "EDX", "EBX", "ESP", "EBP", "ESI", "EDI",
                                           "ES", "CS", "SS", "DS", "FS", "GS",
                                           "CF", "PF", "AF", "ZF", "SF", "TF", "IF", "DF", "OF"};

// PowerPC: the general registers, the link and count registers, the condition register's eight
// fields (four bits each, a byte apiece in the specification), XER's bits, the floating registers
std::vector<std::string> regsPowerPC() {
  std::vector<std::string> v;
  for (int i = 0; i < 32; ++i) v.push_back("r" + std::to_string(i));
  v.push_back("LR");
  v.push_back("CTR");
  for (int i = 0; i < 8; ++i) v.push_back("cr" + std::to_string(i));
  for (const char *b : {"xer_so", "xer_ov", "xer_ca"}) v.push_back(b);
  for (int i = 0; i < 32; ++i) v.push_back("f" + std::to_string(i));
  return v;
}

// Memory of the case: bytes given, zero elsewhere; every byte whose value a write changes is
// remembered. Words of 8 bytes, little-endian, as the core's own banks (MemoryBank's setValue
// and getValue take an access to span at most two words: with 1-byte words a 2-byte write lost
// its first byte).
class CaseRam : public MemoryBank {
public:
  std::map<uintb, uint1> bytes;
  std::set<uintb> written;
  bool big;                                       // (a word's first byte is its high one)
  uintb mask = ~(uintb)0;                         // (ppc-be-64: addresses are 32 bits, as the
                                                  // language's ram is truncated to 4 bytes)
  CaseRam(AddrSpace *spc) : MemoryBank(spc, 8, 4096), big(spc->isBigEndian()) {}
  uint1 byte(uintb a) const {
    auto it = bytes.find(a & mask);
    return it == bytes.end() ? 0 : it->second;
  }
  void insert(uintb addr, uintb val) override {
    for (int i = 0; i < 8; ++i) {
      uint1 b = (uint1)(val >> (8 * (big ? 7 - i : i)));
      if (b != byte(addr + i)) {
        bytes[(addr + i) & mask] = b;
        written.insert((addr + i) & mask);
      }
    }
  }
  uintb find(uintb addr) const override {
    uintb w = 0;
    for (int i = 0; i < 8; ++i) w = (w << 8) | byte(addr + (big ? i : 7 - i));
    return w;
  }
};

// The instruction's bytes for the decoder, read from the case's memory.
// (ppc-be-64: code not given decodes as nop -- the emulator decodes the instruction a branch goes
// to once the branch is done, and a zero word is no PowerPC instruction)
class CaseImage : public LoadImage {
public:
  CaseRam *ram = nullptr;
  bool nops = false;
  CaseImage() : LoadImage("case") {}
  void loadFill(uint1 *ptr, int4 size, const Address &addr) override {
    for (int4 i = 0; i < size; ++i) {
      uintb a = addr.getOffset() + i;
      ptr[i] = !nops || ram->bytes.count(a & ram->mask) ? ram->byte(a) : (a & 3) == 0 ? 0x60 : 0;
    }
  }
  string getArchType(void) const override { return "x86"; }
  void adjustVma(long) override {}
};

// segment(base, inner) in real mode: base * 16 + inner (languages/x86-16-real.pspec).
class RealSegment : public BreakCallBack {
public:
  bool pcodeCallback(PcodeOpRaw *op) override {
    MemoryState *mem = static_cast<EmulateMemory *>(emulate)->getMemoryState();
    uintb base = mem->getValue(op->getInput(1));
    uintb inner = mem->getValue(op->getInput(2));
    mem->setValue(op->getOutput(), ((base & 0xffff) << 4) + (inner & 0xffff));
    return true;
  }
};

// segment(base, inner) in a flat 32-bit program: the segment adds nothing. (FS and GS, whose bases
// are not 0 -- a thread's own block -- are left out by the caller.)
class FlatSegment : public BreakCallBack {
public:
  bool pcodeCallback(PcodeOpRaw *op) override {
    MemoryState *mem = static_cast<EmulateMemory *>(emulate)->getMemoryState();
    mem->setValue(op->getOutput(), mem->getValue(op->getInput(2)) & 0xffffffff);
    return true;
  }
};

// Any other user operation: the case is not something the emulator can judge.
struct Untestable {
  std::string name;
};
class Refuse : public BreakCallBack {
public:
  std::string name;
  explicit Refuse(const std::string &n) : name(n) {}
  bool pcodeCallback(PcodeOpRaw *) override { throw Untestable{name}; }
};

}  // namespace

int main(int argc, char **argv) {
  const std::string mode = argc == 3 ? argv[2] : "";
  if (mode != "x86-16-real" && mode != "x86-32-flat" && mode != "ppc-be-64") {
    std::fprintf(stderr, "usage: %s <file.sla> x86-16-real|x86-32-flat|ppc-be-64   (cases on stdin)\n", argv[0]);
    return 2;
  }
  const bool ppc = mode == "ppc-be-64";
  const bool flat = mode == "x86-32-flat" || ppc;     // (an address is the address itself)
  const std::vector<std::string> REGS = ppc ? regsPowerPC() : REGS_X86;
  AttributeId::initialize();
  ElementId::initialize();
  CaseImage image;
  ContextInternal context;
  Sleigh trans(&image, &context);
  DocumentStorage store;
  std::istringstream slaname(std::string("<sleigh>") + argv[1] + "</sleigh>");
  Element *root = store.parseDocument(slaname)->getRoot();
  store.registerTag(root);
  trans.initialize(store);
  auto defaults = [&](ContextInternal &ctx) {
    if (ppc) {
    } else if (flat) {                            // (as languages/x86.pspec)
      ctx.setVariableDefault("addrsize", 1);
      ctx.setVariableDefault("opsize", 1);
    } else {                                     // (as languages/x86-16-real.pspec)
      ctx.setVariableDefault("addrsize", 0);
      ctx.setVariableDefault("opsize", 0);
      ctx.setVariableDefault("protectedMode", 0);
    }
  };
  defaults(context);

  AddrSpace *ramspace = trans.getDefaultCodeSpace();
  std::vector<std::string> userops;
  trans.getUserOpNames(userops);

  std::string line;
  while (std::getline(std::cin, line)) {
    std::istringstream in(line);
    std::string id;
    in >> id;
    if (id.empty()) continue;
    CaseRam ram(ramspace);
    if (ppc) ram.mask = 0xffffffff;
    image.ram = &ram;
    image.nops = ppc;
    MemoryHashOverlay regs(trans.getSpaceByName("register"), 8, 4096, 4096, (MemoryBank *)0);
    MemoryHashOverlay tmp(trans.getUniqueSpace(), 8, 4096, 4096, (MemoryBank *)0);
    MemoryState mem(&trans);
    mem.setMemoryBank(&ram);
    mem.setMemoryBank(&regs);
    mem.setMemoryBank(&tmp);
    std::map<std::string, uintb> given;
    std::string tok;
    while (in >> tok) {
      size_t eq = tok.find('=');
      if (eq == std::string::npos) continue;
      std::string k = tok.substr(0, eq), v = tok.substr(eq + 1);
      if (k[0] == 'M') {
        uintb at = std::strtoull(k.c_str() + 1, nullptr, 16);
        for (size_t i = 0; i + 1 < v.size(); i += 2)
          ram.bytes[at + i / 2] = (uint1)std::strtoul(v.substr(i, 2).c_str(), nullptr, 16);
      } else {
        given[k] = std::strtoull(v.c_str(), nullptr, 16);
      }
    }
    try {
      // (every case is decoded afresh: the same address holds another instruction each time --
      // reset() drops the decode caches, initialize() makes them again, registering the context
      // variables anew: in a context of the case's own, the one before being locked by then)
      ContextInternal caseContext;
      trans.reset(&image, &caseContext);
      trans.initialize(store);
      trans.setDefaultFloatFormats();               // (else every floating-point operation is "unimplemented")
      defaults(caseContext);
      BreakTableCallBack breaks(&trans);
      RealSegment segment;
      FlatSegment flatSegment;
      std::vector<Refuse *> refusals;
      for (size_t i = 0; i < userops.size(); ++i) {
        if (userops[i].empty()) continue;
        if (userops[i] == "segment")
          breaks.registerPcodeCallback("segment", flat ? (BreakCallBack *)&flatSegment : &segment);
        else {
          refusals.push_back(new Refuse(userops[i]));
          breaks.registerPcodeCallback(userops[i], refusals.back());
        }
      }
      EmulatePcodeCache emu(&trans, &mem, &breaks);
      for (const std::string &r : REGS) mem.setValue(r, given.count(r) ? given[r] : 0);
      if (ppc) mem.setValue("XER", (given["xer_so"] << 31) | (given["xer_ov"] << 30) | (given["xer_ca"] << 29));
      uintb cs = given["CS"] & 0xffff, ip = given["IP"] & (flat ? 0xffffffff : 0xffff);
      if (!ppc) mem.setValue("EIP", ip);
      emu.setExecuteAddress(Address(ramspace, flat ? ip : (cs << 4) + ip));
      std::string outcome;
      try {
        emu.executeInstruction();
      } catch (Untestable &u) {
        outcome = "skip userop:" + u.name;
      }
      for (Refuse *r : refusals) delete r;
      if (!outcome.empty()) {
        std::printf("%s %s\n", id.c_str(), outcome.c_str());
        std::fflush(stdout);
        continue;
      }
      std::ostringstream out;
      out << id << " ok LIN=" << std::hex << emu.getExecuteAddress().getOffset();
      for (const std::string &r : REGS) out << " " << r << "=" << std::hex << mem.getValue(r);
      for (uintb a : ram.written) out << " W" << std::hex << a << "=" << (unsigned)ram.bytes[a];
      std::printf("%s\n", out.str().c_str());
    } catch (LowlevelError &e) {
      std::printf("%s err %s\n", id.c_str(), e.explain.c_str());
    } catch (std::exception &e) {
      std::printf("%s err %s\n", id.c_str(), e.what());
    }
    std::fflush(stdout);
  }
  return 0;
}
