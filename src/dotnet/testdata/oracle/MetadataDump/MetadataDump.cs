// The oracle of dotnet/metadata's tests: every part of an assembly's metadata as .NET's own reader
// (System.Reflection.Metadata) reads it, written as the text dotnet/metadata/dump.cpp writes. Its output is checked in as
// the golden (testdata/golden/*.metadata.txt), so the tests never need .NET; build.cmd refreshes it.
//
//     dotnet run -c Release -- <assembly>    (the dump on stdout)
using System;
using System.Collections.Generic;
using System.Collections.Immutable;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Reflection.Metadata;
using System.Reflection.Metadata.Ecma335;
using System.Reflection.PortableExecutable;
using System.Text;

static class Program
{
    static MetadataReader md;
    static readonly StringBuilder o = new();

    static string Tok(EntityHandle h) => h.IsNil ? "nil" : MetadataTokens.GetToken(h).ToString("x8");
    static string Tok(Handle h) => h.IsNil ? "nil" : MetadataTokens.GetToken(h).ToString("x8");
    static string Hex(byte[] b) => b.Length == 0 ? "-" : Convert.ToHexString(b).ToLowerInvariant();
    static string Blob(BlobHandle h) => h.IsNil ? "nil" : Hex(md.GetBlobBytes(h));
    static string Str(StringHandle h) => Esc(md.GetString(h));
    static string Esc(string s)
    {
        var b = new StringBuilder("\"");
        foreach (char c in s)
        {
            if (c == '\\' || c == '"') b.Append('\\').Append(c);
            else if (c < 0x20 || c > 0x7e) b.Append("\\u").Append(((int)c).ToString("x4"));
            else b.Append(c);
        }
        return b.Append('"').ToString();
    }
    static void Line(string s) => o.Append(s).Append('\n');

    sealed class TypeText : ISignatureTypeProvider<string, object>, ICustomAttributeTypeProvider<string>
    {
        public string GetPrimitiveType(PrimitiveTypeCode t) => t switch
        {
            PrimitiveTypeCode.Void => "void", PrimitiveTypeCode.Boolean => "bool", PrimitiveTypeCode.Char => "char",
            PrimitiveTypeCode.SByte => "int8", PrimitiveTypeCode.Byte => "uint8", PrimitiveTypeCode.Int16 => "int16",
            PrimitiveTypeCode.UInt16 => "uint16", PrimitiveTypeCode.Int32 => "int32", PrimitiveTypeCode.UInt32 => "uint32",
            PrimitiveTypeCode.Int64 => "int64", PrimitiveTypeCode.UInt64 => "uint64", PrimitiveTypeCode.Single => "float32",
            PrimitiveTypeCode.Double => "float64", PrimitiveTypeCode.String => "string", PrimitiveTypeCode.TypedReference => "typedref",
            PrimitiveTypeCode.IntPtr => "intptr", PrimitiveTypeCode.UIntPtr => "uintptr", PrimitiveTypeCode.Object => "object",
            _ => "?" + (int)t,
        };
        public string GetTypeFromDefinition(MetadataReader r, TypeDefinitionHandle h, byte kind) => "T:" + Tok(h) + "@" + kind.ToString("x2");
        public string GetTypeFromReference(MetadataReader r, TypeReferenceHandle h, byte kind) => "T:" + Tok(h) + "@" + kind.ToString("x2");
        public string GetTypeFromSpecification(MetadataReader r, object c, TypeSpecificationHandle h, byte kind) => "S:" + Tok(h) + "@" + kind.ToString("x2");
        public string GetSZArrayType(string e) => e + "[]";
        public string GetArrayType(string e, ArrayShape s) =>
            e + "[" + s.Rank + ";" + string.Join(",", s.Sizes) + ";" + string.Join(",", s.LowerBounds) + "]";
        public string GetPointerType(string e) => e + "*";
        public string GetByReferenceType(string e) => e + "&";
        public string GetPinnedType(string e) => "pinned(" + e + ")";
        public string GetModifiedType(string m, string t, bool req) => t + (req ? " modreq(" : " modopt(") + m + ")";
        public string GetGenericInstantiation(string g, ImmutableArray<string> a) => g + "<" + string.Join(",", a) + ">";
        public string GetGenericTypeParameter(object c, int i) => "!" + i;
        public string GetGenericMethodParameter(object c, int i) => "!!" + i;
        public string GetFunctionPointerType(MethodSignature<string> s) => "fnptr(" + Sig(s) + ")";
        // the custom attribute side
        public string GetSystemType() => "[System.Type]";
        // System.Type: the provider's own, or a reference to System.Type (as a constructor's parameter names it).
        public bool IsSystemType(string t)
        {
            if (t == "[System.Type]") return true;
            if (!t.StartsWith("T:01")) return false;
            var r = md.GetTypeReference(MetadataTokens.TypeReferenceHandle(Convert.ToInt32(t.Substring(4, 6), 16)));
            return md.GetString(r.Namespace) == "System" && md.GetString(r.Name) == "Type";
        }
        public string GetTypeFromSerializedName(string name) => "ser:" + Esc(name);
        public PrimitiveTypeCode GetUnderlyingEnumType(string t) => EnumUnderlying(t);
    }
    static readonly TypeText tt = new();

