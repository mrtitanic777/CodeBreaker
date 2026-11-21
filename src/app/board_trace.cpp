// app/board_trace.cpp -- see board_trace.hpp.
#include "app/board_trace.hpp"

#include <algorithm>
#include <exception>
#include <format>
#include <map>
#include <memory>
#include <set>
#include <tuple>

#include "core/fs.hpp"
#include "core/text.hpp"
#include "formats/pe.hpp"

namespace cb::app::board {

namespace stdfs = std::filesystem;

namespace {

constexpr std::string_view kMeasuredHeader = "#codebreaker-board-measured 1";
constexpr std::uint32_t kRing0 = 0xC0000000u;
constexpr std::string_view kImageExtensions[] = {".dll", ".exe", ".drv", ".cpl", ".ocx", ".scr", ".acm", ".ax", ".tlb", ".ime"};

std::vector<std::string_view> trace_tabs(std::string_view line) {
    std::vector<std::string_view> out;
    std::size_t at = 0;
    while (true) {
        const std::size_t tab = line.find('\t', at);
        out.push_back(line.substr(at, tab == std::string_view::npos ? std::string_view::npos : tab - at));
        if (tab == std::string_view::npos) return out;
        at = tab + 1;
    }
}

std::uint64_t trace_decimal(std::string_view s) {
    std::uint64_t v = 0;
    for (const char c : s) {
        if (c < '0' || c > '9') break;
        v = v * 10 + static_cast<std::uint64_t>(c - '0');
    }
    return v;
}

std::uint32_t trace_hex32(std::string_view s) {
    std::uint32_t v = 0;
    for (const char c : s) {
        const int d = c >= '0' && c <= '9' ? c - '0' : c >= 'a' && c <= 'f' ? c - 'a' + 10 : c >= 'A' && c <= 'F' ? c - 'A' + 10 : -1;
        if (d < 0) break;
        v = v * 16 + static_cast<std::uint32_t>(d);
    }
    return v;
}

template <class Each>
void trace_each_line(std::string_view all, Each&& each) {
    std::size_t at = 0;
    while (at < all.size()) {
        std::size_t end = all.find('\n', at);
        if (end == std::string_view::npos) end = all.size();
        std::string_view line = all.substr(at, end - at);
        if (!line.empty() && line.back() == '\r') line.remove_suffix(1);
        each(line);
        at = end + 1;
    }
}

}  // namespace

std::vector<InstalledImage> read_installed(const stdfs::path& folder) {
    std::vector<InstalledImage> images;
    std::error_code ec;
    for (stdfs::recursive_directory_iterator it(folder, stdfs::directory_options::skip_permission_denied, ec), end; !ec && it != end; it.increment(ec)) {
        if (!it->is_regular_file(ec)) continue;
        const std::string extension = text::lower(fs::to_text(it->path().extension()));
        if (std::find(std::begin(kImageExtensions), std::end(kImageExtensions), extension) == std::end(kImageExtensions)) continue;
        try {
            const Bytes data = fs::read_file(it->path());
            std::optional<formats::pe::Image> parsed = formats::pe::Image::try_parse(data);
            if (!parsed) continue;
            const formats::pe::OptionalHeader header = parsed->optional_header();
            if (header.magic != 0x10B) continue;
            InstalledImage image;
            image.name = text::lower(fs::to_text(it->path().filename()));
            image.base = static_cast<std::uint32_t>(header.image_base);
            image.size = header.image_size;
            if (parsed->directory(0).size != 0) {
                for (const formats::pe::ExportSymbol& s : parsed->export_table().symbols) {
                    if (s.rva == 0 || s.forward) continue;
                    image.exports.try_emplace(s.rva, s.name ? *s.name : "#" + std::to_string(s.ordinal));
                }
            }
            auto held = std::make_shared<const formats::pe::Image>(std::move(*parsed));
            image.has_transfer = [held](std::uint32_t rva) {
                const ByteView d = held->data();
                for (const formats::pe::Section& s : held->sections()) {
                    // only bytes the file really holds: past a section's raw data the next section's bytes would pass for this one's
                    if (rva < s.virtual_address || rva - s.virtual_address >= s.raw_size) continue;
                    const std::uint64_t offset = std::uint64_t{s.raw_ptr} + (rva - s.virtual_address);
                    if (offset + 1 >= d.size() || d[offset] != 0xFF) return false;
                    const unsigned reg = (d[offset + 1] >> 3) & 7;
                    return reg == 2 || reg == 4;
                }
                return false;
            };
            images.push_back(std::move(image));
        } catch (const std::exception&) {}   // (a damaged file is no image)
    }
    return images;
}

std::optional<Measured> read_trace(std::string_view trace, std::span<const InstalledImage> images) {
    if (!trace.starts_with("#calltrace 1")) return std::nullopt;
    Measured m;
    std::uint64_t cycles = 0, hz = 0;
    std::map<std::pair<std::string, std::string>, std::uint64_t> channels;
    std::map<std::tuple<std::string, std::string, std::string>, std::uint64_t> functions;
    const auto holding = [&](std::uint32_t address) {
        std::vector<const InstalledImage*> out;
        for (const InstalledImage& image : images)
            if (address >= image.base && address - image.base < image.size) out.push_back(&image);
        return out;
    };
    const auto one_name = [](const std::vector<const InstalledImage*>& found) {
        return std::all_of(found.begin(), found.end(), [&](const InstalledImage* i) { return i->name == found[0]->name; });
    };
    trace_each_line(trace, [&](std::string_view line) {
        const std::vector<std::string_view> p = trace_tabs(line);
        if (p[0] == "cycles" && p.size() >= 2) cycles = trace_decimal(p[1]);
        else if (p[0] == "cpu_hz" && p.size() >= 2) hz = trace_decimal(p[1]);
        else if (p[0] == "transfers" && p.size() >= 2) m.transfers = trace_decimal(p[1]);
        if (p[0] != "c" || p.size() < 5) return;
        const std::uint32_t from = trace_hex32(p[1]), to = trace_hex32(p[2]);
        const std::uint64_t count = trace_decimal(p[4]);
        if (from >= kRing0 || to >= kRing0) {
            m.ring0 += count;
            return;
        }
        std::vector<const InstalledImage*> targets = holding(to), sources = holding(from);
        std::erase_if(targets, [&](const InstalledImage* i) { return !i->exports.contains(to - i->base); });
        if (targets.empty()) {
            m.not_export += count;
            return;
        }
        std::erase_if(sources, [&](const InstalledImage* i) { return !i->has_transfer || !i->has_transfer(from - i->base); });
        if (sources.empty()) {
            m.no_source += count;
            return;
        }
        if (!one_name(targets) || !one_name(sources)) {
            m.ambiguous += count;
            return;
        }
        const InstalledImage& target = *targets[0];
        const InstalledImage& source = *sources[0];
        if (source.name == target.name) {
            m.inside += count;
            return;
        }
        m.named += count;
        channels[{source.name, target.name}] += count;
        functions[{target.name, target.exports.at(to - target.base), source.name}] += count;
    });
    if (hz == 0) return std::nullopt;
    m.seconds = static_cast<double>(cycles) / static_cast<double>(hz);
    for (const auto& [key, count] : channels) m.channels.push_back({key.first, key.second, count});
    for (const auto& [key, count] : functions) m.functions.push_back({std::get<0>(key), std::get<1>(key), std::get<2>(key), count});
    std::stable_sort(m.channels.begin(), m.channels.end(), [](const auto& a, const auto& b) { return a.count > b.count; });
    std::stable_sort(m.functions.begin(), m.functions.end(), [](const auto& a, const auto& b) { return a.count > b.count; });
    return m;
}

void save_measured(const stdfs::path& root, const Measured& m) {
    std::string out(kMeasuredHeader);
    out += '\n';
    out += std::format("seconds\t{:.3f}\ncounts\t{}\t{}\t{}\t{}\t{}\t{}\t{}\n", m.seconds, m.transfers, m.ring0, m.not_export, m.no_source, m.ambiguous, m.inside, m.named);
    for (const MeasuredChannel& c : m.channels) out += "c\t" + c.from + '\t' + c.to + '\t' + std::to_string(c.count) + '\n';
    for (const MeasuredFunction& f : m.functions) out += "f\t" + f.module + '\t' + f.function + '\t' + f.user + '\t' + std::to_string(f.count) + '\n';
    fs::make_dirs(root);
    fs::write_file_atomic(root / "measured.tsv", as_bytes(out));
}

std::optional<Measured> load_measured(const stdfs::path& root) {
    std::error_code ec;
    if (!stdfs::is_regular_file(root / "measured.tsv", ec)) return std::nullopt;
    const std::string all = fs::read_text(root / "measured.tsv");
    if (!all.starts_with(kMeasuredHeader)) return std::nullopt;
    Measured m;
    trace_each_line(all, [&](std::string_view line) {
        const std::vector<std::string_view> p = trace_tabs(line);
        if (p[0] == "seconds" && p.size() >= 2) {
            const std::string_view s = p[1];
            const std::size_t dot = s.find('.');
            m.seconds = static_cast<double>(trace_decimal(s.substr(0, dot)));
            if (dot != std::string_view::npos) {
                double place = 0.1;
                for (const char c : s.substr(dot + 1)) {
                    m.seconds += (c - '0') * place;
                    place /= 10;
                }
            }
        } else if (p[0] == "counts" && p.size() >= 8) {
            m.transfers = trace_decimal(p[1]);
            m.ring0 = trace_decimal(p[2]);
            m.not_export = trace_decimal(p[3]);
            m.no_source = trace_decimal(p[4]);
            m.ambiguous = trace_decimal(p[5]);
            m.inside = trace_decimal(p[6]);
            m.named = trace_decimal(p[7]);
        } else if (p[0] == "c" && p.size() >= 4) {
            m.channels.push_back({std::string(p[1]), std::string(p[2]), trace_decimal(p[3])});
        } else if (p[0] == "f" && p.size() >= 5) {
            m.functions.push_back({std::string(p[1]), std::string(p[2]), std::string(p[3]), trace_decimal(p[4])});
        }
    });
    if (m.seconds <= 0) return std::nullopt;
    return m;
}

}  // namespace cb::app::board
