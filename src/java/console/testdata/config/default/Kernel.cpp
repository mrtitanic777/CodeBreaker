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
#if defined(_XBOX) || defined(_XENON)
    XOVERLAPPED ov; memset(&ov, 0, sizeof(ov));
    DWORD r = XShowKeyboardUI(0, VKBD_DEFAULT, deflt ? deflt : L"", title ? title : L"",
                              L"", out, (DWORD)outCount, &ov);
    if (r != ERROR_IO_PENDING) return false;
    while (!XHasOverlappedIoCompleted(&ov)) { /* pump: */ }
    DWORD ext = 0; return XGetOverlappedResult(&ov, &ext, TRUE) == ERROR_SUCCESS;
#else
    if (out && outCount > 0) { if (deflt) wcsncpy(out, deflt, outCount); else out[0]=0; }
    return true;
#endif
}

// ---- save / load storage ---------------------------------------------
#if defined(_XBOX) || defined(_XENON)
static DWORD g_deviceId = 0;
#endif
bool Kernel_SelectDevice()
{
#if defined(_XBOX) || defined(_XENON)
    XOVERLAPPED ov; memset(&ov, 0, sizeof(ov));
    ULARGE_INTEGER needed; needed.QuadPart = 8ULL * 1024 * 1024;   // space to reserve for the save
    DWORD r = XShowDeviceSelectorUI(0, XCONTENTTYPE_SAVEDGAME, XCONTENTFLAG_MANAGESTORAGE,
                                    needed, &g_deviceId, &ov);
    if (r != ERROR_IO_PENDING) return false;
    while (!XHasOverlappedIoCompleted(&ov)) { }
    DWORD ext = 0; return XGetOverlappedResult(&ov, &ext, TRUE) == ERROR_SUCCESS;
#else
    return true;
#endif
}

bool Kernel_OpenSave(const char* rootName)
{
#if defined(_XBOX) || defined(_XENON)
    XCONTENT_DATA data; memset(&data, 0, sizeof(data));
    data.DeviceID = g_deviceId; data.dwContentType = XCONTENTTYPE_SAVEDGAME;
    strncpy(data.szFileName, rootName, XCONTENT_MAX_FILENAME_LENGTH - 1);
    DWORD disp = 0;
    DWORD r = XContentCreate(0, "Save", &data, XCONTENTFLAG_OPENALWAYS,
                             &disp, NULL, NULL);
    return r == ERROR_SUCCESS;
#else
    (void)rootName; return true;
#endif
}

void Kernel_CloseSave()
{
#if defined(_XBOX) || defined(_XENON)
    XContentClose("Save", NULL);
#endif
}
