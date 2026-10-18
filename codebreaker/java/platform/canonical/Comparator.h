#ifndef COMPARATOR_H
#define COMPARATOR_H
#include "JavaRT.h"
// java.util.Comparator. Java's compare(Object, Object) lowers to compare(JObject, JObject);
// JObject converts to/from any T* and void*, so void*-based call sites still bind.
// A `class` (not struct) so it matches the emitter's `class Comparator;` forward decls (no C4099).
class Comparator {
public:
	virtual ~Comparator() {}
	virtual int compare(JObject, JObject) = 0;
};
#endif // COMPARATOR_H
