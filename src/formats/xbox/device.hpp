// formats/xbox/device.hpp -- a source of bytes for the Xbox disc readers: a disc image file, a Games-on-Demand package's parts, a
// compressed (CSO / CCI) image. The XDVDFS reader (gdf.hpp) walks whichever it is given.
//
// A device is read at offsets and answers short at its end (and with nothing past it), as a file does; it is not safe to use from two
// threads at once.
#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <memory>

#include "core/bytes.hpp"
#include "core/random_file.hpp"
#include "formats/xbox/god.hpp"

namespace cb::formats::xbox {

class Device {
public:
    virtual ~Device() = default;
    // Up to `length` bytes at `offset`: fewer where the device ends, none at or past its end.
    virtual Bytes read_at(std::uint64_t offset, std::size_t length) = 0;
};

// A disc image in one file. NotFoundError / AccessError when it cannot be opened.
class FileDevice final : public Device {
public:
    explicit FileDevice(const std::filesystem::path& path) : file_(path, fs::FileMode::Read) {}
    Bytes read_at(std::uint64_t offset, std::size_t length) override { return file_.read_at(offset, length); }
    std::uint64_t size() const { return file_.size(); }

private:
    fs::RandomFile file_;
};

// The disc volume a Games-on-Demand package holds, its parts de-interleaved on demand.
class SvodDevice final : public Device {
public:
    explicit SvodDevice(god::SvodReader reader) : reader_(std::move(reader)) {}
    // The `.data` folder of a package. NotFoundError / FormatError as god::SvodReader::open.
    static std::shared_ptr<SvodDevice> open(const std::filesystem::path& data_dir) {
        return std::make_shared<SvodDevice>(god::SvodReader::open(data_dir));
    }
    Bytes read_at(std::uint64_t offset, std::size_t length) override { return reader_.read_at(offset, length); }

private:
    god::SvodReader reader_;
};

}  // namespace cb::formats::xbox