    // The underlying type of an enum named in a custom attribute: a type of this module by its value__ field (by token,
    // or by its serialized name's first part); anything else (another assembly's enum) is taken to be int32.
    static PrimitiveTypeCode EnumUnderlying(string t)
    {
        TypeDefinitionHandle def = default;
        if (t.StartsWith("T:02"))
            def = MetadataTokens.TypeDefinitionHandle(Convert.ToInt32(t.Substring(4, 6), 16));
        else if (t.StartsWith("ser:"))
        {
            string name = t.Substring(5, t.Length - 6).Split(',')[0].Trim();
            foreach (var h in md.TypeDefinitions)
            {
                var d = md.GetTypeDefinition(h);
                string full = d.Namespace.IsNil || md.GetString(d.Namespace).Length == 0 ? md.GetString(d.Name) : md.GetString(d.Namespace) + "." + md.GetString(d.Name);
                if (full == name) def = h;
            }
        }
        if (!def.IsNil)
            foreach (var f in md.GetTypeDefinition(def).GetFields())
            {
                var fd = md.GetFieldDefinition(f);
                if ((fd.Attributes & FieldAttributes.Static) == 0)
                {
                    var br = md.GetBlobReader(fd.Signature);
                    br.ReadSignatureHeader();
                    return (PrimitiveTypeCode)br.ReadByte();
                }
            }
        return PrimitiveTypeCode.Int32;
    }

    static string Sig(MethodSignature<string> s) =>
        s.Header.RawValue.ToString("x2") + " g" + s.GenericParameterCount + " r" + s.RequiredParameterCount + " " + s.ReturnType +
        "(" + string.Join(", ", s.ParameterTypes) + ")";

    static string Value(CustomAttributeTypedArgument<string> a)
    {
        string v = a.Value switch
        {
            null => "null",
            string s => Esc(s),
            bool b => b ? "true" : "false",
            char c => ((int)c).ToString(),
            float f => "f" + BitConverter.SingleToInt32Bits(f).ToString("x8"),
            double d => "d" + BitConverter.DoubleToInt64Bits(d).ToString("x16"),
            ImmutableArray<CustomAttributeTypedArgument<string>> arr => "[" + string.Join(", ", arr.Select(Value)) + "]",
            IFormattable n => n.ToString(null, System.Globalization.CultureInfo.InvariantCulture),
            _ => a.Value.ToString(),
        };
        return a.Type + ":" + v;
    }

