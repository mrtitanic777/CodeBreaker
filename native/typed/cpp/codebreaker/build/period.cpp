// codebreaker/build/period.cpp -- C++ from codebreaker.build.period (see period.hpp).

#include "codebreaker/build/period.hpp"

#include <algorithm>

#include "codebreaker/exact/aout.hpp"
#include "codebreaker/exact/toolchain.hpp"
#include "codebreaker/exact/xenix_c.hpp"
#include "codebreaker/verify/authored/match.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"
#include "py_stdlib.hpp"

namespace codebreaker::build::period {

const py::list<py::str> NAMES = py::list<py::str>{py::str("msvc2"), py::str("xenix-c"), py::str("msc6"), py::str("msvc152"), py::str("msc203")};

std::function<std::shared_ptr<Xcc>(const py::str&, const py::str&)>& xcc_loader() {
    static std::function<std::shared_ptr<Xcc>(const py::str&, const py::str&)> hook = [](const py::str&, const py::str&) -> std::shared_ptr<Xcc> {
        throw py::ModuleNotFoundError("No module named 'xcc'");
    };
    return hook;
}

namespace {

// settings.get(key) of a path or name setting ("" when it is not set)
py::str setting(const char* key) { return py::settings::get_str(py::str(key)); }

// toolchain.load(profile): a profile whose running is code (xenixc: "codebreaker.exact.xenix_c:XenixC") names a plugin, which the Python
// imports when the profile is loaded; here its port is registered first
std::shared_ptr<exact::toolchain::Toolchain> load_toolchain(const py::str& profile) {
    exact::xenix_c::register_plugin();
    return exact::toolchain::load(profile);
}

}  // namespace

// ---- XenixC ----------------------------------------------------------------------------------------

XenixC::XenixC(const py::str& tools, const py::str& target) : tools(tools), target(target) {
    name = py::str("xenix-c");
    bits = 16;
    tc_ = load_toolchain(py::str("xenixc"));
}

std::shared_ptr<Xcc> XenixC::_load() {
    if (!xcc_) {
        xcc_ = xcc_loader()(tools, target);
    }
    return xcc_;
}

std::optional<Code> XenixC::compile(const py::str& source, const std::optional<py::str>& work, const py::str& header, const py::str& function) {
    (void)work;
    (void)function;   // (a file's code is one stretch)
    py::bytes text = (header + source).replace(py::str("\r\n"), py::str("\n")).encode("latin1", "replace");
    std::optional<py::bytes> obj;
    try {
        py::list<py::str> include;
        include.append(py::os::path::join(_load()->WORK, py::str("src")));
        auto [made, printed] = tc_->compile(text, py::dict<py::str, py::bytes>(), include, std::nullopt);
        (void)printed;
        obj = made;
    } catch (const py::Exception&) {
        return std::nullopt;
    }
    if (!obj || !py::truthy(*obj)) {
        return std::nullopt;
    }
    exact::aout::Obj o = exact::aout::read(*obj);
    if (!o.seg.contains(py::str("_TEXT"))) {
        throw py::KeyError(py::repr(py::str("_TEXT")).utf8());
    }
    py::bytes code = o.seg.get(py::str("_TEXT"), py::bytes());
    if (!py::truthy(code)) {
        return std::nullopt;
    }
    py::list<int64_t> fixed;
    for (const auto& fx : o.fix) {
        if (std::get<0>(fx) == py::str("_TEXT")) {
            fixed.append(std::get<1>(fx));
        }
    }
    return Code(code, _mask(py::len(code), fixed));
}

py::bytes XenixC::_mask(int64_t size, const py::list<int64_t>& fixed) {
    py::bytearray mask = py::bytearray::zeros(size);
    for (int64_t at : fixed) {
        if (0 <= at && at < size) {
            // mask[at:at + 2] = b"\1\1"[:size - at]
            mask.set_slice(at, at + 2, py::bytes("\1\1").slice(0, size - at));
        }
    }
    return mask.view();
}

py::list<Unit> XenixC::units() {
    std::shared_ptr<Xcc> xcc = _load();
    py::bytes image = xcc->image();
    py::str src = py::os::path::join(xcc->WORK, py::str("src"));
    py::list<std::tuple<py::str, int64_t>> order;
    {
        py::file f = py::open(py::os::path::join(src, py::str("ORDER")), py::str("r"), py::str("latin1"));
        for (const py::str& line : f) {
            py::list<py::str> parts = py::split(line);
            if (py::len(parts) >= 2 && !line.startswith(py::str("#"))) {
                order.append(std::make_tuple(parts[0], py::int_parse(parts[1], 16)));
            }
        }
        f.close();
    }
    // a function kept in a file of its own is named for its address: f01034.c
    py::set<py::str> listed;
    for (const auto& n : order) {
        listed.add(std::get<0>(n));
    }
    for (const py::str& name : py::sorted(py::os::listdir(src))) {
        if (!listed.contains(name) && name.slice(0, 1) == py::str("f") && name.endswith(py::str(".c"))) {
            try {
                order.append(std::make_tuple(name, py::int_parse(name.slice(1, py::len(name) - 2), 16)));
            } catch (const py::ValueError&) {
            }
        }
    }
    py::list<Unit> out;
    for (const auto& item : order) {
        const py::str& name = std::get<0>(item);
        int64_t at = std::get<1>(item);
        py::str path = py::os::path::join(src, name);
        if (!py::os::path::isfile(path)) {
            continue;
        }
        py::file f = py::open_rb(path);
        py::str text = f.read().decode("latin1");
        f.close();
        std::optional<Code> built = compile(text, py::os::path::join(_scratch(), py::str("units")));
        if (!built) {
            continue;
        }
        const py::bytes& code = std::get<0>(*built);
        const py::bytes& mask = std::get<1>(*built);
        out.append(Unit(name, text, Code(image.slice(at, at + py::len(code)), mask), at));
    }
    return out;
}

// ---- Msvc2 -----------------------------------------------------------------------------------------

Msvc2::Msvc2(const py::str& cl, const py::str& flags) : cl(cl), flags(flags), env(verify::authored::match::compiler_env(cl)) {
    name = py::str("msvc2");
    bits = 32;
}

std::optional<Code> Msvc2::compile(const py::str& source, const std::optional<py::str>& work, const py::str& header, const py::str& function) {
    if (!work) {
        throw py::TypeError("expected str, bytes or os.PathLike object, not NoneType");
    }
    return verify::authored::match::compile_function(cl, env, header, source, function, *work, flags);
}

// ---- MicrosoftC16 ----------------------------------------------------------------------------------

MicrosoftC16::MicrosoftC16(const py::str& name_, const py::str& flags) : flags(flags) {
    name = name_;
    bits = 16;
}

std::optional<Code> MicrosoftC16::compile(const py::str& source, const std::optional<py::str>& work, const py::str& header, const py::str& function) {
    std::optional<py::dict<msc6::Key, py::str>> names;
    if (py::truthy(function)) {
        py::dict<msc6::Key, py::str> one;
        one.set(msc6::Key(py::str("x")), function);
        names = one;
    }
    py::dict<msc6::Key, py::str> sources;
    sources.set(msc6::Key(py::str("x")), source);
    return msc6::compile_many(sources, header, flags, work, 900, name, names)[msc6::Key(py::str("x"))];
}

py::dict<msc6::Key, std::optional<Code>> MicrosoftC16::compile_many(const py::dict<msc6::Key, py::str>& sources, const py::str& header,
                                                                    const ManyOptions& kw) {
    return msc6::compile_many(sources, header, flags, kw.work, kw.timeout, name, kw.names, kw.batch, kw.together);
}

// ---- Msc203 ----------------------------------------------------------------------------------------

Msc203::Msc203() {
    name = py::str("msc203");
    bits = 16;
    tc_ = load_toolchain(py::str("msc203"));
}

std::optional<py::bytes> Msc203::object(const py::str& source, const py::str& stem, int64_t timeout) {
    (void)stem;
    (void)timeout;
    auto [obj, printed] = tc_->compile(source.encode("latin1", "replace"), py::dict<py::str, py::bytes>(), py::list<py::str>(), std::nullopt);
    (void)printed;
    return obj;
}

std::optional<Code> Msc203::compile(const py::str& source, const std::optional<py::str>& work, const py::str& header, const py::str& function) {
    (void)work;
    std::optional<py::bytes> obj = object(header + source);
    if (!obj) {
        return std::nullopt;
    }
    if (py::truthy(function)) {
        return msc6::function_code(*obj, function);
    }
    auto [code, mask] = msc6::object_code(*obj);
    if (py::truthy(code)) {
        return Code(code, mask);
    }
    return std::nullopt;
}

// ---- which are set up ------------------------------------------------------------------------------

py::str _scratch() {
    py::str path = py::os::path::join(setting("state"), py::str("period"));
    py::os::makedirs(py::os::path::dirname(path), 0777, true);   // settings.state("period"): the state folder is made
    return path;
}

bool _kit(const py::str& profile, const py::list<py::str>& files) {
    py::str kit;
    try {
        std::shared_ptr<exact::toolchain::Toolchain> tc = load_toolchain(profile);
        if (!tc->p.kit) {
            throw py::KeyError(py::repr(py::str("kit")).utf8());
        }
        kit = exact::toolchain::kit_dir(*tc->p.kit);
    } catch (const py::RuntimeError&) {
        return false;
    } catch (const py::OSError&) {
        return false;
    } catch (const py::ValueError&) {
        return false;
    }
    if (!py::os::path::isdir(kit)) {
        return false;
    }
    for (const py::str& f : files) {
        if (!py::os::path::isfile(py::os::path::join(kit, f))) {
            return false;
        }
    }
    return true;
}

py::dict<py::str, bool> _set_up() {
    // folder(key, *files): the setting names a folder that holds these files
    auto folder = [](const char* key, const std::vector<const char*>& files) {
        py::str value = setting(key);
        if (!py::truthy(value)) {
            return false;
        }
        for (const char* f : files) {
            if (!py::os::path::isfile(py::os::path::join(py::str(setting(key)), py::str(f)))) {
                return false;
            }
        }
        return true;
    };
    py::dict<py::str, bool> out;
    out.set(py::str("msvc2"), folder("period_compiler", {"CL.EXE"}));
    bool xenix = folder("xenix_c", {"xcc.py"});
    if (xenix) {
        py::list<py::str> files;
        files.append(py::str("lib/c1"));
        files.append(py::str("lib/c2"));
        xenix = _kit(py::str("xenixc"), files);
    }
    out.set(py::str("xenix-c"), xenix);
    out.set(py::str("msc6"), msc6::available(py::str("msc6")));
    out.set(py::str("msvc152"), msc6::available(py::str("msvc152")));
    out.set(py::str("msc203"), _kit(py::str("msc203")));
    return out;
}

py::list<py::str> names() {
    py::dict<py::str, bool> have = _set_up();
    py::list<py::str> out;
    for (const py::str& n : NAMES) {
        if (have.get(n, false)) {
            out.append(n);
        }
    }
    return out;
}

std::shared_ptr<Period> get(const py::str& name, const std::optional<py::str>& target, const std::optional<py::str>& flags) {
    if (!NAMES.contains(name) || !_set_up().get(name, false)) {
        py::list<py::str> have = names();
        throw py::ValueError(py::percent(py::str("no period compiler %r set up (have: %s)"), name,
                                         py::truthy(have) ? py::join(py::str(", "), have) : py::str("none")));
    }
    bool has_flags = flags && py::truthy(*flags);
    if (name == py::str("xenix-c")) {
        return std::make_shared<XenixC>(setting("xenix_c"), (target && py::truthy(*target)) ? *target : py::str("link240"));
    }
    if (name == py::str("msvc2")) {
        py::str cl = py::os::path::join(setting("period_compiler"), py::str("CL.EXE"));
        return has_flags ? std::make_shared<Msvc2>(cl, *flags) : std::make_shared<Msvc2>(cl);
    }
    if (name == py::str("msc203")) {
        return std::make_shared<Msc203>();
    }
    return has_flags ? std::make_shared<MicrosoftC16>(name, *flags) : std::make_shared<MicrosoftC16>(name);
}

}  // namespace codebreaker::build::period
