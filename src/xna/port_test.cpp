// cb-unity: alone
// Tests of cb::xna::port: an XNA game's decompiled C# made into a MonoGame program. What is tested is the text it rewrites (on C# in
// the form ILSpy writes it), the compiler's errors it reads and the lines it edits for them, the XNB reader names, and -- with a
// stand-in `dotnet` (a batch file that answers with a fixed error, then with success) -- the whole of prepare and build over a small
// project: the folder it writes, the project file, the rounds, reports/port.json. The real dotnet, MonoGame and mgfxc are theirs.
//
// Hand-checked: each rewrite's pattern and replacement, the order of the fixes, the keys of port.json, the lines
// `xna port` prints.
//
// Behaviour pinned here:
//   * the files a rewrite or a fix touches keep their line endings and are written without a byte-order mark;
//   * the report names files with `/`; the errors of the report are listed by code in sorted order; reports/port.json is written
//     with nlohmann's dump(1);
//   * a missing XNA Game Studio, ffmpeg or hook is a NotFoundError/UnsupportedError.
#include "xna/port.hpp"

#include <nlohmann/json.hpp>

#include "core/crypto.hpp"
#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"

namespace {

namespace port = cb::xna::port;
namespace stdfs = std::filesystem;
namespace cfs = cb::fs;

bool has(std::string_view hay, std::string_view needle) { return hay.find(needle) != std::string_view::npos; }

std::size_t occurrences(std::string_view hay, std::string_view needle) {
    std::size_t n = 0;
    for (std::size_t at = hay.find(needle); at != std::string_view::npos; at = hay.find(needle, at + needle.size())) ++n;
    return n;
}

// C# as ILSpy writes a game of the XNA 4.0 days
const char* const kGame =
    "using System;\n"
    "using Microsoft.Xna.Framework;\n"
    "using Microsoft.Xna.Framework.Audio;\n"
    "using Microsoft.Xna.Framework.Content;\n"
    "using Microsoft.Xna.Framework.Graphics;\n"
    "using Microsoft.Xna.Framework.Input;\n"
    "\n"
    "namespace Tetris\n"
    "{\n"
    "\tpublic class TetrisGame : Game\n"
    "\t{\n"
    "\t\tprivate GraphicsDeviceManager graphics;\n"
    "\t\tprivate AudioEngine audioEngine;\n"
    "\t\tprivate Microsoft.Xna.Framework.Audio.WaveBank waveBank;\n"
    "\t\tprivate ContentManager extra;\n"
    "\n"
    "\t\tpublic TetrisGame()\n"
    "\t\t{\n"
    "\t\t\tgraphics = new GraphicsDeviceManager(this);\n"
    "\t\t\tbase.Content.RootDirectory = \"Content\";\n"
    "\t\t\textra = new ContentManager(base.Services);\n"
    "\t\t}\n"
    "\n"
    "\t\tprotected override void Update(GameTime gameTime)\n"
    "\t\t{\n"
    "\t\t\tGamePadState state = GamePad.GetState(PlayerIndex.One);\n"
    "\t\t\t((Game)this).Update(gameTime);\n"
    "\t\t\t((IDisposable)this).Dispose();\n"
    "\t\t}\n"
    "\t}\n"
    "\tpublic class Blur : Effect\n"
    "\t{\n"
    "\t\tprivate EffectParameter shaderIndex;\n"
    "\t\tpublic Blur(GraphicsDevice device, byte[] code)\n"
    "\t\t\t: base(device, code)\n"
    "\t\t{\n"
    "\t\t\tshaderIndex = base.Parameters[\"ShaderIndex\"];\n"
    "\t\t\tshaderIndex.SetValue(2);\n"
    "\t\t}\n"
    "\t}\n"
    "\tpublic class Maker\n"
    "\t{\n"
    "\t\tEffect Make(GraphicsDevice d) { return new Effect(d, File.ReadAllBytes(\"a,b.bin\")); }\n"
    "\t}\n"
    "}\n";

// ---- the rewrites --------------------------------------------------------------------------------------------------------------

CB_TEST("xna.port.rewrite_game", [] {
    const port::Rewritten r = port::rewrite(kGame);
    // the runtime is installed first in the Game's constructor, once
    CB_CHECK(has(r.text, "public TetrisGame()\n\t\t{\n\t\tXnaPort.Runtime.Install(this);\n\t\t\tgraphics"));
    CB_CHECK_EQ(occurrences(r.text, "XnaPort.Runtime.Install(this);"), std::size_t{1});
    CB_CHECK(has(r.text, "graphics = XnaPort.Runtime.Graphics(this);"));
    // XACT: aliases after the using line, and the qualified name
    CB_CHECK(has(r.text, "using Microsoft.Xna.Framework.Audio;\nusing AudioEngine = XnaPort.Xact.AudioEngine;\nusing WaveBank = XnaPort.Xact.WaveBank;\n"
                         "using SoundBank = XnaPort.Xact.SoundBank;\nusing Cue = XnaPort.Xact.Cue;\nusing AudioCategory = XnaPort.Xact.AudioCategory;\n"));
    CB_CHECK(has(r.text, "private XnaPort.Xact.WaveBank waveBank;"));
    // the content manager the game makes is the runtime's
    CB_CHECK(has(r.text, "extra = new XnaPort.ContentManager(base.Services);"));
    // a cast of `this` to its base class would call the override again
    CB_CHECK(has(r.text, "base.Update(gameTime);") && has(r.text, "base.Dispose();"));
    CB_CHECK(!has(r.text, "((Game)this)") && !has(r.text, "((IDisposable)this)"));
    // the controller goes through the runtime's
    CB_CHECK(has(r.text, "XnaPort.Input.GetState(PlayerIndex.One)"));
    // an effect made from bytes, and a class deriving from Effect, are given the PC's bytes
    CB_CHECK(has(r.text, ": base(device, XnaPort.Effects.ForPC(code))"));
    CB_CHECK(has(r.text, "new Effect(d, XnaPort.Effects.ForPC(File.ReadAllBytes(\"a,b.bin\")))"));
    // ShaderIndex picks the technique
    CB_CHECK(has(r.text, "XnaPort.Effects.SetShaderIndex(this, shaderIndex, 2);"));
    const std::vector<std::string> expect = {"Install in TetrisGame()", "GraphicsDeviceManager x1", "XACT aliases", "content manager x1",
                                             "base calls x2", "effect bytes x2", "ShaderIndex x1", "GamePad.GetState x1"};
    CB_CHECK((r.changes == expect));
    // a file the rewrites made is not made again
    const port::Rewritten again = port::rewrite(r.text);
    CB_CHECK(!again.changes.empty());   // (the aliases and the effects are found again: Install and ForPC are not repeated)
    CB_CHECK_EQ(occurrences(again.text, "XnaPort.Runtime.Install(this);"), std::size_t{1});
    CB_CHECK_EQ(occurrences(again.text, "XnaPort.Effects.ForPC(XnaPort.Effects.ForPC"), std::size_t{0});
});

CB_TEST("xna.port.rewrite_leaves_what_it_does_not_know", [] {
    const std::string plain = "using System;\nclass Thing\n{\n\tint x = 1;\n}\n";
    const port::Rewritten r = port::rewrite(plain);
    CB_CHECK_EQ(r.text, plain);
    CB_CHECK(r.changes.empty());
    // the game's own types of those names are never aliased away
    const port::Rewritten own = port::rewrite(kGame, {"AudioEngine", "WaveBank", "SoundBank", "Cue", "AudioCategory", "GamePad"});
    CB_CHECK(!has(own.text, "XnaPort.Xact") && !has(own.text, "using AudioEngine"));
    CB_CHECK(has(own.text, "GamePad.GetState(PlayerIndex.One)"));
    CB_CHECK(has(own.text, "Microsoft.Xna.Framework.Audio.WaveBank"));
    // only the constructor of the Game class gets the install
    const port::Rewritten ctor = port::rewrite("public class Main : Microsoft.Xna.Framework.Game\n{\n\tinternal Main(int a) : base()\n\t{\n\t}\n\tvoid Main2() { }\n}\n");
    CB_CHECK(has(ctor.text, "internal Main(int a) : base()\n\t{\n\t\tXnaPort.Runtime.Install(this);"));
    // a game with no constructor of its own: nothing to put it in
    CB_CHECK(!has(port::rewrite("class G : Game\n{\n}\n").text, "Install"));
});

CB_TEST("xna.port.call_arguments", [] {
    const std::string t = "f(a, g(b, c), \"x,y)\", 'c', new[] { 1, 2 }) tail";
    const auto args = port::call_arguments(t, 1);
    CB_CHECK(args.has_value());
    CB_CHECK_EQ(args->spans.size(), std::size_t{5});
    const auto text_of = [&](std::size_t i) { return std::string(cb::text::trim(std::string_view(t).substr(args->spans[i].first, args->spans[i].second - args->spans[i].first))); };
    CB_CHECK_EQ(text_of(1), std::string("g(b, c)"));
    CB_CHECK_EQ(text_of(2), std::string("\"x,y)\""));
    CB_CHECK_EQ(text_of(4), std::string("new[] { 1, 2 }"));
    CB_CHECK_EQ(t.substr(args->after), std::string(" tail"));
    CB_CHECK(port::call_arguments("f()", 1)->spans.empty());      // no arguments
    CB_CHECK(!port::call_arguments("f(a, (b)", 1));               // does not close
});

CB_TEST("xna.port.declared_types_and_extensions", [] {
    cfs::ScopedTempDir dir("cbxnaport");
    cfs::write_text(dir.path() / "a" / "One.cs",
                    "namespace Game.Util\n{\n\tpublic static class RectExt\n\t{\n\t\tpublic static Rectangle Inflate(this Rectangle r, int x, int y) { return r; }\n"
                    "\t\tpublic static T Pick<T>(this List<T> l, int i) { return l[i]; }\n\t\tpublic static int Plain(int a) { return a; }\n\t}\n\tpublic enum Mode { A }\n"
                    "\tinternal interface IThing { }\n\tpublic struct Pt { }\n}\n");
    cfs::write_text(dir.path() / "b" / "Two.cs", "\xEF\xBB\xBF" "class Other { }\n");   // with a byte-order mark
    const auto types = port::declared_types(dir.path());
    CB_CHECK((types == std::set<std::string>{"RectExt", "Mode", "IThing", "Pt", "Other"}));
    const auto ext = port::extension_methods(dir.path());
    CB_CHECK_EQ(ext.size(), std::size_t{2});
    CB_CHECK_EQ(ext.at("Inflate").front().cls, std::string("global::Game.Util.RectExt"));
    CB_CHECK_EQ(ext.at("Inflate").front().receiver_type, std::string("Rectangle"));
    CB_CHECK_EQ(ext.at("Pick").front().receiver_type, std::string("List<T>"));
});

// ---- the compiler's errors -----------------------------------------------------------------------------------------------------

CB_TEST("xna.port.parse_errors", [] {
    const auto e = port::parse_error("C:\\proj\\src\\Game\\Main.cs(12,34): error CS1620: Argument 1 must be passed with the 'out' keyword [C:\\proj\\Port.csproj]");
    CB_CHECK(e.has_value());
    CB_CHECK_EQ(e->file, std::string("C:\\proj\\src\\Game\\Main.cs"));
    CB_CHECK_EQ(e->line, 12);
    CB_CHECK_EQ(e->column, 34);
    CB_CHECK_EQ(e->code, std::string("CS1620"));
    CB_CHECK_EQ(e->text, std::string("Argument 1 must be passed with the 'out' keyword"));   // without the project in brackets
    CB_CHECK(!port::parse_error("Build succeeded."));
    CB_CHECK(!port::parse_error("C:\\x.cs(1,1): warning CS0168: unused"));
    CB_CHECK(!port::parse_error("main.cs(1,1): error CS0001: relative path"));
    const std::string out =
        "  C:\\a.cs(1,2): error CS0001: one [C:\\p.csproj]\r\nnoise\r\nC:\\b.cs(3,4): error CS0002: two [C:\\p.csproj]\r\n"
        "C:\\a.cs(1,2): error CS0001: one [C:\\p.csproj]\r\n";
    const auto all = port::parse_errors(out);
    CB_CHECK_EQ(all.size(), std::size_t{2});   // each once, in the order they came
    CB_CHECK_EQ(all[0].code, std::string("CS0001"));
    CB_CHECK_EQ(all[1].file, std::string("C:\\b.cs"));
});

port::BuildError error_of(const stdfs::path& file, int line, int column, std::string code, std::string text) {
    return {file.string(), line, column, std::move(code), std::move(text)};
}

CB_TEST("xna.port.fix_ref_out", [] {
    cfs::ScopedTempDir dir("cbxnaport");
    const stdfs::path f = dir.path() / "A.cs";
    cfs::write_text(f, "class A\n{\n\tvoid M()\n\t{\n\t\tDo(ref x, out y);\n\t\tOther(z, out w);\n\t}\n}\n");
    // `ref x` where the method wants out, `out w` where it wants ref (the column the compiler names is the keyword's)
    const std::vector<port::BuildError> errors = {
        error_of(f, 5, 6, "CS1620", "Argument 1 must be passed with the 'out' keyword"),
        error_of(f, 6, 12, "CS1620", "Argument 2 must be passed with the 'ref' keyword"),
        error_of(f, 5, 6, "CS0001", "unrelated")};
    CB_CHECK_EQ(port::fix_ref_out(errors), std::size_t{2});
    CB_CHECK(has(cfs::read_text(f), "\t\tDo(out x, out y);\n\t\tOther(z, ref w);\n"));
    CB_CHECK_EQ(port::fix_ref_out(std::vector<port::BuildError>{}), std::size_t{0});
});

CB_TEST("xna.port.fix_access", [] {
    cfs::ScopedTempDir dir("cbxnaport");
    const stdfs::path f = dir.path() / "A.cs";
    cfs::write_text(f, "class A : B\n{\n\tprotected  override void Go() { }\n\tprotected override void Stop() { }\n}\n");
    CB_CHECK_EQ(port::fix_access({error_of(f, 3, 21, "CS0507", "'A.Go()': cannot change access modifiers when overriding 'protected internal' inherited member 'B.Go()'")}),
                std::size_t{1});
    CB_CHECK(has(cfs::read_text(f), "\tprotected internal override void Go() { }\n\tprotected override void Stop() { }\n"));
    // an error of another kind is not this
    CB_CHECK_EQ(port::fix_access({error_of(f, 4, 1, "CS0507", "wants 'public'")}), std::size_t{0});
});

CB_TEST("xna.port.fix_color_and_vertex_binding", [] {
    cfs::ScopedTempDir dir("cbxnaport");
    const stdfs::path f = dir.path() / "A.cs";
    cfs::write_text(f,
                    "class A\n{\n\tvoid M()\n\t{\n\t\tvar c = new Color(r * 2, g, Math.Max(b, 1), 255);\n\t\tvar d = new Color(1, 2, 3);\n\t\tvar e = new Color((int)(1), 2, 3);\n"
                    "\t\tdevice.SetVertexBuffers(new VertexBufferBinding[] { buffer0, new VertexBufferBinding(buffer1) });\n\t}\n}\n");
    const std::vector<port::BuildError> errors = {
        error_of(f, 5, 1, "CS0121", "The call is ambiguous between 'Color.Color(int, int, int, int)' and 'Color.Color(byte, byte, byte, byte)'"),
        error_of(f, 6, 1, "CS0121", "The call is ambiguous between 'Color.Color(int, int, int)' and 'Color.Color(byte, byte, byte)'"),
        error_of(f, 7, 1, "CS0121", "The call is ambiguous between 'Color.Color(int, int, int)' and 'Color.Color(byte, byte, byte)'")};
    CB_CHECK_EQ(port::fix_color(errors), std::size_t{2});   // the third already has its casts
    const std::string t = cfs::read_text(f);
    CB_CHECK(has(t, "new Color((int)(r * 2), (int)(g), (int)(Math.Max(b, 1)), (int)(255))"));
    CB_CHECK(has(t, "new Color((int)(1), (int)(2), (int)(3));"));
    CB_CHECK(has(t, "new Color((int)(1), 2, 3);"));
    CB_CHECK_EQ(port::fix_vertex_binding({error_of(f, 8, 1, "CS0029", "Cannot implicitly convert type 'VertexBuffer' to 'Microsoft.Xna.Framework.Graphics.VertexBufferBinding'")}),
                std::size_t{1});
    CB_CHECK(has(cfs::read_text(f), "new VertexBufferBinding[] { new VertexBufferBinding(buffer0), new VertexBufferBinding(buffer1) }"));
});

CB_TEST("xna.port.fix_named_arguments_and_moved_members", [] {
    cfs::ScopedTempDir dir("cbxnaport");
    const stdfs::path f = dir.path() / "A.cs";
    cfs::write_text(f,
                    "class A\n{\n\tvoid M()\n\t{\n\t\tnew Texture2D(device, 4, 4, mipMap: false);\n\t\tVector3.Lerp(vector1: a, vector2: b, 0.5f);\n"
                    "\t\tdevice.ReferenceStencil = 1;\n\t\tint s = device.ReferenceStencil;\n\t\tvar face = binding.CubeMapFace;\n\t}\n}\n");
    CB_CHECK_EQ(port::fix_named_arguments({error_of(f, 5, 1, "CS1739", "The best overload for 'Texture2D' does not have a parameter named 'mipMap'"),
                                           error_of(f, 6, 1, "CS1739", "The best overload for 'Lerp' does not have a parameter named 'vector1'")}),
                std::size_t{2});
    CB_CHECK_EQ(port::fix_moved_members({error_of(f, 7, 1, "CS1061", "'GraphicsDevice' does not contain a definition for 'ReferenceStencil'"),
                                         error_of(f, 8, 1, "CS1061", "'GraphicsDevice' does not contain a definition for 'ReferenceStencil'"),
                                         error_of(f, 9, 1, "CS1061", "'RenderTargetBinding' does not contain a definition for 'CubeMapFace'")}),
                std::size_t{3});
    const std::string t = cfs::read_text(f);
    CB_CHECK(has(t, "mipmap: false") && has(t, "value1: a") && has(t, "vector2: b"));   // (only the names the compiler named: vector2 stays)
    CB_CHECK(has(t, "XnaPort.Compat.SetReferenceStencil(device, 1);"));
    CB_CHECK(has(t, "int s = XnaPort.Compat.GetReferenceStencil(device);"));
    CB_CHECK(has(t, "var face = ((Microsoft.Xna.Framework.Graphics.CubeMapFace)binding.ArraySlice);"));
});

CB_TEST("xna.port.fix_shadowed_extensions", [] {
    cfs::ScopedTempDir dir("cbxnaport");
    const stdfs::path f = dir.path() / "A.cs";
    cfs::write_text(f,
                    "class A\n{\n\tvoid M()\n\t{\n\t\tRectangle r2 = rect.Inflate(4, 4);\n\t\tRectangle r3 = this.Inflate(1, 1);\n"
                    "\t\tRectangle r4 = area.bounds[0].Inflate(Math.Max(1, 2), 3);\n\t}\n}\n");
    std::map<std::string, std::vector<port::Extension>> ext;
    ext["Inflate"].push_back({"global::Game.Util.RectExt", "Rectangle"});
    CB_CHECK_EQ(port::fix_shadowed_extensions({error_of(f, 5, 1, "CS0029", "Cannot implicitly convert type 'void' to 'Rectangle'"),
                                               error_of(f, 6, 1, "CS0029", "Cannot implicitly convert type 'void' to 'Rectangle'"),
                                               error_of(f, 7, 1, "CS1503", "Argument 1: cannot convert from 'void' to 'int'")},
                                              ext),
                std::size_t{2});
    const std::string t = cfs::read_text(f);
    CB_CHECK(has(t, "Rectangle r2 = global::Game.Util.RectExt.Inflate(rect, 4, 4);"));
    CB_CHECK(has(t, "Rectangle r3 = this.Inflate(1, 1);"));   // `this` is the game's own method
    CB_CHECK(has(t, "global::Game.Util.RectExt.Inflate(area.bounds[0], Math.Max(1, 2), 3)"));
    CB_CHECK_EQ(port::fix_shadowed_extensions({}, {}), std::size_t{0});
});

// ---- the XNB reader names -------------------------------------------------------------------------------------------------------

CB_TEST("xna.port.reader_names", [] {
    const std::string prog = "Tetris";
    CB_CHECK_EQ(port::port_reader_name("Microsoft.Xna.Framework.Content.Texture2DReader, Microsoft.Xna.Framework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=842cf8be1de50553", prog),
                std::string("Microsoft.Xna.Framework.Content.Texture2DReader, Microsoft.Xna.Framework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=842cf8be1de50553"));
    // a type of an assembly MonoGame does not have names the program's own (the stubs)
    CB_CHECK_EQ(port::port_reader_name("X.GamerReader, Microsoft.Xna.Framework.GamerServices, Version=4.0.0.0, Culture=neutral, PublicKeyToken=842cf8be1de50553", prog),
                std::string("X.GamerReader, Tetris"));
    CB_CHECK_EQ(port::port_reader_name("Y.NetReader, Microsoft.Xna.Framework.Net", prog), std::string("Y.NetReader, Tetris"));
    // XNA's reflective reader is the runtime's
    CB_CHECK_EQ(port::port_reader_name("Microsoft.Xna.Framework.Content.ReflectiveReader`1[[Game.Level, Game]]", prog),
                std::string("XnaPort.XnaReflectiveReader`1[[Game.Level, Game]], Tetris"));
    // the program's name goes in as it is, `$` and all
    CB_CHECK_EQ(port::port_reader_name("Z.AvatarReader, Microsoft.Xna.Framework.Avatar", "A$1"), std::string("Z.AvatarReader, A$1"));
});

CB_TEST("xna.port.reader_names_of_a_folder_are_stamped", [] {
    cfs::ScopedTempDir dir("cbxnaport");
    const stdfs::path game = dir.path() / "game";
    cfs::write_text(game / "Content" / "a.xnb", "XNB-with-readers");
    cfs::write_text(game / "Content" / "b.xnb", "XNB-no-readers");
    cfs::write_text(game / "Content" / "c.xnb", "not an xnb");
    cfs::write_text(game / "Content" / "d.txt", "XNB but not named so");
    port::Hooks hooks;
    std::size_t asked = 0;
    hooks.rename_readers = [&](cb::ByteView xnb, const std::function<std::string(const std::string&)>& rename) -> std::optional<cb::Bytes> {
        ++asked;
        const std::string s(cb::as_text(xnb));
        if (s != "XNB-with-readers") return std::nullopt;
        const std::string renamed = "XNB:" + rename("X.R, Microsoft.Xna.Framework.Net");
        return cb::to_bytes(renamed);
    };
    CB_CHECK_EQ(port::port_reader_names(game, "Prog", hooks), std::size_t{1});
    CB_CHECK_EQ(asked, std::size_t{2});   // the two that begin with XNB
    CB_CHECK_EQ(cfs::read_text(game / "Content" / "a.xnb"), std::string("XNB:X.R, Prog"));
    CB_CHECK_EQ(cfs::read_text(game / "Content" / "b.xnb"), std::string("XNB-no-readers"));
    // once: the stamp beside the folder says so
    CB_CHECK(stdfs::is_regular_file(dir.path() / "reader-names.stamp"));
    CB_CHECK_EQ(port::port_reader_names(game, "Prog", hooks), std::size_t{0});
    CB_CHECK_EQ(asked, std::size_t{2});
    // an .xnb and no converter: said so
    cfs::ScopedTempDir other("cbxnaport");
    cfs::write_text(other.path() / "game" / "a.xnb", "XNB");
    CB_CHECK_THROWS_AS(cb::UnsupportedError, port::port_reader_names(other.path() / "game", "Prog", port::Hooks{}));
});

// ---- content ----------------------------------------------------------------------------------------------------------------------

CB_TEST("xna.port.convert_xact", [] {
    cfs::ScopedTempDir dir("cbxnaport");
    cfs::write_text(dir.path() / "game" / "Sound" / "Waves.xwb", "wave bank");
    cfs::write_text(dir.path() / "game" / "Sound" / "Sounds.xsb", "sound bank");
    cfs::write_text(dir.path() / "game" / "Other.xsb", "sound bank");
    port::Hooks hooks;
    int decoded = 0;
    hooks.decode_wave_bank = [&](const stdfs::path& bank, const stdfs::path& out) {
        ++decoded;
        cfs::write_text(out / "stream_000.wav", "RIFF " + bank.filename().string());
    };
    hooks.wave_bank_name = [](const stdfs::path&) { return std::string("Wave Bank"); };
    hooks.sound_bank_cues = [](const stdfs::path& bank) { return "{\"bank\": \"" + bank.filename().string() + "\"}"; };
    const stdfs::path converted = dir.path() / "converted";
    const auto [waves, sounds] = port::convert_xact(dir.path() / "game", converted, hooks);
    CB_CHECK_EQ(waves, std::size_t{1});
    CB_CHECK_EQ(sounds, std::size_t{2});
    CB_CHECK_EQ(cfs::read_text(converted / "Sound" / "Waves" / "stream_000.wav"), std::string("RIFF Waves.xwb"));
    CB_CHECK_EQ(cfs::read_text(converted / "Sound" / "Waves" / "bank.txt"), std::string("Wave Bank"));
    CB_CHECK_EQ(cfs::read_text(converted / "Sound" / "Sounds.cues.json"), std::string("{\"bank\": \"Sounds.xsb\"}"));
    CB_CHECK(stdfs::is_regular_file(converted / "Other.cues.json"));
    // a bank already decoded is not decoded again
    port::convert_xact(dir.path() / "game", converted, hooks);
    CB_CHECK_EQ(decoded, 1);
    CB_CHECK_THROWS_AS(cb::UnsupportedError, port::convert_xact(dir.path() / "game", dir.path() / "other", port::Hooks{}));
});

CB_TEST("xna.port.compile_effects_uses_what_was_compiled_before", [] {
    cfs::ScopedTempDir dir("cbxnaport");
    port::Tools tools;
    tools.state = dir.path() / "state";
    const std::string source = "// sha1 0123456789abcdef\nfloat4 main() : COLOR { return 1; }\n";
    const stdfs::path converted = dir.path() / "converted";
    cfs::write_text(converted / "fx" / "blur.fx", source);
    cfs::write_text(converted / "fx" / "no_sha.fx", "float4 main() : COLOR { return 2; }\n");   // not a recovered effect: left alone
    // an effect compiled before is kept under the digest of its source
    const std::string key = [&] {
        std::string out;
        for (const std::uint8_t b : cb::crypto::sha1(cb::as_bytes(source))) out += std::format("{:02x}", static_cast<unsigned>(b));
        return out;
    }();
    cfs::write_text(tools.state / "xna" / "mgfxo" / (key + ".mgfxo"), "MGFXO bytes");
    const auto report = port::compile_effects(converted, dir.path() / "shaders", tools);
    CB_CHECK_EQ(report.compiled, std::size_t{1});
    CB_CHECK(report.failed.empty());
    CB_CHECK_EQ(cfs::read_text(dir.path() / "shaders" / "0123456789abcdef.mgfxo"), std::string("MGFXO bytes"));
    // one never compiled is built by mgfxc when it is installed (and fails on this, which is no effect), or is a failure said by file
    cfs::write_text(converted / "fx" / "new.fx", "// sha1 aaaa\nnothing\n");
    const auto failed = port::compile_effects(converted, dir.path() / "shaders", tools);
    CB_CHECK_EQ(failed.compiled, std::size_t{1});
    CB_CHECK_EQ(failed.failed.size(), std::size_t{1});
    CB_CHECK(failed.failed.contains("fx/new.fx"));
});

// ---- a whole project, with a stand-in dotnet ------------------------------------------------------------------------------------

cb::Bytes dotnet_pe() {
    cb::Bytes b(0x40 + 24 + 112 + 16 * 8, 0);
    b[0] = 'M';
    b[1] = 'Z';
    b[0x3C] = 0x40;
    const std::size_t nt = 0x40;
    b[nt] = 'P';
    b[nt + 1] = 'E';
    b[nt + 24] = 0x0B;
    b[nt + 25] = 0x01;
    b[nt + 24 + 92] = 16;
    const std::size_t clr = nt + 24 + 96 + 8 * 14;
    b[clr + 1] = 0x20;
    b[clr + 4] = 0x48;
    return b;
}

// A converted XNA project on disk: a game assembly, its content, and a decompiled project of C#.
struct PortProject {
    cfs::ScopedTempDir dir{"cbxnaport"};
    port::Tools tools;
    port::Hooks hooks;
    stdfs::path root() const { return dir.path() / "Converted" / "tetris"; }
    PortProject() {
        cfs::write_file(root() / "assets" / "game" / "Tetris Game.exe", dotnet_pe());
        cfs::write_text(root() / "assets" / "game" / "Content" / "hero.bin", "x");
        cfs::write_text(root() / "assets" / "content" / "Content" / "hero.png", "png");
        cfs::write_text(root() / "src" / "Tetris" / "Game1.cs", std::string("\xEF\xBB\xBF") + kGame);
        cfs::write_text(root() / "src" / "Tetris" / "Tetris.csproj", "<Project />");
        cfs::write_text(root() / "src" / "Tetris" / "obj" / "junk.cs", "class Junk { }");
        cfs::write_text(root() / "src" / "Tetris" / "Util.cs", "class Util { void M() { Do(ref x); } }\n");
        cfs::write_text(root() / "project.json", "{}");
        tools.runtime_dir = dir.path() / "runtime";
        cfs::write_text(tools.runtime_dir / "Runtime.cs", "// the runtime\n");
        cfs::write_text(tools.runtime_dir / "sub" / "Nested.cs", "// not copied: only the folder's own files\n");
        tools.state = dir.path() / "state";
        tools.overlays = dir.path() / "overlays";
        hooks.stubs = [](const stdfs::path& out) { cfs::write_text(out / "Avatar.stubs.cs", "// stub\n"); };
    }
};

CB_TEST("xna.port.prepare_writes_the_project", [] {
    PortProject p;
    // a hand overlay of the game's own: a file, a replaced folder, a dropped file
    const stdfs::path ov = p.dir.path() / "overlays" / "tetris";
    cfs::write_text(ov / "Port" / "Hand.cs", "// by hand\n");
    cfs::write_text(ov / "overlay.json", "{\"drop\": [\"XnaPort/Runtime.cs\"], \"replace\": [\"src/Tetris/Util.cs\"]}");
    const cb::base::converted::Project project(p.root());
    const port::Prepared prepared = port::prepare(project, p.tools, p.hooks);
    CB_CHECK_EQ(prepared.name, std::string("TetrisGame"));   // the .exe's name without its spaces
    const stdfs::path port_dir = p.root() / "build" / "port";
    CB_CHECK(prepared.dir == port_dir);
    // the game's C# rewritten, the projects' own .csproj and obj/ left out
    const std::string game = cfs::read_text(port_dir / "src" / "Tetris" / "Game1.cs");
    CB_CHECK(has(game, "XnaPort.Runtime.Install(this);") && !game.starts_with("\xEF\xBB\xBF"));
    CB_CHECK(!stdfs::exists(port_dir / "src" / "Tetris" / "Tetris.csproj"));
    CB_CHECK(!stdfs::exists(port_dir / "src" / "Tetris" / "obj"));
    CB_CHECK(prepared.report.rewrites.contains("src/Tetris/Game1.cs"));
    CB_CHECK(!prepared.report.rewrites.contains("src/Tetris/Util.cs"));
    // the runtime (its own folder's files), the stubs, the game's files beside the program, the converted content
    CB_CHECK(!stdfs::exists(port_dir / "XnaPort" / "Runtime.cs"));   // the overlay dropped it
    CB_CHECK(!stdfs::exists(port_dir / "XnaPort" / "Nested.cs"));
    CB_CHECK(stdfs::is_regular_file(port_dir / "Stubs" / "Avatar.stubs.cs"));
    CB_CHECK(stdfs::is_regular_file(port_dir / "game" / "Content" / "hero.bin"));
    CB_CHECK(stdfs::is_regular_file(port_dir / "ContentConverted" / "Content" / "hero.png"));
    CB_CHECK(stdfs::is_regular_file(port_dir / "Port" / "Hand.cs"));
    CB_CHECK(!stdfs::exists(port_dir / "overlay.json"));
    CB_CHECK(!stdfs::exists(port_dir / "src" / "Tetris" / "Util.cs"));   // replaced
    CB_CHECK_EQ(prepared.report.overlay.value_or(""), cfs::to_text(ov));
    // the project file
    const std::string csproj = cfs::read_text(port_dir / "TetrisGame.Port.csproj");
    CB_CHECK(has(csproj, "<AssemblyName>TetrisGame</AssemblyName>") && has(csproj, "Include=\"MonoGame.Framework.DesktopGL\" Version=\"3.8.1.303\""));
    CB_CHECK(has(csproj, "Link=\"%(RecursiveDir)%(Filename)%(Extension)\""));
    // a game with no assemblies yet: said so
    cfs::write_text(p.dir.path() / "Converted" / "empty" / "project.json", "{}");
    CB_CHECK_THROWS_AS(cb::UsageError, port::prepare(cb::base::converted::Project(p.dir.path() / "Converted" / "empty"), p.tools, p.hooks));
});

// A dotnet that fails the first build with one error the port knows the fix for, and passes the second.
void write_fake_dotnet(const stdfs::path& folder, const stdfs::path& error_file) {
    cfs::write_text(folder / "dotnet.cmd",
                    "@echo off\r\n"
                    "if exist \"%~dp0second.flag\" goto ok\r\n"
                    "echo.>\"%~dp0second.flag\"\r\n"
                    "echo " + error_file.string() + "(1,28): error CS1620: Argument 1 must be passed with the 'out' keyword [x.csproj]\r\n"
                    "exit /b 1\r\n"
                    ":ok\r\n"
                    "echo Build succeeded.\r\n"
                    "exit /b 0\r\n");
}

CB_TEST("xna.port.build_fixes_and_builds_again", [] {
    CB_WINDOWS_ONLY("its stand-in build tools are .cmd files");
    PortProject p;
    const stdfs::path util = p.root() / "build" / "port" / "src" / "Tetris" / "Util.cs";
    const stdfs::path bin = p.dir.path() / "bin";
    write_fake_dotnet(bin, util);
    const std::string old_path = cb::fs::get_env("PATH").value_or("");
    cfs::ScopedEnv path("PATH", bin.string() + ";" + old_path);
    const cb::base::converted::Project project(p.root());
    const port::BuildResult r = port::build(project, p.tools, p.hooks);
    CB_CHECK(r.ok);
    CB_CHECK(r.exe.has_value() && r.exe->filename() == "TetrisGame.exe");
    CB_CHECK_EQ(r.rounds.size(), std::size_t{2});
    CB_CHECK_EQ(r.rounds[0].errors, std::size_t{1});
    CB_CHECK_EQ(r.rounds[0].fixed, std::size_t{1});   // `ref x` was `out x`'s... CS1620 flipped it
    CB_CHECK_EQ(r.rounds[1].errors, std::size_t{0});
    CB_CHECK(has(cfs::read_text(util), "Do(out x)"));
    // the report
    const nlohmann::json j = nlohmann::json::parse(cfs::read_text(p.root() / "reports" / "port.json"));
    CB_CHECK(j["ok"].get<bool>() && j["errors"].empty());
    CB_CHECK_EQ(j["rounds"].size(), std::size_t{2});
    CB_CHECK(j["rewrites"].contains("src/Tetris/Game1.cs"));
    CB_CHECK_EQ(j["content"]["wave banks"].get<int>(), 0);
    CB_CHECK(j["content"]["effects"]["failed"].empty());
});

CB_TEST("xna.port.build_that_cannot_be_fixed_lists_the_errors", [] {
    CB_WINDOWS_ONLY("its stand-in build tools are .cmd files");
    PortProject p;
    const stdfs::path bin = p.dir.path() / "bin";
    cfs::write_text(bin / "dotnet.cmd",
                    "@echo off\r\n"
                    "echo C:\\p\\A.cs(1,1): error CS0103: The name 'x' does not exist [x.csproj]\r\n"
                    "echo C:\\p\\B.cs(2,2): error CS0103: The name 'y' does not exist [x.csproj]\r\n"
                    "echo C:\\p\\B.cs(3,3): error CS0246: The type 'Z' was not found [x.csproj]\r\n"
                    "exit /b 1\r\n");
    cfs::ScopedEnv path("PATH", bin.string() + ";" + cb::fs::get_env("PATH").value_or(""));
    const cb::base::converted::Project project(p.root());
    const port::BuildResult r = port::build(project, p.tools, p.hooks);
    CB_CHECK(!r.ok && !r.exe);
    CB_CHECK_EQ(r.rounds.size(), std::size_t{1});   // nothing was fixed: it stops
    CB_CHECK_EQ(r.errors.at("CS0103"), std::size_t{2});
    CB_CHECK_EQ(r.errors.at("CS0246"), std::size_t{1});
    CB_CHECK_EQ(r.first.size(), std::size_t{3});
    CB_CHECK_EQ(r.first[0], std::string("C:/p/A.cs(1): CS0103 The name 'x' does not exist"));
    CB_CHECK(r.build_output_tail.empty());   // the errors are there to read
    // no error line at all: the tail of the output instead
    cfs::write_text(bin / "dotnet.cmd", "@echo off\r\necho MSBUILD : error MSB1009: Project file does not exist.\r\nexit /b 1\r\n");
    const port::BuildResult t = port::build(project, p.tools, p.hooks);
    CB_CHECK(!t.ok && t.errors.empty() && has(t.build_output_tail, "MSB1009"));
});

CB_TEST("xna.port.main_prints_what_happened", [] {
    CB_WINDOWS_ONLY("its stand-in build tools are .cmd files");
    PortProject p;
    const stdfs::path bin = p.dir.path() / "bin";
    cfs::write_text(bin / "dotnet.cmd", "@echo off\r\necho C:\\p\\A.cs(1,1): error CS0103: The name 'x' does not exist [x.csproj]\r\nexit /b 1\r\n");
    cfs::ScopedEnv path("PATH", bin.string() + ";" + cb::fs::get_env("PATH").value_or(""));
    const cb::base::converted::Store store(p.dir.path() / "Converted");
    {
        cb::testing::CapturedStd cap;
        const std::vector<std::string> args = {"tetris"};
        CB_CHECK_EQ(port::main(args, store, p.tools, p.hooks), 1);
        const std::string out = cap.out();
        CB_CHECK(has(out, "not built: 1 errors (CS0103 1); reports/port.json lists them"));
        CB_CHECK(has(out, "  C:/p/A.cs(1): CS0103 The name 'x' does not exist"));
    }
    {
        cb::testing::CapturedStd cap;
        CB_CHECK_EQ(port::main(std::vector<std::string>{"nothing-here"}, store, p.tools, p.hooks), 2);
        CB_CHECK(has(cap.out(), "xna port: nothing-here is not a Converted project"));
    }
    {
        cb::testing::CapturedStd cap;
        CB_CHECK_EQ(port::main(std::vector<std::string>{}, store, p.tools, p.hooks), 2);
        CB_CHECK_EQ(port::main(std::vector<std::string>{"--help"}, store, p.tools, p.hooks), 0);
    }
    // a build that fails is a step of the project, with its count
    const cb::base::converted::Project project(p.root());
    CB_CHECK(has(cfs::read_text(p.root() / "project.json"), "1 errors left"));
});

CB_TEST("xna.port.run_for_ends_a_program_that_does_not_stop", [] {
    CB_WINDOWS_ONLY("its stand-in build tools are .cmd files");
    cfs::ScopedTempDir dir("cbxnaport");
    cfs::write_text(dir.path() / "quick.cmd", "@echo off\r\necho started\r\necho written> xnaport.log\r\nexit /b 3\r\n");
    const port::RunResult quick = port::run_for(dir.path() / "quick.cmd", std::chrono::seconds(30));
    CB_CHECK(quick.exit == std::optional<int>(3));
    CB_CHECK(has(quick.output, "started"));
    CB_CHECK(has(quick.log, "written"));
    // a program that outlasts the time is ended, and says it was still running
    cfs::write_text(dir.path() / "slow.cmd", "@echo off\r\necho going\r\nping -n 30 127.0.0.1 >nul\r\n");
    const auto t0 = std::chrono::steady_clock::now();
    const port::RunResult slow = port::run_for(dir.path() / "slow.cmd", std::chrono::seconds(2));
    CB_CHECK(!slow.exit.has_value());
    CB_CHECK(std::chrono::steady_clock::now() - t0 < std::chrono::seconds(20));
});

CB_TEST("xna.port.check_content_reads_what_the_program_wrote", [] {
    CB_WINDOWS_ONLY("its stand-in build tools are .cmd files");
    cfs::ScopedTempDir dir("cbxnaport");
    cfs::write_text(dir.path() / "game.cmd",
                    "@echo off\r\nif \"%XNAPORT_CHECK_CONTENT%\"==\"1\" echo {\"a.xnb\": \"Texture2D\", \"b.xnb\": \"error: boom\"}> content_check.json\r\nexit /b 0\r\n");
    const auto checked = port::check_content(dir.path() / "game.cmd", std::chrono::seconds(30));
    CB_CHECK(checked.has_value());
    CB_CHECK_EQ(checked->at("a.xnb"), std::string("Texture2D"));
    CB_CHECK_EQ(checked->at("b.xnb"), std::string("error: boom"));
    // a program that wrote nothing
    cfs::write_text(dir.path() / "quiet.cmd", "@echo off\r\nexit /b 0\r\n");
    CB_CHECK(!port::check_content(dir.path() / "quiet.cmd", std::chrono::seconds(30)));
});

}  // namespace
