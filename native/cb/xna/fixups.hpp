// cb/xna/fixups.hpp -- what ILSpy writes for an XNA game that is not valid C#, rewritten as the C# it stands for.
//
//     ref_casts(text)        ((Color)(ref white)).A = 64   ->   (white).A = 64
//     constructors(text)     base._002Ector(a);  ->  `: base(a)` on the constructor;
//                            v._002Ector(a);     ->  v = new T(a);   (T from v's declaration)
//     data_holders(text)     a compiler's static-data holder (<PrivateImplementationDetails>, renamed by an
//                            obfuscator): its private blob structs become internal
//     apply(folder)          all of them, over every .cs file under the folder
//
// Each rewrite gives the same program: a cast of a value to its own type is no cast (the `ref` in it is how the
// decompiler marks a mutable struct's member being reached in place), and a base or this constructor call at the top
// of a constructor is what an initializer is (it runs before the body either way). What a rewrite cannot place with
// certainty is left as it is and listed.
//
// These came from the Arcadecraft decompile (../Arcadecraft/tools/fix_*.py), where they made 65 000 lines of
// decompiled C# compile again. Text is handled as bytes (the C# is UTF-8, the rewrites only look at ASCII), so the
// bytes a rewrite does not touch, line endings included, come back exactly as they went in.
#pragma once

#include <cstddef>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

namespace cb::xna::fixups {

// A text and how many rewrites made it.
struct Rewritten {
    std::string text;
    std::size_t count = 0;
};

// A construct a rewrite saw and could not place.
struct Unplaced {
    std::size_t line = 0;   // 1-based
    std::string why;
};

struct ConstructorsResult {
    std::string text;
    std::size_t count = 0;
    std::vector<Unplaced> left;
};

// ((Color)(ref white)).A = 64   ->   (white).A = 64   (repeated, so ((IDisposable)(Enumerator)(ref e)) goes too).
Rewritten ref_casts(std::string_view text);

// `base._002Ector(a);` at the top of a constructor becomes `: base(a)` after its parameter list (and the call's
// text is dropped); `v._002Ector(a)` becomes `v = new T(a)` with T the type of v's nearest declaration above it.
// `count` is the number of edits made: a base or this call is two (the initializer, and the call's removal). A call
// that is not directly in a constructor body (the constructor already has an initializer, or it is in an `if`) is
// left and listed.
ConstructorsResult constructors(std::string_view text);

// The compiler's holder of static array data: ILSpy writes its fields as
// `internal static StructN X /* Not supported: data(...) */` over private blob structs, which C# refuses (CS0052),
// and turns the code that used the data into array literals. The blob structs become internal; nothing reads them.
Rewritten data_holders(std::string_view text);

// A construct left in a file.
struct LeftInFile {
    std::string file;   // relative to the folder, '/' separated
    std::size_t line = 0;
    std::string why;
};

struct Report {
    std::size_t ref_casts = 0;
    std::size_t constructors = 0;
    std::size_t data_holders = 0;
    std::vector<LeftInFile> left;   // files in path order
};

// All the rewrites over every .cs file under `folder` (a file that changes is written back, without its BOM;
// one that does not is not touched). Throws cb::IoError when a file cannot be read or written.
Report apply(const std::filesystem::path& folder);

}  // namespace cb::xna::fixups
