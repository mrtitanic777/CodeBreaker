// CsTreeDump -- the C# front end's eyes: Roslyn parses and binds a set of C# files, and each file is
// written out as JSON with what the compiler resolved about it. Python lowers the JSON into the
// translator's IR (codebreaker/translate/frontends/csharp/lower.py).
//
// Declarations come from the syntax and the symbols the semantic model gives for them (types, fields,
// properties, methods, constructors, enum members, a record's parameters). Bodies and initialisers come
// from Roslyn's IOperation tree: the bound form, in which every call names the overload chosen and its
// arguments are in parameter order (defaults and `params` arrays filled in), every implicit conversion
// is a node, `foreach`, `using`, `lock`, string interpolation, patterns and LINQ's query syntax are
// already the calls and operations they mean. The walk over operations is generic: for each operation
// it reads the properties of the Microsoft.CodeAnalysis.Operations interfaces the operation implements,
// so every operation kind Roslyn knows is dumped without being named here.
//
//   dotnet CsTreeDump.dll --out DIR --refs DIR [--langversion V] @files.txt | files...
//
// One DIR/<file name>.json per source file, and DIR/_diagnostics.json with the compiler's errors and
// warnings. Errors do not stop the dump: an unresolved type is written as {"k":"error"}.
using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.Immutable;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp;
using Microsoft.CodeAnalysis.CSharp.Syntax;
using Microsoft.CodeAnalysis.Operations;

public static class CsTreeDump
{
    static SemanticModel model;
    static readonly Dictionary<Type, List<PropertyInfo>> props = new Dictionary<Type, List<PropertyInfo>>();
    static readonly HashSet<string> skip = new HashSet<string> {
        "Parent", "ChildOperations", "Children", "SemanticModel", "Syntax", "Language", "Kind", "Type",
        "ConstantValue", "IsImplicit" };

    public static int Main(string[] args)
    {
        string outDir = null, refs = null, langVersion = null;
        var files = new List<string>();
        for (int i = 0; i < args.Length; i++)
        {
            string a = args[i];
            if (a == "--out") outDir = args[++i];
            else if (a == "--refs") refs = args[++i];
            else if (a == "--langversion") langVersion = args[++i];
            else if (a.StartsWith("@"))
                files.AddRange(File.ReadAllLines(a.Substring(1)).Where(l => l.Trim().Length > 0).Select(l => l.Trim()));
            else files.Add(a);
        }
        if (outDir == null || refs == null || files.Count == 0)
        {
            Console.Error.WriteLine("usage: CsTreeDump --out DIR --refs DIR [--langversion V] @files | files...");
            return 2;
        }
        var lv = LanguageVersion.Latest;
        if (langVersion != null) LanguageVersionFacts.TryParse(langVersion, out lv);
        var opts = new CSharpParseOptions(lv);
        var trees = files.Select(f => CSharpSyntaxTree.ParseText(File.ReadAllText(f), opts, f, Encoding.UTF8)).ToList();
        var refList = Directory.GetFiles(refs, "*.dll").Select(p => (MetadataReference)MetadataReference.CreateFromFile(p)).ToList();
        var comp = CSharpCompilation.Create("CsTreeDump", trees, refList,
            new CSharpCompilationOptions(OutputKind.ConsoleApplication, allowUnsafe: true));
        Directory.CreateDirectory(outDir);
        int n = 0;
        foreach (var tree in trees)
        {
            model = comp.GetSemanticModel(tree);
            var sb = new StringBuilder(1 << 16);
            Unit(sb, (CompilationUnitSyntax)tree.GetRoot(), comp);
            string dest = Path.Combine(outDir, Path.GetFileNameWithoutExtension(tree.FilePath) + ".json");
            File.WriteAllText(dest, sb.ToString(), new UTF8Encoding(false));
            n++;
        }
        var d = new StringBuilder("[");
        bool first = true;
        foreach (var dg in comp.GetDiagnostics())
        {
            if (dg.Severity < DiagnosticSeverity.Warning) continue;
            if (!first) d.Append(',');
            first = false;
            var ls = dg.Location.GetLineSpan();
            d.Append("{\"kind\":").Append(Str(dg.Severity.ToString().ToUpperInvariant()))
             .Append(",\"id\":").Append(Str(dg.Id))
             .Append(",\"file\":").Append(Str(ls.Path ?? ""))
             .Append(",\"line\":").Append(ls.StartLinePosition.Line + 1)
             .Append(",\"message\":").Append(Str(dg.GetMessage())).Append('}');
        }
        File.WriteAllText(Path.Combine(outDir, "_diagnostics.json"), d.Append(']').ToString(), new UTF8Encoding(false));
        Console.WriteLine("dumped " + n + " files");
        return 0;
    }

