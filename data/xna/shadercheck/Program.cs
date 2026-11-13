// shadercheck -- two compiled effects (.mgfxo) drawn the same way and compared pixel by pixel: the recovered HLSL of
// an Xbox 360 effect against the HLSL it was compiled from (tests/test_xbox_effect.py).
//
//   shadercheck <a.mgfxo> <b.mgfxo> <out.json>
//
// Every technique of A is drawn with A and with B (the technique of the same name): a quad over a 64x64 target, its
// vertices carrying every usage a shader may read (position, normal, texture coordinates 0-3, colours 0-1, blend
// indices and weights), every parameter set alike in both -- matrices to the identity (so positions come through),
// numbers to values from one seeded sequence, textures to one patterned texture. out.json: per technique, the largest
// channel difference and how many pixels were drawn.
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text.Json;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

struct Vertex : IVertexType
{
    public Vector4 Position, Normal, Uv0, Uv1, Uv2, Uv3, Color0, Color1, Indices, Weights;
    public static readonly VertexDeclaration Declaration = new VertexDeclaration(
        new VertexElement(0, VertexElementFormat.Vector4, VertexElementUsage.Position, 0),
        new VertexElement(16, VertexElementFormat.Vector4, VertexElementUsage.Normal, 0),
        new VertexElement(32, VertexElementFormat.Vector4, VertexElementUsage.TextureCoordinate, 0),
        new VertexElement(48, VertexElementFormat.Vector4, VertexElementUsage.TextureCoordinate, 1),
        new VertexElement(64, VertexElementFormat.Vector4, VertexElementUsage.TextureCoordinate, 2),
        new VertexElement(80, VertexElementFormat.Vector4, VertexElementUsage.TextureCoordinate, 3),
        new VertexElement(96, VertexElementFormat.Vector4, VertexElementUsage.Color, 0),
        new VertexElement(112, VertexElementFormat.Vector4, VertexElementUsage.Color, 1),
        new VertexElement(128, VertexElementFormat.Vector4, VertexElementUsage.BlendIndices, 0),
        new VertexElement(144, VertexElementFormat.Vector4, VertexElementUsage.BlendWeight, 0));
    VertexDeclaration IVertexType.VertexDeclaration => Declaration;
}

class Check : Game
{
    const int Size = 64;
    readonly string a, b, output;
    readonly GraphicsDeviceManager graphics;

    public Check(string a, string b, string output)
    {
        this.a = a; this.b = b; this.output = output;
        graphics = new GraphicsDeviceManager(this) { GraphicsProfile = GraphicsProfile.HiDef, PreferredBackBufferWidth = Size, PreferredBackBufferHeight = Size };
    }

    static Vertex Corner(float x, float y, int i)
    {
        Vector4 v(float s) => new Vector4(0.2f + 0.6f * x * s % 1, 0.3f + 0.5f * y, 0.25f * (i + 1) * s % 1, 0.5f + 0.1f * i);
        return new Vertex
        {
            Position = new Vector4(x * 2 - 1, y * 2 - 1, 0.5f, 1),
            Normal = Vector4.Normalize(new Vector4(x - 0.4f, y - 0.3f, 0.8f, 0)),
            Uv0 = new Vector4(x, y, 0.3f, 1), Uv1 = v(1.3f), Uv2 = v(1.7f), Uv3 = v(2.3f),
            Color0 = new Vector4(x, y, 1 - x, 0.5f + 0.5f * y), Color1 = v(0.7f),
            Indices = new Vector4(i % 3, (i + 1) % 3, 0, 0), Weights = new Vector4(0.7f, 0.3f, 0, 0),
        };
    }

