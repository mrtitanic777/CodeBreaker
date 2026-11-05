// flow16.h -- invert/flow16.py in C++: the control-flow graph and the structure tree of a 16-bit
// function, for the compiler models.  Same blocks, same edges, same tree as the Python, decision
// for decision (docs/NATIVE.md: the invert gate).
#pragma once
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace cbinv {

// A Python exception at the place the Python raised it: kind 1 IndexError, 2 ValueError.
struct PyError {
  int kind;
  std::string msg;
};

struct Ins {
  long long addr;
  std::string mn, op;
};

constexpr int NONE = -1;

struct Block {
  int id = 0, lo = 0, hi = 0;
  std::string term;                                  // ret jmp cond fall tailcall switch dead
  std::vector<int> succ;
  int cond_idx = NONE;
  bool has_table = false;
  std::vector<int> table;
  bool has_cases = false;
  std::vector<std::pair<long long, int>> cases;
  int def = NONE;
  std::string selreg;                                // "" = None
  int selbase = NONE;
};

// build_cfg: `sw` holds the SWITCH hook's answer for an instruction's address (absent = None).
std::vector<Block> build_cfg(const std::vector<Ins> &insns,
                             const std::map<long long, std::vector<long long>> &sw, bool cmpswitch);

// A node of the structure tree, the Python tuple's fields.
struct Node;
using Seq = std::vector<Node>;
struct CaseVal {
  bool is_default;
  long long v;
};
struct Node {
  enum Kind { BLOCK, BREAK, CONTINUE, GOTO, IF, IFELSE, WHILE, DOWHILE, LOOP, SWITCH } kind;
  int a = NONE;                                      // the block (header, test, target of a goto)
  int join = NONE;
  bool flag = false;
  Seq s1, s2;
  std::vector<std::pair<std::vector<CaseVal>, Seq>> cases;
};

// structure: blocks need term, succ, lo, hi, cases/default, table.
Seq structure(const std::vector<Block> &blocks, int entry, bool nocap);

std::string json_blocks(const std::vector<Block> &blocks);
std::string json_seq(const Seq &s);

}  // namespace cbinv