    // ---------------------------------------------------------------------------------- declarations
    static void Unit(StringBuilder sb, CompilationUnitSyntax cu, CSharpCompilation comp)
    {
        sb.Append("{\"k\":\"unit\",\"path\":").Append(Str(cu.SyntaxTree.FilePath));
        sb.Append(",\"usings\":[");
        Join(sb, cu.Usings, (b, u) => b.Append("{\"name\":").Append(Str(u.Name == null ? "" : u.Name.ToString()))
            .Append(",\"static\":").Append(u.StaticKeyword.IsKind(SyntaxKind.StaticKeyword) ? "true" : "false")
            .Append(",\"alias\":").Append(u.Alias == null ? "null" : Str(u.Alias.Name.ToString())).Append('}'));
        sb.Append("],\"types\":[");
        bool first = true;
        foreach (var m in Types(cu.Members))
        {
            if (!first) sb.Append(',');
            first = false;
            TypeDecl(sb, m);
        }
        sb.Append(']');
        // top-level statements: the compiler's synthesized entry point
        var globals = cu.Members.OfType<GlobalStatementSyntax>().ToList();
        if (globals.Count > 0)
        {
            sb.Append(",\"main\":");
            Op(sb, model.GetOperation(cu));
            var entry = comp.GetEntryPoint(default);
            if (entry != null) sb.Append(",\"mainSym\":").Append(Sym(entry));
        }
        sb.Append('}');
    }

    static IEnumerable<MemberDeclarationSyntax> Types(SyntaxList<MemberDeclarationSyntax> members)
    {
        foreach (var m in members)
        {
            if (m is BaseNamespaceDeclarationSyntax ns)
            {
                foreach (var t in Types(ns.Members)) yield return t;
            }
            else if (m is BaseTypeDeclarationSyntax || m is DelegateDeclarationSyntax) yield return m;
        }
    }

    static void Pos(StringBuilder sb, SyntaxNode n)
    {
        var p = n.GetLocation().GetLineSpan().StartLinePosition;
        sb.Append(",\"l\":").Append(p.Line + 1).Append(",\"c\":").Append(p.Character + 1);
    }

    static void Mods(StringBuilder sb, SyntaxTokenList mods)
    {
        sb.Append(",\"mods\":[");
        Join(sb, mods, (b, t) => b.Append(Str(t.Text)));
        sb.Append(']');
    }

