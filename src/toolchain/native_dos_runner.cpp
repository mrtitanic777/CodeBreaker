// DosRunner on CodeBreaker's own DOS executive (native_dos_runner.hpp).
#include "toolchain/native_dos_runner.hpp"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <map>
#include <system_error>

#include "core/fs.hpp"
#include "core/text.hpp"

namespace cb::toolchain {

namespace {

namespace stdfs = std::filesystem;

std::string ndr_trim(std::string_view s) {
    while (!s.empty() && std::isspace(static_cast<unsigned char>(s.front()))) s.remove_prefix(1);
    while (!s.empty() && std::isspace(static_cast<unsigned char>(s.back()))) s.remove_suffix(1);
    return std::string(s);
}

std::string ndr_upper(std::string s) {
    for (char& c : s) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    return s;
}

// (text written as a text-mode DOS program would: CRLF line ends)
std::string ndr_dos_text(std::string_view s) {
    std::string out;
    for (std::size_t k = 0; k < s.size(); ++k) {
        if (s[k] == '\n' && (k == 0 || s[k - 1] != '\r')) out += '\r';
        out += s[k];
    }
    return out;
}

// The session's drives and current drive, and how a DOS name ("D:\T.OBJ", "t.obj", "C:BIN\CL.EXE") is found on the host.
struct NdrSession {
    std::map<char, stdfs::path> drives;
    char current = 'D';
    OrderedMap<std::string, std::string> variables;

    std::optional<stdfs::path> host(std::string_view name) const {
        std::string n(name);
        for (char& c : n)
            if (c == '/') c = '\\';
        char letter = current;
        if (n.size() > 1 && n[1] == ':') {
            letter = static_cast<char>(std::toupper(static_cast<unsigned char>(n[0])));
            n = n.substr(2);
        }
        const auto d = drives.find(letter);
        if (d == drives.end()) return std::nullopt;
        stdfs::path p = d->second;
        for (const std::string_view part : text::split(n, '\\'))
            if (!part.empty() && part != ".") p /= part;
        // (DOS names are not case-sensitive: an existing file is found by its name in any case)
        std::error_code ec;
        if (!stdfs::exists(p, ec) && stdfs::is_directory(p.parent_path(), ec))
            for (const auto& e : stdfs::directory_iterator(p.parent_path(), ec))
                if (ndr_upper(e.path().filename().string()) == ndr_upper(p.filename().string())) return e.path();
        return p;
    }

    // COMMAND.COM's search: the name as given when it has an extension, else .COM, .EXE; the current folder, then PATH.
    std::optional<stdfs::path> program(std::string_view name) const {
        const std::string n(name);
        const bool has_ext = n.find('.', n.find_last_of("\\:") == std::string::npos ? 0 : n.find_last_of("\\:")) != std::string::npos;
        std::vector<std::string> folders;
        if (n.find_first_of("\\:") != std::string::npos) {
            folders.emplace_back();
        } else {
            folders.emplace_back("");
            for (const auto& [k, v] : variables)
                if (ndr_upper(k) == "PATH")
                    for (const std::string_view entry : text::split(v, ';'))
                        if (!ndr_trim(entry).empty()) folders.push_back(ndr_trim(entry));
        }
        std::error_code ec;
        for (const std::string& folder : folders) {
            std::string base = folder.empty() ? n : (folder.back() == '\\' ? folder : folder + "\\") + n;
            const std::vector<std::string> tries = has_ext ? std::vector<std::string>{base} : std::vector<std::string>{base + ".COM", base + ".EXE"};
            for (const std::string& t : tries)
                if (const auto p = host(t); p && stdfs::is_regular_file(*p, ec)) return p;
        }
        return std::nullopt;
    }

