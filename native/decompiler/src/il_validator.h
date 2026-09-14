// XEXMagic - IL Validator
// Validates IL for structural integrity, SSA correctness, and completeness.

#pragma once

#include "il.h"

struct ILValidationResult {
  bool valid = true;
  std::vector<std::string> errors;
  std::vector<std::string> warnings;
  int total_instructions = 0;
  int inline_asm_count = 0;
  float coverage_pct = 100.0f;
};

// Validate an IL function. Populates func.validation_* fields.
ILValidationResult ValidateILFunction(ILFunction& func);
