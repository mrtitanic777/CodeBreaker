#include "formats/powerpc_image.hpp"

#include <algorithm>
#include <nlohmann/json.hpp>
#include <set>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/json.hpp"
#include "formats/pe.hpp"

namespace cb::formats::powerpc_image {
namespace {

constexpr std::uint32_t kMflrR12 = 0x7D8802A6, kBlr = 0x4E800020, kMtctrR11 = 0x7D6903A6, kBctr = 0x4E800420;
constexpr std::uint32_t kMtlrR12 = 0x7D8803A6;   // mtlr r12
constexpr std::uint32_t kExecutable = 0x20000000;

// The primary opcode of a word, and the fields of the forms the runs and stubs are made of.
constexpr std::uint32_t opcode(std::uint32_t w) { return w >> 26; }
constexpr std::uint32_t ra_of(std::uint32_t w) { return (w >> 16) & 31; }
constexpr std::uint32_t rs_of(std::uint32_t w) { return (w >> 21) & 31; }

std::optional<std::uint32_t> be32_at(ByteView data, std::uint64_t at) {
    if (at > data.size() || data.size() - at < 4) return std::nullopt;
    return (std::uint32_t{data[at]} << 24) | (std::uint32_t{data[at + 1]} << 16) | (std::uint32_t{data[at + 2]} << 8) | data[at + 3];
}

Address address_of(std::uint64_t value, std::string_view name) {
    if (value > 0xFFFFFFFFull) throw FormatError("powerpc image", 0, std::string(name) + ": an address past 32 bits");
    return static_cast<Address>(value);
}

std::optional<std::uint64_t> offset_in(std::span<const Span> spans, std::uint64_t va) {
    for (const Span& s : spans)
        if (s.va <= va && va < std::uint64_t{s.va} + s.size) return s.file_offset + (va - s.va);
    return std::nullopt;
}

// ---- the exception table -----------------------------------------------------------------------------------------------

struct Record {
    std::uint32_t begin = 0, packed = 0;   // the function's start; its length in words (22 bits), prologue length, flags
};

// The table's records (8 bytes each) that name a function of some length, read from `offset` of the file.
std::vector<Record> exception_table(ByteView file, std::optional<std::uint64_t> offset, std::uint64_t size) {
    std::vector<Record> out;
    if (!offset) return out;
    for (std::uint64_t i = 0; i < size / 8; ++i) {
        const std::optional<std::uint32_t> begin = be32_at(file, *offset + 8 * i);
        const std::optional<std::uint32_t> packed = be32_at(file, *offset + 8 * i + 4);
        if (!begin || !packed) break;
        if (*begin != 0 && ((*packed >> 8) & 0x3FFFFF) != 0) out.push_back({*begin, *packed});
    }
    return out;
}

// How much the word at a function start looks like the first instruction of code: 2 for a prologue, 1 for any other instruction, 0 for
// padding, data or nothing there.
int looks_like_code(ByteView file, std::optional<std::uint64_t> offset) {
    if (!offset) return 0;
    const std::optional<std::uint32_t> w = be32_at(file, *offset);
    if (!w) return 0;
    if (*w == kMflrR12 || (*w >> 16) == 0x9421) return 2;
    const std::uint32_t op = opcode(*w);
    return (*w != 0 && op != 0 && op != 1 && op != 5 && op != 6) ? 1 : 0;
}

// ---- the register runs --------------------------------------------------------------------------------------------------

// Walk back from the end of a run (the instruction at `end`) over the instructions of one opcode on one base register; each is named for the
// register it saves or restores. `entered_by_call`: the runs a function enters by `bl`.
struct RunFinder {
    std::span<const std::uint32_t> words;
    Address base = 0;
    std::map<Address, std::string>& names;
    std::set<Address>& entered_by_call;

    void back(std::size_t end, std::uint32_t op, std::uint32_t base_register, const char* name, bool entered, bool double_word) const {
        for (std::size_t j = end; j-- > 0;) {
            const std::uint32_t w = words[j];
            if (opcode(w) != op || ra_of(w) != base_register || (double_word && (w & 3) != 0)) break;
            const Address at = base + static_cast<Address>(4 * j);
            names[at] = std::string(name) + "_" + std::to_string(rs_of(w));
            if (entered) entered_by_call.insert(at);
        }
    }

