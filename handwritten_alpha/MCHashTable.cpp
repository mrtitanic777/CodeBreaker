#include "stdafx.h"
// Hand-ported from MCHashTable.java (Minecraft Alpha v1.2.6)
// Target: Xbox 360 / C++03 / DirectX 9
//
// Mojang's custom int-keyed hash map (used by the chunk provider / pathfinder /
// WorldClient). Faithful bucket-array port of the Java original.
//
// PORT NOTE: the auto-generated MCHashTable.h dropped the transient fields
// (slots, count, versionStamp) during conversion -- it only declares
// m_threshold and m_growFactor. We are NOT allowed to touch the header (callers
// link against it as-is), so the per-instance bucket state is kept in a
// file-local side-table keyed by the instance pointer. MCHashTable is always
// heap-allocated via `new MCHashTable()` (Pathfinder, WorldClient), so the
// `this` address is stable for the object's whole lifetime -- safe as a key.

#include "MCHashTable.h"
#include "Common.h"
#include "HashEntry.h"
#include <map>

// ----------------------------------------------------------------------------
// Per-instance internal state (the dropped transient fields live here).
// ----------------------------------------------------------------------------
namespace {
	struct MCHashTableImpl {
		HashEntry** slots;       // bucket array
		int         slots_count; // == Java slots.length (power of two)
		int         count;       // number of live entries
		int         versionStamp;
		int         growNewLen;  // length of the new array during a grow()/copyTo()

		MCHashTableImpl()
			: slots(NULL)
			, slots_count(16)
			, count(0)
			, versionStamp(0)
			, growNewLen(16)
		{
			slots = new HashEntry*[16];
			for (int i = 0; i < 16; ++i) {
				slots[i] = NULL;
			}
		}

		~MCHashTableImpl() {
			if (slots != NULL) {
				for (int i = 0; i < slots_count; ++i) {
					HashEntry* e = slots[i];
					while (e != NULL) {
						HashEntry* next = e->m_nextEntry;
						delete e;
						e = next;
					}
				}
				delete[] slots;
				slots = NULL;
			}
		}
	};

	// Side-table mapping each MCHashTable instance to its hidden bucket state.
	std::map<const MCHashTable*, MCHashTableImpl*> g_impls;

	MCHashTableImpl* implFor(const MCHashTable* self) {
		std::map<const MCHashTable*, MCHashTableImpl*>::iterator it = g_impls.find(self);
		if (it != g_impls.end()) {
			return it->second;
		}
		// Defensive: should never happen (ctor inserts), but never deref NULL.
		MCHashTableImpl* impl = new MCHashTableImpl();
		g_impls[self] = impl;
		return impl;
	}
}

// ----------------------------------------------------------------------------
// Construction / destruction
// ----------------------------------------------------------------------------
MCHashTable::MCHashTable()
	: m_threshold(12)
	, m_growFactor(12.0f / 16.0f)
{
	g_impls[this] = new MCHashTableImpl();
}

MCHashTable::~MCHashTable() {
	std::map<const MCHashTable*, MCHashTableImpl*>::iterator it = g_impls.find(this);
	if (it != g_impls.end()) {
		delete it->second;
		g_impls.erase(it);
	}
}

// ----------------------------------------------------------------------------
// Static-style helpers (Java: private static)
// ----------------------------------------------------------------------------
int MCHashTable::computeHash(int var0) {
	var0 ^= (int)((unsigned int)var0 >> 20) ^ (int)((unsigned int)var0 >> 12);
	return var0 ^ (int)((unsigned int)var0 >> 7) ^ (int)((unsigned int)var0 >> 4);
}

int MCHashTable::getSlotIndex(int var0, int var1) {
	return var0 & (var1 - 1);
}

// ----------------------------------------------------------------------------
// Public API
// ----------------------------------------------------------------------------
void* /* PORT: was Object */ MCHashTable::lookup(int var1) {
	MCHashTableImpl* impl = implFor(this);
	int var2 = computeHash(var1);

	for (HashEntry* var3 = impl->slots[getSlotIndex(var2, impl->slots_count)]; var3 != NULL; var3 = var3->m_nextEntry) {
		if (var3->m_hashEntry == var1) {
			return var3->m_valueEntry;
		}
	}

	return NULL;
}

