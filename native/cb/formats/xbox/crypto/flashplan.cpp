#include "cb/formats/xbox/crypto/flashplan.hpp"

#include <algorithm>
#include <array>
#include <format>

#include "cb/core/error.hpp"
#include "cb/core/log.hpp"

namespace cb::formats::xbox::crypto::flashplan {

namespace {

constexpr std::array<Motherboard, 8> kBoards = {Motherboard::Xenon,   Motherboard::Zephyr,  Motherboard::Falcon, Motherboard::Opus,
                                                Motherboard::Jasper,  Motherboard::Trinity, Motherboard::Corona, Motherboard::Winchester};

// The hack names that resolve to each image type (matched exactly).
constexpr std::string_view kGlitchHacks[] = {"RGH 1"};
constexpr std::string_view kGlitch2Hacks[] = {"RGH 1.2", "RGH 2", "S-RGH", "RGH2"};
constexpr std::string_view kGlitch3Hacks[] = {"RGH 3", "RGH 1.3", "EXT_CLK.3", "RGH3"};
constexpr std::string_view kJtagHacks[] = {"Argon Data", "AUD_CLAMP", "R-JTAG", "JTAG"};
constexpr std::string_view kDevGlHacks[] = {"DevGL"};
constexpr std::string_view kDevRglHacks[] = {"DevRGL"};

struct HackGroup {
    ImageType type;
    std::span<const std::string_view> names;
};
constexpr std::array<HackGroup, 6> kHackGroups = {{{ImageType::Glitch, kGlitchHacks},
                                                   {ImageType::Glitch2, kGlitch2Hacks},
                                                   {ImageType::Glitch3, kGlitch3Hacks},
                                                   {ImageType::Jtag, kJtagHacks},
                                                   {ImageType::DevGl, kDevGlHacks},
                                                   {ImageType::DevRgl, kDevRglHacks}}};

bool in_group(ImageType type, std::string_view hack) {
    for (const HackGroup& group : kHackGroups)
        if (group.type == type) return std::find(group.names.begin(), group.names.end(), hack) != group.names.end();
    return false;
}

std::string capitalised(std::string_view s) {
    std::string out(s);
    if (!out.empty()) out[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(out[0])));
    return out;
}

struct CpldPart {
    std::uint32_t idcode;
    std::string_view part;
};
// The CoolRunner-II XC2C64A is the canonical glitch CPLD; both silicon revisions share the low IDCODE bits.
constexpr std::array<CpldPart, 2> kCplds = {{{0x06E5E093, "XC2C64A (CoolRunner-II)"}, {0x06E5C093, "XC2C64A (CoolRunner-II)"}}};

}  // namespace

std::string_view name(Motherboard board) {
    switch (board) {
        case Motherboard::Xenon: return "xenon";
        case Motherboard::Zephyr: return "zephyr";
        case Motherboard::Falcon: return "falcon";
        case Motherboard::Opus: return "opus";
        case Motherboard::Jasper: return "jasper";
        case Motherboard::Trinity: return "trinity";
        case Motherboard::Corona: return "corona";
        case Motherboard::Winchester: return "winchester";
        case Motherboard::Unknown: return "unknown";
    }
    return "unknown";
}

std::string_view name(ImageType type) {
    switch (type) {
        case ImageType::Retail: return "retail";
        case ImageType::Glitch: return "glitch";
        case ImageType::Glitch2: return "glitch2";
        case ImageType::Glitch3: return "glitch3";
        case ImageType::Jtag: return "jtag";
        case ImageType::DevGl: return "devgl";
        case ImageType::DevRgl: return "devrgl";
        case ImageType::Devkit: return "devkit";
    }
    return "retail";
}

std::string_view name(SecureBootBlock block) {
    switch (block) {
        case SecureBootBlock::Ksb: return "KSB";
        case SecureBootBlock::Psb: return "PSB";
        case SecureBootBlock::Xsb: return "XSB";
    }
    return "XSB";
}

Motherboard classify_motherboard(std::string_view model) {
    const std::string lowered = text::lower(model);
    for (const Motherboard board : kBoards)
        if (lowered.find(name(board)) != std::string::npos) return board;
    return Motherboard::Unknown;
}

ImageType image_type(std::string_view console_class, std::string_view hack) {
    const std::string klass = text::lower(text::trim(console_class));
    const std::string_view hk = text::trim(hack);
    if (klass == "freeboot" || klass == "rgh" || klass == "jtag") {
        if (in_group(ImageType::Glitch, hk)) return ImageType::Glitch;
        if (in_group(ImageType::Glitch3, hk)) return ImageType::Glitch3;
        if (in_group(ImageType::Jtag, hk)) return ImageType::Jtag;
        return ImageType::Glitch2;
    }
    if (klass == "devkit") {
        if (in_group(ImageType::DevGl, hk)) return ImageType::DevGl;
        if (in_group(ImageType::DevRgl, hk)) return ImageType::DevRgl;
        return ImageType::Devkit;
    }
    return ImageType::Retail;
}

