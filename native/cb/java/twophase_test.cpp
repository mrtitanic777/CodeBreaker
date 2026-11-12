#include "cb/java/twophase.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/java/parse.hpp"

namespace {

namespace tp = cb::java::twophase;
namespace jp = cb::java::parse;
using cb::java::ir::JClass;

// A hierarchy where Block's constructor reads its own virtual isOpaqueCube() -- the Java rule C++ gets wrong.
constexpr const char* kBlock = R"(public class Block {
	public static final boolean[] opaque = new boolean[16];
	public final int blockID;
	public int light = 3;

	protected Block(int var1) {
		this.blockID = var1;
		opaque[var1] = this.isOpaqueCube();
		this.light = this.lightValue() + this.light;
	}

	protected Block(int var1, int var2) {
		this(var1);
		this.light += var2;
	}

	public boolean isOpaqueCube() {
		return true;
	}

	public int lightValue() {
		return 0;
	}

	public static boolean opaqueAt(int var0) {
		return opaque[var0 & 15];
	}
}
)";

constexpr const char* kGlass = R"(public class BlockGlass extends Block {
	public int extra = 5;

	public BlockGlass(int var1) {
		super(var1, 2);
		this.extra += var1;
	}

	public boolean isOpaqueCube() {
		return false;
	}

	public int lightValue() {
		return this.extra + 10;
	}
}
)";

constexpr const char* kPlain = R"(public class Plain {
	public int v;

	public Plain(int var1) {
		this.v = var1;
	}

	public int get() {
		return this.v;
	}
}
)";

std::vector<JClass> classes(std::initializer_list<const char*> sources) {
    std::vector<JClass> out;
    for (const char* s : sources) out.push_back(jp::parse_source(s));
    return out;
}

CB_TEST("java.twophase.compute_picks_the_hierarchy_with_the_virtual_call", [] {
    const tp::NameSet set = tp::compute(classes({kBlock, kGlass, kPlain}));
    CB_CHECK(set == (tp::NameSet{"Block", "BlockGlass"}));
});

CB_TEST("java.twophase.a_call_nobody_overrides_does_not_count", [] {
    // Plain calls its own helper, but no subclass overrides it and it is not abstract
    const auto cs = classes({"class P {\n int v;\n P() {\n this.v = helper();\n }\n int helper() {\n return 1;\n }\n}\n",
                             "class Q extends P {\n void other() {\n }\n}\n"});
    CB_CHECK(tp::compute(cs).empty());
});

CB_TEST("java.twophase.an_abstract_method_called_by_the_constructor_counts", [] {
    const auto cs = classes({"abstract class E {\n E() {\n entityInit();\n }\n abstract void entityInit();\n}\n",
                             "class M extends E {\n void entityInit() {\n }\n}\n"});
    CB_CHECK(tp::compute(cs) == (tp::NameSet{"E", "M"}));
});

CB_TEST("java.twophase.keywords_and_calls_on_other_objects_are_not_own_calls", [] {
    const auto cs = classes({"class P {\n P() {\n if (x) {\n other.virt();\n }\n while (y) {\n }\n return;\n }\n void virt() {\n }\n}\n",
                             "class Q extends P {\n void virt() {\n }\n}\n"});
    CB_CHECK(tp::compute(cs).empty());   // `other.virt()` is a call on another object
});

CB_TEST("java.twophase.field_initializers_and_instance_blocks_run_in_the_constructor", [] {
    const auto cs = classes({"class P {\n int a = make();\n P() {\n }\n int make() {\n return 1;\n }\n}\n",
                             "class Q extends P {\n int make() {\n return 2;\n }\n}\n",
                             "class R {\n int z;\n {\n setup();\n }\n void setup() {\n }\n}\n",
                             "class S extends R {\n void setup() {\n }\n}\n"});
    CB_CHECK(tp::compute(cs) == (tp::NameSet{"P", "Q", "R", "S"}));
});

CB_TEST("java.twophase.value_types_and_roots_constructed_with_super_args_are_left_out", [] {
    // a value type is never built in two phases (with its subtree)
    const tp::NameSet no_values = tp::compute(classes({kBlock, kGlass}));
    CB_CHECK(no_values.contains("Block") && no_values.contains("BlockGlass"));
    CB_CHECK(tp::compute(classes({kBlock, kGlass}), {"Block"}).empty());
    // a subclass whose user-class parent is not itself two-phase and is constructed with arguments is dropped
    const auto cs = classes({"class Base {\n Base(int a) {\n }\n}\n",
                             "class Mid extends Base {\n Mid() {\n super(1);\n go();\n }\n void go() {\n }\n}\n",
                             "class Leaf extends Mid {\n void go() {\n }\n}\n"});
    CB_CHECK(tp::compute(cs).empty());   // Mid would be a root, but Base's constructor (with arguments) would have to run in Mid()
    // the same root with a no-argument super() is kept, with its subtree
    const auto ok = classes({"class Base {\n Base() {\n }\n}\n",
                             "class Mid extends Base {\n Mid() {\n super();\n go();\n }\n void go() {\n }\n}\n",
                             "class Leaf extends Mid {\n void go() {\n }\n}\n"});
    CB_CHECK(tp::compute(ok) == (tp::NameSet{"Mid", "Leaf"}));
});

CB_TEST("java.twophase.a_parent_cycle_does_not_hang", [] {
    const auto cs = classes({"class A extends B {\n A() {\n f();\n }\n void f() {\n }\n}\n", "class B extends A {\n void f() {\n }\n}\n"});
    CB_CHECK_NOTHROW((void)tp::compute(cs));
});

