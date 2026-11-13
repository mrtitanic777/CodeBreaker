#include "cb/exact/bss.hpp"

#include <algorithm>
#include <format>
#include <set>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"

namespace cb::exact::bss {

namespace {

bool bss_word_char(char c) { return std::isalnum(static_cast<unsigned char>(c)) != 0 || c == '_'; }

std::uint16_t bss_word(ByteView b, std::size_t k) {
    if (k + 2 > b.size()) throw TruncatedError("exact bss", k, 2, b.size() > k ? b.size() - k : 0);
    return static_cast<std::uint16_t>(b[k] | (b[k + 1] << 8));
}

bool bss_ends_with(const std::string& name, std::string_view suffix) {
    if (name.size() < suffix.size()) return false;
    for (std::size_t i = 0; i < suffix.size(); ++i)
        if (std::tolower(static_cast<unsigned char>(name[name.size() - suffix.size() + i])) != suffix[i]) return false;
    return true;
}

// The files of the sources' folder whose names end as asked (any capitals), in name order: (file name, path).
std::vector<std::pair<std::string, std::filesystem::path>> bss_files(const std::filesystem::path& src, std::initializer_list<std::string_view> endings,
                                                                     char first = '\0') {
    std::vector<std::pair<std::string, std::filesystem::path>> out;
    std::error_code ec;
    for (const auto& entry : std::filesystem::directory_iterator(src, ec)) {
        if (!entry.is_regular_file(ec)) continue;
        const std::string name = entry.path().filename().string();
        if (first != '\0' && (name.empty() || std::tolower(static_cast<unsigned char>(name[0])) != first)) continue;
        if (std::any_of(endings.begin(), endings.end(), [&](std::string_view e) { return bss_ends_with(name, e); })) out.emplace_back(name, entry.path());
    }
    std::sort(out.begin(), out.end());
    return out;
}

}  // namespace

Proposal propose(project::Project& project, const library::Map* map) {
    std::optional<library::Map> own;
    if (map == nullptr) {
        const x86::Decoder16* decoder = project.services().decoder;
        own = library::locate(*project.as_library(), decoder ? *decoder : x86::capstone_decoder());
        map = &*own;
    }
    const ByteView img = project.image();
    const std::int64_t dg = project.dgroup();
    std::int64_t edata = static_cast<std::int64_t>(img.size()) - dg;
    std::int64_t stack = project.header() ? static_cast<std::int64_t>((*project.header())[7]) * 16 - dg : edata;
    if (const auto& packed = project.packed()) {   // (an image unpacked from EXEPACK holds its variables as zeros: the packed file says where its data ended, and where its stack is)
        edata = static_cast<std::int64_t>(packed->dest) * 16 - dg;
        stack = static_cast<std::int64_t>(packed->ss) * 16 - dg;
    }
    const std::shared_ptr<const project::ProfileParts> profile = project.profile_parts();

    std::set<std::string> lib_defined;   // what the placed modules of the library define (membership only)
    for (const auto* group : {&map->coded, &map->donly, &map->first})
        for (const library::Placed& p : *group) {
            for (const auto& [name, place] : p.module->pubs) lib_defined.insert(name);
            for (const library::Member* m : p.also)
                for (const auto& [name, place] : m->pubs) lib_defined.insert(name);
        }
    std::set<std::string> linker;
    if (profile->link) linker.insert(profile->link->names.begin(), profile->link->names.end());
    else linker = {"_edata", "_end"};
    std::set<std::string> funcs;   // the one-function files name their functions
    for (const auto& [name, path] : bss_files(project.src(), {".c"}, 'f')) funcs.insert("_" + name.substr(0, name.size() - 2));
    const auto skip = [&](const std::string& n) { return lib_defined.contains(n) || linker.contains(n) || funcs.contains(n) || n.find_first_of(":@") != std::string::npos; };

    const SymbolTable syms = project.symbols();
    // every word of the sources, and each without its leading underscores
    std::set<std::string> words;
    for (const auto& [name, path] : bss_files(project.src(), {".c", ".h"})) {
        const std::string t = fs::read_text(path);
        for (std::size_t i = 0; i < t.size();) {
            if (!bss_word_char(t[i])) {
                ++i;
                continue;
            }
            std::size_t e = i;
            while (e < t.size() && bss_word_char(t[e])) ++e;
            words.insert(t.substr(i, e - i));
            i = e;
        }
    }
    std::set<std::string> stripped;
    for (const std::string& w : words) stripped.insert(w.substr(w.find_first_not_of('_') == std::string::npos ? w.size() : w.find_first_not_of('_')));
    const auto in_sources = [&](const std::string& n) {   // a name no source uses any more is not the program's
        const std::string bare = n.substr(n.find_first_not_of('_') == std::string::npos ? n.size() : n.find_first_not_of('_'));
        return words.contains(bare) || stripped.contains(bare);
    };

    Proposal out;
    out.edata = edata;
    out.stack = stack;
    std::map<std::string, std::int64_t>& names = out.names;
    for (const auto& [n, place] : syms)
        if (place.kind == SymKind::Data && edata <= place.address && place.address < stack && !skip(n) && in_sources(n)) names[n] = place.address;

    // the data the sources name by its place (g_XXXX): defined in the data files, or only used
    static const Regex defined_rx(R"(^(?:static )?(?:unsigned |struct \w+ |union \w+ )?\w+ \**(g_[0-9a-f]{4})\b)", RegexFlags::Multiline);
    static const Regex data_file_rx(R"([de][0-9a-f]{5}\.c)");
    static const Regex used_rx(R"(\bg_([0-9a-f]{4})\b)");
    std::set<std::string> defined, used;
    for (const auto& [name, path] : bss_files(project.src(), {".c"})) {
        const std::string t = fs::read_text(path);
        if (data_file_rx.full_match(name).has_value() || bss_ends_with(name, "-data.c"))
            defined_rx.for_each_match(t, [&](const RegexMatch& m) {
                defined.emplace(m.group(1));
                return true;
            });
        used_rx.for_each_match(t, [&](const RegexMatch& m) {
            used.emplace(m.group(1));
            return true;
        });
    }
    for (const std::string& x : used) {
        const std::string n = "_g_" + x;
        const std::int64_t at = std::stoll(x, nullptr, 16);
        if (!defined.contains("g_" + x) && !names.contains(n) && !skip(n) && !syms.contains(n) && edata <= at && at < stack) names[n] = at;
    }
    // the library's communal names, by the words of its code and data that name them
    for (const auto& [n, ref] : map->all_refs)
        if (ref.kind == library::RefKind::Data && edata <= ref.address && ref.address < stack && !names.contains(n) && !skip(n)) names[n] = ref.address;

    // the library modules' own _BSS: where their code points into it
    std::vector<Hole> holes;
    for (const library::Placed& p : map->coded) {
        const library::Member& m = *p.module;
        const std::uint32_t length = m.seglen("_BSS").length;
        if (length == 0 || m.code.empty()) continue;
        const std::string& s0 = m.code.front().segment;
        const Bytes* code = m.object.segments.find(s0);
        if (code == nullptr) continue;
        for (const formats::omf::Fixup& fx : m.object.fixups) {
            if (fx.segment != s0 || fx.kind != formats::omf::FixupKind::Offset || fx.target.kind != formats::omf::Target::Kind::Segment || fx.target.name != "_BSS" ||
                fx.at < 0 || static_cast<std::size_t>(fx.at) + 2 > code->size())
                continue;
            const std::int64_t at = (bss_word(img, static_cast<std::size_t>(*p.code + fx.at)) - ((bss_word(*code, static_cast<std::size_t>(fx.at)) + fx.displacement) & 0xFFFF)) & 0xFFFF;
            holes.push_back({at, length, {m.label()}});
            break;
        }
    }
    std::sort(holes.begin(), holes.end(), [](const Hole& a, const Hole& b) { return a.address != b.address ? a.address < b.address : a.length != b.length ? a.length < b.length : a.modules < b.modules; });
    std::vector<Hole> merged;
    for (const Hole& h : holes) {
        if (!merged.empty() && h.address <= merged.back().address + merged.back().length + 1) {
            Hole& last = merged.back();
            last.length = std::max(last.length, h.address + h.length - last.address);
            last.modules.insert(last.modules.end(), h.modules.begin(), h.modules.end());
        } else {
            merged.push_back(h);
        }
    }
    if (profile->communal) {   // a compiler whose communal names go to a segment of their own after every module's _BSS (c_common)
        std::int64_t lo = edata;   // (behind the library's _BSS: an unpacked image can hold its variables as zeros, so not the image's end)
        if (!merged.empty()) {
            lo = merged.front().address + merged.front().length;
            for (const Hole& h : merged) lo = std::max(lo, h.address + h.length);
        }
        if (profile->communal->para) lo = (dg + lo + 15) / 16 * 16 - dg;
        std::erase_if(names, [lo](const auto& kv) { return kv.second < lo; });
        out.edata = lo;
        return out;
    }
    out.holes = std::move(merged);
    return out;
}

std::vector<std::string> lines(const std::optional<project::Communal>& communal, const Proposal& p) {
    std::vector<std::string> out = {"# the variables that begin without a value (codebreaker exact bss): their range; a library module's",
                                    "# own _BSS between them (hole: its place and length); then a name and its DS offset each (hex)",
                                    std::format("range {:04x} {:04x}", p.edata, p.stack)};
    if (communal)
        out.push_back(std::format("segment {}{}     # (the compiler's segment of communal names: after the library)", communal->segment, communal->para ? " para" : ""));
    for (const Hole& h : p.holes) out.push_back(std::format("hole {:04x} {:x}     # the _BSS of the library's {}", h.address, h.length, text::join(h.modules, ", ")));
    std::vector<std::pair<std::string, std::int64_t>> sorted(p.names.begin(), p.names.end());
    std::sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) { return a.second != b.second ? a.second < b.second : a.first < b.first; });
    for (const auto& [n, a] : sorted) out.push_back(std::format("{} {:04x}", n, a));
    return out;
}

int run(project::Project& project, const std::vector<std::string>& args) {
    const Proposal p = propose(project);
    const std::string txt = text::join(lines(project.profile_parts()->communal, p), "\n") + "\n";
    out("{}\n", txt);
    if (std::find(args.begin(), args.end(), "--write") != args.end()) fs::write_text(project.path("BSS"), txt);
    return 0;
}

}  // namespace cb::exact::bss