    static void TypeDecl(StringBuilder sb, MemberDeclarationSyntax m)
    {
        var s = model.GetDeclaredSymbol(m) as INamedTypeSymbol;
        string kind;
        if (m is DelegateDeclarationSyntax) kind = "delegate";
        else if (m is EnumDeclarationSyntax) kind = "enum";
        else if (m is InterfaceDeclarationSyntax) kind = "interface";
        else if (m is StructDeclarationSyntax) kind = "struct";
        else if (m is RecordDeclarationSyntax r) kind = r.ClassOrStructKeyword.IsKind(SyntaxKind.StructKeyword) ? "record struct" : "record";
        else kind = "class";
        sb.Append("{\"k\":\"type\",\"kind\":").Append(Str(kind));
        Pos(sb, m);
        sb.Append(",\"name\":").Append(Str(s?.Name ?? ""));
        sb.Append(",\"sym\":").Append(s == null ? "null" : Sym(s));
        Mods(sb, m.Modifiers);
        if (s != null)
        {
            sb.Append(",\"typeparams\":[");
            Join(sb, s.TypeParameters, (b, tp) => TypeParam(b, tp));
            sb.Append(']');
            if (s.BaseType != null && kind != "enum" && kind != "delegate" && !kind.Contains("struct"))
                sb.Append(",\"base\":").Append(Ty(s.BaseType));
            if (kind == "enum" && s.EnumUnderlyingType != null)
                sb.Append(",\"underlying\":").Append(Ty(s.EnumUnderlyingType));
            sb.Append(",\"interfaces\":[");
            Join(sb, s.Interfaces, (b, i) => b.Append(Ty(i)));
            sb.Append(']');
            if (kind == "delegate" && s.DelegateInvokeMethod != null)
                sb.Append(",\"invoke\":").Append(Sym(s.DelegateInvokeMethod));
            // what the compiler declares that the source does not: a default constructor, a record's
            // members (its primary constructor, properties, Equals, ToString, Deconstruct, ...)
            sb.Append(",\"implicit\":[");
            Join(sb, s.GetMembers().Where(x => x.IsImplicitlyDeclared && x.Kind != SymbolKind.NamedType), (b, x) => b.Append(Sym(x)));
            sb.Append(']');
        }
        if (m is TypeDeclarationSyntax td && td.ParameterList != null)
        {
            sb.Append(",\"params\":[");
            Join(sb, td.ParameterList.Parameters, (b, p) => Param(b, p));
            sb.Append(']');
        }
        if (td2(m) is TypeDeclarationSyntax tds && tds.BaseList != null)
        {
            // a record's or class's primary-constructor base call: `record B(int X) : A(X)`
            foreach (var bt in tds.BaseList.Types)
                if (bt is PrimaryConstructorBaseTypeSyntax pc)
                {
                    sb.Append(",\"baseCall\":");
                    Op(sb, model.GetOperation(pc));
                }
        }
        sb.Append(",\"members\":[");
        bool first = true;
        if (m is EnumDeclarationSyntax en)
        {
            foreach (var em in en.Members)
            {
                if (!first) sb.Append(',');
                first = false;
                var es = model.GetDeclaredSymbol(em);
                sb.Append("{\"k\":\"enummember\"");
                Pos(sb, em);
                sb.Append(",\"sym\":").Append(Sym(es));
                if (es is IFieldSymbol f && f.HasConstantValue) sb.Append(",\"value\":").Append(Value(f.ConstantValue));
                sb.Append('}');
            }
        }
        else if (m is TypeDeclarationSyntax t)
        {
            foreach (var mm in t.Members)
            {
                if (!first) sb.Append(',');
                first = false;
                Member(sb, mm);
            }
        }
        sb.Append("]}");
    }

    static TypeDeclarationSyntax td2(MemberDeclarationSyntax m) => m as TypeDeclarationSyntax;

    static void TypeParam(StringBuilder b, ITypeParameterSymbol tp)
    {
        b.Append("{\"name\":").Append(Str(tp.Name));
        b.Append(",\"variance\":").Append(Str(tp.Variance.ToString()));
        b.Append(",\"class\":").Append(tp.HasReferenceTypeConstraint ? "true" : "false");
        b.Append(",\"struct\":").Append(tp.HasValueTypeConstraint ? "true" : "false");
        b.Append(",\"new\":").Append(tp.HasConstructorConstraint ? "true" : "false");
        b.Append(",\"bounds\":[");
        Join(b, tp.ConstraintTypes, (bb, c) => bb.Append(Ty(c)));
        b.Append("]}");
    }

    static void Param(StringBuilder b, ParameterSyntax p)
    {
        var ps = model.GetDeclaredSymbol(p);
        b.Append("{\"k\":\"param\"");
        Pos(b, p);
        b.Append(",\"sym\":").Append(ps == null ? "null" : Sym(ps));
        Mods(b, p.Modifiers);
        if (ps != null && ps.HasExplicitDefaultValue) b.Append(",\"default\":").Append(Value(ps.ExplicitDefaultValue));
        b.Append('}');
    }

    static void Params(StringBuilder sb, BaseParameterListSyntax pl)
    {
        sb.Append(",\"params\":[");
        if (pl != null) Join(sb, pl.Parameters, (b, p) => Param(b, p));
        sb.Append(']');
    }

