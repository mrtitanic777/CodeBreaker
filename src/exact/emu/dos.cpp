#include "exact/emu/dos.hpp"

#include <cctype>
#include <algorithm>
#include <chrono>
#include <format>
#include <thread>

#include "core/fs.hpp"
#include "core/text.hpp"
#ifdef _WIN32
#include "core/win.hpp"
#endif
#include "exact/emu/dosexec/cbdos.h"   // the DOS executive (exact/emu/dosexec, linked in on Windows)

namespace cb::exact::emu::dos {

namespace stdfs = std::filesystem;

namespace {

// The engine's C interface takes wide paths: UTF-16 on Windows, UTF-32 (wchar_t is 32 bits) elsewhere.
#ifdef _WIN32
std::wstring to_wide(std::string_view s) { return win::to_wide(s); }
std::string from_wide(std::wstring_view s) { return win::from_wide(s); }
#else
std::wstring to_wide(std::string_view s) {
    std::wstring out;
    for (std::size_t i = 0; i < s.size();) {
        const unsigned char c = static_cast<unsigned char>(s[i]);
        const int extra = c < 0x80 ? 0 : c >= 0xF0 ? 3 : c >= 0xE0 ? 2 : c >= 0xC0 ? 1 : 0;
        char32_t cp = extra == 0 ? c : c & (0x3Fu >> extra);
        for (int k = 1; k <= extra && i + static_cast<std::size_t>(k) < s.size(); ++k) cp = (cp << 6) | (static_cast<unsigned char>(s[i + static_cast<std::size_t>(k)]) & 0x3Fu);
        out.push_back(static_cast<wchar_t>(cp));
        i += static_cast<std::size_t>(extra) + 1;
    }
    return out;
}
std::string from_wide(std::wstring_view s) {
    std::string out;
    for (const wchar_t w : s) {
        const char32_t cp = static_cast<char32_t>(w);
        if (cp < 0x80) out.push_back(static_cast<char>(cp));
        else if (cp < 0x800) { out.push_back(static_cast<char>(0xC0 | (cp >> 6))); out.push_back(static_cast<char>(0x80 | (cp & 0x3F))); }
        else if (cp < 0x10000) { out.push_back(static_cast<char>(0xE0 | (cp >> 12))); out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F))); out.push_back(static_cast<char>(0x80 | (cp & 0x3F))); }
        else { out.push_back(static_cast<char>(0xF0 | (cp >> 18))); out.push_back(static_cast<char>(0x80 | ((cp >> 12) & 0x3F))); out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F))); out.push_back(static_cast<char>(0x80 | (cp & 0x3F))); }
    }
    return out;
}
#endif

constexpr std::uint8_t kSignatureMore = 0x4D;   // 'M'
constexpr std::uint8_t kSignatureLast = 0x5A;   // 'Z'
constexpr std::size_t kHeaderParagraph = 16;    // a header is one paragraph
constexpr std::uint16_t kErrorTrashed = 7;      // DOS: memory control blocks destroyed
constexpr std::uint16_t kErrorBadBlock = 9;     // DOS: memory block address invalid
constexpr int kMaxOpenRetries = 40;             // a file just made or closed can refuse to open for a moment on a busy machine

std::uint32_t dos_linear(std::uint16_t segment) { return static_cast<std::uint32_t>(segment) * 16u; }

std::uint16_t dos_word(const Bytes& b, std::size_t at) { return static_cast<std::uint16_t>(b[at] | (b[at + 1] << 8)); }

void dos_set_fields(dpmi::Engine& memory, std::uint16_t segment, std::optional<char> signature, std::optional<std::uint16_t> owner, std::optional<std::uint16_t> size) {
    if (signature) memory.write(dos_linear(segment), Bytes{static_cast<std::uint8_t>(*signature)});
    if (owner) memory.write(dos_linear(segment) + 1, Bytes{static_cast<std::uint8_t>(*owner & 0xFF), static_cast<std::uint8_t>(*owner >> 8)});
    if (size) memory.write(dos_linear(segment) + 3, Bytes{static_cast<std::uint8_t>(*size & 0xFF), static_cast<std::uint8_t>(*size >> 8)});
}

