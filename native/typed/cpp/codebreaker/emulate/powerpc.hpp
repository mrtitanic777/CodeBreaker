// codebreaker/emulate/powerpc.hpp -- SHIM over cb/emulate/powerpc.hpp (docs/CPP_DESIGN.md 6.3): the old surface (regions and
// pages as py:: values, the run's keywords and record, a Machine whose registers are py::lists, Shadow) for the ports that
// still call it (verify/lanes/powerpc, emulate/shadow). Conversion and forwarding only; deleted with its last py:: consumer.
//
// The old Machine here is a view of the new one: its registers are copies made when it is handed over (to a Shadow before
// each instruction, to the caller after the run), `pages.get(n)` / `shared.get(n)` copy the page they find, and `read` /
// `write` / `load` forward. What the old header also had and this one does not (no consumer used them): the Python's
// internals (`_step`, `_s64`, `_mask`, the opcode tables, `_Branches`), Machine's page dictionaries as mutable members, and
// the Python's `Stop` raised for an entry that cannot be placed (Machine construction past 2048 pages is a Stop still).
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>
#include <variant>

#include "cb/compat/py_bridge.hpp"
#include "cb/emulate/powerpc.hpp"
#include "codebreaker/emulate/run.hpp"

namespace codebreaker::emulate::powerpc {

namespace emu_run = ::codebreaker::emulate::run;   // (this namespace has a function run(), which hides the name)
namespace np = ::cb::emulate::powerpc;

inline constexpr uint64_t M64 = 0xFFFFFFFFFFFFFFFFULL;
inline constexpr int64_t M32 = 0xFFFFFFFF;
inline constexpr int64_t RETURN = 0x7FFF0000;          // where the function returns to
inline constexpr int64_t STACK_TOP = 0x70100000;       // stack: the megabyte below this
inline constexpr int64_t STACK_LOW = 0x70000000;
inline constexpr int64_t ARGS_BASE = 0x60000000;       // memory pointer-shaped inputs can point into
inline constexpr int64_t PAGE = 0x1000;
inline constexpr int64_t MAX_PAGES = 2048;
inline constexpr int64_t MAX_STEPS = 20000;
inline constexpr int64_t MAX_CALLS = 400;

class Stop : public py::Exception {
  public:
    Stop() : py::Exception() {}
    explicit Stop(const py::str& why) : py::Exception(why) {}
    explicit Stop(const char* why) : py::Exception(why) {}
    const char* type() const override { return "Stop"; }
};

using Region = std::tuple<int64_t, py::bytes>;         // (address, bytes)

namespace shim {

inline std::vector<np::Region> regions_of(const py::list<Region>& regions) {
    std::vector<np::Region> out;
    for (const Region& r : regions) {
        np::Region region;
        region.address = static_cast<uint32_t>(std::get<0>(r) & 0xFFFFFFFFLL);
        region.data = cb::compat::from_py(std::get<1>(r));
        out.push_back(std::move(region));
    }
    return out;
}

// Python's errors for what the new code throws.
template <class F>
decltype(auto) guard(F&& f) {
    try {
        return std::forward<F>(f)();
    } catch (const np::Fault& e) {
        throw Stop(py::str(std::string(np::to_text(e.reason()))));
    } catch (const cb::Error& e) {
        cb::compat::rethrow_as_py(e);
    }
}

inline py::bytes page_bytes(const np::Page& p) { return py::bytes(std::string(reinterpret_cast<const char*>(p.data()), p.size())); }

}  // namespace shim

// {page number: bytes} for [(address, bytes)] -- an image prepared once and shared by every run of it (a run copies a page
// only when it writes to it). The pages are the new PageImage; `get` copies one out.
class Pages {
  public:
    Pages() = default;
    explicit Pages(np::PageImage image) : image_(std::move(image)) {}
    std::optional<py::bytes> get(int64_t page_number) const {
        if (page_number < 0 || page_number > 0xFFFFF) return std::nullopt;
        const np::Page* p = image_.find(static_cast<uint32_t>(page_number));
        return p ? std::optional<py::bytes>(shim::page_bytes(*p)) : std::nullopt;
    }
    bool contains(int64_t page_number) const { return get(page_number).has_value(); }
    int64_t size() const { return static_cast<int64_t>(image_.size()); }
    const np::PageImage& image() const { return image_; }

