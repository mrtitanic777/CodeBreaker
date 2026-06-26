#ifndef NOISEGENERATOR2_H
#define NOISEGENERATOR2_H

// Auto-generated from NoiseGenerator2.java
// Target: Xbox 360 / C++03 / DirectX 9

class Random;

class NoiseGenerator2 {
public:
	NoiseGenerator2();
	NoiseGenerator2(Random* var1);
	virtual ~NoiseGenerator2();

	double m_field_4292_a;
	double m_field_4291_b;
	double m_field_4297_c;

	virtual void func_4157_a(double* var1, double var2, double var4, int var6, int var7, double var8, double var10, double var12);

public: // was private
	static int** s_field_4296_d;
	int* m_field_4295_e;
	static double s_field_4294_f;
	static double s_field_4293_g;

	int func_4155_a(double var0);
	double func_4156_a(int* var0, double var1, double var3);

public:  // array-length companions
	int m_field_4295_e_count;

};

#endif // NOISEGENERATOR2_H