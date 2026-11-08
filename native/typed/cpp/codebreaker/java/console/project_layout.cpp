// codebreaker/java/console/project_layout.cpp -- typed C++ port of codebreaker.java.console.project_layout. See the header.

#include "codebreaker/java/console/project_layout.hpp"

#include "codebreaker/java/console/vcxproj.hpp"

namespace codebreaker::java::console::project_layout {

namespace {

py::str template_of_(const py::str& here) {
    return py::os::path::join(here, py::str("templates"), py::str("xbox360.vcxproj.in"));
}

// __FILE__ is <checkout>/native/typed/cpp/codebreaker/java/console/project_layout.cpp: six folders up is the
// checkout, whose codebreaker/java/console is where project_layout.py (and templates/) live.
py::str default_here_() {
    py::str d = py::os::path::dirname(py::os::path::abspath(py::str(__FILE__)));
    for (int i = 0; i < 6; ++i) d = py::os::path::dirname(d);
    return py::os::path::join(d, py::str("codebreaker"), py::str("java"), py::str("console"));
}


// Two things the runtime's os/shutil leave out (reported for the runtime):
//   * os.makedirs(path, exist_ok=True): when the folder cannot be made the OSError is the system's own
//     (FileNotFoundError for WinError 3, ...), as raise_os_ maps it; the runtime's makedirs throws a bare OSError;
//   * shutil.move(src, dst) of a folder into itself: shutil.Error ("Cannot move a directory '<src>' into itself
//     '<dst>'.", an OSError) -- the runtime's move would copy a folder into itself without end.
// os.makedirs(name, exist_ok=True), as posixpath/ntpath's makedirs does it: make the missing parents first,
// then mkdir name itself (a name that is already a folder is fine).
void makedirs_(const py::str& name) {
    auto [head, tail] = py::os::path::split(name);
    if (!head.empty() && !tail.empty() && !py::os::path::exists(head)) {
        try {
            makedirs_(head);
        } catch (const py::FileExistsError&) {
        }
        if (tail == py::os::curdir) {   // (xxx/newdir/. exists if xxx/newdir exists)
            return;
        }
    }
    std::error_code ec;
    std::filesystem::create_directory(std::filesystem::u8path(name.utf8()), ec);
    if (ec && !py::os::path::isdir(name)) py::os::raise_os_(ec, name);
}

py::str move_(const py::str& src, const py::str& dst) {
    if (py::os::path::isdir(src)) {
        py::str s = py::os::path::abspath(src), d = py::os::path::abspath(dst);
        if (!s.endswith(py::os::sep)) s = s + py::os::sep;
        if (!d.endswith(py::os::sep)) d = d + py::os::sep;
        if (s != d && d.startswith(s)) {   // (shutil._destinsrc)
            throw py::OSError(py::percent(py::str("Cannot move a directory '%s' into itself '%s'."), src, dst).utf8());
        }
    }
    return py::shutil::move(src, dst);
}

}  // namespace

py::str HERE = default_here_();
py::str _TEMPLATE = template_of_(HERE);

void set_here(const py::str& here) {
    HERE = here;
    _TEMPLATE = template_of_(here);
}

// things that stay at the solution root (never moved down into the project subfolder)
const std::tuple<py::str, py::str, py::str> _ROOT_KEEP_EXT = {py::str(".sln"), py::str(".sdf"), py::str(".suo")};

// (str.format: {name} {guid} {cfg} are filled by _sln_text; "{{" and "}}" are literal braces)
const py::str _SLN = py::str(
    "\n"
    "Microsoft Visual Studio Solution File, Format Version 11.00\n"
    "# Visual Studio 2010\n"
    "Project(\"{{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}}\") = \"{name}\", \"{name}\\{name}.vcxproj\", \"{guid}\"\n"
    "EndProject\n"
    "Global\n"
    "\tGlobalSection(SolutionConfigurationPlatforms) = preSolution\n"
    "\t\tCodeAnalysis|Xbox 360 = CodeAnalysis|Xbox 360\n"
    "\t\tDebug|Xbox 360 = Debug|Xbox 360\n"
    "\t\tProfile_FastCap|Xbox 360 = Profile_FastCap|Xbox 360\n"
    "\t\tProfile|Xbox 360 = Profile|Xbox 360\n"
    "\t\tRelease_LTCG|Xbox 360 = Release_LTCG|Xbox 360\n"
    "\t\tRelease|Xbox 360 = Release|Xbox 360\n"
    "\tEndGlobalSection\n"
    "\tGlobalSection(ProjectConfigurationPlatforms) = postSolution\n"
    "{cfg}\n"
    "\tEndGlobalSection\n"
    "\tGlobalSection(SolutionProperties) = preSolution\n"
    "\t\tHideSolutionNode = FALSE\n"
    "\tEndGlobalSection\n"
    "EndGlobal\n");

const py::list<py::str> _CONFIGS = {py::str("CodeAnalysis"), py::str("Debug"), py::str("Profile_FastCap"), py::str("Profile"),
                                    py::str("Release_LTCG"), py::str("Release")};

// A stable per-name GUID (so re-running yields the same .sln/.vcxproj binding).
py::str project_guid(const py::str& name) {
    py::str h = py::hashlib::md5((py::str("j2ctool-project-") + name).encode("utf-8", "strict")).hexdigest().upper();
    return py::percent("{%s-%s-%s-%s-%s}", h.slice_to(8), h.slice(8, 12), h.slice(12, 16), h.slice(16, 20), h.slice(20, 32));
}

std::optional<py::str> _existing_guid(const py::str& vcx_path) {
    if (!py::os::path::isfile(vcx_path)) {
        return std::nullopt;
    }
    py::str text;
    try {
        py::file f = py::open(vcx_path, py::str("r"), std::optional<py::str>(py::str("utf-8-sig")), std::optional<py::str>(py::str("replace")));
        text = f.read_text();
        f.close();
    } catch (const py::OSError&) {   // (IOError is OSError)
        return std::nullopt;
    }
    std::optional<py::re::Match> m = py::re::search(py::str("<ProjectGuid>(\\{[^}]*\\})</ProjectGuid>"), text);
    return m ? std::optional<py::str>(m->group(1)) : std::nullopt;
}

py::str _sln_text(const py::str& name, const py::str& guid) {
    py::list<py::str> lines;
    for (const py::str& c : _CONFIGS) {
        for (const char* kind : {"ActiveCfg", "Build.0"}) {
            lines.append(py::percent("\t\t%s.%s|Xbox 360.%s = %s|Xbox 360", guid, c, py::str(kind), c));
        }
    }
    py::str cfg = py::join(py::str("\n"), lines);
    // _SLN.format(name=name, guid=guid, cfg=cfg): one left-to-right pass, the replacement text is not rescanned
    const std::u32string& t = _SLN.u32();
    std::u32string out;
    for (size_t i = 0; i < t.size();) {
        if (t.compare(i, 2, U"{{") == 0) {
            out += U'{';
            i += 2;
        } else if (t.compare(i, 2, U"}}") == 0) {
            out += U'}';
            i += 2;
        } else if (t.compare(i, 6, U"{name}") == 0) {
            out += name.u32();
            i += 6;
        } else if (t.compare(i, 6, U"{guid}") == 0) {
            out += guid.u32();
            i += 6;
        } else if (t.compare(i, 5, U"{cfg}") == 0) {
            out += cfg.u32();
            i += 5;
        } else {
            out += t[i++];
        }
    }
    return py::str(out);
}

py::str write_solution(const py::str& root, const py::str& name, const py::str& guid) {
    py::str path = py::os::path::join(root, name + py::str(".sln"));
    py::file f = py::open(path, py::str("w"), std::optional<py::str>(py::str("utf-8")));
    f.write(_sln_text(name, guid));
    f.close();
    return path;
}

void _write_vcxproj_from_template(const py::str& vcx_path, const py::str& guid) {
    py::str tmpl;
    {
        py::file t = py::open(_TEMPLATE, py::str("r"), std::optional<py::str>(py::str("utf-8")));
        tmpl = t.read_text();
        t.close();
    }
    py::file f = py::open(vcx_path, py::str("w"), std::optional<py::str>(py::str("utf-8")));
    f.write(tmpl.replace(py::str("{{GUID}}"), guid));
    f.close();
}

bool _has_flat_sources(const py::str& d) {
    try {
        for (const py::str& f : py::os::listdir(d)) {
            if (f.endswith(py::str(".cpp"))) {
                return true;
            }
        }
        return false;
    } catch (const py::OSError&) {
        return false;
    }
}

int64_t _move_flat_into(const py::str& root, const py::str& proj_dir, const py::str& name) {
    makedirs_(proj_dir);
    int64_t moved = 0;
    for (const py::str& entry : py::os::listdir(root)) {
        if (entry == name) {                                       // the project subfolder
            continue;
        }
        if (entry.lower().endswith(_ROOT_KEEP_EXT)) {              // keep .sln/.sdf/.suo at root
            continue;
        }
        py::str src = py::os::path::join(root, entry);
        py::str dst = py::os::path::join(proj_dir, entry);
        if (py::os::path::abspath(src) == py::os::path::abspath(proj_dir)) {
            continue;
        }
        try {
            if (py::os::path::exists(dst)) {
                if (py::os::path::isdir(dst)) {
                    py::shutil::rmtree(dst, true);
                } else {
                    py::os::remove(dst);
                }
            }
            move_(src, dst);
            moved += 1;
        } catch (const py::OSError&) {                             // locked (e.g. an open log) -> leave at root
            continue;
        }
    }
    return moved;
}

Layout layout(const py::str& root_, const std::optional<py::str>& name_, bool regen) {
    py::str root = py::os::path::abspath(root_);
    // `name or os.path.basename(...)`: an empty name is the basename too
    py::str name = (name_.has_value() && !name_->empty()) ? *name_ : py::os::path::basename(py::os::path::normpath(root));
    py::str proj_dir = py::os::path::join(root, name);

    int64_t moved = 0;
    if (!_has_flat_sources(proj_dir) && _has_flat_sources(root)) {
        moved = _move_flat_into(root, proj_dir, name);
    }
    makedirs_(proj_dir);

    py::str vcx = py::os::path::join(proj_dir, name + py::str(".vcxproj"));
    std::optional<py::str> existing = _existing_guid(vcx);
    py::str guid = (existing.has_value() && !existing->empty()) ? *existing : project_guid(name);
    if (!py::os::path::isfile(vcx)) {
        _write_vcxproj_from_template(vcx, guid);
    }
    if (regen) {
        vcxproj::run(proj_dir, name);                              // refresh ClCompile/ClInclude + filters
    }

    py::str sln = write_solution(root, name, guid);
    return Layout{name, root, proj_dir, sln, vcx, guid, moved};
}

}  // namespace codebreaker::java::console::project_layout