    static void Body(StringBuilder sb, string key, SyntaxNode n)
    {
        if (n == null) return;
        IOperation op = null;
        try { op = model.GetOperation(n); } catch (Exception) { }
        sb.Append(",\"").Append(key).Append("\":");
        Op(sb, op);
    }

    static void Member(StringBuilder sb, MemberDeclarationSyntax m)
    {
        if (m is BaseTypeDeclarationSyntax || m is DelegateDeclarationSyntax)
        {
            TypeDecl(sb, m);
            return;
        }
        if (m is BaseFieldDeclarationSyntax fd)
        {
            sb.Append("{\"k\":").Append(Str(m is EventFieldDeclarationSyntax ? "eventfield" : "field"));
            Pos(sb, m);
            Mods(sb, m.Modifiers);
            sb.Append(",\"vars\":[");
            Join(sb, fd.Declaration.Variables, (b, v) =>
            {
                var vs = model.GetDeclaredSymbol(v);
                b.Append("{\"sym\":").Append(vs == null ? "null" : Sym(vs));
                Pos(b, v);
                if (v.Initializer != null)
                {
                    IOperation init = model.GetOperation(v.Initializer);
                    b.Append(",\"init\":");
                    Op(b, init is ISymbolInitializerOperation si ? si.Value : init);
                }
                b.Append('}');
            });
            sb.Append("]}");
            return;
        }
        if (m is BasePropertyDeclarationSyntax pd)
        {
            var ps = model.GetDeclaredSymbol(pd);
            string kind = m is IndexerDeclarationSyntax ? "indexer" : m is EventDeclarationSyntax ? "event" : "property";
            sb.Append("{\"k\":").Append(Str(kind));
            Pos(sb, m);
            sb.Append(",\"sym\":").Append(ps == null ? "null" : Sym(ps));
            Mods(sb, m.Modifiers);
            if (m is IndexerDeclarationSyntax ix) Params(sb, ix.ParameterList);
            ArrowExpressionClauseSyntax arrow = (m as PropertyDeclarationSyntax)?.ExpressionBody ?? (m as IndexerDeclarationSyntax)?.ExpressionBody;
            if (arrow != null) Body(sb, "expr", arrow);
            if (m is PropertyDeclarationSyntax prop && prop.Initializer != null)
            {
                IOperation init = model.GetOperation(prop.Initializer);
                sb.Append(",\"init\":");
                Op(sb, init is ISymbolInitializerOperation si ? si.Value : init);
            }
            sb.Append(",\"accessors\":[");
            if (pd.AccessorList != null)
                Join(sb, pd.AccessorList.Accessors, (b, a) =>
                {
                    b.Append("{\"kind\":").Append(Str(a.Keyword.Text));
                    Pos(b, a);
                    Mods(b, a.Modifiers);
                    b.Append(",\"auto\":").Append(a.Body == null && a.ExpressionBody == null ? "true" : "false");
                    if (a.Body != null || a.ExpressionBody != null) Body(b, "body", a);
                    b.Append('}');
                });
            sb.Append("]}");
            return;
        }
        if (m is BaseMethodDeclarationSyntax md)
        {
            var ms = model.GetDeclaredSymbol(md) as IMethodSymbol;
            string kind = m is ConstructorDeclarationSyntax ? "ctor" : m is DestructorDeclarationSyntax ? "dtor"
                : m is OperatorDeclarationSyntax ? "operator" : m is ConversionOperatorDeclarationSyntax ? "conversion" : "method";
            sb.Append("{\"k\":").Append(Str(kind));
            Pos(sb, m);
            sb.Append(",\"sym\":").Append(ms == null ? "null" : Sym(ms));
            Mods(sb, m.Modifiers);
            if (ms != null)
            {
                sb.Append(",\"typeparams\":[");
                Join(sb, ms.TypeParameters, (b, tp) => TypeParam(b, tp));
                sb.Append(']');
            }
            Params(sb, md.ParameterList);
            if (md.Body != null || md.ExpressionBody != null) Body(sb, "body", md);
            sb.Append('}');
            return;
        }
        sb.Append("{\"k\":").Append(Str("unsupported:" + m.Kind()));
        Pos(sb, m);
        sb.Append('}');
    }

