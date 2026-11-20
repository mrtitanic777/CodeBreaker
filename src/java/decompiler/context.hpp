// java/decompiler/context.hpp -- the decompiler's per-run state, reachable from every pass as the original's thread-local DecompilerContext is: the
// options (by their short keys, "rer", "din" ..., with the defaults the original has), the logger, and the run's struct context.
//
//     DecompilerScope run(DecompilerContext{.properties = options});   // current for this thread until `run` ends
//     DecompilerContext::option(pref::REMOVE_EMPTY_RANGES);                     // true: "1"
//
// Ported from Fernflower (de.fernflower.main.DecompilerContext, main.extern.IFernflowerPreferences), Copyright 2000-2015 JetBrains s.r.o., Apache
// License 2.0 (see NOTICE in this folder). The rest of the original's context (the class processor, the import and variable-name collectors, the
// counters, the bytecode mapper) joins as the layers that own it are ported.
#pragma once

#include <deque>
#include <functional>
#include <memory>
#include <memory_resource>
#include <optional>
#include <stdexcept>
#include <vector>

#include "java/decompiler/var_type.hpp"
#include <map>
#include <string>
#include <string_view>

namespace cb::java::decompiler {

class StructContext;
class StructClass;
class StructMethod;

namespace pref {
inline constexpr std::string_view REMOVE_BRIDGE = "rbr", REMOVE_SYNTHETIC = "rsy", DECOMPILE_INNER = "din", DECOMPILE_CLASS_1_4 = "dc4",
                                  DECOMPILE_ASSERTIONS = "das", HIDE_EMPTY_SUPER = "hes", HIDE_DEFAULT_CONSTRUCTOR = "hdc", DECOMPILE_GENERIC_SIGNATURES = "dgs",
                                  NO_EXCEPTIONS_RETURN = "ner", DECOMPILE_ENUM = "den", REMOVE_GET_CLASS_NEW = "rgn", LITERALS_AS_IS = "lit",
                                  BOOLEAN_TRUE_ONE = "bto", ASCII_STRING_CHARACTERS = "asc", SYNTHETIC_NOT_SET = "nns", UNDEFINED_PARAM_TYPE_OBJECT = "uto",
                                  USE_DEBUG_VAR_NAMES = "udv", REMOVE_EMPTY_RANGES = "rer", FINALLY_DEINLINE = "fdi", IDEA_NOT_NULL_ANNOTATION = "inn",
                                  LAMBDA_TO_ANONYMOUS_CLASS = "lac", BYTECODE_SOURCE_MAPPING = "bsm", USE_DEBUG_LINE_NUMBERS = "udl",
                                  NO_COMMENT_OUTPUT = "nco", OVERRIDE_ANNOTATION = "ovr", LOG_LEVEL = "log", MAX_PROCESSING_METHOD = "mpm",
                                  RENAME_ENTITIES = "ren", USER_RENAMER_CLASS = "urc", NEW_LINE_SEPARATOR = "nls", INDENT_STRING = "ind", BANNER = "ban",
                                  DUMP_ORIGINAL_LINES = "__dump_original_lines__", UNIT_TEST_MODE = "__unit_test_mode__";
// The defaults (the new-line separator: "0" on Windows, "1" elsewhere, as the original decides).
std::map<std::string, std::string> defaults();
}  // namespace pref

// An object the run's arena owns (expressions, statements, edges, variable nodes): it lives while its class is written, as a Java object lives while
// referenced (a class written, the collector frees what was made for it). A type whose objects outlive the class they were made in (a class node: a
// lambda's joins the class tree) says so with `static constexpr bool kept_across_classes = true;`.
class ArenaObject {
public:
    virtual ~ArenaObject() = default;
    static constexpr bool kept_across_classes = false;
};

// The run's objects and the memory they (and their arrays, through ArenaAllocator) are made in: bump regions, one for the run and one for each class
// being written, each given back whole (Arena::ClassRegion). A kept object is made on the heap, as before.
class Arena {
public:
    Arena() { regions_.push_back(std::make_unique<std::pmr::monotonic_buffer_resource>(kFirstBlock)); }
    ~Arena() { destroy_since(0, false); }
    Arena(const Arena&) = delete;
    Arena& operator=(const Arena&) = delete;

    void* allocate(std::size_t bytes, std::size_t align) { return regions_.back()->allocate(bytes, align); }

    template <class T, class... A>
    T* make(A&&... args) {
        if constexpr (T::kept_across_classes) {
            auto p = std::make_unique<T>(std::forward<A>(args)...);
            objects_.push_back({p.get(), true});
            return p.release();
        } else {
            T* raw = new (allocate(sizeof(T), alignof(T))) T(std::forward<A>(args)...);
            objects_.push_back({raw, false});
            return raw;
        }
    }

    // A class being written: what is made while it lives is freed when it ends, but the kept objects.
    class ClassRegion {
    public:
        explicit ClassRegion(Arena& arena) : arena_(arena), mark_(arena.objects_.size()) {
            arena.regions_.push_back(std::make_unique<std::pmr::monotonic_buffer_resource>(kFirstBlock));
        }
        ~ClassRegion() {
            arena_.destroy_since(mark_, true);
            arena_.regions_.pop_back();
        }
        ClassRegion(const ClassRegion&) = delete;
        ClassRegion& operator=(const ClassRegion&) = delete;

