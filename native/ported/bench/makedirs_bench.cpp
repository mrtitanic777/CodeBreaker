// makedirs / OSError text microbenchmark (CPython reference in the comment at the end of this file)
#include <chrono>
#include <cstdio>
#include "py.hpp"
static double ms(std::chrono::steady_clock::time_point t) {
    return std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - t).count();
}
int main(int argc, char** argv) {
    const py::str root(argv[1]);
    py::os::makedirs(root, 0777, true);
    double best = 1e9;
    for (int round = 0; round < 5; round++) {
        auto t = std::chrono::steady_clock::now();
        for (int i = 0; i < 100; i++)
            py::os::makedirs(py::os::path::join(root, py::str("r") + py::to_str(round), py::str("a"), py::str("d") + py::to_str(i)), 0777, false);
        best = std::min(best, ms(t) / 100);
    }
    std::printf("cpp makedirs (3 new levels deep, 1 leaf folder each) best ms per call %.3f\n", best);
    auto t = std::chrono::steady_clock::now();
    for (int i = 0; i < 1000; i++) py::os::makedirs(py::os::path::join(root, py::str("r0"), py::str("a"), py::str("d1")), 0777, true);
    std::printf("cpp makedirs exist_ok on an existing folder ms per call %.4f\n", ms(t) / 1000);
    try {
        py::os::listdir(py::os::path::join(root, py::str("nope")));
    } catch (const py::Exception& e) {
        std::printf("%s: %s\n", e.type(), e.what());
    }
    try {
        py::os::makedirs(py::os::path::join(root, py::str("r0")), 0777, false);
    } catch (const py::Exception& e) {
        std::printf("%s: %s\n", e.type(), e.what());
    }
}
// CPython:
//   t = time.perf_counter()
//   for i in range(100): os.makedirs(os.path.join(root, 'r%d' % rnd, 'a', 'd%d' % i))
//   -> ms per call;  os.listdir(root + '/nope') -> FileNotFoundError: [WinError 3] The system cannot find the path specified: '...'