    Bytes environment() const {
        Bytes env;
        for (const auto& [k, v] : variables) {
            const std::string line = ndr_upper(k) + "=" + v;
            env.insert(env.end(), line.begin(), line.end());
            env.push_back(0);
        }
        if (env.empty()) env = {'P', 'A', 'T', 'H', '=', 0};
        return env;
    }
};

}  // namespace

DosCommandLine parse_dos_command(std::string_view raw) {
    DosCommandLine c;
    std::string line = ndr_trim(raw);
    if (!line.empty() && line.front() == '@') line = ndr_trim(line.substr(1));
    if (line.empty()) return c;
    const std::string upper = ndr_upper(line);
    const auto word = [&](std::string_view w) { return upper == w || upper.starts_with(std::string(w) + " ") || upper.starts_with(std::string(w) + "\t"); };
    if (word("REM") || word("ECHO") || word("CD") || word("CHDIR") || upper.starts_with("ECHO.")) return c;
    if (line.size() == 2 && line[1] == ':' && std::isalpha(static_cast<unsigned char>(line[0]))) {
        c.kind = DosCommandLine::Kind::Drive;
        c.name = std::string(1, static_cast<char>(std::toupper(static_cast<unsigned char>(line[0]))));
        return c;
    }
    if (word("SET")) {
        const std::string rest = ndr_trim(line.substr(3));
        const std::size_t eq = rest.find('=');
        if (eq == std::string::npos) return c;
        c.kind = DosCommandLine::Kind::Set;
        c.name = ndr_upper(ndr_trim(rest.substr(0, eq)));
        c.value = rest.substr(eq + 1);
        return c;
    }
    // the redirections, taken out of the line wherever they are
    std::string kept;
    for (std::size_t k = 0; k < line.size();) {
        if (line.compare(k, 4, "2>&1") == 0) {
            c.with_errors = true;
            k += 4;
            continue;
        }
        if (line[k] == '>') {
            c.append = k + 1 < line.size() && line[k + 1] == '>';
            k += c.append ? 2 : 1;
            while (k < line.size() && line[k] == ' ') ++k;
            std::size_t e = k;
            while (e < line.size() && line[e] != ' ' && line[e] != '>' && line[e] != '<') ++e;
            c.out = line.substr(k, e - k);
            k = e;
            continue;
        }
        kept += line[k++];
    }
    kept = ndr_trim(kept);
    if (word("DEL") || word("ERASE")) {
        c.kind = DosCommandLine::Kind::Delete;
        c.name = ndr_trim(kept.substr(kept.find(' ') == std::string::npos ? kept.size() : kept.find(' ')));
        return c;
    }
    c.kind = DosCommandLine::Kind::Program;
    // (COMMAND.COM ends a program's name at a blank, a switch or a separator: `CL/c` runs CL with "/c")
    std::size_t e = 0;
    while (e < kept.size() && kept[e] != ' ' && kept[e] != '\t' && kept[e] != '/' && kept[e] != ',' && kept[e] != ';' && kept[e] != '=') ++e;
    c.program = kept.substr(0, e);
    c.tail = kept.substr(e);
    if (!c.tail.empty() && c.tail.front() != ' ') c.tail = " " + c.tail;   // (the tail as DOS hands it on: a blank first)
    while (c.tail.size() > 1 && c.tail.back() == ' ') c.tail.pop_back();
    return c;
}

DosRunResult NativeDosRunner::run(const DosRunSpec& spec) const {
    try {
        if (!dos_.available()) throw native_dos::NativeDosError("the DOS executive is not available here");
        return run_here(spec);
    } catch (const native_dos::NativeDosError& e) {   // (a program the executive cannot complete: the run says so, nothing is pretended)
        DosRunResult none;
        none.why = e.what();
        return none;
    }
}

DosRunResult NativeDosRunner::run_here(const DosRunSpec& spec) const {
    DosRunResult result;
    const stdfs::path work = spec.work && !spec.work->empty() ? *spec.work : fs::ScopedTempDir("cb_dos").release();
    fs::make_dirs(work);
    result.work = work;
    for (const auto& [name, data] : spec.files) {
        if (const auto* text = std::get_if<std::string>(&data)) fs::write_text(work / name, ndr_dos_text(*text));
        else fs::write_file(work / name, std::get<Bytes>(data));
    }
    std::error_code ec;
    for (const std::string& name : spec.artifacts) stdfs::remove(work / name, ec);   // (a stale one would pass for this run's)

    NdrSession session;
    for (const DosMount& m : spec.mounts) session.drives[static_cast<char>(std::toupper(static_cast<unsigned char>(m.drive.front())))] = m.folder;
    if (!session.drives.contains('D')) session.drives['D'] = work;
    // (the last drive listed is the current one: the work drive when none names it)
    session.current = spec.mounts.empty() || !std::any_of(spec.mounts.begin(), spec.mounts.end(), [](const DosMount& m) { return std::toupper(static_cast<unsigned char>(m.drive.front())) == 'D'; })
                          ? 'D'
                          : static_cast<char>(std::toupper(static_cast<unsigned char>(spec.mounts.back().drive.front())));
    session.variables = spec.variables;

    for (const std::string& raw : spec.commands) {
        const DosCommandLine c = parse_dos_command(raw);
        switch (c.kind) {
            case DosCommandLine::Kind::Nothing:
                break;
            case DosCommandLine::Kind::Drive:
                if (session.drives.contains(c.name.front())) session.current = c.name.front();
                break;
            case DosCommandLine::Kind::Set:
                session.variables[c.name] = c.value;
                break;
            case DosCommandLine::Kind::Delete:
                if (const auto p = session.host(c.name)) stdfs::remove(*p, ec);
                break;
            case DosCommandLine::Kind::Program: {
                const std::optional<stdfs::path> exe = session.program(c.program);
                if (!exe) break;   // ("Bad command or file name": the line does nothing, as under DOS)
                native_dos::SessionRun run;
                run.exe = *exe;
                run.drives = session.drives;
                run.drive = session.current;
                run.cmdline = c.tail;
                run.env = session.environment();
                native_dos::SessionExit exit;
                try {
                    exit = dos_.run_session(run);
                } catch (const native_dos::NativeDosError&) {
                    run.dpmi = true;   // (a DOS-extended program: again, with the DPMI host)
                    exit = dos_.run_session(run);
                }
                if (c.out && ndr_upper(*c.out) != "NUL") {
                    if (const auto to = session.host(*c.out)) {
                        const Bytes& what = c.with_errors ? exit.console : exit.standard_out;
                        Bytes all = c.append && stdfs::exists(*to, ec) ? fs::read_file(*to) : Bytes{};
                        all.insert(all.end(), what.begin(), what.end());
                        fs::write_file(*to, all);
                    }
                }
                break;
            }
        }
    }
    // (the batch file's last line: `echo _DONE_ > done.txt` -- the drivers take it as "the session ran to its end")
    fs::write_text(work / "done.txt", "_DONE_ \r\n");
    for (const std::string& name : spec.artifacts) {
        const stdfs::path file = work / name;
        if (stdfs::exists(file, ec)) result.made[name] = fs::read_file(file);
    }
    result.ok = result.made.size() == spec.artifacts.size();
    return result;
}

}  // namespace cb::toolchain