#include "stdafx.h"
#include <xtl.h>
#include <xbox.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "Save360.h"

#ifndef snprintf
#define snprintf _snprintf
#endif

// ------------------------------------------------------------
// DEBUG
// ------------------------------------------------------------

static void Save360DbgPrint(const char* fmt, ...)
{
    char buf[1024];
    va_list ap;
    va_start(ap, fmt);
    _vsnprintf(buf, sizeof(buf) - 1, fmt, ap);
    va_end(ap);
    buf[sizeof(buf) - 1] = 0;
    OutputDebugStringA(buf);
    OutputDebugStringA("\n");
}

// ------------------------------------------------------------
// CONFIG
// ------------------------------------------------------------

static const char* kMountName = "Save";
static const char* kContainerName = "LOGANCRAFT";

// ------------------------------------------------------------
// STATE
// ------------------------------------------------------------

static volatile LONG g_mountState = 0;
static DWORD g_titleId = 0;
static DWORD g_userIndex = 0;

static char g_baseDir[260];
static char g_lastError[128] = "INIT NOT CALLED";

static Save360PresentFn g_presentFn = NULL;

void Save360_SetPresentCallback(Save360PresentFn fn) { g_presentFn = fn; }

// Call during waits so guide UI can render
static void pump(void)
{
    if (g_presentFn) g_presentFn();
    Sleep(16);
}

// ------------------------------------------------------------
// HELPERS
// ------------------------------------------------------------

static void safe_strcpy(char* dst, size_t dstSz, const char* src)
{
    if (!dst || dstSz == 0) return;
    if (!src) { dst[0] = 0; return; }
    strncpy(dst, src, dstSz - 1);
    dst[dstSz - 1] = 0;
}

static void rom_basename_no_ext(char* out, size_t outSz, const char* romFullPath)
{
    const char* p;
    char tmp[260];
    char* dot;

    if (!out || outSz == 0) return;
    out[0] = 0;
    if (!romFullPath || !romFullPath[0]) return;

    p = strrchr(romFullPath, '\\');
    if (p) p++; else p = romFullPath;
    safe_strcpy(tmp, sizeof(tmp), p);

    dot = strrchr(tmp, '.');
    if (dot) *dot = 0;

    for (size_t i = 0; tmp[i]; i++) {
        char c = tmp[i];
        if (c == ':' || c == '*' || c == '?' || c == '"' ||
            c == '<' || c == '>' || c == '|' || c == '/')
            tmp[i] = '_';
    }
    safe_strcpy(out, outSz, tmp);
}

static int test_writable_file(const char* dir)
{
    char p[260];
    snprintf(p, sizeof(p), "%s_wtest", dir);

    FILE* f = fopen(p, "wb");
    if (!f) return 0;
    fclose(f);
    remove(p);
    return 1;
}

// ------------------------------------------------------------
// Step 1: Ensure a profile is signed in
// ------------------------------------------------------------

static int ensure_signed_in(DWORD userIndex)
{
    XUSER_SIGNIN_STATE state = XUserGetSigninState(userIndex);
    Save360DbgPrint("[Save360] SigninState(%lu)=%d", (unsigned long)userIndex, (int)state);

    if (state != eXUserSigninState_NotSignedIn) {
        snprintf(g_lastError, sizeof(g_lastError), "SIGNIN state=%d", (int)state);
        return 1;
    }

    // Show signin UI
    snprintf(g_lastError, sizeof(g_lastError), "SIGNIN UI...");
    DWORD rc = XShowSigninUI(1, 0);
    Save360DbgPrint("[Save360] XShowSigninUI rc=%lu", (unsigned long)rc);

    if (rc != ERROR_SUCCESS && rc != ERROR_IO_PENDING) {
        snprintf(g_lastError, sizeof(g_lastError), "SIGNIN UI FAIL rc=%lu", (unsigned long)rc);
        return 0;
    }

    // Poll until signed in or timeout (~30s)
    for (int i = 0; i < 1800; i++) {
        pump();
        state = XUserGetSigninState(userIndex);
        if (state != eXUserSigninState_NotSignedIn) {
            snprintf(g_lastError, sizeof(g_lastError), "SIGNIN OK state=%d", (int)state);
            return 1;
        }
    }

    snprintf(g_lastError, sizeof(g_lastError), "SIGNIN TIMEOUT");
    return 0;
}

// ------------------------------------------------------------
// Step 2: Pick a storage device
// ------------------------------------------------------------

static int pick_device(DWORD userIndex, XCONTENTDEVICEID* outDeviceId)
{
    if (!outDeviceId) return 0;
    *outDeviceId = 0;

    XOVERLAPPED ov;
    memset(&ov, 0, sizeof(ov));

    ULARGE_INTEGER uli;
    uli.QuadPart = 512ULL * 1024ULL;

    DWORD rc = XShowDeviceSelectorUI(
        userIndex,
        XCONTENTTYPE_SAVEDGAME,
        0,
        uli,
        outDeviceId,
        &ov
    );

    Save360DbgPrint("[Save360] XShowDeviceSelectorUI rc=%lu", (unsigned long)rc);

    if (rc == ERROR_IO_PENDING) {
        // Wait for guide UI to complete
        while (!XHasOverlappedIoCompleted(&ov)) {
            pump();
        }
        DWORD orc = XGetOverlappedResult(&ov, NULL, FALSE);
        Save360DbgPrint("[Save360] DevSel result=%lu devId=%I64u", (unsigned long)orc, (unsigned long long)*outDeviceId);
        if (orc != ERROR_SUCCESS) {
            snprintf(g_lastError, sizeof(g_lastError), "DEVSEL orc=%lu", (unsigned long)orc);
            return 0;
        }
    } else if (rc != ERROR_SUCCESS) {
        snprintf(g_lastError, sizeof(g_lastError), "DEVSEL rc=%lu", (unsigned long)rc);
        return 0;
    }

    if (*outDeviceId == 0) {
        snprintf(g_lastError, sizeof(g_lastError), "DEVID=0");
        return 0;
    }

    return 1;
}

