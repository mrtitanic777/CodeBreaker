// shadow.h -- the path explorer's symbolic shadow of a run (shadow.cc), as the recorders call it:
// stepped before each instruction, told of each call the environment answered, of memory it
// wrote and registers it set.  The same calls emulate/shadow.py's Shadow made from the Python
// hooks, now made from the C++ ones.
#pragma once
#include <cstdint>

#include "libs.h"

namespace cbt {

struct Shadow;

void sh_step(Shadow *s, uc_engine *uc, uint64_t address, uint64_t cs_base);
// after_call(m, symbolic_result, value, args_at, nargs, why, word[, carry])
void sh_after_call(Shadow *s, uc_engine *uc, bool symbolic, uint64_t value, bool has_args_at = false,
                   uint64_t args_at = 0, int64_t nargs = 0, const char *why = nullptr, int word = 2,
                   uint64_t carry = 0);
void sh_written(Shadow *s, uint64_t at, uint64_t size);
void sh_forget(Shadow *s, const char *name);
uint64_t sh_segbase(Shadow *s, uc_engine *uc, uint64_t sel);

}  // namespace cbt