    // ---------------------------------------------------------------------------------- operations
    static void Op(StringBuilder sb, IOperation op)
    {
        if (op == null)
        {
            sb.Append("null");
            return;
        }
        sb.Append("{\"k\":").Append(Str(op.Kind.ToString()));
        if (op.Syntax != null) Pos(sb, op.Syntax);
        if (op.Type != null) sb.Append(",\"$type\":").Append(Ty(op.Type));
        if (op.IsImplicit) sb.Append(",\"implicit\":true");
        if (op.ConstantValue.HasValue) sb.Append(",\"$const\":").Append(Value(op.ConstantValue.Value));
        if (op is IInvocationOperation inv && inv.TargetMethod.IsGenericMethod && inv.Syntax is InvocationExpressionSyntax ies)
        {
            // were the type arguments written (F<int>(x)) or inferred (F(x))?
            SimpleNameSyntax nm = ies.Expression is MemberAccessExpressionSyntax ma ? ma.Name
                : ies.Expression as SimpleNameSyntax;
            if (nm is GenericNameSyntax) sb.Append(",\"typeArgsWritten\":true");
        }
        foreach (var p in PropsOf(op.GetType()))
        {
            object v;
            try { v = p.GetValue(op); } catch (Exception) { continue; }
            if (v == null) continue;
            if (p.Name == "Member" && v is ISymbol) continue;      // (the field, property or method again)
            string name = char.ToLowerInvariant(p.Name[0]) + p.Name.Substring(1);
            if (v is IOperation child)
            {
                sb.Append(",\"").Append(name).Append("\":");
                Op(sb, child);
            }
            else if (v is ITypeSymbol ts)
                sb.Append(",\"").Append(name).Append("\":").Append(Ty(ts));
            else if (v is ISymbol s)
                sb.Append(",\"").Append(name).Append("\":").Append(Sym(s));
            else if (v is CommonConversion cv)
                sb.Append(",\"").Append(name).Append("\":").Append(Conv(cv));
            else if (v is string str)
                sb.Append(",\"").Append(name).Append("\":").Append(Str(str));
            else if (v is bool bl)
                sb.Append(",\"").Append(name).Append("\":").Append(bl ? "true" : "false");
            else if (v is Enum)
                sb.Append(",\"").Append(name).Append("\":").Append(Str(v.ToString()));
            else if (v is int || v is long)
                sb.Append(",\"").Append(name).Append("\":").Append(v);
            else if (p.PropertyType.IsGenericType && p.PropertyType.GetGenericTypeDefinition() == typeof(ImmutableArray<>))
            {
                if ((bool)p.PropertyType.GetProperty("IsDefault").GetValue(v)) continue;
                var items = ((IEnumerable)v).Cast<object>().ToList();
                sb.Append(",\"").Append(name).Append("\":[");
                for (int i = 0; i < items.Count; i++)
                {
                    if (i > 0) sb.Append(',');
                    var it = items[i];
                    if (it is IOperation io) Op(sb, io);
                    else if (it is ITypeSymbol its) sb.Append(Ty(its));
                    else if (it is ISymbol isy) sb.Append(Sym(isy));
                    else sb.Append(Str(it == null ? "" : it.ToString()));
                }
                sb.Append(']');
            }
        }
        sb.Append('}');
    }

    static List<PropertyInfo> PropsOf(Type t)
    {
        if (props.TryGetValue(t, out var ps)) return ps;
        var found = new SortedDictionary<string, PropertyInfo>(StringComparer.Ordinal);
        foreach (var i in t.GetInterfaces())
        {
            if (i.Namespace != "Microsoft.CodeAnalysis.Operations" || !i.IsPublic) continue;
            foreach (var p in i.GetProperties())
                if (!skip.Contains(p.Name) && p.GetIndexParameters().Length == 0 && !found.ContainsKey(p.Name))
                    found[p.Name] = p;
        }
        ps = found.Values.ToList();
        props[t] = ps;
        return ps;
    }

