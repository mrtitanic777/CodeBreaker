// codebreaker/java/run_parser.inc -- the command-line parser of codebreaker/java/run.py main() as data, and the usage / help
// texts argparse prints for it (captured from the Python with COLUMNS=80 and no colour). Included by run.cpp only.

static const char* const MODULE_DOC = R"CBX(run.py -- the Java transpiler's tools: a Java program as C++03, and what its console port needs.

  python -m codebreaker java <tool> [arguments]      (`java <tool> --help` for a tool's options)

Transpile
  convert JAVA OUT [--platform P] [--edition E]   Java source -> C++ (also --mc-version ID to
                                  decompile a Minecraft version first, --project for a buildable tree)
  decompile VERSION WORKSPACE     a Minecraft version as Java source (RetroMCP / Fernflower;
                                  settings.json: java, retromcp)
  scaffold JAVA OUT               structural skeletons in C, C# or Rust (not a translation)
  signatures JAVA                 the codebase map: class -> fields and methods, Java -> C++ types
Judge and report     (classes judged class by class: `codebreaker module JAVA_FOLDER`)
  report OUT                      crash-class report of an emitted tree (and an MSVC build log)
  worklist OUT                    compile every .cpp with the host compiler, worst last
  bugfix-ledger                   every FIXED BUG / J2X-BUGFIX marker -> state/java/BUGFIX_LEDGER.md
  apimap ...                      the API-mapping registry: coverage, lookups
Console port (Xbox 360)
  sdk                             is the console SDK there (settings.json: xbox360_sdk)?
  platform-config OUT             controller, storage and keyboard choices -> platform.config.json
  assets JAR OUT                  textures and resources from the game's jar, PNGs normalised
  xact ...                        XACT3 audio banks: dump, build, coverage, match
  achievements JAVA               achievements -> the console's achievement data + unlock calls
  project-layout ROOT             the Visual Studio solution layout over an emitted tree
  vcxproj [--proj DIR]            the project's file list and filters, regenerated
  build SLN                       build the solution with MSBuild (settings.json: msbuild)
  debug OUT                       the one switchable Debug.h/.cpp
Rewrites of an emitted tree (each runs by itself in an xbox360 convert)
  reflect-factories, sound-fix, controller-nav, render-fix, applet-ping, ui-mark, keybind-mark
  scan-project FOLDER             a decompiled game's jar, sound folder, source and language file
)CBX";
static const char* const MAIN_PROG = "codebreaker java";
static const char* const MAIN_USAGE = R"CBX(usage: codebreaker java [-h]
                        {convert,decompile,report,signatures,worklist,vcxproj,xact,assets,platform-config,achievements,ui-mark,keybind-mark,bugfix-ledger,debug,sdk,project-layout,reflect-factories,sound-fix,controller-nav,render-fix,applet-ping,apimap,build,scaffold,scan-project} ...
)CBX";
static const char* const MAIN_HELP = R"CBX(usage: codebreaker java [-h]
                        {convert,decompile,report,signatures,worklist,vcxproj,xact,assets,platform-config,achievements,ui-mark,keybind-mark,bugfix-ledger,debug,sdk,project-layout,reflect-factories,sound-fix,controller-nav,render-fix,applet-ping,apimap,build,scaffold,scan-project} ...

The Java transpiler: a Java program as C++03.

