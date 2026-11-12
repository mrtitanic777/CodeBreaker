#include "cb/exact/emu/native_dosexec.hpp"

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"

namespace cb::exact::emu::native_dosexec {

bool switched_off() {
    const auto v = fs::get_env("CB_NATIVE_DOSEXEC");
    if (!v) return false;
    const std::string s = text::lower(text::trim(*v));
    return s == "0" || s == "off" || s == "no" || s == "false";
}

dos::Limits limits_from_environment(double fast_limit) {
    dos::Limits l;
    l.fast_limit = fast_limit;
    if (const auto v = fs::get_env("DOSRUN_DPMI_LIMIT")) {
        try {
            std::size_t used = 0;
            const double seconds = std::stod(*v, &used);
            if (used == v->size() && seconds > 0) l.dpmi_limit = seconds;
        } catch (const std::exception&) {   // not a number: the default stands
        }
    }
    if (const auto v = fs::get_env("DOSRUN_DRIVE"))
        if (const auto drive = text::parse_int<int>(text::trim(*v), 10)) l.drive = *drive;
    if (const auto v = fs::get_env("DOSRUN_DEBUG")) l.debug = !v->empty();
    return l;
}

Places locate(const config::Config& config) {
    Places p;
    const std::filesystem::path root = config.checkout_root();
    if (const auto env = fs::get_env("CB_CBDOS_DLL"); env && !env->empty()) {
        p.cbdos = fs::from_text(*env);
    } else {
        const auto bin = config.path("dosexec_bin");
        p.cbdos = (bin ? *bin : root / "native" / "dosexec" / "build" / "bin") / "cbdos.dll";
    }
    if (const auto env = fs::get_env("CB_UNICORN_DLL"); env && !env->empty()) p.unicorn = fs::from_text(*env);
    else p.unicorn = root / "native" / "third_party" / "bin" / "unicorn.dll";
    return p;
}

Opened open(const Places& places, const dos::Limits& limits) {
    Opened out;
    if (switched_off()) {
        out.why.push_back("the DOS executive is switched off (CB_NATIVE_DOSEXEC)");
        return out;
    }
    try {
        out.executive = dos::Executive::load(places.cbdos, places.unicorn, limits);
    } catch (const Error& e) {   // a DLL that is missing or is not the executive: the way the lanes fall back is by this reason
        out.why.push_back(e.what());
    }
    return out;
}

}  // namespace cb::exact::emu::native_dosexec
