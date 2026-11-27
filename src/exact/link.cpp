#include "exact/link.hpp"

#include <algorithm>
#include <format>
#include <map>
#include <set>

#include "core/crypto.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/log.hpp"
#include "core/regex.hpp"
#include "core/text.hpp"
#include "exact/library.hpp"
#include "exact/modules.hpp"
#include "exact/recut.hpp"
#include "exact/standin.hpp"
#include "formats/omf.hpp"

namespace cb::exact::link {

namespace {

using linkwhole::LinkError;

// The project's toolchain and its linker part. UsageError when the profile has none.
const project::LinkSpec& lk_spec(project::Project& project) {
    const std::shared_ptr<const project::ProfileParts> parts = project.profile_parts();
    if (!parts->link) throw UsageError(std::format("the toolchain {} has no linker (its profile's \"link\")", parts->name));
    return *parts->link;   // (the parts are kept by the project)
}

std::filesystem::path lk_kit_file(project::Project& project, const project::LinkSpec& spec, std::string_view path) {
    toolchain::Toolchains* toolchains = project.services().toolchains;
    if (toolchains == nullptr) throw UsageError("linking needs the toolchains (Services::toolchains)");
    const std::shared_ptr<const project::ProfileParts> parts = project.profile_parts();
    return toolchains->locations().kit_path(spec.kit ? spec.kit : parts->kit, path);
}

std::string lk_dir_file(const project::LinkSpec& spec, std::string_view name) {
    return spec.dir ? (std::filesystem::path(*spec.dir) / std::string(name)).generic_string() : std::string(name);
}

// img[a, b) as the original takes a slice: what is there.
ByteView lk_slice(ByteView v, std::int64_t a, std::int64_t b) {
    const std::int64_t size = static_cast<std::int64_t>(v.size());
    a = std::clamp<std::int64_t>(a, 0, size);
    b = std::clamp<std::int64_t>(b, 0, size);
    return a >= b ? ByteView() : v.subspan(static_cast<std::size_t>(a), static_cast<std::size_t>(b - a));
}

std::uint16_t lk_word(ByteView v, std::size_t at) {
    if (at + 2 > v.size()) throw TruncatedError("exact link", at, 2, v.size() > at ? v.size() - at : 0);
    return static_cast<std::uint16_t>(v[at] | (v[at + 1] << 8));
}

std::string lk_stem(std::string_view file) { return std::filesystem::path(std::string(file)).stem().string(); }

std::vector<std::string> lk_lines_of(const std::filesystem::path& file) {
    std::vector<std::string> out;
    const std::string contents = fs::read_text(file);   // (the lines are views of it)
    for (std::string_view l : text::lines(contents)) out.emplace_back(l);
    return out;
}

// The text of a line before its comment, trimmed.
std::string lk_code_of(std::string_view line) { return std::string(text::trim(line.substr(0, line.find('#')))); }

bool lk_exists(const std::filesystem::path& p) {
    std::error_code ec;
    return std::filesystem::exists(p, ec);
}

modules::CompileOptions lk_compile_options(const modules::SetOf& set) {
    return modules::CompileOptions{set.own->headers(), set.own->include(), std::nullopt};
}

}  // namespace

// ---- the objects -----------------------------------------------------------------------------------------------------------

std::uint32_t library_start(project::Project& project) {
    const std::filesystem::path p = project.path("CRTLIB");
    std::vector<std::uint32_t> at;
    if (lk_exists(p)) {
        static const Regex code_rx(R"(\bcode[= ]([0-9a-f]{5})\b)");
        for (const std::string& l : lk_lines_of(p))
            code_rx.for_each_match(l, [&](const RegexMatch& m) {
                at.push_back(static_cast<std::uint32_t>(std::stoul(std::string(m.group(1)), nullptr, 16)));
                return true;
            });
    }
    return at.empty() ? project.own_end() : *std::min_element(at.begin(), at.end());
}

Objects objects(project::Project& project, bool verbose) {
    const project::LinkSpec& spec = lk_spec(project);
    const std::shared_ptr<const modules::Project> view = project.as_modules();
    std::vector<modules::Row> rows = modules::rows(*view);
    std::vector<std::uint32_t> starts;
    for (const modules::Row& r : rows)
        if (r.code_start) starts.push_back(*r.code_start);
    std::sort(starts.begin(), starts.end());
    starts.push_back(library_start(project));
    if (project.config().link_order == "name")   // (the objects named to the linker in the order of their names -- the data segment shows it)
        std::stable_sort(rows.begin(), rows.end(), [](const modules::Row& a, const modules::Row& b) { return text::lower(lk_stem(a.name)) < text::lower(lk_stem(b.name)); });
    const modules::FunctionTypes types = modules::FunctionTypes::read(project.src());
    const std::vector<project::NamedToolchain> toolchains = project.toolchains();
    const x86::Decoder16* decoder = project.services().decoder;
    const project::StandinInput standin_input = project.standin_input();
    Objects out;
    for (const modules::Row& row : rows) {
        const std::string title = text::upper(lk_stem(row.name));
        if (text::iends_with(row.name, ".OBJ")) {
            out.objects.push_back({title, fs::read_file(lk_kit_file(project, spec, lk_dir_file(spec, text::upper(row.name))))});
            continue;
        }
        modules::Compiled first = modules::compile_module(*view, row, types, toolchains.at(0).toolchain.get());
        if (!first.object) {
            const std::string tail = first.console.size() > 1500 ? first.console.substr(first.console.size() - 1500) : first.console;
            throw LinkError(std::format("{} does not compile:\n{}", row.name, tail));
        }
        Bytes raw = std::move(*first.object);
        if (row.code_start && !first.addresses.empty()) {
            std::vector<Bytes> alternatives;
            for (std::size_t i = 1; i < toolchains.size(); ++i) {
                modules::Compiled other = modules::compile_module(*view, row, types, toolchains[i].toolchain.get());
                if (other.object) alternatives.push_back(std::move(*other.object));
            }
            std::uint32_t end = row.code_end && *row.code_end != 0 ? *row.code_end : UINT32_MAX;
            for (std::uint32_t s : starts)
                if (s > *row.code_start) end = std::min(end, s);
            standin::Rebuilt rebuilt = standin::rebuild(standin_input.program(), raw, first.addresses, end, title, alternatives, decoder ? *decoder : x86::capstone_decoder());
            raw = std::move(rebuilt.object);
            if (verbose && !rebuilt.stand_ins.empty()) out.notes.push_back(std::format("   {}: stand-ins {}", row.name, text::join(rebuilt.stand_ins, " ")));
            out.stand_ins.insert(out.stand_ins.end(), rebuilt.stand_ins.begin(), rebuilt.stand_ins.end());
        }
        if (row.code_start && project.config().recut) raw = recut::recut(project.recut_input().original(), raw);   // (its code's records cut as the program's relocations show)
        out.objects.push_back({title, std::move(raw)});
    }
    return out;
}

std::vector<NamedObject> first_objects(project::Project& project) {
    const project::LinkSpec& spec = lk_spec(project);
    const std::filesystem::path p = project.path("FIRSTOBJ");
    std::vector<NamedObject> out;
    if (!lk_exists(p)) return out;
    for (const std::string& l : lk_lines_of(p)) {
        const std::string f = lk_code_of(l);
        if (!f.empty()) out.push_back({lk_stem(f), fs::read_file(lk_kit_file(project, spec, lk_dir_file(spec, f)))});
    }
    return out;
}

BssObjects bss_objects(project::Project& project) {
    BssObjects out;
    const std::filesystem::path p = project.path("BSS");
    if (!lk_exists(p)) return out;
    OrderedMap<std::string, std::int64_t> names;
    std::optional<std::pair<std::int64_t, std::int64_t>> range;
    std::vector<std::pair<std::int64_t, std::int64_t>> holes;
    std::optional<std::pair<std::string, bool>> segment;
    auto hex = [](std::string_view s) { return std::stoll(std::string(s), nullptr, 16); };
    for (const std::string& l : lk_lines_of(p)) {
        const std::string code = lk_code_of(l);
        const std::vector<std::string_view> f = text::split_ws(code);
        if (f.empty()) continue;
        if (f[0] == "segment") {   // (communal names in a segment of their own: after all)
            if (f.size() < 2) throw ParseError("src/BSS", 0, 0, "a segment line without a name");
            segment = {std::string(f[1]), std::find(f.begin() + 2, f.end(), "para") != f.end()};
        } else if (f[0] == "range") {
            if (f.size() < 3) throw ParseError("src/BSS", 0, 0, "a range line without its limits");
            range = {hex(f[1]), hex(f[2])};
        } else if (f[0] == "hole") {
            if (f.size() < 3) throw ParseError("src/BSS", 0, 0, "a hole line without its place and length");
            holes.emplace_back(hex(f[1]), hex(f[2]));
        } else {
            if (f.size() < 2) throw ParseError("src/BSS", 0, 0, "a name line without its place");
            names.insert_or_assign(std::string(f[0]), hex(f[1]));
        }
    }
    if (!range) throw ParseError("src/BSS", 0, 0, "no range line");
    std::vector<std::int64_t> cuts = {range->first};
    for (const auto& [a, n] : holes) {
        cuts.push_back(a);
        cuts.push_back(a + n);
    }
    cuts.push_back(range->second);
    for (std::size_t k = 0; k + 1 < cuts.size(); k += 2) {
        const std::int64_t lo = cuts[k], hi = cuts[k + 1];
        const std::string name = std::format("bssvar{}", k / 2);
        OrderedMap<std::string, std::int64_t> here;
        for (const auto& [n, a] : names)
            if (lo <= a && a < hi) here.insert_or_assign(n, a);
        out.objects.push_back({name, formats::omf::bss_object(here, lo, hi, name, segment ? segment->first : "_BSS", segment ? segment->second : false)});
    }
    out.after = segment.has_value();
    return out;
}

CrtResult crt(project::Project& project, const std::vector<NamedObject>& objs) {
    CrtResult out;
    const std::filesystem::path p = project.path("CRTLIB");
    if (!lk_exists(p)) return out;
    std::map<std::string, Bytes> mods;   // the library's modules by label (and what CRTLIB adds): looked up
    for (const std::unique_ptr<library::Member>& m : library::read_modules(*project.as_library())) mods[m->label()] = m->raw;
    const ByteView img = project.image();
    const std::int64_t dg = project.dgroup();
    std::vector<std::string> order, always;
    OrderedMap<std::string, std::vector<std::string>> group;   // (modules of the same bytes at one place: one is taken)
    std::set<std::string> proven;
    static const Regex code_note_rx(R"(\bcode ([0-9a-f]{5})\b)");
    static const Regex place_note_rx(R"(^\s*(?:[0-9a-f]{5}\b|\(data only))");
    static const Regex data_note_rx(R"(^\s*data [0-9a-f]{4}\b)");
    static const Regex note_rx(R"(\s#)");
    const std::vector<std::string> lines = lk_lines_of(p);
    for (const std::string& l : lines) {
        if (l.starts_with('#')) continue;
        std::string body = l, note;
        if (const auto m = note_rx.search(l)) {   // (the note begins at " #": a name may hold a #, NAME#2)
            body = l.substr(0, m->start());
            note = l.substr(m->end());
        }
        const std::vector<std::string_view> words = text::split_ws(body);
        if (words.empty()) continue;
        std::vector<std::string> f(words.begin(), words.end());
        if (text::iends_with(f[0], ".asm") && f[0].find(':') == std::string::npos && !mods.contains(f[0])) {   // (a transcription of the program's bytes: src/crt)
            const modules::SetOf set = modules::set_of(*project.as_modules(), f[0]);
            const modules::CompileResult c = set.toolchain->compile(fs::read_file(project.src() / "crt" / f[0]), lk_compile_options(set));
            if (!c.object) throw LinkError(std::format("crt/{} does not assemble:\n{}", f[0], c.console.size() > 600 ? c.console.substr(c.console.size() - 600) : c.console));
            mods[f[0]] = project.config().recut ? recut::recut(project.recut_input().original(), *c.object) : *c.object;
        }
        if (f.size() > 1 && f[1].starts_with('!')) {   // TAG:NAME !SEG+OFF=HEX: a byte or two of its data as the program has them (the version linked had them)
            const std::string key = f[0] + " " + f[1];
            if (!mods.contains(key) && mods.contains(f[0])) mods[key] = formats::omf::patched(mods.at(f[0]), std::string_view(f[1]).substr(1));
            f = {key};
        }
        const auto mc = code_note_rx.search(note);
        if (!mc && place_note_rx.search(note) && f[0] != "RAW") always.push_back(f[0]);   // (a place alone, or "data only": a module written down as found in the program)
        const bool md = data_note_rx.search(note).has_value();
        if (f[0] != "RAW" && (mc || md)) {   // (found in the program by its bytes: it is there)
            std::vector<std::string>& ms = group.try_emplace(mc ? std::string(mc->group(1)) : "data:" + f[0]).first->second;
            if (note.find("the same bytes as") != std::string::npos) ms.push_back(f[0]);   // (the one the library stage took goes first)
            else ms.insert(ms.begin(), f[0]);
            if (note.find("the one whose calls go where") != std::string::npos) proven.insert(f[0]);   // (told apart by their calls: that one is the program's, whatever the objects name)
        }
        if (f[0] == "RAW") {   // RAW NAME code=LO-HI [data=LO-HI] [pub=N@OFF ..] [dpub=..]
            if (f.size() < 2) throw ParseError("src/CRTLIB", 0, 0, "a RAW line without a name");
            std::map<std::string, std::vector<std::string>> kv;
            for (std::size_t i = 2; i < f.size(); ++i) {
                const std::size_t eq = f[i].find('=');
                kv[f[i].substr(0, eq)].push_back(eq == std::string::npos ? "" : f[i].substr(eq + 1));
            }
            const auto range = [&](const char* key) {
                std::pair<std::int64_t, std::int64_t> r{0, 0};
                const auto it = kv.find(key);
                if (it == kv.end()) return r;
                const std::size_t dash = it->second[0].find('-');
                r.first = std::stoll(it->second[0].substr(0, dash), nullptr, 16);
                r.second = dash == std::string::npos ? 0 : std::stoll(it->second[0].substr(dash + 1), nullptr, 16);
                return r;
            };
            const auto publics = [&](const char* key) {   // name@OFFSET, comma separated
                OrderedMap<std::string, std::int64_t> ps;
                const auto it = kv.find(key);
                if (it == kv.end()) return ps;
                for (const std::string& v : it->second)
                    for (std::string_view y : text::split(v, ',')) {
                        if (y.empty()) continue;
                        const std::size_t at = y.find('@');
                        if (at == std::string_view::npos) throw ParseError("src/CRTLIB", 0, 0, "a public without its offset: " + std::string(y));
                        ps.insert_or_assign(std::string(y.substr(0, at)), std::stoll(std::string(y.substr(at + 1)), nullptr, 16));
                    }
                return ps;
            };
            const auto [c0, c1] = range("code");
            const auto [d0, d1] = range("data");
            mods[f[1]] = formats::omf::raw_object(f[1], lk_slice(img, c0, c1), publics("pub"), d1 != 0 ? lk_slice(img, dg + d0, dg + d1) : ByteView(), publics("dpub"),
                                                  "_TEXT", d1 != 0 && d0 % 2 == 0 ? 2 : 1);
            order.push_back(f[1]);
            if (!kv.contains("pub") && !kv.contains("dpub")) always.push_back(f[1]);   // (a piece no name calls for: always taken)
            continue;
        }
        order.push_back(f[0]);
    }
    std::vector<std::string> unknown;
    for (const std::string& m : order)
        if (!mods.contains(m)) unknown.push_back(m);
    if (!unknown.empty()) throw LinkError("CRTLIB names modules no library of the profile has: " + text::join(unknown, " "));

    std::map<std::string, std::pair<std::set<std::string>, std::set<std::string>>> names;   // module -> (publics, externals): worked out once
    const auto names_of = [&](const std::string& m) -> const std::pair<std::set<std::string>, std::set<std::string>>& {
        const auto it = names.find(m);
        return it != names.end() ? it->second : names.emplace(m, formats::omf::names_of(mods.at(m))).first->second;
    };
    std::set<std::string> have, need;
    for (const NamedObject& o : objs) {
        const auto [pb, e] = formats::omf::names_of(o.object);
        have.insert(pb.begin(), pb.end());
        need.insert(e.begin(), e.end());
    }
    std::vector<std::string> take;
    const auto add = [&](const std::string& m) {
        take.push_back(m);
        const auto& [pb, e] = names_of(m);
        have.insert(pb.begin(), pb.end());
        need.insert(e.begin(), e.end());
    };
    std::set<std::string> others;
    for (const auto& [at, ms] : group)
        if (std::any_of(ms.begin(), ms.end(), [&](const std::string& x) { return proven.contains(x); }))
            for (const std::string& m : ms)
                if (!proven.contains(m)) others.insert(m);
    const auto in_take = [&](const std::string& m) { return std::find(take.begin(), take.end(), m) != take.end(); };
    const auto close = [&] {
        while (true) {
            std::set<std::string> miss;
            std::set_difference(need.begin(), need.end(), have.begin(), have.end(), std::inserter(miss, miss.end()));
            const std::string* next = nullptr;
            for (const std::string& m : order) {
                if (in_take(m) || others.contains(m)) continue;
                const auto& defined = names_of(m).first;
                if (std::any_of(defined.begin(), defined.end(), [&](const std::string& n) { return miss.contains(n); })) {
                    next = &m;
                    break;
                }
            }
            if (next == nullptr) return;
            add(*next);
        }
    };
    for (const std::string& m : always) add(m);
    close();
    // what the library stage found by its bytes is in the program: a group of the same bytes none of whose members anything names is
    // taken all the same (its first: a module only a stand-in calls)
    for (const auto& [at, ms] : group)
        if (!std::any_of(ms.begin(), ms.end(), in_take)) {
            add(ms.front());
            close();
        }
    for (const auto& [at, ms] : group) {   // (the program has one routine where ours names two)
        std::vector<std::string> both;
        for (const std::string& m : ms)
            if (in_take(m)) both.push_back(m);
        if (both.size() < 2) continue;
        std::vector<std::string> firsts;
        for (const std::string& m : both) firsts.push_back(names_of(m).first.empty() ? std::string() : *names_of(m).first.begin());
        std::sort(firsts.begin(), firsts.end());
        out.conflicts.push_back(std::format("the program has one routine at {} for {}; ours names {} - the sources call it by another name than the program's "
                                            "(a name the library's modules use is the program's)",
                                            at, text::join(both, " and "), text::join(firsts, " and ")));
    }
    for (const std::string& m : order)
        if (in_take(m)) out.modules.push_back({m, mods.at(m)});
    std::set_difference(need.begin(), need.end(), have.begin(), have.end(), std::back_inserter(out.missing));
    return out;
}

std::string dos_name(std::string_view name) {
    std::string out;
    for (char c : name) {
        const char u = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
        if ((u >= 'A' && u <= 'Z') || (u >= '0' && u <= '9')) out += u;
    }
    if (out.size() > 8) out.resize(8);
    return out.empty() ? "M" : out;
}

// ---- running the linker ------------------------------------------------------------------------------------------------------

linkwhole::LinkOutput run_link(project::Project& project, const std::vector<NamedObject>& objs, const linkwhole::DosHost& dos,
                               const std::optional<std::filesystem::path>& keep, std::string_view options) {
    const project::LinkSpec& spec = lk_spec(project);
    std::optional<fs::ScopedTempDir> temp;
    if (keep) fs::make_dirs(*keep);
    else temp.emplace("exlink");
    const std::filesystem::path w = keep ? *keep : temp->path();
    fs::write_file(w / "LINK.EXE", fs::read_file(lk_kit_file(project, spec, *spec.run)));
    std::vector<std::string> libs;
    if (const auto it = spec.libs.find(project.config().model); it != spec.libs.end()) libs = it->second;
    if (lk_exists(project.path("CRTLIB"))) libs.clear();   // (the library's part goes in as objects: a name left open -- one only a stand-in used -- must not pull a module in)
    std::set<std::string> copies(libs.begin(), libs.end());
    copies.insert(spec.copy.begin(), spec.copy.end());
    for (const std::string& l : copies) {
        const std::filesystem::path source = lk_kit_file(project, spec, lk_dir_file(spec, l));
        fs::write_file(w / source.filename(), fs::read_file(source));
    }
    std::vector<std::string> names;
    for (const NamedObject& o : objs) {
        std::string dn = dos_name(o.name);
        int k = 1;
        while (std::find(names.begin(), names.end(), dn) != names.end()) {   // (DOS names: _exit and exit)
            dn = dos_name(o.name).substr(0, 6) + std::format("{:02}", k);
            ++k;
        }
        names.push_back(dn);
        fs::write_file(w / (dn + ".OBJ"), o.object);
    }
    std::string rsp;
    for (std::size_t k = 0; k < names.size(); k += 6) {
        std::vector<std::string> chunk(names.begin() + static_cast<std::ptrdiff_t>(k), names.begin() + static_cast<std::ptrdiff_t>(std::min(names.size(), k + 6)));
        rsp += text::join(chunk, "+") + (k + 6 < names.size() ? "+" : "") + "\r\n";
    }
    rsp += std::format("OUT.EXE\r\nOUT.MAP\r\n{}{};\r\n", text::join(libs, "+"), options);
    fs::write_text(w / "L.LNK", rsp);
    const auto psp = static_cast<std::uint32_t>(spec.psp);
    linkwhole::LinkOutput out;
    std::string console = dos.run(w / "LINK.EXE", w, "@L.LNK", psp);
    console.erase(std::remove(console.begin(), console.end(), '\r'), console.end());
    out.console = text::from_latin1(ByteView(reinterpret_cast<const std::uint8_t*>(console.data()), console.size()));
    const std::filesystem::path mf = project.path("EXEMOD");
    if (lk_exists(w / "OUT.EXE") && lk_exists(mf) && spec.exemod) {   // (the header changed after the link, by the kit's EXEMOD: the checksum stays the link's)
        fs::write_file(w / "EXEMOD.EXE", fs::read_file(lk_kit_file(project, spec, *spec.exemod)));
        for (const std::string& l : lk_lines_of(mf)) {
            const std::string a = lk_code_of(l);
            if (a.empty()) continue;
            const std::string said = dos.run(w / "EXEMOD.EXE", w, "OUT.EXE " + a, psp);
            out.console += text::from_latin1(ByteView(reinterpret_cast<const std::uint8_t*>(said.data()), said.size()));
        }
    }
    if (lk_exists(w / "OUT.EXE")) out.exe = fs::read_file(w / "OUT.EXE");
    if (lk_exists(w / "OUT.MAP")) out.map = text::from_latin1(fs::read_file(w / "OUT.MAP"));
    return out;
}

// ---- the comparison ------------------------------------------------------------------------------------------------------------

const std::vector<std::string>& field_names() {
    static const std::vector<std::string> names = {"magic", "lastpage", "pages", "relocs", "hdrparas", "minalloc", "maxalloc", "ss", "sp", "csum", "ip", "cs", "reloctab", "ovl"};
    return names;
}

Parts parts(ByteView exe) {
    Parts p;
    for (std::size_t i = 0; i < 14; ++i) p.header[i] = lk_word(exe, i * 2);
    const std::int64_t size = static_cast<std::int64_t>(p.header[2]) * 512 - (p.header[1] != 0 ? 512 - p.header[1] : 0);
    const ByteView image = lk_slice(exe, static_cast<std::int64_t>(p.header[4]) * 16, size);
    p.image.assign(image.begin(), image.end());
    for (std::size_t k = 0; k < p.header[3]; ++k) {
        const std::size_t at = p.header[12] + 4 * k;
        p.relocations.push_back((static_cast<std::uint64_t>(lk_word(exe, at)) << 16) | lk_word(exe, at + 2));
    }
    return p;
}

std::vector<Shift> shifts(ByteView a, ByteView b, std::size_t step, std::size_t span, std::size_t reach) {
    std::vector<Shift> out;
    std::optional<std::int64_t> prev;
    for (std::size_t k = 0; k + span < a.size(); k += step) {
        const ByteView seg = a.subspan(k, span);
        if (std::set<std::uint8_t>(seg.begin(), seg.end()).size() <= 3) continue;
        std::optional<std::int64_t> found;
        std::vector<std::int64_t> tries = {0};
        for (std::int64_t r = 1; r < static_cast<std::int64_t>(reach / 2); ++r) {
            tries.push_back(2 * r);
            tries.push_back(-2 * r);
        }
        for (const std::int64_t d : tries) {
            const std::int64_t at = static_cast<std::int64_t>(k) + d;   // (a place before the start is none: a negative index would wrap)
            if (at >= 0 && static_cast<std::size_t>(at) + span <= b.size() && std::equal(seg.begin(), seg.end(), b.begin() + at)) {
                found = d;
                break;
            }
        }
        if (!found) continue;   // (a word that points elsewhere: not a move)
        if (found != prev) {
            out.push_back({k, *found});
            prev = found;
        }
    }
    return out;
}

std::vector<std::string> drift(ByteView a, ByteView b, std::size_t step, std::size_t span, std::size_t reach) {
    std::vector<std::string> out;
    for (const Shift& s : shifts(a, b, step, span, reach)) {
        if (out.size() == 16) break;
        out.push_back(std::format("DS:{:04x} on, ours is {:+}", s.at, s.by));
    }
    return out;
}

std::vector<std::string> diagnose(project::Project& project, const std::optional<ByteView>& reference, ByteView exe) {
    std::vector<std::string> out;
    const Parts ours = parts(exe);
    const auto& hb = ours.header;
    Bytes ia, ib = ours.image;
    std::vector<std::uint64_t> ra, rb = ours.relocations;
    std::optional<std::array<std::uint16_t, 14>> ha;
    if (!reference) {   // (the original packed: ours, linked unpacked, against the unpacked image -- its segment words as loaded at loadseg -- and the relocations EXEPACK kept, in their order)
        const formats::exepack::Original& packed = *project.packed();
        const ByteView unpacked = lk_slice(project.image(), 0, static_cast<std::int64_t>(packed.dest) * 16);
        ia.assign(unpacked.begin(), unpacked.end());
        ra.assign(packed.relocs.begin(), packed.relocs.end());
        for (std::uint64_t& r : rb) r = (r & 0xFFFF) * 16 + (r >> 16);   // (offset, segment) as the image offset
        for (std::uint64_t a : rb)
            if (a + 2 <= ib.size()) {
                const std::uint16_t w = static_cast<std::uint16_t>(lk_word(ib, a) + project.loadseg());
                ib[a] = static_cast<std::uint8_t>(w);
                ib[a + 1] = static_cast<std::uint8_t>(w >> 8);
            }
        if (ib.size() < ia.size()) ib.resize(ia.size(), 0);   // (its variables: zeros in the unpacked image)
        if (static_cast<std::uint32_t>(packed.ss) * 16 + packed.sp != static_cast<std::uint32_t>(hb[7]) * 16 + hb[8])
            out.push_back(std::format("the stack: theirs {:04x}:{:04x}, ours {:04x}:{:04x} (LINKOPT /STACK, the variables' length)", packed.ss, packed.sp, hb[7], hb[8]));
    } else {
        const Parts theirs = parts(*reference);
        ha = theirs.header;
        ia = theirs.image;
        ra = theirs.relocations;
    }
    if (ha)
        for (std::size_t i = 0; i < 14; ++i) {
            const std::string& f = field_names()[i];
            if ((*ha)[i] != hb[i] && f != "csum")
                out.push_back(std::format("header {:<8} theirs {:04x} ours {:04x}{}", f, (*ha)[i], hb[i], f == "minalloc" || f == "maxalloc" || f == "sp" ? "  (EXEMOD, LINKOPT: /STACK)" : ""));
        }
    std::vector<std::uint64_t> sa = ra, sb = rb;
    std::sort(sa.begin(), sa.end());
    std::sort(sb.begin(), sb.end());
    if (sa != sb) out.push_back(std::format("the relocations differ (theirs {}, ours {}): the program's or the library's far references", ra.size(), rb.size()));
    else if (ra != rb) out.push_back("the relocations are the same but in another order: the order of the objects and their records (fixups first, CRTLIB)");
    if (ia.size() != ib.size()) out.push_back(std::format("the image is {} bytes, ours {}", ia.size(), ib.size()));
    std::vector<std::size_t> bad;
    for (std::size_t x = 0; x < std::min(ia.size(), ib.size()); ++x)
        if (ia[x] != ib[x]) bad.push_back(x);
    if (bad.empty()) return out;

    const std::size_t first = bad.front();
    const std::int64_t dg = project.dgroup();
    const std::uint32_t own = project.own_end();
    const x86::Decoder16* decoder = project.services().decoder;
    const library::Map map = library::locate(*project.as_library(), decoder ? *decoder : x86::capstone_decoder());
    const std::int64_t lib0 = map.start && *map.start != 0 ? *map.start : own;
    const std::string where = static_cast<std::int64_t>(first) < std::min<std::int64_t>(own, lib0) ? "the program's own code (a module's functions or their order: MODULES)"
                              : static_cast<std::int64_t>(first) < dg                                ? "the library's part (CRTLIB: a module, its order, a stand-in)"
                                                                                                      : "the data segment (the modules' data: dADDR.c, the library's data, BSS)";
    std::vector<std::pair<std::size_t, std::size_t>> runs;
    std::size_t s = bad.front(), p = bad.front();
    for (std::size_t i = 1; i < bad.size(); ++i) {
        if (bad[i] > p + 8) {
            runs.emplace_back(s, p);
            s = bad[i];
        }
        p = bad[i];
    }
    runs.emplace_back(s, p);
    out.push_back(std::format("{} bytes of the image differ, in {} places; the first at {:05x}: {}", bad.size(), runs.size(), first, where));
    const auto moved_by = [&](ByteView mine, std::size_t at, std::size_t length, ByteView theirs, std::size_t theirs_at) -> std::optional<int> {
        for (int d = -64; d <= 64; ++d) {   // (ours the same bytes, moved: something before is too long)
            if (d == 0) continue;
            bool all = true;
            for (std::size_t k = 0; k < length && all; ++k) {
                const std::int64_t m = static_cast<std::int64_t>(at) + d + static_cast<std::int64_t>(k);
                all = m >= 0 && static_cast<std::size_t>(m) < mine.size() && theirs_at + k < theirs.size() && mine[static_cast<std::size_t>(m)] == theirs[theirs_at + k];
            }
            if (all) return d;
        }
        return std::nullopt;
    };
    if (const auto shift = moved_by(ib, first, 32, ia, first))
        out.push_back(std::format("   from there ours is the original moved by {:+} bytes: what comes before it is {} bytes {}", *shift, std::abs(*shift), *shift > 0 ? "too long" : "too short"));
    const auto hex = [](ByteView v, std::size_t a, std::size_t n) { return crypto::to_hex(lk_slice(v, static_cast<std::int64_t>(a), static_cast<std::int64_t>(a + n))); };
    for (std::size_t i = 0; i < runs.size() && i < 12; ++i) {
        const auto [a, b] = runs[i];
        const std::size_t n = std::min(b + 1, a + 10) - a;
        out.push_back(std::format("   {:05x}..{:05x}  theirs {}  ours {}", a, b, hex(ia, a, n), hex(ib, a, n)));
    }
    // the data segment on its own (a shift there moves every word of the code that points into it)
    const ByteView da = lk_slice(ia, dg, static_cast<std::int64_t>(ia.size())), db = static_cast<std::int64_t>(ib.size()) > dg ? lk_slice(ib, dg, static_cast<std::int64_t>(ib.size())) : ByteView();
    std::optional<std::size_t> dbad;
    for (std::size_t k = 0; k < std::min(da.size(), db.size()); ++k)
        if (da[k] != db[k]) {
            dbad = k;
            break;
        }
    if (dbad) {
        const std::vector<modules::Row> rows = modules::rows(*project.as_modules());
        const modules::Row* who = nullptr;   // the module whose data begins last at or before the first difference
        for (const modules::Row& r : rows)
            if (r.data_start && *r.data_start <= *dbad && (who == nullptr || *r.data_start > *who->data_start)) who = &r;
        struct LibData {
            std::int64_t data = 0;
            std::string label;
        };
        std::optional<LibData> lib;
        for (const library::OrderEntry& e : map.order) {
            std::optional<LibData> candidate;
            if (const auto* placed = std::get_if<const library::Placed*>(&e.what); placed && (*placed)->data) candidate = LibData{*(*placed)->data, (*placed)->module->label()};
            else if (const auto* raw = std::get_if<const library::DataRaw*>(&e.what)) candidate = LibData{(*raw)->data, (*raw)->member ? (*raw)->member->label() : (*raw)->name};
            if (candidate && candidate->data <= static_cast<std::int64_t>(*dbad) && (!lib || candidate->data > lib->data)) lib = candidate;
        }
        const bool name_library = lib && (who == nullptr || lib->data > *who->data_start);
        out.push_back(std::format("the data segment parts at DS:{:04x} (after the module {}{})", *dbad, who ? who->name : std::string("?"), name_library ? ", the library's " + lib->label : std::string()));
        if (const auto d = moved_by(db, *dbad, 24, da, *dbad))
            out.push_back(std::format("   from there ours is the program's moved by {:+}: what comes before is {} bytes {}", *d, std::abs(*d), *d > 0 ? "too long" : "too short"));
        out.push_back(std::format("   theirs {}\n   ours   {}", hex(da, *dbad, 24), hex(db, *dbad, 24)));
        for (const std::string& l : drift(da, db)) out.push_back("   " + l);
    }
    return out;
}

// ---- building ----------------------------------------------------------------------------------------------------------------------

std::string options_of(project::Project& project) {
    const std::filesystem::path lf = project.path("LINKOPT");
    std::vector<std::string> parts;
    if (lk_exists(lf))
        for (const std::string& l : lk_lines_of(lf)) {
            const std::string code = lk_code_of(l);
            if (!code.empty()) parts.push_back(code);
        }
    return text::join(parts, " ");
}

Built build(project::Project& project, const linkwhole::DosHost& dos, const std::optional<std::filesystem::path>& keep, bool verbose, bool quiet,
            const std::optional<std::string>& options) {
    Built out;
    Objects own = objects(project, verbose);
    out.notes = std::move(own.notes);
    std::vector<NamedObject> all = first_objects(project);
    all.insert(all.end(), std::make_move_iterator(own.objects.begin()), std::make_move_iterator(own.objects.end()));
    const BssObjects bss = bss_objects(project);
    std::vector<NamedObject> after;
    for (const std::string& f : project.config().after_library) {   // (whole files linked after the library: a module of the communal variables at their places, as c_common is last)
        const modules::SetOf set = modules::set_of(*project.as_modules(), f);
        const modules::CompileResult c = set.toolchain->compile(fs::read_file(project.src() / f), lk_compile_options(set));
        if (!c.object) throw LinkError(std::format("{} does not compile:\n{}", f, c.console.size() > 600 ? c.console.substr(c.console.size() - 600) : c.console));
        after.push_back({lk_stem(f), *c.object});
    }
    std::vector<NamedObject> known = all;
    known.insert(known.end(), bss.objects.begin(), bss.objects.end());
    known.insert(known.end(), after.begin(), after.end());
    CrtResult lib = crt(project, known);
    std::set<std::string> comm;   // (communal names are the linker's to place, and _edata, _end its own: not left open)
    for (const std::vector<NamedObject>* group : {&known, &lib.modules})
        for (const NamedObject& o : *group)
            for (const auto& [n, c] : formats::omf::read(o.object).communals) comm.insert(n);
    const project::LinkSpec* link_spec = project.profile_parts()->link ? &*project.profile_parts()->link : nullptr;
    const std::vector<std::string> own_names = link_spec ? link_spec->names : std::vector<std::string>{"_edata", "_end"};
    std::vector<std::string> miss;
    for (const std::string& m : lib.missing)
        if (!comm.contains(m) && std::find(own_names.begin(), own_names.end(), m) == own_names.end()) miss.push_back(m);
    if (!quiet) {
        if (!miss.empty()) out.notes.push_back("   not resolved: " + text::join(miss, " "));
        for (const std::string& c : lib.conflicts) out.notes.push_back("   " + c);
    }
    std::vector<NamedObject> link_order = all;
    if (bss.after) {
        link_order.insert(link_order.end(), lib.modules.begin(), lib.modules.end());
        link_order.insert(link_order.end(), bss.objects.begin(), bss.objects.end());
    } else {
        if (!bss.objects.empty()) link_order.push_back(bss.objects.front());
        link_order.insert(link_order.end(), lib.modules.begin(), lib.modules.end());
        if (bss.objects.size() > 1) link_order.insert(link_order.end(), bss.objects.begin() + 1, bss.objects.end());
    }
    link_order.insert(link_order.end(), after.begin(), after.end());
    linkwhole::LinkOutput linked = run_link(project, link_order, dos, keep, options ? *options : options_of(project));
    out.exe = std::move(linked.exe);
    out.console = std::move(linked.console);
    out.map = std::move(linked.map);
    out.stand_ins = std::move(own.stand_ins);
    return out;
}

std::vector<std::string> fit(project::Project& project, const linkwhole::DosHost& dos, int rounds) {
    std::vector<std::string> out;
    const x86::Decoder16* decoder = project.services().decoder;
    const auto locate = [&] { return library::locate(*project.as_library(), decoder ? *decoder : x86::capstone_decoder()); };
    const auto write_crtlib = [&] { fs::write_text(project.path("CRTLIB"), text::join(library::lines(locate()), "\n") + "\n"); };   // (the library stage's, with the sizes so far)
    for (int r = 0; r < rounds; ++r) {
        write_crtlib();
        const Built b = build(project, dos, std::nullopt, false, true);
        if (!b.exe) {
            out.push_back(std::format("round {}: no program was written", r));
            break;
        }
        const std::int64_t dg = project.dgroup();
        const Bytes linked_image = parts(*b.exe).image;
        const ByteView theirs_data = lk_slice(project.image(), dg, static_cast<std::int64_t>(project.image().size()));
        const ByteView ours_data = lk_slice(linked_image, dg, static_cast<std::int64_t>(linked_image.size()));
        std::optional<Shift> first;
        for (const Shift& s : shifts(theirs_data, ours_data))
            if (s.by != 0) {
                first = s;
                break;
            }
        if (!first) {
            out.push_back(std::format("round {}: the data segment agrees", r));
            break;
        }
        const std::int64_t p = static_cast<std::int64_t>(first->at), d = first->by;
        const library::Map lib = locate();
        const std::map<std::string, std::uint32_t>& sizes = project.config().data_sizes;
        const library::DataRaw* chosen = nullptr;   // the stand-in of a guessed length just before the place, the latest
        for (const library::DataRaw& x : lib.data_raws) {
            const bool sized = x.why.find("a guess") != std::string::npos || std::any_of(x.pubs.begin(), x.pubs.end(), [&](const auto& kv) { return sizes.contains(kv.first); });
            if (x.end <= p && p <= x.end + 32 && sized && (chosen == nullptr || x.end > chosen->end)) chosen = &x;
        }
        if (chosen == nullptr) {
            const ByteView ds = lk_slice(project.image(), dg, static_cast<std::int64_t>(project.image().size()));
            std::int64_t q = p;
            while (q > 0 && static_cast<std::size_t>(q) <= ds.size() && ds[static_cast<std::size_t>(q - 1)] == 0) --q;
            if (d < 0 && p - q >= -d) {   // (zeros no module accounts for: a pad, where they begin)
                const std::int64_t at = q + (q & 1);
                project.set_entry("data_pads", std::format("{:04x}", at), -d);
                out.push_back(std::format("round {}: at DS:{:04x} ours is {:+}, after {} zeros: a pad of {} bytes at DS:{:04x}", r, p, d, p - q, -d, at));
                continue;
            }
            out.push_back(std::format("round {}: at DS:{:04x} ours is {:+}, and no stand-in of a guessed length is before it", r, p, d));
            break;
        }
        const std::string name = chosen->pubs.begin()->first;
        const std::int64_t size = chosen->end - chosen->data - d;
        project.set_entry("data_sizes", name, size);
        out.push_back(std::format("round {}: at DS:{:04x} ours is {:+}: {}'s stand-in made {} bytes", r, p, d, name, size));
        write_crtlib();
    }
    return out;
}

// ---- the command ---------------------------------------------------------------------------------------------------------------------

int run(project::Project& project, const std::vector<std::string>& args, const linkwhole::DosHost& dos) {
    if (linkwhole::wanted(project.config().link)) return linkwhole::run(*project.as_linkwhole(), args, dos);   // (a program of whole source files)
    const auto flag = [&](std::string_view f) { return std::find(args.begin(), args.end(), f) != args.end(); };
    std::optional<std::filesystem::path> keep;
    if (const auto k = std::find(args.begin(), args.end(), "--keep"); k != args.end()) {
        if (k + 1 == args.end()) throw UsageError("--keep needs a folder");
        keep = fs::from_text(*(k + 1));
    }
    if (flag("--fit"))
        for (const std::string& l : fit(project, dos)) out("   {}\n", l);
    const Built b = build(project, dos, keep, flag("-v"));
    for (const std::string& n : b.notes) out("{}\n", n);
    std::size_t shown = 0;
    for (std::string_view l : text::split(b.console, '\n')) {
        if (text::trim(l).empty() || l.find("Microsoft") != std::string_view::npos || l.find("Copyright") != std::string_view::npos) continue;
        if (shown++ < 20) out("   LINK: {}\n", l);
    }
    if (flag("--map")) out("{}\n", b.map);
    if (!b.exe) {
        out("no program was written\n");
        return 1;
    }
    const std::optional<std::string> packed_path = project.config().packed;
    Bytes packed_file;
    if (packed_path) packed_file = fs::read_file((project.dir() / fs::from_text(*packed_path)).lexically_normal());
    const ByteView ref = packed_path ? ByteView(packed_file) : project.file();
    const bool same = std::equal(b.exe->begin(), b.exe->end(), ref.begin(), ref.end());
    std::vector<std::string> diag;
    if (!same && project.packed()) {   // (where ours parts from it: linked again, not packed, and held to the unpacked image)
        static const Regex exepack_rx(R"(\s*/E(?:XEPACK|XE|X)?\b)", RegexFlags::IgnoreCase);
        const std::string unpacked_options = exepack_rx.replace_all(options_of(project), std::string_view(""));
        const Built again = build(project, dos, keep, false, true, unpacked_options);
        if (again.exe) diag = diagnose(project, std::nullopt, *again.exe);
        const bool says_differ = std::any_of(diag.begin(), diag.end(), [](const std::string& l) { return l.find("differ") != std::string::npos || l.find("stack") != std::string::npos || l.find("image is") != std::string::npos; });
        if (again.exe && !again.exe->empty() && !says_differ) diag.push_back("unpacked, ours is the program; the packed files differ: EXEPACK's work (its table's order: the order of the fixups)");
    } else if (!same) {
        diag = diagnose(project, ref, *b.exe);
    }
    for (const std::string& l : diag) out("   {}\n", l);
    const bool ok = same || !std::any_of(diag.begin(), diag.end(), [](const std::string& l) {
        return l.find("image") != std::string::npos || l.find("header") != std::string::npos || l.find("relocations differ") != std::string::npos;
    });
    out("{} linked from the sources: {} bytes; {}\n", fs::to_text(project.exe().filename()), b.exe->size(),
        same ? "the FILE IDENTICAL" : (ok ? "the image, header and relocations identical" : "not identical"));
    out("   {} functions are stand-ins of the program's bytes{}\n", b.stand_ins.size(), b.stand_ins.empty() ? std::string() : ": " + text::join(b.stand_ins, " "));
    return ok ? 0 : 1;
}

}  // namespace cb::exact::link
