// Kernel.cpp -- CodeBreaker-emitted 360 kernel wrappers for the chosen mediums. See KernelConfig.h.
#include "stdafx.h"
#include "KernelConfig.h"

#if defined(_XBOX) || defined(_XENON)
  #include <xtl.h>
#endif
#include <string.h>
#include <wchar.h>

// ---- text input -------------------------------------------------------
bool Kernel_ShowKeyboard(const wchar_t* title, const wchar_t* deflt, wchar_t* out, int outCount)
{
    // In-game OSK: implemented in OSK.reference (controller-driven keyboard).
    extern bool OSK_Show(const wchar_t*, const wchar_t*, wchar_t*, int);
    return OSK_Show(title, deflt, out, outCount);
}

// ---- save / load storage ---------------------------------------------
bool Kernel_SelectDevice() { return true; }   // raw path: no device selector
bool Kernel_OpenSave(const char* rootName)
{   // dev HDD path (game:\Save\); not a content container -- devkit only.
    (void)rootName; return true;
}
void Kernel_CloseSave() {}
