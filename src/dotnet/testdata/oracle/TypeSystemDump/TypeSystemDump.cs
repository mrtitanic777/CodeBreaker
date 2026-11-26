// The oracle of dotnet/decompiler/typesystem's tests: an assembly's type system as ILSpy's own (ICSharpCode.Decompiler 11.0.0.9375) builds
// it, written as the text dotnet/decompiler/typesystem/type_system_dump.cpp writes. Its output is checked in as the golden
// (testdata/golden/*.typesystem.txt), so the tests never need .NET; build.cmd refreshes it.
//
//     TypeSystemDump <assembly> [--resolve]     (the dump on stdout; --resolve: ILSpy's UniversalAssemblyResolver, else no references)
using System;
using System.Collections.Generic;
using System.Collections.Immutable;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Reflection.Metadata;
using System.Reflection.Metadata.Ecma335;
using System.Text;
using System.Threading.Tasks;
using ICSharpCode.Decompiler.Metadata;
using ICSharpCode.Decompiler.TypeSystem;
using ICSharpCode.Decompiler.TypeSystem.Implementation;

static class Program
{
    static readonly StringBuilder o = new();
    static void Line(string s) => o.Append(s).Append('\n');
    static string B(bool b) => b ? "1" : "0";

    sealed class NoReferences : IAssemblyResolver
    {
        public MetadataFile Resolve(IAssemblyReference reference) => null;
        public MetadataFile ResolveModule(MetadataFile mainModule, string moduleName) => null;
        public Task<MetadataFile> ResolveAsync(IAssemblyReference reference) => Task.FromResult<MetadataFile>(null);
        public Task<MetadataFile> ResolveModuleAsync(MetadataFile mainModule, string moduleName) => Task.FromResult<MetadataFile>(null);
    }

    static string Esc(string s)
    {
        if (s == null) return "null";
        var b = new StringBuilder("\"");
        foreach (char c in s)
        {
            if (c == '\\' || c == '"') b.Append('\\').Append(c);
            else if (c < 0x20 || c > 0x7e) b.Append("\\u").Append(((int)c).ToString("x4"));
            else b.Append(c);
        }
        return b.Append('"').ToString();
    }

    static string T(IType t)
    {
        switch (t)
        {
            case null: return "null";
            case NullabilityAnnotatedType n: return T(n.TypeWithoutAnnotation) + (n.Nullability == Nullability.Nullable ? "?" : "!");
            case TupleType tt:
                return "(" + string.Join(", ", tt.ElementTypes.Select((e, i) => T(e) + (tt.ElementNames[i] != null ? " " + tt.ElementNames[i] : ""))) + ")";
            case ParameterizedType p: return T(p.GenericType) + "<" + string.Join(",", p.TypeArguments.Select(T)) + ">";
            case ArrayType a:
                return T(a.ElementType) + "[" + new string(',', a.Dimensions - 1) + "]" + (a.Nullability == Nullability.Nullable ? "?" : a.Nullability == Nullability.NotNullable ? "!" : "");
            case PointerType pt: return T(pt.ElementType) + "*";
            case ByReferenceType r: return T(r.ElementType) + "&";
            case ModifiedType m: return T(m.ElementType) + (m.Kind == TypeKind.ModReq ? " modreq(" : " modopt(") + T(m.Modifier) + ")";
            case PinnedType pn: return T(pn.ElementType) + " pinned";
            case FunctionPointerType f:
                return "delegate*<" + f.CallingConvention + (f.CustomCallingConventions.Length > 0 ? "[" + string.Join(",", f.CustomCallingConventions.Select(T)) + "]" : "")
                    + ">(" + string.Join(", ", f.ParameterTypes.Select((p, i) => f.ParameterReferenceKinds[i] + " " + T(p))) + "):" + (f.ReturnIsRefReadOnly ? "ref readonly " : "") + T(f.ReturnType)
                    + (f.Kind == TypeKind.FunctionPointer ? "" : "/struct");
            case ITypeParameter tp: return (tp.OwnerType == SymbolKind.Method ? "!!" : "!") + tp.Index + ":" + tp.Name;
            case UnknownType u: return "unknown:" + u.FullTypeName.ReflectionName + ":" + Ref(u.IsReferenceType);
            case ITypeDefinition td: return td.FullTypeName.ReflectionName + "@" + td.ParentModule.AssemblyName;
            default:
                if (t.Kind is TypeKind.Unknown or TypeKind.Null or TypeKind.None or TypeKind.Dynamic or TypeKind.UnboundTypeArgument or TypeKind.ArgList or TypeKind.NInt or TypeKind.NUInt)
                    return "special:" + t.Kind;
                return "other:" + t.GetType().Name;
        }
    }

