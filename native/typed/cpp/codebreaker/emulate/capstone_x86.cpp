// codebreaker/emulate/capstone_x86.cpp -- see capstone_x86.hpp.
#include "codebreaker/emulate/capstone_x86.hpp"

#include <windows.h>

#include <map>
#include <mutex>
#include <string>

#include "codebreaker/base/capstone_dll.hpp"

namespace codebreaker::emulate::capstone_x86 {

namespace {
std::mutex g_mu;
Api g_api;
bool g_loaded = false;
std::optional<py::str> g_path;

std::wstring wide(const py::str& path) {
    std::wstring w;
    for (char32_t c : path.u32()) w.push_back(static_cast<wchar_t>(c));   // (a BMP path)
    return w;
}

// The capstone.dll: the one named with load_capstone, else the one already in the process (the recorder's, the host's),
// else the one `import capstone` would find (base/capstone_dll).
HMODULE find_module() {
    if (g_path) return LoadLibraryW(wide(*g_path).c_str());
    if (HMODULE m = GetModuleHandleW(L"capstone.dll")) return m;
    return LoadLibraryW(wide(base::capstone_dll::find()).c_str());
}
}  // namespace

void load_capstone(const py::str& path) {
    std::lock_guard<std::mutex> g(g_mu);
    g_path = path;
}

py::str dll() {
    {
        std::lock_guard<std::mutex> g(g_mu);
        if (g_path) return *g_path;
    }
    api();
    wchar_t buf[1024];
    DWORD n = GetModuleFileNameW(GetModuleHandleW(L"capstone.dll"), buf, 1024);
    if (n == 0 || n >= 1024) return base::capstone_dll::find();
    std::string u(static_cast<size_t>(WideCharToMultiByte(CP_UTF8, 0, buf, static_cast<int>(n), nullptr, 0, nullptr, nullptr)), ' ');
    WideCharToMultiByte(CP_UTF8, 0, buf, static_cast<int>(n), &u[0], static_cast<int>(u.size()), nullptr, nullptr);
    return py::str(u);
}

const Api& api() {
    std::lock_guard<std::mutex> g(g_mu);
    if (g_loaded) return g_api;
    HMODULE m = find_module();
    if (!m) throw py::ImportError("cannot load capstone.dll (error " + std::to_string(GetLastError()) + ")");
    g_api.open = reinterpret_cast<decltype(&cs_open)>(GetProcAddress(m, "cs_open"));
    g_api.option = reinterpret_cast<decltype(&cs_option)>(GetProcAddress(m, "cs_option"));
    g_api.disasm = reinterpret_cast<decltype(&cs_disasm)>(GetProcAddress(m, "cs_disasm"));
    g_api.free_ = reinterpret_cast<decltype(&cs_free)>(GetProcAddress(m, "cs_free"));
    g_api.reg_name = reinterpret_cast<decltype(&cs_reg_name)>(GetProcAddress(m, "cs_reg_name"));
    g_api.regs_access = reinterpret_cast<decltype(&cs_regs_access)>(GetProcAddress(m, "cs_regs_access"));
    if (!g_api.open || !g_api.option || !g_api.disasm || !g_api.free_ || !g_api.reg_name || !g_api.regs_access)
        throw py::ImportError("capstone.dll lacks its entry points");
    g_loaded = true;
    return g_api;
}

csh handle(int64_t bits, bool detail) {
    const Api& a = api();
    thread_local std::map<std::pair<int64_t, bool>, csh> held;
    auto key = std::make_pair(bits, detail);
    auto it = held.find(key);
    if (it != held.end()) return it->second;
    cs_mode mode = bits == 16 ? CS_MODE_16 : bits == 64 ? CS_MODE_64 : CS_MODE_32;
    csh h = 0;
    if (a.open(CS_ARCH_X86, mode, &h) != CS_ERR_OK) throw py::RuntimeError("cs_open failed");
    a.option(h, CS_OPT_DETAIL, detail ? CS_OPT_ON : CS_OPT_OFF);
    held[key] = h;
    return h;
}

}  // namespace codebreaker::emulate::capstone_x86
