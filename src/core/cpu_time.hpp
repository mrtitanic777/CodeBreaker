// core/cpu_time.hpp -- the CPU time this thread has used, in seconds: what a timing of a computation measures when other work shares the machine.
//
//     const double start = cb::thread_cpu_seconds();
//     ...
//     const double used = cb::thread_cpu_seconds() - start;
#pragma once

namespace cb {

double thread_cpu_seconds();

}  // namespace cb
