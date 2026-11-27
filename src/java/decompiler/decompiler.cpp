// java/decompiler/decompiler.cpp -- see decompiler.hpp. Ported from Fernflower (de.fernflower.main.decompiler: BaseDecompiler, ConsoleDecompiler,
// PrintStreamLogger; main.extern.IFernflowerLogger), Copyright 2000-2015 JetBrains s.r.o., Apache License 2.0 (see NOTICE in this folder).
// cb-unity: alone (file-local helpers share names with the other decompiler sources)
#include "java/decompiler/decompiler.hpp"

#include <algorithm>
#include <cctype>
#include <exception>
#include <set>
#include <stdexcept>

#include "core/fs.hpp"
#include "formats/zip.hpp"
#include "java/decompiler/classes.hpp"
#include "java/decompiler/context.hpp"
#include "java/decompiler/text_buffer.hpp"

#ifdef _WIN32
#include <windows.h>
#undef ERROR   // (wingdi's, against Severity::ERROR)
#endif

namespace cb::java::decompiler {

// ---- IFernflowerLogger

std::string_view IFernflowerLogger::prefix(Severity severity) {
    switch (severity) {
        case Severity::TRACE: return "TRACE: ";
        case Severity::INFO: return "INFO:  ";
        case Severity::WARN: return "WARN:  ";
        case Severity::ERROR: return "ERROR: ";
    }
    return "";
}

std::optional<IFernflowerLogger::Severity> IFernflowerLogger::severity_of(std::string_view name) {
    std::string up(name);
    std::transform(up.begin(), up.end(), up.begin(), [](unsigned char c) { return static_cast<char>(std::toupper(c)); });
    if (up == "TRACE") return Severity::TRACE;
    if (up == "INFO") return Severity::INFO;
    if (up == "WARN") return Severity::WARN;
    if (up == "ERROR") return Severity::ERROR;
    return std::nullopt;
}

// ---- PrintStreamLogger

void PrintStreamLogger::write_message(const std::string& message, Severity severity) {
    if (accepts(severity)) sink_(std::string(prefix(severity)) + indent_string(indent_) + message);
}

void PrintStreamLogger::write_message(const std::string& message, const std::exception& t) {
    if (accepts(Severity::ERROR)) {
        write_message(message, Severity::ERROR);
        sink_(t.what());   // (the original prints the stack trace here)
    }
}

void PrintStreamLogger::start_reading_class(const std::string& class_name) {
    if (accepts(Severity::INFO)) {
        write_message("Decompiling class " + class_name, Severity::INFO);
        ++indent_;
    }
}

void PrintStreamLogger::end_reading_class() {
    if (accepts(Severity::INFO)) {
        --indent_;
        write_message("... done", Severity::INFO);
    }
}

void PrintStreamLogger::start_class(const std::string& class_name) {
    if (accepts(Severity::INFO)) {
        write_message("Processing class " + class_name, Severity::TRACE);
        ++indent_;
    }
}

void PrintStreamLogger::end_class() {
    if (accepts(Severity::INFO)) {
        --indent_;
        write_message("... proceeded", Severity::TRACE);
    }
}

void PrintStreamLogger::start_method(const std::string& method_name) {
    if (accepts(Severity::INFO)) {
        write_message("Processing method " + method_name, Severity::TRACE);
        ++indent_;
    }
}

void PrintStreamLogger::end_method() {
    if (accepts(Severity::INFO)) {
        --indent_;
        write_message("... proceeded", Severity::TRACE);
    }
}

void PrintStreamLogger::start_write_class(const std::string& class_name) {
    if (accepts(Severity::INFO)) {
        write_message("Writing class " + class_name, Severity::TRACE);
        ++indent_;
    }
}

void PrintStreamLogger::end_write_class() {
    if (accepts(Severity::INFO)) {
        --indent_;
        write_message("... written", Severity::TRACE);
    }
}

// ---- DecompileResult

std::map<std::string, std::string> DecompileResult::java_texts() const {
    std::map<std::string, std::string> res;
    for (const OutputFile& f : files) {
        if (f.is_class) res[f.container.empty() ? f.path : f.container + "!/" + f.path] = f.content;
    }
    return res;
}

// ---- MemoryResultSaver

namespace {

std::string join_path(const std::string& path, const std::string& name) { return path.empty() ? name : path + "/" + name; }

std::string bytes_to_string(const Bytes& b) { return std::string(b.begin(), b.end()); }

}  // namespace

std::string MemoryResultSaver::archive_key(const std::string& path, const std::string& archive_name) { return join_path(path, archive_name); }

void MemoryResultSaver::save_folder(const std::string& path) {
    (void)path;   // (nothing to create in memory)
}

void MemoryResultSaver::copy_file(const std::string& source, const std::string& path, const std::string& entry_name) {
    try {
        OutputFile f;
        f.path = join_path(path, entry_name);
        f.content = bytes_to_string(fs::read_file(source));
        result_.files.push_back(std::move(f));
    } catch (const std::exception& ex) {
        DecompilerContext::get_logger().write_message("Cannot copy " + source + " to " + entry_name, ex);
    }
}

void MemoryResultSaver::save_class_file(const std::string& path, const std::string& qualified_name, const std::string& entry_name, const std::string& content,
                                        const std::vector<int>& mapping) {
    (void)mapping;
    OutputFile f;
    f.path = join_path(path, entry_name);
    f.content = content;
    f.is_class = true;
    f.qualified_name = qualified_name;
    result_.files.push_back(std::move(f));
}

void MemoryResultSaver::create_archive(const std::string& path, const std::string& archive_name, const std::optional<std::string>& manifest) {
    const std::string key = archive_key(path, archive_name);
    map_archive_entries_[key];
    result_.archives.push_back(key);
    if (manifest) {
        OutputFile f;
        f.container = key;
        f.path = "META-INF/MANIFEST.MF";
        f.content = *manifest;
        result_.files.push_back(std::move(f));
    }
}

bool MemoryResultSaver::check_entry(const std::string& entry_name, const std::string& file) {
    const bool added = map_archive_entries_[file].insert(entry_name).second;
    if (!added) {
        DecompilerContext::get_logger().write_message("Zip entry " + entry_name + " already exists in " + file, IFernflowerLogger::Severity::WARN);
    }
    return added;
}

void MemoryResultSaver::save_dir_entry(const std::string& path, const std::string& archive_name, const std::string& entry_name) {
    const std::string file = archive_key(path, archive_name);
    if (!check_entry(entry_name, file)) return;
    OutputFile f;
    f.container = file;
    f.path = entry_name;
    f.is_directory = true;
    result_.files.push_back(std::move(f));
}

void MemoryResultSaver::copy_entry(const std::string& source, const std::string& path, const std::string& archive_name, const std::string& entry) {
    const std::string file = archive_key(path, archive_name);
    if (!check_entry(entry, file)) return;

    try {
        const formats::ZipArchive src = formats::ZipArchive::open(source);
        const formats::ZipArchive::Entry* e = src.find(entry);
        if (e != nullptr) {
            OutputFile f;
            f.container = file;
            f.path = entry;
            f.content = bytes_to_string(src.read(*e));
            f.is_directory = e->is_directory();
            result_.files.push_back(std::move(f));
        }
    } catch (const std::exception& ex) {
        DecompilerContext::get_logger().write_message("Cannot copy entry " + entry + " from " + source + " to " + file, ex);
    }
}

void MemoryResultSaver::save_class_entry(const std::string& path, const std::string& archive_name, const std::string& qualified_name, const std::string& entry_name,
                                         const std::optional<std::string>& content) {
    const std::string file = archive_key(path, archive_name);
    if (!check_entry(entry_name, file)) return;

    OutputFile f;
    f.container = file;
    f.path = entry_name;
    f.content = content.value_or(std::string());
    f.is_class = !qualified_name.empty();   // a class that could not be decompiled is still an entry, empty (a directory entry has no class)
    f.qualified_name = qualified_name;
    result_.files.push_back(std::move(f));
}

void MemoryResultSaver::close_archive(const std::string& path, const std::string& archive_name) {
    map_archive_entries_.erase(archive_key(path, archive_name));
}

// ---- decompile

namespace {

DecompileResult decompile_here(const DecompileRequest& request) {
    DecompileResult result;
    PrintStreamLogger logger([&result, &request](const std::string& line) {
        result.log.push_back(line);
        if (request.on_log) request.on_log(line);
    });
    MemoryResultSaver saver(result);

    Fernflower fernflower(saver, request.options, logger);
    try {
        // the libraries first, then the sources (ConsoleDecompiler's order, and RetroMCP's): a class a library also has is the source's
        for (const std::filesystem::path& library : request.libraries) {
            if (std::filesystem::exists(library)) fernflower.add_space(library, false);
            else result.log.push_back("warn: missing '" + library.string() + "', ignored");
        }
        for (const std::filesystem::path& source : request.sources) {
            if (std::filesystem::exists(source)) fernflower.add_space(source, true);
            else result.log.push_back("warn: missing '" + source.string() + "', ignored");
        }
        fernflower.decompile_context();
    } catch (...) {
        fernflower.clear_context();
        throw;
    }
    fernflower.clear_context();
    return result;
}

// The original's recursions (statements in statements, expressions in expressions) run as deep as the code is nested, on Java frames that are smaller
// than ours: a big client jar overflows a program's usual 1 MB. The run gets a stack of its own, reserved (not committed) as it needs it.
constexpr std::size_t kRunStack = std::size_t{512} << 20;

#ifdef _WIN32
struct RunOnBigStack {
    const DecompileRequest* request;
    DecompileResult result;
    std::exception_ptr error;
    static DWORD WINAPI entry(LPVOID self) {
        auto* run = static_cast<RunOnBigStack*>(self);
        try {
            run->result = decompile_here(*run->request);
        } catch (...) {
            run->error = std::current_exception();
        }
        return 0;
    }
};
#endif

}  // namespace

DecompileResult decompile(const DecompileRequest& request) {
#ifdef _WIN32
    RunOnBigStack run{&request, {}, nullptr};
    HANDLE thread = CreateThread(nullptr, kRunStack, &RunOnBigStack::entry, &run, STACK_SIZE_PARAM_IS_A_RESERVATION, nullptr);
    if (thread == nullptr) return decompile_here(request);   // (no thread to be had: the caller's stack)
    WaitForSingleObject(thread, INFINITE);
    CloseHandle(thread);
    if (run.error) std::rethrow_exception(run.error);
    return std::move(run.result);
#else
    (void)kRunStack;
    return decompile_here(request);
#endif
}

int save_into_folder(const DecompileResult& result, const std::filesystem::path& folder) {
    std::set<std::string> containers;
    int classes = 0;
    for (const OutputFile& f : result.files) {
        const bool first_of_archive = !f.container.empty() && containers.insert(f.container).second;
        if (first_of_archive && !f.is_class && f.path == "META-INF/MANIFEST.MF") continue;   // (createArchive's manifest)
        const std::filesystem::path p = folder / fs::from_text(f.path);
        if (f.is_directory) {
            std::filesystem::create_directories(p);
            continue;
        }
        fs::write_file(p, as_bytes(f.content));
        if (f.is_class) ++classes;
    }
    return classes;
}

bool parse_option_argument(const std::string& arg, std::map<std::string, std::string>& options) {
    if (arg.size() > 5 && arg[0] == '-' && arg[4] == '=') {
        std::string value = arg.substr(5);
        std::string up = value;
        std::transform(up.begin(), up.end(), up.begin(), [](unsigned char c) { return static_cast<char>(std::toupper(c)); });
        if (up == "TRUE") value = "1";   // ("true".equalsIgnoreCase(value): any other value is kept as written)
        else if (up == "FALSE") value = "0";
        options[arg.substr(1, 3)] = value;
        return true;
    }
    return false;
}

}  // namespace cb::java::decompiler
