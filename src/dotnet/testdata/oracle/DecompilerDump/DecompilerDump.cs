// The oracle of dotnet/decompiler's IL AST tests (and of the later stages): what ILSpy's own decompiler (ICSharpCode.Decompiler
// 11.0.0.9375) makes of every method body of an assembly, as text. Its output is checked in as goldens (testdata/golden/*.ilast.txt),
// so the tests never need .NET; build.cmd refreshes them.
//
//     DecompilerDump ilast <assembly> [--resolve]          every method read by ILReader (the ILAst before any transform), on stdout
//     DecompilerDump transforms <N> <assembly> [--resolve] the same after the first N of CSharpDecompiler.GetILTransforms()
//                                                         (default DecompilerSettings), as CSharpDecompiler runs them
//
// --resolve: references found by ILSpy's UniversalAssemblyResolver, else none. Lines end in "\n".
using System;
using System.IO;
using System.Linq;
using System.Reflection.Metadata;
using System.Reflection.Metadata.Ecma335;
using System.Text;
using System.Threading.Tasks;
using ICSharpCode.Decompiler;
using ICSharpCode.Decompiler.IL;
using ICSharpCode.Decompiler.Metadata;
using ICSharpCode.Decompiler.TypeSystem;

static class Program
{
    sealed class NoReferences : IAssemblyResolver
    {
        public MetadataFile Resolve(IAssemblyReference reference) => null;
        public MetadataFile ResolveModule(MetadataFile mainModule, string moduleName) => null;
        public Task<MetadataFile> ResolveAsync(IAssemblyReference reference) => Task.FromResult<MetadataFile>(null);
        public Task<MetadataFile> ResolveModuleAsync(MetadataFile mainModule, string moduleName) => Task.FromResult<MetadataFile>(null);
    }

    static int Main(string[] args)
    {
        string mode = args[0];
        int transformCount = 0;
        int a = 1;
        if (mode == "transforms")
            transformCount = int.Parse(args[a++]);
        string path = args[a++];
        bool resolve = args.Length > a && args[a] == "--resolve";
        var file = new PEFile(path);
        IAssemblyResolver resolver = resolve ? new UniversalAssemblyResolver(path, false, file.DetectTargetFrameworkId()) : new NoReferences();
        var ts = new DecompilerTypeSystem(file, resolver, TypeSystemOptions.Default);
        var module = (MetadataModule)ts.MainModule;
        var md = file.Metadata;
        var writer = new StringWriter { NewLine = "\n" };
        if (mode != "ilast" && mode != "transforms")
        {
            Console.Error.WriteLine("unknown mode " + mode);
            return 2;
        }
        foreach (var h in md.MethodDefinitions)
        {
            var def = md.GetMethodDefinition(h);
            if (def.RelativeVirtualAddress == 0)
                continue;
            writer.Write($"method {MetadataTokens.GetToken(h):x8} {md.GetString(md.GetTypeDefinition(def.GetDeclaringType()).Name)}::{md.GetString(def.Name)}\n");
            try
            {
                var body = file.GetMethodBody(def.RelativeVirtualAddress);
                var reader = new ILReader(module) { UseDebugSymbols = false, UseRefLocalsForAccurateOrderOfEvaluation = true };
                var function = reader.ReadIL(h, body, kind: ILFunctionKind.TopLevelFunction);
                if (mode == "transforms")
                {
                    var settings = new DecompilerSettings();
                    var context = new ICSharpCode.Decompiler.IL.Transforms.ILTransformContext(function, ts, null, settings);
                    function.RunTransforms(ICSharpCode.Decompiler.CSharp.CSharpDecompiler.GetILTransforms().Take(transformCount), context);
                }
                var output = new PlainTextOutput(writer);
                function.WriteTo(output, new ILAstWritingOptions());
                writer.Write("\n");
            }
            catch (Exception ex)
            {
                writer.Write($"error {ex.GetType().Name}: {ex.Message}\n");
            }
        }
        var stdout = Console.OpenStandardOutput();
        var bytes = new UTF8Encoding(false).GetBytes(writer.ToString());
        stdout.Write(bytes, 0, bytes.Length);
        return 0;
    }
}
