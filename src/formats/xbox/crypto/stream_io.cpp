#include "formats/xbox/crypto/stream_io.hpp"

#include <algorithm>
#include <format>

#include "core/error.hpp"
#include "core/fs.hpp"

namespace cb::formats::xbox::crypto {

StreamIO StreamIO::open(const std::filesystem::path& path, Endian endian) {
    StreamIO s(endian);
    std::error_code ec;
    const bool exists = std::filesystem::exists(path, ec);
    s.file_.emplace(path, exists ? fs::FileMode::ReadWrite : fs::FileMode::Create);
    return s;
}

std::uint64_t StreamIO::length() const { return file_ ? file_->size() : memory_.size(); }

Bytes StreamIO::contents() const { return read_at(0, static_cast<std::size_t>(length())); }

std::uint64_t StreamIO::seek(std::int64_t offset, Whence whence) {
    const std::int64_t base = whence == Whence::Set       ? 0
                              : whence == Whence::Current ? static_cast<std::int64_t>(position_)
                                                          : static_cast<std::int64_t>(length());
    if (offset < -base) throw UsageError(std::format("StreamIO: a seek to {} is before the start", base + offset));
    position_ = static_cast<std::uint64_t>(base + offset);
    return position_;
}

std::uint64_t StreamIO::label(const std::string& name) const {
    const auto it = labels_.find(name);
    if (it == labels_.end()) throw UsageError(std::format("StreamIO: no position is named '{}'", name));
    return it->second;
}

std::uint64_t StreamIO::goto_label(const std::string& name, std::int64_t delta) {
    return seek(static_cast<std::int64_t>(label(name)) + delta);
}

Bytes StreamIO::read_at(std::uint64_t offset, std::size_t count) const {
    if (file_) return file_->read_at(offset, count);
    if (offset >= memory_.size()) return {};
    const auto n = static_cast<std::ptrdiff_t>(std::min<std::uint64_t>(count, memory_.size() - offset));
    const auto from = static_cast<std::ptrdiff_t>(offset);
    return Bytes(memory_.begin() + from, memory_.begin() + from + n);
}

Bytes StreamIO::read(std::size_t count) {
    Bytes out = read_at(position_, count);
    position_ += out.size();
    return out;
}

Bytes StreamIO::read_exact(std::size_t count) {
    const std::uint64_t at = position_;
    Bytes out = read(count);
    if (out.size() < count) throw TruncatedError("StreamIO", at, count, out.size());
    return out;
}

void StreamIO::write(ByteView data) {
    if (file_) {
        file_->write_at(position_, data);
    } else {
        const std::uint64_t end = position_ + data.size();
        if (end > memory_.size()) memory_.resize(static_cast<std::size_t>(end), 0);   // (zero-filled between)
        std::copy(data.begin(), data.end(), memory_.begin() + static_cast<std::ptrdiff_t>(position_));
    }
    position_ += data.size();
}

void StreamIO::write_at(std::uint64_t offset, ByteView data) {
    at_offset(offset, [&] { write(data); return 0; });
}

Bytes StreamIO::perform_function_at(std::uint64_t offset, std::size_t size, const std::function<Bytes(ByteView)>& function) {
    const Bytes in = read_at(offset, size);
    Bytes out = function(in);
    write_at(offset, out);
    return out;
}

}  // namespace cb::formats::xbox::crypto