positional arguments:
  {convert,decompile,report,signatures,worklist,vcxproj,xact,assets,platform-config,achievements,ui-mark,keybind-mark,bugfix-ledger,debug,sdk,project-layout,reflect-factories,sound-fix,controller-nav,render-fix,applet-ping,apimap,build,scaffold,scan-project}
    convert             Convert a Java source tree to C++.
    decompile           Decompile a Minecraft version to Java source (bundled
                        RetroMCP/Fernflower).
    report              Build the conversion report from existing output.
    signatures          Generate signatures.md (codebase map).
    worklist            Compile output and write the failure worklist.
    vcxproj             Regenerate the Xbox .vcxproj file list.
    xact                XACT3 audio banks: dump/build/coverage/match.
    assets              Extract textures/resources from the game jar into
                        Content\.
    platform-config     Select controller mapping + kernel calls
                        (keyboard/storage); emit them.
    achievements        Map the program's achievements to the console system
                        (XLAST + unlock seam).
    ui-mark             Tag every UI function with a greppable J2X-UI marker +
                        index.
    keybind-mark        Tag every hardcoded keyboard key-press with a
                        greppable J2X-KEYBIND marker + KEYBIND_INDEX.md (route
                        each to a controller button or menu toggle). Runs
                        automatically on every xbox360 convert.
    bugfix-ledger       Catalog every FIXED BUG / J2X-BUGFIX marker in the
                        converter's authored sources into BUGFIX_LEDGER.md.
                        Runs automatically on every convert.
    debug               Emit the single toggleable debug file (Debug.h/.cpp +
                        DEBUG.md).
    sdk                 Detect the platform SDK the converter would gate on.
    project-layout      Give a converted tree the VS solution layout
                        (<root>/<Name>.sln + <root>/<Name>/).
    reflect-factories   Turn Java reflection registries (newInstance) into
                        generated C++ factories.
    sound-fix           Drop the empty-OGG-pool null-guard in SoundManager so
                        sound effects route to the XACT bridge (fixes silent
                        SFX on console). Runs automatically on every xbox360
                        convert.
    controller-nav      Inject console controller MENU navigation into
                        GuiScreen (D-pad moves the selection, snaps the cursor
                        so hover+A-click work). The nav algorithm lives in
                        Input.reference; this injects the thin adapter. Runs
                        automatically on every xbox360 convert.
    render-fix          Apply the two console render fixes: cap
                        RenderGlobal::updateRenderers to a few chunk rebuilds
                        per frame (else a fresh world never presents -- stuck
                        on "Spawning"), and guard
                        EntityRenderer::hurtCameraEffect against the spawn 0/0
                        NaN (else the world renders invisible). Runs
                        automatically on every xbox360 convert.
    applet-ping         Neutralize the applet host ping in setLevel
                        (getDocumentBase()->openStream()) so the world is set
                        unconditionally on console. Runs automatically on
                        every xbox360 convert.
    apimap              Query the API-mapping registry
                        (languages/subsystems/resolve/coverage).
    build               Build a Visual Studio solution with MSBuild
                        (settings.json: msbuild).
    scaffold            Emit one STRUCTURAL skeleton file per Java class in C,
                        C# or Rust (types, fields, method signatures; not a
                        working translation).
    scan-project        From one folder of a decompiled game, find its jar,
                        sound resources, Java source and language file.

options:
  -h, --help            show this help message and exit
)CBX";

static const std::vector<ParserSpec>& PARSERS() {
    static const std::vector<ParserSpec> specs = {
        {"convert", "codebreaker java convert",
         R"CBX(usage: codebreaker java convert [-h] [--mc-version MC_VERSION]
                                [--mc-workspace MC_WORKSPACE] [--java JAVA]
                                [--platform PLATFORM] [--cpp-std CPP_STD]
                                [--platform-refs] [--project] [--sdk SDK]
                                [--edition {rubydung,indev,infdev,alpha,beta,release}]
                                [java_dir] [out_dir]
)CBX",
         R"CBX(usage: codebreaker java convert [-h] [--mc-version MC_VERSION]
                                [--mc-workspace MC_WORKSPACE] [--java JAVA]
                                [--platform PLATFORM] [--cpp-std CPP_STD]
                                [--platform-refs] [--project] [--sdk SDK]
                                [--edition {rubydung,indev,infdev,alpha,beta,release}]
                                [java_dir] [out_dir]

positional arguments:
  java_dir              Directory of .java source. Omit and use --mc-version
                        to decompile a Minecraft version first.
  out_dir               Output directory for the generated .h/.cpp.

options:
  -h, --help            show this help message and exit
  --mc-version MC_VERSION
                        Decompile this Minecraft version (bundled
                        RetroMCP/Fernflower backend) and convert its source in
                        one step. Requires a Java runtime + network. Usage:
                        convert --mc-version <id> <out_dir>
  --mc-workspace MC_WORKSPACE
                        Workspace dir for the decompile step (default:
                        <out_dir>/_decompiled).
  --java JAVA           java executable for the decompile step (else
                        $JAVA_HOME / PATH).
  --platform PLATFORM   Target platform id (xbox360, pc_windows, ps3, wii).
                        Drives endianness/null-page rule messages + stub
                        annotations.
  --cpp-std CPP_STD     Target C++ standard (C++98, C++03, C++11, ...).
                        Default C++03 (Xbox 360 XDK).
  --platform-refs       Emit the REAL hand-written platform layer (GL->D3D9
                        emulator, audio, input, socket) from
                        docs/platform/reference/ instead of no-op stubs.
  --project             One-step buildable project: after converting, also
                        (re)emit the platform config (preserving controller
                        binds) and produce the Visual Studio project --
                        BOOTSTRAP a new .sln + .vcxproj from the template if
                        none exists yet, or refresh the .vcxproj/.filters file
                        list if it does. No GUI needed. Implies --platform-
                        refs.
  --sdk SDK             Path to the target platform SDK root (else auto-
                        detected via env/standard paths). A console platform
                        with no located SDK is refused.
  --edition {rubydung,indev,infdev,alpha,beta,release}
                        Force the target Minecraft edition profile. Omit to
                        infer from --mc-version, else auto-detect from the
                        source, else keep the default profile.
)CBX",
         {
             {Kind::HELP, "help", {py::str("-h"), py::str("--help")}, '0', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "java_dir", {}, '?', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "out_dir", {}, '?', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "mc_version", {py::str("--mc-version")}, 'N', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "mc_workspace", {py::str("--mc-workspace")}, 'N', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "java", {py::str("--java")}, 'N', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "platform", {py::str("--platform")}, 'N', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "cpp_std", {py::str("--cpp-std")}, 'N', 's', {}, py::str("C++03"), false, std::nullopt},
             {Kind::STORE_TRUE, "platform_refs", {py::str("--platform-refs")}, '0', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE_TRUE, "project", {py::str("--project")}, '0', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "sdk", {py::str("--sdk")}, 'N', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "edition", {py::str("--edition")}, 'N', 's', {py::str("rubydung"), py::str("indev"), py::str("infdev"), py::str("alpha"), py::str("beta"), py::str("release")}, std::nullopt, false, std::nullopt},
         }},
        {"decompile", "codebreaker java decompile",
         R"CBX(usage: codebreaker java decompile [-h] [--java JAVA] version workspace
)CBX",
         R"CBX(usage: codebreaker java decompile [-h] [--java JAVA] version workspace

