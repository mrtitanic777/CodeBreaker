// XboxContent.cs -- MonoGame's own content readers for an Xbox 360 .xnb, where its bulk data is big-endian.
//
// An Xbox 360 XNB writes its stream little-endian like a PC's, but the GPU data in it as the Xbox 360's GPU reads it:
// vertex and index buffers, and texture pixels, in big-endian units. These readers take the places of XNA's
// VertexBuffer, IndexBuffer, Texture2D and TextureCube readers (registered under XNA's own names, which MonoGame looks
// up first), read what MonoGame's would, and turn each unit around -- by vertex element format, index size or surface
// format -- when the file being read came from an Xbox (its header says 'x'). Being readers, they serve every model,
// skinned model or custom type the game's own readers read buffers through, however deeply nested.
//
// A Song names its music file (.wma on the Xbox 360); the port converted it to Ogg Vorbis under ContentConverted/, and
// that is what plays.
//
// An Effect is the Xbox 360's GPU microcode and has no PC form to turn into: Content/shaders/<asset name>.mgfxo, or
// Content/shaders/<sha1 of the effect's bytes>.mgfxo for one inside another asset, is loaded instead, and a missing one
// names itself in the exception.
using System;
using System.IO;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;

namespace XnaPort
{
    public static class XboxContent
    {
        [ThreadStatic] internal static bool Swapping;          // the .xnb being read is an Xbox 360 one

        const string Graphics = ", Microsoft.Xna.Framework.Graphics, Version=4.0.0.0, Culture=neutral, PublicKeyToken=842cf8be1de50553";

        public static void Register()
        {
            void add(string reader, Func<ContentTypeReader> make)
            {
                ContentTypeReaderManager.AddTypeCreator("Microsoft.Xna.Framework.Content." + reader + Graphics, make);
                ContentTypeReaderManager.AddTypeCreator("Microsoft.Xna.Framework.Content." + reader, make);
            }
            add("VertexBufferReader", () => new XboxVertexBufferReader());
            add("IndexBufferReader", () => new XboxIndexBufferReader());
            add("Texture2DReader", () => new XboxTexture2DReader());
            add("TextureCubeReader", () => new XboxTextureCubeReader());
            add("EffectReader", () => new XboxEffectReader());
            ContentTypeReaderManager.AddTypeCreator("Microsoft.Xna.Framework.Content.SongReader, Microsoft.Xna.Framework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=842cf8be1de50553", () => new XboxSongReader());
            ContentTypeReaderManager.AddTypeCreator("Microsoft.Xna.Framework.Content.SongReader", () => new XboxSongReader());
        }

        internal static GraphicsDevice Device(ContentReader input) =>
            ((IGraphicsDeviceService)input.ContentManager.ServiceProvider.GetService(typeof(IGraphicsDeviceService))).GraphicsDevice;

        internal static void Swap(byte[] data, int unit)
        {
            if (!Swapping || unit < 2) return;
            for (int i = 0; i + unit <= data.Length; i += unit)
                Array.Reverse(data, i, unit);
        }

        // Each vertex element's units, by its format.
        internal static void SwapVertices(byte[] data, VertexDeclaration declaration, int count)
        {
            if (!Swapping) return;
            int stride = declaration.VertexStride;
            foreach (var e in declaration.GetVertexElements())
            {
                var (unit, units) = e.VertexElementFormat switch
                {
                    VertexElementFormat.Single => (4, 1), VertexElementFormat.Vector2 => (4, 2),
                    VertexElementFormat.Vector3 => (4, 3), VertexElementFormat.Vector4 => (4, 4),
                    VertexElementFormat.Color => (4, 1), VertexElementFormat.Byte4 => (4, 1),
                    VertexElementFormat.Short2 => (2, 2), VertexElementFormat.Short4 => (2, 4),
                    VertexElementFormat.NormalizedShort2 => (2, 2), VertexElementFormat.NormalizedShort4 => (2, 4),
                    VertexElementFormat.HalfVector2 => (2, 2), VertexElementFormat.HalfVector4 => (2, 4),
                    _ => (1, 0)
                };
                for (int v = 0; v < count; v++)
                    for (int u = 0; u < units; u++)
                    {
                        int at = v * stride + e.Offset + u * unit;
                        if (at + unit <= data.Length) Array.Reverse(data, at, unit);
                    }
            }
        }

        // The unit a surface format's pixels are stored in (DXT blocks are 16-bit words on the Xbox 360).
        internal static int Unit(SurfaceFormat f) => f switch
        {
            SurfaceFormat.Color or SurfaceFormat.NormalizedByte4 or SurfaceFormat.Rgba1010102 or SurfaceFormat.Rg32 or
            SurfaceFormat.Single or SurfaceFormat.Vector2 or SurfaceFormat.Vector4 => 4,
            SurfaceFormat.Alpha8 => 1,
            _ => 2   // Bgr565, Bgra5551, Bgra4444, Dxt1/3/5, NormalizedByte2, Rgba64, the half formats, HdrBlendable
        };
    }