    void find() const {
        const std::size_t n = words.size();
        for (std::size_t i = 0; i < n; ++i) {
            const std::uint32_t w = words[i];
            if (w == 0x9181FFF8 && i + 1 < n && words[i + 1] == kBlr) {                                      // stw r12,-8(r1); blr
                back(i, 62, 1, "__savegprlr", true, true);
            } else if (w == 0x8181FFF8 && i + 2 < n && words[i + 1] == kMtlrR12 && words[i + 2] == kBlr) {   // lwz r12,-8(r1); mtlr r12; blr
                back(i, 58, 1, "__restgprlr", false, true);
            } else if (w == kBlr && i >= 2) {
                const std::uint32_t previous = words[i - 1];
                if (opcode(previous) == 54 && ra_of(previous) == 12 && opcode(words[i - 2]) == 54) back(i, 54, 12, "__savefpr", true, false);       // stfd
                else if (opcode(previous) == 50 && ra_of(previous) == 12 && opcode(words[i - 2]) == 50) back(i, 50, 12, "__restfpr", true, false);   // lfd
            }
        }
    }
};

// ---- one reading of the file, in one of its two layouts -------------------------------------------------------------------

struct Layout {
    bool memory = false;
    std::vector<Span> spans;
    std::vector<Section> sections;
    std::map<Address, Extent> extents;
    std::vector<Address> starts;   // the table's
    int score = 0;                 // how many of the starts look like code
};

Layout read_layout(ByteView file, const formats::pe::Image& pe, Address base, bool memory, const std::vector<Record>& records) {
    Layout out;
    out.memory = memory;
    if (memory) {
        out.spans.push_back({base, 0, file.size()});
    } else {
        for (const formats::pe::Section& s : pe.sections()) {
            const std::uint64_t size = std::min<std::uint64_t>(s.raw_size, s.virtual_size != 0 ? s.virtual_size : s.raw_size);
            if (size != 0 && s.raw_ptr != 0) out.spans.push_back({address_of(std::uint64_t{base} + s.virtual_address, "a section"), s.raw_ptr, size});
        }
    }
    for (const formats::pe::Section& s : pe.sections())
        out.sections.push_back({s.name, address_of(std::uint64_t{base} + s.virtual_address, "a section"),
                                s.virtual_size != 0 ? s.virtual_size : s.raw_size, s.characteristics});
    for (const Record& r : records) {
        out.starts.push_back(r.begin);
        out.extents.insert_or_assign(r.begin, Extent{((r.packed >> 8) & 0x3FFFFF) * 4, (r.packed & 0xFF) * 4, (r.packed >> 31) != 0});
    }
    for (const Address start : out.starts) out.score += looks_like_code(file, offset_in(out.spans, start));
    return out;
}

}  // namespace

// ---- reading ------------------------------------------------------------------------------------------------------------

Image Image::read(const std::filesystem::path& path) {
    const Bytes file = fs::read_file(path);
    std::optional<Address> base;
    // the load address of an .xex's image is in the header, which `xex::basefile` copies to <image>.json
    const std::filesystem::path sidecar = fs::from_text(fs::to_text(path) + ".json");
    std::error_code ignored;
    if (std::filesystem::is_regular_file(sidecar, ignored)) {
        Json j;
        if (json::try_parse(fs::read_text(sidecar), j) && j.is_object()) {
            const auto it = j.find("base");
            if (it != j.end() && it->is_number_unsigned() && it->get<std::uint64_t>() != 0 && it->get<std::uint64_t>() <= 0xFFFFFFFFull)
                base = static_cast<Address>(it->get<std::uint64_t>());
        }
    }
    return parse(file, base, fs::to_text(path.filename()));
}

Image Image::read_xex(const xex::Tools& tools, const std::filesystem::path& xex_file) { return read(xex::basefile(tools, xex_file)); }

Image Image::parse(Bytes file, std::optional<Address> base_override, std::string_view name) {
    const formats::pe::Image pe = formats::pe::Image::parse(ByteView(file), name);
    if (pe.header().machine != kMachine) throw FormatError("powerpc image", 0, std::string(name) + ": not a PowerPC image (machine type " + pe.machine_name() + ")");
    const ByteView data(file);
    const Address base = base_override ? *base_override : address_of(pe.header().image_base, name);

    // The exception table is in the file as packed, or the file is the image as it sits in memory (its table is at its RVA and the section
    // headers' file positions are not true). Each is read, and the one whose entries point at code is the file's layout.
    const formats::pe::DataDirectory table_directory = pe.directory(3);
    const std::optional<std::uint64_t> packed_offset = pe.rva_to_offset(table_directory.rva);
    const std::vector<Record> packed_records = exception_table(data, packed_offset, table_directory.size);
    const std::vector<Record> memory_records = exception_table(data, std::uint64_t{table_directory.rva}, table_directory.size);
    Layout as_file = read_layout(data, pe, base, false, packed_records);
    Layout as_memory = read_layout(data, pe, base, true, memory_records);
    bool memory;
    if (as_memory.starts.empty() && as_file.starts.empty()) {
        // no table to tell by: an image that is as long as its sections reach is the image in memory
        std::uint64_t end = 0;
        for (const formats::pe::Section& s : pe.sections()) end = std::max<std::uint64_t>(end, std::uint64_t{s.virtual_address} + std::max<std::uint32_t>(s.virtual_size, 1));
        memory = data.size() >= end;
    } else {
        memory = as_memory.score > as_file.score;
    }
    Layout& chosen = memory ? as_memory : as_file;

    Image out;
    out.file_ = std::move(file);
    out.base_ = base;
    out.memory_layout_ = memory;
    out.spans_ = std::move(chosen.spans);
    out.sections_ = std::move(chosen.sections);
    out.extents_ = std::move(chosen.extents);
    out.entry_ = pe.header().entry_rva != 0 ? address_of(std::uint64_t{base} + pe.header().entry_rva, name) : 0;

    // the code: every executable section
    bool any = false;
    Address lo = 0, hi = 0;
    for (const Section& s : out.sections_) {
        if (!s.executable()) continue;
        const Address end = address_of(std::uint64_t{s.va} + s.size, name);
        lo = any ? std::min(lo, s.va) : s.va;
        hi = any ? std::max(hi, end) : end;
        any = true;
    }
    out.has_code_ = any;
    if (!any) return out;
    out.code_begin_ = lo;
    out.code_end_ = hi;

    // the code's words (as many as the file holds of it)
    const ByteView code = out.bytes_at(lo, hi - lo);
    std::vector<std::uint32_t> words(code.size() / 4);
    for (std::size_t i = 0; i < words.size(); ++i) words[i] = *be32_at(code, 4 * i);
    const std::size_t count = words.size();

    // the import stubs: two record words (library index, ordinal) and `mtctr r11; bctr`
    for (std::size_t i = 0; i + 3 < count; ++i)
        if (words[i + 2] == kMtctrR11 && words[i + 3] == kBctr && (words[i] >> 24) == 1 && (words[i + 1] >> 24) == 2)
            out.imports_[lo + static_cast<Address>(4 * i)] = "import" + std::to_string((words[i] >> 16) & 0xFF) + "_ord_" + std::to_string(words[i] & 0xFFFF);

    std::set<Address> entered_by_call;
    RunFinder{words, lo, out.run_names_, entered_by_call}.find();

    // function starts: the table's, and every `bl` into the code
    std::set<Address> found(chosen.starts.begin(), chosen.starts.end());
    for (std::size_t i = 0; i < count; ++i) {
        const std::uint32_t w = words[i];
        if (opcode(w) != 18 || (w & 3) != 1) continue;   // bl
        std::int64_t displacement = w & 0x03FFFFFC;
        if (displacement & 0x02000000) displacement -= 0x04000000;
        const std::int64_t target = std::int64_t{lo} + static_cast<std::int64_t>(4 * i) + displacement;
        if (target >= lo && target < hi) found.insert(static_cast<Address>(target));
    }
    for (const Address va : entered_by_call) out.inlined_[va] = out.run_names_.at(va);
    for (const Address va : found)
        if (!out.run_names_.contains(va) && !out.imports_.contains(va) && va >= lo && va < hi) out.functions_.push_back(va);
    return out;
}

// ---- addressing ---------------------------------------------------------------------------------------------------------

std::optional<std::uint64_t> Image::offset(Address va) const { return offset_in(spans_, va); }

std::optional<std::uint32_t> Image::word(Address va) const {
    const std::optional<std::uint64_t> at = offset(va);
    return at ? be32_at(file_, *at) : std::nullopt;
}

ByteView Image::bytes_at(Address va, std::size_t size) const {
    const std::optional<std::uint64_t> at = offset(va);
    if (!at || *at >= file_.size()) return {};
    return ByteView(file_).subspan(static_cast<std::size_t>(*at), static_cast<std::size_t>(std::min<std::uint64_t>(size, file_.size() - *at)));
}

std::vector<Image::Region> Image::regions() const {
    std::vector<Region> out;
    for (const Span& s : spans_) {
        if (s.file_offset >= file_.size()) {
            out.push_back({s.va, {}});
            continue;
        }
        out.push_back({s.va, ByteView(file_).subspan(static_cast<std::size_t>(s.file_offset),
                                                      static_cast<std::size_t>(std::min<std::uint64_t>(s.size, file_.size() - s.file_offset)))});
    }
    return out;
}

// ---- run-time type information -------------------------------------------------------------------------------------------

namespace {

bool library_class(std::string_view decorated) {
    return decorated.find("@std@@") != std::string_view::npos || decorated.find("@tr1@") != std::string_view::npos ||
           decorated.find("@stlp") != std::string_view::npos;
}

// `.?AVInner@Outer@@` -> `Outer::Inner`: the names between the `.?AV` and the `@@`, last first.
std::string class_of(std::string_view decorated) {
    std::vector<std::string_view> parts;
    std::string_view rest = decorated.size() > 4 ? decorated.substr(4) : std::string_view();
    while (!rest.empty()) {
        const std::size_t at = rest.find('@');
        const std::string_view part = rest.substr(0, at);
        if (!part.empty()) parts.push_back(part);
        if (at == std::string_view::npos) break;
        rest.remove_prefix(at + 1);
    }
    if (parts.empty()) return std::string(decorated);
    std::string out;
    for (std::size_t k = parts.size(); k-- > 0;) {
        out += parts[k];
        if (k != 0) out += "::";
    }
    return out;
}

bool name_char(std::uint8_t b) {
    return (b >= '0' && b <= '9') || (b >= 'a' && b <= 'z') || (b >= 'A' && b <= 'Z') || b == '_' || b == '@' || b == '?' || b == '$';
}

// The type descriptors in the file: `.?AV<name>\0` strings (1 to 300 name characters) that sit eight bytes after the start of a descriptor
// (its table pointer, a spare word that is 0, then the name), where the descriptor's table pointer points into the image. By address.
std::map<Address, std::string> type_descriptors(const Image& image) {
    std::map<Address, std::string> out;
    const ByteView data = image.file();
    const std::span<const Span> spans = image.spans();
    const std::size_t n = data.size();
    for (std::size_t i = 0; i + 4 < n;) {
        if (data[i] == '.' && data[i + 1] == '?' && data[i + 2] == 'A' && (data[i + 3] == 'V' || data[i + 3] == 'U')) {
            std::size_t j = i + 4;
            std::size_t length = 0;
            while (j < n && name_char(data[j]) && length < 301) {
                ++j;
                ++length;
            }
            if (length >= 1 && length <= 300 && j < n && data[j] == 0) {
                // the file offset of the name, as an address
                std::optional<Address> name_va;
                for (const Span& s : spans)
                    if (s.file_offset <= i && i < s.file_offset + s.size) {
                        name_va = static_cast<Address>(s.va + (i - s.file_offset));
                        break;
                    }
                if (name_va && *name_va >= 8) {
                    const Address descriptor = *name_va - 8;
                    const std::optional<std::uint32_t> table = image.word(descriptor);
                    const std::optional<std::uint32_t> spare = image.word(descriptor + 4);
                    if (spare && *spare == 0 && image.offset(table.value_or(0)))
                        out[descriptor] = std::string(reinterpret_cast<const char*>(data.data() + i), j - i);
                }
                i = j + 1;
                continue;
            }
        }
        ++i;
    }
    return out;
}

}  // namespace

std::vector<VirtualMethod> virtual_methods(const Image& image) {
    const std::map<Address, std::string> descriptors = type_descriptors(image);
    const ByteView data = image.file();
    // the locators: a zero signature and, in the fourth word, a type descriptor
    std::map<Address, std::string> locators;
    for (const Span& s : image.spans())
        for (std::uint64_t i = 0; i + 19 < s.size; i += 4)
            if (be32_at(data, s.file_offset + i) == 0u) {
                const std::optional<std::uint32_t> descriptor = be32_at(data, s.file_offset + i + 12);
                if (!descriptor) continue;
                const auto found = descriptors.find(*descriptor);
                if (found != descriptors.end()) locators[static_cast<Address>(s.va + i)] = found->second;
            }
    const std::set<Address> starts(image.functions().begin(), image.functions().end());
    std::map<Address, VirtualMethod> out;   // by address, in address order
    for (const Span& s : image.spans())
        for (std::uint64_t i = 0; i + 3 < s.size; i += 4) {
            const std::optional<std::uint32_t> locator = be32_at(data, s.file_offset + i);
            if (!locator) continue;
            const auto found = locators.find(*locator);
            if (found == locators.end() || library_class(found->second)) continue;
            const std::string class_name = class_of(found->second);
            // the table's entries, as long as they are function starts
            for (std::uint32_t slot = 0;; ++slot) {
                const std::optional<std::uint32_t> entry = image.word(static_cast<Address>(s.va + i + 4 + 4 * std::uint64_t{slot}));
                if (!entry || !starts.contains(*entry)) break;
                out.try_emplace(*entry, VirtualMethod{*entry, class_name, slot});
            }
        }
    std::vector<VirtualMethod> methods;
    methods.reserve(out.size());
    for (auto& [address, method] : out) methods.push_back(std::move(method));
    return methods;
}

}  // namespace cb::formats::powerpc_image
