// codebreaker/invert/compilers.cpp -- C++17 port of codebreaker/invert/compilers.py (see compilers.hpp).

#include "codebreaker/invert/compilers.hpp"

#include <map>

#include "codebreaker/build/msc6.hpp"
#include "codebreaker/exact/toolchain.hpp"
#include "codebreaker/verify/authored/match.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"
#include "py_subprocess.hpp"

namespace codebreaker::invert::compilers {

namespace msc6 = codebreaker::build::msc6;
namespace match = codebreaker::verify::authored::match;

bool available() { return msc6::available(); }

py::list<std::optional<py::bytes>> mc6_compile_batch(const py::list<std::tuple<py::str, py::str>>& jobs, int64_t timeout) {
    if (!msc6::available()) throw py::RuntimeError("Microsoft C 6 is not set up (settings.json: dosbox_x, msc6)");
    std::vector<std::optional<py::bytes>> out(static_cast<size_t>(jobs.size()));
    std::vector<py::str> order;   // the flag sets, in first-seen order
    std::map<std::string, py::dict<msc6::Key, py::str>> by_flags;
    for (int64_t i = 0; i < jobs.size(); ++i) {
        const py::str& flags = std::get<1>(jobs[i]);
        if (!by_flags.count(flags.utf8())) order.push_back(flags);
        by_flags[flags.utf8()][msc6::Key(i)] = std::get<0>(jobs[i]);
    }
    for (const py::str& flags : order) {
        auto built = msc6::compile_many(by_flags[flags.utf8()], py::str(""), flags, std::nullopt, timeout);
        for (const auto& kv : built.items()) {
            const std::optional<msc6::Code>& c = std::get<1>(kv);
            if (c) out[static_cast<size_t>(std::get<int64_t>(std::get<0>(kv)))] = py::bytes(std::get<0>(*c));
        }
    }
    return py::list<std::optional<py::bytes>>(out);
}

py::bytes mc6_compile(const py::str& csrc, const py::str& flags, int64_t timeout) {
    py::list<std::tuple<py::str, py::str>> jobs{std::make_tuple(csrc, flags)};
    std::optional<py::bytes> code = mc6_compile_batch(jobs, timeout)[0];
    if (!code) throw py::RuntimeError("Microsoft C 6 gave no object (flags=" + py::repr(flags).utf8() + ")");
    return *code;
}

py::dict<py::str, std::tuple<py::bytes, py::list<int64_t>>> Obj::functions() const {
    py::dict<py::str, std::tuple<py::bytes, py::list<int64_t>>> out;
    for (const auto& kv : match::object_functions(data).items()) {
        const py::bytes& code = std::get<0>(std::get<1>(kv));
        const py::bytes& mask = std::get<1>(std::get<1>(kv));
        py::list<int64_t> positions;
        for (int64_t i = 0; i < mask.size(); ++i)
            if (mask[i]) positions.append(i);
        out[std::get<0>(kv)] = std::make_tuple(code, positions);
    }
    return out;
}

Msvc2::Msvc2(const py::str& cl_) : cl(cl_), env(match::compiler_env(cl_)) {
    work = py::os::path::join(py::settings::get_str(py::str("state")), py::str("invert"), py::str("msvc2"));
}

CompileResult Msvc2::compile_c(const py::str& text, const py::str& flags) const {
    py::os::makedirs(work, 0777, true);
    py::str obj = py::os::path::join(work, py::str("one.obj"));
    if (py::os::path::isfile(obj)) py::os::remove(obj);
    {
        py::file f = py::open(py::os::path::join(work, py::str("one.c")), py::str("w"), py::str("latin1"), py::str("replace"), py::str("\n"));
        f.write(text + py::str("\n"));
        f.close();
    }
    py::list<py::str> args{cl, py::str("/nologo"), py::str("/W0")};
    args.extend(py::split(flags));
    args.append(py::str("one.c"));
    py::subprocess::RunOptions o;
    o.cwd = work;
    o.env = env;
    o.encoding = "cp1252";
    o.errors = "replace";
    o.timeout = std::variant<int64_t, double>(int64_t(300));
    py::subprocess::CompletedProcess r = py::subprocess::run(args, o);
    CompileResult res;
    if (r.returncode || !py::os::path::isfile(obj)) {
        res.err = (r.stdout_.value_or(py::str("")) + r.stderr_.value_or(py::str(""))).strip();
        return res;
    }
    py::file f = py::open(obj, py::str("rb"));
    res.obj = f.read();
    f.close();
    return res;
}

Msvc2 msvc2() {
    py::str cl = match::compiler();
    if (cl.empty()) throw py::RuntimeError("the 1994 32-bit compiler is not set up (settings.json: period_compiler)");
    return Msvc2(cl);
}

std::optional<py::bytes> mc_compile(const py::str& csrc, const py::str& stem, int64_t timeout) {
    (void)stem;
    (void)timeout;   // (the profile names the file, and the passes run in this process)
    namespace tcm = codebreaker::exact::toolchain;
    bool set_up = false;
    std::shared_ptr<tcm::Toolchain> tc;
    try {   // period._kit("msc203")
        tc = tcm::load(py::str("msc203"));
        py::str kit = tcm::kit_dir(*tc->p.kit);
        set_up = py::os::path::isdir(kit);
    } catch (const py::RuntimeError&) {
    } catch (const py::OSError&) {
    } catch (const py::ValueError&) {
    }
    if (!set_up) throw py::RuntimeError("Microsoft C 2.03 is not set up (settings.json: kits.msc203)");
    auto r = tc->compile(csrc.encode("latin1", "replace"), py::dict<py::str, py::bytes>(), py::list<py::str>(), std::nullopt);
    return std::get<0>(r);
}

std::optional<py::bytes> mc_compile_multi(const py::str& csrc) { return mc_compile(csrc, py::str("probe"), 180); }

}  // namespace codebreaker::invert::compilers
