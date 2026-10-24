// XnaPort.cs -- CodeBreaker's XNA runtime on MonoGame: what an Xbox 360 XNA game needs to run on a PC that MonoGame
// does not give it. The `xna` lane's port step puts these files beside the game's decompiled source.
//
//   Runtime.Install(game)       first thing in the game's constructor (put there by the port step): the game's
//                               Content becomes a ContentManager that reads what CodeBreaker converted
//   Runtime.Graphics(game)      in place of `new GraphicsDeviceManager(game)`: the HiDef profile the Xbox 360 had
//   XNAPORT_EXCEPTIONS=1        every exception thrown is logged with its stack (a game's own crash screen hides them)
//   XNAPORT_SCREENSHOT=<s,...>  the frame at those seconds as screenshot_<s>.png, then the program ends
//   XNAPORT_CHECK_CONTENT=1     the program loads every .xnb under Content/ through the runtime instead of playing,
//                               writes content_check.json beside itself ({asset: type or "error: why"}) and exits
//   ContentManager              Load<T>: a converted file when there is one (ContentConverted/: textures and fonts
//                               as PNG and JSON, sounds as WAV), else the game's own .xnb (MonoGame reads Xbox XNB
//                               headers; its stream is little-endian, and XboxContent's readers turn the
//                               big-endian buffers and pixels in it around; an Effect is the one CodeBreaker
//                               recovered as HLSL, see XboxContent.cs).
using System;
using System.Collections.Generic;
using System.IO;
using System.Text.Json;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;

namespace XnaPort
{
    public static class Runtime
    {
        public static Game Game { get; private set; }
        public static string BaseDirectory => AppContext.BaseDirectory;

        public static void Install(Game game)
        {
            Game = game;
            XboxContent.Register();
            game.Content = new ContentManager(game.Services, "Content");
            game.IsMouseVisible = true;
            // a type an .xnb names by the assembly it was built in (a game's own "Hazzah Engine", XNA's Avatar or
            // GamerServices, which the stubs give) is in this program
            AppDomain.CurrentDomain.AssemblyResolve += (s, e) => System.Reflection.Assembly.GetEntryAssembly();
            if (Environment.GetEnvironmentVariable("XNAPORT_CHECK_CONTENT") == "1")
                game.Components.Add(new ContentCheck(game));
            // XNAPORT_EXCEPTIONS=1: every exception thrown, with where, in the log -- a game that catches its own
            // (a crash screen) says what went wrong
            if (Environment.GetEnvironmentVariable("XNAPORT_EXCEPTIONS") == "1")
            {
                int logged = 0;
                AppDomain.CurrentDomain.FirstChanceException += (s, e) =>
                {
                    if (logged++ < 50)
                        Log("XnaPort: exception " + e.Exception.GetType().FullName + ": " + e.Exception.Message + "\n" +
                            new System.Diagnostics.StackTrace(1, false));
                };
            }
            if (!string.IsNullOrWhiteSpace(Environment.GetEnvironmentVariable("XNAPORT_SCREENSHOT")))
                game.Components.Add(new Screenshot(game, Environment.GetEnvironmentVariable("XNAPORT_SCREENSHOT")));
            Log("XnaPort: runtime installed for " + game.GetType().FullName);
        }

        public static GraphicsDeviceManager Graphics(Game game)
        {
            var g = new GraphicsDeviceManager(game) { GraphicsProfile = GraphicsProfile.HiDef };
            g.PreparingDeviceSettings += (s, e) => e.GraphicsDeviceInformation.GraphicsProfile = GraphicsProfile.HiDef;
            return g;
        }

        public static void Log(string line)
        {
            try { File.AppendAllText(Path.Combine(BaseDirectory, "xnaport.log"), DateTime.Now.ToString("HH:mm:ss.fff ") + line + Environment.NewLine); }
            catch (IOException) { }
            Console.WriteLine(line);
        }
    }

    // Every .xnb under Content/ loaded as the game would load it (by what its own reader makes), on the first frame.
    // XNAPORT_SCREENSHOT=<seconds>[,<seconds>...]: the frame drawn that long after the start (by the clock), as
    // screenshot.png -- screenshot_<seconds>.png when there are several -- beside the program; after the last the
    // program ends.
    class Screenshot : DrawableGameComponent
    {
        readonly List<double> times = new List<double>();
        readonly System.Diagnostics.Stopwatch clock = System.Diagnostics.Stopwatch.StartNew();
        readonly bool several;

        public Screenshot(Game game, string seconds) : base(game)
        {
            foreach (string s in seconds.Split(','))
                if (double.TryParse(s, System.Globalization.NumberStyles.Float, System.Globalization.CultureInfo.InvariantCulture, out double t))
                    times.Add(t);
            times.Sort();
            several = times.Count > 1;
            DrawOrder = int.MaxValue;
        }

