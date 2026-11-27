// dotnet/decompiler/csharp/syntax/iannotatable.hpp -- annotations: extra objects a syntax node carries beside its children (the resolve
// result of an expression, the ILInstruction it came from, the variable it names ...), found again by type.
//
//     expr->add_annotation(resolve_result);                 // any il::ArenaObject: ILInstruction, ILVariable, ResolveResult ...
//     if (auto* rr = expr->annotation<ResolveResult>()) ... // the first annotation that is a ResolveResult
//     expr->remove_annotations<ILInstruction>();
//
// An annotation is an object of the thread's ILArena (il::ArenaObject), like the nodes themselves, so a node keeps raw pointers; the
// small annotation classes (ForeachAnnotation ...) derive from ArenaObject and are made with il::make. An annotation that is also an
// ICloneable is cloned when the node is (Clone), every other one is shared by the copy; the order of the annotations is kept.
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: IAnnotatable, AbstractAnnotatable, from IAnnotatable.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler). Not ported: the lock-free and locking thread safety (one
// ILArena belongs to one thread), and Annotation(Type) / RemoveAnnotations(Type) (the templates take their place).
#pragma once

#include <functional>
#include <vector>

#include "dotnet/decompiler/il/il_instruction.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

using il::ArenaObject;

// System.ICloneable: an annotation (or node) that can make a copy of itself, which the copy of the node that carries it holds.
class ICloneable {
public:
    virtual ~ICloneable() = default;
    // A new object made in the current ILArena.
    virtual ArenaObject* clone_object() const = 0;
};

class IAnnotatable {
public:
    virtual ~IAnnotatable() = default;

    // The annotations in the order they were added.
    virtual std::vector<ArenaObject*> annotations() const = 0;
    virtual void add_annotation(ArenaObject* annotation) = 0;
    // The first annotation `matches` accepts, or nullptr (annotation<T>() builds on it).
    virtual ArenaObject* find_annotation(const std::function<bool(const ArenaObject*)>& matches) const = 0;
    // Removes every annotation `matches` accepts.
    virtual void remove_annotations_where(const std::function<bool(const ArenaObject*)>& matches) = 0;
};

class AbstractAnnotatable : public IAnnotatable {
public:
    std::vector<ArenaObject*> annotations() const override { return annotations_; }
    // Throws std::invalid_argument for nullptr.
    void add_annotation(ArenaObject* annotation) override;
    ArenaObject* find_annotation(const std::function<bool(const ArenaObject*)>& matches) const override;
    void remove_annotations_where(const std::function<bool(const ArenaObject*)>& matches) override;

    // The first annotation that is a T (upstream's Annotation<T>()), or nullptr.
    template <class T>
    T* annotation() const {
        for (ArenaObject* a : annotations_)
            if (T* result = dynamic_cast<T*>(a)) return result;
        return nullptr;
    }

    // Removes every annotation that is a T (upstream's RemoveAnnotations<T>()).
    template <class T>
    void remove_annotations() {
        std::erase_if(annotations_, [](ArenaObject* a) { return dynamic_cast<T*>(a) != nullptr; });
    }

protected:
    AbstractAnnotatable() = default;
    // A copy starts with the same annotation objects; Clone then replaces the cloneable ones (clone_annotations).
    AbstractAnnotatable(const AbstractAnnotatable& other) : annotations_(other.annotations_) {}
    AbstractAnnotatable& operator=(const AbstractAnnotatable&) = delete;

    // Replaces each annotation that is an ICloneable by a clone of it (upstream's CloneAnnotations).
    void clone_annotations();

private:
    std::vector<ArenaObject*> annotations_;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
