// cb/cb.hpp -- the CodeBreaker SDK for C++: the C interface (cb.h, binary.h, disasm.h, decompile.h) with RAII handles,
// std::string_view and std::span, and errors as exceptions. Header only, C++20; it adds nothing to a call but the check of its status.
//
//     cb::sdk::Session session;                                     // cb_init ... cb_shutdown
//     cb::sdk::Binary binary = cb::sdk::Binary::open("lz32.dll");
//     for (const cb_section& s : binary.sections()) std::cout << s.name << "\n";
//     std::cout << binary.function("LZCopy").decompile().view();
//     for (const cb_instruction& i : cb::sdk::disasm("z80", bytes, 0x0000)) ...
//
// A call that fails throws cb::sdk::Error, with the status and cb_last_error()'s message.
#ifndef CB_CB_HPP
#define CB_CB_HPP

#include <cstddef>
#include <cstdint>
#include <iterator>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

#include "cb/binary.h"
#include "cb/cb.h"
#include "cb/decompile.h"
#include "cb/disasm.h"

namespace cb::sdk {

// A failed call: its status, and what the library said.
class Error : public std::runtime_error {
public:
    Error(cb_status status, const char* message) : std::runtime_error(message), status_(status) {}
    cb_status status() const noexcept { return status_; }

private:
    cb_status status_;
};

// Throws Error for any status but CB_OK.
inline void check(cb_status status) {
    if (status != CB_OK) throw Error(status, cb_last_error());
}

// The library's version, and whether it is one this program's headers work with (same major, minor at least the headers').
inline std::uint32_t version() noexcept { return cb_version(); }
inline std::string_view version_text() noexcept { return cb_version_string(); }
inline bool compatible() noexcept {
    const std::uint32_t v = cb_version();
    return (v >> 16) == CB_SDK_VERSION_MAJOR && ((v >> 8) & 0xFF) >= CB_SDK_VERSION_MINOR;
}

// cb_init on construction, cb_shutdown on destruction.
class Session {
public:
    struct Options {
        std::string data_root;   // empty: the default (cb_init_options)
        std::string state_dir;
    };
    Session() { check(cb_init(nullptr)); }
    explicit Session(const Options& options) {
        cb_init_options o = CB_INIT_OPTIONS_INIT;
        o.data_root = options.data_root.empty() ? nullptr : options.data_root.c_str();
        o.state_dir = options.state_dir.empty() ? nullptr : options.state_dir.c_str();
        check(cb_init(&o));
    }
    ~Session() { cb_shutdown(); }
    Session(const Session&) = delete;
    Session& operator=(const Session&) = delete;
};

// Text the library made: owns a cb_string, frees it on destruction.
class String {
public:
    String() = default;
    ~String() { cb_string_free(&s_); }
    String(String&& other) noexcept : s_(std::exchange(other.s_, cb_string{nullptr, 0})) {}
    String& operator=(String&& other) noexcept {
        if (this != &other) {
            cb_string_free(&s_);
            s_ = std::exchange(other.s_, cb_string{nullptr, 0});
        }
        return *this;
    }
    String(const String&) = delete;
    String& operator=(const String&) = delete;

    std::string_view view() const noexcept { return s_.data ? std::string_view(s_.data, s_.size) : std::string_view(); }
    operator std::string_view() const noexcept { return view(); }
    std::string str() const { return std::string(view()); }
    const char* c_str() const noexcept { return s_.data ? s_.data : ""; }
    std::size_t size() const noexcept { return s_.size; }
    // The cb_string a C call fills (what it held is freed first).
    cb_string* out() noexcept {
        cb_string_free(&s_);
        return &s_;
    }

private:
    cb_string s_{nullptr, 0};
};

// An address as text: "0x10001000", "3:01a4" ...
inline std::string address_text(const cb_address& address) {
    char buffer[32];
    check(cb_address_format(&address, buffer, sizeof buffer, nullptr));
    return buffer;
}

inline cb_address linear(std::uint64_t address) noexcept { return cb_address{address, 0, CB_ADDRESS_LINEAR}; }

// One function of a binary (a view: the binary owns it).
class Function {
public:
    Function() = default;
    explicit Function(const cb_function* f) noexcept : f_(f) {}
    std::string_view name() const noexcept { return cb_function_name(f_); }
    cb_address address() const noexcept { return cb_function_address(f_); }
    const cb_function* get() const noexcept { return f_; }
    explicit operator bool() const noexcept { return f_ != nullptr; }
    // The function as C.
    String decompile(const cb_decompile_options* options = nullptr) const {
        String c;
        check(cb_function_decompile(f_, options, c.out()));
        return c;
    }

private:
    const cb_function* f_ = nullptr;
};

// An open binary: cb_binary_open on open(), cb_binary_close on destruction.
class Binary {
public:
    static Binary open(const std::string& path) {
        cb_binary* b = nullptr;
        check(cb_binary_open(path.c_str(), &b));
        return Binary(b);
    }
    Binary() = default;
    ~Binary() { cb_binary_close(b_); }
    Binary(Binary&& other) noexcept : b_(std::exchange(other.b_, nullptr)) {}
    Binary& operator=(Binary&& other) noexcept {
        if (this != &other) {
            cb_binary_close(b_);
            b_ = std::exchange(other.b_, nullptr);
        }
        return *this;
    }
    Binary(const Binary&) = delete;
    Binary& operator=(const Binary&) = delete;