        public override void Draw(GameTime gameTime)
        {
            if (times.Count == 0 || clock.Elapsed.TotalSeconds < times[0])
                return;
            string file = several ? "screenshot_" + times[0].ToString(System.Globalization.CultureInfo.InvariantCulture) + ".png" : "screenshot.png";
            times.RemoveAt(0);
            var pp = GraphicsDevice.PresentationParameters;
            var pixels = new Color[pp.BackBufferWidth * pp.BackBufferHeight];
            GraphicsDevice.GetBackBufferData(pixels);
            for (int i = 0; i < pixels.Length; i++)
                pixels[i].A = 255;
            using (var t = new Texture2D(GraphicsDevice, pp.BackBufferWidth, pp.BackBufferHeight))
            using (var f = File.Create(Path.Combine(Runtime.BaseDirectory, file)))
            {
                t.SetData(pixels);
                t.SaveAsPng(f, t.Width, t.Height);
            }
            Runtime.Log("XnaPort: " + file + " at " + clock.Elapsed.TotalSeconds.ToString("0.0") + " s");
            if (times.Count == 0)
                Game.Exit();
        }
    }

    class ContentCheck : GameComponent
    {
        public ContentCheck(Game game) : base(game) { }

        public override void Update(GameTime gameTime)
        {
            string root = Path.Combine(Runtime.BaseDirectory, "Content");
            var results = new SortedDictionary<string, string>(StringComparer.OrdinalIgnoreCase);
            int ok = 0;
            foreach (string f in Directory.EnumerateFiles(root, "*.xnb", SearchOption.AllDirectories))
            {
                string name = Path.ChangeExtension(Path.GetRelativePath(root, f), null).Replace('\\', '/');
                var head = new byte[3];
                using (var h = File.OpenRead(f))
                    h.ReadExactly(head, 0, Math.Min(3, (int)h.Length));
                if (new FileInfo(f).Length == 0)
                {
                    results[name] = "empty: the package has the file with nothing in it";
                    continue;
                }
                if (head[0] != 'X' || head[1] != 'N' || head[2] != 'B')
                {
                    results[name] = "not an XNB: a file the game reads itself";
                    continue;
                }
                using var content = new ContentManager(Game.Services, "Content");
                string converted = Path.Combine(Runtime.BaseDirectory, "ContentConverted", "Content", name);
                try
                {
                    // what was converted loads as the game loads it: by its type, from the converted file
                    object asset = File.Exists(converted + ".wav") ? content.Load<Microsoft.Xna.Framework.Audio.SoundEffect>(name)
                                 : File.Exists(converted + ".json") ? content.Load<SpriteFont>(name)
                                 : File.Exists(converted + ".png") ? content.Load<Texture2D>(name)
                                 : content.Load<object>(name);
                    string bad = asset is Model model ? Geometry(model) : null;
                    results[name] = (asset?.GetType().FullName ?? "null") + (bad == null ? "" : " -- " + bad);
                    ok++;
                }
                catch (Exception e)
                {
                    var inner = e;
                    while (inner.InnerException != null) inner = inner.InnerException;
                    results[name] = "error: " + inner.GetType().Name + ": " + inner.Message;
                    Runtime.Log("XnaPort: content check: " + name + ": " + e);
                }
            }
            File.WriteAllText(Path.Combine(Runtime.BaseDirectory, "content_check.json"),
                              JsonSerializer.Serialize(results, new JsonSerializerOptions { WriteIndented = true }));
            Runtime.Log("XnaPort: content check: " + ok + " loaded of " + results.Count);
            Game.Exit();
        }

        // A model's buffers as the GPU will read them: positions finite and within reach, indices inside their
        // vertex range -- what a buffer left in the Xbox's byte order fails. -> why not, or null
        static string Geometry(Model model)
        {
            foreach (ModelMesh mesh in model.Meshes)
                foreach (ModelMeshPart part in mesh.MeshParts)
                {
                    VertexBuffer vb = part.VertexBuffer;
                    var position = Array.Find(vb.VertexDeclaration.GetVertexElements(),
                                              e => e.VertexElementUsage == VertexElementUsage.Position && e.VertexElementFormat == VertexElementFormat.Vector3);
                    if (vb.VertexCount == 0 || position.VertexElementFormat != VertexElementFormat.Vector3)
                        continue;
                    var bytes = new byte[vb.VertexCount * vb.VertexDeclaration.VertexStride];
                    vb.GetData(bytes);
                    // big-endian floats read little-endian come out huge, not finite or vanishingly small -- most of
                    // them; a modeller's own leaves a few (cos 90 degrees: 3e-17)
                    int odd = 0;
                    for (int v = 0; v < vb.VertexCount; v++)
                        for (int c = 0; c < 3; c++)
                        {
                            float x = BitConverter.ToSingle(bytes, v * vb.VertexDeclaration.VertexStride + position.Offset + c * 4);
                            if (!float.IsFinite(x) || Math.Abs(x) > 1e6f || (x != 0 && Math.Abs(x) < 1e-12f))
                                odd++;
                        }
                    if (odd * 10 > vb.VertexCount * 3)
                        return odd + " of " + mesh.Name + "'s " + vb.VertexCount * 3 + " position values are not plausible";
                    IndexBuffer ib = part.IndexBuffer;
                    int count = part.PrimitiveCount * 3;
                    int[] index = new int[count];
                    if (ib.IndexElementSize == IndexElementSize.SixteenBits)
                    {
                        var shorts = new ushort[count];
                        ib.GetData(part.StartIndex * 2, shorts, 0, count);
                        for (int i = 0; i < count; i++) index[i] = shorts[i];
                    }
                    else
                        ib.GetData(part.StartIndex * 4, index, 0, count);
                    foreach (int i in index)
                        if (i + part.VertexOffset >= vb.VertexCount)
                            return "index " + i + " in " + mesh.Name + " is past its " + vb.VertexCount + " vertices";
                }
            return null;
        }
    }