  private:
    np::PageImage image_;
};

inline Pages pages(const py::list<Region>& regions) {
    return shim::guard([&] {
        const auto rs = shim::regions_of(regions);
        return Pages(np::PageImage::of(rs));
    });
}

class Machine;

// The pages a run has written (`pages`) and the shared ones it only reads (`shared`), as the old dictionaries answered `get`.
class WrittenPages {
  public:
    explicit WrittenPages(const Machine* owner) : owner_(owner) {}
    std::optional<py::bytearray> get(int64_t page_number) const;
  private:
    const Machine* owner_;
};
class SharedPages {
  public:
    explicit SharedPages(const Machine* owner) : owner_(owner) {}
    std::optional<py::bytes> get(int64_t page_number) const;
  private:
    const Machine* owner_;
};

// A view of a run's machine: the registers as they were when it was handed over, memory through the new Machine.
class Machine {
  public:
    py::list<uint64_t> r;
    py::list<uint64_t> f;                              // floating registers, as 64 raw bits
    int64_t lr = 0;
    uint64_t ctr = 0;
    int64_t cr = 0;
    int64_t ca = 0;
    int64_t pc = 0;
    std::optional<int64_t> reserve;
    WrittenPages pages{this};                          // pages this run has written or created
    SharedPages shared{this};                          // pages it only reads so far

    explicit Machine(std::shared_ptr<np::Machine> impl) : impl_(std::move(impl)) { sync(); }
    Machine(const Machine&) = delete;
    Machine& operator=(const Machine&) = delete;

    uint64_t read(int64_t a, int64_t n) { return shim::guard([&] { return impl_->read(static_cast<uint32_t>(a & 0xFFFFFFFFLL), static_cast<unsigned>(n)); }); }
    void write(int64_t a, int64_t n, uint64_t v) {
        shim::guard([&] { impl_->write(static_cast<uint32_t>(a & 0xFFFFFFFFLL), static_cast<unsigned>(n), v); });
    }
    void load(int64_t va, const py::bytes& data) {
        shim::guard([&] { impl_->load(static_cast<uint32_t>(va & 0xFFFFFFFFLL), cb::compat::from_py(data)); });
    }

    np::Machine& impl() { return *impl_; }
    // The registers of the new machine, copied in.
    void sync() {
        r.vec().assign(impl_->r.begin(), impl_->r.end());
        f.vec().assign(impl_->f.begin(), impl_->f.end());
        lr = impl_->lr;
        ctr = impl_->ctr;
        cr = impl_->cr;
        ca = impl_->ca ? 1 : 0;
        pc = impl_->pc;
        reserve = impl_->reserve ? std::optional<int64_t>(*impl_->reserve) : std::nullopt;
    }

  private:
    std::shared_ptr<np::Machine> impl_;
};

inline std::optional<py::bytearray> WrittenPages::get(int64_t page_number) const {
    if (page_number < 0 || page_number > 0xFFFFF) return std::nullopt;
    const np::Page* p = const_cast<Machine*>(owner_)->impl().find_written(static_cast<uint32_t>(page_number));
    if (!p) return std::nullopt;
    return py::bytearray(shim::page_bytes(*p));
}
inline std::optional<py::bytes> SharedPages::get(int64_t page_number) const {
    if (page_number < 0 || page_number > 0xFFFFF) return std::nullopt;
    const np::Page* p = const_cast<Machine*>(owner_)->impl().find_unwritten(static_cast<uint32_t>(page_number));
    if (!p) return std::nullopt;
    return shim::page_bytes(*p);
}

// What the last compare into a condition field compared: ("cmp", size, a, b, where a was loaded from or None, where b was)
struct Compare {
    int64_t size = 0;
    uint64_t a = 0;
    uint64_t b = 0;
    std::optional<int64_t> a_from;
    std::optional<int64_t> b_from;
};
// (address, taken, compare)
using BranchEvent = std::tuple<int64_t, bool, std::optional<Compare>>;

// emulate/shadow.py's ShadowPPC, kept up beside the run (the path explorer's): stepped before each instruction, told of
// each call out.
class Shadow {
  public:
    virtual ~Shadow() = default;
    virtual void step(Machine& m, int64_t pc) = 0;
    virtual void after_call(Machine& m, bool known, uint64_t ret) = 0;
};

// run's keywords after `args`
struct RunOptions {
    // {address: name}: functions outside what was given: a call to one is recorded with its eight argument registers and
    // returns `stub_ret`
    std::optional<py::dict<int64_t, py::str>> externs;
    std::optional<std::tuple<int64_t, int64_t>> span;   // (lo, hi): the addresses whose execution is recorded
    int64_t stub_ret = 0;
    int64_t steps = MAX_STEPS;
    py::list<double> fargs;                             // floats, in f1..
    std::optional<Pages> shared;                        // memory prepared with pages()
    std::optional<py::str> fill;                        // what the memory behind pointer-shaped inputs holds
    bool branches = false;                              // log each conditional branch inside `span`
    // (low, high): where code the image calls may be missing -- a call that lands on nothing there is a call out, recorded
    // as "@<address>"
    std::optional<std::tuple<int64_t, int64_t>> absent;
    Shadow* shadow = nullptr;                           // None
};

// run's record: {exit, truncated, calls, ret, fret, steps, path, pc, writes, saved, machine[, branches]}
struct Out {
    py::str exit = py::str("none");
    bool truncated = false;
    py::list<std::tuple<py::str, py::list<uint64_t>>> calls;
    uint64_t ret = 0;
    uint64_t fret = 0;
    int64_t steps = 0;
    py::set<int64_t> path;
    int64_t pc = 0;
    py::dict<int64_t, std::tuple<int64_t, int64_t>> writes;
    bool saved = false;
    py::ref<Machine> machine;
    std::optional<py::list<BranchEvent>> branches;      // present when run(branches=True)
    std::shared_ptr<np::RunResult> result;              // the new record this one was made from
};

// Contents for the memory pointer-shaped inputs point into (ARGS_BASE upward): "pointers", "counts" or "text" (anything
// else: nothing).
inline py::bytes filled(const py::str& kind, int64_t size = 0x10000) {
    const auto which = np::fill_from_text(kind.utf8());
    if (!which || size <= 0) return py::bytes();
    return cb::compat::to_py(cb::ByteView(np::filled(*which, static_cast<size_t>(size))));
}

namespace shim {

// The new Shadow interface in front of an old one: the old Machine view is brought up to date before each call.
class ShadowAdapter : public np::Shadow {
  public:
    explicit ShadowAdapter(::codebreaker::emulate::powerpc::Shadow* old) : old_(old) {}
    void step(np::Machine& m, uint32_t pc) override { old_->step(view(m), pc); }
    void after_call(np::Machine& m, bool known, uint64_t ret) override { old_->after_call(view(m), known, ret); }

