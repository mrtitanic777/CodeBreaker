#include "cb/formats/xbox/crypto/patch_tools.hpp"

#include <algorithm>
#include <format>

#include "cb/core/capstone_api.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/log.hpp"
#include "cb/formats/xbox/crypto/bin2lang.hpp"
#include "cb/formats/xbox/crypto/exp_signer.hpp"

namespace cb::formats::xbox::crypto::patch_tools {

namespace {

namespace stdfs = std::filesystem;

constexpr std::string_view kRglSource = "Patches/RGL/17559-dev/RGLoader-dev.S";
constexpr std::string_view kSc0Source = "Patches/SC0.S";
constexpr std::uint32_t kEnd = 0xFFFFFFFF;

struct RglBuild {
    build_lib::PatchTarget target;
    std::string_view output;
};
constexpr RglBuild kRglBuilds[] = {{build_lib::PatchTarget::Flash, "Output/NonZero/RGL_flash.bin"},
                                   {build_lib::PatchTarget::Hdd, "Output/NonZero/RGL_hdd.bin"},
                                   {build_lib::PatchTarget::VfusesFlash, "Output/Zero/VRGL_flash.bin"},
                                   {build_lib::PatchTarget::VfusesHdd, "Output/Zero/VRGL_hdd.bin"}};

stdfs::path at(const stdfs::path& base, std::string_view relative) { return base / fs::from_text(relative); }

// Assemble and make sure the output's folder is there.
stdfs::path assemble(const build_lib::PatchTools& tools, const stdfs::path& base, std::string_view source, std::string_view output, build_lib::PatchTarget target,
                     std::span<const std::string> defines = {}) {
    const stdfs::path out = at(base, output);
    fs::make_dirs(out.parent_path());
    build_lib::assemble_patch(tools, at(base, source), out, target, defines);
    return out;
}

// One signed expansion and its C++ array: <dir>/<name>_signed.bin and <dir>/<name>.h
void sign_expansion(const xecrypt::RsaKey& key, const Bytes& bl1_key, const stdfs::path& payload_file, const stdfs::path& dir, std::string_view name,
                    exp_signer::Options options, xecrypt::RandomSource& random, std::vector<stdfs::path>& written) {
    options.bl1_key = bl1_key;
    const Bytes signed_image = exp_signer::sign(key, fs::read_file(payload_file), options, random);
    fs::make_dirs(dir);
    const stdfs::path binary = dir / (std::string(name) + "_signed.bin");
    const stdfs::path header = dir / (std::string(name) + ".h");
    fs::write_file(binary, signed_image);
    fs::write_text(header, bin2lang::format(signed_image, bin2lang::Language::Cpp, "ExpansionData"));
    written.push_back(binary);
    written.push_back(header);
}

// Capstone's PowerPC decoder for one word at a time (the patch words are not guaranteed to be instructions).
class PpcDecoder {
public:
    PpcDecoder() {
        const auto& api = disasm::capstone::api();
        disasm::capstone::check(api.cs_open(CS_ARCH_PPC, static_cast<cs_mode>(CS_MODE_32 | CS_MODE_BIG_ENDIAN), &handle_), "cs_open");
    }
    PpcDecoder(const PpcDecoder&) = delete;
    PpcDecoder& operator=(const PpcDecoder&) = delete;
    ~PpcDecoder() { disasm::capstone::api().cs_close(&handle_); }

