#ifndef COLLECTIONS_H
#define COLLECTIONS_H
#include <algorithm>
#include "JavaRT.h"
#include "JList.h"
#include "Comparator.h"
#include <vector>
#include <cstddef>
class Random;                                            // shuffle(list, Random) -- fwd-decl (no RNG dep here)
// java.util.Collections.
//
// sort(List, Comparator): Java's Collections.sort is a STABLE merge sort that tolerates
// comparators which are NOT a strict weak ordering -- e.g. RenderSorter returns compare(a,a) == -1
// (faithful to the Java source). std::sort REQUIRES a strict weak ordering; feeding it such a
// comparator is undefined behaviour and on the Xbox 360 STL reads out of bounds -> heap corruption
// / IP 0x0 hang. It fires exactly at world entry, where the whole dirty-chunk queue (up to a few
// thousand WorldRenderers) is sorted EVERY frame. So we do NOT use std::sort here: a bottom-up
// stable merge sort that only ever asks compare(a,b) < 0 -- and only within a run -- is memory-safe
// for ANY 3-way Java comparator by construction, and matches Java's stable ordering. Comparator::
// compare takes JObject; the list element type converts to JObject implicitly (void* / typed
// pointers / boxed values all do).
namespace _j2x_coll {
	struct _CmpLess {
		Comparator* c;
		_CmpLess(Comparator* cc) : c(cc) {}
		template<class T> bool operator()(const T& a, const T& b) const { return c->compare(a, b) < 0; }
	};
	// Bottom-up STABLE merge sort over contiguous [a, a+n). Uses less(x,y) only as a predicate,
	// only within run bounds -> never reads out of range no matter what compare() returns.
	// Stable: on equal elements the earlier (left-run) element is emitted first.
	template<class T, class Less>
	void _stable_merge_sort(T* a, std::size_t n, Less less) {
		if (n < 2) { return; }
		std::vector<T> tmp(n);
		for (std::size_t width = 1; width < n; width <<= 1) {
			for (std::size_t lo = 0; lo < n; lo += (width << 1)) {
				std::size_t mid = lo + width;          if (mid > n) { mid = n; }
				std::size_t hi  = lo + (width << 1);   if (hi  > n) { hi  = n; }
				std::size_t i = lo, j = mid, k = lo;
				while (i < mid && j < hi) {
					tmp[k++] = less(a[j], a[i]) ? a[j++] : a[i++];   // take right only if strictly less -> stable
				}
				while (i < mid) { tmp[k++] = a[i++]; }
				while (j < hi)  { tmp[k++] = a[j++]; }
			}
			for (std::size_t x = 0; x < n; ++x) { a[x] = tmp[x]; }
		}
	}
}
struct Collections {
	template<class T> static void sort(JList<T>& list, Comparator* cmp) {
		if (list.size() < 2) { return; }
		_j2x_coll::_stable_merge_sort(&list[0], (std::size_t)list.size(), _j2x_coll::_CmpLess(cmp));
	}
	template<class T> static void sort(JList<T>& list) { std::sort(list.begin(), list.end()); }
	template<class T> static void reverse(JList<T>& list) { std::reverse(list.begin(), list.end()); }
	template<class T> static JList<T>& synchronizedList(JList<T>& list) { return list; }   // no JVM monitor on the port
	template<class M> static M& synchronizedMap(M& m) { return m; }
	template<class T> static JList<T> emptyList() { return JList<T>(); }
	template<class T> static void shuffle(JList<T>&) {}            // no-op (no RNG dependency here)
	template<class T> static void shuffle(JList<T>&, Random*) {}
};
#endif // COLLECTIONS_H
