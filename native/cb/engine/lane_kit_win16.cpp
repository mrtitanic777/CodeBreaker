// cb/engine/lane_kit_win16.cpp -- the 16-bit lane and its gate, wired (see lane_kit.hpp).
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"
#include "cb/decompile/c_text.hpp"
#include "cb/emulate/windows16.hpp"
#include "cb/engine/lane_kit_internal.hpp"
#include "cb/formats/ne.hpp"
#include "cb/verify/lanes/win16.hpp"

namespace cb::engine {

namespace stdfs = std::filesystem;
using namespace kit_detail;

namespace kit_detail {

// The runner's surroundings: the models of Windows' functions and how many bytes of arguments each removes. The governor is the caller's.
emulate::windows16::Environment win16_environment_of(knowledge::Knowledge& kb, KitSizes& sizes, emulate::native_trace::Governor& governor) {
    emulate::windows16::Environment env;
    env.governor = &governor;
    env.models = emulate::windows16::models_of(kb);
    env.argument_bytes = [&sizes](std::string_view api) { return sizes.win16_bytes(api); };
    return env;
}

}  // namespace kit_detail

std::shared_ptr<const verify::lanes::Kind> LaneKit::win16_lane() {
    {
        std::lock_guard hold(impl_->lock);
        if (impl_->win16_) return impl_->win16_;
    }
    Impl& i = *impl_;
    decompile::decompiler::Decompiler& decompiler = i.decompiler();
    build::functions::FunctionBuilder& builder = i.builder();
    KitSizes& sizes = i.environment().sizes();
    knowledge::Knowledge& kb = i.knowledge();
    emulate::pcode::LifterSet* lifters = i.lifters();
    const build::watcom::OpenWatcom ow = i.openwatcom();

    verify::lanes::Win16LaneSpec spec;
    // the module decompiled and made compilable
    spec.compilable = [&decompiler](const stdfs::path& module, const stdfs::path& work) {
        return std::make_shared<decompile::c_text::Module16>(decompile::c_text::compilable16(decompiler, module, work, decompile::c_text::Policy::from_environment()));
    };
    // OpenWatcom's 16-bit tools
    build::functions::Tools16 tools;
    tools.wcc = {ow.tool("wcc"), ow.environment(build::watcom::Target::Win16)};
    tools.wlink = ow.tool("wlink");
    tools.wasm = ow.tool("wasm");
    tools.lib_dir = ow.lib_dir(build::watcom::Target::Win16);
    tools.interrupt_routines = [](const std::set<int>& vectors) {
        const std::set<std::uint32_t> v(vectors.begin(), vectors.end());
        const decompile::c_text::InterruptRoutines r = decompile::c_text::interrupt_routines(v);
        return build::functions::Tools16::InterruptCode{r.assembly, r.c};
    };
    spec.build_dll = [&builder, tools](const decompile::c_text::Module16& m, const stdfs::path& work, const std::map<int, Bytes>& code_images) {
        const OrderedMap<std::string, std::string> made_texts = m.made.texts();
        build::functions::Texts texts;
        for (const auto& [name, text] : made_texts) texts[name] = text;
        build::functions::Build16Input in{.header = m.made.header(), .texts = texts, .order = m.made.order(), .outdir = work};
        for (const auto& [symbol, import] : m.imports) in.imports[symbol] = {import.first, static_cast<std::uint32_t>(import.second)};
        for (const auto& [seg, bytes] : m.segments) in.segments[static_cast<int>(seg)] = bytes;
        for (const auto& [va, name] : m.made.by_va()) {
            const auto where = m.where.find(static_cast<std::int64_t>(va));
            if (where != m.where.end()) in.code_segment[name] = static_cast<int>(where->second.seg);
        }
        for (const decompile::c_text::Interrupt& interrupt : m.interrupts) in.interrupts.insert(static_cast<int>(interrupt.vector));
        for (const auto& [seg, text] : m.code_data) in.code_data[static_cast<int>(seg)] = text;
        in.code_image = code_images;
        const build::functions::Built16 b = builder.build16(tools, in);
        verify::lanes::Win16LaneSpec::Rebuilt r;
        r.dll = b.dll;
        r.where = b.where;
        r.dropped = b.dropped;
        return r;
    };
    std::string options;
    for (const std::string& flag : build::functions::cflags16()) options += (options.empty() ? "" : " ") + flag;
    spec.compiler_options = options;
    spec.environment = [&kb, &sizes](emulate::native_trace::Governor& governor) { return win16_environment_of(kb, sizes, governor); };
    spec.lifters = lifters;
    auto lane = std::make_shared<verify::lanes::Win16Lane>(std::move(spec));
    std::lock_guard hold(impl_->lock);
    return impl_->win16_ = std::move(lane);
}

verify::gate16::Isolated16Spec LaneKit::isolated16_spec() {
    Impl& i = *impl_;
    knowledge::Knowledge& kb = i.knowledge();
    KitSizes& sizes = i.environment().sizes();
    verify::gate16::Isolated16Spec spec;
    spec.environment = [&kb, &sizes](emulate::native_trace::Governor& governor) { return win16_environment_of(kb, sizes, governor); };
    spec.lifters = i.lifters();
    spec.tables = [this](const stdfs::path& original) { return gate_tables(original); };
    return spec;
}

}  // namespace cb::engine