    private:
        Arena& arena_;
        std::size_t mark_;
    };

private:
    static constexpr std::size_t kFirstBlock = 1 << 16;
    struct Held {
        ArenaObject* object;
        bool on_heap;
    };
    // The objects from `mark` on, last first; the kept ones stay when `keep_heap`.
    void destroy_since(std::size_t mark, bool keep_heap) {
        for (std::size_t i = objects_.size(); i-- > mark;) {
            const Held h = objects_[i];
            if (!h.on_heap) h.object->~ArenaObject();
            else if (!keep_heap) delete h.object;
        }
        std::size_t kept = mark;
        for (std::size_t i = mark; i < objects_.size(); ++i) {
            if (keep_heap && objects_[i].on_heap) objects_[kept++] = objects_[i];
        }
        objects_.resize(kept);
    }

    std::deque<Held> objects_;   // (blocks: no copying as it grows)
    std::vector<std::unique_ptr<std::pmr::monotonic_buffer_resource>> regions_;
};

// The run's counters: statement ids, expression ids, variable indexes (each starts at 1).
struct CounterContainer {
    static constexpr int STATEMENT_COUNTER = 0, EXPRENT_COUNTER = 1, VAR_COUNTER = 2;
    int values[3] = {1, 1, 1};
    void set_counter(int counter, int value) { values[counter] = value; }
    int get_counter(int counter) const { return values[counter]; }
    int get_counter_and_increment(int counter) { return values[counter]++; }
};

// The original's StackOverflowError, where the original recurses without end (a lambda that is its own parent): a recursion that must stand for
// Java's holds a StackDepth; past `limit` levels it throws, and the original's catch of Throwable takes it as it takes the overflow.
struct JavaStackOverflow : std::runtime_error {
    JavaStackOverflow() : std::runtime_error("java.lang.StackOverflowError") {}
};
class StackDepth {
public:
    explicit StackDepth(int limit = 4096) {
        if (++depth_ > limit) {
            --depth_;
            throw JavaStackOverflow();
        }
    }
    ~StackDepth() { --depth_; }
    StackDepth(const StackDepth&) = delete;
    StackDepth& operator=(const StackDepth&) = delete;

private:
    static inline thread_local int depth_ = 0;
};

class ClassesProcessor;
class ImportCollector;
class VarNamesCollector;
class BytecodeSourceMapper;
struct ClassNode;
class MethodWrapper;
class VarProcessor;
class PoolInterceptor;       // constant_pool.hpp (the renamer's is RenamerPoolInterceptor, renamer.hpp)
class IFernflowerLogger;     // decompiler.hpp

struct DecompilerContext {
    std::map<std::string, std::string> properties = pref::defaults();
    std::function<void(const std::string& message)> logger;
    StructContext* struct_context = nullptr;
    std::shared_ptr<CounterContainer> counters = std::make_shared<CounterContainer>();
    std::shared_ptr<Arena> arena = std::make_shared<Arena>();
    ClassesProcessor* class_processor = nullptr;
    ImportCollector* import_collector = nullptr;
    VarNamesCollector* var_names_collector = nullptr;
    BytecodeSourceMapper* bytecode_source_mapper = nullptr;
    // The renamer's names (null unless RENAME_ENTITIES), and the run's logger with its severity and indent (the original's getLogger()).
    std::shared_ptr<const PoolInterceptor> pool_interceptor;
    IFernflowerLogger* fernflower_logger = nullptr;
    // The original's CURRENT_* properties: what is being written now.
    const StructClass* current_class = nullptr;
    ClassNode* current_class_node = nullptr;
    const StructMethod* current_method = nullptr;
    std::optional<MethodDescriptor> current_method_descriptor;
    MethodWrapper* current_method_wrapper = nullptr;
    VarProcessor* current_var_processor = nullptr;

    static CounterContainer& counter_container() { return *current().counters; }

    // The context current on this thread; std::logic_error when there is none.
    static DecompilerContext& current();
    // Makes `context` (or none) the one current on THIS thread without owning it: the original's setCurrentContext, which a method's worker thread
    // (MethodProcessorRunnable) uses to share its parent's context. (Defined in context.cpp.)
    static void set_current(DecompilerContext* context);
    // The run's logger (the original's getLogger()); std::logic_error when none. (Defined in context.cpp.)
    static IFernflowerLogger& get_logger();
    // The option is "1".
    static bool option(std::string_view key);
    static std::string property(std::string_view key);
    static std::string new_line_separator() { return option(pref::NEW_LINE_SEPARATOR) ? "\n" : "\r\n"; }
    static void log(const std::string& message);
};

// A new object of the current run's arena (the original's `new` of a Statement, an Exprent ...).
template <class T, class... A>
T* make(A&&... args) {
    return DecompilerContext::current().arena->make<T>(std::forward<A>(args)...);
}

// The allocator of the arrays of arena objects (the SSA sets' blocks): memory of the current region, given back with it (deallocate does nothing).
template <class T>
struct ArenaAllocator {
    using value_type = T;
    using is_always_equal = std::true_type;
    ArenaAllocator() noexcept = default;
    template <class U>
    ArenaAllocator(const ArenaAllocator<U>&) noexcept {}
    T* allocate(std::size_t n) { return static_cast<T*>(DecompilerContext::current().arena->allocate(n * sizeof(T), alignof(T))); }
    void deallocate(T*, std::size_t) noexcept {}
    template <class U>
    bool operator==(const ArenaAllocator<U>&) const noexcept { return true; }
};
template <class T>
using ArenaVector = std::vector<T, ArenaAllocator<T>>;

// Makes a context current on this thread for its lifetime (the one before it comes back after).
class DecompilerScope {
public:
    explicit DecompilerScope(DecompilerContext context);
    ~DecompilerScope();
    DecompilerScope(const DecompilerScope&) = delete;
    DecompilerScope& operator=(const DecompilerScope&) = delete;
    DecompilerContext& context() { return context_; }

private:
    DecompilerContext context_;
    DecompilerContext* previous_;
};

}  // namespace cb::java::decompiler