positional arguments:
  version      Minecraft version id (e.g. a1.2.6, b1.6.6, c0.30_01c,
               inf-20100618).
  workspace    Workspace dir; decompiled source lands under
               <workspace>/minecraft_*/src.

options:
  -h, --help   show this help message and exit
  --java JAVA  java executable (else $JAVA_HOME / PATH).
)CBX",
         {
             {Kind::HELP, "help", {py::str("-h"), py::str("--help")}, '0', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "version", {}, 'N', 's', {}, std::nullopt, true, std::nullopt},
             {Kind::STORE, "workspace", {}, 'N', 's', {}, std::nullopt, true, std::nullopt},
             {Kind::STORE, "java", {py::str("--java")}, 'N', 's', {}, std::nullopt, false, std::nullopt},
         }},
        {"report", "codebreaker java report",
         R"CBX(usage: codebreaker java report [-h] [--build-log BUILD_LOG] [--out OUT]
                               [--profile PROFILE] [--platform PLATFORM]
                               output_dir
)CBX",
         R"CBX(usage: codebreaker java report [-h] [--build-log BUILD_LOG] [--out OUT]
                               [--profile PROFILE] [--platform PLATFORM]
                               output_dir

positional arguments:
  output_dir            Directory of converted .cpp/.h.

options:
  -h, --help            show this help message and exit
  --build-log BUILD_LOG
                        MSVC build log to mine for errors.
  --out OUT             Report output dir (default: <output_dir>/report).
  --profile PROFILE     Profile name for the report header.
  --platform PLATFORM   Target platform id (xbox360, pc_windows, etc.).
)CBX",
         {
             {Kind::HELP, "help", {py::str("-h"), py::str("--help")}, '0', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "output_dir", {}, 'N', 's', {}, std::nullopt, true, std::nullopt},
             {Kind::STORE, "build_log", {py::str("--build-log")}, 'N', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "out", {py::str("--out")}, 'N', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "profile", {py::str("--profile")}, 'N', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "platform", {py::str("--platform")}, 'N', 's', {}, std::nullopt, false, std::nullopt},
         }},
        {"signatures", "codebreaker java signatures",
         R"CBX(usage: codebreaker java signatures [-h] [--out OUT]
                                   [--edition {rubydung,indev,infdev,alpha,beta,release}]
                                   java_dir
)CBX",
         R"CBX(usage: codebreaker java signatures [-h] [--out OUT]
                                   [--edition {rubydung,indev,infdev,alpha,beta,release}]
                                   java_dir

positional arguments:
  java_dir              Directory of .java source.

options:
  -h, --help            show this help message and exit
  --out OUT             Output path (default: signatures.md at repo root).
  --edition {rubydung,indev,infdev,alpha,beta,release}
                        Use this edition profile's package roles (else the
                        default profile).
)CBX",
         {
             {Kind::HELP, "help", {py::str("-h"), py::str("--help")}, '0', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "java_dir", {}, 'N', 's', {}, std::nullopt, true, std::nullopt},
             {Kind::STORE, "out", {py::str("--out")}, 'N', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "edition", {py::str("--edition")}, 'N', 's', {py::str("rubydung"), py::str("indev"), py::str("infdev"), py::str("alpha"), py::str("beta"), py::str("release")}, std::nullopt, false, std::nullopt},
         }},
        {"worklist", "codebreaker java worklist",
         R"CBX(usage: codebreaker java worklist [-h] [--cc CC] [--std STD] [--out OUT]
                                 out_dir
)CBX",
         R"CBX(usage: codebreaker java worklist [-h] [--cc CC] [--std STD] [--out OUT]
                                 out_dir

