// app/convert_backends.cpp -- the routes of the conversion pipeline (convert.hpp): which stages each kind of file passes through, and the
// writers and the decompiler each calls.
//
//   Windows 32-bit (PE, x86), Windows 16-bit (NE), drivers (LE/LX), DOS     Opening, Disassembling (the NASM / MASM-style source that builds back
//                                                                           into the original: source/pe_writer ...), Decompiling (C)
//   a console cartridge, disc executable or snapshot of a registered lane   Opening, Disassembling (its machine's assembly source)
//   a disc image, floppy or hard-disk image, cabinet                        Unpacking (its programs taken out), Disassembling (each program through
//                                                                           the route of its own kind)
//
//   a C# or Python source file, with a language asked for                   Opening, Translating (through the hub), Checking (both run, outputs compared)
//   a Java game's folder: a RetroMCP workspace (or any folder inside it),   Opening (the source root, jar, libraries and entry found: java/port.hpp),
//   a decompiled game folder with its source, a Java source tree            Translating (the whole source to C++03 for PC, with Content: `java port`),
//                                                                           Writing, then Building (CMake, in the output folder: the program)
//
// Anything else is Unsupported, with what identification says it is.
#include <algorithm>
#include <cctype>
#include <format>
#include <set>
#include <stdexcept>

#include "app/convert.hpp"
#include "base/config.hpp"
#include "build/watcom.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/text.hpp"
#include "decompile/decompiler.hpp"
#include "engine/lane_kit.hpp"
#include "engine/lane_kit_internal.hpp"
#include "engine/lane_registry.hpp"
#include "engine/machine_lanes.hpp"
#include "formats/cab.hpp"
#include "formats/detect.hpp"
#include "formats/media.hpp"
#include "formats/pe.hpp"
#include "java/port.hpp"
#include "java/tools.hpp"
#include "source/dos_writer.hpp"
#include "source/le_writer.hpp"
#include "source/ne_writer.hpp"
#include "source/pe_writer.hpp"
#include "translate/translate_file.hpp"
#include "verify/lanes/image_lane.hpp"

namespace cb::app::convert {

namespace stdfs = std::filesystem;

namespace {

constexpr std::uint16_t kCvMachineX86 = 0x014C;

// ---- the tools every route finds the same way the commands do ------------------------------------------------------------------

struct CvTools {
    config::Config config;