    // "mnemonic operands" of the word at `address`, nothing when it is not an instruction.
    std::string text_of(ByteView word, std::uint64_t address) {
        const auto& api = disasm::capstone::api();
        cs_insn* insn = nullptr;
        const std::size_t count = api.cs_disasm(handle_, word.data(), word.size(), address, 0, &insn);
        std::string out;
        for (std::size_t i = 0; i < count; ++i) {
            if (i) out += "\n\t";
            const std::string line = std::format("{} {}", insn[i].mnemonic, insn[i].op_str);
            out += text::trim(line, " ");
        }
        if (insn) api.cs_free(insn, count);
        return out;
    }

private:
    csh handle_ = 0;
};

}  // namespace

std::vector<Conflict> find_conflicts(ByteView table) {
    struct Span {
        std::uint32_t start, end;
    };
    std::vector<Span> spans;
    for (const build_lib::Patch& patch : build_lib::patch_records(table)) spans.push_back({patch.address, static_cast<std::uint32_t>(patch.address + patch.words.size())});
    std::vector<Conflict> conflicts;
    for (std::size_t i = 0; i < spans.size(); ++i) {
        for (std::size_t j = 0; j < spans.size(); ++j) {
            if (i == j) continue;
            const Span &x = spans[i], &y = spans[j];
            if (std::uint64_t{x.start} <= std::uint64_t{y.end} + 1 && std::uint64_t{y.start} <= std::uint64_t{x.end} + 1)
                conflicts.push_back({x.start, x.end, y.start, y.end});
        }
    }
    return conflicts;
}

std::string conflict_report(std::span<const Conflict> conflicts) {
    std::string out;
    for (const Conflict& c : conflicts)
        out += std::format("Patch Conflict!\nPatch A: 0x{:08X} - 0x{:08X}\nPatch B: 0x{:08X} - 0x{:08X}\n", c.a_start, c.a_end, c.b_start, c.b_end);
    return out;
}

std::vector<stdfs::path> compile_patches(const build_lib::PatchTools& tools, const stdfs::path& base) {
    std::vector<stdfs::path> written;
    for (const RglBuild& build : kRglBuilds) written.push_back(assemble(tools, base, kRglSource, build.output, build.target));
    written.push_back(assemble(tools, base, kSc0Source, "Output/Compiled/Patches/SC0_dev.bin", build_lib::PatchTarget::Devkit));
    const std::string spoofy = "SPOOFY";
    written.push_back(assemble(tools, base, kSc0Source, "Output/Compiled/Patches/SpoofySC0_dev.bin", build_lib::PatchTarget::Devkit, std::span<const std::string>(&spoofy, 1)));
    return written;
}

std::vector<stdfs::path> build_expansions(const build_lib::PatchTools& tools, const keystore::KeyStore& store, const stdfs::path& base, xecrypt::RandomSource& random) {
    std::vector<stdfs::path> written;
    cb::out("Assembling HVPP.S...\n");
    const stdfs::path hvpp = assemble(tools, base, "Patches/HVPP.S", "Output/Compiled/HVPP.bin", build_lib::PatchTarget::Plain);
    cb::out("Assembling devkit Spoofy.S...\n");
    const stdfs::path spoofy_dev = assemble(tools, base, "Patches/Spoofy.S", "Output/Compiled/Spoofy_dev.bin", build_lib::PatchTarget::Devkit);
    cb::out("Assembling retail Spoofy.S...\n");
    const stdfs::path spoofy_retail = assemble(tools, base, "Patches/Spoofy.S", "Output/Compiled/Spoofy_retail.bin", build_lib::PatchTarget::Retail);
    written.insert(written.end(), {hvpp, spoofy_dev, spoofy_retail});

    const xecrypt::RsaKey key = store.load_and_verify(keystore::KeyFile::HvxPrv);
    const std::optional<Bytes> bl1_key = xecrypt::load_store_key(store.folder(), "1BL_key.bin");
    if (!bl1_key) throw NotFoundError(store.folder() / "1BL_key.bin", 0, "the 1BL key isn't in the key store");

    struct Family {
        std::string_view label;
        std::string_view folder;
        std::uint32_t id;
    };
    for (const Family& family : {Family{"generic", "Output/Compiled/Generic", exp_signer::kDefaultId}, Family{"xbOnline", "Output/Compiled/xbOnline", exp_signer::kXbOnlineId}}) {
        const stdfs::path dir = at(base, family.folder);
        const auto sign = [&](std::string_view what, const stdfs::path& payload, std::string_view name, exp_signer::ExpansionMagic magic, bool encrypt) {
            cb::out("Creating {} {}...\n", family.label, what);
            exp_signer::Options options;
            options.magic = magic;
            options.id = family.id;
            options.encrypt = encrypt;
            sign_expansion(key, *bl1_key, payload, dir, name, options, random, written);
        };
        sign("Spoofy", spoofy_dev, "Spoofy", exp_signer::ExpansionMagic::Hxpr, true);
        sign("devkit HVPP", hvpp, "HVPP_dev", exp_signer::ExpansionMagic::Hxpr, true);
        sign("test kit HVPP", hvpp, "HVPP_test", exp_signer::ExpansionMagic::Sigm, true);
        sign("retail HVPP", hvpp, "HVPP_retail", exp_signer::ExpansionMagic::Hxpr, false);
    }
    cb::out("Done!\n");
    return written;
}

std::string disassemble(ByteView table) {
    PpcDecoder decoder;
    io::ByteReader r(table, "patch table");
    std::string out = "# Patch table read back as assembly by CodeBreaker; check it before building from it.\n\n.include \"macros.S\"\n\n";
    while (!r.eof()) {
        for (;;) {
            const std::uint32_t address = r.u32_be();
            if (address == kEnd) {
                out += "# done with patch section\n\n";
                break;
            }
            const std::uint32_t words = r.u32_be();
            out += std::format("MAKEPATCH 0x{:08X}\n0:\n", address);
            for (std::uint32_t i = 0; i < words; ++i) {
                const ByteView word = r.bytes(4);
                const std::string instruction = decoder.text_of(word, std::uint64_t{address} + std::uint64_t{i} * 4);
                out += instruction.empty() ? std::format("\t.long 0x{}\n", cb::crypto::to_hex(word)) : "\t" + instruction + "\n";
            }
            out += "9:\n\n";
        }
    }
    return out;
}

cli::Command checker_command() {
    return cli::Command{
        "patch_checker",
        "look for patches of a patch table that overlap",
        {cli::Positional{"tables", "patch tables (default: the four RGLoader tables under Output/)", false, true}},
        [](const cli::Args& args) {
            std::vector<std::string> files = args.values("tables");
            if (files.empty()) files = {"Output/NonZero/RGL_flash.bin", "Output/NonZero/RGL_hdd.bin", "Output/Zero/VRGL_flash.bin", "Output/Zero/VRGL_hdd.bin"};
            for (const std::string& file : files) {
                const std::vector<Conflict> conflicts = find_conflicts(fs::read_file(fs::from_text(file)));
                cb::out_text(conflict_report(conflicts));
            }
            return 0;
        },
        {},
        "",
        ""};
}

cli::Command compile_command(const build_lib::PatchTools& tools) {
    return cli::Command{"patch_compile",
                        "assemble the RGLoader and SC0 patches (Patches/ -> Output/)",
                        {cli::Option{"--base", "", "the folder with Patches/ (default: the current folder)", cli::Arg::Value, "DIR", "."}},
                        [tools](const cli::Args& args) {
                            compile_patches(tools, fs::from_text(args.required("base")));
                            return 0;
                        },
                        {},
                        "",
                        ""};
}

cli::Command build_task_command(const build_lib::PatchTools& tools) {
    return cli::Command{"patch_build_task",
                        "compile the patches, then check them for overlaps",
                        {cli::Option{"--base", "", "the folder with Patches/ (default: the current folder)", cli::Arg::Value, "DIR", "."}},
                        [tools](const cli::Args& args) {
                            const stdfs::path base = fs::from_text(args.required("base"));
                            cb::out("Compiling...\n");
                            compile_patches(tools, base);
                            cb::out("Checking...\n");
                            for (const RglBuild& build : kRglBuilds) cb::out_text(conflict_report(find_conflicts(fs::read_file(at(base, build.output)))));
                            cb::out("Done!\n");
                            return 0;
                        },
                        {},
                        "",
                        ""};
}

cli::Command exp_build_command(const keystore::KeyStore& store, const build_lib::PatchTools& tools) {
    return cli::Command{"exp_build_task",
                        "assemble HVPP and Spoofy and sign them as expansions, with their C++ arrays",
                        {cli::Option{"--base", "", "the folder with Patches/ (default: the current folder)", cli::Arg::Value, "DIR", "."}},
                        [store, tools](const cli::Args& args) {
                            build_expansions(tools, store, fs::from_text(args.required("base")));
                            return 0;
                        },
                        {},
                        "",
                        ""};
}

cli::Command dumper_command() {
    return cli::Command{"patch_dumper",
                        "write a patch table (xeBuild's patches_*.bin) as assembly",
                        {cli::Positional{"input", "the patch table"},
                         cli::Option{"--output", "-o", "the assembly file to write (default Output/decompiled_patches.asm)", cli::Arg::Value, "FILE",
                                     "Output/decompiled_patches.asm"}},
                        [](const cli::Args& args) {
                            const stdfs::path output = fs::from_text(args.required("output"));
                            const std::string source = disassemble(fs::read_file(fs::from_text(args.required("input"))));
                            if (output.has_parent_path()) fs::make_dirs(output.parent_path());
                            fs::write_text(output, source);
                            cb::out("Done!\n");
                            return 0;
                        },
                        {},
                        "",
                        ""};
}

}  // namespace cb::formats::xbox::crypto::patch_tools
