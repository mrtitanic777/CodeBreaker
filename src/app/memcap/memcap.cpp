// memcap.exe <MB> <program> [args...]
// Runs a program inside a Windows Job Object whose per-process and whole-job commit limit is <MB>,
// with KILL_ON_JOB_CLOSE, stdio inherited. Allocation failures in the child then surface as
// MemoryError / std::bad_alloc instead of exhausting the machine.
// Exit code: the child's; 97 when the limit was reached and the child then failed. The stderr line
// "memcap: limit <MB> MB reached" is printed whenever the limit was hit, even if the child coped
// (caught MemoryError / bad_alloc and finished cleanly keeps its own exit code).
// 96 for usage/launch errors. MEMCAP_REPORT=1 prints the peak commit and the child's exit code.
// Build: cl /nologo /O1 /EHsc memcap.cpp /Fe:memcap.exe
#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <string>

static std::string quote(const char* a) {
    std::string s(a);
    if (!s.empty() && s.find_first_of(" \t\"") == std::string::npos) return s;
    std::string r = "\"";
    size_t bs = 0;
    for (char c : s) {
        if (c == '\\') { ++bs; continue; }
        if (c == '"') { r.append(bs * 2 + 1, '\\'); r += '"'; }
        else { r.append(bs, '\\'); r += c; }
        bs = 0;
    }
    r.append(bs * 2, '\\');
    return r + "\"";
}

int main(int argc, char** argv) {
    if (argc < 3) { std::fprintf(stderr, "usage: memcap <MB> <program> [args...]\n"); return 96; }
    const unsigned long long mb = std::strtoull(argv[1], nullptr, 10);
    if (mb == 0) { std::fprintf(stderr, "memcap: bad limit\n"); return 96; }
    const SIZE_T bytes = static_cast<SIZE_T>(mb) << 20;

    HANDLE job = CreateJobObjectA(nullptr, nullptr);
    if (!job) { std::fprintf(stderr, "memcap: CreateJobObject failed\n"); return 96; }
    JOBOBJECT_EXTENDED_LIMIT_INFORMATION li = {};
    li.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_PROCESS_MEMORY | JOB_OBJECT_LIMIT_JOB_MEMORY | JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
    li.ProcessMemoryLimit = bytes;
    li.JobMemoryLimit = bytes;
    if (!SetInformationJobObject(job, JobObjectExtendedLimitInformation, &li, sizeof li)) {
        std::fprintf(stderr, "memcap: SetInformationJobObject failed (%lu)\n", GetLastError()); return 96;
    }

    // limit-exceeded notifications arrive on a completion port the moment a commit is refused
    HANDLE port = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 1);
    JOBOBJECT_ASSOCIATE_COMPLETION_PORT ap = {};
    ap.CompletionKey = job;
    ap.CompletionPort = port;
    SetInformationJobObject(job, JobObjectAssociateCompletionPortInformation, &ap, sizeof ap);

    std::string cmd;
    for (int i = 2; i < argc; ++i) { if (i > 2) cmd += ' '; cmd += quote(argv[i]); }
    STARTUPINFOA si = {}; si.cb = sizeof si;
    PROCESS_INFORMATION pi = {};
    // suspended, so the child is in the job before it runs a single instruction
    if (!CreateProcessA(nullptr, &cmd[0], nullptr, nullptr, TRUE, CREATE_SUSPENDED | CREATE_BREAKAWAY_FROM_JOB, nullptr, nullptr, &si, &pi) &&
        !CreateProcessA(nullptr, &cmd[0], nullptr, nullptr, TRUE, CREATE_SUSPENDED, nullptr, nullptr, &si, &pi)) {
        std::fprintf(stderr, "memcap: cannot start %s (%lu)\n", argv[2], GetLastError()); return 96;
    }
    if (!AssignProcessToJobObject(job, pi.hProcess)) {
        std::fprintf(stderr, "memcap: AssignProcessToJobObject failed (%lu)\n", GetLastError());
        TerminateProcess(pi.hProcess, 96); return 96;
    }
    ResumeThread(pi.hThread);
    WaitForSingleObject(pi.hProcess, INFINITE);
    DWORD code = 0;
    GetExitCodeProcess(pi.hProcess, &code);

    JOBOBJECT_EXTENDED_LIMIT_INFORMATION q = {};
    QueryInformationJobObject(job, JobObjectExtendedLimitInformation, &q, sizeof q, nullptr);
    bool hit = false;
    DWORD msg; ULONG_PTR key; LPOVERLAPPED ov;
    while (GetQueuedCompletionStatus(port, &msg, &key, &ov, 0))
        if (msg == JOB_OBJECT_MSG_JOB_MEMORY_LIMIT || msg == JOB_OBJECT_MSG_PROCESS_MEMORY_LIMIT) hit = true;
    if (std::getenv("MEMCAP_REPORT"))
        std::fprintf(stderr, "memcap: peak %llu MB, child exit %lu\n", (unsigned long long)(q.PeakJobMemoryUsed >> 20), static_cast<unsigned long>(code));
    CloseHandle(pi.hProcess); CloseHandle(pi.hThread); CloseHandle(job);
    if (hit) {
        std::fprintf(stderr, "memcap: limit %llu MB reached\n", mb);
        // a child that handled the refusal (MemoryError / bad_alloc caught) and finished cleanly keeps its own code
        if (code != 0) return 97;
    }
    return static_cast<int>(code);
}
