// engine/lane_kit_other.cpp -- the driver lane, the PowerPC lane and the worker kinds (the Java lane: lane_kit_java.cpp), wired (see lane_kit.hpp).
#include "build/driver.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "emulate/driver.hpp"
#include "formats/powerpc_image.hpp"
#include "verify/lane_common.hpp"
#include "verify/lanes/powerpc.hpp"
#include "verify/lanes/win64.hpp"
#include "knowledge/package.hpp"
#include "symbols/prototypes.hpp"
#include "engine/lane_kit_internal.hpp"
#include "source/le_writer.hpp"
#include "symbols/driver_services.hpp"
#include "verify/lanes/driver.hpp"

namespace cb::engine {

namespace stdfs = std::filesystem;
using namespace kit_detail;

std::shared_ptr<const verify::lanes::Kind> LaneKit::driver_lane() {
    {
        std::lock_guard hold(impl_->lock);
        if (impl_->driver_) return impl_->driver_;
    }
    Impl& i = *impl_;
    knowledge::Knowledge& kb = i.knowledge();
    const build::watcom::OpenWatcom ow = i.openwatcom();
    const config::Config& config = i.config;
    emulate::pcode::LifterSet* lifters = i.lifters();
    base::Corpus& corpus = *i.corpus;

    // the names of the services and the messages: the driver kit's, from the knowledge base
    auto table = std::make_shared<symbols::driver_services::Table>(symbols::driver_services::Table::from_rows(
        kb.table("vxd", "device"), kb.table("vxd", "service_table"), kb.table("vxd", "driver"), kb.table("vxd", "message")));

    std::filesystem::path design_reference;
    try {
        design_reference = corpus.design_dir() / "reference";
    } catch (const Error&) {
    }
    const std::filesystem::path kit = symbols::driver_services::find_include_dir(config.text("driver_kit"), design_reference);
    const build::driver::Tools tools = build::driver::find_tools(ow, kit, config.state());

    verify::lanes::DriverLaneSpec spec;
    // The driver written back as assembly source (one NASM file that assembles to the file) and built by the assembler.
    spec.writer = [](const stdfs::path& module, const stdfs::path& out_dir) {
        verify::lanes::Written w;
        fs::make_dirs(out_dir);
        const source::le_writer::Result r = source::le_writer::reconstruct(module, out_dir, {});
        w.source = r.source;
        w.linked = r.image.has_value();
        w.same = r.same;
        if (r.image) w.built = *r.image;
        if (!r.log.empty()) w.notes.push_back(r.log);
        return w;
    };
    // A source folder of the driver kit's macros built by OpenWatcom's wasm and wlink, checked against the original.
    spec.build_source = [tools](const stdfs::path& source, const stdfs::path& original, const stdfs::path& out) {
        fs::make_dirs(out);
        const build::driver::BuildResult r = build::driver::build(tools, {.source = source, .work = out, .original = original});
        verify::lanes::DriverLaneSpec::Rebuilt rebuilt;
        rebuilt.linked = r.linked;
        rebuilt.log = r.log;
        if (r.path) rebuilt.image = *r.path;
        return rebuilt;
    };
    spec.tools = [tools] {
        std::map<std::string, stdfs::path> out;
        out["wasm"] = tools.wasm;
        out["wlink"] = tools.wlink;
        return out;
    };
    spec.services = table.get();
    spec.resolver = emulate::driver::resolver_of(
        [table](std::uint32_t device, std::uint32_t service) { return table->service_name(device, service); },
        [&kb](std::string_view name) -> std::optional<std::string> {
            const std::optional<knowledge::Value> model = kb.api_model("vxd", name);
            if (!model) return std::nullopt;
            const knowledge::Value* where = model->get("answers_in");
            return where != nullptr && where->text() != nullptr ? std::optional<std::string>(*where->text()) : std::nullopt;
        });
    spec.lifters = lifters;
    spec.store = verify::lane_common::store_of(i.cache());
    spec.state = config.state("driver_judge");
    spec.retail = [&corpus](std::string_view name) { return corpus.retail(name); };
    auto lane = std::make_shared<verify::lanes::DriverLane>(std::move(spec));
    // (the lane keeps `spec.services`: the table lives as long as the kit)
    std::lock_guard hold(impl_->lock);
    impl_->driver_tables_ = table;
    return impl_->driver_ = std::move(lane);
}

// What the PowerPC lane reads of an image: its bytes at their addresses (copied: the image is not kept), where it is loaded, where its code lies, and the
// writable data its globals live in (writable and not code).
static verify::lanes::PowerPcLane::Image read_powerpc_image(const std::filesystem::path& path) {
    const formats::powerpc_image::Image image = formats::powerpc_image::Image::read(path);
    verify::lanes::PowerPcLane::Image out;
    for (const formats::powerpc_image::Image::Region& r : image.regions()) out.regions.push_back({r.va, Bytes(r.bytes.begin(), r.bytes.end())});
    out.base = image.base();
    out.functions.assign(image.functions().begin(), image.functions().end());
    out.code = {image.code_begin(), image.code_end()};
    constexpr std::uint32_t kWritable = 0x80000000u, kCode = 0x20000000u;
    for (const formats::powerpc_image::Section& s : image.sections())
        if ((s.characteristics & kWritable) != 0 && (s.characteristics & kCode) == 0) out.writable.push_back({s.va, s.va + s.size});
    return out;
}

std::shared_ptr<const verify::lanes::Kind> LaneKit::powerpc_lane() {
    {
        std::lock_guard hold(impl_->lock);
        if (impl_->powerpc_) return impl_->powerpc_;
    }
    Impl& i = *impl_;
    const std::optional<stdfs::path> folder = i.config.path("powerpc_tools"), lib = i.config.path("powerpc_lib");
    if (!folder || !lib) return nullptr;
    build::functions::PowerPcTools tools{*folder / "cl.exe", *folder / "link.exe", *lib};
    std::error_code ec;
    if (!stdfs::is_regular_file(tools.cl, ec) || !stdfs::is_regular_file(tools.link, ec) || !stdfs::is_regular_file(tools.lib, ec)) return nullptr;
    decompile::decompiler::Decompiler& decompiler = i.decompiler();
    build::functions::FunctionBuilder& builder = i.builder();
    emulate::pcode::LifterSet* lifters = i.lifters();
    base::cache::Cache& cache = i.cache();
    base::Corpus& corpus = *i.corpus;
    verify::lanes::PowerPcLane::Services services{decompiler, builder, std::move(tools), read_powerpc_image, lifters, verify::lane_common::store_of(cache), nullptr};
    services.content_sha = [&corpus](const stdfs::path& p) { return corpus.sha(p); };
    auto lane = std::make_shared<verify::lanes::PowerPcLane>(std::move(services));
    std::lock_guard hold(impl_->lock);
    return impl_->powerpc_ = std::move(lane);
}

std::shared_ptr<const verify::lanes::Kind> LaneKit::win64_lane() {
    {
        std::lock_guard hold(impl_->lock);
        if (impl_->win64_) return impl_->win64_;
    }
    Impl& i = *impl_;
    // Visual C++'s x64 compiler: the setting `msvc_x64` (the folder of its cl.exe and link.exe), else the newest Visual Studio installed
    std::optional<build::functions::X64Tools> tools;
    if (const std::optional<stdfs::path> folder = i.config.path("msvc_x64")) {
        std::error_code ec;
        if (stdfs::is_regular_file(*folder / "cl.exe", ec) && stdfs::is_regular_file(*folder / "link.exe", ec)) tools = build::functions::X64Tools{*folder / "cl.exe", *folder / "link.exe"};
    } else {
        tools = verify::lanes::Win64Lane::find_tools();
    }
    if (!tools) return nullptr;
    knowledge::Knowledge& kb = i.knowledge();
    base::Corpus& corpus = *i.corpus;
    verify::lanes::Win64Lane::Services services{i.decompiler(), i.builder(), std::move(*tools), nullptr, verify::lane_common::store_of(i.cache()), nullptr};
    // how many arguments an import takes, as the Windows SDK declares it (a varargs function: the ones before the dots)
    services.import_argc = [&kb](std::string_view name) -> std::optional<int> {
        const symbols::prototypes::Prototype* p = kb.prototype("win64", name);
        if (p == nullptr) return std::nullopt;
        int n = 0;
        for (const symbols::prototypes::Parameter& parameter : p->parameters) {
            if (parameter.type == "..." || (p->parameters.size() == 1 && parameter.type == "void" && parameter.name.empty())) break;
            ++n;
        }
        return n;
    };
    services.content_sha = [&corpus](const stdfs::path& p) { return corpus.sha(p); };
    auto lane = std::make_shared<verify::lanes::Win64Lane>(std::move(services));
    std::lock_guard hold(impl_->lock);
    return impl_->win64_ = std::move(lane);
}

std::shared_ptr<const verify::lanes::Kind> LaneKit::lane_named(std::string_view name) {
    if (name == "pe") return win32_lane();
    if (name == "ne") return win16_lane();
    if (name == "le") return driver_lane();
    if (name == "powerpc") return powerpc_lane();
    if (name == "win64") return win64_lane();
    if (name == "java") return java_lane();
    if (name == "dos") return dos_lane();
    if (auto machine = machine_lane(name)) return machine;
    return nullptr;
}

}  // namespace cb::engine
