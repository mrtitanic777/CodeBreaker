// cb-unity: alone
#include "cb/java/runtime.hpp"

#include <algorithm>
#include <array>
#include <format>
#include <set>

#include "cb/core/fs.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"
#include "cb/java/console/platform_equivalents.hpp"
#include "cb/java/emit/std_transforms.hpp"
#include "cb/java/platforms.hpp"
#include "cb/java/runtime_data.hpp"
#include "cb/java/runtime_texts.hpp"
#include "cb/java/stubs.hpp"
#include "cb/java/textutil.hpp"

namespace cb::java::runtime {

namespace {

using StrList = std::vector<std::string>;

// The include guard of a header: NAME_H.
std::string guard(std::string_view name) {
    std::string g = text::upper(name);
    std::ranges::replace(g, '.', '_');
    return g + "_H";
}

void append(StrList& out, const StrList& more) { out.insert(out.end(), more.begin(), more.end()); }

// The lines a header starts with: the include guard and the banner of an external-library stub.
StrList guarded_banner(std::string_view name) {
    const std::string g = guard(name);
    StrList lines{"#ifndef " + g, "#define " + g};
    append(lines, external_banner(name));
    return lines;
}

// Every stub method carries a greppable J2X-STUB marker, so one scan (`grep -rn J2X-STUB`) lists the whole placeholder
// surface -- a no-op that turns out to be load-bearing (the glPolygonOffset z-fight class) is never left silently undone.
std::string noop_marker(std::string_view name) {
    return std::format("   // J2X-STUB[{}]: no-op placeholder -- verify not load-bearing", name);
}
std::string default_marker(std::string_view name) { return std::format("   // J2X-STUB[{}]: returns default -- verify", name); }

// The frame of a stub header is the same for every kind: lines, then the guard's #endif and a final newline.
std::string finish(StrList lines, std::string_view name) {
    lines.push_back("#endif // " + guard(name));
    lines.emplace_back();
    return text::join(lines, "\n");
}

// `Type* method(` or `static Type* method(` in a verbatim member line: a pointer getter or factory the stub defines by
// hand (Runtime::getRuntime, Toolkit::getDefaultToolkit), whose return type the chain resolver should know.
std::optional<std::pair<std::string, std::string>> pointer_method(std::string_view line) {
    static const Regex re(R"((?:static\s+)?([A-Za-z_]\w*)\s*\*\s*(\w+)\s*\()");
    const auto m = re.search(line);
    if (!m || m->group(1) == "void") return std::nullopt;
    return std::pair{std::string(m->group(2)), std::string(m->group(1)) + "*"};
}

// The lines of java.awt.Color: not a symbol-only stub, getHSBColor/HSBtoRGB/getRGB are REAL. The Alpha sky colour is
// MobSpawnerBase::getSkyColorByTemp() -> Color::getHSBColor(h,s,b).getRGB(); a stub returning Color()/0 made the in-world
// sky render black. The constants are `static const int` (ARGB): a real `static Color X;` member would need an
// out-of-line definition (stubs are header-only) and fail to link; an integral constant with an in-class initialiser
// needs none, and the variadic Color constructor and setColor accept the int.
}  // namespace

std::string color_header() {
    // (defined outside the unnamed namespace: runtime_data.cpp refers to it)
    std::string out(verbatim::kColorPrefix);
    for (const auto& [name, argb] : color_constants()) out += std::format("    static const int {} = (int)0x{:08X};\n", name, argb);
    out += verbatim::kColorSuffix;
    return out;
}

namespace {

// What the platform's banner says about the platform layer (the first replacement of generate_files).
constexpr std::string_view kBannerPlatformLayer = "the real behaviour is the Xbox 360 platform layer";
constexpr std::string_view kBannerSdkLine = "// (Direct3D 9 / XInput / file IO / sound).";
constexpr std::string_view kStubBannerMarker = "// ==============================================================================";

// Add a platform's SDK hint after the banner of an external-library stub; other files are returned as they are.
std::string annotate_stub(std::string text, std::string_view class_name, std::string_view platform) {
    if (text.find("EXTERNAL-LIBRARY STUB") == std::string::npos || platform.empty()) return text;
    const stubs::SubsystemInfo* subsystem = stubs::classify_stub(class_name);
    const std::string comment = platforms::stub_comment(platform, subsystem ? subsystem->key : "unknown");
    const std::size_t at = text.find(kStubBannerMarker);
    if (at != std::string::npos) text.insert(at + kStubBannerMarker.size(), "\n" + comment);
    return text;
}

// Replace the Windows timer calls of JavaRT.h and Random.h with the platform's.
std::string patch_timers(std::string content, std::string_view platform) {
    const auto& table = timer_patches();
    const auto it = table.find(platform);
    if (it == table.end()) return content;
    const TimerPatch& patch = it->second;

    // `inline long long <name>(...) {...}` up to the function's first closing brace
    const auto replace_function = [&content](std::string_view head, bool brace_follows, const std::string& replacement) {
        std::size_t from = 0;
        while (true) {
            const std::size_t at = content.find(head, from);
            if (at == std::string::npos) return;
            std::size_t after = at + head.size();
            if (brace_follows) {
                while (after < content.size() && text::is_space(content[after])) ++after;
                if (after >= content.size() || content[after] != '{') { from = at + head.size(); continue; }
                ++after;
            }
            const std::size_t close = content.find('}', after);
            if (close == std::string::npos) return;
            content.replace(at, close + 1 - at, replacement);
            from = at + replacement.size();
        }
    };
    replace_function("inline long long currentTimeMillis()", false, patch.current_time_millis);
    replace_function("inline long long nanoTime()", true, patch.nano_time);
    content = text::replace_all(content, "(long long)GetTickCount()", patch.get_tick_count);
    return content;
}

// The listeners and adapters the desktop applet and window code implements.
const StrList kListenerMethods = {
    "keyPressed", "keyReleased", "keyTyped", "mouseClicked", "mousePressed", "mouseReleased", "mouseEntered", "mouseExited",
    "mouseMoved", "mouseDragged", "mouseWheelMoved", "windowClosing", "windowOpened", "windowClosed", "windowActivated",
    "windowDeactivated", "windowIconified", "windowDeiconified", "componentResized", "componentMoved", "componentShown",
    "componentHidden", "focusGained", "focusLost"};
constexpr auto kListeners = std::to_array<std::string_view>({"KeyListener", "MouseListener", "MouseMotionListener", "MouseWheelListener",
                                                              "WindowListener", "ComponentListener", "FocusListener"});
constexpr auto kEvents = std::to_array<std::string_view>({"KeyEvent", "MouseEvent", "WindowEvent", "ComponentEvent", "FocusEvent", "InputEvent"});
constexpr auto kLayouts = std::to_array<std::string_view>({"BorderLayout", "GridLayout", "FlowLayout", "CardLayout", "GridBagLayout"});
constexpr auto kBuffers = std::to_array<std::string_view>({"FloatBuffer", "IntBuffer", "ByteBuffer"});

// Exceptions are off on the console: these are type stubs so `extends Error`, `new IOException(...)` and catch/throws
// references compile. (exception, its base)
constexpr auto kExceptions = std::to_array<std::pair<std::string_view, std::string_view>>({
    {"Exception", "Throwable"}, {"Error", "Throwable"}, {"RuntimeException", "Exception"}, {"IOException", "Exception"},
    {"InterruptedException", "Exception"}, {"IllegalStateException", "RuntimeException"},
    {"IllegalArgumentException", "RuntimeException"}, {"MalformedURLException", "IOException"}, {"AWTException", "Exception"},
    {"LWJGLException", "Exception"}, {"NoSuchFieldError", "Error"},
    // java.io / java.net / java.lang exceptions used in catch/throws clauses
    {"FileNotFoundException", "IOException"}, {"EOFException", "IOException"}, {"SocketException", "IOException"},
    {"UnknownHostException", "IOException"}, {"ConnectException", "SocketException"}, {"BindException", "SocketException"},
    {"NumberFormatException", "IllegalArgumentException"}, {"NullPointerException", "RuntimeException"},
    {"ArrayIndexOutOfBoundsException", "RuntimeException"}, {"IndexOutOfBoundsException", "RuntimeException"},
    {"ClassCastException", "RuntimeException"}, {"NoSuchElementException", "RuntimeException"},
    {"UnsupportedOperationException", "RuntimeException"}, {"ConcurrentModificationException", "RuntimeException"},
    {"OutOfMemoryError", "Error"}, {"NoSuchMethodException", "Exception"}, {"CloneNotSupportedException", "Exception"},
    {"ClassNotFoundException", "Exception"}});

}  // namespace

// ---------------------------------------------------------------------------------------------- stubs

std::vector<std::string> external_banner(std::string_view name) {
    const auto& packages = external_packages();
    const auto found = packages.find(name);
    const std::string who = found != packages.end() ? found->second + "." + std::string(name) : "the external Java type " + std::string(name);
    StrList banner = {
        "",
        "// ============================ EXTERNAL-LIBRARY STUB ============================",
        "// Stands in for " + who + ".",
        "// This is NOT part of the decompiled Minecraft source -- it is an external Java",
        "// library. Symbol-only: methods are no-ops / return defaults so the converted game",
        "// code compiles and links; the real behaviour is the Xbox 360 platform layer",
        "// (Direct3D 9 / XInput / file IO / sound)."};
    // The exact XDK API this stub resolves to (from platform_equivalents), so nobody hand-guesses the port. No line:
    // there is no mapping yet for this symbol.
    const std::string annotation = console::platform_equivalents::annotation(name);
    if (!annotation.empty()) {
        banner.emplace_back("// ------------------------------------------------------------------------------");
        banner.push_back(annotation);
    }
    banner.emplace_back(kStubBannerMarker);
    return banner;
}

std::string ns_stub(const NsStub& spec) {
    StrList lines = guarded_banner(spec.name);
    lines.emplace_back("#include <string>");
    append(lines, spec.prelude);
    for (const std::string& h : spec.includes) lines.push_back("#include \"" + h + ".h\"");
    for (const std::string& f : spec.forward_decls) lines.push_back("class " + f + ";");
    lines.emplace_back();
    lines.push_back("namespace " + spec.name + " {");
    for (const Constant& c : spec.constants) lines.push_back(std::format("\tconst int {} = {};", c.name, c.value));
    for (const std::string& f : spec.void_fns) lines.push_back("\tinline void " + f + "(...) {}" + noop_marker(spec.name));
    for (const Function& f : spec.ret_fns) {
        lines.push_back(std::format("\tinline {} {}(...) {{ return {}; }}", f.type, f.name, f.value) + default_marker(spec.name));
    }
    append(lines, spec.extra_lines);
    lines.emplace_back("}");
    return finish(std::move(lines), spec.name);
}

std::string class_stub(const ClassStub& spec) {
    StrList lines = guarded_banner(spec.name);
    lines.emplace_back("#include <string>");
    append(lines, spec.prelude);
    lines.emplace_back("#include \"JavaString.h\"");
    for (const std::string& h : spec.includes) lines.push_back("#include \"" + h + ".h\"");
    for (const std::string& f : spec.forward_decls) lines.push_back("class " + f + ";");
    std::string declaration = "class " + spec.name;
    if (!spec.bases.empty()) {
        StrList bases;
        for (const std::string& b : spec.bases) bases.push_back("public " + b);
        declaration += " : " + text::join(bases, ", ");
    }
    lines.emplace_back();
    lines.push_back(declaration + " {");
    lines.emplace_back("public:");
    if (spec.ctors) lines.push_back("\t" + spec.name + "(...) {}");
    lines.push_back("\tvirtual ~" + spec.name + "() {}");
    for (const std::string& m : spec.void_methods) lines.push_back("\tvirtual void " + m + "(...) {}" + noop_marker(spec.name));
    for (const Function& m : spec.ret_methods) {
        // A method that returns a Java String must return JavaString, not std::string -- else the caller's
        // `.startsWith()/.substring()/.trim()` (Java String methods) do not compile.
        const bool is_string = m.type == "std::string";
        lines.push_back(std::format("\tvirtual {} {}(...) {{ return {}; }}", is_string ? "JavaString" : m.type, m.name,
                                    is_string ? "JavaString()" : m.value) + default_marker(spec.name));
    }
    for (const std::string& line : spec.extra) lines.push_back("\t" + line);
    lines.emplace_back("};");
    append(lines, spec.trailer);
    return finish(std::move(lines), spec.name);
}

std::string buffer_stub(std::string_view name) {
    const std::string n(name);
    StrList lines = guarded_banner(name);
    append(lines, {"class FloatBuffer; class IntBuffer; class ByteBuffer;", "", "class " + n + " {", "public:", "\t" + n + "() {}",
                   "\tvirtual ~" + n + "() {}"});
    for (const char* m : {"clear", "flip", "rewind", "mark"}) lines.push_back(std::format("\t{}& {}() {{ return *this; }}", n, m));   // no argument, return self
    for (const char* m : {"put", "putInt", "putShort", "putLong", "putFloat", "putDouble", "putChar"}) {
        lines.push_back(std::format("\t{}& {}(...) {{ return *this; }}", n, m));   // variadic, return self
    }
    append(lines, {"\tint get(...) { return 0; }", "\tint getInt(...) { return 0; }", "\tshort getShort(...) { return 0; }",
                   "\tlong long getLong(...) { return 0; }", "\tfloat getFloat(...) { return 0.0F; }",
                   "\tdouble getDouble(...) { return 0.0; }", "\tchar getChar(...) { return 0; }"});
    for (const char* m : {"position", "limit"}) {   // a getter and a setter, which do not clash
        lines.push_back(std::format("\tint {}() {{ return 0; }}", m));
        lines.push_back(std::format("\t{}& {}(int) {{ return *this; }}", n, m));
    }
    append(lines, {"\tint remaining() { return 0; }", "\tint capacity() { return 0; }", "\tFloatBuffer* asFloatBuffer() { return 0; }",
                   "\tIntBuffer* asIntBuffer() { return 0; }",
                   // ByteBuffer.order(ByteOrder) answers itself: a pointer, matching the member types and chaining into a ByteBuffer* local
                   "\t" + n + "* order(...) { return this; }",
                   "\tstatic " + n + "* allocateDirect(...) { return new " + n + "(); }",   // ByteBuffer.allocateDirect(n)
                   "\tstatic " + n + "* allocate(...) { return new " + n + "(); }", "\toperator void*() { return 0; }", "};"});
    return finish(std::move(lines), name);
}

std::vector<std::string> keyboard_name_lines() {
    StrList lines = {"\tinline JavaString getKeyName(int key) {", "\t\tswitch (key) {"};
    for (const auto& [code, name] : lwjgl_key_names()) lines.push_back(std::format("\t\tcase {}: return JavaString(\"{}\");", code, name));
    append(lines, {"\t\t}", "\t\treturn JavaString();   // null, as LWJGL", "\t}", "\tinline int getKeyIndex(const std::string& name) {"});
    for (const auto& [code, name] : lwjgl_key_names()) lines.push_back(std::format("\t\tif (name == \"{}\") return {};", name, code));
    append(lines, {"\t\treturn 0;   // KEY_NONE, as LWJGL", "\t}"});
    return lines;
}

// ---------------------------------------------------------------------------------------------- the runtime

Runtime::Runtime(const JavaData& data) : canonical_dir_(data.path({"platform", "canonical"})) {
    build_base();
    build_stubs();
    build_repeated_stubs();
}

void Runtime::put_file(std::string name, std::string_view text) { files_.insert_or_assign(std::move(name), std::string(text)); }

// A hand-written header, real and not a stub, kept verbatim in the platform/canonical folder.
void Runtime::add_canonical(const std::string& name) {
    files_.insert_or_assign(name, textutil::read_text(canonical_dir_ / name, textutil::Decode::Strict));
}

void Runtime::add_ns_stub(const std::string& file, const NsStub& spec) { put_file(file, ns_stub(spec)); }

// Registers the stub's member types for the chain resolver as a side effect of making it: its parent, the return types of
// its `ret_methods` and of the pointer getters among its verbatim lines. The last definition of a name wins.
void Runtime::add_class_stub(const std::string& file, const ClassStub& spec) {
    StubMembers record;
    if (!spec.bases.empty()) record.parent = spec.bases.front();
    for (const Function& m : spec.ret_methods) record.methods[m.name] = m.type;
    for (const std::string& line : spec.extra) {
        if (const auto method = pointer_method(line)) record.methods[method->first] = method->second;
    }
    members_.insert_or_assign(spec.name, std::move(record));
    put_file(file, class_stub(spec));
}

void Runtime::build_base() {
    for (const char* name : {"Random.h", "JavaString.h", "JArray.h", "JIterator.h", "JList.h", "JSet.h", "JMap.h", "JavaRT.h", "Runnable.h",
                             "Thread.h", "Comparator.h", "Collections.h", "FilenameFilter.h"}) {
        add_canonical(name);
    }
    // java.util.Iterator maps to the `Iterator` struct, which lives in JIterator.h. Code that names the type emits
    // `#include "Iterator.h"` (typemap::runtime_headers()), so it gets a thin alias header.
    put_file("Iterator.h", "#ifndef ITERATOR_H\n#define ITERATOR_H\n#include \"JIterator.h\"\n#endif\n");
    put_file("GL11.h", verbatim::kGl11Header);
}

void Runtime::build_repeated_stubs() {
    for (const std::string_view name : kBuffers) put_file(std::string(name) + ".h", buffer_stub(name));
    // java.awt event listeners and adapters, implemented or extended by the desktop applet and window code
    for (const std::string_view name : kListeners) add_class_stub(std::string(name) + ".h", {.name = std::string(name), .void_methods = kListenerMethods});
    // java.awt event objects
    for (const std::string_view name : kEvents) {
        add_class_stub(std::string(name) + ".h",
                       {.name = std::string(name),
                        .void_methods = {"consume"},
                        .ret_methods = {{"getKeyCode", "int", "0"}, {"getKeyChar", "char", "0"}, {"getX", "int", "0"}, {"getY", "int", "0"},
                                        {"getButton", "int", "0"}, {"getClickCount", "int", "0"}, {"getWheelRotation", "int", "0"},
                                        {"getWindow", "void*", "0"}, {"getComponent", "void*", "0"}, {"isShiftDown", "bool", "false"},
                                        {"isControlDown", "bool", "false"}, {"isAltDown", "bool", "false"}}});
    }
    // layout managers (`new BorderLayout()` passed to setLayout): no-op stubs
    for (const std::string_view name : kLayouts) add_class_stub(std::string(name) + ".h", {.name = std::string(name)});
    for (const auto& [name, base] : kExceptions) {
        add_class_stub(std::string(name) + ".h", {.name = std::string(name), .bases = {std::string(base)}, .includes = {std::string(base)}});
    }
}

std::vector<std::string> Runtime::unwired_canonicals() const {
    std::vector<std::string> unwired;
    std::error_code ec;
    if (!std::filesystem::is_directory(canonical_dir_, ec)) return unwired;
    for (const auto& entry : std::filesystem::directory_iterator(canonical_dir_, ec)) {
        const std::string name = fs::to_text(entry.path().filename());
        if (name.ends_with(".h") && !files_.contains(name)) unwired.push_back(name);
    }
    std::ranges::sort(unwired);
    return unwired;
}

std::map<std::string, std::string, std::less<>> Runtime::generate_files(const GenerateOptions& options) const {
    const platforms::Platform* platform = options.platform.empty() ? nullptr : platforms::find_platform(options.platform);
    const std::string_view platform_label = platform ? platform->label : "Xbox 360";
    const std::string std_label = options.cpp_std ? options.cpp_std->label : "C++03";

    std::map<std::string, std::string, std::less<>> out;
    if (!options.platform.empty()) {
        out["stdafx.h"] = platforms::generate_stdafx(options.platform, std_label);
        // The precompiled-header create source: Xbox 360 projects build <Project>.pch from stdafx.cpp (/Yc) and every other .cpp
        // uses it (/Yu); without this file the project's stdafx.cpp Create block has no source and every file fails with C1083.
        // (Written for every platform: it is only `#include "stdafx.h"`, harmless where no PCH is used.)
        out["stdafx.cpp"] = "#include \"stdafx.h\"\n";
    } else {
        // Every emitted file includes stdafx.h; with no platform it is the generic one (the C/C++ standard library and the
        // fixed-width typedefs), so a conversion with no platform compiles ...
        out["stdafx.h"] = platforms::generate_stdafx("generic", std_label);
        // ... and the out-of-line pieces of the file layer that a platform's reference layer defines (FileIO.reference.cpp on
        // the 360), in standard C, so a program that opens a file still links.
        out["j2x_host_io.cpp"] = std::string(verbatim::kHostIoCpp);
    }
    if (options.platform == "pc_opengl") out["j2x_host_io.cpp"] = std::string(verbatim::kHostIoCpp);   // the PC's files are the C library's

    const bool non_windows = options.platform == "ps3" || options.platform == "wii" || options.platform == "psp" || options.platform == "nds";
    for (const auto& [name, original] : files_) {
        std::string content = original;
        const std::string class_name = name.ends_with(".h") ? name.substr(0, name.size() - 2) : name;
        if (platform && platform_label != "Xbox 360") {
            content = text::replace_all(content, kBannerPlatformLayer, "the real behaviour is the " + std::string(platform_label) + " platform layer");
            if (const stubs::SubsystemInfo* subsystem = stubs::classify_stub(class_name)) {
                const platforms::SubsystemApi& api = platform->api(subsystem->id);
                content = text::replace_all(content, kBannerSdkLine, std::format("// ({} / {}).", api.sdk, api.libs));
            }
        }
        if ((non_windows || options.platform.empty()) && (name == "JavaRT.h" || name == "Random.h")) {
            content = patch_timers(std::move(content), options.platform.empty() ? "generic" : options.platform);
        }
        if (!options.platform.empty()) content = annotate_stub(std::move(content), class_name, options.platform);
        if (options.cpp_std) content = emit::std_transforms::apply_std_transforms(content, options.cpp_std->year);
        out[name] = std::move(content);
    }
    return out;
}

}  // namespace cb::java::runtime
