// KernelConfig.h -- CodeBreaker-emitted 360 kernel-call selection (edit platform.config.json + re-emit).
#ifndef J2X_KERNEL_CONFIG_H
#define J2X_KERNEL_CONFIG_H

// text-input medium: osk
#define KERNEL_KEYBOARD_OSK 1
// storage medium: rawhdd
#define KERNEL_STORAGE_RAWHDD 1

// Route ALL text entry and save/load through these -- the one seam the medium choice swaps.
bool Kernel_ShowKeyboard(const wchar_t* title, const wchar_t* deflt, wchar_t* out, int outCount);
bool Kernel_SelectDevice();               // pick + remember the storage device (once)
bool Kernel_OpenSave(const char* rootName); // mount the writable save root
void Kernel_CloseSave();

#endif // J2X_KERNEL_CONFIG_H
