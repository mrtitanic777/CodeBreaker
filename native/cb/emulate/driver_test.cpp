// Tests of cb/emulate/driver: real Windows 95 VxDs (BIOS, SPLITTER, SRAMMTD) loaded into flat memory and run from every entry point with
// several inputs, compared with the recorded results of the same runs (testdata/driver.golden.json); the image's layout and relocation; the resolver; the run record; errors.
//
// Note: the seven registers the
// lanes set (eax ... ebp) are fields, and the rest cannot be named. None of the recorded runs sets another.
#include "cb/emulate/driver.hpp"

#include <format>
#include <map>

#include <nlohmann/json.hpp>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/knowledge/package.hpp"
#include "cb/symbols/driver_services.hpp"

using namespace cb;
using namespace cb::emulate;
using namespace cb::emulate::driver;
namespace sfs = std::filesystem;
using nlohmann::ordered_json;

namespace {

std::uint64_t fold_numbers(const std::vector<std::uint64_t>& values) {
    std::uint64_t h = 0xCBF29CE484222325ULL;
    for (const std::uint64_t v : values) {
        for (int k = 0; k < 8; ++k) {
            h ^= (v >> (8 * k)) & 0xFF;
            h *= 0x100000001B3ULL;
        }
    }
    return h;
}

const ordered_json& driver_golden() {
    static const ordered_json g = json::load(testing::data_path("emulate/testdata/driver.golden.json"));
    return g;
}

template <class A, class B>
void drv_same(const std::string& name, const char* what, const A& got, const B& want) {
    testing::count_check();
    if (got != static_cast<A>(want)) {
        testing::record_failure(__FILE__, __LINE__, std::format("{}: {} was {} but expected {}", name, what, got, static_cast<A>(want)));
    }
}

struct DriverWorld {
    fs::ScopedTempDir dir{"cb-driver"};
    knowledge::Store store{dir.path() / "k.sqlite", knowledge::DataFolders::under(sfs::path(CB_SOURCE_DIR).parent_path())};
    knowledge::Knowledge kb{store};
    symbols::driver_services::Table table = [] {
        const knowledge::Value doc = knowledge::Value::from_json_text(fs::read_text(testing::data_path("symbols/testdata/driver_services.json")));
        auto rows = [&](const char* kind) { return knowledge::Rows(*doc.get(kind)->map()); };
        return symbols::driver_services::Table::from_rows(rows("devices"), rows("services"), rows("drivers"), rows("messages"));
    }();
    native_trace::Governor governor;
    Environment env = [this] {
        Environment e;
        e.governor = &governor;
        e.resolver = resolver_of([this](std::uint32_t device, std::uint32_t service) { return table.service_name(device, service); },
                                 [this](std::string_view name) -> std::optional<std::string> {
                                     const std::optional<knowledge::Value> model = kb.api_model("vxd", name);
                                     if (!model) return std::nullopt;
                                     const knowledge::Value* where = model->get("answers_in");
                                     return where && where->text() ? std::optional<std::string>(*where->text()) : std::nullopt;
                                 });
        return e;
    }();
};

Bytes sequence() {
    Bytes b;
    for (int i = 0; i < 256; ++i) b.push_back(static_cast<std::uint8_t>(i));
    b.insert(b.end(), b.begin(), b.begin() + 256);
    return b;
}

std::vector<MemoryInit> inputs_memory() {
    return {{kVm, Bytes(0x100, 0)}, {kClient, Bytes(0x100, 0)}, {kBuffer, sequence()}, {kObject, Bytes(0x400, 0)}, {kSafe, Bytes(16, 0xC3)}};
}

// The inputs of each run, by name.
RunOptions options_named(const std::string& vector_name) {
    RunOptions o;
    o.memory = inputs_memory();
    if (vector_name == "zeros") {
        o.registers.ebx = kVm;
        o.registers.ebp = kClient;
    } else if (vector_name == "words") {
        o.registers = {1, kVm, 0x1234, kBuffer, kObject, kSafe, kClient};
        o.stack = {1, 2, 3};
    } else if (vector_name == "service") {
        o.registers.eax = 0;
        o.registers.ebx = kVm;
        o.registers.ebp = kClient;
        o.stack = {0, kBuffer};
        o.service_eax = 0x55;
        o.service_carry = true;
        o.flags = 1;
    } else if (vector_name == "cap") {
        o.registers.ebx = kVm;
        o.registers.ebp = kClient;
        o.cap = 50;
    }
    return o;
}

void check_run(const std::string& name, const ordered_json& want, const Record& r) {
    drv_same(name, "exit", r.exit_text(), want["exit"].get<std::string>());
    drv_same(name, "eax", r.eax, want["eax"].get<std::uint32_t>());
    drv_same(name, "flags", r.flags, want["flags"].get<std::uint32_t>());
    for (std::size_t i = 0; i < 6; ++i) drv_same(name, "result register", r.registers[i], want["registers"][i].get<std::uint32_t>());
    drv_same(name, "esp_delta", r.esp_delta, want["esp_delta"].get<std::uint32_t>());
    drv_same(name, "insns", r.observed.insns, want["insns"].get<std::int64_t>());
    drv_same(name, "outside", r.observed.outside, want["outside"].get<std::int64_t>());
    drv_same(name, "truncated", r.observed.truncated, want["truncated"].get<bool>());
    const ordered_json& calls = want["calls"];
    drv_same(name, "calls", r.observed.calls.size(), calls.size());
    for (std::size_t i = 0; i < calls.size() && i < r.observed.calls.size(); ++i) {
        const native_driver::DriverCall& c = r.observed.calls[i];
        if (calls[i][0].is_null()) drv_same(name, "call through a pointer", c.target.device.has_value(), false);
        else drv_same(name, "call device", c.target.device.value_or(0xFFFFFFFFu), calls[i][0].get<std::uint32_t>());
        drv_same(name, "call service", c.target.number, calls[i][1].get<std::uint32_t>());
        drv_same(name, "call registers", c.registers.size(), calls[i][2].size());
        for (std::size_t k = 0; k < calls[i][2].size() && k < c.registers.size(); ++k) drv_same(name, "call register", c.registers[k], calls[i][2][k].get<std::uint64_t>());
        drv_same(name, "call words", c.stack_words.size(), calls[i][3].size());
        for (std::size_t k = 0; k < calls[i][3].size() && k < c.stack_words.size(); ++k) drv_same(name, "call word", c.stack_words[k], calls[i][3][k].get<std::uint64_t>());
    }
    const ordered_json& ports = want["ports"];
    drv_same(name, "ports", r.observed.ports.size(), ports.size());
    for (std::size_t i = 0; i < ports.size() && i < r.observed.ports.size(); ++i) {
        const native_driver::PortAccess& p = r.observed.ports[i];
        drv_same(name, "port kind", p.kind == native_driver::PortAccess::Kind::In ? std::string("in") : std::string("out"), ports[i][0].get<std::string>());
        drv_same(name, "port", p.port, ports[i][1].get<std::uint64_t>());
        drv_same(name, "port size", p.size, ports[i][2].get<std::uint64_t>());
        drv_same(name, "port value", p.value, ports[i][3].get<std::uint64_t>());
    }
    std::vector<std::uint64_t> cover = r.observed.cover;
    std::sort(cover.begin(), cover.end());
    drv_same(name, "cover length", cover.size(), want["cover_len"].get<std::size_t>());
    drv_same(name, "cover checksum", fold_numbers(cover), want["cover_fold"].get<std::uint64_t>());
    std::vector<std::uint64_t> writes;
    for (const native_trace::MemWrite& w : r.observed.mem_writes) writes.insert(writes.end(), {w.addr, w.size, w.value});
    drv_same(name, "writes", r.observed.mem_writes.size(), want["writes_len"].get<std::size_t>());
    drv_same(name, "writes checksum", fold_numbers(writes), want["writes_fold"].get<std::uint64_t>());
    std::vector<std::uint64_t> reads;
    for (const native_trace::MemRead& m : r.observed.mem_reads) reads.push_back(m.addr);
    drv_same(name, "reads", r.observed.mem_reads.size(), want["reads_len"].get<std::size_t>());
    drv_same(name, "reads checksum", fold_numbers(reads), want["reads_fold"].get<std::uint64_t>());
}

sfs::path vxd_path(const std::string& name) { return testing::data_path("formats/testdata/" + name); }

}  // namespace