// block at header `seg` (size paragraphs) keeps `keep`; the rest becomes a free block behind it.
void dos_split(dpmi::Engine& memory, std::uint16_t segment, char signature, std::uint16_t size, std::uint16_t keep) {
    if (size > keep) {
        write_mcb(memory, static_cast<std::uint16_t>(segment + 1 + keep), signature, kOwnerFree, static_cast<std::uint16_t>(size - keep - 1));
        dos_set_fields(memory, segment, 'M', std::nullopt, keep);
    }
}

}  // namespace

// ---- the memory arena ----------------------------------------------------------------------------------------------

Mcb read_mcb(dpmi::Engine& memory, std::uint16_t segment) {
    const Bytes h = memory.read(dos_linear(segment), 5);
    if (h[0] != kSignatureMore && h[0] != kSignatureLast) throw ArenaError(kErrorTrashed);
    return Mcb{segment, static_cast<char>(h[0]), dos_word(h, 1), dos_word(h, 3)};
}

void write_mcb(dpmi::Engine& memory, std::uint16_t segment, char signature, std::uint16_t owner, std::uint16_t size) {
    Bytes h(kHeaderParagraph, 0);
    h[0] = static_cast<std::uint8_t>(signature);
    h[1] = static_cast<std::uint8_t>(owner & 0xFF);
    h[2] = static_cast<std::uint8_t>(owner >> 8);
    h[3] = static_cast<std::uint8_t>(size & 0xFF);
    h[4] = static_cast<std::uint8_t>(size >> 8);
    memory.write(dos_linear(segment), h);
}

std::vector<Mcb> mcb_chain(dpmi::Engine& memory, std::uint16_t first) {
    std::vector<Mcb> out;
    std::uint32_t segment = first;
    for (std::uint32_t k = 0; k < 0x10000; ++k) {
        const Mcb m = read_mcb(memory, static_cast<std::uint16_t>(segment));
        out.push_back(m);
        if (m.signature == 'Z') return out;
        segment += 1u + m.size;
        if (segment > 0xFFFF) break;
    }
    throw ArenaError(kErrorTrashed);
}

void coalesce(dpmi::Engine& memory, std::uint16_t first) {
    const std::vector<Mcb> chain = mcb_chain(memory, first);
    std::size_t i = 0;
    while (i < chain.size()) {
        Mcb m = chain[i];
        std::size_t j = i;
        std::uint32_t size = m.size;
        char signature = m.signature;
        while (m.owner == kOwnerFree && j + 1 < chain.size() && chain[j + 1].owner == kOwnerFree) {
            ++j;
            size += 1u + chain[j].size;
            signature = chain[j].signature;
        }
        if (j > i) dos_set_fields(memory, m.segment, signature, std::nullopt, static_cast<std::uint16_t>(size));
        i = j + 1;
    }
}

std::uint16_t largest(dpmi::Engine& memory, std::uint16_t first) {
    try {
        coalesce(memory, first);
        std::uint16_t best = 0;
        for (const Mcb& m : mcb_chain(memory, first))
            if (m.owner == kOwnerFree) best = std::max(best, m.size);
        return best;
    } catch (const ArenaError&) {
        return 0;
    }
}

std::optional<std::uint16_t> allocate(dpmi::Engine& memory, std::uint16_t first, std::uint16_t paragraphs, std::uint16_t owner, Strategy strategy) {
    std::vector<Mcb> chain;
    try {
        coalesce(memory, first);
        chain = mcb_chain(memory, first);
    } catch (const ArenaError&) {
        return std::nullopt;
    }
    std::vector<Mcb> fits;
    for (const Mcb& m : chain)
        if (m.owner == kOwnerFree && m.size >= paragraphs) fits.push_back(m);
    if (fits.empty()) return std::nullopt;
    // (DOS reads the low two bits of the strategy)
    const Strategy s = static_cast<Strategy>(static_cast<std::uint8_t>(strategy) & 3);
    Mcb pick = fits.front();
    if (s == Strategy::Last) {
        pick = fits.back();
    } else if (s != Strategy::First) {   // the first of the smallest
        for (const Mcb& m : fits)
            if (m.size < pick.size) pick = m;
    }
    std::uint16_t segment = pick.segment;
    if (s == Strategy::Last && pick.size > paragraphs) {   // the block at the free block's top end
        dos_set_fields(memory, segment, 'M', std::nullopt, static_cast<std::uint16_t>(pick.size - paragraphs - 1));
        segment = static_cast<std::uint16_t>(segment + pick.size - paragraphs);
        write_mcb(memory, segment, pick.signature, owner, paragraphs);
        return static_cast<std::uint16_t>(segment + 1);
    }
    dos_split(memory, segment, pick.signature, pick.size, paragraphs);
    dos_set_fields(memory, segment, std::nullopt, owner, std::nullopt);
    return static_cast<std::uint16_t>(segment + 1);
}

