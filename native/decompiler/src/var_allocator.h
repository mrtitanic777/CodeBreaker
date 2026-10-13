// XEXMagic - Variable Allocator
// Assigns meaningful names to SSA values based on usage patterns.
// Classification: arg, result, ExternReference, Index, Temp

#pragma once

#include "il.h"

// Assign variable names to all SSA values in a function.
void AllocateVariables(ILFunction& func);