    static string Ref(bool? b) => b == null ? "null" : b.Value ? "ref" : "value";

    static string Src(IMember m) => m switch {
        VarArgInstanceMethod => "vararg",
        SpecializedMember => "specialized",
        _ when m.GetType().Name.StartsWith("Fake") => "fake",
        _ => m.GetType().Name.StartsWith("Metadata") ? "metadata" : m.GetType().Name,
    };

    static string M(IMember m)
    {
        if (m == null) return "null";
        var s = new StringBuilder(Src(m)).Append(' ').Append(m.SymbolKind).Append(' ').Append(T(m.DeclaringType)).Append('.').Append(m.Name);
        if (m is IMethod method && method.TypeArguments.Count > 0) s.Append('<').Append(string.Join(",", method.TypeArguments.Select(T))).Append('>');
        if (m is IParameterizedMember pm) s.Append('(').Append(string.Join(", ", pm.Parameters.Select(p => T(p.Type)))).Append(')');
        return s.Append(':').Append(T(m.ReturnType)).ToString();
    }

    static string V(object v)
    {
        switch (v)
        {
            case null: return "null";
            case string s: return Esc(s);
            case bool b: return b ? "True" : "False";
            case char c: return "char:" + (int)c;
            case float f: return "float:" + f.ToString("R", CultureInfo.InvariantCulture);
            case double d: return "double:" + d.ToString("R", CultureInfo.InvariantCulture);
            case decimal m: return "decimal:" + m.ToString(CultureInfo.InvariantCulture);
            case IType t: return "type:" + T(t);
            case ImmutableArray<CustomAttributeTypedArgument<IType>> a: return "[" + string.Join(", ", a.Select(e => T(e.Type) + "=" + V(e.Value))) + "]";
            default: return v.GetType().Name + ":" + Convert.ToString(v, CultureInfo.InvariantCulture);
        }
    }

    static string A(IAttribute a)
    {
        var s = new StringBuilder(T(a.AttributeType));
        s.Append(" ctor=").Append(M(a.Constructor)).Append(" err=").Append(B(a.HasDecodeErrors));
        s.Append(" (").Append(string.Join(", ", a.FixedArguments.Select(f => T(f.Type) + "=" + V(f.Value)))).Append(')');
        s.Append(" {").Append(string.Join(", ", a.NamedArguments.Select(n => n.Kind + " " + n.Name + ":" + T(n.Type) + "=" + V(n.Value)))).Append('}');
        return s.ToString();
    }

    static void Attrs(string indent, IEnumerable<IAttribute> attrs)
    {
        foreach (var a in attrs) Line(indent + "attr " + A(a));
    }

    static void Guard(string indent, Action a)
    {
        try { a(); }
        catch (Exception) { Line(indent + "error"); }
    }

    static void TypeParameters(string indent, IReadOnlyList<ITypeParameter> tps, int from)
    {
        for (int i = from; i < tps.Count; i++)
        {
            var tp = tps[i];
            Line($"{indent}tp {tp.Index} {tp.Name} owner={tp.OwnerType} var={tp.Variance} class={B(tp.HasReferenceTypeConstraint)} struct={B(tp.HasValueTypeConstraint)} new={B(tp.HasDefaultConstructorConstraint)} unmanaged={B(tp.HasUnmanagedConstraint)} allowsref={B(tp.AllowsRefLikeType)} nullc={tp.NullabilityConstraint}");
            Attrs(indent + "  ", tp.GetAttributes());
            foreach (var c in tp.TypeConstraints)
            {
                Line(indent + "  constraint " + T(c.Type));
                Attrs(indent + "    ", c.Attributes);
            }
            Line(indent + "  ebc " + T(tp.EffectiveBaseClass));
            foreach (var i2 in tp.EffectiveInterfaceSet) Line(indent + "  eis " + T(i2));
        }
    }