CB_TEST("emulate.driver.the_images_are_laid_out_and_relocated_as_recorded", [] {
    for (const auto& [file, want] : driver_golden().items()) {
        const Image image = Image::load(vxd_path(file));
        drv_same(file, "digest", image.digest, want["digest"].get<std::string>());
        drv_same(file, "objects", image.shape.size(), want["shape"].size());
        for (std::size_t i = 0; i < want["shape"].size() && i < image.shape.size(); ++i) drv_same(file, "object size", image.shape[i], want["shape"][i].get<std::uint32_t>());
        drv_same(file, "regions", image.regions.size(), want["regions"].size());
        for (std::size_t i = 0; i < want["regions"].size() && i < image.regions.size(); ++i) {
            const Region& r = image.regions[i];
            drv_same(file, "region address", r.address, want["regions"][i][0].get<std::uint32_t>());
            drv_same(file, "region length", r.bytes.size(), want["regions"][i][1].get<std::size_t>());
            drv_same(file, "region checksum", fold_numbers(std::vector<std::uint64_t>(r.bytes.begin(), r.bytes.end())), want["regions"][i][2].get<std::uint64_t>());
        }
        drv_same(file, "spans", image.spans.size(), want["spans"].size());
        for (std::size_t i = 0; i < want["spans"].size() && i < image.spans.size(); ++i) {
            drv_same(file, "span low", image.spans[i].low, want["spans"][i][0].get<std::uint32_t>());
            drv_same(file, "span high", image.spans[i].high, want["spans"][i][1].get<std::uint32_t>());
            drv_same(file, "span object", image.spans[i].object, want["spans"][i][2].get<std::uint32_t>());
        }
        drv_same(file, "entries", image.entries.size(), want["entries"].size());   // (in the order the descriptor gives them)
        std::size_t i = 0;
        for (const auto& [label, address] : image.entries) {
            if (i >= want["entries"].size()) break;
            drv_same(file, "entry label", label, want["entries"][i][0].get<std::string>());
            drv_same(file, "entry address", address, want["entries"][i][1].get<std::uint32_t>());
            ++i;
        }
        if (!image.regions.empty()) {
            CB_CHECK(image.holds(image.regions[0].address));
            CB_CHECK(image.holds(image.spans[0].high - 1));
            CB_CHECK(!image.holds(image.spans[0].low - 1));
        }
    }
});

