// XnaPort.cs -- CodeBreaker's XNA runtime on MonoGame: what an Xbox 360 XNA game needs to run on a PC that MonoGame
// does not give it. The `xna` lane's port step puts these files beside the game's decompiled source.
//
//   Runtime.Install(game)       first thing in the game's constructor (put there by the port step): the game's
//                               Content becomes a ContentManager that reads what CodeBreaker converted
//   Runtime.Graphics(game)      in place of `new GraphicsDeviceManager(game)`: the HiDef profile the Xbox 360 had
//   ContentManager              Load<T>: a converted file when there is one (ContentConverted/: textures and fonts
//                               as PNG and JSON, sounds as WAV), else the game's own .xnb (MonoGame reads Xbox XNB
//                               headers; most readers work on its little-endian stream), and an Effect from
//                               Content/shaders/<name>.mgfxo -- one without names itself in the exception, so a run
//                               says which shader to write next.
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
            game.Content = new ContentManager(game.Services, "Content");
            game.IsMouseVisible = true;
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
                if (typeof(T) == typeof(Effect) || typeof(Effect).IsAssignableFrom(typeof(T)))
                    asset = LoadEffect(assetName);
                else
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
            Texture2D t;
            using (var f = File.OpenRead(png))
                t = Texture2D.FromStream(Device, f);
            // XNA's content pipeline premultiplied alpha at build time; a PNG has straight alpha
            var px = new Color[t.Width * t.Height];
            t.GetData(px);
            for (int i = 0; i < px.Length; i++)
                px[i] = Color.FromNonPremultiplied(px[i].R, px[i].G, px[i].B, px[i].A);
            t.SetData(px);
            return t;
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

        Effect LoadEffect(string assetName)
        {
            string name = Path.GetFileName(Clean(assetName));
            string mgfx = Path.Combine(Runtime.BaseDirectory, "Content", "shaders", name + ".mgfxo");
            if (!File.Exists(mgfx))
            {
                string why = "XnaPort: the Xbox 360 shader '" + assetName + "' has no PC version yet: write it as HLSL and " +
                             "compile it to Content/shaders/" + name + ".mgfxo (mgfxc in.fx out.mgfxo /Profile:OpenGL)";
                Runtime.Log(why);
                throw new ContentLoadException(why);
            }
            return new Effect(Device, File.ReadAllBytes(mgfx));
        }
    }
}