    static void Parameters(string indent, IReadOnlyList<IParameter> ps)
    {
        for (int i = 0; i < ps.Count; i++)
        {
            var p = ps[i];
            string line = $"{indent}param {i} {p.Name} type={T(p.Type)} ref={p.ReferenceKind} params={B(p.IsParams)} opt={B(p.IsOptional)} hasconst={B(p.HasConstantValueInSignature)} scoped={B(p.Lifetime.ScopedRef)}";
            if (p.HasConstantValueInSignature) line += " value=" + V(p.GetConstantValue());
            Line(line);
            Attrs(indent + "  ", p.GetAttributes());
        }
    }

    static string MemberFlags(IMember m) =>
        $"acc={m.Accessibility} static={B(m.IsStatic)} abstract={B(m.IsAbstract)} sealed={B(m.IsSealed)} virtual={B(m.IsVirtual)} override={B(m.IsOverride)} overridable={B(m.IsOverridable)} explicit={B(m.IsExplicitInterfaceImplementation)}";

    static void Member(MetadataModule module, IMember member)
    {
        switch (member)
        {
            case IField f:
            {
                string line = $"  field {f.Name} type={T(f.ReturnType)} {MemberFlags(f)} readonly={B(f.IsReadOnly)} volatile={B(f.IsVolatile)} const={B(f.IsConst)} rro={B(f.ReturnTypeIsRefReadOnly)}";
                if (f.IsConst) line += " value=" + V(f.GetConstantValue());
                Line(line);
                Attrs("    ", f.GetAttributes());
                break;
            }
            case IMethod m:
            {
                Line($"  method {m.Name} kind={m.SymbolKind} {MemberFlags(m)} ext={B(m.IsExtensionMethod)} body={B(m.HasBody)} init={B(m.IsInitOnly)} rro={B(m.ReturnTypeIsRefReadOnly)} thisro={B(m.ThisIsRefReadOnly)} accessor={m.AccessorKind} owner={(m.AccessorOwner?.Name ?? "-")} ret={T(m.ReturnType)}");
                TypeParameters("    ", m.TypeParameters, 0);
                Parameters("    ", m.Parameters);
                Attrs("    ", m.GetAttributes());
                foreach (var a in m.GetReturnTypeAttributes()) Line("    retattr " + A(a));
                foreach (var i in m.ExplicitlyImplementedInterfaceMembers) Line("    impl " + M(i));
                if (m.HasBody && !m.MetadataToken.IsNil)
                {
                    var def = module.MetadataFile.Metadata.GetMethodDefinition((MethodDefinitionHandle)m.MetadataToken);
                    var body = module.MetadataFile.GetMethodBody(def.RelativeVirtualAddress);
                    if (!body.LocalSignature.IsNil)
                    {
                        var locals = module.DecodeLocalSignature(body.LocalSignature, new GenericContext(m.DeclaringType.TypeParameters, m.TypeParameters));
                        for (int i = 0; i < locals.Length; i++) Line($"    local {i} {T(locals[i])}");
                    }
                }
                break;
            }
            case IProperty p:
            {
                Line($"  property {p.Name} kind={p.SymbolKind} {MemberFlags(p)} get={B(p.CanGet)} set={B(p.CanSet)} indexer={B(p.IsIndexer)} rro={B(p.ReturnTypeIsRefReadOnly)} ret={T(p.ReturnType)} getter={(p.Getter?.Name ?? "-")} setter={(p.Setter?.Name ?? "-")}");
                Parameters("    ", p.Parameters);
                Attrs("    ", p.GetAttributes());
                foreach (var i in p.ExplicitlyImplementedInterfaceMembers) Line("    impl " + M(i));
                break;
            }
            case IEvent e:
            {
                Line($"  event {e.Name} {MemberFlags(e)} add={(e.AddAccessor?.Name ?? "-")} remove={(e.RemoveAccessor?.Name ?? "-")} invoke={(e.InvokeAccessor?.Name ?? "-")} ret={T(e.ReturnType)}");
                Attrs("    ", e.GetAttributes());
                foreach (var i in e.ExplicitlyImplementedInterfaceMembers) Line("    impl " + M(i));
                break;
            }
        }
    }

