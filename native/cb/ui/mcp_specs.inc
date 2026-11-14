        {"describe", "describe", R"DOC(What a file is, the platform it belongs to, and the lane CodeBreaker reconstructs it with
(None when there is none yet). A corpus name is accepted too.)DOC", Kind::Reading,
         {{"path", ParamType::String, std::nullopt}}},
        {"inspect", "inspect_module", R"DOC(The inspector: headers, sections, imports, exports, resources, version. Without `section`
the whole description (long parts are listed instead when it is too big); with it, one part:
summary, dos, headers, sections, exports, imports, resources, directories, relocations, ...
Addresses are hex text; file offsets and sizes are numbers (as hex_dump and resource take
them). Imports by number carry the name when one is known, and where it came from.)DOC", Kind::Reading,
         {{"module", ParamType::String, std::nullopt}, {"section", ParamType::String, std::string_view(R"JSON("")JSON")}}},
        {"strings", "strings", R"DOC(Text found in a file (ASCII and UTF-16), with file offsets; `find` keeps those containing it.)DOC", Kind::Reading,
         {{"module", ParamType::String, std::nullopt}, {"find", ParamType::String, std::string_view(R"JSON("")JSON")}, {"min_len", ParamType::Integer, std::string_view(R"JSON(4)JSON")}, {"offset", ParamType::Integer, std::string_view(R"JSON(0)JSON")}, {"limit", ParamType::Integer, std::string_view(R"JSON(300)JSON")}}},
        {"hex_dump", "hex_dump", R"DOC(A hex dump of a file's bytes from file `offset`, or from the place a 32-bit module's
address (or function name) `addr` is stored (at most 16 KB).)DOC", Kind::Reading,
         {{"module", ParamType::String, std::nullopt}, {"offset", ParamType::Integer, std::string_view(R"JSON(0)JSON")}, {"length", ParamType::Integer, std::string_view(R"JSON(512)JSON")}, {"addr", ParamType::String, std::string_view(R"JSON("")JSON")}}},
        {"resource", "resource", R"DOC(One resource, by the file offset and size `inspect` gives for it: a picture (icon,
bitmap, cursor) as an image, anything else as text.)DOC", Kind::Reading,
         {{"module", ParamType::String, std::nullopt}, {"offset", ParamType::Integer, std::nullopt}, {"size", ParamType::Integer, std::nullopt}, {"type_id", ParamType::Integer, std::string_view(R"JSON(-1)JSON")}, {"wide", ParamType::Boolean, std::string_view(R"JSON(true)JSON")}}},
        {"modules", "modules", R"DOC(The corpus modules with their ledger numbers (functions, verified, weak, unverified, ...).
`kind`: pe, ne, le, dos, powerpc, ...; `find`: part of a name.)DOC", Kind::Reading,
         {{"kind", ParamType::String, std::string_view(R"JSON("")JSON")}, {"find", ParamType::String, std::string_view(R"JSON("")JSON")}, {"only_in_store", ParamType::Boolean, std::string_view(R"JSON(false)JSON")}, {"offset", ParamType::Integer, std::string_view(R"JSON(0)JSON")}, {"limit", ParamType::Integer, std::string_view(R"JSON(200)JSON")}, {"refresh", ParamType::Boolean, std::string_view(R"JSON(false)JSON")}}},
        {"coverage", "coverage", R"DOC(How the corpus stands: totals and one row per kind of module.)DOC", Kind::Reading,
         {{"refresh", ParamType::Boolean, std::string_view(R"JSON(false)JSON")}}},
        {"progress", "progress", R"DOC(What a running sweep is doing: results per minute, recent modules, busy workers, a rough ETA.)DOC", Kind::Reading,
         {}},
        {"functions", "functions", R"DOC(Every function the store holds for a module, with its class (verified, weak, unverified,
not-judged, no-compile, no-source), name and why; plus counts and the commonest reasons. A
function a runtime library or a tool (the thunk compiler) made carries `library`, and is
counted under `identified`. `cls` keeps one class (or "identified"); `find` matches a name or
an address.)DOC", Kind::Reading,
         {{"module", ParamType::String, std::nullopt}, {"cls", ParamType::String, std::string_view(R"JSON("")JSON")}, {"find", ParamType::String, std::string_view(R"JSON("")JSON")}, {"offset", ParamType::Integer, std::string_view(R"JSON(0)JSON")}, {"limit", ParamType::Integer, std::string_view(R"JSON(300)JSON")}}},
        {"function", "function", R"DOC(One function: its reconstructed source, the verdict and the verdict's detail. `addr` is
its address (hex, with or without 0x) or its name.)DOC", Kind::Reading,
         {{"module", ParamType::String, std::nullopt}, {"addr", ParamType::String, std::nullopt}}},
        {"names", "names", R"DOC(Every function and import of a module with its best name and where the name came from
(exports, debug symbols, runtime libraries, bodies named in other modules).)DOC", Kind::Reading,
         {{"module", ParamType::String, std::nullopt}, {"find", ParamType::String, std::string_view(R"JSON("")JSON")}, {"offset", ParamType::Integer, std::string_view(R"JSON(0)JSON")}, {"limit", ParamType::Integer, std::string_view(R"JSON(300)JSON")}}},
        {"identify", "identify", R"DOC(Functions of a module that belong to a runtime library (by signature), and what built it.)DOC", Kind::Reading,
         {{"module", ParamType::String, std::nullopt}}},
        {"demangle", "demangle", R"DOC(Decorated linker names (Microsoft C++ ?Foo@@..., _Bar@8) as readable declarations.)DOC", Kind::Reading,
         {{"decorated", ParamType::StringList, std::nullopt}}},
        {"commands", "commands", R"DOC(Every command of `python -m codebreaker` with its one-line usage (any of them can be run as
a job through run_command, except the interfaces and servers).)DOC", Kind::Reading,
         {}},
        {"command_help", "command_help", R"DOC(The full description of one command: its module's text (arguments, options, what it writes).)DOC", Kind::Reading,
         {{"command", ParamType::String, std::nullopt}}},
        {"jobs", "jobs", R"DOC(The jobs started, newest first: with the service running, those of every interface (the
terminal's, each MCP server's), each with its owner; else this server's.)DOC", Kind::Reading,
         {}},
        {"job", "job", R"DOC(A job's state and its output from byte `offset` (up to 256 KB; `next` is where to go on).
`tail`: just the last that many characters of the output instead.)DOC", Kind::Reading,
         {{"job_id", ParamType::String, std::nullopt}, {"offset", ParamType::Integer, std::string_view(R"JSON(0)JSON")}, {"tail", ParamType::Integer, std::string_view(R"JSON(0)JSON")}}},
        {"job_file", "job_file", R"DOC(The file a job wrote (a decompile's C), from character `offset`.)DOC", Kind::Reading,
         {{"job_id", ParamType::String, std::nullopt}, {"offset", ParamType::Integer, std::string_view(R"JSON(0)JSON")}, {"length", ParamType::Integer, std::string_view(R"JSON(60000)JSON")}}},
        {"reach_results", "reach_results", R"DOC(The results `reach` kept (state/reach/<run>.json). Without `run`, the runs there are. With
it, the programs found: what each is, its platform and lane, and for those judged the tally;
`platform` and `find` (part of a path) narrow it, plus a count per platform.)DOC", Kind::Reading,
         {{"run", ParamType::String, std::string_view(R"JSON("")JSON")}, {"platform", ParamType::String, std::string_view(R"JSON("")JSON")}, {"judged_only", ParamType::Boolean, std::string_view(R"JSON(false)JSON")}, {"find", ParamType::String, std::string_view(R"JSON("")JSON")}, {"offset", ParamType::Integer, std::string_view(R"JSON(0)JSON")}, {"limit", ParamType::Integer, std::string_view(R"JSON(100)JSON")}}},
        {"projects", "projects", R"DOC(The project folders under Converted/ (one per binary CodeBreaker has taken apart): name,
the original's file name, its kind and the commands run on it.)DOC", Kind::Reading,
         {{"find", ParamType::String, std::string_view(R"JSON("")JSON")}}},
        {"project", "project", R"DOC(One project folder: its project.json (what was done, each output) and its files.)DOC", Kind::Reading,
         {{"name", ParamType::String, std::nullopt}, {"find", ParamType::String, std::string_view(R"JSON("")JSON")}}},
        {"source_tree", "source_tree", R"DOC(Where a module's source lives in the product's source tree (and why there), and its files,
each marked source, notes (.md, .txt) or built (a build's output or working folder); counts
of each, source first.)DOC", Kind::Reading,
         {{"module", ParamType::String, std::nullopt}, {"find", ParamType::String, std::string_view(R"JSON("")JSON")}}},
        {"read_file", "read_file", R"DOC(A text file from a project folder, the source tree, state/ or this checkout, from character
`offset` (a binary file: use hex_dump).)DOC", Kind::Reading,
         {{"path", ParamType::String, std::nullopt}, {"offset", ParamType::Integer, std::string_view(R"JSON(0)JSON")}, {"length", ParamType::Integer, std::string_view(R"JSON(40000)JSON")}}},
        {"decompile_function", "decompile_function", R"DOC(One function decompiled now, without starting a job: a 32-bit x86 function by itself (cached
by its inputs); any other kind decompiles its whole module once (kept) and the function is cut
out of it. `addr` is an address (hex) or a name. Not built or judged, and not necessarily the
text the store holds (that was made by the measure that judged it: `function`). Says so when it
takes longer than `wait` seconds: call again.)DOC", Kind::Reading,
         {{"module", ParamType::String, std::nullopt}, {"addr", ParamType::String, std::nullopt}, {"wait", ParamType::Integer, std::string_view(R"JSON(60)JSON")}}},
        {"compare_versions", "compare_versions", R"DOC(Two builds of a module function by function: identical, changed (with
a likeness score), added, removed, and the names the old build gives functions the new one
leaves unnamed. Changed pairs come first. `status` keeps identical or changed pairs only.)DOC", Kind::Reading,
         {{"module_a", ParamType::String, std::nullopt}, {"module_b", ParamType::String, std::nullopt}, {"status", ParamType::String, std::string_view(R"JSON("")JSON")}, {"offset", ParamType::Integer, std::string_view(R"JSON(0)JSON")}, {"limit", ParamType::Integer, std::string_view(R"JSON(200)JSON")}, {"wait", ParamType::Integer, std::string_view(R"JSON(60)JSON")}}},
        {"diff_function", "diff_function", R"DOC(One function in two builds, instruction by instruction: `addr` is its address (or name) in
the first build, or in the second. The listings are lined up by the kinds of instruction and
operand, each line marked ' ' the same (addresses aside), '~' the same instruction with other
registers or constants, '-' only in the first, '+' only in the second.)DOC", Kind::Reading,
         {{"module_a", ParamType::String, std::nullopt}, {"module_b", ParamType::String, std::nullopt}, {"addr", ParamType::String, std::nullopt}, {"wait", ParamType::Integer, std::string_view(R"JSON(60)JSON")}}},
        {"disasm", "disasm", R"DOC(The instructions at an address or function name. At a function's start (count 0): the whole
function as the analyses decode it, following its branches. Anywhere else, or with `count`: that
many instructions read straight on from there (64 when count is 0). Each line: address, file
offset (32-bit modules), bytes, instruction, and what it refers to (a function, an import by its
name, a string).)DOC", Kind::Reading,
         {{"module", ParamType::String, std::nullopt}, {"addr", ParamType::String, std::nullopt}, {"count", ParamType::Integer, std::string_view(R"JSON(0)JSON")}}},
        {"xrefs", "xrefs", R"DOC(What refers to an address or function, and what a function calls: the call and jump sites
that go there, the code that reads, writes or takes the address, the data that holds it, and,
for a function, each call it makes. `addr` may also be an import, as MODULE!name, its name
alone, or MODULE!#ordinal: then the code that calls it, directly or through a thunk.)DOC", Kind::Reading,
         {{"module", ParamType::String, std::nullopt}, {"addr", ParamType::String, std::nullopt}}},
        {"triage", "triage", R"DOC(A module at a glance: what it is, its exports and imports, what made it, where the function
store stands (counts, and each unverified function with its verdict's detail), the functions
a tool or runtime library made, and where its authored source lives. What to look at next.)DOC", Kind::Reading,
         {{"module", ParamType::String, std::nullopt}}},
        {"run_command", "run_command", R"DOC(Start `python -m codebreaker <command> <args...>` as a background job; returns at once with
a job id (follow it with `job`). For anything that takes more than a few seconds: decompile,
measure, reach --judge, pe-source, recompile-project, xna, java, ... At most two jobs run at a
time and only one measure.)DOC", Kind::Job,
         {{"command", ParamType::String, std::nullopt}, {"args", ParamType::OptionalStringList, std::string_view(R"JSON(null)JSON")}}},
        {"stop_job", "stop_job", R"DOC(Stop a job this server started (only that process is stopped).)DOC", Kind::Job,
         {{"job_id", ParamType::String, std::nullopt}}},
        {"name_function", "name_function", R"DOC(Give the function at `addr` a name in the function store. The name goes on its body, so
every module holding the same code gets it too (the next module write uses it).)DOC", Kind::Writing,
         {{"module", ParamType::String, std::nullopt}, {"addr", ParamType::String, std::nullopt}, {"name", ParamType::String, std::nullopt}}},