    static string Conv(CommonConversion c)
    {
        var b = new StringBuilder("{");
        b.Append("\"exists\":").Append(c.Exists ? "true" : "false");
        b.Append(",\"identity\":").Append(c.IsIdentity ? "true" : "false");
        b.Append(",\"implicit\":").Append(c.IsImplicit ? "true" : "false");
        b.Append(",\"numeric\":").Append(c.IsNumeric ? "true" : "false");
        b.Append(",\"reference\":").Append(c.IsReference ? "true" : "false");
        b.Append(",\"nullable\":").Append(c.IsNullable ? "true" : "false");
        b.Append(",\"userDefined\":").Append(c.IsUserDefined ? "true" : "false");
        if (c.MethodSymbol != null) b.Append(",\"method\":").Append(Sym(c.MethodSymbol));
        return b.Append('}').ToString();
    }

    // ---------------------------------------------------------------------------------- symbols and types
    static string Qualified(ISymbol s)
    {
        if (s == null) return "";
        if (s is INamespaceSymbol ns && ns.IsGlobalNamespace) return "";
        string outer = s.ContainingType != null ? Qualified(s.ContainingType)
            : s.ContainingNamespace != null ? Qualified(s.ContainingNamespace) : "";
        return outer.Length > 0 ? outer + "." + s.Name : s.Name;
    }

    static string Sym(ISymbol s)
    {
        if (s == null) return "null";
        var b = new StringBuilder("{\"kind\":").Append(Str(s.Kind.ToString()));
        b.Append(",\"name\":").Append(Str(s.Name));
        if (s is ILabelSymbol) return b.Append('}').ToString();
        ISymbol owner = s.ContainingType ?? (ISymbol)s.ContainingNamespace;
        if (s.Kind == SymbolKind.Local || s.Kind == SymbolKind.Parameter || s.Kind == SymbolKind.RangeVariable)
            owner = s.ContainingSymbol;
        if (owner != null) b.Append(",\"owner\":").Append(Str(owner is IMethodSymbol om ? Qualified(om.ContainingType) + "." + om.Name : Qualified(owner)));
        if (s.IsStatic) b.Append(",\"static\":true");
        if (s.IsVirtual) b.Append(",\"virtual\":true");
        if (s.IsAbstract) b.Append(",\"abstract\":true");
        if (s.IsOverride) b.Append(",\"override\":true");
        if (s.IsSealed) b.Append(",\"sealed\":true");
        if (s.IsImplicitlyDeclared) b.Append(",\"implicit\":true");
        if (s.DeclaredAccessibility != Accessibility.NotApplicable) b.Append(",\"access\":").Append(Str(s.DeclaredAccessibility.ToString()));
        b.Append(",\"source\":").Append(s.Locations.Any(l => l.IsInSource) ? "true" : "false");
        switch (s)
        {
            case IMethodSymbol m:
                b.Append(",\"mk\":").Append(Str(m.MethodKind.ToString()));
                var def = m.ReducedFrom ?? m.OriginalDefinition;
                b.Append(",\"ret\":").Append(Ty(m.ReturnType));
                b.Append(",\"params\":[");
                Join(b, m.Parameters, (bb, p) => bb.Append(Ty(p.Type)));
                b.Append("],\"pnames\":[");
                Join(b, m.Parameters, (bb, p) => bb.Append(Str(p.Name)));
                b.Append("],\"refs\":[");
                Join(b, m.Parameters, (bb, p) => bb.Append(Str(p.RefKind.ToString())));
                b.Append(']');
                if (m.Parameters.Length > 0 && m.Parameters[m.Parameters.Length - 1].IsParams) b.Append(",\"varargs\":true");
                if (m.TypeArguments.Length > 0 && !Enumerable.Range(0, m.TypeArguments.Length).All(i => SymbolEqualityComparer.Default.Equals(m.TypeArguments[i], m.TypeParameters[i])))
                {
                    b.Append(",\"typeargs\":[");
                    Join(b, m.TypeArguments, (bb, t) => bb.Append(Ty(t)));
                    b.Append(']');
                }
                if (m.TypeParameters.Length > 0)
                {
                    b.Append(",\"typeparams\":[");
                    Join(b, m.TypeParameters, (bb, t) => bb.Append(Str(t.Name)));
                    b.Append(']');
                }
                if (m.IsExtensionMethod) b.Append(",\"ext\":true");
                if (m.ReducedFrom != null) b.Append(",\"reduced\":true");
                if (m.AssociatedSymbol != null) b.Append(",\"assoc\":").Append(Str(m.AssociatedSymbol.Name));
                if (m.IsAsync) b.Append(",\"async\":true");
                if (m.ReturnsVoid) b.Append(",\"void\":true");
                if (m.ContainingType != null) b.Append(",\"ownerType\":").Append(Ty(m.ContainingType));
                break;
            case IPropertySymbol p:
                b.Append(",\"type\":").Append(Ty(p.Type));
                if (p.IsIndexer)
                {
                    b.Append(",\"indexer\":true,\"params\":[");
                    Join(b, p.Parameters, (bb, q) => bb.Append(Ty(q.Type)));
                    b.Append(']');
                }
                if (p.SetMethod == null) b.Append(",\"readonly\":true");
                else if (p.SetMethod.IsInitOnly) b.Append(",\"initonly\":true");
                if (p.ContainingType != null) b.Append(",\"ownerType\":").Append(Ty(p.ContainingType));
                break;
            case IFieldSymbol f:
                b.Append(",\"type\":").Append(Ty(f.Type));
                if (f.IsConst) b.Append(",\"const\":true,\"value\":").Append(Value(f.ConstantValue));
                if (f.IsReadOnly) b.Append(",\"readonly\":true");
                if (f.AssociatedSymbol != null) b.Append(",\"assoc\":").Append(Str(f.AssociatedSymbol.Name));
                if (f.ContainingType != null) b.Append(",\"ownerType\":").Append(Ty(f.ContainingType));
                break;
            case IEventSymbol e:
                b.Append(",\"type\":").Append(Ty(e.Type));
                break;
            case ILocalSymbol l:
                b.Append(",\"type\":").Append(Ty(l.Type));
                if (l.IsConst) b.Append(",\"const\":true");
                if (l.RefKind != RefKind.None) b.Append(",\"ref\":").Append(Str(l.RefKind.ToString()));
                break;
            case IParameterSymbol p:
                b.Append(",\"type\":").Append(Ty(p.Type));
                b.Append(",\"ref\":").Append(Str(p.RefKind.ToString()));
                if (p.IsParams) b.Append(",\"varargs\":true");
                b.Append(",\"ordinal\":").Append(p.Ordinal);
                break;
            case INamedTypeSymbol t:
                b.Append(",\"qname\":").Append(Str(Qualified(t)));
                b.Append(",\"tk\":").Append(Str(t.TypeKind.ToString()));
                if (t.IsRecord) b.Append(",\"record\":true");
                b.Append(",\"type\":").Append(Ty(t));
                break;
            case ITypeParameterSymbol tp:
                b.Append(",\"type\":").Append(Ty(tp));
                break;
            case IRangeVariableSymbol rv:
                break;
            case ILabelSymbol lb:
                break;
        }
        return b.Append('}').ToString();
    }

