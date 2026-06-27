#ifndef COLLECTIONS_H
#define COLLECTIONS_H
#include <algorithm>
#include "JavaRT.h"
#include "JList.h"
#include "Comparator.h"
// java.util.Collections. sort(List, Comparator) adapts Java's 3-way compare to a strict-weak
// less-than for std::sort. NOTE: std::sort is NOT stable (Collections.sort is guaranteed stable),
// so elements that compare equal may reorder. Comparator::compare takes JObject; the list element
// type converts to JObject implicitly (void* / typed pointers / boxed values all do).
namespace _j2x_coll {
	struct _CmpLess {
		Comparator* c;
		_CmpLess(Comparator* cc) : c(cc) {}
		template<class T> bool operator()(const T& a, const T& b) const { return c->compare(a, b) < 0; }
	};
}
struct Collections {
	template<class T> static void sort(JList<T>& list, Comparator* cmp) {
		std::sort(list.begin(), list.end(), _j2x_coll::_CmpLess(cmp));
	}
	template<class T> static void sort(JList<T>& list) { std::sort(list.begin(), list.end()); }
	template<class T> static void reverse(JList<T>& list) { std::reverse(list.begin(), list.end()); }
	template<class T> static void shuffle(JList<T>&) {}            // no-op (no RNG dependency here)
	template<class T> static void shuffle(JList<T>&, Random*) {}
};
#endif // COLLECTIONS_H