// ------------------------------------------------------------
// PUBLIC API
// ------------------------------------------------------------

int Save360_Init(DWORD titleId, DWORD userIndex)
{
    Save360DbgPrint("[Save360] Init title=%08X user=%lu", (unsigned)titleId, (unsigned long)userIndex);
    if (g_mountState == 2) return 1;

    if (InterlockedCompareExchange(&g_mountState, 1, 0) != 0) {
        for (int i = 0; i < 3000; i++) {
            if (g_mountState != 1) break;
            Sleep(10);
        }
        return (g_mountState == 2) ? 1 : 0;
    }

    g_titleId = titleId;
    g_userIndex = userIndex;
    g_baseDir[0] = 0;

    // 1) Ensure signed in
    if (!ensure_signed_in(g_userIndex)) {
        g_mountState = 0;
        return 0;
    }

    // 2) Pick storage device
    XCONTENTDEVICEID deviceId = 0;
    if (!pick_device(g_userIndex, &deviceId)) {
        g_mountState = 0;
        return 0;
    }

    snprintf(g_lastError, sizeof(g_lastError), "DID=%I64u", (unsigned long long)deviceId);

    // 3) Create/open content container
    XCONTENT_CROSS_TITLE_DATA cd;
    memset(&cd, 0, sizeof(cd));
    cd.dwContentType = XCONTENTTYPE_SAVEDGAME;
    cd.DeviceID = deviceId;
    cd.dwTitleId = g_titleId;
    wcsncpy(cd.szDisplayName, L"LoganCraft Saves", XCONTENT_MAX_DISPLAYNAME_LENGTH - 1);
    cd.szDisplayName[XCONTENT_MAX_DISPLAYNAME_LENGTH - 1] = 0;
    safe_strcpy(cd.szFileName, sizeof(cd.szFileName), kContainerName);

    DWORD disposition = 0;
    DWORD licenseMask = 0;

#ifdef XCONTENTFLAG_OPENALWAYS
    DWORD flags = XCONTENTFLAG_OPENALWAYS;
#else
    DWORD flags = 4;
#endif

    DWORD rc = XContentCreate(
        g_userIndex,
        kMountName,
        (PXCONTENT_DATA)&cd,
        flags,
        &disposition,
        &licenseMask,
        NULL
    );

    Save360DbgPrint("[Save360] XContentCreate rc=%lu disp=%lu", (unsigned long)rc, (unsigned long)disposition);

    if (rc == ERROR_ALREADY_EXISTS)
        rc = ERROR_SUCCESS;

    if (rc != ERROR_SUCCESS) {
        snprintf(g_lastError, sizeof(g_lastError), "XCREATE rc=%lu did=%I64u", (unsigned long)rc, (unsigned long long)deviceId);
        g_mountState = 0;
        return 0;
    }

    // 4) Mount root - no trailing backslash
    {
        char dirMount[64];
        snprintf(dirMount, sizeof(dirMount), "%s:", kMountName);
        safe_strcpy(g_baseDir, sizeof(g_baseDir), dirMount);
    }

    // 5) Verify writable
    if (!test_writable_file(g_baseDir)) {
        // Try with backslash
        char dirMount2[64];
        snprintf(dirMount2, sizeof(dirMount2), "%s:\\", kMountName);
        safe_strcpy(g_baseDir, sizeof(g_baseDir), dirMount2);
        if (!test_writable_file(g_baseDir)) {
            snprintf(g_lastError, sizeof(g_lastError), "WRITE FAIL %s", g_baseDir);
            g_baseDir[0] = 0;
            g_mountState = 0;
            return 0;
        }
    }

    snprintf(g_lastError, sizeof(g_lastError), "OK %s", g_baseDir);
    g_mountState = 2;
    return 1;
}

void Save360_Flush(void)
{
    if (g_mountState == 2) {
        XContentFlush(kMountName, NULL);
        Save360DbgPrint("[Save360] Flushed");
    }
}

void Save360_Shutdown(void)
{
    if (g_mountState == 2) {
        /* Flush then close - szRootName is the mount name without colon */
        XContentFlush(kMountName, NULL);
        XContentClose(kMountName, NULL);
    }
    g_mountState = 0;
    g_baseDir[0] = 0;
}

int Save360_BuildPath(const char* romFullPath, char* outPath, size_t outPathSz)
{
    if (!outPath || outPathSz == 0) return 0;
    outPath[0] = 0;
    if (g_mountState != 2 || !g_baseDir[0]) return 0;

    char romName[260];
    rom_basename_no_ext(romName, sizeof(romName), romFullPath);
    if (!romName[0]) safe_strcpy(romName, sizeof(romName), "NONAME");

    // If baseDir ends with : or \, just append filename directly
    size_t blen = strlen(g_baseDir);
    char lastCh = (blen > 0) ? g_baseDir[blen - 1] : 0;
    if (lastCh == '\\' || lastCh == ':')
        snprintf(outPath, outPathSz, "%s%s.sav", g_baseDir, romName);
    else
        snprintf(outPath, outPathSz, "%s\\%s.sav", g_baseDir, romName);
    return 1;
}

const char* Save360_GetMountName(void) { return kMountName; }
const char* Save360_GetBaseDir(void) { return g_baseDir; }
const char* Save360_GetLastError(void) { return g_lastError; }
