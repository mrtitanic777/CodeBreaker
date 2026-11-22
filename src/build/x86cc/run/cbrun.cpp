/* cbrun.cpp -- see cbrun.hpp: the start and the end of a run of the compiler, and what a run owns besides its state: the memory
 * it allocates and the files it opens. A compile that ends in a fatal error leaves by longjmp, past the code that frees them
 * (a process that ended freed them for it), so the run keeps the list and releases what is left when it ends. */
#include "cbrun.hpp"

#include <cstdio>
#include <cstdlib>
#include <unordered_set>
#include <vector>

#undef malloc   /* (cbcxx.h renamed them for the compilers: here are the real ones) */
#undef calloc
#undef realloc
#undef free
#undef fopen
#undef fclose

namespace {

struct Owned {
    std::unordered_set<void*> blocks;   /* the memory handed out by cbrun_malloc & co. and not freed */
    std::unordered_set<FILE*> files;    /* the files opened by cbrun_fopen and not closed */
};

Owned& owned() {
    static Owned all;   /* (the lists of what the run owns: empty between runs) */
    return all;
}

}  // namespace

cbrun_parts& cbrun_registry() {
    static cbrun_parts all;   /* (the list of what to destroy: not a run's state, and empty between runs) */
    return all;
}

namespace {
std::vector<bool> made;   /* which parts of the registry the last CBRunBegin made (so CBRunEnd destroys those) */
}  // namespace

extern "C" {

void* cbrun_malloc(size_t size) {
    void* p = std::malloc(size);
    if (p != nullptr) owned().blocks.insert(p);
    return p;
}

void* cbrun_calloc(size_t count, size_t size) {
    void* p = std::calloc(count, size);
    if (p != nullptr) owned().blocks.insert(p);
    return p;
}

void* cbrun_realloc(void* old, size_t size) {
    void* p = std::realloc(old, size);
    if (p != nullptr) {
        if (old != nullptr && p != old) owned().blocks.erase(old);
        owned().blocks.insert(p);
    } else if (size == 0 && old != nullptr) {
        owned().blocks.erase(old);
    }
    return p;
}

void cbrun_free(void* p) {
    if (p != nullptr) owned().blocks.erase(p);
    std::free(p);
}

FILE* cbrun_fopen(const char* name, const char* mode) {
    FILE* f = std::fopen(name, mode);
    if (f != nullptr) owned().files.insert(f);
    return f;
}

int cbrun_fclose(FILE* f) {
    if (f != nullptr) owned().files.erase(f);
    return std::fclose(f);
}

/* a run ends: the files and memory it still holds are given back, and every part of its state is destroyed (newest first: a
 * part may name another's address in its initial value) */
void CBRunEnd(void) {
    Owned& o = owned();
    for (FILE* f : o.files) std::fclose(f);
    o.files.clear();
    for (void* p : o.blocks) std::free(p);
    o.blocks.clear();
    cbrun_parts& all = cbrun_registry();
    for (std::size_t k = all.all.size(); k-- > 0;) {
        if (k < made.size() && made[k]) all.all[k].unmake();
    }
    made.assign(all.all.size(), false);
}

/* a run begins: nothing of an earlier one is left, and every part of the new one is made */
void CBRunBegin(void) {
    CBRunEnd();
    cbrun_parts& all = cbrun_registry();
    made.assign(all.all.size(), true);
    for (const cbrun_part_ops& op : all.all) op.make();
}

}  // extern "C"
