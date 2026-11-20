// java/decompiler/generics.cpp -- see generics.hpp. Ported from Fernflower (de.fernflower.struct.gen.generics), Copyright 2000-2015 JetBrains s.r.o.,
// Apache License 2.0.
#include "java/decompiler/generics.hpp"

#include <stdexcept>

#include "java/decompiler/code_constants.hpp"
#include "java/decompiler/var_type.hpp"

namespace cb::java::decompiler {

namespace {

// Java's String.substring: out of range when the indexes do not fit (the signatures' readers rely on it to fail on a malformed one).
std::string gn_sub(const std::string& s, std::size_t begin, std::size_t end) {
    if (begin > end || end > s.size()) throw std::out_of_range("signature index");
    return s.substr(begin, end - begin);
}
std::string gn_sub(const std::string& s, std::size_t begin) { return gn_sub(s, begin, s.size()); }

std::string gn_next_class_signature(const std::string& value) {
    int counter = 0;
    std::size_t index = 0;
    for (; index < value.size(); ++index) {
        const char c = value[index];
        if (c == '<') ++counter;
        else if (c == '>') --counter;
        else if (c == '.' && counter == 0) break;
    }
    return gn_sub(value, 0, index);
}

void gn_parse_arguments(const std::optional<std::string>& list, GenericType& type) {
    if (!list) return;
    std::string value = *list;
    while (!value.empty()) {
        std::string type_str = GenericType::next_type(value);
        const std::size_t len = type_str.size();
        int wildcard = GenericType::WILDCARD_NO;
        switch (type_str.at(0)) {
            case '*': wildcard = GenericType::WILDCARD_UNBOUND; break;
            case '+': wildcard = GenericType::WILDCARD_EXTENDS; break;
            case '-': wildcard = GenericType::WILDCARD_SUPER; break;
            default: break;
        }
        type.wildcards.push_back(wildcard);
        if (wildcard != GenericType::WILDCARD_NO) type_str = gn_sub(type_str, 1);
        type.arguments.push_back(type_str.empty() ? nullptr : std::make_shared<GenericType>(GenericType::parse(type_str)));
        value = gn_sub(value, len);
    }
}

std::string gn_parse_formal_parameters(std::string signature, std::vector<std::string>& parameters, std::vector<std::vector<GenericType>>& bounds) {
    if (signature.at(0) != '<') return signature;
    int counter = 1;
    std::size_t index = 1;
    for (; index < signature.size(); ++index) {
        const char c = signature[index];
        if (c == '<') {
            ++counter;
        } else if (c == '>') {
            if (--counter == 0) break;
        }
    }
    std::string value = gn_sub(signature, 1, index);
    signature = gn_sub(signature, index + 1);
    while (!value.empty()) {
        const std::size_t to = value.find(':');
        if (to == std::string::npos) throw std::out_of_range("signature: no ':' after a type parameter");
        const std::string param = gn_sub(value, 0, to);
        value = gn_sub(value, to + 1);
        std::vector<GenericType> list;
        for (;;) {
            if (value.at(0) == ':') value = gn_sub(value, 1);   // (an empty class bound)
            const std::string bound = GenericType::next_type(value);
            list.push_back(GenericType::parse(bound));
            value = gn_sub(value, bound.size());
            if (value.empty() || value[0] != ':') break;
            value = gn_sub(value, 1);
        }
        parameters.push_back(param);
        bounds.push_back(std::move(list));
    }
    return signature;
}

constexpr const char* kGnTypeNames[] = {"byte", "char", "double", "float", "int", "long", "short", "boolean"};

std::string gn_java_class_name(const GenericType& type, const generics::NameContext& names) {
    std::string name;
    for (const GenericType& tp : type.enclosing_classes) name += tp.value.value_or("null") + "$";
    name += type.value.value_or("null");
    std::string res = name;
    for (char& c : res)
        if (c == '/') c = '.';
    if (res.find('$') != std::string::npos && !(names.is_own_class && names.is_own_class(name)))
        for (char& c : res)
            if (c == '$') c = '.';
    return res;
}

std::string gn_type_name(const GenericType& type, const generics::NameContext& names) {
    const int tp = type.type;
    if (tp >= 0 && tp <= cc::TYPE_BOOLEAN) return kGnTypeNames[tp];
    if (tp == cc::TYPE_VOID) return "void";
    if (tp == cc::TYPE_GENVAR) return type.value.value_or("null");
    if (tp == cc::TYPE_OBJECT) {
        const std::string java_name = gn_java_class_name(type, names);
        std::string buffer = names.short_name ? names.short_name(java_name) : java_name;
        if (!type.arguments.empty()) {
            buffer += '<';
            for (std::size_t i = 0; i < type.arguments.size(); ++i) {
                if (i > 0) buffer += ", ";
                const int wildcard = type.wildcards.at(i);
                if (wildcard != GenericType::WILDCARD_NO) {
                    buffer += '?';
                    if (wildcard == GenericType::WILDCARD_EXTENDS) buffer += " extends ";
                    else if (wildcard == GenericType::WILDCARD_SUPER) buffer += " super ";
                }
                if (type.arguments[i]) buffer += generics::cast_type_name(*type.arguments[i], names);
            }
            buffer += '>';
        }
        return buffer;
    }
    throw std::runtime_error("Invalid type: " + type.value.value_or("null"));
}

}  // namespace

GenericType GenericType::parse(std::string signature) {
    GenericType t;
    std::size_t index = 0;
    while (index < signature.size()) {
        const char c = signature[index];
        if (c == '[') {
            ++t.array_dim;
        } else if (c == 'T') {
            t.type = cc::TYPE_GENVAR;
            t.value = gn_sub(signature, index + 1, signature.size() - 1);
            break;
        } else if (c == 'L') {
            t.type = cc::TYPE_OBJECT;
            signature = gn_sub(signature, index + 1, signature.size() - 1);
            for (;;) {
                const std::string cl = gn_next_class_signature(signature);
                std::string name = cl;
                std::optional<std::string> args;
                if (const std::size_t arg_start = cl.find('<'); arg_start != std::string::npos) {
                    name = gn_sub(cl, 0, arg_start);
                    args = gn_sub(cl, arg_start + 1, cl.size() - 1);
                }
                if (cl.size() < signature.size()) {
                    signature = gn_sub(signature, cl.size() + 1);   // (past the '.')
                    GenericType outer(cc::TYPE_OBJECT, 0, name);
                    gn_parse_arguments(args, outer);
                    t.enclosing_classes.push_back(std::move(outer));
                } else {
                    t.value = name;
                    gn_parse_arguments(args, t);
                    break;
                }
            }
            break;
        } else {
            t.value = gn_sub(signature, index, index + 1);
            t.type = VarType::type_of(c);
        }
        ++index;
    }
    return t;
}

std::string GenericType::next_type(const std::string& value) {
    int counter = 0;
    std::size_t index = 0;
    bool cont_mode = false;
    for (; index < value.size(); ++index) {
        const char c = value[index];
        bool stop = false;
        switch (c) {
            case '*':
                if (!cont_mode) stop = true;
                break;
            case 'L':
            case 'T':
                cont_mode = true;
                break;
            case '[':
            case '+':
            case '-': break;
            case '<': ++counter; break;
            case '>': --counter; break;
            case ';':
                if (counter == 0) stop = true;
                break;
            default:
                if (!cont_mode) stop = true;
                break;
        }
        if (stop) break;
    }
    return gn_sub(value, 0, index + 1);
}

namespace generics {

std::optional<GenericClassDescriptor> parse_class_signature(const std::string& original) {
    try {
        GenericClassDescriptor d;
        std::string signature = gn_parse_formal_parameters(original, d.fparameters, d.fbounds);
        const std::string super_class = GenericType::next_type(signature);
        d.superclass = GenericType::parse(super_class);
        signature = gn_sub(signature, super_class.size());
        while (!signature.empty()) {
            const std::string super_if = GenericType::next_type(signature);
            d.superinterfaces.push_back(GenericType::parse(super_if));
            signature = gn_sub(signature, super_if.size());
        }
        return d;
    } catch (const std::exception&) {
        return std::nullopt;
    }
}

std::optional<GenericFieldDescriptor> parse_field_signature(const std::string& signature) {
    try {
        return GenericFieldDescriptor{GenericType::parse(signature)};
    } catch (const std::exception&) {
        return std::nullopt;
    }
}

std::optional<GenericMethodDescriptor> parse_method_signature(const std::string& original) {
    try {
        GenericMethodDescriptor d;
        std::string signature = gn_parse_formal_parameters(original, d.fparameters, d.fbounds);
        const std::size_t to = signature.find(')');
        if (to == std::string::npos) throw std::out_of_range("signature: no ')'");
        std::string pars = gn_sub(signature, 1, to);
        signature = gn_sub(signature, to + 1);
        while (!pars.empty()) {
            const std::string par = GenericType::next_type(pars);
            d.params.push_back(GenericType::parse(par));
            pars = gn_sub(pars, par.size());
        }
        const std::string ret = GenericType::next_type(signature);
        d.ret = GenericType::parse(ret);
        signature = gn_sub(signature, ret.size());
        if (!signature.empty()) {   // "^E1^E2": split on '^', the first piece (before the first '^') skipped
            std::vector<std::string> pieces;
            std::size_t from = 0;
            for (std::size_t at; (at = signature.find('^', from)) != std::string::npos; from = at + 1) pieces.push_back(signature.substr(from, at - from));
            pieces.push_back(signature.substr(from));
            while (!pieces.empty() && pieces.back().empty()) pieces.pop_back();   // (Java's split drops trailing empty pieces)
            for (std::size_t i = 1; i < pieces.size(); ++i) d.exceptions.push_back(GenericType::parse(pieces[i]));
        }
        return d;
    } catch (const std::exception&) {
        return std::nullopt;
    }
}

std::string cast_type_name(const GenericType& type, const NameContext& names) {
    std::string s = gn_type_name(type, names);
    for (int dim = type.array_dim; dim > 0; --dim) s += "[]";
    return s;
}

}  // namespace generics

}  // namespace cb::java::decompiler
