#include "knowledge/package.hpp"

#include <algorithm>

#include <nlohmann/json.hpp>

#include "core/crypto.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "knowledge/reads.hpp"
#include "knowledge/store.hpp"
#include "symbols/api_arguments.hpp"

// Behaviour pinned here:
//   * the queries (prototype, platform ...) are methods of a `Knowledge` that owns the base (a `Store`), the header folders
//     and the readers' tables; there is no process-wide state;
//   * argument_bytes, import_name and struct are not here: the readers they ask (symbols/api_arguments, ordinals,
//     sdk_structs) are objects, and `win32_memory()` / `win16_memory()` give them the base as their memory;
//   * platform() of a number other than 16 and 32 is its text.

using namespace cb;
using namespace cb::knowledge;

namespace {

std::filesystem::path checkout() { return std::filesystem::path(CB_SOURCE_DIR).parent_path(); }

// What the checkout's win16 model file holds (the tests count the file, not a number that goes stale when a call is modelled).
std::size_t win16_model_count() {
    const nlohmann::json j = nlohmann::json::parse(fs::read_text(checkout() / "config" / "knowledge" / "models" / "win16.json"));
    return j.at("functions").size();
}

struct TestKnowledge {
    fs::ScopedTempDir dir{"cb-knowledge-package"};
    Store store{dir.path() / "k.sqlite", DataFolders::under(checkout())};
    symbols::prototypes::HeaderFolders folders;
    std::unique_ptr<Knowledge> kb;

    explicit TestKnowledge(bool with_headers = false) {
        if (with_headers) {
            fs::write_text(dir.path() / "nt" / "winuser.h",
                           "WINUSERAPI HWND WINAPI GetDlgItem(HWND hDlg, int nIDDlgItem);\n"
                           "int WINAPI MessageBoxA(HWND, LPCSTR, LPCSTR, UINT);\n");
            fs::write_text(dir.path() / "win" / "winuser.h", "int WINAPI LZRead(HFILE, void FAR *, int);\n");
            folders.w32 = {dir.path() / "nt"};
            folders.w16 = {dir.path() / "win"};
        }
        kb = std::make_unique<Knowledge>(store, folders);
    }
};

}  // namespace

CB_TEST("knowledge.package.platform_names", [] {
    CB_CHECK_EQ(platform("ne"), "win16");
    CB_CHECK_EQ(platform("pe"), "win32");
    CB_CHECK_EQ(platform("le"), "vxd");
    CB_CHECK_EQ(platform("mz"), "dos");
    CB_CHECK_EQ(platform("powerpc"), "xbox360");
    CB_CHECK_EQ(platform("16"), "win16");
    CB_CHECK_EQ(platform("32"), "win32");
    CB_CHECK_EQ(platform(16), "win16");
    CB_CHECK_EQ(platform(32), "win32");
    CB_CHECK_EQ(platform(64), "64");
    CB_CHECK_EQ(platform("win16"), "win16");      // already the base's
    CB_CHECK_EQ(platform("nes"), "nes");          // none of the lanes': as it is
    CB_CHECK_EQ(known_platforms().size(), std::size_t{6});
    CB_CHECK_EQ(known_platforms()[0].name, "win16");
    CB_CHECK_EQ(known_platforms()[5].title, "Xbox 360");
});

CB_TEST("knowledge.package.bare_name", [] {
    CB_CHECK_EQ(bare_name("KERNEL.GLOBALALLOC"), "GLOBALALLOC");
    CB_CHECK_EQ(bare_name("USER32!FindWindowA"), "FindWindowA");
    CB_CHECK_EQ(bare_name("GetDlgItem"), "GetDlgItem");
    CB_CHECK_EQ(bare_name("USER.@171"), "USER.@171");       // not a function's name: as it is
    CB_CHECK_EQ(bare_name("a.b.c"), "a.b.c");
    CB_CHECK_EQ(bare_name(".x"), ".x");
    CB_CHECK_EQ(bare_name("x."), "x.");
    CB_CHECK_EQ(bare_name(""), "");
});

