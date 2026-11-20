// java/decompiler/decompiler.hpp -- the decompiler's public entry: Java classes (a jar, a zip, a folder or a class file) in, the Java source out as the
// original's console tool writes it (a jar becomes a jar of .java files, a folder a folder of them, other entries are copied), kept in memory with the
// run's log; plus the logger and result-saver interfaces the driver is made of.
//
//     DecompileRequest request;
//     request.sources = {"game.jar"};
//     request.options = {{"dgs", "1"}, {"udv", "0"}};          // the original's option keys (context.hpp, pref::)
//     DecompileResult result = decompile(request);
//     result.java_texts()["game.jar!/net/minecraft/Main.java"];   // the text of one class
//
// Ported from Fernflower (de.fernflower.main.decompiler: BaseDecompiler, ConsoleDecompiler; main.extern: IFernflowerLogger, IResultSaver;
// main.decompiler.PrintStreamLogger), Copyright 2000-2015 JetBrains s.r.o., Apache License 2.0 (see NOTICE in this folder). ConsoleDecompiler writes
// to a destination directory; here the same calls (IResultSaver) are caught in memory (MemoryResultSaver), and what a caller does with them (write a
// zip, compare with a golden) is its own. A Java exception's stack trace is not reproduced in the log: the line of its message is.
#pragma once

#include <exception>
#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <vector>

namespace cb::java::decompiler {

// The decompiler's log (the original's IFernflowerLogger): messages with a severity at or above the run's LOG_LEVEL, and the start and end of each
// phase (a class read, processed, written; the save).
class IFernflowerLogger {
public:
    enum class Severity { TRACE, INFO, WARN, ERROR };
    // "TRACE: ", "INFO:  ", "WARN:  ", "ERROR: ".
    static std::string_view prefix(Severity severity);
    // The severity named "trace", "info", "warn" or "error" (any case); nullopt for another word.
    static std::optional<Severity> severity_of(std::string_view name);

    virtual ~IFernflowerLogger() = default;
    bool accepts(Severity severity) const { return static_cast<int>(severity) >= static_cast<int>(severity_); }
    void set_severity(Severity severity) { severity_ = severity; }

