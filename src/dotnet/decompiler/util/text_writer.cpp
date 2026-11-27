#include "dotnet/decompiler/util/text_writer.hpp"

#include <cerrno>

#include "core/error.hpp"

namespace cb::dotnet::decompiler::util {

StreamWriter::StreamWriter(const std::filesystem::path& path, bool utf8_bom) : path_(path) {
#ifdef _WIN32
    errno_t err = _wfopen_s(&file_, path.c_str(), L"wb");
    if (err != 0) file_ = nullptr;
#else
    file_ = std::fopen(path.c_str(), "wb");
#endif
    if (file_ == nullptr) throw IoError(path_, errno, "cannot create the file");
    if (utf8_bom) write("\xEF\xBB\xBF");   // the byte order mark of Encoding.UTF8's preamble
}

StreamWriter::~StreamWriter() {
    // A destructor must not throw: a failure that matters is thrown by close(), which callers use where upstream disposes.
    if (file_ != nullptr) std::fclose(file_);
}

void StreamWriter::write(std::string_view text) {
    if (file_ == nullptr) throw UsageError("the writer is closed");
    if (!text.empty() && std::fwrite(text.data(), 1, text.size(), file_) != text.size()) throw IoError(path_, errno, "cannot write to the file");
}

void StreamWriter::flush() {
    if (file_ != nullptr && std::fflush(file_) != 0) throw IoError(path_, errno, "cannot write to the file");
}

void StreamWriter::close() {
    if (file_ == nullptr) return;
    std::FILE* file = file_;
    file_ = nullptr;
    const bool flushed = std::fflush(file) == 0;
    const int flush_errno = errno;
    const bool closed = std::fclose(file) == 0;
    if (!flushed) throw IoError(path_, flush_errno, "cannot write to the file");
    if (!closed) throw IoError(path_, errno, "cannot write to the file");
}

}  // namespace cb::dotnet::decompiler::util
