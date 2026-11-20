#include "invert/module_pass.hpp"

#include <algorithm>
#include <format>

#include "formats/ne.hpp"
#include "invert/grammars.hpp"
#include "invert/msc6_segment.hpp"

namespace cb::invert::module_pass {

namespace {

std::uint32_t u16(ByteView code, std::size_t i) { return code[i] | (static_cast<std::uint32_t>(code[i + 1]) << 8); }

bool contains(ByteView code, std::initializer_list<std::uint8_t> pattern) {
    return std::search(code.begin(), code.end(), pattern.begin(), pattern.end()) != code.end();
}

bool starts_with(ByteView code, std::initializer_list<std::uint8_t> pattern) {
    return code.size() >= pattern.size() && std::equal(pattern.begin(), pattern.end(), code.begin());
}

// The bytes equal outside the places the linker fills in (the module's relocations, and the operands `msc6_small::mask` zeroes), and as long.
bool same_code(ByteView built_whole, ByteView original, const std::set<std::size_t>& fixups) {
    // (a compiler's own padding after the function -- int 3, nop, zero after a near return or more padding -- is not part of what is compared)
    std::size_t n = built_whole.size();
    while (n > 1 && (built_whole[n - 1] == 0xCC || built_whole[n - 1] == 0x90 || built_whole[n - 1] == 0x00) &&
           (built_whole[n - 2] == 0xC3 || built_whole[n - 2] == 0xCC || built_whole[n - 2] == 0x90 || built_whole[n - 2] == 0x00))
        --n;
    const ByteView built = built_whole.first(n);
    if (built.size() != original.size()) return false;
    const Bytes a = msc6_small::mask(built), b = msc6_small::mask(original);
    for (std::size_t i = 0; i < original.size(); ++i)
        if (!fixups.contains(i) && a[i] != b[i]) return false;
    return true;
}

}  // namespace

Compiler compiler_of(ByteView code) {
    const bool lea_teardown = contains(code, {0x8d, 0x66, 0xfe, 0x1f, 0x5d});
    if (starts_with(code, {0x8c, 0xd8}) || starts_with(code, {0x55, 0x8b, 0xec, 0x1e, 0x8e, 0xd8}) || lea_teardown) return Compiler::Msvc152;
    return Compiler::Msc6;
}

std::string_view to_text(Verdict v) {
    switch (v) {
        case Verdict::Identical: return "identical";
        case Verdict::Recognised: return "recognised";
        case Verdict::Declined: return "declined";
        case Verdict::Assembly: return "assembly";
    }
    return "declined";
}

msc6_small::Globals globals_of(ByteView code) {
    msc6_small::Globals out;
    const std::size_t n = code.size();
    for (std::size_t i = 0; i + 6 < n; ++i) {
        if (code[i] == 0xA1 && code[i + 3] == 0x0B && code[i + 4] == 0x06 && u16(code, i + 1) == u16(code, i + 5) + 2) {
            const std::uint32_t at = u16(code, i + 5);
            out[at] = {std::format("gL_{:x}", at), msc6_small::Width::Long};
        }
        if (i + 8 <= n && code[i] == 0x2b && code[i + 1] == 0xc0 && code[i + 2] == 0xA3 && code[i + 5] == 0xA3) {
            const std::uint32_t at = u16(code, i + 6);
            out.try_emplace(at, msc6_small::Global{std::format("gL_{:x}", at), msc6_small::Width::Long});
        }
    }
    for (std::size_t i = 0; i + 3 < n; ++i) {
        if ((code[i] == 0xc7 && code[i + 1] == 0x06) || (code[i] == 0xff && code[i + 1] == 0x36)) {
            const std::uint32_t at = u16(code, i + 2);
            if (!out.contains(at) && !(at >= 2 && out.contains(at - 2))) out[at] = {std::format("gW_{:x}", at), msc6_small::Width::Word};
        }
    }
    return out;
}

Reading read_function(const Function& f) {
    Reading out;
    if (const auto thunk = msc6_segment::thunk(f.code)) {
        out.verdict = Verdict::Assembly;
        out.text = msc6_segment::thunk_assembly(*thunk, f.name);
        return out;
    }
    std::vector<std::string>& tries = out.tries;
    try {
        for (const msc6_segment::Shape& s : msc6_segment::shapes(f.code)) tries.push_back(s.text);   // a function that is one known shape
    } catch (const Error&) {
    }
    std::string returns = "int";
    std::optional<std::vector<msc6::Param>> params = f.called;
    if (f.sdk) {
        returns = f.sdk->returns;
        params = f.sdk->params;
    }
    std::string why;
    msc6::Context context;
    context.globals = globals_of(f.code);
    context.return_type = returns;
    context.function_base = f.offset;
    const bool known = params.has_value() && !params->empty();
    for (const bool use_params : known ? std::vector<bool>{true, false} : std::vector<bool>{false}) {   // the parameters known, then the model's own reading
        context.params = use_params ? params : std::nullopt;
        const Result<msc6::Reading> r = msc6::invert(f.code, context);
        if (!r.ok()) {
            why = r.error().what();
            continue;
        }
        if (std::ranges::find(tries, r.value().text) == tries.end()) tries.push_back(r.value().text);
    }
    if (tries.empty()) {
        // shapes the model does not read, by search
        if (contains(f.code, {0x9a})) {
            grammars::Shape shape;
            shape.nargs = params ? params->size() : 0;
            for (const grammars::Grammar& g : grammars::all())
                for (std::string& text : g.generate(shape)) tries.push_back(std::move(text));
        }
        out.verdict = Verdict::Declined;
        out.text = why;
        return out;
    }
    out.verdict = Verdict::Recognised;
    out.text = tries.back();
    return out;
}

std::vector<Function> framed_functions(const std::filesystem::path& module) {
    const formats::ne::Module ne = formats::ne::Module::open_file(module);
    std::vector<Function> out;
    for (const formats::ne::Segment& s : ne.segments()) {
        if (s.length == 0 || s.is_data() || s.file_offset == 0) continue;
        const ByteView segment = ne.data().subspan(s.file_offset, std::min<std::size_t>(s.length, ne.data().size() - s.file_offset));
        std::map<std::uint32_t, std::vector<msc6::Param>> called;
        try {
            for (msc6_segment::Signature& sig : msc6_segment::infer_signatures(segment)) called[sig.offset] = std::move(sig.params);
        } catch (const Error&) {   // a segment whose calls cannot be followed: its functions are read without them
        }
        const std::set<std::size_t> places = relocated_places(ne, s, segment);
        for (const auto& [start, end] : msc6_segment::framed_functions(segment)) {
            Function f;
            f.name = std::format("sub_{}_{:x}", s.index, start);
            f.segment = s.index;
            f.offset = start;
            f.code.assign(segment.begin() + start, segment.begin() + end);
            for (const std::size_t at : places)
                if (at >= start && at < end) f.fixups.insert(at - start);
            if (const auto it = called.find(start); it != called.end()) f.called = it->second;
            out.push_back(std::move(f));
        }
    }
    return out;
}

namespace {

// The batch compiler as the one-text `Compile` gives it.
CompileMany batched(const Compile& one) {
    if (!one) return {};
    return [one](std::span<const std::string> texts, std::string_view flags) {
        std::vector<std::optional<Bytes>> out;
        out.reserve(texts.size());
        for (const std::string& t : texts) out.push_back(one(t, flags));
        return out;
    };
}

}  // namespace

std::set<std::size_t> relocated_places(const formats::ne::Module& module, const formats::ne::Segment& segment, ByteView code) {
    std::set<std::size_t> out;
    for (const formats::ne::Relocation& r : module.relocations(segment)) {
        std::size_t width = 2;
        switch (r.source) {
            case 0: width = 1; break;
            case 3: case 13: width = 4; break;
            case 11: width = 6; break;
            default: break;
        }
        std::size_t at = r.offset;
        for (int seen = 0; at < code.size() && seen < 0x4000; ++seen) {   // the places are chained through the code
            const std::uint32_t next = at + 2 <= code.size() ? u16(code, at) : 0xFFFFu;
            for (std::size_t k = 0; k < width && at + k < code.size(); ++k) out.insert(at + k);
            if (r.additive() || next == 0xFFFF) break;
            at = next;
        }
    }
    return out;
}

std::map<std::string, Reading> run(const std::vector<Function>& functions, const Compile& msc6, const Compile& msvc152, std::string_view flags,
                                   const std::function<void(const std::string&)>& log) {
    return run_batched(functions, batched(msc6), batched(msvc152), flags, log);
}

std::map<std::string, Reading> run_batched(const std::vector<Function>& functions, const CompileMany& msc6, const CompileMany& msvc152, std::string_view flags,
                                           const std::function<void(const std::string&)>& log) {
    std::map<std::string, Reading> out;
    std::map<Compiler, std::vector<const Function*>> by_compiler;
    for (const Function& f : functions) {
        Reading r = read_function(f);
        const bool compilable = !r.tries.empty() && r.verdict != Verdict::Assembly;
        if (compilable) by_compiler[compiler_of(f.code)].push_back(&f);
        out[f.name] = std::move(r);
    }
    for (auto& [which, group] : by_compiler) {
        const CompileMany& compile = which == Compiler::Msvc152 ? msvc152 : msc6;
        const char* name = which == Compiler::Msvc152 ? "msvc152" : "msc6";
        if (!compile) {
            if (log) log(std::format("  {} functions are {}'s; it is not set up (settings.json: {})", group.size(), name, name));
            continue;
        }
        std::vector<std::string_view> sets;
        if (!flags.empty()) sets.push_back(flags);
        else if (which == Compiler::Msvc152) sets.assign(std::begin(kFlagsMsvc152), std::end(kFlagsMsvc152));
        else sets.assign(std::begin(kFlagsMsc6), std::end(kFlagsMsc6));
        std::vector<const Function*> left = group;
        for (const std::string_view fl : sets) {
            std::vector<std::string> texts;   // every reading of every function still left, one list
            for (const Function* f : left)
                for (const std::string& t : out[f->name].tries) texts.push_back(t);
            const std::vector<std::optional<Bytes>> built = compile(texts, fl);
            std::vector<const Function*> still;
            std::size_t next = 0;
            for (const Function* f : left) {
                Reading& r = out[f->name];
                bool found = false;
                for (const std::string& text : r.tries) {
                    const std::optional<Bytes>& got = next < built.size() ? built[next] : std::optional<Bytes>();
                    ++next;
                    if (!found && got && same_code(*got, f->code, f->fixups)) {
                        r.verdict = Verdict::Identical;
                        r.text = text;
                        r.flags = std::string(fl);
                        found = true;
                    }
                }
                if (!found) still.push_back(f);
            }
            left = std::move(still);
            if (log) log(std::format("  {:<8} {:<20} {} of {} now identical", name, fl, group.size() - left.size(), group.size()));
            if (left.empty()) break;
        }
    }
    return out;
}

}  // namespace cb::invert::module_pass
