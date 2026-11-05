// files.h -- reading a file by its UTF-8 path, for the few questions that take a path (they read
// only part of a file, as their Python did).
#pragma once

#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>

namespace cbf {

// `size` bytes from `offset` (fewer at the end of the file).  False when the file cannot be read.
bool read_part(const std::string &path_utf8, uint64_t offset, uint64_t size, std::string &out);
// the whole file
bool read_file(const std::string &path_utf8, std::string &out);

// (the container readers) os.path.getsize -- false when it cannot be read; os.path.isdir;
// the names in a folder (os.listdir); a file written whole ("wb"); os.makedirs(exist_ok=True);
// os.utime(path, (t, t)); os.path.abspath / dirname / join as Python's ntpath gives them
bool file_size(const std::string &path_utf8, uint64_t &size);
bool is_dir(const std::string &path_utf8);
bool list_dir(const std::string &path_utf8, std::vector<std::string> &names);
bool write_file(const std::string &path_utf8, const std::string &data);
bool make_dirs(const std::string &path_utf8);
bool set_times(const std::string &path_utf8, double when);
std::string abspath(const std::string &path_utf8);
std::string dirname(const std::string &path_utf8);
std::string basename(const std::string &path_utf8);
std::string join(const std::string &a, const std::string &b);

// a file opened "r+b": read and written in place
class RwFile {
public:
    explicit RwFile(const std::string &path_utf8);
    ~RwFile();
    bool ok() const { return f_ != nullptr; }
    std::string read(uint64_t at, size_t n);
    void write(uint64_t at, const std::string &data);
    void close();

private:
    FILE *f_ = nullptr;
};

// the OSError Python's open() raises for a path it cannot open
[[noreturn]] void raise_open_error(const std::string &path_utf8);

}  // namespace cbf