    static void Namespace(INamespace ns, string indent)
    {
        Line($"{indent}ns {Esc(ns.FullName)} name={Esc(ns.Name)} types={ns.Types.Count()}");
        foreach (var child in ns.ChildNamespaces) Namespace(child, indent + "  ");
    }

    static int Main(string[] args)
    {
        string path = args[0];
        bool resolve = args.Length > 1 && args[1] == "--resolve";
        var file = new PEFile(path);
        IAssemblyResolver resolver = resolve ? new UniversalAssemblyResolver(path, false, file.DetectTargetFrameworkId()) : new NoReferences();
        var ts = new DecompilerTypeSystem(file, resolver, TypeSystemOptions.Default);
        var module = ts.MainModule;
        var md = module.MetadataFile.Metadata;
        Line("options " + ((int)ts.TypeSystemOptions).ToString("x"));
        foreach (var m in ts.Modules)
            Line($"module {Esc(m.AssemblyName)} full={Esc(m.FullAssemblyName)} version={m.AssemblyVersion} main={B(m.IsMainModule)} file={B(m.MetadataFile != null)}");
        for (int i = 1; i <= (int)KnownTypeCode.Range; i++)
        {
            if (KnownTypeReference.Get((KnownTypeCode)i) == null) continue;
            Line($"known {(KnownTypeCode)i} {T(ts.FindType((KnownTypeCode)i))}");
        }
        Guard("", () => { foreach (var a in module.GetAssemblyAttributes()) Line("asmattr " + A(a)); });
        Guard("", () => { foreach (var a in module.GetModuleAttributes()) Line("modattr " + A(a)); });
        Namespace(module.RootNamespace, "");
        foreach (var td in module.TypeDefinitions)
        {
            Guard("", () => {
                Line($"type {td.FullTypeName.ReflectionName} kind={td.Kind} acc={td.Accessibility} static={B(td.IsStatic)} abstract={B(td.IsAbstract)} sealed={B(td.IsSealed)} reftype={Ref(td.IsReferenceType)} byreflike={B(td.IsByRefLike)} readonly={B(td.IsReadOnly)} record={B(td.IsRecord)} ktc={td.KnownTypeCode} nullctx={td.NullableContext} ext={B(td.HasExtensions)} meta={Esc(td.MetadataName)} decl={(td.DeclaringTypeDefinition?.FullTypeName.ReflectionName ?? "-")}" + (td.Kind == TypeKind.Enum ? " enumtype=" + T(td.EnumUnderlyingType) : ""));
                TypeParameters("  ", td.TypeParameters, td.DeclaringTypeDefinition?.TypeParameterCount ?? 0);
                foreach (var b in td.DirectBaseTypes) Line("  base " + T(b));
                Attrs("  ", td.GetAttributes());
                foreach (var member in td.Members) Guard("  ", () => Member(module, member));
            });
        }
        foreach (var h in md.MemberReferences) Guard("", () => Line($"ref {MetadataTokens.GetToken(h):x8} {M(module.ResolveEntity(h) as IMember)}"));
        for (int row = 1; row <= md.GetTableRowCount(TableIndex.MethodSpec); row++)
        {
            var h = MetadataTokens.MethodSpecificationHandle(row);
            Guard("", () => Line($"spec {MetadataTokens.GetToken(h):x8} {M(module.ResolveEntity(h) as IMember)}"));
        }
        foreach (var h in md.TypeReferences) Guard("", () => Line($"tref {MetadataTokens.GetToken(h):x8} {T(module.ResolveType(h, default))}"));
        for (int row = 1; row <= md.GetTableRowCount(TableIndex.TypeSpec); row++)
        {
            var h = MetadataTokens.TypeSpecificationHandle(row);
            Guard("", () => Line($"tspec {MetadataTokens.GetToken(h):x8} {T(module.ResolveType(h, default))}"));
        }
        var stdout = Console.OpenStandardOutput();
        var bytes = new UTF8Encoding(false).GetBytes(o.ToString());
        stdout.Write(bytes, 0, bytes.Length);
        return 0;
    }
}
