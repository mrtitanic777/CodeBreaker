#pragma once
#include <xtl.h>

// STFS save-container mount (pulled from the reference port). Mounts the title's
// saved-game STFS package as a virtual drive (default mount name "Save"), after
// which standard CreateFile/fopen works on "Save:\\...".
int Save360_Init(DWORD titleId, DWORD userIndex);
void Save360_Flush(void);
void Save360_Shutdown(void);
int Save360_BuildPath(const char* romFullPath, char* outPath, size_t outPathSz);
const char* Save360_GetBaseDir(void);
const char* Save360_GetLastError(void);
const char* Save360_GetMountName(void);

typedef void (*Save360PresentFn)(void);
void Save360_SetPresentCallback(Save360PresentFn fn);
