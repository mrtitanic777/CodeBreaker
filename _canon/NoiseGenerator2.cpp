#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from NoiseGenerator2.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NoiseGenerator2.h"
#include "Random.h"

JArray<JArray<int > > NoiseGenerator2::s_grad = (JArrayInit<JArray<int > >(),
	(JArrayInit<int >(), 1, 1, 0), (JArrayInit<int >(), -1, 1, 0), (JArrayInit<int >(), 1, -1, 0), (JArrayInit<int >(), -1, -1, 0),
	(JArrayInit<int >(), 1, 0, 1), (JArrayInit<int >(), -1, 0, 1), (JArrayInit<int >(), 1, 0, -1), (JArrayInit<int >(), -1, 0, -1),
	(JArrayInit<int >(), 0, 1, 1), (JArrayInit<int >(), 0, -1, 1), (JArrayInit<int >(), 0, 1, -1), (JArrayInit<int >(), 0, -1, -1));
double NoiseGenerator2::s_F2 = 0.5 * (sqrt(3.0) - 1.0);
double NoiseGenerator2::s_G2 = (3.0 - sqrt(3.0)) / 6.0;

NoiseGenerator2::NoiseGenerator2()
	: xOffset(0)
	, yOffset(0)
	, zOffset(0)
{
	// inlined this(new Random()) -- C++03 lacks ctor delegation.
	// Use ONE Random instance (matches Java's single `new Random()` argument).
	Random* var1 = new Random();
	this->permutations = JArray<int >(512);
	this->xOffset = var1->nextDouble() * 256.0;
	this->yOffset = var1->nextDouble() * 256.0;
	this->zOffset = var1->nextDouble() * 256.0;
	int var2;
	for(var2 = 0; var2 < 256; this->permutations[var2] = var2++) {
	}
	for(var2 = 0; var2 < 256; ++var2) {
	int var3 = var1->nextInt(256 - var2) + var2;
	int var4 = this->permutations[var2];
	this->permutations[var2] = this->permutations[var3];
	this->permutations[var3] = var4;
	this->permutations[var2 + 256] = this->permutations[var2];
	}
}

NoiseGenerator2::NoiseGenerator2(Random* var1)
	: xOffset(0)
	, yOffset(0)
	, zOffset(0)
{
	this->permutations = JArray<int >(512);
	this->xOffset = var1->nextDouble() * 256.0;
	this->yOffset = var1->nextDouble() * 256.0;
	this->zOffset = var1->nextDouble() * 256.0;
	int var2;
	for(var2 = 0; var2 < 256; this->permutations[var2] = var2++) {
	}
	for(var2 = 0; var2 < 256; ++var2) {
	int var3 = var1->nextInt(256 - var2) + var2;
	int var4 = this->permutations[var2];
	this->permutations[var2] = this->permutations[var3];
	this->permutations[var3] = var4;
	this->permutations[var2 + 256] = this->permutations[var2];
	}
}

NoiseGenerator2::~NoiseGenerator2()
{
}

int NoiseGenerator2::fastFloor(double var0)
{
	return var0 > 0.0 ? (int)var0 : (int)var0 - 1;
}

double NoiseGenerator2::dot(JArray<int > var0, double var1, double var3)
{
	return (double)var0[0] * var1 + (double)var0[1] * var3;
}

void NoiseGenerator2::generateNoise(JArray<double > var1, double var2, double var4, int var6, int var7, double var8, double var10, double var12)
{
	int var14 = 0;
	for(int var15 = 0; var15 < var6; ++var15) {
	double var16 = (var2 + (double)var15) * var8 + this->xOffset;
	for(int var18 = 0; var18 < var7; ++var18) {
	double var19 = (var4 + (double)var18) * var10 + this->yOffset;
	double var27 = (var16 + var19) * s_F2;
	int var29 = fastFloor(var16 + var27);
	int var30 = fastFloor(var19 + var27);
	double var31 = (double)(var29 + var30) * s_G2;
	double var33 = (double)var29 - var31;
	double var35 = (double)var30 - var31;
	double var37 = var16 - var33;
	double var39 = var19 - var35;
	unsigned char var41;
	unsigned char var42;
	if(var37 > var39) {
	var41 = 1;
	var42 = 0;
	} else {
	var41 = 0;
	var42 = 1;
	}
	double var43 = var37 - (double)var41 + s_G2;
	double var45 = var39 - (double)var42 + s_G2;
	double var47 = var37 - 1.0 + 2.0 * s_G2;
	double var49 = var39 - 1.0 + 2.0 * s_G2;
	int var51 = var29 & 255;
	int var52 = var30 & 255;
	int var53 = this->permutations[var51 + this->permutations[var52]] % 12;
	int var54 = this->permutations[var51 + var41 + this->permutations[var52 + var42]] % 12;
	int var55 = this->permutations[var51 + 1 + this->permutations[var52 + 1]] % 12;
	double var56 = 0.5 - var37 * var37 - var39 * var39;
	double var21;
	if(var56 < 0.0) {
	var21 = 0.0;
	} else {
	var56 *= var56;
	var21 = var56 * var56 * dot(s_grad[var53], var37, var39);
	}
	double var58 = 0.5 - var43 * var43 - var45 * var45;
	double var23;
	if(var58 < 0.0) {
	var23 = 0.0;
	} else {
	var58 *= var58;
	var23 = var58 * var58 * dot(s_grad[var54], var43, var45);
	}
	double var60 = 0.5 - var47 * var47 - var49 * var49;
	double var25;
	if(var60 < 0.0) {
	var25 = 0.0;
	} else {
	var60 *= var60;
	var25 = var60 * var60 * dot(s_grad[var55], var47, var49);
	}
	int var10001 = var14++;
	var1[var10001] += 70.0 * (var21 + var23 + var25) * var12;
	}
	}
}
