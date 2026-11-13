#ifndef JITERATOR_H
#define JITERATOR_H

// java.util.Iterator over any container. Its elements are JObject, so iterating a JList<JObject>
// yields the boxed value (a plain void* would drop the kind/int/String tag): next() returns a JObject
// that converts back to the element -- a pointer via operator T*, a boxed primitive via
// .intValue()/operator int, a String via operator JavaString -- exactly as Java casts Iterator.next().
//
// Only FORWARD-DECLARED here: Iterator stores JObject, but JObject (JavaRT.h) is built on JList/JMap,
// which include THIS header -- a cycle. So the full definition + j2xIter/j2xValues/j2xKeys live in
// JavaRT.h AFTER JObject. JList/JSet/JMap only need `Iterator*` (a pointer -> forward decl suffices) and
// call j2xIter in inline bodies that are instantiated at their .cpp use sites, where JObject is complete.
struct JObject;
struct Iterator;
template<class C> Iterator* j2xIter(const C& c);
template<class M> Iterator* j2xValues(const M& m);
template<class M> Iterator* j2xKeys(const M& m);
#endif // JITERATOR_H