    static void Main(string[] args)
    {
        using var stream = File.OpenRead(args[0]);
        using var pe = new PEReader(stream);
        md = pe.GetMetadataReader();
        var cor = pe.PEHeaders.CorHeader;
        Line($"cor runtime={cor.MajorRuntimeVersion}.{cor.MinorRuntimeVersion} metadata={cor.MetadataDirectory.RelativeVirtualAddress:x}+{cor.MetadataDirectory.Size:x} " +
             $"flags={(int)cor.Flags:x} entry={cor.EntryPointTokenOrRelativeVirtualAddress:x8} resources={cor.ResourcesDirectory.RelativeVirtualAddress:x}+{cor.ResourcesDirectory.Size:x} " +
             $"strongname={cor.StrongNameSignatureDirectory.RelativeVirtualAddress:x}+{cor.StrongNameSignatureDirectory.Size:x}");
        Line($"metadata version={Esc(md.MetadataVersion)} kind={(int)md.MetadataKind} assembly={(md.IsAssembly ? 1 : 0)}");
        foreach (HeapIndex h in new[] { HeapIndex.UserString, HeapIndex.String, HeapIndex.Blob, HeapIndex.Guid })
            Line($"heap {h} offset={md.GetHeapMetadataOffset(h):x} size={md.GetHeapSize(h):x}");
        for (int t = 0; t <= 0x2c; t++)
        {
            var ti = (TableIndex)t;
            int n = md.GetTableRowCount(ti);
            if (n > 0) Line($"table {t:x2} rows={n} rowsize={md.GetTableRowSize(ti)} offset={md.GetTableMetadataOffset(ti):x}");
        }

        // the heaps
        for (var h = MetadataTokens.StringHandle(0); ;)
        {
            h = md.GetNextHandle(h);
            if (h.IsNil) break;
            Line($"string {MetadataTokens.GetHeapOffset(h):x} {Str(h)}");
        }
        if (md.GetHeapSize(HeapIndex.UserString) > 1)
            for (var h = MetadataTokens.UserStringHandle(0); ;)
            {
                h = md.GetNextHandle(h);
                if (h.IsNil) break;
                Line($"userstring {MetadataTokens.GetHeapOffset(h):x} {Esc(md.GetUserString(h))}");
            }
        for (var h = MetadataTokens.BlobHandle(0); ;)
        {
            h = md.GetNextHandle(h);
            if (h.IsNil) break;
            Line($"blob {MetadataTokens.GetHeapOffset(h):x} {Blob(h)}");
        }
        for (int i = 1; i <= md.GetHeapSize(HeapIndex.Guid) / 16; i++)
            Line($"guid {i} {md.GetGuid(MetadataTokens.GuidHandle(i)):D}");

        // the tables, row by row
        var mod = md.GetModuleDefinition();
        Line($"module gen={mod.Generation} name={Str(mod.Name)} mvid={md.GetGuid(mod.Mvid):D} enc={(mod.GenerationId.IsNil ? "nil" : md.GetGuid(mod.GenerationId).ToString("D"))} encbase={(mod.BaseGenerationId.IsNil ? "nil" : md.GetGuid(mod.BaseGenerationId).ToString("D"))}");
        foreach (var h in md.TypeReferences)
        {
            var r = md.GetTypeReference(h);
            Line($"typeref {Tok(h)} scope={Tok(r.ResolutionScope)} ns={Str(r.Namespace)} name={Str(r.Name)}");
        }
        foreach (var h in md.TypeDefinitions)
        {
            var d = md.GetTypeDefinition(h);
            var layout = d.GetLayout();
            Line($"typedef {Tok(h)} flags={(int)d.Attributes:x} ns={Str(d.Namespace)} name={Str(d.Name)} base={Tok(d.BaseType)} " +
                 $"declaring={Tok(d.GetDeclaringType())} layout={layout.PackingSize},{layout.Size}");
            Line("  fields " + string.Join(" ", d.GetFields().Select(f => Tok(f))));
            Line("  methods " + string.Join(" ", d.GetMethods().Select(m => Tok(m))));
            Line("  properties " + string.Join(" ", d.GetProperties().Select(p => Tok(p))));
            Line("  events " + string.Join(" ", d.GetEvents().Select(e => Tok(e))));
            Line("  nested " + string.Join(" ", d.GetNestedTypes().Select(n => Tok(n))));
            Line("  interfaces " + string.Join(" ", d.GetInterfaceImplementations().Select(i => Tok(i) + "=" + Tok(md.GetInterfaceImplementation(i).Interface))));
            Line("  generic " + string.Join(" ", d.GetGenericParameters().Select(g => Tok(g))));
            Line("  methodimpls " + string.Join(" ", d.GetMethodImplementations().Select(i =>
            {
                var mi = md.GetMethodImplementation(i);
                return Tok(i) + "=" + Tok(mi.MethodBody) + "/" + Tok(mi.MethodDeclaration);
            })));
            Line("  security " + string.Join(" ", d.GetDeclarativeSecurityAttributes().Select(s => Tok(s))));
        }
        foreach (var h in md.FieldDefinitions)
        {
            var f = md.GetFieldDefinition(h);
            Line($"field {Tok(h)} flags={(int)f.Attributes:x} name={Str(f.Name)} sig={f.DecodeSignature(tt, null)} owner={Tok(f.GetDeclaringType())} " +
                 $"const={Tok(f.GetDefaultValue())} offset={f.GetOffset()} rva={f.GetRelativeVirtualAddress():x} marshal={Blob(f.GetMarshallingDescriptor())}");
        }
        foreach (var h in md.MethodDefinitions)
        {
            var m = md.GetMethodDefinition(h);
            var imp = m.GetImport();
            Line($"method {Tok(h)} flags={(int)m.Attributes:x} impl={(int)m.ImplAttributes:x} rva={m.RelativeVirtualAddress:x} name={Str(m.Name)} " +
                 $"sig={Sig(m.DecodeSignature(tt, null))} owner={Tok(m.GetDeclaringType())}");
            Line("  params " + string.Join(" ", m.GetParameters().Select(p => Tok(p))));
            Line("  generic " + string.Join(" ", m.GetGenericParameters().Select(g => Tok(g))));
            Line("  security " + string.Join(" ", m.GetDeclarativeSecurityAttributes().Select(s => Tok(s))));
            if (!imp.Module.IsNil) Line($"  import flags={(int)imp.Attributes:x} name={Str(imp.Name)} module={Tok(imp.Module)}");
            if (m.RelativeVirtualAddress != 0)
            {
                MethodBodyBlock b;
                try { b = pe.GetMethodBody(m.RelativeVirtualAddress); }
                catch (BadImageFormatException) { Line("  body undecodable"); continue; }   // (a native body: C++/CLI)
                Line($"  body maxstack={b.MaxStack} init={(b.LocalVariablesInitialized ? 1 : 0)} locals={Tok(b.LocalSignature)} size={b.Size} il={Hex(b.GetILBytes())}");
                foreach (var r in b.ExceptionRegions)
                    Line($"  region {(int)r.Kind} try={r.TryOffset:x}+{r.TryLength:x} handler={r.HandlerOffset:x}+{r.HandlerLength:x} catch={Tok(r.CatchType)} filter={r.FilterOffset:x}");
            }
        }
        for (int i = 1; i <= md.GetTableRowCount(TableIndex.Param); i++)
        {
            var h = MetadataTokens.ParameterHandle(i);
            var p = md.GetParameter(h);
            Line($"param {Tok(h)} flags={(int)p.Attributes:x} seq={p.SequenceNumber} name={Str(p.Name)} const={Tok(p.GetDefaultValue())} marshal={Blob(p.GetMarshallingDescriptor())}");
        }
        foreach (var h in md.MemberReferences)
        {
            var r = md.GetMemberReference(h);
            string sig = r.GetKind() == MemberReferenceKind.Method ? Sig(r.DecodeMethodSignature(tt, null)) : r.DecodeFieldSignature(tt, null);
            Line($"memberref {Tok(h)} parent={Tok(r.Parent)} name={Str(r.Name)} kind={(int)r.GetKind()} sig={sig}");
        }
        for (int i = 1; i <= md.GetTableRowCount(TableIndex.Constant); i++)
        {
            var h = MetadataTokens.ConstantHandle(i);
            var c = md.GetConstant(h);
            Line($"constant {Tok(h)} type={(int)c.TypeCode:x2} parent={Tok(c.Parent)} value={Blob(c.Value)}");
        }
        foreach (var h in md.CustomAttributes)
        {
            var a = md.GetCustomAttribute(h);
            string value;
            try
            {
                var v = a.DecodeValue(tt);
                value = "(" + string.Join(", ", v.FixedArguments.Select(Value)) + ") {" +
                        string.Join(", ", v.NamedArguments.Select(n => (n.Kind == CustomAttributeNamedArgumentKind.Field ? "F " : "P ") + n.Name + "=" + Value(new CustomAttributeTypedArgument<string>(n.Type, n.Value)))) + "}";
            }
            catch (Exception e) when (e is BadImageFormatException || e is ArgumentException) { value = "undecodable"; }
            Line($"attribute {Tok(h)} parent={Tok(a.Parent)} ctor={Tok(a.Constructor)} blob={Blob(a.Value)} value={value}");
        }
        foreach (var h in md.DeclarativeSecurityAttributes)
        {
            var s = md.GetDeclarativeSecurityAttribute(h);
            Line($"security {Tok(h)} action={(int)s.Action} parent={Tok(s.Parent)} blob={Blob(s.PermissionSet)}");
        }
        for (int i = 1; i <= md.GetTableRowCount(TableIndex.StandAloneSig); i++)
        {
            var h = MetadataTokens.StandaloneSignatureHandle(i);
            var s = md.GetStandaloneSignature(h);
            string sig;
            try
            {
                sig = s.GetKind() == StandaloneSignatureKind.LocalVariables
                    ? "locals(" + string.Join(", ", s.DecodeLocalSignature(tt, null)) + ")"
                    : Sig(s.DecodeMethodSignature(tt, null));
            }
            catch (BadImageFormatException) { sig = "undecodable"; }   // (old compilers wrote field signatures here)
            Line($"standalonesig {Tok(h)} sig={sig}");
        }
        foreach (var h in md.EventDefinitions)
        {
            var e = md.GetEventDefinition(h);
            var acc = e.GetAccessors();
            Line($"event {Tok(h)} flags={(int)e.Attributes:x} name={Str(e.Name)} type={Tok(e.Type)} add={Tok(acc.Adder)} remove={Tok(acc.Remover)} raise={Tok(acc.Raiser)} " +
                 "others=" + string.Join(",", acc.Others.Select(x => Tok(x))));
        }
        foreach (var h in md.PropertyDefinitions)
        {
            var p = md.GetPropertyDefinition(h);
            var acc = p.GetAccessors();
            Line($"property {Tok(h)} flags={(int)p.Attributes:x} name={Str(p.Name)} sig={Sig(p.DecodeSignature(tt, null))} const={Tok(p.GetDefaultValue())} " +
                 $"get={Tok(acc.Getter)} set={Tok(acc.Setter)} others=" + string.Join(",", acc.Others.Select(x => Tok(x))));
        }
        for (int i = 1; i <= md.GetTableRowCount(TableIndex.ModuleRef); i++)
        {
            var h = MetadataTokens.ModuleReferenceHandle(i);
            Line($"moduleref {Tok(h)} name={Str(md.GetModuleReference(h).Name)}");
        }
        for (int i = 1; i <= md.GetTableRowCount(TableIndex.TypeSpec); i++)
        {
            var h = MetadataTokens.TypeSpecificationHandle(i);
            Line($"typespec {Tok(h)} sig={md.GetTypeSpecification(h).DecodeSignature(tt, null)}");
        }
        if (md.IsAssembly)
        {
            var a = md.GetAssemblyDefinition();
            Line($"assembly name={Str(a.Name)} version={a.Version} culture={Str(a.Culture)} flags={(int)a.Flags:x} hash={(int)a.HashAlgorithm:x} key={Blob(a.PublicKey)}");
        }
        foreach (var h in md.AssemblyReferences)
        {
            var a = md.GetAssemblyReference(h);
            Line($"assemblyref {Tok(h)} name={Str(a.Name)} version={a.Version} culture={Str(a.Culture)} flags={(int)a.Flags:x} key={Blob(a.PublicKeyOrToken)} hash={Blob(a.HashValue)}");
        }
        foreach (var h in md.AssemblyFiles)
        {
            var f = md.GetAssemblyFile(h);
            Line($"file {Tok(h)} name={Str(f.Name)} metadata={(f.ContainsMetadata ? 1 : 0)} hash={Blob(f.HashValue)}");
        }
        foreach (var h in md.ExportedTypes)
        {
            var t = md.GetExportedType(h);
            Line($"exportedtype {Tok(h)} flags={(int)t.Attributes:x} ns={Str(t.Namespace)} name={Str(t.Name)} impl={Tok(t.Implementation)} typedef={t.GetTypeDefinitionId():x}");
        }
        foreach (var h in md.ManifestResources)
        {
            var r = md.GetManifestResource(h);
            Line($"resource {Tok(h)} name={Str(r.Name)} offset={r.Offset:x} flags={(int)r.Attributes:x} impl={Tok(r.Implementation)}");
        }
        for (int i = 1; i <= md.GetTableRowCount(TableIndex.GenericParam); i++)
        {
            var h = MetadataTokens.GenericParameterHandle(i);
            var g = md.GetGenericParameter(h);
            Line($"genericparam {Tok(h)} index={g.Index} flags={(int)g.Attributes:x} name={Str(g.Name)} parent={Tok(g.Parent)} constraints=" +
                 string.Join(",", g.GetConstraints().Select(c => Tok(c) + "=" + Tok(md.GetGenericParameterConstraint(c).Type))));
        }
        for (int i = 1; i <= md.GetTableRowCount(TableIndex.MethodSpec); i++)
        {
            var h = MetadataTokens.MethodSpecificationHandle(i);
            var s = md.GetMethodSpecification(h);
            Line($"methodspec {Tok(h)} method={Tok(s.Method)} inst=<" + string.Join(",", s.DecodeSignature(tt, null)) + ">");
        }
        // attributes by owner (the reverse lookups)
        foreach (var h in md.TypeDefinitions)
            Line($"attributes-of {Tok(h)} " + string.Join(" ", md.GetCustomAttributes(h).Select(a => Tok(a))));
        foreach (var h in md.MethodDefinitions)
            Line($"attributes-of {Tok(h)} " + string.Join(" ", md.GetCustomAttributes(h).Select(a => Tok(a))));
        foreach (var h in md.FieldDefinitions)
            Line($"attributes-of {Tok(h)} " + string.Join(" ", md.GetCustomAttributes(h).Select(a => Tok(a))));

        var stdout = Console.OpenStandardOutput();
        var bytes = new UTF8Encoding(false).GetBytes(o.ToString());
        stdout.Write(bytes, 0, bytes.Length);
    }
}