    static readonly Dictionary<SpecialType, string> prims = new Dictionary<SpecialType, string> {
        { SpecialType.System_Boolean, "bool" }, { SpecialType.System_SByte, "sbyte" }, { SpecialType.System_Byte, "byte" },
        { SpecialType.System_Int16, "short" }, { SpecialType.System_UInt16, "ushort" }, { SpecialType.System_Int32, "int" },
        { SpecialType.System_UInt32, "uint" }, { SpecialType.System_Int64, "long" }, { SpecialType.System_UInt64, "ulong" },
        { SpecialType.System_Single, "float" }, { SpecialType.System_Double, "double" }, { SpecialType.System_Char, "char" },
        { SpecialType.System_Void, "void" }, { SpecialType.System_IntPtr, "nint" }, { SpecialType.System_UIntPtr, "nuint" } };

    static string Ty(ITypeSymbol t)
    {
        if (t == null) return "null";
        var b = new StringBuilder("{");
        if (prims.TryGetValue(t.SpecialType, out var p) && !(t is INamedTypeSymbol nt0 && nt0.IsNativeIntegerType && false))
            b.Append("\"k\":\"prim\",\"n\":").Append(Str(p));
        else if (t is IArrayTypeSymbol a)
            b.Append("\"k\":\"array\",\"rank\":").Append(a.Rank).Append(",\"elem\":").Append(Ty(a.ElementType));
        else if (t is IPointerTypeSymbol pt)
            b.Append("\"k\":\"ptr\",\"elem\":").Append(Ty(pt.PointedAtType));
        else if (t is ITypeParameterSymbol tp)
            b.Append("\"k\":\"tv\",\"n\":").Append(Str(tp.Name));
        else if (t.TypeKind == TypeKind.Dynamic)
            b.Append("\"k\":\"dynamic\"");
        else if (t.TypeKind == TypeKind.Error)
            b.Append("\"k\":\"error\",\"n\":").Append(Str(t.ToDisplayString()));
        else if (t is INamedTypeSymbol n)
        {
            b.Append("\"k\":\"named\",\"n\":").Append(Str(Qualified(n.OriginalDefinition)));
            b.Append(",\"tk\":").Append(Str(n.TypeKind.ToString()));
            if (n.TypeArguments.Length > 0)
            {
                b.Append(",\"args\":[");
                Join(b, n.TypeArguments, (bb, x) => bb.Append(Ty(x)));
                b.Append(']');
            }
            if (n.IsTupleType)
            {
                b.Append(",\"tuple\":[");
                Join(b, n.TupleElements, (bb, e) => bb.Append(Str(e.IsExplicitlyNamedTupleElement ? e.Name : "")));
                b.Append(']');
            }
            if (n.IsRecord) b.Append(",\"record\":true");
            if (n.IsAnonymousType) b.Append(",\"anonymous\":true");
        }
        else b.Append("\"k\":\"other\",\"n\":").Append(Str(t.ToDisplayString()));
        if (t.NullableAnnotation == NullableAnnotation.Annotated && t.IsReferenceType) b.Append(",\"nullable\":true");
        return b.Append('}').ToString();
    }