positional arguments:
  out_dir     Directory of converted .cpp/.h.

options:
  -h, --help  show this help message and exit
  --cc CC     Compiler (default: settings.json host_cxx, else g++).
  --std STD   C++ standard (default: c++03).
  --out OUT   Worklist output path.
)CBX",
         {
             {Kind::HELP, "help", {py::str("-h"), py::str("--help")}, '0', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "out_dir", {}, 'N', 's', {}, std::nullopt, true, std::nullopt},
             {Kind::STORE, "cc", {py::str("--cc")}, 'N', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "std", {py::str("--std")}, 'N', 's', {}, py::str("c++03"), false, std::nullopt},
             {Kind::STORE, "out", {py::str("--out")}, 'N', 's', {}, std::nullopt, false, std::nullopt},
         }},
        {"vcxproj", "codebreaker java vcxproj",
         R"CBX(usage: codebreaker java vcxproj [-h] [--proj PROJ]
                                [--edition {rubydung,indev,infdev,alpha,beta,release}]
)CBX",
         R"CBX(usage: codebreaker java vcxproj [-h] [--proj PROJ]
                                [--edition {rubydung,indev,infdev,alpha,beta,release}]

options:
  -h, --help            show this help message and exit
  --proj PROJ           Xbox project dir (default:
                        ../MinecraftAlpha/MinecraftAlpha).
  --edition {rubydung,indev,infdev,alpha,beta,release}
                        Classify files with this edition profile's .filters
                        taxonomy (else default).
)CBX",
         {
             {Kind::HELP, "help", {py::str("-h"), py::str("--help")}, '0', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "proj", {py::str("--proj")}, 'N', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "edition", {py::str("--edition")}, 'N', 's', {py::str("rubydung"), py::str("indev"), py::str("infdev"), py::str("alpha"), py::str("beta"), py::str("release")}, std::nullopt, false, std::nullopt},
         }},
        {"xact", "codebreaker java xact",
         R"CBX(usage: codebreaker java xact [-h] [--run]
                             {dump,build,coverage,match} a [b] [c]
)CBX",
         R"CBX(usage: codebreaker java xact [-h] [--run]
                             {dump,build,coverage,match} a [b] [c]

positional arguments:
  {dump,build,coverage,match}
                        dump <bank> | build <java_dir> <out_dir> | coverage
                        <java_dir> <bank.xsb> | match <java_dir>
                        <resources_dir> [out_dir]
  a                     dump: bank file; build/coverage/match: java source
                        dir.
  b                     build: out dir; coverage: .xsb; match: resources dir.
  c                     match: output dir for convert_audio.bat (default:
                        cwd).

options:
  -h, --help            show this help message and exit
  --run                 build: autorun convert_audio.bat (ffmpeg) then
                        build_banks.bat (XACTBLD) now.
)CBX",
         {
             {Kind::HELP, "help", {py::str("-h"), py::str("--help")}, '0', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "action", {}, 'N', 's', {py::str("dump"), py::str("build"), py::str("coverage"), py::str("match")}, std::nullopt, true, std::nullopt},
             {Kind::STORE, "a", {}, 'N', 's', {}, std::nullopt, true, std::nullopt},
             {Kind::STORE, "b", {}, '?', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "c", {}, '?', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE_TRUE, "run", {py::str("--run")}, '0', 's', {}, std::nullopt, false, std::nullopt},
         }},
        {"assets", "codebreaker java assets",
         R"CBX(usage: codebreaker java assets [-h] [--code CODE] [--only-needed] [--no-rgba]
                               jar content_dir
)CBX",
         R"CBX(usage: codebreaker java assets [-h] [--code CODE] [--only-needed] [--no-rgba]
                               jar content_dir

positional arguments:
  jar            Path to the game jar (e.g. deobfuscated.jar).
  content_dir    Destination Content directory (assets land at Content\<jar-
                 path>).