    static void SetAll(Effect e, Texture2D texture)
    {
        var random = new Random(1234);
        foreach (EffectParameter p in e.Parameters)
        {
            string key = p.Name;
            uint h = 2166136261;                         // FNV-1a: the same values for the same name, every run
            foreach (char ch in key) h = (h ^ ch) * 16777619;
            random = new Random((int)(h & 0x7fffffff));
            float next() => (float)random.NextDouble();
            try
            {
                if (p.ParameterType == EffectParameterType.Texture || p.ParameterType == EffectParameterType.Texture2D)
                    p.SetValue(texture);
                else if (p.ParameterClass == EffectParameterClass.Matrix)
                {
                    int n = Math.Max(1, p.Elements.Count);
                    var m = Enumerable.Repeat(Matrix.Identity, n).ToArray();
                    if (p.Elements.Count > 0) p.SetValue(m); else p.SetValue(Matrix.Identity);
                }
                else if (p.ParameterType == EffectParameterType.Single)
                {
                    int count = Math.Max(1, p.Elements.Count) * Math.Max(1, p.RowCount * p.ColumnCount);
                    var values = new float[count];
                    for (int i = 0; i < count; i++) values[i] = 0.1f + 0.8f * next();
                    if (p.Elements.Count == 0 && p.ParameterClass == EffectParameterClass.Scalar) p.SetValue(values[0]);
                    else if (p.Elements.Count == 0 && p.ColumnCount == 2) p.SetValue(new Vector2(values[0], values[1]));
                    else if (p.Elements.Count == 0 && p.ColumnCount == 3) p.SetValue(new Vector3(values[0], values[1], values[2]));
                    else if (p.Elements.Count == 0 && p.ColumnCount == 4) p.SetValue(new Vector4(values[0], values[1], values[2], values[3]));
                    else p.SetValue(values);
                }
                else if (p.ParameterType == EffectParameterType.Bool)
                    p.SetValue(true);
                else if (p.ParameterType == EffectParameterType.Int32)
                    p.SetValue(1);
            }
            catch (Exception) { }                        // a parameter of a form not set here keeps its default
        }
    }

    Color[] Draw(Effect e, string technique, Texture2D texture, VertexBuffer vb, IndexBuffer ib, RenderTarget2D target)
    {
        GraphicsDevice.SetRenderTarget(target);
        GraphicsDevice.Clear(new Color(0, 0, 0, 0));
        GraphicsDevice.BlendState = BlendState.Opaque;
        GraphicsDevice.DepthStencilState = DepthStencilState.None;
        GraphicsDevice.RasterizerState = RasterizerState.CullNone;
        GraphicsDevice.SetVertexBuffer(vb);
        GraphicsDevice.Indices = ib;
        SetAll(e, texture);
        e.CurrentTechnique = e.Techniques[technique];
        foreach (var pass in e.CurrentTechnique.Passes)
        {
            pass.Apply();
            GraphicsDevice.SamplerStates[0] = SamplerState.PointClamp;
            GraphicsDevice.DrawIndexedPrimitives(PrimitiveType.TriangleList, 0, 0, 2);
        }
        GraphicsDevice.SetRenderTarget(null);
        var pixels = new Color[Size * Size];
        target.GetData(pixels);
        return pixels;
    }

    protected override void Draw(GameTime gameTime)
    {
        var results = new Dictionary<string, object>();
        try
        {
            var texture = new Texture2D(GraphicsDevice, 16, 16);
            var px = new Color[256];
            for (int i = 0; i < 256; i++) px[i] = new Color((i * 37) % 256, (i * 11) % 256, (i % 16) * 16, 128 + i % 128);
            texture.SetData(px);
            var vb = new VertexBuffer(GraphicsDevice, Vertex.Declaration, 4, BufferUsage.None);
            vb.SetData(new[] { Corner(0, 0, 0), Corner(1, 0, 1), Corner(0, 1, 2), Corner(1, 1, 3) });
            var ib = new IndexBuffer(GraphicsDevice, IndexElementSize.SixteenBits, 6, BufferUsage.None);
            ib.SetData(new short[] { 0, 1, 2, 2, 1, 3 });
            var target = new RenderTarget2D(GraphicsDevice, Size, Size, false, SurfaceFormat.Color, DepthFormat.None);
            var ea = new Effect(GraphicsDevice, File.ReadAllBytes(a));
            var eb = new Effect(GraphicsDevice, File.ReadAllBytes(b));
            foreach (var t in ea.Techniques)
            {
                if (eb.Techniques[t.Name] == null) { results[t.Name] = new { error = "B has no such technique" }; continue; }
                Color[] pa = Draw(ea, t.Name, texture, vb, ib, target), pb = Draw(eb, t.Name, texture, vb, ib, target);
                int worst = 0, drawn = 0;
                for (int i = 0; i < pa.Length; i++)
                {
                    if (pa[i].PackedValue != 0) drawn++;
                    worst = Math.Max(worst, Math.Max(Math.Max(Math.Abs(pa[i].R - pb[i].R), Math.Abs(pa[i].G - pb[i].G)),
                                                     Math.Max(Math.Abs(pa[i].B - pb[i].B), Math.Abs(pa[i].A - pb[i].A))));
                }
                results[t.Name] = new { worst, drawn, pixels = pa.Length };
            }
        }
        catch (Exception x) { results["error"] = x.GetType().Name + ": " + x.Message; }
        File.WriteAllText(output, JsonSerializer.Serialize(results, new JsonSerializerOptions { WriteIndented = true }));
        Exit();
    }

    static void Main(string[] args)
    {
        using var game = new Check(args[0], args[1], args[2]);
        game.Run();
    }
}