    cb_binary* get() const noexcept { return b_; }
    std::string_view path() const noexcept { return cb_binary_path(b_); }
    std::string_view format() const noexcept { return cb_binary_format(b_); }
    std::string_view kind() const noexcept { return cb_binary_kind(b_); }
    std::string_view arch() const noexcept { return cb_binary_arch(b_); }
    std::uint64_t size() const noexcept { return cb_binary_size(b_); }
    std::uint64_t image_base() const noexcept { return cb_binary_image_base(b_); }
    cb_address entry() const noexcept { return cb_binary_entry(b_); }

    std::span<const cb_section> sections() const noexcept {
        std::size_t n = 0;
        const cb_section* p = cb_binary_sections(b_, &n);
        return {p, n};
    }
    std::span<const cb_import> imports() const noexcept {
        std::size_t n = 0;
        const cb_import* p = cb_binary_imports(b_, &n);
        return {p, n};
    }
    std::span<const cb_export> exports() const noexcept {
        std::size_t n = 0;
        const cb_export* p = cb_binary_exports(b_, &n);
        return {p, n};
    }

    // The functions the binary names, as a range of Function.
    class Functions {
    public:
        class iterator {
        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = Function;
            using difference_type = std::ptrdiff_t;
            iterator() = default;
            iterator(const cb_binary* b, std::size_t i) noexcept : b_(b), i_(i) {}
            Function operator*() const noexcept { return Function(cb_binary_function(b_, i_)); }
            iterator& operator++() noexcept {
                ++i_;
                return *this;
            }
            iterator operator++(int) noexcept {
                iterator was = *this;
                ++i_;
                return was;
            }
            bool operator==(const iterator& other) const noexcept { return i_ == other.i_; }

        private:
            const cb_binary* b_ = nullptr;
            std::size_t i_ = 0;
        };
        explicit Functions(const cb_binary* b) noexcept : b_(b) {}
        iterator begin() const noexcept { return {b_, 0}; }
        iterator end() const noexcept { return {b_, size()}; }
        std::size_t size() const noexcept { return cb_binary_function_count(b_); }
        Function operator[](std::size_t i) const noexcept { return Function(cb_binary_function(b_, i)); }

    private:
        const cb_binary* b_;
    };
    Functions functions() const noexcept { return Functions(b_); }

    // The function with this name; Error (CB_ERROR_NOT_FOUND) when there is none.
    Function function(const std::string& name) const {
        const cb_function* f = nullptr;
        check(cb_binary_function_find(b_, name.c_str(), &f));
        return Function(f);
    }
    // The function at an address (made when the binary names none there).
    Function function_at(const cb_address& address) {
        const cb_function* f = nullptr;
        check(cb_binary_function_at(b_, address, &f));
        return Function(f);
    }

    // Every function as one .c file.
    String decompile(cb_module_info* info = nullptr, const cb_decompile_options* options = nullptr) {
        String c;
        check(cb_module_decompile(b_, options, c.out(), info));
        return c;
    }

private:
    explicit Binary(cb_binary* b) noexcept : b_(b) {}
    cb_binary* b_ = nullptr;
};

// The instructions of one cb_disasm call: a range of cb_instruction, freed on destruction.
class Instructions {
public:
    Instructions() = default;
    explicit Instructions(cb_instructions* p) noexcept : p_(p) {}
    ~Instructions() { cb_instructions_free(p_); }
    Instructions(Instructions&& other) noexcept : p_(std::exchange(other.p_, nullptr)) {}
    Instructions& operator=(Instructions&& other) noexcept {
        if (this != &other) {
            cb_instructions_free(p_);
            p_ = std::exchange(other.p_, nullptr);
        }
        return *this;
    }
    Instructions(const Instructions&) = delete;
    Instructions& operator=(const Instructions&) = delete;

    std::span<const cb_instruction> view() const noexcept {
        std::size_t n = 0;
        const cb_instruction* p = cb_instructions_data(p_, &n);
        return {p, n};
    }
    const cb_instruction* begin() const noexcept { return view().data(); }
    const cb_instruction* end() const noexcept { return view().data() + view().size(); }
    std::size_t size() const noexcept { return view().size(); }
    const cb_instruction& operator[](std::size_t i) const noexcept { return view()[i]; }

private:
    cb_instructions* p_ = nullptr;
};

// `count` instructions (0: all) of `code` for processor `arch`, the first at `address`.
inline Instructions disasm(const std::string& arch, std::span<const std::uint8_t> code, std::uint64_t address, std::size_t count = 0) {
    cb_instructions* p = nullptr;
    check(cb_disasm(arch.c_str(), code.data(), code.size(), address, count, &p));
    return Instructions(p);
}

// Every processor CodeBreaker knows, and one by name.
inline std::span<const cb_arch> archs() noexcept {
    std::size_t n = 0;
    const cb_arch* p = cb_arch_list(&n);
    return {p, n};
}
inline const cb_arch& arch(const std::string& name) {
    const cb_arch* a = nullptr;
    check(cb_arch_find(name.c_str(), &a));
    return *a;
}

// Whether the decompiler can run; Error says what is missing.
inline void check_decompiler() { check(cb_decompiler_check()); }

}  // namespace cb::sdk

#endif
