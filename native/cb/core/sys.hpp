// cb/core/sys.hpp -- the operating system's small services, one interface: the standard streams and the console, the local
// time, the machine's memory and cores, the text of an OS error code. Windows and POSIX implementations (sys_win.cpp,
// sys_posix.cpp). Processes are cb/core/process.hpp, files fs.hpp, locks file_lock.hpp, libraries dynlib.hpp, sockets
// socket.hpp.
#pragma once

#include <cstdint>
#include <ctime>
#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <string_view>

namespace cb::sys {

enum class Stream { In, Out, Err };

// Put the console in UTF-8 for input and output (Windows: code page 65001; elsewhere it already is). Called once at startup.
void use_utf8_console();
// Whether our standard stream is a terminal (an interactive console) and not a file or a pipe.
bool is_terminal(Stream which);
// Write bytes to our standard output or error, unbuffered, to whatever the stream is now (a test may have redirected it);
// false if they could not all be written.
bool write_stream(Stream which, std::string_view data);

// Redirect our standard output or error to a file (truncated) for the life of the object, for the C and C++ libraries'
// writers and for children that inherit the stream; the stream is restored afterwards.
class StreamRedirect {
public:
    StreamRedirect(Stream which, const std::filesystem::path& file);
    StreamRedirect(StreamRedirect&&) noexcept;
    StreamRedirect(const StreamRedirect&) = delete;
    StreamRedirect& operator=(const StreamRedirect&) = delete;
    StreamRedirect& operator=(StreamRedirect&&) = delete;
    ~StreamRedirect();

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

struct LocalTime {
    int year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0, millisecond = 0;
};
// The wall clock now, in the machine's time zone.
LocalTime local_time();

// Memory the machine can give new work now, in GiB (physical, and what the paging limit leaves); -1 if unknown.
double available_memory_gb();
// Lower this process's scheduling priority (background work); false if the system refused.
bool lower_priority();
// Restrict this process to the first `cores` processors; false if the system refused or does not allow it.
bool limit_cores(int cores);
// Physical memory installed, in GiB.
std::optional<double> total_memory_gb();
// The system's processor time since boot, in one unit: the part spent idle and the whole (an interval's idle share is the
// difference of two samples); false if the system will not say.
bool cpu_times(std::uint64_t& idle, std::uint64_t& total);
// Lower the calling thread's priority only (a worker of a pool), not the process's.
void lower_thread_priority();
// A time of the clock as the machine's time zone writes it (localtime_s / localtime_r).
std::tm local_tm(std::time_t when);

// The text of an operating-system error code, without a trailing newline (Windows: GetLastError codes; elsewhere: errno).
std::string error_message(std::int64_t code);
// The calling thread's last operating-system error code (GetLastError; errno).
std::int64_t last_error();

}  // namespace cb::sys
