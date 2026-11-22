/* cbrun.hpp -- the state of one run of the compiler (wcc or wcc386), made new at the start of each run and gone at its end.
 *
 * OpenWatcom's compilers kept it in globals and statics: a process that compiled twice carried the first compile's flags,
 * lists and counters into the second. Here each file's statics, and each header's globals, are a struct of their own
 * (`ScanState`, `CvarsState` ...) reached through one accessor (`scan_state()`, `cvars_state()` ...), which is
 * `cbrun_part<T>()`: the run's one T. x86cc.cpp calls CBRunBegin() before a compile, which makes every part (zero, or its
 * members' initial values, as a variable of a fresh process was), and CBRunEnd() after it, which destroys them newest first
 * and gives back the memory and files the run still holds (cbrun.cpp).
 *
 * A part lives at a fixed address (its slot's storage), so the compiler's own tables may keep the address of a member for the
 * run, as they kept the address of a global, and reaching a part costs what reaching a global did. One compile runs at a
 * time (x86cc.cpp holds a lock). Each target's symbols carry its own prefix (cbnames.h), so the two compilers in one program
 * keep their runs apart.
 */
#ifndef CB_RUN_HPP
#define CB_RUN_HPP

#include <cstddef>
#include <new>
#include <vector>

/* how a part is made and destroyed, in the order the parts announced themselves (at the start of the program) */
struct cbrun_part_ops {
    void (*make)();
    void (*unmake)();
};
struct cbrun_parts {
    std::vector<cbrun_part_ops> all;
};
cbrun_parts& cbrun_registry();

template <class T>
struct cbrun_slot {
    alignas(T) static inline unsigned char storage[sizeof(T)];   /* where the part lives while a run lasts */
    static void make() { ::new (static_cast<void*>(storage)) T{}; }
    static void unmake() { reinterpret_cast<T*>(storage)->~T(); }
    static bool announce() {
        cbrun_registry().all.push_back({&make, &unmake});
        return true;
    }
    static inline const bool announced = announce();   /* (a part is announced once, before main) */
};

/* the run's T: valid between CBRunBegin and CBRunEnd */
template <class T>
inline T& cbrun_part() {
    (void)cbrun_slot<T>::announced;
    return *reinterpret_cast<T*>(cbrun_slot<T>::storage);
}

#endif