void free_block(dpmi::Engine& memory, std::uint16_t first, std::uint16_t block) {
    if (block < 1) throw ArenaError(kErrorBadBlock);
    try {
        (void)read_mcb(memory, static_cast<std::uint16_t>(block - 1));
    } catch (const ArenaError&) {
        throw ArenaError(kErrorBadBlock);   // no header there: not a block
    }
    const std::vector<Mcb> chain = mcb_chain(memory, first);
    if (std::ranges::none_of(chain, [&](const Mcb& m) { return m.segment == block - 1; })) throw ArenaError(kErrorBadBlock);
    dos_set_fields(memory, static_cast<std::uint16_t>(block - 1), std::nullopt, kOwnerFree, std::nullopt);
    coalesce(memory, first);
}

std::optional<std::uint16_t> resize(dpmi::Engine& memory, std::uint16_t first, std::uint16_t block, std::uint16_t paragraphs) {
    coalesce(memory, first);
    const std::vector<Mcb> chain = mcb_chain(memory, first);
    const auto at = std::ranges::find_if(chain, [&](const Mcb& m) { return m.segment == block - 1; });
    if (block < 1 || at == chain.end()) throw ArenaError(kErrorBadBlock);
    Mcb m = *at;
    const Mcb* next = at + 1 != chain.end() ? &*(at + 1) : nullptr;
    const std::uint32_t available = m.size + (next != nullptr && next->owner == kOwnerFree ? 1u + next->size : 0u);
    if (paragraphs > available) return static_cast<std::uint16_t>(available);
    if (paragraphs > m.size) {   // grow into the free block behind
        m.signature = next->signature;
        m.size = static_cast<std::uint16_t>(available);
        dos_set_fields(memory, m.segment, m.signature, std::nullopt, m.size);
    }
    dos_split(memory, m.segment, m.signature, m.size, paragraphs);
    coalesce(memory, first);
    return std::nullopt;
}

// ---- the files a run sees -----------------------------------------------------------------------------------------

namespace {

// The file's bytes, tried again while the system refuses it for a moment (a virus scanner, the indexer): a failed open here
// would reach the emulated program as a DOS error and change what it does.
Bytes dos_read_when_free(const stdfs::path& path) {
    for (int k = 0;; ++k) {
        try {
            return fs::read_file(path);
        } catch (const NotFoundError&) {
            throw;
        } catch (const IoError&) {
            if (k + 1 >= kMaxOpenRetries) throw;
            std::this_thread::sleep_for(std::chrono::milliseconds(50 * (k + 1)));
        }
    }
}

}  // namespace

Bytes DiskCache::read(const stdfs::path& path) {
    std::error_code ec;
    const auto modified = cb::fs::last_write_time(path, ec);
    const std::uint64_t size = ec ? 0 : stdfs::file_size(path, ec);
    if (ec) throw NotFoundError(path, 2, "not found");
    const std::string key = MemFS::key(path);
    {
        const std::lock_guard<std::mutex> lock(mutex_);
        if (const auto it = entries_.find(key); it != entries_.end() && it->second.modified == modified && it->second.size == size) return *it->second.data;
    }
    auto data = std::make_shared<const Bytes>(dos_read_when_free(path));
    const std::lock_guard<std::mutex> lock(mutex_);
    entries_[key] = Entry{modified, size, data};
    return *data;
}

Bytes MemFile::read(std::optional<std::size_t> count) {
    const std::uint64_t length = data_->size();
    const std::uint64_t end = count ? std::min<std::uint64_t>(length, position_ + *count) : length;
    Bytes out;
    if (position_ < end) out.assign(data_->begin() + static_cast<std::ptrdiff_t>(position_), data_->begin() + static_cast<std::ptrdiff_t>(end));
    position_ = std::max(position_, end);
    return out;
}

std::size_t MemFile::write(ByteView data) {
    if (position_ > data_->size()) data_->resize(position_, 0);
    const std::uint64_t end = position_ + data.size();
    if (end > data_->size()) data_->resize(end);
    std::copy(data.begin(), data.end(), data_->begin() + static_cast<std::ptrdiff_t>(position_));
    position_ = end;
    return data.size();
}

