// codebreaker/emulate/pcode.cpp -- typed C++ of codebreaker.emulate.pcode (see pcode.hpp).

#include "codebreaker/emulate/pcode.hpp"

#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#include <map>
#include <mutex>
#include <string>

#include "codebreaker/base/cache.hpp"
#include "py_settings.hpp"
#include "py_stdlib.hpp"
#include "py_subprocess.hpp"

// decompile/decompiler.hpp is declared here by the one function this file calls (decompiler._languages()): the header
// itself reaches the whole decompiler.
namespace codebreaker::decompile::decompiler {
py::str _languages();
}

namespace codebreaker::emulate::pcode {

using std::string;

// ---- Lifted ---------------------------------------------------------------------------------------------------------
// One instruction's p-code, and what the shadow needs to know of it without running it: the register bytes it reads and
// writes, whether it reaches memory, a user operation or a branch (anything that must be followed op by op).
Lifted::Lifted(int64_t length_, const py::list<Op>& ops_) : length(length_), ops(ops_) {
    for (const Op& op : ops_) {
        const string& opc = op.opcode.utf8();
        if (opc == "LOAD" || opc == "STORE" || opc == "CALLOTHER" || opc == "CBRANCH" || opc == "BRANCHIND" || opc == "CALLIND" || opc == "RETURN")
            full = true;
        for (const Input& v : op.inputs) {
            if (const Varnode* vn = std::get_if<Varnode>(&v)) {
                if (vn->space == "r") {
                    for (int64_t k = vn->offset; k < vn->offset + vn->size; k++) reads.add(k);
                } else if (vn->space == "m") {
                    full = true;
                }
            }
        }
        if (op.out) {
            if (op.out->space == "r") {
                for (int64_t k = op.out->offset; k < op.out->offset + op.out->size; k++) writes.add(k);
            } else if (op.out->space == "m") {
                full = true;
            }
        }
    }
}

namespace {

// _varnode(text, window): None for "-", the name for "@name", else (space, offset, size)
Input varnode(const string& text, bool window) {
    if (text == "-") return py::none_t{};
    if (!text.empty() && text[0] == '@') return py::str(text.substr(1));
    size_t a = text.find(':');
    size_t b = a == string::npos ? string::npos : text.find(':', a + 1);
    if (a == string::npos || b == string::npos || text.find(':', b + 1) != string::npos) {
        throw py::ValueError("not enough values to unpack (expected 3)");
    }
    Varnode v;
    v.space = py::str(text.substr(0, a));
    v.offset = py::int_parse(py::str(text.substr(a + 1, b - a - 1)), 16);
    if (v.space == "m" && window) v.offset &= 0xFFFF;
    v.size = py::int_parse(py::str(text.substr(b + 1)), 10);
    return v;
}
std::vector<string> split_on(const string& s, char sep) {
    std::vector<string> out;
    size_t at = 0;
    while (true) {
        size_t k = s.find(sep, at);
        out.push_back(s.substr(at, k == string::npos ? string::npos : k - at));
        if (k == string::npos) break;
        at = k + 1;
    }
    return out;
}
std::wstring wide(const string& u8) {
    int n = MultiByteToWideChar(CP_UTF8, 0, u8.c_str(), -1, nullptr, 0);
    std::wstring w(static_cast<size_t>(n > 0 ? n - 1 : 0), L'\0');
    if (n > 0) MultiByteToWideChar(CP_UTF8, 0, u8.c_str(), -1, &w[0], n);
    return w;
}

}  // namespace

// ---- Lifter ---------------------------------------------------------------------------------------------------------
struct Lifter::Impl {
    HANDLE process = nullptr, to_child = nullptr, from_child = nullptr;
    string pending;
    std::mutex lock;
    std::map<std::pair<string, int64_t>, std::shared_ptr<const Lifted>> memo;   // null: it does not decode

