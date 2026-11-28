// sdk/module_folder.cpp -- the folder the SDK's library is loaded from (cbsdk.dll; the test program when it is linked in).
#include "sdk/module_folder.hpp"

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#include "core/fs.hpp"

namespace cb::sdk_impl {

std::filesystem::path module_folder() {
#ifdef _WIN32
    HMODULE self = nullptr;
    if (!GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                            reinterpret_cast<LPCWSTR>(&module_folder), &self))
        return fs::executable_path().parent_path();
    std::wstring name(MAX_PATH, L'\0');
    for (;;) {
        const DWORD n = GetModuleFileNameW(self, name.data(), static_cast<DWORD>(name.size()));
        if (n == 0) return fs::executable_path().parent_path();
        if (n < name.size()) {
            name.resize(n);
            break;
        }
        name.resize(name.size() * 2);
    }
    return std::filesystem::path(name).parent_path();
#else
    Dl_info info{};
    if (dladdr(reinterpret_cast<void*>(&module_folder), &info) == 0 || info.dli_fname == nullptr) return fs::executable_path().parent_path();
    return std::filesystem::absolute(info.dli_fname).parent_path();
#endif
}

}  // namespace cb::sdk_impl
