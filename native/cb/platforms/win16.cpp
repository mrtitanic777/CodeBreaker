#include "cb/platforms/win16.hpp"

#include <string_view>

#include "cb/core/error.hpp"

namespace cb::platforms {

Platform win16() {
    Platform p;
    p.id = Id::Win16;
    p.name = "ne";
    p.title = "16-bit Windows";
    p.family = "x86";
    p.source = SourceLanguage::CorAsm;
    p.lane = "win16";
    p.steps = Steps::Win16;
    p.loader.container = "NE";
    p.cpu = {"x86:LE:16:Protected Mode:default", 16, Endian::Little, CapstoneDecoder{Arch::X86, 16},
             {"AX", "BX", "CX", "DX", "SI", "DI", "BP", "SP", "CS", "DS", "ES", "SS"}, "SP"};
    p.convention.names = {"__stdcall16far",   "__stdcall16near", "__cdecl16far",
                          "__cdecl16near",    "__fastcall16near", "__fastcall16far"};
    p.convention.arguments = kWin16ArgumentWords;
    p.convention.word = 2;
    p.convention.order = CallOrder::Pascal;
    p.convention.results = {"AX", "DX:AX"};
    p.convention.pops = Pops::Either;
    p.convention.preserved = {"SI", "DI", "DS"};
    p.emulator = {Engine::Unicorn, true};
    p.compiler = {Language::C, "openwatcom"};
    return p;
}

std::vector<formats::ne::Segment> ne_segments(const std::filesystem::path& path) {
    return formats::ne::Module::open_file(path).segments();
}

std::vector<formats::ne::Entry> ne_entries(const std::filesystem::path& path) {
    return formats::ne::Module::open_file(path).entries();
}

Win16Call call_win16_export(const std::filesystem::path& path, std::uint32_t ordinal, const Win16Runner& run) {
    const std::vector<std::int64_t> args(kWin16ArgumentWords, 0);
    try {
        const Win16Run r = run(path, ordinal, args);
        Win16Call out{r.ended, std::nullopt};
        if (const auto it = r.results.find("AX"); it != r.results.end()) out.ax = it->second;
        return out;
    } catch (const UsageError&) {
        throw;   // a broken setup is not a result
    } catch (const InternalError&) {
        throw;
    } catch (const Error& e) {   // the emulator giving up is a result too (isolation boundary of one export)
        return {"error: " + std::string(e.kind()), std::nullopt};
    }
}

}  // namespace cb::platforms