CB_TEST("emulate.driver.every_entry_point_runs_as_recorded", [] {
    DriverWorld w;
    std::size_t ran = 0;
    for (const auto& [file, want] : driver_golden().items()) {
        const Image image = Image::load(vxd_path(file));
        for (const auto& [name, golden_run] : want["runs"].items()) {
            const std::size_t space = name.rfind(' ');
            const std::string label = name.substr(0, space), vector_name = name.substr(space + 1);
            const auto entry = image.entries.find(label);
            if (!entry) {
                testing::record_failure(__FILE__, __LINE__, "no entry " + label + " in " + file);
                continue;
            }
            w.governor = {};
            const Record r = run(w.env, image, *entry, options_named(vector_name));
            check_run(file + " " + name, golden_run, r);
            ++ran;
        }
    }
    CB_CHECK(ran >= 50);
});

CB_TEST("emulate.driver.the_resolver_names_the_register_a_service_answers_in", [] {
    std::vector<std::pair<std::uint32_t, std::uint32_t>> asked;
    const auto resolver = resolver_of(
        [&](std::uint32_t device, std::uint32_t service) -> std::optional<std::string> {
            asked.emplace_back(device, service);
            if (service == 1) return "Get_Cur_VM_Handle";
            if (service == 2) return "Nothing";
            return std::nullopt;
        },
        [](std::string_view name) -> std::optional<std::string> {
            if (name == "Get_Cur_VM_Handle") return "ebx";
            if (name == "Nothing") return "eax";
            if (name.empty()) return "esi";          // (a service with no name is asked with the empty one, as the stand-ins' lookup is)
            return "xyz";                             // a register the stand-ins do not name
        });
    CB_CHECK_EQ(resolver(1, 1).value_or(99), 1u);     // ebx
    CB_CHECK_EQ(resolver(1, 2).value_or(99), 0u);     // eax
    CB_CHECK_EQ(resolver(1, 3).value_or(99), 4u);     // no name: asked with "", which answers esi
    CB_CHECK_EQ(asked.size(), std::size_t{3});
    const auto none = resolver_of({}, {});
    CB_CHECK(!none(1, 1).has_value());                // nothing known: every service answers in EAX
});

