/* cbrun.hpp -- the state of one run of the compiler (wcc or wcc386), made new when the host starts the compiler and gone when it
 * finishes it.
 *
 * OpenWatcom's compilers kept it in globals and statics, so a process that compiled twice carried the first compile's flags, lists
 * and counters into the second. Here it is a context passed through the functions: the compiler is four parts, each with its own
 * (a header of its own states the variables of its files, and no part's headers fit another's: the inline assembler's tokens are
 * the scanner's names for other things):
 *   FrontRun    the front end         front/driver/frontrun.hpp
 *   BackRun     the code generator    codegen/base/backrun.hpp     (with its libraries: cfloat, dwarf, owl)
 *   AsmRun      the inline assembler  asm/asmrun.hpp
 *   SupportRun  runtime support       support/supportrun.hpp
 * A function of one part takes that part's context first (`FrontRun& front`) and reaches another part's through it (`front.back`).
 * RunSet holds the four of one run: cbrun_begin() makes them, the IDE entry points of the compiler (front/driver/ideentry.cpp) keep
 * it as the handle the host passes back, and cbrun_end() frees them and what the run still holds (cbrun.cpp). Each target's symbols
 * carry its own prefix (cbnames.h), so the two compilers in one program keep their runs apart. */
#ifndef CB_RUN_HPP
#define CB_RUN_HPP

#include <cstddef>
#include <cstdio>
#include <unordered_set>

/* what a run holds besides its state: the memory it allocates and the files it opens. A compile that ends in a fatal error leaves
 * by longjmp, past the code that frees them (a process that ended freed them for it), so the run keeps the list (the compiler calls
 * these where it called malloc, free, fopen ... ) and releases what is left when it ends. */
struct Held {
    std::unordered_set<void*> blocks;   /* the memory handed out and not freed */
    std::unordered_set<std::FILE*> files;   /* the files opened and not closed */

    void* malloc(std::size_t size);
    void* calloc(std::size_t count, std::size_t size);
    void* realloc(void* old, std::size_t size);
    void free(void* block);
    std::FILE* fopen(const char* name, const char* mode);
    int fclose(std::FILE* file);
    void release();   /* gives back what is left */
    ~Held() { release(); }
};

struct FrontRun;
struct BackRun;
struct AsmRun;
struct SupportRun;
struct AsminlinState;   /* (what the front end and the assembler share: the inline assembly being built) */

struct RunSet {
    Held held;
    AsminlinState* asminlin = nullptr;
    FrontRun* front = nullptr;
    BackRun* back = nullptr;
    AsmRun* assembler = nullptr;
    SupportRun* support = nullptr;
};

/* the front end of the run a code generator (or library of it) names: for the callbacks of a library the front end gives to it */
FrontRun& front_of(BackRun& back);

/* the run of a compile: made new, or the one the host kept from the last compile (what is left of it is in the state of a new one) */
RunSet* cbrun_begin(RunSet* kept);
/* the run ends: what it holds is given back and its state is new again, for the next compile (the host keeps the run) */
void cbrun_end(RunSet* run);
/* the host has no more compiles: the run is freed */
void cbrun_free(RunSet* run);

#endif
