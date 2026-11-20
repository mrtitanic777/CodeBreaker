#include "build/masm.hpp"

#include <algorithm>
#include <format>
#include <set>
#include <variant>

#include "core/fs.hpp"
#include "core/io.hpp"
#include "core/log.hpp"
#include "core/parallel.hpp"
#include "core/text.hpp"

namespace cb::build::masm {

namespace {

namespace stdfs = std::filesystem;
using dos_sessions::Finished;
using dos_sessions::Product;
using dos_sessions::Source;

// The cache's layout number for a kept assembly: 4 bytes of the object's length, the object, the listing's text.
// (The listing is Latin-1 text held as UTF-8; a log is not kept -- a kept assembly made an object, and its log said nothing.)
constexpr std::uint32_t kCacheSchema = 2;
// Bump when the invocation changes in a way that alters what MASM makes.
constexpr std::uint32_t kInvocationVersion = 1;
constexpr std::string_view kCacheKind = "masm";

constexpr std::chrono::seconds kColdBase{120}, kColdPerSource{3};
constexpr std::string_view kSelfTestStem = "t";

std::optional<std::string> env(const char* name) {
    auto v = fs::get_env(name);
    if (v && v->empty()) return std::nullopt;
    return v;
}

// An environment number: absent or not a number is the default; below `least` is `least`.
std::size_t env_count(const char* name, std::size_t fallback, std::size_t least) {
    const auto v = env(name);
    if (!v) return fallback;
    const auto n = text::parse_int<std::int64_t>(text::trim(*v));
    if (!n) return fallback;
    return static_cast<std::size_t>(std::max<std::int64_t>(*n, static_cast<std::int64_t>(least)));
}

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

// The product of a stem from what a DOS run made (`made` by upper-case artifact name).
Product product_of(const std::map<std::string, Bytes>& made, const std::string& stem) {
    Product p;
    const auto part = [&](const char* extension) -> const Bytes* {
        const auto it = made.find(text::upper(stem) + extension);
        return it == made.end() ? nullptr : &it->second;
    };
    if (const Bytes* obj = part(".OBJ")) p.obj = *obj;
    if (const Bytes* lst = part(".LST")) p.lst = text::from_latin1(*lst);
    if (const Bytes* log = part(".LOG")) p.log = text::from_latin1(*log);
    return p;
}

}  // namespace

// ---- tools and options ----------------------------------------------------------------------------------------------

Tools Tools::from_config(const config::Config& config) {
    Tools t;
    t.masm = config.path("masm").value_or(stdfs::path());
    t.dosbox_x = config.path("dosbox_x").value_or(stdfs::path());
    t.dosbox_forbidden = env("CB_NO_DOSBOX").has_value();
    return t;
}

Options Options::from_environment() {
    Options o;
    if (const auto core = env("WT_DOSBOX_CORE")) o.dosbox_core = *core;
    o.pool_max = env_count("WT_POOL_MAX", o.pool_max, 1);
    o.warm_pool = env("WT_NO_WARM_POOL").value_or("") != "1";
    o.worker_max_uses = env_count("WT_WORKER_MAX_USES", o.worker_max_uses, 0);
    o.batch = env_count("WT_ASM_BATCH", o.batch, 1);
    return o;
}

// ---- the Assembler -----------------------------------------------------------------------------------------------------

// The warm sessions, one pool per assembler folder, and how many batches each session has done.
struct Assembler::Pools {
    std::mutex lock;
    std::map<stdfs::path, std::shared_ptr<dos_sessions::WorkerPool>> by_root;
    std::map<const dos_sessions::Session*, std::size_t> uses;
};

Assembler::Assembler(Context context) : context_(std::move(context)), pools_(std::make_unique<Pools>()) {}

Assembler::~Assembler() { close_sessions(); }

void Assembler::close_sessions() {
    std::lock_guard lock(pools_->lock);
    for (auto& [root, pool] : pools_->by_root) {
        try {
            pool->close();
        } catch (const std::exception&) {   // closing a dead session: nothing left to do
        }
    }
    pools_->by_root.clear();
    pools_->uses.clear();
}

void Assembler::require_dosbox() const {
    if (context_.tools.dosbox_forbidden)
        throw ToolError("dosbox-x", 0, "",
                        "DOSBox is disabled (CB_NO_DOSBOX=1): the optional period-compiler byte-match cannot run. Unset "
                        "CB_NO_DOSBOX to allow the spot-check (the native behaviour gate does not need it)");
    std::error_code ec;
    if (context_.tools.dosbox_x.empty() || !stdfs::exists(context_.tools.dosbox_x, ec))
        throw ToolError("dosbox-x", 0, "",
                        "dosbox-x not found: " + fs::to_text(context_.tools.dosbox_x) + " (set it: CodeBreaker config dosbox_x <path>)");
}

Product Assembler::assemble(std::string_view source, std::string_view stem, std::chrono::milliseconds timeout,
                            const std::optional<stdfs::path>& root_override) {
    const stdfs::path root = root_override && !root_override->empty() ? *root_override : context_.tools.masm;
    if (native_dos::enabled(context_.dos)) {
        try {
            return native_dos::assemble(context_.dos, source, stem, root);
        } catch (const native_dos::NativeDosError&) {   // the native runner could not: DOSBox
        }
    }
    require_dosbox();
    if (!has_masm(root)) throw NotFoundError(root / "BIN" / "MASM.EXE", 2, "MASM.EXE not found (set it: CodeBreaker config masm <path>)");
    const std::vector<Source> one{{std::string(stem), std::string(source)}};
    return cold_boot(one, timeout, root).results.front().second;
}

Finished Assembler::assemble_many(std::span<const Source> items, std::optional<std::chrono::milliseconds> timeout,
                                  const std::optional<stdfs::path>& root_override) {
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
        Finished fresh = assemble_uncached(misses, timeout, root);
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

Finished Assembler::assemble_uncached(std::span<const Source> items, std::optional<std::chrono::milliseconds> timeout,
                                      const stdfs::path& root) {
    if (native_dos::enabled(context_.dos)) {
        try {
            return native_dos::assemble_many(context_.dos, items, root);
        } catch (const native_dos::NativeDosError&) {   // the native runner could not: DOSBox
        }
    }
    require_dosbox();   // (honours CB_NO_DOSBOX)
    if (!has_masm(root)) throw NotFoundError(root / "BIN" / "MASM.EXE", 2, "MASM.EXE not found (set it: CodeBreaker config masm <path>)");
    return assemble_with_dosbox(items, timeout, root);
}

// Chunk a large module so that no single dispatch can overflow the cold boot's autoexec; the chunks run side by side, each
// leasing a session of its own (the pool caps the live sessions, so callers that run concurrently share the same cap).
Finished Assembler::assemble_with_dosbox(std::span<const Source> items, std::optional<std::chrono::milliseconds> timeout,
                                         const stdfs::path& root) {
    const Options& options = context_.options;
    if (items.size() <= options.batch) return assemble_batch(items, timeout, root);
    std::vector<std::span<const Source>> chunks;
    for (std::size_t b = 0; b < items.size(); b += options.batch) chunks.push_back(items.subspan(b, std::min(options.batch, items.size() - b)));
    const std::size_t threads = options.warm_pool ? std::min(options.pool_max, chunks.size()) : 1;   // serial: one boot at a time
    std::vector<Finished> parts;
    if (threads <= 1) {
        for (const auto& chunk : chunks) parts.push_back(assemble_batch(chunk, timeout, root));
    } else {
        parallel::ThreadPool pool(threads);
        parts = parallel::parallel_map(pool, chunks, [&](const std::span<const Source>& chunk) { return assemble_batch(chunk, timeout, root); });
    }
    Finished merged;
    for (Finished& part : parts)
        for (auto& result : part.results) merged.results.push_back(std::move(result));
    return merged;
}

// One dispatch: a warm session when one can be had, else a boot of its own.
Finished Assembler::assemble_batch(std::span<const Source> items, std::optional<std::chrono::milliseconds> timeout,
                                   const stdfs::path& root) {
    const Options& options = context_.options;
    if (options.warm_pool) {
        std::shared_ptr<dos_sessions::WorkerPool> pool;
        std::shared_ptr<dos_sessions::Session> session;
        try {   // a pool that cannot make a session (DOSBox will not start) is no pool: the cold boot below
            {
                std::lock_guard lock(pools_->lock);
                auto& slot = pools_->by_root[root];
                if (!slot) {
                    slot = context_.make_pool ? std::shared_ptr<dos_sessions::WorkerPool>(context_.make_pool(root))
                                              : std::shared_ptr<dos_sessions::WorkerPool>(
                                                    dos_sessions::WorkerPool::for_masm(root, context_.tools.dosbox_x, options.pool_max));
                }
                pool = slot;
            }
            session = pool->lease();
        } catch (const std::exception&) {
            session = nullptr;
        }
        if (session) {
            dos_sessions::Outcome outcome = session->assemble_many(items, timeout);
            if (auto* done = std::get_if<Finished>(&outcome)) {
                // Retire the session before it degrades (see Options::worker_max_uses): not kept, the pool boots a fresh one next lease.
                bool keep;
                {
                    std::lock_guard lock(pools_->lock);
                    const std::size_t n = pools_->uses[session.get()] += 1;
                    keep = options.worker_max_uses == 0 || n < options.worker_max_uses;
                    if (!keep) pools_->uses.erase(session.get());   // (a new session may take the address)
                }
                pool->release(session, keep);
                done->work.clear();   // (the session owns and reuses its folder)
                return std::move(*done);
            }
            {
                std::lock_guard lock(pools_->lock);
                pools_->uses.erase(session.get());
            }
            pool->release(session, false);   // dead (or cancelled): drop it, boot below
        }
    }
    return cold_boot(items, timeout, root);
}

// A DOSBox of its own for these sources: the assembler's folder is C:, a work folder is D:, MASM runs once per source.
Finished Assembler::cold_boot(std::span<const Source> items, std::optional<std::chrono::milliseconds> timeout,
                              const stdfs::path& root) {
    fs::ScopedTempDir work("masm_");
    DosRunSpec spec;
    spec.mounts = {{"c", root}, {"d", work.path()}};
    spec.work = work.path();
    for (const Source& item : items) {
        spec.files[item.stem + ".ASM"] = item.text;   // (text: written with CRLF line ends)
        spec.commands.push_back(std::format("MASM {0}.ASM, {0}.OBJ, {0}.LST, NUL; > {0}.LOG 2>&1", item.stem));
        for (const char* extension : {".OBJ", ".LST", ".LOG"}) spec.artifacts.push_back(text::upper(item.stem) + extension);
    }
    spec.timeout = std::chrono::duration_cast<std::chrono::seconds>(
        timeout.value_or(kColdBase + kColdPerSource * static_cast<long long>(items.size())));
    spec.cpu = {{"cputype", "386"}, {"core", context_.options.dosbox_core}, {"cycles", "max"}};
    spec.dos = {{"ver", "6.22"}};
    spec.variables = {{"PATH", "C:\\BIN"}, {"INCLUDE", "C:\\INC"}, {"LIB", "C:\\LIB"}, {"TMP", "D:\\"}};
    const DosRunResult ran = context_.runner.run(spec);
    Finished done;
    for (const Source& item : items) done.results.emplace_back(item.stem, product_of(ran.made, item.stem));
    return done;   // (the work folder goes with `work`: everything the run made is in the results)
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