options:
  -h, --help     show this help message and exit
  --code CODE    Source tree (.java or converted .cpp/.h) to audit coverage
                 against.
  --only-needed  Extract only paths the code references (needs --code);
                 default extracts all assets.
  --no-rgba      Do NOT normalize palettized/grayscale PNGs to RGBA (default
                 normalizes them; the 360 loader needs 32-bit RGBA or textures
                 misrender).
)CBX",
         {
             {Kind::HELP, "help", {py::str("-h"), py::str("--help")}, '0', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "jar", {}, 'N', 's', {}, std::nullopt, true, std::nullopt},
             {Kind::STORE, "content_dir", {}, 'N', 's', {}, std::nullopt, true, std::nullopt},
             {Kind::STORE, "code", {py::str("--code")}, 'N', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE_TRUE, "only_needed", {py::str("--only-needed")}, '0', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE_TRUE, "no_rgba", {py::str("--no-rgba")}, '0', 's', {}, std::nullopt, false, std::nullopt},
         }},
        {"platform-config", "codebreaker java platform-config",
         R"CBX(usage: codebreaker java platform-config [-h] [--platform PLATFORM] [--sdk SDK]
                                        [--layout {default,southpaw,legacy,legacy_southpaw}]
                                        [--invert-y] [--no-invert-y]
                                        [--sensitivity SENSITIVITY]
                                        [--keyboard {xshowkeyboard,osk,stub}]
                                        [--storage {stfs,rawhdd,memory}]
                                        [--bind ACTION=CONTROL] [--list]
                                        out_dir
)CBX",
         R"CBX(usage: codebreaker java platform-config [-h] [--platform PLATFORM] [--sdk SDK]
                                        [--layout {default,southpaw,legacy,legacy_southpaw}]
                                        [--invert-y] [--no-invert-y]
                                        [--sensitivity SENSITIVITY]
                                        [--keyboard {xshowkeyboard,osk,stub}]
                                        [--storage {stfs,rawhdd,memory}]
                                        [--bind ACTION=CONTROL] [--list]
                                        out_dir

positional arguments:
  out_dir               Output dir for
                        ControllerConfig.h/KernelConfig.h/Kernel.cpp.

options:
  -h, --help            show this help message and exit
  --platform PLATFORM   Target platform (default xbox360).
  --sdk SDK             SDK root override (else auto-detect).
  --layout {default,southpaw,legacy,legacy_southpaw}
                        Stick layout preset
                        (default/southpaw/legacy/legacy_southpaw).
  --invert-y            Invert look Y.
  --no-invert-y         Do not invert look Y.
  --sensitivity SENSITIVITY
                        Look sensitivity multiplier.
  --keyboard {xshowkeyboard,osk,stub}
                        Text-input medium.
  --storage {stfs,rawhdd,memory}
                        Save/load medium.
  --bind ACTION=CONTROL
                        Rebind an action, e.g. --bind jump=A (repeatable).
  --list                Print the menu + current choices, do not emit.
)CBX",
         {
             {Kind::HELP, "help", {py::str("-h"), py::str("--help")}, '0', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "out_dir", {}, 'N', 's', {}, std::nullopt, true, std::nullopt},
             {Kind::STORE, "platform", {py::str("--platform")}, 'N', 's', {}, py::str("xbox360"), false, std::nullopt},
             {Kind::STORE, "sdk", {py::str("--sdk")}, 'N', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "layout", {py::str("--layout")}, 'N', 's', {py::str("default"), py::str("southpaw"), py::str("legacy"), py::str("legacy_southpaw")}, std::nullopt, false, std::nullopt},
             {Kind::STORE_TRUE, "invert_y", {py::str("--invert-y")}, '0', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE_TRUE, "no_invert_y", {py::str("--no-invert-y")}, '0', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "sensitivity", {py::str("--sensitivity")}, 'N', 'f', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "keyboard", {py::str("--keyboard")}, 'N', 's', {py::str("xshowkeyboard"), py::str("osk"), py::str("stub")}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "storage", {py::str("--storage")}, 'N', 's', {py::str("stfs"), py::str("rawhdd"), py::str("memory")}, std::nullopt, false, std::nullopt},
             {Kind::APPEND, "bind", {py::str("--bind")}, 'N', 's', {}, std::nullopt, false, py::str("ACTION=CONTROL")},
             {Kind::STORE_TRUE, "list", {py::str("--list")}, '0', 's', {}, std::nullopt, false, std::nullopt},
         }},
        {"achievements", "codebreaker java achievements",
         R"CBX(usage: codebreaker java achievements [-h] [--lang LANG]
                                     [--profile {retail,arcade}]
                                     [--delivery {online,local,off}]
                                     [--title-id TITLE_ID]
                                     [--title-name TITLE_NAME]
                                     [--content-dir CONTENT_DIR]
                                     code_dir out_dir
)CBX",
         R"CBX(usage: codebreaker java achievements [-h] [--lang LANG]
                                     [--profile {retail,arcade}]
                                     [--delivery {online,local,off}]
                                     [--title-id TITLE_ID]
                                     [--title-name TITLE_NAME]
                                     [--content-dir CONTENT_DIR]
                                     code_dir out_dir