CB_TEST("java.twophase.rewrite_news_is_balanced_and_idempotent", [] {
    const tp::Rewritten once = tp::rewrite_news("x = new Block(f(a, b), (c)); y = new Block[4]; z = new BlockX(1);", {"Block"});
    CB_CHECK_EQ(once.count, static_cast<std::size_t>(1));
    CB_CHECK_EQ(once.text, "x = (new Block())->j2xInit(f(a, b), (c)); y = new Block[4]; z = new BlockX(1);");
    const tp::Rewritten again = tp::rewrite_news(once.text, {"Block"});
    CB_CHECK_EQ(again.count, static_cast<std::size_t>(0));
    CB_CHECK_EQ(again.text, once.text);
});

CB_TEST("java.twophase.rewrite_news_handles_nesting_strings_and_spacing", [] {
    const tp::NameSet names = {"A", "BlockGlass"};
    const tp::Rewritten nested = tp::rewrite_news("p = new A(new A(1), \"a)b(\", ')');", names);
    CB_CHECK_EQ(nested.count, static_cast<std::size_t>(2));
    CB_CHECK_EQ(nested.text, "p = (new A())->j2xInit((new A())->j2xInit(1), \"a)b(\", ')');");
    const tp::Rewritten spaced = tp::rewrite_news("q = new \tBlockGlass (7);", names);
    CB_CHECK_EQ(spaced.text, "q = (new BlockGlass())->j2xInit(7);");
    CB_CHECK_EQ(tp::rewrite_news("r = renew A(1); s = anew A(2);", names).count, static_cast<std::size_t>(0));   // not the keyword
    CB_CHECK_EQ(tp::rewrite_news("t = new A(1", names).count, static_cast<std::size_t>(0));   // an unclosed call is left alone
    CB_CHECK_EQ(tp::rewrite_news("no construction here", names).text, "no construction here");
    CB_CHECK_EQ(tp::rewrite_news("x = new A(1);", {}).count, static_cast<std::size_t>(0));
});

CB_TEST("java.twophase.rewrite_stack_temporaries", [] {
    const tp::Rewritten r = tp::rewrite_stack("{ Block _j2xleak0(3); _j2xleak0.go(); }", {"Block"});
    CB_CHECK_EQ(r.count, static_cast<std::size_t>(1));
    CB_CHECK_EQ(r.text, "{ Block _j2xleak0; _j2xleak0.j2xInit(3); _j2xleak0.go(); }");
    const tp::Rewritten bare = tp::rewrite_stack("Block _j2xleak12;\nBlock _j2xleak13 ( f(1) , 2 ) ;\n", {"Block"});
    CB_CHECK_EQ(bare.count, static_cast<std::size_t>(2));
    CB_CHECK_EQ(bare.text, "Block _j2xleak12; _j2xleak12.j2xInit();\nBlock _j2xleak13; _j2xleak13.j2xInit( f(1) , 2 );\n");
    // not a stack temporary: a pointer, a reference, a template argument, a scoped name, another type, another variable name
    for (const char* text : {"Block* _j2xleak0;", "Block& _j2xleak0 = x;", "std::vector<Block _j2xleak0;", "N::Block _j2xleak0;",
                             "Other _j2xleak0;", "Block local;", "XBlock _j2xleak0;"}) {
        CB_CHECK_EQ(tp::rewrite_stack(text, {"Block"}).count, static_cast<std::size_t>(0));
        CB_CHECK_EQ(tp::rewrite_stack(text, {"Block"}).text, text);
    }
});

CB_TEST("java.twophase.run_rewrites_the_emitted_tree_byte_for_byte", [] {
    cb::fs::ScopedTempDir tmp("cbtwophase");
    const std::string crlf_site = "void f() {\r\n\tBlock* b = new Block(1);\r\n}\r\n";   // CRLF line ends are kept
    cb::fs::write_text(tmp.path() / "A.cpp", crlf_site);
    cb::fs::write_text(tmp.path() / "B.h", "Block* make();\n");                           // no site: not written
    cb::fs::write_text(tmp.path() / "C.cpp", "Block _j2xleak0(2);\nauto x = new Block(3);\n");
    cb::fs::write_text(tmp.path() / "notes.txt", "new Block(1)\n");                       // not a source file
    const tp::RunResult r = tp::run(tmp.path(), {"Block"});
    CB_CHECK_EQ(r.sites, static_cast<std::size_t>(3));
    CB_CHECK_EQ(cb::text::join(r.files, ","), "A.cpp,C.cpp");
    CB_CHECK_EQ(cb::fs::read_text(tmp.path() / "A.cpp"), "void f() {\r\n\tBlock* b = (new Block())->j2xInit(1);\r\n}\r\n");
    CB_CHECK_EQ(cb::fs::read_text(tmp.path() / "B.h"), "Block* make();\n");
    CB_CHECK_EQ(cb::fs::read_text(tmp.path() / "C.cpp"), "Block _j2xleak0; _j2xleak0.j2xInit(2);\nauto x = (new Block())->j2xInit(3);\n");
    CB_CHECK_EQ(cb::fs::read_text(tmp.path() / "notes.txt"), "new Block(1)\n");
    // a construction already rewritten is left as it is
    const tp::RunResult again = tp::run(tmp.path(), {"Block"});
    CB_CHECK_EQ(cb::fs::read_text(tmp.path() / "A.cpp"), "void f() {\r\n\tBlock* b = (new Block())->j2xInit(1);\r\n}\r\n");
    CB_CHECK(!again.files.empty() && again.files[0] == "C.cpp");   // (only the stack temporary of C.cpp matches again)
    CB_CHECK_EQ(tp::run(tmp.path(), {}).sites, static_cast<std::size_t>(0));   // no two-phase classes: nothing
});

}  // namespace
