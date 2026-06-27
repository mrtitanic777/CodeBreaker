#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from MCHashTable.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "MCHashTable.h"
#include "HashEntry.h"

MCHashTable::MCHashTable()
	: count(0)
	, threshold(0)
	, growFactor(0)
	, versionStamp(0)
{
	this->slots = JArray<HashEntry* >(16);
	this->threshold = 12;
	this->growFactor = 12.0F / 16.0F;
}

MCHashTable::~MCHashTable()
{
}

int MCHashTable::computeHash(int var0)
{
	// Java >>> is logical (unsigned) right shift; emulate with an unsigned cast.
	var0 ^= (int)((unsigned int)var0 >> 20) ^ (int)((unsigned int)var0 >> 12);
	return var0 ^ (int)((unsigned int)var0 >> 7) ^ (int)((unsigned int)var0 >> 4);
}

int MCHashTable::getSlotIndex(int var0, int var1)
{
	return var0 & (var1 - 1);
}

void* MCHashTable::lookup(int var1)
{
	int var2 = computeHash(var1);
	for(HashEntry* var3 = this->slots[getSlotIndex(var2, this->slots.length)]; var3 != NULL; var3 = var3->nextEntry) {
	if(var3->hashEntry == var1) {
	return var3->valueEntry;
	}
	}
	return NULL;
}

void MCHashTable::addKey(int var1, void* var2)
{
	int var3 = computeHash(var1);
	int var4 = getSlotIndex(var3, this->slots.length);
	for(HashEntry* var5 = this->slots[var4]; var5 != NULL; var5 = var5->nextEntry) {
	if(var5->hashEntry == var1) {
	var5->valueEntry = var2;
	}
	}
	++this->versionStamp;
	this->insert(var3, var1, var2, var4);
}

void MCHashTable::grow(int var1)
{
	JArray<HashEntry* > var2 = this->slots;
	int var3 = var2.length;
	if(var3 == 1073741824) {
	this->threshold = Integer::MAX_VALUE;
	} else {
	JArray<HashEntry* > var4 = JArray<HashEntry* >(var1);
	this->copyTo(var4);
	this->slots = var4;
	this->threshold = (int)((float)var1 * this->growFactor);
	}
}

void MCHashTable::copyTo(JArray<HashEntry* > var1)
{
	JArray<HashEntry* > var2 = this->slots;
	int var3 = var1.length;
	for(int var4 = 0; var4 < var2.length; ++var4) {
	HashEntry* var5 = var2[var4];
	if(var5 != NULL) {
	var2[var4] = NULL;
	HashEntry* var6;
	do {
	var6 = var5->nextEntry;
	int var7 = getSlotIndex(var5->slotHash, var3);
	var5->nextEntry = var1[var7];
	var1[var7] = var5;
	var5 = var6;
	} while(var6 != NULL);
	}
	}
}

void* MCHashTable::removeObject(int var1)
{
	HashEntry* var2 = this->removeEntry(var1);
	return var2 == NULL ? NULL : var2->valueEntry;
}

HashEntry* MCHashTable::removeEntry(int var1)
{
	int var2 = computeHash(var1);
	int var3 = getSlotIndex(var2, this->slots.length);
	HashEntry* var4 = this->slots[var3];
	HashEntry* var5;
	HashEntry* var6;
	for(var5 = var4; var5 != NULL; var5 = var6) {
	var6 = var5->nextEntry;
	if(var5->hashEntry == var1) {
	++this->versionStamp;
	--this->count;
	if(var4 == var5) {
	this->slots[var3] = var6;
	} else {
	var4->nextEntry = var6;
	}
	return var5;
	}
	var4 = var5;
	}
	return var5;
}

void MCHashTable::clearMap()
{
	++this->versionStamp;
	JArray<HashEntry* > var1 = this->slots;
	for(int var2 = 0; var2 < var1.length; ++var2) {
	var1[var2] = NULL;
	}
	this->count = 0;
}

void MCHashTable::insert(int var1, int var2, void* var3, int var4)
{
	HashEntry* var5 = this->slots[var4];
	this->slots[var4] = new HashEntry(var1, var2, var3, var5);
	if(this->count++ >= this->threshold) {
	this->grow(2 * this->slots.length);
	}
}

int MCHashTable::getHash(int var0)
{
	return computeHash(var0);
}