CB_TEST("emulate.driver.the_run_record_carries_what_the_judge_compares", [] {
    DriverWorld w;
    const Image image = Image::load(vxd_path("bios.vxd"));
    RunOptions o = options_named("words");
    const Run r = recorded(w.env, image, *image.entries.find("control"), o);
    CB_CHECK_EQ(r.exit.value_or(""), "ret");
    CB_CHECK_EQ(r.ended(), "ret");
    CB_CHECK_EQ(r.results.size(), std::size_t{8});   // eax, flags, and the six result registers
    CB_CHECK(std::holds_alternative<std::uint64_t>(r.results.at("eax")));
    CB_CHECK(r.results.contains("ebp"));
    CB_CHECK(!r.capped);
    CB_CHECK_EQ(r.steps.value_or(0), std::uint64_t{19});
    CB_CHECK_EQ(r.calls.size(), std::size_t{1});
    CB_CHECK(!r.calls[0].name.has_value());
    CB_CHECK_EQ(r.calls[0].args->size(), std::size_t{10});   // six registers, four stack words
    CB_CHECK(std::holds_alternative<ServiceCall>(r.calls[0].detail));
    CB_CHECK(std::is_sorted(r.path.begin(), r.path.end()));
    CB_CHECK(!r.path.empty());
    const auto raw = std::any_cast<std::shared_ptr<const Record>>(r.raw);
    CB_CHECK_EQ(raw->observed.insns, 19);
    CB_CHECK_EQ(r.stack.value_or(-1), static_cast<std::int64_t>(raw->esp_delta));
});

CB_TEST("emulate.driver.endings", [] {
    DriverWorld w;
    const Image image = Image::load(vxd_path("bios.vxd"));
    RunOptions capped = options_named("zeros");
    capped.cap = 3;
    const Record t = run(w.env, image, *image.entries.find("control"), capped);
    CB_CHECK(t.ending == Ending::Truncated);
    CB_CHECK_EQ(t.exit_text(), "truncated");
    // an entry outside the image, where nothing is mapped, is a call that leaves it for nowhere: recorded (no device, the address it reached)
    // and returned from as a service is
    const Record f = run(w.env, image, 0x70000000, options_named("zeros"));
    CB_CHECK(f.ending == Ending::Returned);
    CB_CHECK_EQ(f.exit_text(), "ret");
    CB_CHECK_EQ(f.observed.calls.size(), std::size_t{1});
    CB_CHECK(!f.observed.calls[0].target.device.has_value());
    CB_CHECK_EQ(f.observed.calls[0].target.number, 0x70000000u);
    Record stopped;
    stopped.ending = Ending::Stopped;
    stopped.number = 0x1234;
    CB_CHECK_EQ(stopped.exit_text(), "exception: stopped at 0x1234");
    stopped.ending = Ending::Faulted;
    stopped.fault_text = "Invalid memory read (UC_ERR_READ_UNMAPPED)";
    CB_CHECK_EQ(stopped.exit_text(), "exception: Invalid memory read (UC_ERR_READ_UNMAPPED)");
});

CB_TEST("emulate.driver.errors", [] {
    CB_CHECK_THROWS_AS(UsageError, (void)run(Environment{}, Image{}, 0));
    fs::ScopedTempDir dir("cb-driver-bad");
    CB_CHECK_THROWS_AS(NotFoundError, (void)Image::load(dir.path() / "no-such.vxd"));
    const sfs::path junk = dir.path() / "junk.vxd";
    fs::write_file(junk, to_bytes(std::string(400, 'x')));
    CB_CHECK_THROWS_AS(FormatError, (void)Image::load(junk));
    // a member name that is not there: the first image
    const Image first = Image::load(vxd_path("splitter.vxd"));
    const Image named = Image::load(vxd_path("splitter.vxd"), "NO-SUCH-MEMBER");
    CB_CHECK_EQ(first.digest, named.digest);
});