std::uint64_t MemFile::seek(std::int64_t offset, int whence) {
    const std::int64_t base = whence == 0 ? 0 : whence == 1 ? static_cast<std::int64_t>(position_) : static_cast<std::int64_t>(data_->size());
    position_ = static_cast<std::uint64_t>(std::max<std::int64_t>(0, base + offset));
    return position_;
}

void MemFile::truncate(std::optional<std::uint64_t> size) {
    const std::uint64_t at = size.value_or(position_);
    if (at < data_->size()) data_->resize(at);
}

MemFS::MemFS(std::vector<stdfs::path> hide, std::shared_ptr<DiskCache> disk) : disk_(disk ? std::move(disk) : std::make_shared<DiskCache>()) {
    for (const stdfs::path& h : hide) hide_.push_back(key(h) + "\\");
}

std::string MemFS::key(const stdfs::path& path) {
    std::string s = fs::to_text(stdfs::absolute(path).lexically_normal());
    std::ranges::replace(s, '/', '\\');
    return text::lower(s);
}

bool MemFS::is_hidden(const std::string& k) const {
    return std::ranges::any_of(hide_, [&](const std::string& h) { return k.starts_with(h); });
}

Bytes MemFS::disk(const stdfs::path& path) const { return disk_->read(path); }

bool MemFS::is_file(const stdfs::path& path) const {
    const std::string k = key(path);
    if (files_.contains(k)) return true;
    std::error_code ec;
    return !gone_.contains(k) && !is_hidden(k) && stdfs::is_regular_file(path, ec);
}

std::uint64_t MemFS::size(const stdfs::path& path) const {
    if (const auto it = files_.find(key(path)); it != files_.end()) return it->second->size();
    std::error_code ec;
    const std::uint64_t n = stdfs::file_size(path, ec);
    if (ec) throw NotFoundError(path, 2, "not found");
    return n;
}

Bytes MemFS::read(const stdfs::path& path) const {
    if (const auto it = files_.find(key(path)); it != files_.end()) return *it->second;
    return disk(path);
}

MemFile MemFS::open(const stdfs::path& path, bool write) const {
    const std::string k = key(path);
    if (write) {
        files_[k] = std::make_shared<Bytes>();
        gone_.erase(k);
    } else if (!files_.contains(k)) {
        if (!is_file(path)) throw NotFoundError(path, 2, "not found");
        files_[k] = std::make_shared<Bytes>(disk(path));
    }
    return MemFile(files_.at(k));
}

void MemFS::put(const stdfs::path& path, ByteView data) const {
    const std::string k = key(path);
    files_[k] = std::make_shared<Bytes>(data.begin(), data.end());
    gone_.erase(k);
}

void MemFS::remove(const stdfs::path& path) const {
    const std::string k = key(path);
    if (files_.contains(k)) files_.erase(k);
    else if (!is_file(path)) throw NotFoundError(path, 2, "not found");
    gone_.insert(k);
}

void MemFS::replace(const stdfs::path& from, const stdfs::path& to) const {
    put(to, read(from));
    remove(from);
}

// ---- the run -----------------------------------------------------------------------------------------------------

struct Executive::Impl {
    Limits limits;
    cbd_result* (*run)(const cbd_args*) = nullptr;
    void (*free_result)(cbd_result*) = nullptr;
    void* (*fs_new)() = nullptr;
    void (*fs_free)(void*) = nullptr;
    void (*fs_hide)(void*, const wchar_t*) = nullptr;
    void (*fs_put)(void*, const wchar_t*, const std::uint8_t*, std::int64_t) = nullptr;
    void (*fs_gone)(void*, const wchar_t*) = nullptr;
    std::int64_t (*fs_changed)(void*) = nullptr;
    int (*fs_entry)(void*, std::int64_t, const wchar_t**, const std::uint8_t**, std::int64_t*) = nullptr;
    std::int64_t (*fs_ngone)(void*) = nullptr;
    const wchar_t* (*fs_gone_at)(void*, std::int64_t) = nullptr;

    explicit Impl(const Limits& lim) : limits(lim) {}
};

Executive::Executive(std::unique_ptr<Impl> impl) : impl_(std::move(impl)) {}
Executive::~Executive() = default;

