// LOCAL scratch (never committed)
#include "cb/build/msc6.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

CB_TEST("w1b4_scratch.sdk", [] {
    const auto home = cb::fs::get_env("CB_W1B4_MSVC152");
    if (!home) return;
    cb::build::msc6::Tools tools;
    tools.msvc152 = cb::fs::from_text(*home);
    const std::string sdk = cb::build::msc6::windows_h(tools).value_or("");
    const auto protos = cb::build::msc6::sdk_prototypes(sdk);
    const std::filesystem::path out = cb::fs::from_text(cb::fs::get_env("CB_W1B4_OUT").value());
    std::map<std::pair<std::string, std::uint32_t>, std::string> ordinal;
    for (const std::string_view line : cb::text::lines(cb::fs::read_text(cb::fs::from_text(cb::fs::get_env("CB_W1B4_MSC6").value()) / "ordinals.txt"))) {
        const auto f = cb::text::split(line, '\t');
        if (f.size() >= 3 && f[2].starts_with('=')) ordinal[{std::string(f[0]), *cb::text::parse_int<std::uint32_t>(f[1])}] = std::string(f[2].substr(1));
    }
    std::string report = "ordinals " + std::to_string(ordinal.size()) + " USER436=" + (ordinal.contains({"USER", 436}) ? ordinal[{"USER", 436}] : "-") + "\n";
    const auto names = [&](std::string_view m, std::uint32_t n) -> std::optional<std::string> {
        const auto it = ordinal.find({std::string(m), n});
        if (it == ordinal.end()) return std::nullopt;
        return it->second;
    };
    const std::string text = cb::fs::read_text(out / "msc6" / "po_lzexpand_030.txt");
    const std::string got = cb::build::msc6::sdk_calls(text, protos, names);
    report += got.find("IsCharLower") != std::string::npos ? "resolved\n" : "NOT resolved\n";
    const std::string one = "      iVar1 = USER_436(CONCAT11((char)((uint4)pcVar3 >> 8),*(uint1 __far *)((uint1 __far *)((uint1 __far *)param_1))));\n";
    report += cb::build::msc6::sdk_calls(one, protos, names).find("IsCharLower") != std::string::npos ? "one resolved\n" : "one NOT resolved\n";
    cb::fs::write_text(out / "scratch.txt", report);
});