  private:
    ::codebreaker::emulate::powerpc::Machine& view(np::Machine& m) {
        if (!view_) {
            view_ = std::make_unique<::codebreaker::emulate::powerpc::Machine>(std::shared_ptr<np::Machine>(&m, [](np::Machine*) {}));   // (the run owns it)
        } else {
            view_->sync();
        }
        return *view_;
    }
    ::codebreaker::emulate::powerpc::Shadow* old_;
    std::unique_ptr<::codebreaker::emulate::powerpc::Machine> view_;
};

}  // namespace shim

// Run the function at `entry`. `regions` is [(address, bytes)] to place in memory; `args` go in r3..r10 and `fargs` (floats)
// in f1..; `externs` {address: name} are functions outside what was given: a call to one is recorded with its eight
// argument registers and returns `stub_ret`. `shared` is memory prepared with pages(), used instead of (or under) `regions`;
// `fill` names what the memory behind pointer-shaped inputs holds (see filled()). With `branches`, each conditional branch
// inside `span` is logged in out.branches. `absent` (low, high): where code the image calls may be missing. `shadow`: the
// path explorer's, stepped before each instruction.
inline Out run(const py::list<Region>& regions, int64_t entry, const py::list<uint64_t>& args = py::list<uint64_t>(),
               const RunOptions& kw = RunOptions()) {
    return shim::guard([&] {
        const auto rs = shim::regions_of(regions);
        np::RunOptions o;
        np::ExternTable externs;
        if (kw.externs) {
            for (const auto& item : kw.externs->items()) {
                externs.emplace(static_cast<uint32_t>(std::get<0>(item) & 0xFFFFFFFFLL), std::get<1>(item).utf8());
            }
            o.externs = &externs;
        }
        if (kw.span) o.span = np::AddressRange{static_cast<uint32_t>(std::get<0>(*kw.span)), static_cast<uint32_t>(std::get<1>(*kw.span))};
        o.stub_ret = static_cast<uint64_t>(kw.stub_ret);
        o.max_steps = kw.steps < 0 ? 0 : static_cast<uint64_t>(kw.steps);
        for (double v : kw.fargs.vec()) o.fargs.push_back(v);
        if (kw.shared) o.shared = kw.shared->image();
        if (kw.fill) o.fill = np::fill_from_text(kw.fill->utf8()).value_or(np::Fill::None);
        o.branches = kw.branches;
        if (kw.absent) o.absent = np::AddressRange{static_cast<uint32_t>(std::get<0>(*kw.absent)), static_cast<uint32_t>(std::get<1>(*kw.absent))};
        std::vector<uint64_t> a;
        for (uint64_t v : args.vec()) a.push_back(v);

        std::unique_ptr<shim::ShadowAdapter> adapter;
        if (kw.shadow) {
            adapter = std::make_unique<shim::ShadowAdapter>(kw.shadow);
            o.shadow = adapter.get();
        }

        auto result = std::make_shared<np::RunResult>(np::run(rs, static_cast<uint32_t>(entry & 0xFFFFFFFFLL), a, o));
        Out out;
        out.exit = cb::compat::to_py(result->exit_text());
        out.truncated = result->truncated;
        for (const np::Call& c : result->calls) {
            py::list<uint64_t> regs;
            for (uint64_t v : c.args) regs.append(v);
            out.calls.append(std::make_tuple(cb::compat::to_py(c.name), regs));
        }
        out.ret = result->ret;
        out.fret = result->fret;
        out.steps = static_cast<int64_t>(result->steps);
        for (uint32_t p : result->path) out.path.add(static_cast<int64_t>(p));
        out.pc = result->pc;
        for (const np::ByteWrite& w : result->writes) out.writes.set(static_cast<int64_t>(w.address), std::make_tuple<int64_t, int64_t>(w.before, w.after));
        out.saved = result->saved;
        out.machine = py::ref<Machine>(std::make_shared<Machine>(result->machine));
        if (result->branches) {
            py::list<BranchEvent> events;
            for (const np::BranchEvent& e : *result->branches) {
                std::optional<Compare> c;
                if (e.compare) {
                    Compare x;
                    x.size = e.compare->size;
                    x.a = e.compare->a;
                    x.b = e.compare->b;
                    if (e.compare->a_from) x.a_from = static_cast<int64_t>(*e.compare->a_from);
                    if (e.compare->b_from) x.b_from = static_cast<int64_t>(*e.compare->b_from);
                    c = x;
                }
                events.append(BranchEvent(static_cast<int64_t>(e.address), e.taken, c));
            }
            out.branches = events;
        }
        out.result = result;
        return out;
    });
}

// run's record as the one run record (emulate/run.py): r3 and f1 the results, each call out with its eight argument
// registers, the bytes it left changed (outside the stack), r14-r31 and the stack pointer as they went in and came out, the
// addresses of `span` that ran.
inline emu_run::Run record(const Out& out) {
    const cb::emulate::Run r = np::record(*out.result);
    py::dict<py::str, emu_run::Register> results;
    for (const auto& kv : r.results) {
        if (const auto* v = std::get_if<uint64_t>(&kv.second)) {
            results.set(cb::compat::to_py(kv.first), emu_run::Register(static_cast<int64_t>(*v)));
        } else if (const auto* d = std::get_if<double>(&kv.second)) {
            results.set(cb::compat::to_py(kv.first), emu_run::Register(*d));
        } else {
            results.set(cb::compat::to_py(kv.first), emu_run::Register(py::none));
        }
    }
    py::list<emu_run::Write> writes;
    for (const cb::emulate::MemoryWrite& w : r.writes) {
        emu_run::Write x;
        x.address = static_cast<int64_t>(w.address);
        x.size = w.size;
        x.value = static_cast<int64_t>(w.value);
        if (w.old) x.old = static_cast<int64_t>(*w.old);
        x.harness = w.harness;
        writes.append(x);
    }
    py::list<emu_run::Call> calls;
    for (const cb::emulate::Call& c : r.calls) {
        emu_run::Call x;
        if (c.name) x.name = cb::compat::to_py(*c.name);
        if (c.args) {
            py::list<int64_t> args;
            for (int64_t v : *c.args) args.append(v);
            x.args = args;
        }
        calls.append(x);
    }
    std::optional<emu_run::Preserved> preserved;
    if (r.preserved) {
        emu_run::Preserved p;
        for (const auto& kv : *r.preserved) {
            auto s = [](const std::optional<uint64_t>& v) { return v ? std::optional<int64_t>(static_cast<int64_t>(*v)) : std::nullopt; };
            p.set(cb::compat::to_py(kv.first), std::make_tuple(s(kv.second.on_entry), s(kv.second.at_end)));
        }
        preserved = p;
    }
    py::list<int64_t> path;
    for (uint64_t a : r.path) path.append(static_cast<int64_t>(a));
    return emu_run::Run(r.exit ? std::optional<py::str>(cb::compat::to_py(*r.exit)) : std::nullopt, results, writes, false, calls,
                        py::list<emu_run::IO>(), std::nullopt, preserved, path, r.steps ? std::optional<int64_t>(static_cast<int64_t>(*r.steps)) : std::nullopt,
                        r.truncated, py::str("big"), std::make_shared<Out>(out));
}

// The platform's runner (platforms/xbox360.py emulator.run): run(...) as a Run.
inline emu_run::Run recorded(const py::list<Region>& regions, int64_t entry, const py::list<uint64_t>& args = py::list<uint64_t>(),
                             const RunOptions& kw = RunOptions()) {
    return record(run(regions, entry, args, kw));
}

}  // namespace codebreaker::emulate::powerpc