positional arguments:
  code_dir              Converted C++ (or Java) tree to scan for achievements.
  out_dir               Output dir for ACHIEVEMENTS.md / achievements.xlast /
                        Achievements.*

options:
  -h, --help            show this help message and exit
  --lang LANG           Optional .lang file for achievement
                        names/descriptions.
  --profile {retail,arcade}
                        Gamerscore budget profile (retail=1000/99,
                        arcade=200/12).
  --delivery {online,local,off}
                        online = write to profile over LIVE; local = track
                        locally; off.
  --title-id TITLE_ID   Self-issued 32-bit Title ID (hex, e.g. FFFE1234). Omit
                        to auto-suggest one.
  --title-name TITLE_NAME
                        Game name used to derive a suggested Title ID (when
                        --title-id is omitted).
  --content-dir CONTENT_DIR
                        Content dir holding the texture atlases (terrain.png,
                        gui/items.png) that the achievement tile art is
                        cropped from. Omit to look next to out_dir
                        (Debug\Content first -- the canonical deploy dir).
)CBX",
         {
             {Kind::HELP, "help", {py::str("-h"), py::str("--help")}, '0', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "code_dir", {}, 'N', 's', {}, std::nullopt, true, std::nullopt},
             {Kind::STORE, "out_dir", {}, 'N', 's', {}, std::nullopt, true, std::nullopt},
             {Kind::STORE, "lang", {py::str("--lang")}, 'N', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "profile", {py::str("--profile")}, 'N', 's', {py::str("retail"), py::str("arcade")}, py::str("retail"), false, std::nullopt},
             {Kind::STORE, "delivery", {py::str("--delivery")}, 'N', 's', {py::str("online"), py::str("local"), py::str("off")}, py::str("online"), false, std::nullopt},
             {Kind::STORE, "title_id", {py::str("--title-id")}, 'N', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "title_name", {py::str("--title-name")}, 'N', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "content_dir", {py::str("--content-dir")}, 'N', 's', {}, std::nullopt, false, std::nullopt},
         }},
        {"ui-mark", "codebreaker java ui-mark",
         R"CBX(usage: codebreaker java ui-mark [-h] [--dry-run] [--extra EXTRA] dir
)CBX",
         R"CBX(usage: codebreaker java ui-mark [-h] [--dry-run] [--extra EXTRA] dir

positional arguments:
  dir            Converted C++ tree to mark.

options:
  -h, --help     show this help message and exit
  --dry-run      Report what would be marked without editing.
  --extra EXTRA  Comma-separated extra UI class names to include.
)CBX",
         {
             {Kind::HELP, "help", {py::str("-h"), py::str("--help")}, '0', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "dir", {}, 'N', 's', {}, std::nullopt, true, std::nullopt},
             {Kind::STORE_TRUE, "dry_run", {py::str("--dry-run")}, '0', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "extra", {py::str("--extra")}, 'N', 's', {}, std::nullopt, false, std::nullopt},
         }},
        {"keybind-mark", "codebreaker java keybind-mark",
         R"CBX(usage: codebreaker java keybind-mark [-h] [--dry-run] dir
)CBX",
         R"CBX(usage: codebreaker java keybind-mark [-h] [--dry-run] dir

positional arguments:
  dir         Converted C++ tree to mark.

options:
  -h, --help  show this help message and exit
  --dry-run   Report what would be tagged without editing.
)CBX",
         {
             {Kind::HELP, "help", {py::str("-h"), py::str("--help")}, '0', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "dir", {}, 'N', 's', {}, std::nullopt, true, std::nullopt},
             {Kind::STORE_TRUE, "dry_run", {py::str("--dry-run")}, '0', 's', {}, std::nullopt, false, std::nullopt},
         }},
        {"bugfix-ledger", "codebreaker java bugfix-ledger",
         R"CBX(usage: codebreaker java bugfix-ledger [-h] [--repo REPO] [--out OUT]
)CBX",
         R"CBX(usage: codebreaker java bugfix-ledger [-h] [--repo REPO] [--out OUT]

options:
  -h, --help   show this help message and exit
  --repo REPO  Converter repo root (default: auto-detect).
  --out OUT    Output path (default: <repo>/BUGFIX_LEDGER.md).
)CBX",
         {
             {Kind::HELP, "help", {py::str("-h"), py::str("--help")}, '0', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "repo", {py::str("--repo")}, 'N', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "out", {py::str("--out")}, 'N', 's', {}, std::nullopt, false, std::nullopt},
         }},
        {"debug", "codebreaker java debug",
         R"CBX(usage: codebreaker java debug [-h] [--off] out_dir
)CBX",
         R"CBX(usage: codebreaker java debug [-h] [--off] out_dir