    ~Impl() {
        if (to_child) CloseHandle(to_child);
        if (from_child) CloseHandle(from_child);
        if (process) {
            TerminateProcess(process, 1);   // (the program this Lifter started)
            CloseHandle(process);
        }
    }
    void write_line(const string& text) {   // (text mode on Windows: "\n" goes out as "\r\n")
        string out = text + "\r\n";
        DWORD done = 0;
        if (!WriteFile(to_child, out.data(), static_cast<DWORD>(out.size()), &done, nullptr)) throw py::OSError("[Errno 22] Invalid argument");
    }
    // readline(): up to and including "\n" (universal newlines: "\r\n" and "\r" are "\n"); "" at the end of the stream
    string read_line() {
        while (true) {
            size_t k = pending.find('\n');
            if (k != string::npos) {
                string line = pending.substr(0, k + 1);
                pending.erase(0, k + 1);
                if (line.size() >= 2 && line[line.size() - 2] == '\r') line.erase(line.size() - 2, 1);
                return line;
            }
            char buf[4096];
            DWORD got = 0;
            if (!ReadFile(from_child, buf, sizeof buf, &got, nullptr) || got == 0) {
                string rest = pending;
                pending.clear();
                if (!rest.empty() && rest.back() == '\r') rest.back() = '\n';
                return rest;
            }
            pending.append(buf, got);
        }
    }
};

Lifter::Lifter(const py::str& exe, const py::str& sla, const py::str& mode_) : mode(mode_), impl_(new Impl()) {
    window = mode == "x86-16-real";
    SECURITY_ATTRIBUTES sa{sizeof sa, nullptr, TRUE};
    HANDLE in_r = nullptr, in_w = nullptr, out_r = nullptr, out_w = nullptr;
    if (!CreatePipe(&in_r, &in_w, &sa, 0) || !CreatePipe(&out_r, &out_w, &sa, 0)) throw py::OSError("[WinError 6] The handle is invalid");
    SetHandleInformation(in_w, HANDLE_FLAG_INHERIT, 0);
    SetHandleInformation(out_r, HANDLE_FLAG_INHERIT, 0);
    HANDLE nul = CreateFileW(L"NUL", GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, &sa, OPEN_EXISTING, 0, nullptr);
    STARTUPINFOW si{};
    si.cb = sizeof si;
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = in_r;
    si.hStdOutput = out_w;
    si.hStdError = nul;
    string cmd = py::subprocess::list2cmdline(py::list<py::str>{exe, sla, py::str(mode.utf8() + "-lift")}).utf8();
    std::wstring wcmd = wide(cmd);
    PROCESS_INFORMATION pi{};
    BOOL ok = CreateProcessW(nullptr, &wcmd[0], nullptr, nullptr, TRUE, 0, nullptr, nullptr, &si, &pi);
    DWORD err = ok ? 0 : GetLastError();
    CloseHandle(in_r);
    CloseHandle(out_w);
    if (nul != INVALID_HANDLE_VALUE) CloseHandle(nul);
    if (!ok) {
        CloseHandle(in_w);
        CloseHandle(out_r);
        py::raise_winerror(err);
    }
    CloseHandle(pi.hThread);
    impl_->process = pi.hProcess;
    impl_->to_child = in_w;
    impl_->from_child = out_r;
    string head_line = impl_->read_line();
    std::vector<string> head;   // .split()
    {
        size_t i = 0;
        while (i < head_line.size()) {
            while (i < head_line.size() && std::isspace(static_cast<unsigned char>(head_line[i]))) i++;
            size_t j = i;
            while (j < head_line.size() && !std::isspace(static_cast<unsigned char>(head_line[j]))) j++;
            if (j > i) head.push_back(head_line.substr(i, j - i));
            i = j;
        }
    }
    if (head.empty() || head[0] != "regs") {
        TerminateProcess(impl_->process, 1);
        throw py::OSError("not a lifting pcodecheck");
    }
    for (size_t k = 1; k < head.size(); k++) {   // name -> (offset, size)
        std::vector<string> p = split_on(head[k], ':');
        if (p.size() != 3) throw py::ValueError("not enough values to unpack (expected 3)");
        regs.set(py::str(p[0]), std::make_tuple(py::int_parse(py::str(p[1]), 16), py::int_parse(py::str(p[2]), 10)));
    }
}

Lifter::~Lifter() = default;

// pcodecheck's line for the instruction at the start of `code` at `offset` (the C++ shadow parses it, and keeps it for the
// process: emulate/native_shadow.py).
py::str Lifter::line(const py::bytes& code, int64_t offset) {
    static const char* hexd = "0123456789abcdef";
    string hex;
    for (unsigned char c : code.raw()) {
        hex.push_back(hexd[c >> 4]);
        hex.push_back(hexd[c & 15]);
    }
    char off[32];
    snprintf(off, sizeof off, "%llx", static_cast<unsigned long long>(offset));
    std::lock_guard<std::mutex> hold(impl_->lock);
    impl_->write_line(hex + " " + off);
    string got = impl_->read_line();
    while (!got.empty() && got.back() == '\n') got.pop_back();   // .rstrip("\n")
    return py::str(got);
}

// The p-code of the instruction at the start of `code` (bytes; 16 are enough for x86), at `offset`. -> Lifted, or None when it
// does not decode.
std::shared_ptr<const Lifted> Lifter::lift(const py::bytes& code, int64_t offset) {
    auto key = std::make_pair(code.raw(), offset);
    {
        std::lock_guard<std::mutex> hold(impl_->lock);
        auto it = impl_->memo.find(key);
        if (it != impl_->memo.end()) return it->second;
    }
    string line = this->line(code, offset).utf8();
    std::shared_ptr<const Lifted> out;
    if (!line.empty() && line.compare(0, 3, "err") != 0) {
        size_t sp = line.find(' ');
        string length_s = sp == string::npos ? line : line.substr(0, sp);
        string body = sp == string::npos ? string() : line.substr(sp + 1);
        py::list<Op> ops;
        if (!body.empty()) {
            for (const string& text : split_on(body, ';')) {
                std::vector<string> parts = split_on(text, ' ');
                Op op;
                op.opcode = py::str(parts.at(0));
                Input o = varnode(parts.at(1), window);
                if (const Varnode* vn = std::get_if<Varnode>(&o)) op.out = *vn;   // (a name is not a varnode: no out)
                for (size_t k = 2; k < parts.size(); k++) op.inputs.append(varnode(parts[k], window));
                ops.append(op);
            }
        }
        out = std::make_shared<const Lifted>(py::int_parse(py::str(length_s), 10), ops);
    }
    std::lock_guard<std::mutex> hold(impl_->lock);
    impl_->memo[key] = out;
    return out;
}

// the specification each lift mode reads (native/decompiler/languages)
const py::dict<py::str, py::str> SLA = {{py::str("x86-16-real"), py::str("x86.sla")},
                                        {py::str("x86-32-flat"), py::str("x86.sla")},
                                        {py::str("ppc-be-64"), py::str("ppc_64_xenon_be.sla")}};

// CB_PCODECHECK, else decompiler_bin/pcodecheck.exe
py::str _exe() {
    auto exe = py::os::getenv(py::str("CB_PCODECHECK"));
    if (exe && !exe->empty()) return *exe;
    auto bin = py::settings::get_opt_str(py::str("decompiler_bin"));
    return py::os::path::join(bin ? *bin : py::str(""), py::str("pcodecheck.exe"));
}

namespace {
std::map<string, std::shared_ptr<Lifter>>& lifters() {
    static std::map<string, std::shared_ptr<Lifter>> m;
    return m;
}
}  // namespace

// The process's lifter for `mode`, started on first use; null when there is none to start (no program, or one without the lift
// mode).
std::shared_ptr<Lifter> lifter(const py::str& mode) {
    string key = mode.utf8();
    auto it = lifters().find(key);
    if (it == lifters().end()) {
        py::str exe = _exe();
        if (!SLA.contains(mode)) throw py::KeyError(repr(mode).utf8());
        py::str sla = py::os::path::join(decompile::decompiler::_languages(), SLA.at(mode));
        std::shared_ptr<Lifter> made;
        try {
            if (py::os::path::exists(exe) && py::os::path::exists(sla)) made = std::make_shared<Lifter>(exe, sla, mode);
        } catch (const py::OSError&) {
            made = nullptr;
        } catch (const py::ValueError&) {
            made = nullptr;
        }
        it = lifters().emplace(key, made).first;
    }
    return it->second;
}

// What the lifted p-code depends on (for keys of anything kept from it): the program and the specification (base/cache.py tool).
std::tuple<py::str, py::str> stamp(const py::str& mode) {
    if (!SLA.contains(mode)) throw py::KeyError(repr(mode).utf8());
    return {base::cache::tool(_exe()), base::cache::tool(py::os::path::join(decompile::decompiler::_languages(), SLA.at(mode)))};
}

}  // namespace codebreaker::emulate::pcode