std::unique_ptr<Executive> Executive::load(const stdfs::path& unicorn_dll, const Limits& limits) {
#ifdef CB_DOSEXEC
    if (!stdfs::exists(unicorn_dll)) throw NotFoundError(unicorn_dll, 2, "the DOS executive needs this file");
    auto impl = std::make_unique<Impl>(limits);
    Impl& i = *impl;
    i.run = &cbd_run;
    i.free_result = &cbd_free;
    i.fs_new = &cbd_fs_new;
    i.fs_free = &cbd_fs_free;
    i.fs_hide = &cbd_fs_hide;
    i.fs_put = &cbd_fs_put;
    i.fs_gone = &cbd_fs_gone;
    i.fs_changed = &cbd_fs_changed;
    i.fs_entry = &cbd_fs_entry;
    i.fs_ngone = &cbd_fs_ngone;
    i.fs_gone_at = &cbd_fs_gone_at;
    char err[256] = {};
    if (cbd_init(stdfs::absolute(unicorn_dll).wstring().c_str(), err, sizeof err) != 0) throw ToolError(fs::to_text(unicorn_dll), 1, err, "the DOS executive could not start");
    return std::unique_ptr<Executive>(new Executive(std::move(impl)));
#else
    (void)unicorn_dll;
    (void)limits;
    throw UnsupportedError("dos", 0, "the DOS executive is built on Windows only");
#endif
}

namespace {

// What the executive reported of a run that failed, as the error it is.
[[noreturn]] void dos_raise(const cbd_result& r, const stdfs::path& exe) {
    const std::string message = r.message != nullptr ? r.message : "";
    if (r.status == CBD_OS) {
        const stdfs::path file = r.filename != nullptr ? stdfs::path(r.filename) : exe;
        const int code = r.err_no < 0 ? -r.err_no : r.err_no;   // (a negative number is the system's own)
        const int errno_kind = r.err_no < 0 ? [&] {
            switch (-r.err_no) {
                case 2: case 3: case 15: case 18: case 53: case 67: case 206: return 2;   // not found
                case 5: case 19: case 32: case 33: case 65: case 82: case 108: case 109: return 13;   // refused
                default: return 22;
            }
        }() : r.err_no;
        if (r.err_no == 0 || errno_kind == 2) throw NotFoundError(file, code, message.empty() ? "not found" : message);
        if (errno_kind == 1 || errno_kind == 13) throw AccessError(file, code, message.empty() ? "access denied" : message);
        throw IoError(file, code, message);
    }
    throw EmulationError(message.empty() ? std::format("the DOS executive failed (status {})", r.status) : message);
}

}  // namespace