    static string Value(object v)
    {
        switch (v)
        {
            case null: return "{\"t\":\"null\"}";
            case bool x: return "{\"t\":\"bool\",\"v\":" + (x ? "true" : "false") + "}";
            case string s: return "{\"t\":\"string\",\"v\":" + Str(s) + "}";
            case char c: return "{\"t\":\"char\",\"v\":" + (int)c + "}";
            case float f: return "{\"t\":\"float\",\"v\":" + Str(f.ToString("R", System.Globalization.CultureInfo.InvariantCulture)) + "}";
            case double d: return "{\"t\":\"double\",\"v\":" + Str(d.ToString("R", System.Globalization.CultureInfo.InvariantCulture)) + "}";
            case decimal m: return "{\"t\":\"decimal\",\"v\":" + Str(m.ToString(System.Globalization.CultureInfo.InvariantCulture)) + "}";
            case ulong u: return "{\"t\":\"ulong\",\"v\":" + Str(u.ToString()) + "}";
            case long l: return "{\"t\":\"long\",\"v\":" + Str(l.ToString()) + "}";
            default:
                return "{\"t\":" + Str(v.GetType().Name) + ",\"v\":" + Str(Convert.ToString(v, System.Globalization.CultureInfo.InvariantCulture)) + "}";
        }
    }

    // ---------------------------------------------------------------------------------- JSON
    static void Join<T>(StringBuilder sb, IEnumerable<T> items, Action<StringBuilder, T> each)
    {
        bool first = true;
        foreach (var x in items)
        {
            if (!first) sb.Append(',');
            first = false;
            each(sb, x);
        }
    }

    static string Str(string s)
    {
        var b = new StringBuilder(s.Length + 2).Append('"');
        foreach (char ch in s)
        {
            switch (ch)
            {
                case '"': b.Append("\\\""); break;
                case '\\': b.Append("\\\\"); break;
                case '\n': b.Append("\\n"); break;
                case '\r': b.Append("\\r"); break;
                case '\t': b.Append("\\t"); break;
                default:
                    if (ch < 0x20 || (ch >= 0xD800 && ch <= 0xDFFF)) b.Append("\\u").Append(((int)ch).ToString("x4"));
                    else b.Append(ch);
                    break;
            }
        }
        return b.Append('"').ToString();
    }
}
