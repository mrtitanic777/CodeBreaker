#pragma once
#ifndef PPC_CONFIG_H_INCLUDED
#define PPC_CONFIG_H_INCLUDED

// The runtime's view of the recompiled code: XenonRecomp's ppc_context.h with the register settings the title was
// recompiled with, and the shared indirect-call table. The settings (which registers are locals of each function)
// change PPCContext's layout, so they are the title's own: x360.cmake copies the PPC_CONFIG_* lines of the title's
// generated ppc_config.h into x360_ppc_settings.h. No module's image constants (PPC_IMAGE_BASE, PPC_CODE_BASE...) are
// visible here: the runtime gets them per module from modules.h.

#include "x360_ppc_settings.h"
#include "ppc_lookup.h"

#endif
