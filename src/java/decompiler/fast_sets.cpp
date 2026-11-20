// SFormsFastMapDirect (de.fernflower.util.SFormsFastMapDirect); the set classes are templates, defined in fast_sets.hpp.
// cb-unity: alone (file-local helpers share names with the other decompiler sources)
#include "java/decompiler/fast_sets.hpp"

namespace cb::java::decompiler {

namespace {
inline std::size_t sz(int i) { return static_cast<std::size_t>(i); }
}

SFormsFastMapDirect* SFormsFastMapDirect::get_copy() const {
    SFormsFastMapDirect* map = make<SFormsFastMapDirect>();
    map->size_ = size_;
    for (int i = 2; i >= 0; i--) {
        const ArenaVector<SparseSet*>& arr = elements_[i];
        const std::size_t length = arr.size();
        if (length > 0) {
            const ArenaVector<int>& arrnext = next_[i];
            map->elements_[i].assign(length, nullptr);
            map->next_[i] = arrnext;
            int pointer = 0;
            do {
                SparseSet* set = arr[sz(pointer)];
                if (set != nullptr) map->elements_[i][sz(pointer)] = set->get_copy();
                pointer = arrnext[sz(pointer)];
            } while (pointer != 0);
        } else {
            map->elements_[i].clear();
            map->next_[i].clear();
        }
    }
    return map;
}

void SFormsFastMapDirect::remove_all_fields() {
    ArenaVector<SparseSet*>& arr = elements_[2];
    ArenaVector<int>& arrnext = next_[2];
    for (int i = static_cast<int>(arr.size()) - 1; i >= 0; i--) {
        SparseSet* val = arr[sz(i)];
        if (val != nullptr) {
            arr[sz(i)] = nullptr;
            size_--;
        }
        arrnext[sz(i)] = 0;
    }
}

void SFormsFastMapDirect::put_internal(int key, SparseSet* value, bool remove) {
    int index = 0;
    int ikey = key;
    if (ikey < 0) {
        index = 2;
        ikey = -ikey;
    } else if (ikey >= VarExprent::STACK_BASE) {
        index = 1;
        ikey -= VarExprent::STACK_BASE;
    }
    ArenaVector<SparseSet*>* arr = &elements_[index];
    if (ikey >= static_cast<int>(arr->size())) {
        if (remove) return;
        arr = &ensure_capacity(index, ikey + 1, false);
    }
    SparseSet* oldval = (*arr)[sz(ikey)];
    (*arr)[sz(ikey)] = value;
    ArenaVector<int>& arrnext = next_[index];
    if (oldval == nullptr && value != nullptr) {
        size_++;
        change_next(arrnext, ikey, arrnext[sz(ikey)], ikey);
    } else if (oldval != nullptr && value == nullptr) {
        size_--;
        change_next(arrnext, ikey, ikey, arrnext[sz(ikey)]);
    }
}

void SFormsFastMapDirect::change_next(ArenaVector<int>& arrnext, int key, int oldnext, int newnext) {
    for (int i = key - 1; i >= 0; i--) {
        if (arrnext[sz(i)] == oldnext) arrnext[sz(i)] = newnext;
        else break;
    }
}

SFormsFastMapDirect::SparseSet* SFormsFastMapDirect::get(int key) const {
    int index = 0;
    if (key < 0) {
        index = 2;
        key = -key;
    } else if (key >= VarExprent::STACK_BASE) {
        index = 1;
        key -= VarExprent::STACK_BASE;
    }
    const ArenaVector<SparseSet*>& arr = elements_[index];
    if (key < static_cast<int>(arr.size())) return arr[sz(key)];
    return nullptr;
}

void SFormsFastMapDirect::complement(const SFormsFastMapDirect* map) {
    for (int i = 2; i >= 0; i--) {
        ArenaVector<SparseSet*>& lst_own = elements_[i];
        if (lst_own.empty()) continue;
        if (map == nullptr) throw std::runtime_error("SFormsFastMapDirect: null map");   // the original's NullPointerException, only here
        const ArenaVector<SparseSet*>& lst_extern = map->elements_[i];
        ArenaVector<int>& arrnext = next_[i];
        int pointer = 0;
        do {
            SparseSet* first = lst_own[sz(pointer)];
            if (first != nullptr) {
                if (pointer >= static_cast<int>(lst_extern.size())) break;
                SparseSet* second = lst_extern[sz(pointer)];
                if (second != nullptr) {
                    first->complement(second);
                    if (first->is_empty()) {
                        lst_own[sz(pointer)] = nullptr;
                        size_--;
                        change_next(arrnext, pointer, pointer, arrnext[sz(pointer)]);
                    }
                }
            }
            pointer = arrnext[sz(pointer)];
        } while (pointer != 0);
    }
}

void SFormsFastMapDirect::intersection(const SFormsFastMapDirect* map) {
    for (int i = 2; i >= 0; i--) {
        ArenaVector<SparseSet*>& lst_own = elements_[i];
        if (lst_own.empty()) continue;
        if (map == nullptr) throw std::runtime_error("SFormsFastMapDirect: null map");   // the original's NullPointerException, only here
        const ArenaVector<SparseSet*>& lst_extern = map->elements_[i];
        ArenaVector<int>& arrnext = next_[i];
        int pointer = 0;
        do {
            SparseSet* first = lst_own[sz(pointer)];
            if (first != nullptr) {
                SparseSet* second = nullptr;
                if (pointer < static_cast<int>(lst_extern.size())) second = lst_extern[sz(pointer)];
                if (second != nullptr) first->intersection(second);
                if (second == nullptr || first->is_empty()) {
                    lst_own[sz(pointer)] = nullptr;
                    size_--;
                    change_next(arrnext, pointer, pointer, arrnext[sz(pointer)]);
                }
            }
            pointer = arrnext[sz(pointer)];
        } while (pointer != 0);
    }
}

void SFormsFastMapDirect::union_(const SFormsFastMapDirect* map) {
    if (map == nullptr) throw std::runtime_error("SFormsFastMapDirect::union: null map");
    for (int i = 2; i >= 0; i--) {
        const ArenaVector<SparseSet*>& lst_extern = map->elements_[i];
        if (lst_extern.empty()) continue;
        ArenaVector<SparseSet*>& lst_own = elements_[i];
        ArenaVector<int>& arrnext = next_[i];
        const ArenaVector<int>& arrnext_extern = map->next_[i];
        int pointer = 0;
        do {
            if (pointer >= static_cast<int>(lst_own.size())) ensure_capacity(i, static_cast<int>(lst_extern.size()), true);
            SparseSet* second = lst_extern[sz(pointer)];
            if (second != nullptr) {
                SparseSet* first = lst_own[sz(pointer)];
                if (first == nullptr) {
                    lst_own[sz(pointer)] = second->get_copy();
                    size_++;
                    change_next(arrnext, pointer, arrnext[sz(pointer)], pointer);
                } else {
                    first->union_(second);
                }
            }
            pointer = arrnext_extern[sz(pointer)];
        } while (pointer != 0);
    }
}

std::string SFormsFastMapDirect::to_string() const {
    std::string buffer = "{";
    const std::vector<Entry> lst = entry_list();
    bool first = true;
    for (const Entry& entry : lst) {
        if (!first) buffer += ", ";
        else first = false;
        buffer += std::to_string(entry.first) + "={" + entry.second->to_string() + "}";
    }
    buffer += "}";
    return buffer;
}

std::vector<SFormsFastMapDirect::Entry> SFormsFastMapDirect::entry_list() const {
    std::vector<Entry> list;
    for (int i = 2; i >= 0; i--) {
        int ikey = 0;
        for (SparseSet* ent : elements_[i]) {
            if (ent != nullptr) {
                const int key = i == 0 ? ikey : (i == 1 ? ikey + VarExprent::STACK_BASE : -ikey);
                list.emplace_back(key, ent);
            }
            ikey++;
        }
    }
    return list;
}

ArenaVector<SFormsFastMapDirect::SparseSet*>& SFormsFastMapDirect::ensure_capacity(int index, int size, bool exact) {
    ArenaVector<SparseSet*>& arr = elements_[index];
    int minsize = size;
    if (!exact) {
        minsize = 2 * static_cast<int>(arr.size()) / 3 + 1;
        if (size > minsize) minsize = size;
    }
    arr.resize(sz(minsize), nullptr);
    next_[index].resize(sz(minsize), 0);
    return arr;
}

}  // namespace cb::java::decompiler
