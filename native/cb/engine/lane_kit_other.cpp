// cb/engine/lane_kit_other.cpp -- the driver lane, the Java lane and the worker kinds, wired (see lane_kit.hpp).
#include "cb/build/driver.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/emulate/driver.hpp"
#include "cb/engine/lane_kit_internal.hpp"
#include "cb/source/le_writer.hpp"
#include "cb/symbols/driver_services.hpp"
#include "cb/verify/lanes/driver.hpp"

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
    const std::filesystem::path nasm = config.path("nasm").value_or(std::filesystem::path());
    spec.writer = [nasm](const stdfs::path& module, const stdfs::path& out_dir) {
        verify::lanes::Written w;
        fs::make_dirs(out_dir);
        const source::le_writer::Result r = source::le_writer::reconstruct(module, out_dir, {.nasm = nasm});
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

std::shared_ptr<const verify::lanes::Kind> LaneKit::java_lane() { return nullptr; }

std::string_view LaneKit::java_missing() {
    return "the Java lane needs the transpiler's Services (data, runtime, target profiles, projects), which have no production assembly in native/cb yet";
}

std::shared_ptr<const verify::lanes::Kind> LaneKit::lane_named(std::string_view name) {
    if (name == "pe") return win32_lane();
    if (name == "ne") return win16_lane();
    if (name == "le") return driver_lane();
    return nullptr;
}

}  // namespace cb::engine