    virtual void write_message(const std::string& message, Severity severity) = 0;
    // An ERROR message followed by what the exception says.
    virtual void write_message(const std::string& message, const std::exception& t) = 0;
    virtual void start_reading_class(const std::string& class_name) { (void)class_name; }
    virtual void end_reading_class() {}
    virtual void start_class(const std::string& class_name) { (void)class_name; }
    virtual void end_class() {}
    virtual void start_method(const std::string& method_name) { (void)method_name; }
    virtual void end_method() {}
    virtual void start_write_class(const std::string& class_name) { (void)class_name; }
    virtual void end_write_class() {}
    virtual void start_save(int total) { (void)total; }
    virtual void update_save(int current) { (void)current; }

private:
    Severity severity_ = Severity::INFO;
};

// The console tool's logger: a line per message, "INFO:  Decompiling class a/B" with the nesting indented, each line handed to `sink`.
class PrintStreamLogger : public IFernflowerLogger {
public:
    using Sink = std::function<void(const std::string& line)>;
    explicit PrintStreamLogger(Sink sink) : sink_(std::move(sink)) {}
    void write_message(const std::string& message, Severity severity) override;
    void write_message(const std::string& message, const std::exception& t) override;
    void start_reading_class(const std::string& class_name) override;
    void end_reading_class() override;
    void start_class(const std::string& class_name) override;
    void end_class() override;
    void start_method(const std::string& method_name) override;
    void end_method() override;
    void start_write_class(const std::string& class_name) override;
    void end_write_class() override;

private:
    Sink sink_;
    int indent_ = 0;
};

// Where the results go (the original's IResultSaver): `path` is the unit's folder relative to the destination.
class IResultSaver {
public:
    virtual ~IResultSaver() = default;
    virtual void save_folder(const std::string& path) = 0;
    virtual void copy_file(const std::string& source, const std::string& path, const std::string& entry_name) = 0;
    // `mapping`: original line -> decompiled line pairs when BYTECODE_SOURCE_MAPPING is on, else empty.
    virtual void save_class_file(const std::string& path, const std::string& qualified_name, const std::string& entry_name, const std::string& content,
                                 const std::vector<int>& mapping) = 0;
    // `manifest`: the source jar's META-INF/MANIFEST.MF, kept as is.
    virtual void create_archive(const std::string& path, const std::string& archive_name, const std::optional<std::string>& manifest) = 0;
    virtual void save_dir_entry(const std::string& path, const std::string& archive_name, const std::string& entry_name) = 0;
    virtual void copy_entry(const std::string& source, const std::string& path, const std::string& archive_name, const std::string& entry) = 0;
    // `content` none: the class could not be decompiled (the entry is made empty, as the original's null content).
    virtual void save_class_entry(const std::string& path, const std::string& archive_name, const std::string& qualified_name, const std::string& entry_name,
                                  const std::optional<std::string>& content) = 0;
    virtual void close_archive(const std::string& path, const std::string& archive_name) = 0;
};

// One file of the output: a loose file of a folder unit (container empty, `path` its relative path), or an entry of an archive (container: the archive's
// path relative to the destination; `path` the entry's name, "a/b/C.java" for a class, "a/" for a directory).
struct OutputFile {
    std::string container;
    std::string path;
    std::string content;          // UTF-8 text for a decompiled class; the bytes for a copied file or entry; empty for a directory
    bool is_directory = false;
    bool is_class = false;        // a decompiled class (content is Java source), else copied from the source
    std::string qualified_name;   // of the class, when is_class
};

struct DecompileResult {
    std::vector<OutputFile> files;      // in the order the original writes them
    std::vector<std::string> log;       // the lines the console tool prints (PrintStreamLogger's)
    std::vector<std::string> archives;  // the archives made, in order
    // The Java texts by name: `path` for a loose file, `container + "!/" + path` for an archive's entry.
    std::map<std::string, std::string> java_texts() const;
};

// Collects what a run saves (ConsoleDecompiler's job, in memory): files, archives with their entries (an entry named twice in an archive is kept once and
// warned about, as the original's check does).
class MemoryResultSaver : public IResultSaver {
public:
    explicit MemoryResultSaver(DecompileResult& result) : result_(result) {}
    void save_folder(const std::string& path) override;
    void copy_file(const std::string& source, const std::string& path, const std::string& entry_name) override;
    void save_class_file(const std::string& path, const std::string& qualified_name, const std::string& entry_name, const std::string& content,
                         const std::vector<int>& mapping) override;
    void create_archive(const std::string& path, const std::string& archive_name, const std::optional<std::string>& manifest) override;
    void save_dir_entry(const std::string& path, const std::string& archive_name, const std::string& entry_name) override;
    void copy_entry(const std::string& source, const std::string& path, const std::string& archive_name, const std::string& entry) override;
    void save_class_entry(const std::string& path, const std::string& archive_name, const std::string& qualified_name, const std::string& entry_name,
                          const std::optional<std::string>& content) override;
    void close_archive(const std::string& path, const std::string& archive_name) override;

private:
    static std::string archive_key(const std::string& path, const std::string& archive_name);
    bool check_entry(const std::string& entry_name, const std::string& file);

    DecompileResult& result_;
    std::map<std::string, std::set<std::string>> map_archive_entries_;   // by archive key: the entries made so far
};

struct DecompileRequest {
    std::vector<std::filesystem::path> sources;     // decompiled (the console tool's <source>s: a jar, a zip, a folder, a class file)
    std::vector<std::filesystem::path> libraries;   // read for the types they hold, not decompiled (the console tool's -e=<path>)
    std::map<std::string, std::string> options;     // over the defaults (pref::defaults): "1"/"0" for a switch, text otherwise
    std::function<void(const std::string& line)> on_log;   // each log line as it is written (besides DecompileResult::log)
};

// Saves `result` into `folder` as RetroMCP's DirectoryResultSaver does: an archive's entries (its classes as Java, its other files as they are, its
// directories) at their own paths under the folder, a loose class at its path; the archive's manifest is not written (the saver's createArchive does
// nothing). Returns how many classes were written.
int save_into_folder(const DecompileResult& result, const std::filesystem::path& folder);

// Decompiles `request` as BaseDecompiler/ConsoleDecompiler do. A class that cannot be decompiled is logged and its entry left empty; a missing source
// or an unreadable archive is logged; std::runtime_error only for what the original would let escape.
DecompileResult decompile(const DecompileRequest& request);

// One command-line argument of the console tool: "-dgs=true" (the key is the three characters after '-', the value upper-cased, TRUE -> "1", FALSE ->
// "0"). Returns false when `arg` is not such an option.
bool parse_option_argument(const std::string& arg, std::map<std::string, std::string>& options);

}  // namespace cb::java::decompiler
