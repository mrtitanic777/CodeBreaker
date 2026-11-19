// cb/app/convert_backends.cpp -- the routes of the conversion pipeline (convert.hpp): which stages each kind of file passes through, and the
// writers and the decompiler each calls.
//
//   Windows 32-bit (PE, x86), Windows 16-bit (NE), drivers (LE/LX), DOS     Opening, Disassembling (the NASM / MASM-style source that builds back
//                                                                           into the original: source/pe_writer ...), Decompiling (C)
//   a console cartridge, disc executable or snapshot of a registered lane   Opening, Disassembling (its machine's assembly source)
//   a disc image, floppy or hard-disk image, cabinet                        Unpacking (its programs taken out), Disassembling (each program through
//                                                                           the route of its own kind)
//
// Anything else is Unsupported, with what identification says it is.
#include <algorithm>
#include <cctype>
#include <format>
#include <set>

#include "cb/app/convert.hpp"
#include "cb/base/config.hpp"
#include "cb/build/watcom.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"
#include "cb/decompile/decompiler.hpp"
#include "cb/engine/lane_kit.hpp"
#include "cb/engine/lane_kit_internal.hpp"
#include "cb/engine/lane_registry.hpp"
#include "cb/engine/machine_lanes.hpp"
#include "cb/formats/cab.hpp"
#include "cb/formats/detect.hpp"
#include "cb/formats/media.hpp"
#include "cb/formats/pe.hpp"
#include "cb/source/dos_writer.hpp"
#include "cb/source/le_writer.hpp"
#include "cb/source/ne_writer.hpp"
#include "cb/source/pe_writer.hpp"
#include "cb/verify/lanes/image_lane.hpp"

namespace cb::app::convert {

namespace stdfs = std::filesystem;

namespace {

constexpr std::uint16_t kCvMachineX86 = 0x014C;

// ---- the tools every route finds the same way the commands do ------------------------------------------------------------------

struct CvTools {
    config::Config config;
    stdfs::path nasm;

    explicit CvTools(const config::Config& c) : config(c) {
        if (const auto n = c.path("nasm"); n && stdfs::is_regular_file(*n)) nasm = *n;
    }
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
// source it names the map by its file name alone, as the assembler would write it beside the source (the scratch folder goes away).
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
            fs::write_file(work / source.filename(), ByteView(reinterpret_cast<const std::uint8_t*>(text.data()), text.size()));
            return;
        }
    }
    stdfs::copy_file(source, work / source.filename(), stdfs::copy_options::overwrite_existing);
}

// ---- Windows and DOS programs ------------------------------------------------------------------------------------------------

enum class CvMz { Pe, Ne, Le, Dos };

// The functions decompiled, as the decompiler counts them, reach the reporter.
void cv_decompile(Reporter& r, const config::Config& config, const stdfs::path& input, const stdfs::path& work) {
    engine::LaneKit kit(config);
    decompile::decompiler::Decompiler& decompiler = kit.decompiler();
    decompiler.set_progress([&r](std::size_t done, std::size_t total) { r.count(done, total, std::format("{} of {} functions", done, total)); });
    const decompile::decompiler::AsC c = decompiler.as_c(input);
    decompiler.set_progress(nullptr);
    r.check();
    fs::write_text(work / (cv_lower(fs::to_text(input.stem())) + ".c"), c.text);
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
                o.nasm = tools.nasm;
                o.find_stub = tools.stub_finder();
                o.say = say;
                cv_keep(source::pe_writer::reconstruct(input, scratch, o).source, work);
                break;
            }
            case CvMz::Ne: {
                source::ne_writer::Options o;
                o.nasm = tools.nasm;
                o.find_stub = tools.stub_finder();
                o.say = say;
                cv_keep(source::ne_writer::reconstruct(input, scratch, o).source, work);
                break;
            }
            case CvMz::Le: {
                source::le_writer::Options o;
                o.nasm = tools.nasm;
                o.say = say;
                cv_keep(source::le_writer::reconstruct(input, scratch, o).source, work);
                break;
            }
            case CvMz::Dos: {
                const source::dos_writer::Result done = source::dos_writer::reconstruct(input, scratch, tools.dos_options());
                if (done.sources.empty()) throw NotConvertible("No source could be written for this DOS program.");
                for (const stdfs::path& s : done.sources) cv_keep(s, work);
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

}  // namespace

std::vector<std::string> program_extensions() {
    engine::register_all_lanes();
    std::vector<std::string> out = {".exe", ".dll", ".com", ".sys", ".vxd", ".drv", ".cpl", ".ocx", ".scr", ".386", ".xex", ".xbe", ".cab", ".iso", ".img", ".bin"};
    for (const std::string& e : engine::lane_program_extensions())
        if (std::find(out.begin(), out.end(), e) == out.end()) out.push_back(e);
    return out;
}

Planner standard_planner() {
    return [](const stdfs::path& input) { return cv_plan(input); };
}

std::string platform_summary() {
    engine::register_all_lanes();
    return std::format("Windows \xC2\xB7 DOS \xC2\xB7 Xbox \xC2\xB7 {} consoles \xC2\xB7 Java \xC2\xB7 .NET", engine::registered_lanes().size());
}

}  // namespace cb::app::convert