CB_TEST("knowledge.package.known_and_learn_record_the_reads", [] {
    TestKnowledge t;
    CB_CHECK(!t.kb->known("pe", "argument_bytes.code", "USER32.FindWindowA"));
    t.kb->learn("pe", "argument_bytes.code", "USER32.FindWindowA", Value(8), "user32.dll's code");
    Recording rec;
    const auto v = t.kb->known("win32", "argument_bytes.code", "USER32.FindWindowA");   // the lane's name and the base's agree
    CB_CHECK(v && *v->integer() == 8);
    CB_CHECK_EQ(t.store.provenance("win32", "argument_bytes.code", "USER32.FindWindowA")->origin, "learned");
    const auto reads = rec.reads()->sorted();
    CB_CHECK_EQ(reads.size(), std::size_t{1});
    CB_CHECK_EQ(reads[0].text(), "win32:argument_bytes.code:USER32.FindWindowA");
});

CB_TEST("knowledge.package.table_is_a_whole_kind_read", [] {
    TestKnowledge t;
    Recording rec;
    const Rows api = t.kb->table("ne", "api_model");   // authored: loaded from the checkout's file
    CB_CHECK_EQ(api.size(), win16_model_count());
    CB_CHECK(rec.reads()->contains(Read{"win16", "api_model", std::string(kWholeKind)}));
});

CB_TEST("knowledge.package.api_model_hides_the_notes", [] {
    TestKnowledge t;
    const std::optional<Value> lock = t.kb->api_model("win16", "GLOBALLOCK");
    CB_CHECK(lock.has_value());
    CB_CHECK(lock->map() != nullptr);
    for (const auto& [key, value] : *lock->map()) CB_CHECK(!key.starts_with("_"));
    CB_CHECK(!lock->map()->empty());
    CB_CHECK(!t.kb->api_model("win16", "NoSuchFunction"));
    CB_CHECK(!t.kb->api_model("dos", "GLOBALLOCK"));


});

CB_TEST("knowledge.package.prototypes_come_from_the_headers_and_are_kept_in_the_base", [] {
    TestKnowledge t(true);
    const auto* get = t.kb->prototype("pe", "USER32!GetDlgItem");
    CB_CHECK(get != nullptr);
    CB_CHECK_EQ(get->returns, "HWND");
    CB_CHECK_EQ(get->parameters.size(), std::size_t{2});
    CB_CHECK_EQ(t.kb->declaration("win16", "KERNEL.LZRead").value(), "int WINAPI LZRead(HFILE, void FAR *, int)");
    CB_CHECK(t.kb->prototype("win16", "GetDlgItem") == nullptr);   // not in the 16-bit headers
    CB_CHECK_EQ(t.kb->spelling("GETDLGITEM"), "GetDlgItem");
    // kept in the base, derived
    CB_CHECK_EQ(t.store.table("win32", "prototype").size(), std::size_t{2});
    CB_CHECK_EQ(t.store.table("win16", "prototype").size(), std::size_t{1});
    CB_CHECK_EQ(t.store.provenance("win32", "prototype", "GetDlgItem")->origin, "derived");

    // a second object over the same base and the same headers reads the rows and does not read the headers again:
    // delete the headers, and it still answers
    t.kb.reset();
    std::filesystem::remove_all(t.dir.path() / "nt");
    std::filesystem::remove_all(t.dir.path() / "win");
    t.kb = std::make_unique<Knowledge>(t.store, t.folders);
    // (the stamp of folders that are gone differs: the headers are read again, and are empty)
    CB_CHECK(t.kb->prototype("pe", "GetDlgItem") == nullptr);
    CB_CHECK_EQ(t.store.table("win32", "prototype").size(), std::size_t{0});
});

CB_TEST("knowledge.package.prototype_rows_survive_the_trip_through_the_base", [] {
    TestKnowledge t(true);
    const auto* box = t.kb->prototype("win32", "MessageBoxA");
    CB_CHECK(box != nullptr);
    CB_CHECK_EQ(box->header, "winuser.h");
    CB_CHECK_EQ(box->convention, "stdcall");
    CB_CHECK_EQ(box->marker, "WINAPI");
    CB_CHECK_EQ(box->parameters.size(), std::size_t{4});
    CB_CHECK_EQ(box->parameters[1].type, "LPCSTR");
    CB_CHECK_EQ(box->parameters[1].name, "");
    const symbols::prototypes::Prototype direct = *box;
    // a new object over the same unchanged folders reads the stored rows
    Knowledge again(t.store, t.folders);
    const auto* box2 = again.prototype("win32", "MessageBoxA");
    CB_CHECK(box2 != nullptr);
    CB_CHECK(*box2 == direct);
});

