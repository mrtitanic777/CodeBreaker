// app/tools.hpp -- the tools of the window's Tools page: each one as simple as the Convert page. A person drops or browses ONE file or folder (two for a
// Compare tool; a module picked from the work folder's list for a module tool), makes at most one plain choice (shown as buttons: "PC / Xbox 360"),
// presses Run, and sees the result with Open folder. There are no typed commands, flags or argument boxes: the command line is the tool's own, filled
// in from what was dropped and chosen. Nothing here knows about windows (so the tests run everywhere).
//
//     const tools::Tool* t = tools::find("java-port");          // "Java to C++": input Either, choice Platform = PC | Xbox 360
//     tools::Values v = tools::empty_values(*t);  v.inputs[0] = "C:/Games/a1.2.6.jar";  v.choice = 1;
//     if (!tools::missing(*t, v)) run(tools::build_argv(*t, v));   // {"java", "port", "C:/Games/a1.2.6.jar", "--platform", "xbox360"}
//
// The tools are tools_catalog.inc, one line each: id, title, group, one plain sentence, the input spec, the choice spec, the command line, and whether
// it is a developer tool. Every command of the command table is the command of at least one tool, or is named in left_out() with the reason (a test
// checks both). The everyday tools are in five plain groups (Convert, Look inside, Extract, Build, Compare); the developer and batch tools are in the
// groups after them and are shown only when the setting "Show developer tools" is on.
//
// The input spec: ""  none,  or  K Label[: what it takes]  then, for a second input, ';' and another.  K is F a file, D a folder, E a file or a
//                 folder, M a module of the work folder.  "F Program: .exe, .dll and other programs"
// The choice spec: "" none, or  Label=Button:value|Button:value  (the first is chosen at first). "Platform=PC:pc|Xbox 360:xbox360"
// The command line: words separated by spaces; {1} and {2} are the inputs, {choice} the chosen value; a word that is only a placeholder whose value is
//                 empty is left out, and so is the word before it when that one is an option (--to {choice} with no choice).
#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace cb::app::tools {

enum class InputKind { File, Folder, Either, Module };

struct Input {
    InputKind kind = InputKind::File;
    std::string label;                  // "Program"
    std::string accepts;                // ".exe, .dll and other programs" (the drop box's second line); may be empty
};

struct Choice {
    std::string label;                  // "PC"
    std::string value;                  // "pc"
};

struct Tool {
    std::string id;                     // unique: "java-port"
    std::string title;                  // "Java to C++"
    std::string group;                  // "Convert"
    std::string description;            // one plain sentence
    std::vector<Input> inputs;          // none, one, or two
    std::string choice_label;           // "Platform"; empty: no choice
    std::vector<Choice> choices;
    std::vector<std::string> argv;      // the command line's words, with the placeholders
    bool developer = false;             // shown only with "Show developer tools"
    std::string_view command() const { return argv.empty() ? std::string_view() : std::string_view(argv.front()); }
};

// The groups in the order the list shows them (the developer groups last), and every tool, in the order of its group (then the catalog's).
const std::vector<std::string>& groups();
bool is_developer_group(std::string_view group);
const std::vector<Tool>& all();
const Tool* find(std::string_view id);

// The commands of the table that are no tool's command, each with the reason.
struct LeftOut {
    std::string name, reason;
};
const std::vector<LeftOut>& left_out();

// The spec parsers (lenient: what they cannot read is left out, never an exception).
std::vector<Input> parse_inputs(std::string_view spec);
std::string parse_choices(std::string_view spec, std::vector<Choice>& out);   // returns the label

// Tools whose title, command or description holds every word of `query` (any case), developer tools only when `developer`; in list order.
std::vector<const Tool*> search(std::string_view query, bool developer);
bool matches(const Tool& tool, std::string_view query);

// What a person has given: a path (or module name) per input, and the chosen button.
struct Values {
    std::vector<std::string> inputs;
    int choice = 0;
};
Values empty_values(const Tool& tool);
// The label of the first input not given; nullopt when Run may be pressed.
std::optional<std::string> missing(const Tool& tool, const Values& values);
// The command line after the program name.
std::vector<std::string> build_argv(const Tool& tool, const Values& values);

// Whether a dropped path suits an input (a folder for a Folder input, a file for a File input, either for Either; a module never takes a drop).
bool accepts(const Input& input, bool is_folder);

// The folder the result is in, read from a finished run's output: the last "Output: <path>" line; else the last absolute path in the output that
// exists (a file: its folder). Empty when there is none (Open folder is then not offered).
std::filesystem::path result_folder(std::string_view output);

// The modules of a work folder's corpus: the program files under `root` (.exe .dll .drv .vxd .386 .cpl .scr .sys .com .xex and the like), each name
// once, sorted (any case). Empty when there is no such folder.
std::vector<std::string> modules_in(const std::filesystem::path& root);
// The modules whose name holds `query` (any case), at most `limit`, names starting with it first.
std::vector<std::string> filter_modules(const std::vector<std::string>& modules, std::string_view query, std::size_t limit);

}  // namespace cb::app::tools