void MCHashTable::addKey(int var1, void* /* PORT: was Object */ var2) {
	MCHashTableImpl* impl = implFor(this);
	int var3 = computeHash(var1);
	int var4 = getSlotIndex(var3, impl->slots_count);

	for (HashEntry* var5 = impl->slots[var4]; var5 != NULL; var5 = var5->m_nextEntry) {
		if (var5->m_hashEntry == var1) {
			var5->m_valueEntry = var2;
		}
	}

	++impl->versionStamp;
	insert(var3, var1, var2, var4);
}

void MCHashTable::grow(int var1) {
	MCHashTableImpl* impl = implFor(this);
	int var3 = impl->slots_count;
	if (var3 == 1073741824) {
		m_threshold = 2147483647;
	} else {
		HashEntry** var4 = new HashEntry*[var1];
		for (int i = 0; i < var1; ++i) {
			var4[i] = NULL;
		}
		impl->growNewLen = var1; // copyTo() rehashes against the new capacity
		copyTo((HashEntry*)var4);  // rehash existing entries into the new array
		delete[] impl->slots;  // old buckets are now empty (entries were moved)
		impl->slots = var4;
		impl->slots_count = var1;
		m_threshold = (int)((float)var1 * m_growFactor);
	}
}

void MCHashTable::copyTo(HashEntry* var1) {
	// PORT: the header declares copyTo(HashEntry*) but it semantically takes the
	// new bucket ARRAY (HashEntry**). The pointer reinterpret keeps the declared
	// signature so callers/vtable stay byte-identical to the header.
	HashEntry** newSlots = (HashEntry**)var1;
	MCHashTableImpl* impl = implFor(this);
	HashEntry** var2 = impl->slots;
	int newLen = impl->growNewLen; // Java: var1.length (the new array's capacity)

	for (int var4 = 0; var4 < impl->slots_count; ++var4) {
		HashEntry* var5 = var2[var4];
		if (var5 != NULL) {
			var2[var4] = NULL;

			HashEntry* var6 = NULL;
			do {
				var6 = var5->m_nextEntry;
				int var7 = getSlotIndex(var5->m_slotHash, newLen);
				var5->m_nextEntry = newSlots[var7];
				newSlots[var7] = var5;
				var5 = var6;
			} while (var6 != NULL);
		}
	}
}

void* /* PORT: was Object */ MCHashTable::removeObject(int var1) {
	HashEntry* var2 = removeEntry(var1);
	return var2 == NULL ? NULL : var2->m_valueEntry;
}

HashEntry* MCHashTable::removeEntry(int var1) {
	MCHashTableImpl* impl = implFor(this);
	int var2 = computeHash(var1);
	int var3 = getSlotIndex(var2, impl->slots_count);
	HashEntry* var4 = impl->slots[var3];

	HashEntry* var5 = NULL;
	HashEntry* var6 = NULL;
	for (var5 = var4; var5 != NULL; var5 = var6) {
		var6 = var5->m_nextEntry;
		if (var5->m_hashEntry == var1) {
			++impl->versionStamp;
			--impl->count;
			if (var4 == var5) {
				impl->slots[var3] = var6;
			} else {
				var4->m_nextEntry = var6;
			}

			return var5;
		}

		var4 = var5;
	}

	return var5;
}

void MCHashTable::clearMap() {
	MCHashTableImpl* impl = implFor(this);
	++impl->versionStamp;

	for (int var2 = 0; var2 < impl->slots_count; ++var2) {
		// Free the chain before dropping the bucket (Java relied on GC).
		HashEntry* e = impl->slots[var2];
		while (e != NULL) {
			HashEntry* next = e->m_nextEntry;
			delete e;
			e = next;
		}
		impl->slots[var2] = NULL;
	}

	impl->count = 0;
}

void MCHashTable::insert(int var1, int var2, void* /* PORT: was Object */ var3, int var4) {
	MCHashTableImpl* impl = implFor(this);
	HashEntry* var5 = impl->slots[var4];
	impl->slots[var4] = new HashEntry(var1, var2, var3, var5);
	if (impl->count++ >= m_threshold) {
		grow(2 * impl->slots_count);
	}
}

int MCHashTable::getHash(int var0) {
	return computeHash(var0);
}