positional arguments:
  out_dir     Directory to emit the debug files into.

options:
  -h, --help  show this help message and exit
  --off       Bake DEBUG_ENABLED=0 (debug support present but off).
)CBX",
         {
             {Kind::HELP, "help", {py::str("-h"), py::str("--help")}, '0', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "out_dir", {}, 'N', 's', {}, std::nullopt, true, std::nullopt},
             {Kind::STORE_TRUE, "off", {py::str("--off")}, '0', 's', {}, std::nullopt, false, std::nullopt},
         }},
        {"sdk", "codebreaker java sdk",
         R"CBX(usage: codebreaker java sdk [-h] [--sdk SDK] platform
)CBX",
         R"CBX(usage: codebreaker java sdk [-h] [--sdk SDK] platform

positional arguments:
  platform    Platform id (xbox360, ps3, wii, pc_windows).

options:
  -h, --help  show this help message and exit
  --sdk SDK   SDK root override to test.
)CBX",
         {
             {Kind::HELP, "help", {py::str("-h"), py::str("--help")}, '0', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "platform", {}, 'N', 's', {}, std::nullopt, true, std::nullopt},
             {Kind::STORE, "sdk", {py::str("--sdk")}, 'N', 's', {}, std::nullopt, false, std::nullopt},
         }},
        {"project-layout", "codebreaker java project-layout",
         R"CBX(usage: codebreaker java project-layout [-h] [--name NAME] root
)CBX",
         R"CBX(usage: codebreaker java project-layout [-h] [--name NAME] root

positional arguments:
  root         Solution root; its basename is the project name.

options:
  -h, --help   show this help message and exit
  --name NAME  Override the project name (default: root folder name).
)CBX",
         {
             {Kind::HELP, "help", {py::str("-h"), py::str("--help")}, '0', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "root", {}, 'N', 's', {}, std::nullopt, true, std::nullopt},
             {Kind::STORE, "name", {py::str("--name")}, 'N', 's', {}, std::nullopt, false, std::nullopt},
         }},
        {"reflect-factories", "codebreaker java reflect-factories",
         R"CBX(usage: codebreaker java reflect-factories [-h] out_dir
)CBX",
         R"CBX(usage: codebreaker java reflect-factories [-h] out_dir

positional arguments:
  out_dir     Converted output directory to rewrite in place.

options:
  -h, --help  show this help message and exit
)CBX",
         {
             {Kind::HELP, "help", {py::str("-h"), py::str("--help")}, '0', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "out_dir", {}, 'N', 's', {}, std::nullopt, true, std::nullopt},
         }},
        {"sound-fix", "codebreaker java sound-fix",
         R"CBX(usage: codebreaker java sound-fix [-h] out_dir
)CBX",
         R"CBX(usage: codebreaker java sound-fix [-h] out_dir

positional arguments:
  out_dir     Converted output directory to rewrite in place.

options:
  -h, --help  show this help message and exit
)CBX",
         {
             {Kind::HELP, "help", {py::str("-h"), py::str("--help")}, '0', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "out_dir", {}, 'N', 's', {}, std::nullopt, true, std::nullopt},
         }},
        {"controller-nav", "codebreaker java controller-nav",
         R"CBX(usage: codebreaker java controller-nav [-h] out_dir
)CBX",
         R"CBX(usage: codebreaker java controller-nav [-h] out_dir

positional arguments:
  out_dir     Converted output directory to rewrite in place.

options:
  -h, --help  show this help message and exit
)CBX",
         {
             {Kind::HELP, "help", {py::str("-h"), py::str("--help")}, '0', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "out_dir", {}, 'N', 's', {}, std::nullopt, true, std::nullopt},
         }},
        {"render-fix", "codebreaker java render-fix",
         R"CBX(usage: codebreaker java render-fix [-h] out_dir
)CBX",
         R"CBX(usage: codebreaker java render-fix [-h] out_dir

positional arguments:
  out_dir     Converted output directory to rewrite in place.

options:
  -h, --help  show this help message and exit
)CBX",
         {
             {Kind::HELP, "help", {py::str("-h"), py::str("--help")}, '0', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "out_dir", {}, 'N', 's', {}, std::nullopt, true, std::nullopt},
         }},
        {"applet-ping", "codebreaker java applet-ping",
         R"CBX(usage: codebreaker java applet-ping [-h] out_dir
)CBX",
         R"CBX(usage: codebreaker java applet-ping [-h] out_dir