Result Executive::run(const RunSpec& spec) const {
    const Impl& i = *impl_;
    const std::wstring exe = to_wide(fs::to_text(stdfs::absolute(spec.exe))), workdir = to_wide(fs::to_text(stdfs::absolute(spec.workdir)));
    const Bytes tail = text::to_latin1(spec.command_tail);
    cbd_args a{};
    a.exe = exe.c_str();
    a.workdir = workdir.c_str();
    a.cmdline = tail.data();
    a.ncmdline = static_cast<std::int64_t>(tail.size());
    a.stdin_data = spec.stdin_data.data();
    a.nstdin = static_cast<std::int64_t>(spec.stdin_data.size());
    a.env = spec.environment.data();
    a.nenv = static_cast<std::int64_t>(spec.environment.size());
    a.max_blocks = static_cast<std::int64_t>(spec.max_blocks);
    a.trace_calls = spec.trace_calls;
    a.psp = spec.psp.value_or(0);
    a.top = spec.top.value_or(0);
    a.fast = spec.fast;
    a.dpmi = spec.dpmi;
    a.debug = i.limits.debug;
    a.drive = spec.drive.value_or(i.limits.drive);
    a.fast_limit = i.limits.fast_limit;
    a.dpmi_limit = i.limits.dpmi_limit;
    a.watch = spec.watch;
    a.watch_near = spec.watch_near;
    a.watch_max = spec.watch_max;
    a.watch_ds_bytes = spec.watch_ds_bytes;
    a.watch_dumps = spec.watch_dumps;
    std::wstring drive_list;
    for (const auto& [letter, folder] : spec.drives)
        drive_list += std::wstring(1, static_cast<wchar_t>(std::toupper(static_cast<unsigned char>(letter)))) + L"=" +
                      to_wide(fs::to_text(stdfs::absolute(folder))) + L"\n";
    a.drives = drive_list.empty() ? nullptr : drive_list.c_str();

    struct HandedOver {   // the MemFS in the executive's own shape, freed whatever happens
        const Impl& impl;
        void* fs = nullptr;
        ~HandedOver() {
            if (fs != nullptr) impl.fs_free(fs);
        }
    } handed{i};
    if (spec.files != nullptr) {
        handed.fs = i.fs_new();
        for (const std::string& h : spec.files->hide_) i.fs_hide(handed.fs, to_wide(h).c_str());
        for (const auto& [key, data] : spec.files->files_) i.fs_put(handed.fs, to_wide(key).c_str(), data->data(), static_cast<std::int64_t>(data->size()));
        for (const std::string& key : spec.files->gone_) i.fs_gone(handed.fs, to_wide(key).c_str());
        a.fs = handed.fs;
    }

    cbd_result* r = i.run(&a);
    if (r == nullptr) throw EmulationError("the DOS executive gave no result");
    struct Release {
        const Impl& impl;
        cbd_result* r;
        ~Release() { impl.free_result(r); }
    } release{i, r};

    if (spec.files != nullptr) {   // what the run changed in the files, even when it failed
        const std::int64_t changed = i.fs_changed(handed.fs);
        for (std::int64_t k = 0; k < changed; ++k) {
            const wchar_t* key = nullptr;
            const std::uint8_t* data = nullptr;
            std::int64_t n = 0;
            const bool present = i.fs_entry(handed.fs, k, &key, &data, &n) != 0;
            const std::string name = from_wide(key);
            if (present) spec.files->files_[name] = std::make_shared<Bytes>(data, data + n);
            else spec.files->files_.erase(name);
        }
        spec.files->gone_.clear();
        for (std::int64_t k = 0, n = i.fs_ngone(handed.fs); k < n; ++k) spec.files->gone_.insert(from_wide(i.fs_gone_at(handed.fs, k)));
    }
    if (r->status != CBD_OK) dos_raise(*r, spec.exe);

    Result out;
    if (r->has_exit) out.exit = static_cast<std::uint32_t>(r->exit);
    out.console.assign(r->console, r->console + r->nconsole);
    if (r->standard_out != nullptr) out.standard_out.assign(r->standard_out, r->standard_out + r->nstandard_out);
    out.blocks.insert(r->blocks, r->blocks + r->nblocks);
    for (std::int64_t k = 0; k + 1 < r->ncalls * 2; k += 2) out.calls[r->calls[k]] = r->calls[k + 1];
    for (std::int64_t k = 0; k < r->nwatched; ++k) {
        const std::uint64_t* w = r->watched + k * CBD_WATCH_STRIDE;
        WatchedCall c;
        c.return_address = w[0];
        for (std::size_t j = 0; j < 8; ++j) c.args[j] = static_cast<std::uint16_t>(w[2 + j]);
        c.ds_in = static_cast<std::uint16_t>(w[10]);
        c.ds_in_hash = w[11];
        c.returned = w[12] != 0;
        c.ax = static_cast<std::uint16_t>(w[13]);
        c.dx = static_cast<std::uint16_t>(w[14]);
        c.ds = static_cast<std::uint16_t>(w[15]);
        c.ds_hash = w[16];
        c.flags = static_cast<std::uint16_t>(w[17]);
        for (std::size_t j = 0; j < 64; ++j) c.ds_parts[j] = static_cast<std::uint32_t>(w[18 + j]);
        const auto n = static_cast<std::size_t>(spec.watch_ds_bytes);
        if (w[82] != 0 && w[82] - 1 + n <= static_cast<std::uint64_t>(r->ndumps)) c.ds_in_bytes.assign(r->dumps + w[82] - 1, r->dumps + w[82] - 1 + n);
        if (w[83] != 0 && w[83] - 1 + n <= static_cast<std::uint64_t>(r->ndumps)) c.ds_bytes.assign(r->dumps + w[83] - 1, r->dumps + w[83] - 1 + n);
        out.watched.push_back(c);
    }
    out.steps = static_cast<std::uint64_t>(r->steps);
    out.load_seg = static_cast<std::uint32_t>(r->load_seg);
    return out;
}

std::string Executive::summary(const Result& r) {
    return std::format("[exit {}, {} basic blocks executed, {} distinct]", r.exit ? std::to_string(*r.exit) : std::string("none"), r.steps, r.blocks.size());
}

}  // namespace cb::exact::emu::dos
