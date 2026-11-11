#include "cb/platforms/vxd.hpp"

#include <format>

#include "cb/core/error.hpp"

namespace cb::platforms {

Platform vxd() {
    Platform p;
    p.id = Id::Vxd;
    p.name = "le";
    p.title = "device drivers";
    p.family = "x86";
    p.source = SourceLanguage::Asm;
    p.lane = "driver";
    p.steps = Steps::Lanes;
    p.loader.container = "LE";
    p.cpu = {"x86:LE:32:default:windows", 32, Endian::Little, CapstoneDecoder{Arch::X86, 32},
             {"EAX", "EBX", "ECX", "EDX", "ESI", "EDI", "EBP", "ESP"}, "ESP"};
    p.convention.names = {"registers"};
    p.convention.arguments = InRegisters{};
    p.convention.word = 4;
    p.convention.order = CallOrder::Registers;
    p.convention.results = {"EAX", "CF", "ZF"};
    p.convention.pops = Pops::Callee;
    p.convention.preserved = {"EBX", "ECX", "EDX", "ESI", "EDI", "EBP"};
    p.emulator = {Engine::Unicorn, true};
    p.compiler = {Language::Asm, "openwatcom wasm, the DDK's macros"};
    return p;
}

OrderedMap<std::string, formats::le::Place> vxd_entry_points(const std::filesystem::path& path) {
    auto modules = formats::le::open_modules(path);
    if (modules.empty()) throw FormatError("vxd entry points", 0, "the file holds no LE image");
    const formats::le::Module& first = modules.front().second;
    OrderedMap<std::string, formats::le::Place> out;
    const std::optional<formats::le::Descriptor> d = first.descriptor();
    if (!d) return out;
    if (d->at_control) out.insert_or_assign("control", *d->at_control);
    if (d->at_v86_api) out.insert_or_assign("v86_api", *d->at_v86_api);
    if (d->at_pm_api) out.insert_or_assign("pm_api", *d->at_pm_api);
    const std::vector<std::optional<formats::le::Place>> table = first.service_table(*d);
    for (std::size_t i = 0; i < table.size(); ++i)
        if (table[i]) out.insert_or_assign(std::format("service_{}", i), *table[i]);
    return out;
}

}  // namespace cb::platforms