positional arguments:
  out_dir     Converted output directory to rewrite in place.

options:
  -h, --help  show this help message and exit
)CBX",
         {
             {Kind::HELP, "help", {py::str("-h"), py::str("--help")}, '0', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "out_dir", {}, 'N', 's', {}, std::nullopt, true, std::nullopt},
         }},
        {"apimap", "codebreaker java apimap",
         R"CBX(usage: codebreaker java apimap [-h] [--from SRC] [--to DST]
                               [--dialect DIALECT] [--out OUT]
                               {languages,subsystems,list,resolve,coverage}
                               [a]
)CBX",
         R"CBX(usage: codebreaker java apimap [-h] [--from SRC] [--to DST]
                               [--dialect DIALECT] [--out OUT]
                               {languages,subsystems,list,resolve,coverage}
                               [a]

positional arguments:
  {languages,subsystems,list,resolve,coverage}
                        languages | subsystems | list <subsystem> | resolve
                        <symbol> --from L --to L | coverage <java_dir> --from
                        L --to L
  a                     subsystem id / symbol / java_dir depending on action.

options:
  -h, --help            show this help message and exit
  --from SRC            source language/api id.
  --to DST              target language/api id.
  --dialect DIALECT     target dialect (e.g. c++03, c++11).
  --out OUT             coverage: output path (default <java_dir>/APIMAP.md).
)CBX",
         {
             {Kind::HELP, "help", {py::str("-h"), py::str("--help")}, '0', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "action", {}, 'N', 's', {py::str("languages"), py::str("subsystems"), py::str("list"), py::str("resolve"), py::str("coverage")}, std::nullopt, true, std::nullopt},
             {Kind::STORE, "a", {}, '?', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "src", {py::str("--from")}, 'N', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "dst", {py::str("--to")}, 'N', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "dialect", {py::str("--dialect")}, 'N', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "out", {py::str("--out")}, 'N', 's', {}, std::nullopt, false, std::nullopt},
         }},
        {"build", "codebreaker java build",
         R"CBX(usage: codebreaker java build [-h] [--config CONFIG] [--platform PLATFORM]
                              [--log LOG]
                              sln
)CBX",
         R"CBX(usage: codebreaker java build [-h] [--config CONFIG] [--platform PLATFORM]
                              [--log LOG]
                              sln

positional arguments:
  sln                  The .sln to build.

options:
  -h, --help           show this help message and exit
  --config CONFIG
  --platform PLATFORM
  --log LOG            Build log (default: state/java/build.log).
)CBX",
         {
             {Kind::HELP, "help", {py::str("-h"), py::str("--help")}, '0', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "sln", {}, 'N', 's', {}, std::nullopt, true, std::nullopt},
             {Kind::STORE, "config", {py::str("--config")}, 'N', 's', {}, py::str("Release"), false, std::nullopt},
             {Kind::STORE, "platform", {py::str("--platform")}, 'N', 's', {}, py::str("Xbox 360"), false, std::nullopt},
             {Kind::STORE, "log", {py::str("--log")}, 'N', 's', {}, std::nullopt, false, std::nullopt},
         }},
        {"scaffold", "codebreaker java scaffold",
         R"CBX(usage: codebreaker java scaffold [-h] [--language {c,csharp,rust}]
                                 java_dir out_dir
)CBX",
         R"CBX(usage: codebreaker java scaffold [-h] [--language {c,csharp,rust}]
                                 java_dir out_dir

positional arguments:
  java_dir
  out_dir

options:
  -h, --help            show this help message and exit
  --language {c,csharp,rust}
)CBX",
         {
             {Kind::HELP, "help", {py::str("-h"), py::str("--help")}, '0', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "java_dir", {}, 'N', 's', {}, std::nullopt, true, std::nullopt},
             {Kind::STORE, "out_dir", {}, 'N', 's', {}, std::nullopt, true, std::nullopt},
             {Kind::STORE, "language", {py::str("--language")}, 'N', 's', {py::str("c"), py::str("csharp"), py::str("rust")}, py::str("c"), false, std::nullopt},
         }},
        {"scan-project", "codebreaker java scan-project",
         R"CBX(usage: codebreaker java scan-project [-h] folder
)CBX",
         R"CBX(usage: codebreaker java scan-project [-h] folder

positional arguments:
  folder

options:
  -h, --help  show this help message and exit
)CBX",
         {
             {Kind::HELP, "help", {py::str("-h"), py::str("--help")}, '0', 's', {}, std::nullopt, false, std::nullopt},
             {Kind::STORE, "folder", {}, 'N', 's', {}, std::nullopt, true, std::nullopt},
         }},
    };
    return specs;
}