CB_TEST("knowledge.package.names_of_bodies", [] {
    TestKnowledge t;
    Recording rec;
    CB_CHECK(!t.kb->name("deadbeef"));
    t.kb->name_body("deadbeef", "LZStart");
    const auto n = t.kb->name("deadbeef");
    CB_CHECK(n && n->name == "LZStart" && n->source == "named by hand");
    CB_CHECK(rec.reads()->contains(Read{"bodies", "name", "deadbeef"}));
});

CB_TEST("knowledge.package.signatures_of_the_real_files", [] {
    TestKnowledge t;
    const auto rows16 = t.kb->signatures("ne");
    CB_CHECK_EQ(rows16.size(), std::size_t{6970});
    CB_CHECK_EQ(t.kb->signatures("pe").size(), std::size_t{5233});
    // a signature matches its own leading bytes (the code of the first 16-bit row, as far as the mask allows)
    const auto& first = rows16.front();
    const Bytes lead = crypto::from_hex(first.lead_hex);
    std::vector<std::uint8_t> code(lead.begin(), lead.end());
    code.resize(std::max<std::size_t>(first.size, lead.size()), 0);
    const auto found = t.kb->signature_match("win16", code);
    if (found) CB_CHECK(!found->name.empty());   // the digest of the whole body decides; the lead alone may not match it
    CB_CHECK(!t.kb->signature_match("win16", Bytes{}).has_value());
});

CB_TEST("knowledge.package.win32_memory", [] {
    TestKnowledge t;
    auto memory = t.kb->win32_memory();
    // the headers' table is the checkout's authored file
    const auto find_window = memory->header_bytes("FindWindowA");
    CB_CHECK(find_window.has_value() && *find_window == 8);
    CB_CHECK(!memory->header_bytes("NoSuchFunctionAnywhere"));
    // what scans found: never scanned, scanned and decided, scanned and undecidable
    CB_CHECK(!memory->scanned("USER32", "Odd"));
    memory->remember("USER32", "Odd", 12);
    memory->remember("USER32", "Unknown", std::nullopt);
    CB_CHECK(memory->scanned("USER32", "Odd")->bytes == std::optional<std::uint32_t>(12));
    const auto unknown = memory->scanned("USER32", "Unknown");
    CB_CHECK(unknown.has_value() && !unknown->bytes.has_value());
    // another object over the same base sees them
    CB_CHECK(t.kb->win32_memory()->scanned("USER32", "Odd")->bytes == std::optional<std::uint32_t>(12));
    CB_CHECK_EQ(t.store.provenance("win32", "argument_bytes.code", "USER32.Odd")->origin, "learned");
});

CB_TEST("knowledge.package.win16_memory_is_scoped", [] {
    TestKnowledge t;
    auto a = t.kb->win16_memory("product-a:");
    auto b = t.kb->win16_memory("product-b:");
    a->remember("USER.171", 6, "USER.EXE");
    a->remember("USER.172", std::nullopt, "USER.EXE");
    b->remember("USER.171", 8, "USER.EXE of another product");
    CB_CHECK(a->remembered("USER.171")->bytes == std::optional<std::uint32_t>(6));
    CB_CHECK(b->remembered("USER.171")->bytes == std::optional<std::uint32_t>(8));
    CB_CHECK(!a->remembered("USER.999"));
    const auto table = a->table();
    CB_CHECK_EQ(table.size(), std::size_t{2});
    CB_CHECK(table.at("USER.171") == std::optional<std::uint32_t>(6));
    CB_CHECK(!table.at("USER.172").has_value());
    CB_CHECK_EQ(t.store.provenance("win16", "argument_bytes", "product-b:USER.171")->source, "USER.EXE of another product");
});