    public class XboxVertexBufferReader : ContentTypeReader<VertexBuffer>
    {
        protected override VertexBuffer Read(ContentReader input, VertexBuffer existing)
        {
            var declaration = input.ReadRawObject<VertexDeclaration>();
            int count = (int)input.ReadUInt32();
            byte[] data = input.ReadBytes(count * declaration.VertexStride);
            XboxContent.SwapVertices(data, declaration, count);
            var buffer = new VertexBuffer(XboxContent.Device(input), declaration, count, BufferUsage.None);
            buffer.SetData(data);
            return buffer;
        }
    }

    public class XboxIndexBufferReader : ContentTypeReader<IndexBuffer>
    {
        protected override IndexBuffer Read(ContentReader input, IndexBuffer existing)
        {
            bool sixteen = input.ReadBoolean();
            int size = input.ReadInt32();
            byte[] data = input.ReadBytes(size);
            XboxContent.Swap(data, sixteen ? 2 : 4);
            var buffer = new IndexBuffer(XboxContent.Device(input), sixteen ? IndexElementSize.SixteenBits : IndexElementSize.ThirtyTwoBits,
                                         size / (sixteen ? 2 : 4), BufferUsage.None);
            buffer.SetData(data);
            return buffer;
        }
    }

    public class XboxTexture2DReader : ContentTypeReader<Texture2D>
    {
        protected override Texture2D Read(ContentReader input, Texture2D existing)
        {
            var format = (SurfaceFormat)input.ReadInt32();
            int width = input.ReadInt32(), height = input.ReadInt32(), levels = input.ReadInt32();
            var texture = existing ?? new Texture2D(XboxContent.Device(input), width, height, levels > 1, format);
            for (int level = 0; level < levels; level++)
            {
                byte[] data = input.ReadBytes(input.ReadInt32());
                XboxContent.Swap(data, XboxContent.Unit(format));
                if (level < texture.LevelCount) texture.SetData(level, null, data, 0, data.Length);
            }
            return texture;
        }
    }

    public class XboxTextureCubeReader : ContentTypeReader<TextureCube>
    {
        protected override TextureCube Read(ContentReader input, TextureCube existing)
        {
            var format = (SurfaceFormat)input.ReadInt32();
            int size = input.ReadInt32(), levels = input.ReadInt32();
            var texture = existing ?? new TextureCube(XboxContent.Device(input), size, levels > 1, format);
            for (int face = 0; face < 6; face++)
                for (int level = 0; level < levels; level++)
                {
                    byte[] data = input.ReadBytes(input.ReadInt32());
                    XboxContent.Swap(data, XboxContent.Unit(format));
                    if (level < texture.LevelCount) texture.SetData((CubeMapFace)face, level, null, data, 0, data.Length);
                }
            return texture;
        }
    }

    public class XboxSongReader : ContentTypeReader<Microsoft.Xna.Framework.Media.Song>
    {
        protected override Microsoft.Xna.Framework.Media.Song Read(ContentReader input, Microsoft.Xna.Framework.Media.Song existing)
        {
            string file = input.ReadString();
            input.ReadObject<int>();                                // the duration, in milliseconds
            string folder = Path.GetDirectoryName(input.AssetName.Replace('\\', '/')) ?? "";
            string ogg = Path.Combine(Runtime.BaseDirectory, "ContentConverted", input.ContentManager.RootDirectory ?? "",
                                      folder, Path.ChangeExtension(file, ".ogg"));
            string path = File.Exists(ogg) ? ogg : Path.Combine(Runtime.BaseDirectory, input.ContentManager.RootDirectory ?? "", folder, file);
            return Microsoft.Xna.Framework.Media.Song.FromUri(Path.GetFileNameWithoutExtension(file), new Uri(path));
        }
    }

    public class XboxEffectReader : ContentTypeReader<Effect>
    {
        protected override Effect Read(ContentReader input, Effect existing)
        {
            byte[] code = input.ReadBytes(input.ReadInt32());
            string name = Path.GetFileName(input.AssetName.Replace('\\', '/'));
            string hash = Convert.ToHexString(System.Security.Cryptography.SHA1.HashData(code)).ToLowerInvariant();
            string shaders = Path.Combine(Runtime.BaseDirectory, "Content", "shaders");
            foreach (string candidate in new[] { name, hash })
            {
                string p = Path.Combine(shaders, candidate + ".mgfxo");
                if (File.Exists(p)) return new Effect(XboxContent.Device(input), File.ReadAllBytes(p));
            }
            string why = "XnaPort: the Xbox 360 shader in '" + input.AssetName + "' (sha1 " + hash + ") has no PC version yet: write it " +
                         "as HLSL and compile it to Content/shaders/" + name + ".mgfxo (or " + hash + ".mgfxo)";
            Runtime.Log(why);
            throw new ContentLoadException(why);
        }
    }
}
