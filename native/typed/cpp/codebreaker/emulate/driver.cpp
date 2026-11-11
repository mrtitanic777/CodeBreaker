// codebreaker/emulate/driver.cpp -- typed C++ port of codebreaker.emulate.driver (see driver.hpp).

#include "codebreaker/emulate/driver.hpp"

#include <algorithm>
#include <functional>
#include <set>
#include <vector>

#include "codebreaker/decompile/describe.hpp"
#include "codebreaker/knowledge/package.hpp"
#include "codebreaker/knowledge/sources.hpp"
#include "codebreaker/emulate/native_shadow.hpp"
#include "codebreaker/emulate/unicorn_uc.hpp"
#include "codebreaker/symbols/driver_services.hpp"

namespace codebreaker::emulate::driver {

using py::str;
namespace le = codebreaker::formats::le;
namespace describe = codebreaker::decompile::describe;

namespace {

// unicorn.x86_const's UC_X86_INS_IN / UC_X86_INS_OUT (Unicorn 2.x)
constexpr int64_t UC_X86_INS_IN = 218, UC_X86_INS_OUT = 500;

}  // namespace

const py::list<str> RESULT_REGISTERS = {str("ebx"), str("ecx"), str("edx"), str("esi"), str("edi"), str("ebp")};
const py::list<str> REGISTERS = {str("eax"), str("ebx"), str("ecx"), str("edx"), str("esi"), str("edi"), str("ebp")};

std::optional<str> answers_in(const std::optional<str>& service) {
    // (knowledge.api_model("vxd", service or "") or {}).get("answers_in")
    const auto row = knowledge::api_model(str("vxd"), (service && !service->empty()) ? *service : str(""));
    if (!row || !row->contains(str("answers_in"))) return std::nullopt;
    const auto& v = row->at(str("answers_in"));
    if (v.kind == knowledge::sources::Json::Str) return v.s;
    return std::nullopt;
}

Image::Image(const le::LEFile& lf) {
    std::map<int64_t, py::bytearray> bufs;   // {object index: its bytes}, in index order
    for (const le::Object& o : lf.objects) bufs[o.idx] = py::bytearray(le::object_data(lf, o));
    for (const auto& ref : le::references(lf)) {
        const auto [obj, off, kind, to, ta] = ref;
        const auto it = bufs.find(obj);
        if (it == bufs.end() || off < 0 || off + 4 > it->second.size() || (kind != 7 && kind != 8)) continue;
        int64_t target = describe::le_address(to, ta);
        if (kind == 8) target -= describe::le_address(obj, off) + 4;   // the distance to it, as in a call
        py::pack_into("<I", it->second, off, target & 0xFFFFFFFF);
    }
    std::vector<int64_t> idx_with_data;
    for (const auto& kv : bufs) {
        if (kv.second.size() == 0) continue;
        regions.append(std::make_tuple(describe::le_address(kv.first, 0), py::bytes(kv.second.view())));
        idx_with_data.push_back(kv.first);
    }
    for (size_t k = 0; k < idx_with_data.size(); k++) {
        const auto& r = regions[static_cast<int64_t>(k)];
        spans.append(std::make_tuple(std::get<0>(r), std::get<0>(r) + std::get<1>(r).size(), idx_with_data[k]));
    }
    for (const auto& kv : le::entry_points(lf).items()) {
        const le::Place& where = std::get<1>(kv);
        entries.set(std::get<0>(kv), describe::le_address(std::get<0>(where), std::get<1>(where)));
    }
    for (const le::Object& o : lf.objects) shape.append(o.vsize);
    py::bytes all;
    for (const auto& r : regions) all = all + std::get<1>(r);
    digest = py::hashlib::sha1(all).hexdigest();
}

bool Image::holds(int64_t address) const {
    for (const auto& s : spans) {
        if (std::get<0>(s) <= address && address < std::get<1>(s)) return true;
    }
    return false;
}

Image load(const str& path, const std::optional<str>& member) {
    const auto found = le::members(path);
    if (found.empty()) throw py::IndexError("list index out of range");
    for (const auto& nx : found) {
        if (member && !member->empty() && std::get<0>(nx).lower() == member->lower()) return Image(std::get<1>(nx));
    }
    return Image(std::get<1>(found[0]));
}

Record run(const Image& image, int64_t entry, const std::optional<py::dict<str, int64_t>>& registers, const py::list<int64_t>& stack,
           const py::list<std::tuple<int64_t, py::bytes>>& memory, int64_t service_eax, int64_t service_carry, int64_t cap, int64_t flags,
           native_shadow::Shadow* shadow) {
    namespace uni = emulate::unicorn_uc;
    auto reg_id = [](const char* name) -> int64_t {
        const std::optional<int64_t> id = uni::x86_reg(str(name).upper());
        if (!id) throw py::KeyError(py::repr(str(name)).utf8());
        return *id;
    };
    std::map<std::string, int64_t> reg;
    for (const str& n : REGISTERS) reg[n.utf8()] = reg_id(n.utf8().c_str());
    for (const char* n : {"esp", "eip", "eflags"}) reg[n] = reg_id(n);
    const int64_t page = 0x1000;
    std::shared_ptr<core16::Uc> uc = uni::open_x86(32);
    std::set<int64_t> mapped;

    auto provide = [&](int64_t address, int64_t size) {
        std::vector<int64_t> want;
        for (int64_t b = address & ~(page - 1); b < address + size; b += page) {
            if (!mapped.count(b)) want.push_back(b);
        }
        size_t at = 0;
        while (at < want.size()) {   // each run of new pages in one piece
            size_t n = 1;
            while (at + n < want.size() && want[at + n] == want[at] + static_cast<int64_t>(n) * page) n++;
            uc->mem_map(want[at], static_cast<int64_t>(n) * page);
            for (size_t k = 0; k < n; k++) mapped.insert(want[at + k]);
            at += n;
        }
    };

    for (const auto& region : image.regions) {
        provide(std::get<0>(region), std::get<1>(region).size());
        uc->mem_write(std::get<0>(region), std::get<1>(region));
    }
    provide(SCRATCH, emulator::_SCRATCH_SIZE);
    provide(STACK_LO, STACK_HI - STACK_LO);
    for (const auto& m : memory) {
        provide(std::get<0>(m), std::get<1>(m).size());
        uc->mem_write(std::get<0>(m), std::get<1>(m));
    }
    int64_t esp = emulator::_STACK_TOP;
    for (int64_t i = stack.size() - 1; i >= 0; i--) {
        esp -= 4;
        uc->mem_write(esp, py::pack("<I", stack[i] & 0xFFFFFFFF));
    }
    esp -= 4;
    uc->mem_write(esp, py::pack("<I", RETURN));
    uc->reg_write(reg.at("esp"), esp);
    if (registers) {
        for (const auto& kv : registers->items()) {
            const auto it = reg.find(std::get<0>(kv).utf8());
            if (it == reg.end()) throw py::KeyError(py::repr(std::get<0>(kv)).utf8());
            uc->reg_write(it->second, std::get<1>(kv) & 0xFFFFFFFF);
        }
    }
    uc->reg_write(reg.at("eflags"), (uc->reg_read(reg.at("eflags")) & ~int64_t(0x41)) | (flags & 0x41));
    Record rec;

    // The hooks are C++ (native/tracer: cbtrace.dll; emulate/native_driver), attached in this
    // order: the code (the driver's own, told from a call out), memory, the INT 20h service calls,
    // the ports; and a shadow (the path explorer's) stepped there too. The caller is asked once per
    // distinct service which register it answers in.
    native_driver::require();
    native_driver::RunArgs ra;
    for (const char* n : {"eax", "ebx", "ecx", "edx", "esi", "edi", "ebp", "esp", "eip", "eflags"}) ra.reg_ids.append(reg.at(n));
    ra.ins_in = UC_X86_INS_IN;
    ra.ins_out = UC_X86_INS_OUT;
    ra.vm_addr = VM;
    ra.ret = RETURN;
    ra.scratch = std::make_tuple(SCRATCH, SCRATCH + emulator::_SCRATCH_SIZE);
    for (const auto& s : image.spans) ra.spans.append(std::make_tuple(std::get<0>(s), std::get<1>(s)));
    ra.cap = cap;
    ra.service_eax = service_eax;
    ra.service_carry = service_carry;
    ra.gov = &emulator::_GOV;
    ra.answers = [](int32_t device, int32_t service) -> int64_t {
        const std::optional<str> where = answers_in(symbols::driver_services::service_name(device, service));
        if (!where) return -1;
        for (int64_t i = 0; i < REGISTERS.size(); i++) {
            if (REGISTERS[i] == *where) return i;
        }
        return -1;
    };
    ra.shadow = native_shadow::handle_of(shadow);
    native_trace::Engine engine;
    engine.uch = uc->handle();
    engine.emu_start = [&](int64_t begin, int64_t until, int64_t count) { uc->emu_start(begin, until, 0, count); };
    native_driver::Run nr(engine, ra);
    std::optional<native_trace::UcError> uc_error;
    try {
        // No clock: a run ends by returning, faulting, or at a count (instructions, memory
        // accesses, pages) -- so the same run always ends the same way and can be remembered.
        uc->emu_start(entry, RETURN, 0, cap + 1);
    } catch (const native_trace::UcError& e) {
        uc_error = e;
    }
    if (const std::exception_ptr exc = nr.raised()) std::rethrow_exception(exc);   // a resolver that raised comes out here
    native_driver::DriverRecord got;
    nr.collect(got);
    if (got.insns) rec.insns = *got.insns;
    if (got.truncated) rec.truncated = *got.truncated;
    if (got.governed) rec.governed = got.governed;
    if (got.outside) rec.outside = *got.outside;
    if (got.exit) rec.exit = got.exit;
    if (got.cover) rec.cover = *got.cover;
    if (got.calls) rec.calls = *got.calls;
    if (got.ports) rec.ports = *got.ports;
    if (got.mem_reads) rec.mem_reads = *got.mem_reads;
    if (got.mem_writes) rec.mem_writes = *got.mem_writes;
    if (got.lazy_maps) rec.lazy_maps = got.lazy_maps;
    if (got.memops) rec.memops = got.memops;
    if (got.writes_seen) rec.writes_seen = got.writes_seen;
    if (got._stuck) rec._stuck = got._stuck;
    if (!uc_error) {
        const int64_t at = uc->reg_read(reg.at("eip"));
        if (!rec.exit) {
            rec.exit = rec.truncated ? str("truncated") : at == RETURN ? str("ret") : py::percent(str("exception: stopped at 0x%x"), at);
        }
    } else if (!rec.exit) {
        rec.exit = str("exception: ") + uni::strerror(uc_error->errno_);
    }
    rec.eax = uc->reg_read(reg.at("eax"));
    rec.flags = uc->reg_read(reg.at("eflags")) & 0x41;   // carry and zero
    for (const str& n : RESULT_REGISTERS) rec.registers.append(uc->reg_read(reg.at(n.utf8())));
    rec.esp_delta = (uc->reg_read(reg.at("esp")) - esp) & 0xFFFFFFFF;
    return rec;
}

run::Run record(const Record& rec) {
    py::list<run::Write> writes;
    for (const auto& w : rec.mem_writes) {
        run::Write x;
        x.address = static_cast<int64_t>(w.addr);
        x.size = static_cast<int64_t>(w.size);
        x.value = static_cast<int64_t>(w.value);
        x.old = static_cast<int64_t>(w.old);
        x.harness = false;
        writes.append(x);
    }
    py::dict<str, run::Register> results;
    results.set(str("eax"), rec.eax ? run::Register(*rec.eax) : run::Register(py::none));
    results.set(str("flags"), run::Register(rec.flags));
    for (int64_t i = 0; i < RESULT_REGISTERS.size() && i < rec.registers.size(); i++) results.set(RESULT_REGISTERS[i], run::Register(rec.registers[i]));
    py::list<run::Call> calls;
    for (const auto& c : rec.calls) {
        const auto& [device, service, regs, words] = c;
        run::Call call;
        py::list<int64_t> args;
        for (const uint64_t r : regs) args.append(static_cast<int64_t>(r));
        for (const uint64_t w : words) args.append(static_cast<int64_t>(w));
        call.args = args;
        // (device, service): the device as its decimal text (run::Detail has no integer device), None through a pointer
        call.detail = std::make_tuple(device ? std::optional<str>(py::to_str(static_cast<int64_t>(*device))) : std::nullopt,
                                      run::Word(static_cast<int64_t>(service)));
        calls.append(call);
    }
    py::list<run::IO> io;
    for (const auto& p : rec.ports) {
        const auto& [kind, port, size, value] = p;
        run::IO x;
        x.kind = kind;
        x.port = static_cast<int64_t>(port);
        x.value = static_cast<int64_t>(value);
        x.size = static_cast<int64_t>(size);
        io.append(x);
    }
    std::vector<int64_t> cover;
    for (const uint64_t a : rec.cover) cover.push_back(static_cast<int64_t>(a));
    std::sort(cover.begin(), cover.end());   // (the Python holds the set; a list is in address order)
    py::list<int64_t> path;
    for (const int64_t a : cover) path.append(a);
    return run::Run(rec.exit, results, writes, rec.mem_writes.size() >= emulator::_MAX_MEM, calls, io, rec.esp_delta, std::nullopt, path, rec.insns,
                    rec.truncated, str("little"), std::make_shared<Record>(rec));
}

run::Run recorded(const Image& image, int64_t entry, const std::optional<py::dict<str, int64_t>>& registers, const py::list<int64_t>& stack,
                  const py::list<std::tuple<int64_t, py::bytes>>& memory, int64_t service_eax, int64_t service_carry, int64_t cap, int64_t flags,
                  native_shadow::Shadow* shadow) {
    return record(run(image, entry, registers, stack, memory, service_eax, service_carry, cap, flags, shadow));
}

}  // namespace codebreaker::emulate::driver
