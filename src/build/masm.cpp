#include "build/masm.hpp"

#include <algorithm>
#include <format>
#include <set>

#include "core/fs.hpp"
#include "core/io.hpp"
#include "core/log.hpp"
#include "core/text.hpp"

namespace cb::build::masm {

namespace {

namespace stdfs = std::filesystem;
using native_dos::Finished;
using native_dos::Product;
using native_dos::Source;

// The cache's layout number for a kept assembly: 4 bytes of the object's length, the object, the listing's text.
// (The listing is Latin-1 text held as UTF-8; a log is not kept -- a kept assembly made an object, and its log said nothing.)
constexpr std::uint32_t kCacheSchema = 2;
// Bump when the invocation changes in a way that alters what MASM makes.
constexpr std::uint32_t kInvocationVersion = 1;
constexpr std::string_view kCacheKind = "masm";

constexpr std::string_view kSelfTestStem = "t";

bool has_masm(const stdfs::path& root) {
    std::error_code ec;
    return stdfs::is_regular_file(root / "BIN" / "MASM.EXE", ec);
}

// What the cache keeps of a product: made when there is an object.
Bytes kept(const Product& p) {
    io::ByteWriter w;
    w.u32(static_cast<std::uint32_t>(p.obj->size()));
    w.bytes(*p.obj);
    w.bytes(as_bytes(p.lst.value_or(std::string())));
    return w.take();
}

std::optional<Product> unkept(const Bytes& data) {
    try {
        io::ByteReader r(data, "kept assembly");
        const std::uint32_t n = r.u32();
        Product p;
        const ByteView obj = r.bytes(n);
        p.obj = Bytes(obj.begin(), obj.end());
        const ByteView lst = r.bytes(r.remaining());
        p.lst = std::string(as_text(lst));
        p.log = std::string();
        return p;
    } catch (const FormatError&) {   // damaged under a valid envelope: a miss
        return std::nullopt;
    }
}

}  // namespace

// ---- tools -----------------------------------------------------------------------------------------------------------

Tools Tools::from_config(const config::Config& config) {
    Tools t;
    t.masm = config.path("masm").value_or(stdfs::path());
    return t;
}

// ---- the Assembler -----------------------------------------------------------------------------------------------------

Product Assembler::assemble(std::string_view source, std::string_view stem, const std::optional<stdfs::path>& root_override) {
    const stdfs::path root = root_override && !root_override->empty() ? *root_override : context_.tools.masm;
    if (!has_masm(root)) throw NotFoundError(root / "BIN" / "MASM.EXE", 2, "MASM.EXE not found (set it: CodeBreaker config masm <path>)");
    require_executive();
    return native_dos::assemble(context_.dos, source, stem, root);
}

Finished Assembler::assemble_many(std::span<const Source> items, const std::optional<stdfs::path>& root_override) {
    const stdfs::path root = root_override && !root_override->empty() ? *root_override : context_.tools.masm;
    std::set<std::string> stems;
    for (const Source& item : items)
        if (!stems.insert(item.stem).second) throw UsageError("assemble_many: the stem " + item.stem + " is given twice");

    // what the assembler's folder is, by bytes: MASM.EXE and the INCLUDE files a source may read
    std::string root_id;
    std::vector<std::string> keys(items.size());
    std::vector<std::optional<Product>> hits(items.size());
    std::vector<Source> misses;
    std::vector<std::size_t> miss_at;
    base::cache::Cache* cache = context_.cache;
    if (cache != nullptr) {
        std::error_code ec;
        if (stdfs::is_directory(root, ec)) {
            const stdfs::path folder = root;
            root_id = cache->files_bytes_id(std::span<const stdfs::path>(&folder, 1));
        } else {
            root_id = cache->tool_bytes_id(root);
        }
    }
    for (std::size_t i = 0; i < items.size(); ++i) {
        if (cache != nullptr) {
            // the object's THEADR and the listing's title name the source file, so the stem is part of what MASM makes
            keys[i] = base::cache::key({std::string(kCacheKind), kInvocationVersion, root_id, text::upper(items[i].stem), items[i].text});
            if (const auto blob = cache->get(kCacheKind, keys[i], kCacheSchema)) hits[i] = unkept(*blob);
        }
        if (!hits[i]) {
            misses.push_back(items[i]);
            miss_at.push_back(i);
        }
    }
    Finished done;
    if (!misses.empty()) {
        if (!has_masm(root)) throw NotFoundError(root / "BIN" / "MASM.EXE", 2, "MASM.EXE not found (set it: CodeBreaker config masm <path>)");
        require_executive();
        Finished fresh = native_dos::assemble_many(context_.dos, misses, root);
        for (std::size_t m = 0; m < miss_at.size(); ++m) {
            Product& p = fresh.results.at(m).second;
            if (cache != nullptr && p.obj && !p.obj->empty()) cache->put(kCacheKind, keys[miss_at[m]], kept(p), kCacheSchema);   // never a failed assembly
            hits[miss_at[m]] = std::move(p);
        }
        done.work = fresh.work;
    }
    for (std::size_t i = 0; i < items.size(); ++i) done.results.emplace_back(items[i].stem, std::move(*hits[i]));
    return done;
}

void Assembler::require_executive() const {
    if (!context_.dos.available()) throw native_dos::NativeDosError("the DOS executive is not available here (no emulator engine could be opened)");
}

OrderedMap<std::string, formats::omf::FunctionBytes> Assembler::functions(std::string_view source) {
    const Product p = assemble(source);
    if (!p.obj) {
        const std::string log = p.log.value_or(std::string());
        throw ToolError("masm", 0, log.substr(0, 600), "MASM produced no object");
    }
    return formats::omf::function_bytes(*p.obj);
}

// ---- the selftest and the command ------------------------------------------------------------------------------------------

const std::string_view kSelfTestSource =
    "_TEXT   segment byte public 'CODE'\n"
    "        assume  cs:_TEXT\n"
    "        public  _addasm\n"
    "_addasm proc near\n"
    "        push    bp\n"
    "        mov     bp, sp\n"
    "        mov     ax, [bp+4]\n"
    "        add     ax, [bp+6]\n"
    "        pop     bp\n"
    "        ret\n"
    "_addasm endp\n"
    "_TEXT   ends\n"
    "        end\n";

bool selftest(Assembler& assembler, std::string& report) {
    const Product p = assembler.assemble(kSelfTestSource, kSelfTestStem);
    if (!p.obj) {
        report += "no object; MASM log:\n" + p.log.value_or(std::string()).substr(0, 800) + "\n";
        return false;
    }
    const auto functions = formats::omf::function_bytes(*p.obj);
    std::string sizes;
    for (const auto& [name, f] : functions) sizes += (sizes.empty() ? "" : ", ") + std::format("{}={}", name, f.code.size());
    report += "functions: " + sizes + "\n";
    // MASM upper-cases public symbols by default (_ADDASM): match case-insensitively
    const Bytes* code = nullptr;
    for (const auto& [name, f] : functions)
        if (text::iequals(name, "_addasm")) code = &f.code;
    std::string hex;
    if (code != nullptr)
        for (std::uint8_t b : *code) hex += std::format("{:02x}", b);
    report += "_addasm bytes: " + hex + "\n";
    // 55 8b ec  8b 46 04  03 46 06  5d c3  =  push bp; mov bp,sp; mov ax,[bp+4]; add ax,[bp+6]; pop bp; ret
    static constexpr std::uint8_t kPrologue[] = {0x55, 0x8B, 0xEC};
    static constexpr std::uint8_t kAdd[] = {0x03, 0x46, 0x06};
    const bool ok = code != nullptr && code->size() >= 3 && std::equal(std::begin(kPrologue), std::end(kPrologue), code->begin()) &&
                    std::search(code->begin(), code->end(), std::begin(kAdd), std::end(kAdd)) != code->end() && code->back() == 0xC3;
    report += std::string("\nSELFTEST: ") + (ok ? "PASS - MASM assembled byte-exact proc" : "FAIL") + "\n";
    return ok;
}

cli::Command command(const Context& context) {
    return cli::Command{
        "masm",
        "assemble a file with the period MASM and list the segments of its object, or check the assembler (--selftest)",
        {cli::Positional{"asm", "the .ASM file", false, false},
         cli::Option{"--selftest", "", "assemble a tiny proc and check its bytes", cli::Arg::Flag}},
        [context](const cli::Args& args) {
            Assembler assembler(context);
            if (args.flag("selftest")) {
                std::string report;
                const bool ok = selftest(assembler, report);
                cb::out("{}", report);
                return ok ? 0 : 1;
            }
            const auto file = args.value("asm");
            if (!file) throw UsageError("give a .ASM file or --selftest");
            const std::string source = fs::read_text(fs::from_text(*file));
            const Product p = assembler.assemble(source);
            if (!p.obj) {
                cb::err("no object produced; MASM log:\n{}\n", p.log.value_or(std::string()).substr(0, 1000));
                return 1;
            }
            const formats::omf::Object object = formats::omf::read(*p.obj);
            for (const formats::omf::SegDef& s : object.segdefs) {
                const Bytes* bytes = object.segments.find(s.name);
                cb::out("  {:<12} class={:<8} {:>6} bytes\n", s.name, s.class_name, bytes != nullptr ? bytes->size() : std::size_t{0});
            }
            return 0;
        },
        {},
        "Build",
        ""};
}

}  // namespace cb::build::masm