    explicit CvTools(const config::Config& c) : config(c) {}
    build::watcom::OpenWatcom open_watcom() const { return build::watcom::OpenWatcom::from_settings(config.text("openwatcom")); }
    source::dos_writer::Options dos_options() const {
        return source::dos_writer::options_for(open_watcom(), config.state() / "assembly_probes.json");
    }
    source::FindStub stub_finder() const { return source::dos_writer::stub_finder(dos_options()); }
};

std::string cv_lower(std::string_view s) { return text::lower(s); }

// Writer output goes into a folder of its own; the source (not the program the writer rebuilt to check itself) is kept in the work folder.
stdfs::path cv_scratch(const stdfs::path& work) {
    stdfs::path scratch = work / ".build";
    stdfs::create_directories(scratch);
    return scratch;
}

// The writers of whole Windows files assemble with a line `[map symbols <path>]` at the top, naming the map file in their scratch folder; in the kept
// source it names the map by its file name alone, as the assembler would write it beside the source (the scratch folder goes away). A file it
// pulls in from the scratch folder (`incbin` of a DOS stub) is kept beside it and named the same way, so the kept source builds on its own.
void cv_keep(const stdfs::path& source, const stdfs::path& work) {
    const Bytes raw = fs::read_file(source);   // (bytes as the writer wrote them: its source text is Latin-1)
    std::string text(reinterpret_cast<const char*>(raw.data()), raw.size());
    const std::string marker = "[map symbols ";
    const std::size_t at = text.find(marker);
    if (at != std::string::npos && at < 8) {
        const std::size_t end = text.find(']', at);
        if (end != std::string::npos) {
            const std::string name = fs::to_text(fs::from_text(text.substr(at + marker.size(), end - at - marker.size())).filename());
            text = text.substr(0, at + marker.size()) + name + text.substr(end);
        }
    }
    const std::string pulled = "incbin \"" + fs::to_text(source.parent_path()) + "/";
    for (std::size_t in = text.find(pulled); in != std::string::npos; in = text.find(pulled, in + 1)) {
        const std::size_t from = in + 8, close = text.find('"', from);
        if (close == std::string::npos) break;
        const stdfs::path file = fs::from_text(text.substr(from, close - from));
        stdfs::copy_file(file, work / file.filename(), stdfs::copy_options::overwrite_existing);
        text.replace(from, close - from, fs::to_text(file.filename()));
    }
    fs::write_file(work / source.filename(), ByteView(reinterpret_cast<const std::uint8_t*>(text.data()), text.size()));
}

// What a DOS program's source needs to build alone (`build --dos`): its .build.json, header, link script, overlay and its unpacker's folder. Not what a build makes again.
void cv_keep_dos_side_files(const stdfs::path& scratch, const stdfs::path& work) {
    std::error_code ec;
    for (stdfs::recursive_directory_iterator it(scratch, ec), end; !ec && it != end; it.increment(ec)) {
        const stdfs::path relative = it->path().lexically_relative(scratch);
        if (it->is_directory(ec)) {
            if (relative == "file") it.disable_recursion_pending();
            continue;
        }
        const std::string extension = cv_lower(fs::to_text(relative.extension()));
        if (extension == ".obj" || extension == ".map" || extension == ".err" || cv_lower(fs::to_text(relative.stem())) == "probe") continue;
        if (extension == ".asm" && relative.parent_path().empty()) continue;   // (the sources are kept by cv_keep)
        if ((extension == ".exe" || extension == ".com" || extension == ".bin") && stdfs::exists(stdfs::path(it->path()).replace_extension(".asm"), ec)) continue;
        stdfs::create_directories((work / relative).parent_path());
        stdfs::copy_file(it->path(), work / relative, stdfs::copy_options::overwrite_existing);
    }
}

// The program the writer assembled from its source to check itself, kept as the conversion's built program: <work>/build/<the input's own
// name>, with BUILD.txt saying how it was made, whether it is the original byte for byte, and what it still needs to run. Nothing when the
// assembler built nothing.
void cv_keep_built(const std::optional<stdfs::path>& image, bool same, const stdfs::path& input, const stdfs::path& work) {
    if (!image || !stdfs::is_regular_file(*image)) return;
    const stdfs::path folder = work / "build";
    stdfs::create_directories(folder);
    const std::string name = fs::to_text(input.filename());
    const std::string asm_name = cv_lower(name) + ".asm";
    stdfs::copy_file(*image, folder / input.filename(), stdfs::copy_options::overwrite_existing);
    std::string note = std::format("{} in this folder was assembled from {}, the program's reconstructed assembly source beside this folder,\n"
                                   "by CodeBreaker's assembler at the end of the conversion.\n\n",
                                   name, asm_name);
    note += same ? "It is the original program byte for byte: it runs exactly as the original does.\n"
                 : "It is NOT the original byte for byte: the assembler built it, but some bytes differ, so it may not run as the original does.\n";
    note += std::format("\nTo build it again, in the folder above this one:\n\n    codebreaker nasm -f bin -o build\\{} {}\n\n", name, asm_name);
    note += "To run it, it needs what the original needs beside it: the DLLs it loads and any plugin folders (a Qt program's platforms\\, for\n"
            "one), copied from where the original is installed.\n\n"
            "The .c files are a readable reconstruction of the same program. They are not what this program was built from, and they do not\n"
            "build it yet.\n";
    fs::write_text(folder / "BUILD.txt", note);
}

// ---- Windows and DOS programs ------------------------------------------------------------------------------------------------

enum class CvMz { Pe, Ne, Le, Dos };

// The words under the progress bar while the decompiler works: which job, how far, and once it has taken a while, for how long.
std::string cv_decompile_words(std::string_view phase, std::size_t done, std::size_t total, std::chrono::steady_clock::duration taken) {
    std::string words = phase == "large" ? std::format("Large functions, retried: {} of {}", done, total) : std::format("{} of {} functions", done, total);
    const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(taken).count();
    if (seconds >= 20 && done < total) words += seconds < 60 ? std::format(" \xC2\xB7 {} s", seconds) : std::format(" \xC2\xB7 {} min {} s", seconds / 60, seconds % 60);
    return words;
}

// The functions decompiled, as the decompiler counts them, reach the reporter.
void cv_decompile(Reporter& r, const config::Config& config, const stdfs::path& input, const stdfs::path& work) {
    engine::LaneKit kit(config);
    decompile::decompiler::Decompiler& decompiler = kit.decompiler();
    std::string phase;
    auto since = std::chrono::steady_clock::now();
    std::size_t last_total = 0;
    decompiler.set_phase([&](std::string_view p) {
        phase = std::string(p);
        since = std::chrono::steady_clock::now();
    });
    decompiler.set_progress([&](std::size_t done, std::size_t total) {
        if (total != last_total) {   // (another run of the decompiler: its own clock)
            last_total = total;
            since = std::chrono::steady_clock::now();
        }
        r.count(done, total, cv_decompile_words(phase, done, total, std::chrono::steady_clock::now() - since));
    });
    const decompile::decompiler::AsC c = decompiler.as_c(input);
    decompiler.set_progress(nullptr);
    decompiler.set_phase(nullptr);
    r.check();
    if (c.files.empty()) fs::write_text(work / (cv_lower(fs::to_text(input.stem())) + ".c"), c.text);
    for (const auto& [name, text] : c.files) fs::write_text(work / name, text);   // (a program as the source files it was made of)
    r.count(c.decompiled, std::max(c.found, c.decompiled), std::format("{} of {} functions", c.decompiled, std::max(c.found, c.decompiled)));
}

Route cv_mz_route(CvMz kind, std::string route_name) {
    Route route;
    route.name = std::move(route_name);
    route.stages = {State::Opening, State::Disassembling, State::Decompiling};
    route.run = [kind](Reporter& r, const stdfs::path& input, const stdfs::path& work) {
        const config::Config& config = config::current();
        const CvTools tools(config);
        r.enter(State::Opening);
        if (kind == CvMz::Pe) {
            const formats::pe::Image image = formats::pe::Image::open(input);
            if (image.header().machine != kCvMachineX86)
                throw NotConvertible("This is a Windows program for a processor other than the 32-bit x86. CodeBreaker converts the 32-bit x86 kind.");
        }
        r.count(1, 1, fs::to_text(input.filename()));

        r.enter(State::Disassembling);
        const stdfs::path scratch = cv_scratch(work);
        const auto say = [&r](std::string_view) { r.check(); };
        switch (kind) {
            case CvMz::Pe: {
                source::pe_writer::Options o;
                o.find_stub = tools.stub_finder();
                o.say = say;
                const source::pe_writer::Result done = source::pe_writer::reconstruct(input, scratch, o);
                cv_keep(done.source, work);
                cv_keep_built(done.image, done.same, input, work);
                break;
            }
            case CvMz::Ne: {
                source::ne_writer::Options o;
                o.find_stub = tools.stub_finder();
                o.say = say;
                const source::ne_writer::Result done = source::ne_writer::reconstruct(input, scratch, o);
                cv_keep(done.source, work);
                cv_keep_built(done.image, done.same, input, work);
                break;
            }
            case CvMz::Le: {
                source::le_writer::Options o;
                o.say = say;
                const source::le_writer::Result done = source::le_writer::reconstruct(input, scratch, o);
                cv_keep(done.source, work);
                cv_keep_built(done.image, done.same, input, work);
                break;
            }
            case CvMz::Dos: {
                const source::dos_writer::Result done = source::dos_writer::reconstruct(input, scratch, tools.dos_options());
                if (done.sources.empty()) throw NotConvertible("No source could be written for this DOS program.");
                for (const stdfs::path& s : done.sources) cv_keep(s, work);
                cv_keep_dos_side_files(scratch, work);
                break;
            }
        }
        r.count(1, 1);
        std::error_code ec;
        stdfs::remove_all(scratch, ec);

        r.enter(State::Decompiling);
        cv_decompile(r, config, input, work);
    };
    return route;
}

// ---- consoles --------------------------------------------------------------------------------------------------------------------

std::string cv_machine_of(const stdfs::path& input) {
    std::string name = engine::kit_detail::machine_of_file(input);
    if (name.empty()) name = engine::machine_lanes().name_of_file(input);
    return name;
}

Route cv_machine_route(std::string machine) {
    Route route;
    route.name = "machine:" + machine;
    route.stages = {State::Opening, State::Disassembling};
    route.run = [machine](Reporter& r, const stdfs::path& input, const stdfs::path& work) {
        engine::register_all_lanes();
        r.enter(State::Opening);
        engine::LaneKit kit(config::current());
        const std::shared_ptr<const verify::lanes::Kind> lane = kit.machine_lane(machine);
        if (!lane) throw NotConvertible("The tools for this machine are not set up on this computer.");
        r.count(1, 1, fs::to_text(input.filename()));

        r.enter(State::Disassembling);
        const stdfs::path scratch = cv_scratch(work);
        const std::unique_ptr<verify::lanes::Prepared> prepared = lane->prepare(input, scratch);
        r.check();
        const auto* image = dynamic_cast<const verify::lanes::ImageLane::PreparedImage*>(prepared.get());
        const stdfs::path source_file = image != nullptr ? image->written.source : stdfs::path();
        if (source_file.empty()) throw NotConvertible("No source could be written for this program.");
        // everything the writer wrote beside its source, except the program it built again to check itself
        std::error_code ec;
        for (const stdfs::directory_entry& e : stdfs::directory_iterator(source_file.parent_path(), ec)) {
            if (!e.is_regular_file(ec)) continue;
            if (image != nullptr && e.path() == image->written.built) continue;
            cv_keep(e.path(), work);
        }
        const std::size_t functions = image->written.functions.size();
        r.count(functions, functions, std::format("{} functions", functions));
        stdfs::remove_all(scratch, ec);
    };
    return route;
}

// ---- containers ------------------------------------------------------------------------------------------------------------------

bool cv_program_name(const std::string& path, const std::set<std::string>& lane_extensions) {
    static const std::set<std::string> kExecutable = {".exe", ".dll", ".com", ".sys", ".vxd", ".drv", ".cpl", ".ocx", ".scr", ".386", ".xex", ".xbe"};
    const std::string ext = cv_lower(fs::to_text(stdfs::path(fs::from_text(path)).extension()));
    return kExecutable.contains(ext) || lane_extensions.contains(ext);
}

constexpr std::uint64_t kCvLargestProgram = 256ull << 20;
constexpr std::size_t kCvMostPrograms = 200;

// What a route passes to a program inside a container: stage changes are not shown (the container's own stages are), counts are dropped.
class cv_InnerReporter final : public Reporter {
public:
    explicit cv_InnerReporter(Reporter& outer) : outer_(outer) {}
    void enter(State) override { outer_.check(); }
    void count(std::uint64_t, std::uint64_t, std::string_view) override { outer_.check(); }
    void check() override { outer_.check(); }

private:
    Reporter& outer_;
};

Plan cv_plan(const stdfs::path& input);

// The programs of a container written under `unpacked`; their source goes to `work/<program>/`.
void cv_container_run(Reporter& r, const stdfs::path& input, const stdfs::path& work) {
    const stdfs::path unpacked = work / ".unpacked";
    stdfs::create_directories(unpacked);
    std::set<std::string> lane_extensions;
    for (const std::string& e : engine::lane_program_extensions()) lane_extensions.insert(e);

    r.enter(State::Unpacking);
    std::vector<stdfs::path> programs;
    const std::string head = [&] {
        const Bytes b = fs::read_file(input);
        return std::string(reinterpret_cast<const char*>(b.data()), std::min<std::size_t>(b.size(), 4));
    }();
    if (head == "MSCF") {
        r.count(0, 1, "cabinet");
        const auto files = formats::cab::extract(input, unpacked);
        std::uint64_t n = 0;
        for (const auto& f : files) {
            r.count(++n, files.size(), f.name);
            if (cv_program_name(f.name, lane_extensions) && programs.size() < kCvMostPrograms) programs.push_back(f.path);
        }
    } else {
        for (const std::unique_ptr<formats::media::Volume>& volume : formats::media::volumes(input)) {
            std::vector<const formats::media::Entry*> wanted;
            for (const formats::media::Entry& e : volume->files())
                if (!e.is_dir && e.size > 0 && e.size <= kCvLargestProgram && cv_program_name(e.path, lane_extensions)) wanted.push_back(&e);
            std::sort(wanted.begin(), wanted.end(), [](auto* a, auto* b) { return a->size < b->size; });   // the small ones first: the most programs
            std::uint64_t n = 0;
            for (const formats::media::Entry* e : wanted) {
                if (programs.size() >= kCvMostPrograms) break;
                r.count(n++, wanted.size(), e->path);
                const stdfs::path target = unpacked / std::format("{:03}", programs.size()) / fs::from_text(e->path).filename();
                fs::make_dirs(target.parent_path());
                fs::write_file(target, volume->read(*e));
                programs.push_back(target);
            }
            r.count(wanted.size(), wanted.size());
        }
    }
    if (programs.empty()) throw NotConvertible("There are no programs inside this image that CodeBreaker can read.");

    r.enter(State::Disassembling);
    std::uint64_t done = 0, converted = 0;
    std::string left_out;
    cv_InnerReporter inner(r);
    for (const stdfs::path& program : programs) {
        r.count(done, programs.size(), fs::to_text(program.filename()));
        Plan plan = cv_plan(program);
        if (plan.route && plan.route->name != "container") {
            const stdfs::path target = unique_output_folder(work, fs::to_text(program.stem()));
            stdfs::create_directories(target);
            try {
                plan.route->run(inner, program, target);
                ++converted;
            } catch (const NotConvertible& e) {
                left_out += fs::to_text(program.filename()) + ": " + e.what() + "\n";
                std::error_code ec;
                stdfs::remove_all(target, ec);
            } catch (const Cancelled&) {
                throw;
            } catch (const std::exception& e) {
                left_out += fs::to_text(program.filename()) + ": " + e.what() + "\n";
                std::error_code ec;
                stdfs::remove_all(target, ec);
            }
        } else {
            left_out += fs::to_text(program.filename()) + ": CodeBreaker cannot convert this kind of file.\n";
        }
        r.count(++done, programs.size(), fs::to_text(program.filename()));
    }
    std::error_code ec;
    stdfs::remove_all(unpacked, ec);
    if (converted == 0) throw NotConvertible("None of the programs inside this image could be converted.\n" + left_out);
    if (!left_out.empty()) fs::write_text(work / "not_converted.txt", left_out);
}

Route cv_container_route() {
    Route route;
    route.name = "container";
    route.stages = {State::Unpacking, State::Disassembling};
    route.weights = {{State::Unpacking, 20}, {State::Disassembling, 70}};
    route.run = cv_container_run;
    return route;
}

// ---- which route a file takes ----------------------------------------------------------------------------------------------------

Plan cv_plan(const stdfs::path& input) {
    engine::register_all_lanes();
    Plan plan;
    std::vector<formats::detect::Detected> identified;
    try {
        identified = formats::detect::identify(input);
    } catch (const std::exception&) {   // unreadable: an ordinary "unknown"
    }
    if (!identified.empty()) plan.what = identified.front().format;

    std::optional<formats::detect::Lane> lane;
    try {
        lane = formats::detect::kind(input);
    } catch (const std::exception&) {
    }
    if (lane) {
        if (plan.what.empty()) plan.what = std::string("Windows or DOS program (") + formats::detect::lane_name(*lane) + ")";
        switch (*lane) {
            case formats::detect::Lane::Pe: plan.route = cv_mz_route(CvMz::Pe, "pe"); break;
            case formats::detect::Lane::Ne: plan.route = cv_mz_route(CvMz::Ne, "ne"); break;
            case formats::detect::Lane::Le: plan.route = cv_mz_route(CvMz::Le, "le"); break;
            case formats::detect::Lane::Dos: plan.route = cv_mz_route(CvMz::Dos, "dos"); break;
        }
        return plan;
    }
    std::string machine;
    try {
        machine = cv_machine_of(input);
    } catch (const std::exception&) {
    }
    if (!machine.empty()) {
        if (plan.what.empty()) plan.what = "console program (" + machine + ")";
        plan.route = cv_machine_route(machine);
        return plan;
    }
    // a container: a cabinet, or an image with volumes of files
    try {
        const Bytes b = fs::read_file(input);
        if (b.size() >= 4 && std::string_view(reinterpret_cast<const char*>(b.data()), 4) == "MSCF") {
            plan.route = cv_container_route();
            return plan;
        }
        if (!formats::media::volumes(input).empty()) {
            if (plan.what.empty()) plan.what = "disc or disk image";
            plan.route = cv_container_route();
            return plan;
        }
    } catch (const std::exception&) {
    }
    return plan;
}

// ---- translating source code through the hub ---------------------------------------------------------------------------------------------

namespace cap = cb::translate::capability;
namespace tf = cb::translate::translate_file;

// The sentence for a translation that did not come out right, by the verdict. The translation is never kept unless both programs were run and agreed.
[[noreturn]] void cv_translation_failed(const tf::Result& result, const std::string& from, const std::string& to) {
    switch (result.verdict) {
        case tf::Verdict::Refused:
            throw NotConvertible("CodeBreaker cannot translate this " + from + " program to " + to + " yet: " + result.detail);
        case tf::Verdict::BadInput:
            throw std::runtime_error("The " + from + " program itself does not build or run, so there is nothing to compare a translation with: " + result.detail);
        case tf::Verdict::CompileFailed:
            throw std::runtime_error("The " + to + " written for this program does not build, so it was not kept: " + result.detail);
        default:
            throw std::runtime_error("The " + to + " translation does not behave like the " + from + " program, so it was not kept. " + result.detail);
    }
}

// Reads the source file, translates it along `path` and proves the result; the files go into `work`. The stages are Opening, Translating, Checking.
Route cv_translate_route(const cap::Path& path, std::string from_name, std::string to_name) {
    Route route;
    route.name = "translate:" + path.from + ":" + path.to;
    route.stages = {State::Opening, State::Translating, State::Checking};
    route.weights = {{State::Opening, 2}, {State::Translating, 30}, {State::Checking, 55}};
    route.run = [path, from_name = std::move(from_name), to_name = std::move(to_name)](Reporter& r, const stdfs::path& input, const stdfs::path& work) {
        r.enter(State::Opening);
        if (fs::read_file(input).empty()) throw std::runtime_error("The file is empty.");   // (it must be readable before anything is started)
        r.count(1, 1, fs::to_text(input.filename()));

        r.enter(State::Translating);
        translate::judge_cs_cpp::Environment environment{config::current(), {}};
        bool checking = false;
        environment.observer.translating = [&r](std::uint64_t done, std::uint64_t total, std::string_view what) {
            r.count(done, total, total > 0 ? std::format("{} of {} functions", done, total) : std::string(what));
        };
        environment.observer.checking = [&r, &checking](std::uint64_t done, std::uint64_t total, std::string_view what) {
            if (!checking) {
                checking = true;
                r.enter(State::Checking);
            }
            r.count(done, total, what.empty() ? std::format("{} of {} steps", done, total) : std::string(what));
        };
        const stdfs::path scratch = work / ".translate";
        const tf::Result result = tf::run(path, input, work, scratch, environment);
        std::error_code ec;
        stdfs::remove_all(scratch, ec);
        if (result.verdict != tf::Verdict::Agrees) cv_translation_failed(result, from_name, to_name);
        r.note(result.checked);
    };
    return route;
}

// The plan for a source file the translator reads, when `target` is a translation (not "original").
Plan cv_plan_source(const cap::Language& language, const std::string& target) {
    Plan plan;
    plan.what = language.name + " source code";
    const std::vector<cap::Target> list = cap::targets_for_source(language.id);
    const cap::Target* t = cap::find_target(list, target);
    if (t == nullptr) {
        plan.refusal = "CodeBreaker does not know a language called \"" + target + "\".";
    } else if (!t->supported) {
        plan.refusal = t->reason + ".";
        if (t->id == "original") plan.refusal = "This is " + plan.what + " already, so there is nothing to turn back into source.";
        const std::string choices = cap::supported_names(list);
        plan.refusal += choices.empty() ? " CodeBreaker cannot translate " + plan.what + " yet."
                                        : " Choose one of the languages CodeBreaker can write from " + plan.what + ": " + choices + ".";
    } else {
        const cap::Path* path = cap::find_path(language.id, t->id);
        plan.route = cv_translate_route(*path, language.name, t->name);
        plan.folder_suffix = "_" + t->id;
    }
    return plan;
}

// ---- a Java game's folder ----------------------------------------------------------------------------------------------------------------

// A folder with a Java game's source (java/port.hpp finds it: a RetroMCP workspace from any folder inside it, a decompiled game folder, a source
// tree) is converted whole: `java port` for PC, without the build, into the work folder. A folder with no Java source has no plan here.
std::optional<Plan> cv_plan_java_folder(const stdfs::path& input, const std::string& target) {
    std::error_code ec;
    if (!stdfs::is_directory(input, ec)) return std::nullopt;
    const java::port::Plan p = java::port::plan(input, java::port::Platform::Pc);
    if (!p.problem.empty() || !p.java_src) return std::nullopt;   // (no source in it: a game folder that would need the network to decompile)
    Plan plan;
    switch (p.kind) {
        case java::port::SourceKind::RetroMcpWorkspace:
            plan.what = std::format("RetroMCP workspace (Minecraft {}, {} Java files)", p.mc_version.empty() ? "?" : p.mc_version, p.java_files);
            break;
        case java::port::SourceKind::GameFolder: plan.what = "decompiled game folder (Java source)"; break;
        default: plan.what = "folder of Java source code"; break;
    }
    const std::string t = cv_lower(target);
    if (!(t.empty() || t == "original" || t == "cpp03" || t == "c++03" || t == "c++")) {
        plan.refusal = "A Java game's folder is converted to C++03 (choose Original or C++03); other languages are not available for a whole game yet.";
        return plan;
    }
    plan.output_name = p.name;
    Route route;
    route.name = "java-port";
    route.stages = {State::Opening, State::Translating};
    route.weights = {{State::Opening, 2}, {State::Translating, 20}, {State::Building, 70}};
    route.run = [p](Reporter& r, const stdfs::path& input, const stdfs::path& work) {
        r.enter(State::Opening);
        r.count(1, 1, p.entry_class.empty() ? fs::to_text(p.java_src->filename()) : "entry " + p.entry_class);
        r.enter(State::Translating);
        r.count(0, 1, std::format("{} Java files", p.java_files));
        // the port writes <work>/<name>/... (the C++, its CMakeLists.txt, Content); the build is left to the person
        const std::vector<std::string> args = {"port", fs::to_text(input), "--platform", "pc", "--out", fs::to_text(work), "--no-build"};
        const int code = java::tools::main(args, java::tools::Environment{config::current()});
        r.check();
        if (code != 0) throw std::runtime_error(std::format("The Java to C++ conversion stopped (exit {}); run `codebreaker java port` on the folder for the details.", code));
        r.count(1, 1, std::format("{} Java files", p.java_files));
        r.note(p.kind == java::port::SourceKind::RetroMcpWorkspace
                   ? std::format("Converted the whole RetroMCP workspace (Minecraft {}): {} Java files from {}, against {} libraries.", p.mc_version, p.java_files,
                                 fs::to_text(*p.java_src), p.libraries.size())
                   : std::string());
    };
    // Building: the C++ is compiled where it now lies (<out>/<name>, with CMake: java::tools::build_pc_project), so its build folder stays usable
    route.build = [p](Reporter& r, const stdfs::path& out) {
        const stdfs::path project = out / p.name;
        r.count(0, 1, "CMake: " + p.name);
        // the build's own "[ 42%] Building CXX object ..." lines are the stage's count (no count would leave the bar where Building began)
        const java::tools::PcBuild b = java::tools::build_pc_project(project, true, [&r](std::string_view line) {
            if (const int pct = java::tools::build_percent_of_line(line); pct >= 0) {
                const std::size_t close = line.find(']');
                r.count(static_cast<std::uint64_t>(pct), 100, text::trim(line.substr(close + 1)));
            }
        });
        r.check();
        const std::string converted = p.kind == java::port::SourceKind::RetroMcpWorkspace
                                          ? std::format("Converted the whole RetroMCP workspace (Minecraft {}): {} Java files. ", p.mc_version, p.java_files)
                                          : std::format("Converted {} Java files. ", p.java_files);
        switch (b.status) {
            case java::tools::PcBuild::Status::NoCMake:
                throw std::runtime_error("CMake is not installed, so the converted C++ was not built (its CMakeLists.txt is written).");
            case java::tools::PcBuild::Status::Failed:
                throw std::runtime_error(std::format("The converted C++ did not build (exit {}; the log is {}):\n{}", b.exit_code, fs::to_text(b.log), b.tail));
            case java::tools::PcBuild::Status::Built:
                r.count(1, 1, b.exe.empty() ? std::string("built") : fs::to_text(b.exe.filename()));
                r.note(converted + "Built " + (b.exe.empty() ? fs::to_text(project) : fs::to_text(b.exe)) + ".");
                break;
        }
    };
    plan.route = std::move(route);
    return plan;
}

// The plan with a language asked for.
Plan cv_plan_for(const stdfs::path& input, const std::string& target) {
    const bool original = target.empty() || cv_lower(target) == "original";
    if (std::optional<Plan> java_folder = cv_plan_java_folder(input, target)) return std::move(*java_folder);
    if (const cap::Language* language = cap::language_of_file(input)) return cv_plan_source(*language, original ? std::string("original") : target);
    Plan plan = cv_plan(input);
    if (original || !plan.route) return plan;   // (Original is today's behaviour; a file with no route is refused as before)
    const std::vector<cap::Target> list = targets_for(input);
    const cap::Target* t = cap::find_target(list, target);
    const std::string reason = t != nullptr && !t->supported ? t->reason : "Translating a decompiled program is not available yet";
    plan.route.reset();
    plan.refusal = reason + ". This program can be written back as its Original source.";
    return plan;
}

}  // namespace

std::vector<cap::Target> targets_for(const stdfs::path& file) {
    if (file.empty()) return cap::targets();   // (no file chosen yet)
    if (cap::language_of_file(file) != nullptr) return cap::targets_for_file(file);
    std::optional<formats::detect::Lane> lane;
    try {
        lane = formats::detect::kind(file);
    } catch (const std::exception&) {
    }
    return cap::targets_for_decompiled(lane ? "c" : "");   // (Windows and DOS programs are decompiled to C; the rest come back as assembly)
}

std::vector<std::string> program_extensions() {
    engine::register_all_lanes();
    std::vector<std::string> out = {".exe", ".dll", ".com", ".sys", ".vxd", ".drv", ".cpl", ".ocx", ".scr", ".386", ".xex", ".xbe", ".cab", ".iso", ".img", ".bin"};
    for (const std::string& e : engine::lane_program_extensions())
        if (std::find(out.begin(), out.end(), e) == out.end()) out.push_back(e);
    for (const std::string& e : cap::source_extensions())
        if (std::find(out.begin(), out.end(), e) == out.end()) out.push_back(e);
    return out;
}

std::string drop_hint() {
    std::string sources;
    for (const std::string& e : cap::source_extensions()) sources += (sources.empty() ? "" : " ") + e;
    return ".exe, .dll, ROMs, disc images, source code (" + sources + ")";
}

Planner standard_planner(const std::string& target) {
    return [target](const stdfs::path& input) { return cv_plan_for(input, target); };
}

std::string platform_summary() {
    engine::register_all_lanes();
    return std::format("Windows \xC2\xB7 DOS \xC2\xB7 Xbox \xC2\xB7 {} consoles \xC2\xB7 Java \xC2\xB7 .NET", engine::registered_lanes().size());
}

}  // namespace cb::app::convert