SecureBootBlock secure_boot_block(std::string_view model, bool nandmu) {
    const std::string m = text::lower(model);
    if (m.find("corona") != std::string::npos && (m.find("4g") != std::string::npos || nandmu)) return SecureBootBlock::Ksb;
    if (m.find("jasper") != std::string::npos || m.find("trinity") != std::string::npos) return SecureBootBlock::Psb;
    return SecureBootBlock::Xsb;
}

std::string_view cpld_part(std::uint32_t idcode) {
    for (const CpldPart& cpld : kCplds)
        if (cpld.idcode == idcode) return cpld.part;
    return "Unknown JTAG device";
}

Plan plan(std::string_view model, std::string_view hack, std::string_view console_class, bool nandmu) {
    return Plan{classify_motherboard(model), std::string(console_class), std::string(hack), image_type(console_class, hack), secure_boot_block(model, nandmu), nandmu};
}

std::span<const Motherboard> motherboards() { return kBoards; }

cli::Command command() {
    cli::Command plan_command{
        "plan",
        "the image type and secure-boot block for a console and a hack",
        {cli::Positional{"model", "the console model, e.g. \"Corona 4G\""}, cli::Positional{"hack", "the hack, e.g. \"RGH 3\""},
         cli::Option{"--class", "", "Retail, FreeBoot (default) or Devkit", cli::Arg::Value, "CLASS", "FreeBoot"},
         cli::Option{"--nandmu", "", "a NAND memory unit build", cli::Arg::Flag, "", ""}},
        [](const cli::Args& args) {
            const std::string model = args.required("model");
            const Plan p = plan(model, args.required("hack"), args.required("class"), args.flag("nandmu"));
            cb::out("console model     : {}\n", model);
            cb::out("motherboard       : {}\n", name(p.motherboard));
            cb::out("console class      : {}\n", p.console_class);
            cb::out("hack               : {}\n", p.hack);
            cb::out("xeBuild image type : {}\n", name(p.image_type));
            cb::out("secure-boot block  : {}\n", name(p.secure_boot_block));
            cb::out("\nbuild the image with xeBuild/J-Runner (image type '{}', {} override).\n", name(p.image_type), name(p.secure_boot_block));
            return 0;
        },
        {}, "", ""};
    cli::Command image_command{
        "image-type",
        "the xeBuild image type for a console class and a hack",
        {cli::Positional{"class", "Retail, FreeBoot or Devkit"}, cli::Positional{"hack", "the hack name"}},
        [](const cli::Args& args) {
            cb::out("{}\n", name(image_type(args.required("class"), args.required("hack"))));
            return 0;
        },
        {}, "", ""};
    cli::Command secure_command{
        "secure-boot",
        "the secure-boot block a console model needs",
        {cli::Positional{"model", "the console model"}, cli::Option{"--nandmu", "", "a NAND memory unit build", cli::Arg::Flag, "", ""}},
        [](const cli::Args& args) {
            cb::out("{}\n", name(secure_boot_block(args.required("model"), args.flag("nandmu"))));
            return 0;
        },
        {}, "", ""};
    cli::Command cpld_command{
        "cpld",
        "the glitch chip a JTAG IDCODE belongs to",
        {cli::Positional{"idcode", "the IDCODE in hex"}},
        [](const cli::Args& args) {
            const std::string text_value = args.required("idcode");
            const std::string_view digits = text::istarts_with(text_value, "0x") ? std::string_view(text_value).substr(2) : std::string_view(text_value);
            const std::optional<std::uint32_t> idcode = text::parse_int<std::uint32_t>(digits, 16);
            if (!idcode) throw UsageError("the IDCODE is not hex: " + text_value);
            cb::out("IDCODE 0x{:08X} -> {}\n", *idcode, cpld_part(*idcode));
            return 0;
        },
        {}, "", ""};
    cli::Command boards_command{
        "motherboards",
        "list the recognised boards",
        {},
        [](const cli::Args&) {
            std::string line;
            for (const Motherboard board : motherboards()) line += (line.empty() ? "" : "  ") + capitalised(name(board));
            cb::out("  {}\n", line);
            return 0;
        },
        {}, "", ""};
    cli::Command hacks_command{
        "hacks",
        "list the recognised hack names by image type",
        {},
        [](const cli::Args&) {
            for (const HackGroup& group : kHackGroups) {
                std::string names;
                for (const std::string_view hack : group.names) names += (names.empty() ? "" : ", ") + std::string(hack);
                cb::out("  {:<8} {}\n", name(group.type), names);
            }
            cb::out("  {:<8} {}\n", "retail", "Retail");
            return 0;
        },
        {}, "", ""};
    return cli::Command{"flashplan",
                        "RGH/JTAG flash planner: the image type and secure-boot block (no image is written)",
                        {},
                        {},
                        {plan_command, image_command, secure_command, cpld_command, boards_command, hacks_command},
                        "",
                        "An advisor only: it writes no flashable image; build with xeBuild or J-Runner."};
}

}  // namespace cb::formats::xbox::crypto::flashplan
