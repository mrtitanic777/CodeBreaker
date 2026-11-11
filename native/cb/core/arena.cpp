#include "cb/core/arena.hpp"

#include <algorithm>
#include <cstring>

#include "cb/core/error.hpp"

namespace cb {

void* Arena::allocate(std::size_t size, std::size_t align) {
    CB_ASSERT(align != 0 && (align & (align - 1)) == 0, "alignment must be a power of two");
    if (size == 0) size = 1;
    while (true) {
        if (current_ == blocks_.size()) {
            const std::size_t want = std::max(block_size_, size + align);
            blocks_.push_back({std::make_unique<std::byte[]>(want), want, 0});
        }
        Block& b = blocks_[current_];
        const auto base = reinterpret_cast<std::uintptr_t>(b.data.get());
        const std::uintptr_t start = (base + b.used + align - 1) & ~(static_cast<std::uintptr_t>(align) - 1);
        const std::size_t offset = static_cast<std::size_t>(start - base);
        if (offset + size <= b.size) {
            b.used = offset + size;
            used_ += size;
            void* p = b.data.get() + offset;
            std::memset(p, 0, size);
            return p;
        }
        ++current_;   // this block cannot hold it: try the next (or add one)
    }
}

std::string_view Arena::intern(std::string_view s) {
    char* p = static_cast<char*>(allocate(s.size() + 1, 1));
    std::memcpy(p, s.data(), s.size());
    p[s.size()] = '\0';
    return {p, s.size()};
}

void Arena::reset() {
    for (auto it = finalizers_.rbegin(); it != finalizers_.rend(); ++it) it->destroy(it->object);
    finalizers_.clear();
    for (Block& b : blocks_) b.used = 0;
    current_ = 0;
    objects_ = 0;
    used_ = 0;
}

}  // namespace cb
