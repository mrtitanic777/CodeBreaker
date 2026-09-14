// XEXMagic - Xenon PPE Cycle Counter
// Estimates instruction cycle counts based on the Xbox 360 Xenon PPE core.
// Reference: IBM Cell Broadband Engine Programming Handbook v1.1 (2007)
// Single-issue, in-order, L1 cache hit assumption.

#pragma once

#include "il.h"

// Get estimated cycle count for a single IL instruction.
int GetXenonCycles(const ILInstr& instr);

// Annotate all instructions in a function with cycle counts,
// compute per-block totals and function summary.
void ComputeFunctionCycles(ILFunction& func);
