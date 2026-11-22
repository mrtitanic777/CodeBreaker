/* cbrun.cpp -- see cbrun.hpp: the start and the end of a run of the compiler (its four parts made and linked, then freed), and what
 * a run holds besides its state: the memory it allocates and the files it opens (Held). */
#include "cbrun.hpp"

#include <cstdio>
#include <cstdlib>

FrontRun* front_run_new();
void front_run_link(FrontRun& run, BackRun* back, AsmRun* assembler, SupportRun* support, Held* held, AsminlinState* asminlin);
void front_run_reset(FrontRun* run);
void front_run_delete(FrontRun* run);
BackRun* back_run_new();
void back_run_link(BackRun& run, FrontRun* front, AsmRun* assembler, SupportRun* support, Held* held, AsminlinState* asminlin);
void back_run_reset(BackRun* run);
void back_run_delete(BackRun* run);
AsmRun* asm_run_new();
void asm_run_link(AsmRun& run, FrontRun* front, BackRun* back, SupportRun* support, Held* held, AsminlinState* asminlin);
void asm_run_reset(AsmRun* run);
void asm_run_delete(AsmRun* run);
SupportRun* support_run_new();
void support_run_link(SupportRun& run, FrontRun* front, BackRun* back, AsmRun* assembler, Held* held, AsminlinState* asminlin);
void support_run_reset(SupportRun* run);
void support_run_delete(SupportRun* run);
AsminlinState* asminlin_new();
void asminlin_reset(AsminlinState* part);
void asminlin_delete(AsminlinState* part);

void* Held::malloc(std::size_t size) {
    void* p = std::malloc(size);
    if (p != nullptr) blocks.insert(p);
    return p;
}

void* Held::calloc(std::size_t count, std::size_t size) {
    void* p = std::calloc(count, size);
    if (p != nullptr) blocks.insert(p);
    return p;
}

void* Held::realloc(void* old, std::size_t size) {
    void* p = std::realloc(old, size);
    if (p != nullptr) {
        if (old != nullptr && p != old) blocks.erase(old);
        blocks.insert(p);
    } else if (size == 0 && old != nullptr) {
        blocks.erase(old);
    }
    return p;
}

void Held::free(void* block) {
    if (block != nullptr) blocks.erase(block);
    std::free(block);
}

std::FILE* Held::fopen(const char* name, const char* mode) {
    std::FILE* f = std::fopen(name, mode);
    if (f != nullptr) files.insert(f);
    return f;
}

int Held::fclose(std::FILE* file) {
    if (file != nullptr) files.erase(file);
    return std::fclose(file);
}

void Held::release() {
    for (std::FILE* f : files) std::fclose(f);
    files.clear();
    for (void* p : blocks) std::free(p);
    blocks.clear();
}


/* the four parts of a run are linked to each other, to what the run holds and to the shared inline-assembler part */
static void link_run(RunSet* run) {
    front_run_link(*run->front, run->back, run->assembler, run->support, &run->held, run->asminlin);
    back_run_link(*run->back, run->front, run->assembler, run->support, &run->held, run->asminlin);
    asm_run_link(*run->assembler, run->front, run->back, run->support, &run->held, run->asminlin);
    support_run_link(*run->support, run->front, run->back, run->assembler, &run->held, run->asminlin);
}

/* a compile begins: the run the host kept from the last one (its state is new), or, the first time, a new one */
RunSet* cbrun_begin(RunSet* kept) {
    if (kept != nullptr) return kept;
    RunSet* run = new RunSet();
    run->front = front_run_new();
    run->back = back_run_new();
    run->assembler = asm_run_new();
    run->support = support_run_new();
    run->asminlin = asminlin_new();
    link_run(run);
    return run;
}

/* a compile ends: what the run still holds is given back and each part's state is made new again (in the storage it has: the next
 * compile starts with it, as a process of its own would with new variables) */
void cbrun_end(RunSet* run) {
    if (run == nullptr) return;
    run->held.release();
    front_run_reset(run->front);
    back_run_reset(run->back);
    asm_run_reset(run->assembler);
    support_run_reset(run->support);
    asminlin_reset(run->asminlin);
    link_run(run);
}

/* the host has no more compiles */
void cbrun_free(RunSet* run) {
    if (run == nullptr) return;
    run->held.release();
    front_run_delete(run->front);
    back_run_delete(run->back);
    asm_run_delete(run->assembler);
    support_run_delete(run->support);
    asminlin_delete(run->asminlin);
    delete run;
}