    public class ContentManager : Microsoft.Xna.Framework.Content.ContentManager
    {
        readonly Dictionary<string, object> loaded = new Dictionary<string, object>(StringComparer.OrdinalIgnoreCase);

        public ContentManager(IServiceProvider services) : base(services) { }
        public ContentManager(IServiceProvider services, string root) : base(services, root) { }

        GraphicsDevice Device => ((IGraphicsDeviceService)ServiceProvider.GetService(typeof(IGraphicsDeviceService))).GraphicsDevice;

        static string Clean(string name) => name.Replace('\\', '/').TrimStart('/');

        string Converted(string assetName, string extension)
        {
            string root = Path.Combine(Runtime.BaseDirectory, "ContentConverted", RootDirectory ?? "");
            string p = Path.Combine(root, Clean(assetName) + extension);
            return File.Exists(p) ? p : null;
        }

        public override T Load<T>(string assetName)
        {
            string key = typeof(T).FullName + "|" + Clean(assetName);
            if (loaded.TryGetValue(key, out object have))
                return (T)have;
            object asset = LoadConverted<T>(assetName);
            if (asset == null)
            {
                {
                    try { asset = base.Load<T>(assetName); }
                    catch (Exception e)
                    {
                        Runtime.Log("XnaPort: cannot load " + typeof(T).Name + " '" + assetName + "': " + e.Message);
                        throw;
                    }
                }
            }
            loaded[key] = asset;
            return (T)asset;
        }

        // Which platform the .xnb was built for ('x': Xbox 360) decides whether XboxContent swaps its GPU data.
        protected override Stream OpenStream(string assetName)
        {
            Stream stream = base.OpenStream(assetName);
            if (stream.CanSeek && stream.Length >= 4)
            {
                var head = new byte[4];
                stream.ReadExactly(head, 0, 4);
                stream.Seek(-4, SeekOrigin.Current);
                XboxContent.Swapping = head[0] == 'X' && head[1] == 'N' && head[2] == 'B' && head[3] == 'x';
            }
            return stream;
        }

        object LoadConverted<T>(string assetName)
        {
            if (typeof(T) == typeof(Texture2D))
            {
                string png = Converted(assetName, ".png");
                return png == null ? null : PremultipliedTexture(png);
            }
            if (typeof(T) == typeof(SpriteFont))
            {
                string png = Converted(assetName, ".png"), json = Converted(assetName, ".json");
                return png == null || json == null ? null : Font(png, json);
            }
            if (typeof(T) == typeof(SoundEffect))
            {
                string wav = Converted(assetName, ".wav");
                if (wav == null)
                    return null;
                using (var f = File.OpenRead(wav))
                    return SoundEffect.FromStream(f);
            }
            return null;
        }

        Texture2D PremultipliedTexture(string png)
        {
            // XNA's content pipeline premultiplied alpha at build time; a PNG has straight alpha. Done as the PNG is
            // decoded: games load content on other threads, where MonoGame cannot read a texture back.
            using (var f = File.OpenRead(png))
                return Texture2D.FromStream(Device, f, DefaultColorProcessors.PremultiplyAlpha);
        }

        SpriteFont Font(string png, string json)
        {
            var tex = PremultipliedTexture(png);
            using var doc = JsonDocument.Parse(File.ReadAllText(json));
            var r = doc.RootElement;
            List<Rectangle> Rects(string name)
            {
                var list = new List<Rectangle>();
                foreach (var e in r.GetProperty(name).EnumerateArray())
                    list.Add(new Rectangle(e[0].GetInt32(), e[1].GetInt32(), e[2].GetInt32(), e[3].GetInt32()));
                return list;
            }
            var chars = new List<char>();
            foreach (var c in r.GetProperty("characters").EnumerateArray())
            {
                string s = c.GetString();
                chars.Add(string.IsNullOrEmpty(s) ? '?' : s[0]);
            }
            var kerning = new List<Vector3>();
            foreach (var k in r.GetProperty("kerning").EnumerateArray())
                kerning.Add(new Vector3(k[0].GetSingle(), k[1].GetSingle(), k[2].GetSingle()));
            return new SpriteFont(tex, Rects("glyphs"), Rects("cropping"), chars, r.GetProperty("line_spacing").GetInt32(),
                                  r.GetProperty("spacing").GetSingle(), kerning, chars.Contains('?') ? '?' : (char?)null);
        }

    }
}
