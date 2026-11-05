// pyset.h -- a set of small non-negative integers that iterates in the order CPython's set does.
//
// The analyses were written in Python, and some of what they report follows the order a set of
// addresses happens to iterate in (Tarjan's walk over a function's callees, the far addresses one
// 16-bit instruction holds). This is CPython's own table (Objects/setobject.c: open addressing,
// LINEAR_PROBES 9, PERTURB_SHIFT 5, resized at 3/5 full to used*4, or used*2 past 50,000), with
// an int's hash being the int itself, so a set built by the same adds iterates the same.
// Adds only: nothing here is ever removed from such a set.
#pragma once
#include <cstdint>
#include <vector>

namespace cba {

class PySet {
public:
    PySet() : table_(8, EMPTY), mask_(7) {}

    bool contains(int64_t v) const {
        size_t i = (size_t)v & mask_, perturb = (size_t)v;
        while (true) {
            size_t probes = (i + LINEAR <= mask_) ? LINEAR : 0;
            for (size_t j = 0; j <= probes; j++) {
                int64_t e = table_[i + j];
                if (e == EMPTY) return false;
                if (e == v) return true;
            }
            perturb >>= SHIFT;
            i = (i * 5 + 1 + perturb) & mask_;
        }
    }

    void add(int64_t v) {
        size_t i = (size_t)v & mask_, perturb = (size_t)v;
        while (true) {
            size_t probes = (i + LINEAR <= mask_) ? LINEAR : 0;
            for (size_t j = 0; j <= probes; j++) {
                int64_t &e = table_[i + j];
                if (e == EMPTY) {
                    e = v;
                    used_++;
                    if (used_ * 5 >= mask_ * 3) resize(used_ > 50000 ? used_ * 2 : used_ * 4);
                    return;
                }
                if (e == v) return;
            }
            perturb >>= SHIFT;
            i = (i * 5 + 1 + perturb) & mask_;
        }
    }

    size_t size() const { return used_; }
    bool empty() const { return used_ == 0; }

    // the members in the order Python iterates them
    std::vector<int64_t> items() const {
        std::vector<int64_t> out;
        out.reserve(used_);
        for (int64_t e : table_)
            if (e != EMPTY) out.push_back(e);
        return out;
    }

private:
    static constexpr int64_t EMPTY = -1;
    static constexpr size_t LINEAR = 9, SHIFT = 5;
    std::vector<int64_t> table_;
    size_t mask_, used_ = 0;

    void resize(size_t minused) {
        size_t size = 8;
        while (size <= minused) size <<= 1;
        std::vector<int64_t> old;
        old.swap(table_);
        table_.assign(size, EMPTY);
        mask_ = size - 1;
        for (int64_t v : old) {
            if (v == EMPTY) continue;
            size_t i = (size_t)v & mask_, perturb = (size_t)v;
            while (true) {
                if (table_[i] == EMPTY) {
                    table_[i] = v;
                    break;
                }
                bool put = false;
                if (i + LINEAR <= mask_) {
                    for (size_t j = 1; j <= LINEAR; j++)
                        if (table_[i + j] == EMPTY) {
                            table_[i + j] = v;
                            put = true;
                            break;
                        }
                }
                if (put) break;
                perturb >>= SHIFT;
                i = (i * 5 + 1 + perturb) & mask_;
            }
        }
    }
};

}  // namespace cba
