// Kernel.cpp -- CodeBreaker-emitted 360 kernel wrappers for the chosen mediums. See KernelConfig.h.
#include "stdafx.h"
#include "KernelConfig.h"

#if defined(_XBOX) || defined(_XENON)
  #include <xtl.h>
#endif
#include <string.h>
#include <wchar.h>

// ---- text input -------------------------------------------------------
bool Kernel_ShowKeyboard(const wchar_t*, const wchar_t* deflt, wchar_t* out, int outCount)
{   // J2X-STUB[kernel-keyboard]: no text entry -- returns the default string unchanged.
    if (out && outCount > 0) { if (deflt) wcsncpy(out, deflt, outCount); else out[0]=0; }
    return true;
}

// ---- save / load storage ---------------------------------------------
bool Kernel_SelectDevice() { return true; }
bool Kernel_OpenSave(const char*)
{   // J2X-STUB[kernel-storage]: memory-only -- worlds do NOT persist across reboots.
    return true; }
void Kernel_CloseSave() {}
